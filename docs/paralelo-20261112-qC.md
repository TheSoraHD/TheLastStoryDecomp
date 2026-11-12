# Lote paralelo 2026-11-12, cola C (Batalla)

Inicio: 2026-11-12 14:55 CET. Rama `agent-C`. Unidades: `game/BattleTask`, `game/BattleCharaTask`,
`game/TacView`, `game/TeamAI_unk`.

## Resultado por unidad

- `game/TeamAI_unk`: funciones al 0 % 7 -> 0. Nuevas al 100 %: 4 (`fn_805B1918` sube de 99,9 % al pasar
  el cuarto argumento, y las tres copias debiles del arbol: `InsertNode_unk` 0x805B3120, `Destroy_unk`
  0x805B3258, `FindInsertPos_unk` 0x805B33B8). `Move_unk<P>` (tres instancias) al 87,6 %, `fn_805B27A0` al
  98,4 %. Orden del `.dol` ya correcto en la unidad (los templates se definen antes de `fn_805AED74`).

- `game/BattleTask` (1): `BattleSlot_unk` es `BattleTask::BetoModel` (nombre del RTTI del functor del
  `boost::bind`). Nuevas al 100 %: `vf0C` (0x800F26D4), `BetoModel::BetoModel` (0x8010D808),
  `BetoModel::fn_8010D854` (0x8010D854, callback de nodos) y el invoker/manager del `boost::function`
  (0x800F2BB8, 0x800F2BE4). `fn_800F3490`/`fn_800F3B4C` pasan a miembros (GameTask.cpp actualizado).
  Tambien al 100 %: `fn_80109864` (mensaje de log de un estado), `fn_8010CB2C` (ritmo de la carga de un
  personaje) y `fn_8010CD3C` (tirada de un estado). BattleTask: 0 % 76 -> 70.

## NONMATCHING

- `TeamAI_unk::Move_unk<P>` 87,6 %: el original tiene un marco mayor (mas temporales de vector en la pila)
  y guarda el resultado sin usar del `map::insert` inline (par iterador/bool con los bool de `.sdata`
  `lbl_8087E6E8`/`E9`); el nuestro lo elimina. Registros.
- `TeamAI_unk::fn_805B27A0` 98,4 %: registros; el original recorre los miembros de un equipo con puntero.

## Decisiones tomadas sin el usuario

- `Move_unk` tiene un cuarto argumento (`int mode` = `TeamMove_unk::m0A`, el original lo lee de r7):
  cambia la declaracion en `include/game/TeamAIManager.h` (incluida por AiTarget, Em206EggMonster,
  GameEventScript y TeamAIManager). Renumera los `@N`: relabel con `pdbatch.sh` de esas cinco unidades y
  cuatro etiquetas de `.sdata2` de GameEventScript (0x80885B10, B2C, B30, B34) corregidas a mano (+1).
- `std::map<float, int>` de MSL escrito a mano en `TeamAI_unk.cpp` (`TeamDistTree_unk`, copia de los
  arboles de `GameEventScript.h` con clave `float`): `<map>` no esta disponible. El miembro se guarda
  como `int` (los dos mapas comparten las copias debiles, asi que tienen el mismo tipo).
- **Los parametros con nombre de una declaracion en una cabecera consumen un `@N` cada uno** (sin nombre,
  no): las declaraciones nuevas de `BattleTask.h` y el cuarto argumento de `Move_unk` en
  `TeamAIManager.h` van sin nombre de parametro para no renumerar las unidades que las incluyen. El
  relabel de 55 unidades del commit anterior se deshizo (symbols.txt vuelve a las etiquetas de la base
  fuera de BattleTask y TeamAI_unk).
- Llamadores `extern "C"` pasados a metodos de `BattleTask` (cast `((BattleTask*)lbl_8087F048)->`, como ya
  hacian): GameTask (`fn_800F3490`), Em001Necromancer y chr/ControlChant (`fn_8010CB2C`), CharaTask y
  DamageCalc_unk (`fn_8010CD3C`). Quitar sus `extern` con parametros con nombre renumera sus `@N`: relabel
  con `pd.sh`/`pdbatch.sh` de esas cinco unidades (y dos etiquetas de GameTask a mano, 0x808857A8/AC).
  **Al integrar, repetir `pdbatch.sh` de esas unidades si otra cola las toco.**
- `BattleTask` declara sus virtuales (`~BattleTask`, `vf0C`, `vf10`, `vf14`); solo `vf0C` esta definida.
- `fn_800F29D0` (copia debil de un inline de `atn::Model` que fija el callback de nodos) sigue como
  `extern "C"`: `ModelBase::mB8` esta declarado como `boost::function<void()>` y cambiarlo toca atn/Model.
- `lbl_8087F048` en TeamAI_unk.cpp pasa de `BulletMgr_unk*` a `BattleTask*` (un objeto, un tipo).

## Hallazgos

- Cada parametro con nombre de una funcion declarada (miembro o `extern`) consume un `@N` en la unidad;
  sin nombre no. Para anadir metodos a una cabecera muy incluida, declararlos sin nombres de parametro.

- Comparaciones de la clave de un arbol MSL con `mfcr`/`srwi.`: el original pasa por un comparador que
  devuelve `bool` (`std::less`); un `<` directo da `bge`.
- Cambiar los parametros de una funcion miembro declarada en una cabecera renumera los `@N` de todas las
  unidades que la incluyen; `pdbatch.sh` deja sin renombrar las que chocan con el nombre siguiente.
