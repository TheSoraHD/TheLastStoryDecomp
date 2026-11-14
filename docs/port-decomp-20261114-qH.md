# port-decomp-20261114 - cola H (combate, segunda pasada)

Inicio: 2026-11-14 04:41 CET. Rama `agent-H`. Unidades: `game/BattleCharaTask`, `game/BattleTask`, `game/AiTarget`.

## Resultado por funcion

- `BattleCharaTask::fn_800E4278` (0x800E4278, 5668 bytes): **escrita, NONMATCHING 99,5 %** (antes 0 %, sin version
  funcional en el port). Firma final `void BattleCharaTask::fn_800E4278(GameTask*)` (antes `extern "C"
  fn_800E4278(PlayerTask*, GameTask*)` en GameTask.cpp). Entrada del lider: bloqueo (`Input_unk::fn_80122330`),
  objetivo mFC4, acciones de cobertura (lado `fn_80153B44`, salto/ataque desde la pared con `fn_8010089C`),
  gimmicks (`GameTask::fn_8036E098`) y rutas (`fn_8036DAF4`) a mano, enemigo agarrable (`fn_8036DDD4`: 0x2711/0x4FBF
  y el texto "hips" del popup, o items 400), pista del HUD (`GameUI::m764`, `m7C0` = 0xD / 0x11, `m728`) y la entrada
  del arma (`fn_800E62AC`/`fn_800E7650` con `GameUI::m748.m00`/`m748.m14`). Resto: r0/r3 permutados en las
  comprobaciones del objetivo, la entrada 0x11 recarga `lbl_8087F0A8` (aqui usa `config`), orden de stores del
  vector al objetivo, cargas de `m7FC`/`m800` de la camara y la recarga de la palabra de flags antes de `mFlag5 = 0`.
- `Effect_unk::fn_80109950` (0x80109950, 1580 bytes, unidad BattleTask): **escrita, NONMATCHING 97,5 %** (antes 0 %).
  Firma `atn::Vector3 Effect_unk::fn_80109950(CharaTask*)` (sin cambio; `this` no se lee). Puesto de `chara` en el
  anillo de su lider (`mC58.mC4`, `m125C` = {angulo, personaje}): busca el mas cercano (o el de tipo 0 / tipo 2
  con `m50 < 100`), reparte la vuelta completa por los anchos `vfA0` y recorre el anillo hacia un lado y el otro hasta
  `chara`; devuelve el centro mas `(0, 0, m64->m30)` girado, con la y de `chara`, o `mPos` si no esta. Resto:
  asignacion de registros (centro/anillo, punteros de los bucles, floats `best`/`offset`).

## Cambios de firma / layout (para el port)

- `BattleCharaTask::fn_800E4278(GameTask*)` nuevo miembro en `include/game/BattleCharaTask.h`; `GameTask::fn_8036CC90`
  llama `leader->fn_800E4278(this)` (antes `extern "C"`).
- `BattleCharaTask::fn_800E7650(Field_unk*, int, int)`: el original pasa `a`/`b` (r5/r6) tambien a esta funcion
  (como a `fn_800E62AC`); la funcion no los usa. Simbolo renombrado a `fn_800E7650__15BattleCharaTaskFP9Field_unkii`.
- `fn_80108C10` declarado en BattleCharaTask.cpp con `const atn::Vector3&` (como en Em203GreaterDaemon), no puntero.

## Decisiones y hallazgos

- `Effect_unk` se define como vista minima en BattleTask.cpp solo para `fn_80109950` (CharaTask.cpp y Em315Coffin
  la llaman con ese nombre mangled); pendiente unificar con `BattleTask` cuando se toquen esas unidades.
- Re-emparejado de BattleTask: `pd.sh` (pairdata --relabel) rompe ~45 funciones (rep100 +45). Con pairdata99 hay que
  iterar y aun quedan 32 etiquetas de funciones NONMATCHING: el desplazamiento de los `@N` es constante (+328, y
  +427 a partir de ~@33000), aplicado a mano por direccion (labelcheck vuelve a "missing 2" como la referencia).

- Un parametro con nombre en la cabecera consume un `@N` en todas las unidades que la incluyen: se deja el nombre
  `field` en `fn_800E7650` para no mover las etiquetas de ~40 unidades; la nueva declaracion va sin nombre.
- `@28783` -> `@28781` (0x808857A8) en symbols.txt a mano tras quitar la declaracion `extern "C"` con dos
  parametros con nombre de GameTask.cpp (pd.sh no la re-emparejo).
- precommit: `labelcheck/rep100: peor` solo por `fn_800EBD00` (report 99,97 -> 100, objdiff 99,95: la etiqueta global
  `lbl_808812D8` frente al `@N` local del 1.0f; el valor del pool ya coincide). No es una bajada; se acepta.
- Temporales de Vector3 en inlines (`Dot_unk`, `Cross_unk`, `Neg_unk`, `enemy->mPos + Vector3(...)` por referencia)
  colocan los huecos de pila detras de los locales con nombre, como el original.
- `bool b = IsX(); int v = 4; if (b) v = 2;` reproduce `cntlzw/srwi.` (bool materializado).
- Bucle sobre `FixedArray_unk`: `items.mData[i]` da base `&items` + 4 con puntero incrementado; `items[i]` (int) no.
