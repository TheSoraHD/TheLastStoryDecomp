# enlazar-20261115, cola P

Agente P, 2026-11-15 17:45-20:15. Unidades: chr/ControlCdnStone, chr/ControlChaseCaught, chr/ControlJumpAt.
Referencia: `/home/claude/ref-P/report.json` (15587 funciones al 100 %).

## Resultado por unidad

| Unidad | Antes | Despues | Notas |
|---|---|---|---|
| chr/ControlCdnStone | 57/57 | 57/57 | sigue sin enlazar: `.sdata2` resuelto (parche), queda el orden de dos cadenas RTTI de `ControlCdnBetray::vf10` |
| chr/ControlJumpAt | 24/28 | 26/28 | `ControlGrappled` ctor y `ControlJumpAt::vf0C` al 100 % |
| chr/ControlChaseCaught | 33/39 | 34/39 | `ControlChaseAssist::vf0C` al 100 % |

Total: 15587 -> 15590 en `report.json` (lost 0, sin bajadas).

## Unidades que enlazan (bytes)

- Ninguna. chr/ControlCdnStone (3304 B de datos) queda a un solo problema (ver Pendientes).

## NONMATCHING con motivo

- ControlChaseCaught::vf0C (99,3): r4/r5 de la direccion del temporal del `+` y de `m08` en el calculo de
  `target`. `Chara_unk* c2 = mChara;` antes de la copia de `mRot` arregla la segunda mitad (32 -> 16 lineas) pero
  sola no es avance (no se commitea). Probado: `front`/`target` locales, `catcher` local, `FrontPos_unk` inline (por
  puntero o por referencias, peor), `Approach_unk` con `c1`, el orden del `+`, constructor de copia.
- ControlChaseHeld::vf0C (99,6): el mismo cruce (temporal en r3, `m08` en r4 en el original). Probado: `c1`/`c2`,
  `VecAssign_unk`, `GetRot_unk()`, locales `off`/`front`/`pos`, `5.0f * v`, `FrontPos_unk`.
- ControlChaseFollow::vf0C (98,x): el original carga `mChara` antes del `if` (r4) y lo vuelve a leer tras las
  llamadas. Un `Chara_unk* chara = mChara;` antes del `if` usado en `m570 = leader->m570` lo reproduce (54 -> 46
  lineas) y `chara->m570` en la ultima rama baja a 45; quedan f29/f30 y r3/r4 de la copia a `m6B8` (probado
  `VecAssign_unk`, `chara->`, locales, `(-d).GetNormalize()`). Sin commitear (solo fuzzy).
- ControlGrappleThrown::vf0C (99,77): r6/r7 de `mChara` en los argumentos de `fn_8004D388` y r3/r4 en la copia de
  `result.m10`; probado `VecAssign_unk`, `pos(result.m10)`, `c0` local, orden de `c`/`pos`.
- ControlGrappled::vf10 (99,44): huecos de pila de los dos `Mtx`; probado `Mtx` primero, bloque `{}` para la
  matriz, `boost::function` local (peor).

## Cambios de splits

- Ninguno. `symbols.txt`: solo renombrados del re-emparejado (`pd.sh`) y la constante ptmf de
  `ControlJumpAt::vf10` en `.data` 0x80780BA0 (`@13391` -> `@13415`, el nombre que da nuestro .o; si no,
  labelcheck/rep100 empeoran).

## Para el port

- Nada (ninguna unidad del port). Solo `src/chr/ControlJumpAt.cpp` y `src/chr/ControlChaseCaught.cpp`; sin
  cambios de cabeceras commiteados.

## Decisiones tomadas sin el usuario

- `ControlGrappled` ctor: la prueba de `mFlag5` con el accesor ya existente `mChara->IsFlag5_unk()`.
- `ControlJumpAt::vf0C`: inline `Approach_unk(from, to, t)` (nombre inventado, el mismo que ya existia en
  `chr/ControlChaseCaught.cpp`) + `Chara_unk* c1 = mChara;` para la copia a `mPos`.
- `ControlChaseAssist::vf0C`: `Chara_unk* c1 = mChara;` + `atn::VecAssign_unk(d, leader->mPos - c1->mPos)`.
- chr/ControlCdnStone: no commiteado el arreglo de `.sdata2` (una funcion inventada que el enlazador quita);
  queda como parche `docs/enlazar-20261115-qP.patch` (incluye `matching: true`) porque sin lo de RTTI no enlaza.

## Hallazgos (una linea cada uno)

- Bitfield que el original relee antes de limpiarlo (`lwz` doble de 0x12A4): probar con el accesor inline
  (`if (c->IsFlag5_unk()) c->mFlag5 = 0;`) (ControlGrappled ctor; mismo patron anotado en CharaTask,
  Em206EggMonster, Em401_01_WhiteLeoPard, QuadrupedTask, ControlCaught).
- Temporales de un `(b - a) * t + a` con huecos de pila cruzados: pasarlo por una inline con referencias
  (`Approach_unk(a, b, t)`) cambia el orden de los temporales (ControlJumpAt::vf0C).
- Tras anadir una llamada a un accesor, la constante ptmf de `.data` renumerada no la arregla `pd.sh`: renombrarla
  a mano en `symbols.txt` con el nombre del .o (`objdump -t | grep .data`) o labelcheck/rep100 empeoran.
- Constantes de `.sdata2` al principio del pool en orden inverso al de uso (150/450 de ControlCdnPiyo): ni
  inline previa, ni `static const`, ni ternario entero las mueven; una funcion previa que el enlazador quita, si.
- Cadenas RTTI de `boost::function` en `.rodata`: cada funcion de primer nivel aporta su grupo de cadenas (en el
  orden de sus `manage` en `.text`) y los grupos salen en orden inverso de funcion; dentro de un grupo `.text` y
  cadenas van siempre en el mismo orden. Un `bind` convertido dentro de una plantilla (instanciada aparte) cae en
  otro grupo (CdnStone).
- Dentro de una funcion, los `invoke`/`manage` debiles salen en orden inverso a su aparicion en el fuente.
- Las funciones que devuelven `boost::function` nunca se inlinean (ni definidas antes, ni plantillas).

## Pendientes

- chr/ControlCdnStone (57/57, 3304 B): con el parche `docs/enlazar-20261115-qP.patch` (stub
  `ControlCdnPiyo::Stripped1_unk`, `matching: true`) el `.dol` difiere solo en `.rodata` (842 B desde la cadena
  0x807392E5) y 12 B de punteros RTTI en `.data` (0x8077D692-0x8077D9F3); `.text`, `.sdata2`, `.bss`, `.sbss`
  identicos. Falta que en `ControlCdnBetray::vf10` la cadena RTTI del bind con `value<int>` (rama `mType == 0`,
  la que cae) quede antes que la de `value<CharaTask::CONTROL_MODE>` sin cambiar el orden de `.text`
  (`invoke`/`manage` de CONTROL_MODE primero) ni el codigo (`bne` a la rama CONTROL_MODE). Segun el modelo de
  grupos de arriba, el original tuvo la conversion del bind `int` en otro grupo (como si viniera de una plantilla
  aparte) pero inlineada. Probado sin exito: `if` invertido (arregla cadenas, rompe codigo y `.text`), `else`,
  `switch`, `!(... != 0)`, local `c`, `boost::function` local con asignaciones, `CONTROL_MODE(2)` como int,
  `?:` de `boost::function`, `function0<void>` como retorno o temporal, inline previa (instancia antes del
  `vf0C`), plantilla auxiliar (no se inlinea), funcion posterior con el mismo bind.
- ControlChaseCaught (ChaseCaught/Held/Follow/Catch/Hold::vf0C) y ControlJumpAt (Grappled::vf10,
  GrappleThrown::vf0C): ver NONMATCHING.
