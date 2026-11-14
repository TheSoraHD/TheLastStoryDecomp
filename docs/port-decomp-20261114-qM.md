# port-decomp-20261114, cola M: `CharaTask::vfBC` con su argumento

Rama `agent-M`. Funciones al 100 % (report.json): 15396 -> **15397** (lost 0, fuzzdrop 0, labelcheck y rep100
iguales que `$REF`).

## Resultado por funcion

| Funcion | Antes | Despues | Firma final |
|---|---|---|---|
| `CharaTask::vfBC` (copia weak 0x800EF308) | 100 % (`vfBC__9CharaTaskFv`) | 100 % | `virtual int vfBC(atn::ColliBase*) { return 1; }` |
| `LastCocoonImago::vfBC` (0x80459FB8, antes `fn_80459FB8`) | 0 % (sin escribir) | **100 %** | `int vfBC(atn::ColliBase* target) { return target != &m18FC; }` |
| `GameUI::fn_803DBD44` (0x803DBD44) | 97,87 % (sin el argumento) | 97,89 % (NONMATCHING) | `void GameUI::fn_803DBD44()`, sin cambios |
| ctor de `LastCocoonImago` (0x80458270) | 0 % | 0 % (NONMATCHING, sin cambios) | - |
| `fn_80458550` (copia weak de `atn::ColliSphere::ColliSphere()`) | - | - (no se emite) | - |

## Cambios de firma o de layout (para el port)

- **`CharaTask::vfBC(atn::ColliBase*)`** (antes `vfBC()`), `include/game/CharaTask.h`, sin nombre de parametro.
  El argumento es el objeto de colision bajo el cursor (`BattleTask::m3E2E8`, o 0 sin BattleTask); devuelve
  `int` (el llamador hace `cmpwi r3, 0`). Unico override en las 78 vtables que heredan de CharaTask:
  `LastCocoonImago` (falso si el cursor esta sobre su esfera `m18FC`). Unico llamador en todo el DOL:
  `GameUI::fn_803DBD44` (buscado `lwz r12, 0xbc(r12)` tras cargar `-0x1d18` en todo el asm).
- `LastCocoonImago.h`: declara `virtual int vfBC(atn::ColliBase*)` (0x80459FB8).
- `GameUI.cpp`: `GetCursorTarget_unk()` devuelve `atn::ColliBase*` (cast de `RayObj_unk*`).
- symbols.txt: `vfBC__9CharaTaskFv` -> `vfBC__9CharaTaskFPQ23atn9ColliBase`, `fn_80459FB8` ->
  `vfBC__15LastCocoonImagoFPQ23atn9ColliBase`; `@enum$6811ControlAppear_cpp` -> `@enum$6812ControlAppear_cpp`
  (6 nombres de chr/ControlAppear: boost::function de `MOTION_ID`).

## Re-emparejado de las `@N` (decisiones tomadas sin el usuario)

- Un parametro **sin nombre** en una inline **definida** en la cabecera tambien consume un `@N`: las 218
  unidades que incluyen CharaTask.h se renumeran (+1).
- `pdbatch.sh` de las 199 unidades con LC+/REP+ (6 min). Tras el, devueltos a mano 7 nombres que
  pairdata cambio mal: `lbl_8078CB00/08/10` (cadenas renombradas a `__vt__...bad_function_call`,
  `shared_ptr_deleter`, `runtime_error`), `lbl_8087D6E8/EC`, `lbl_8087EE78` (sInstance) y `lbl_80885CB0`
  (renombrada a `sInstance__9SetConfig`).
- `lblmap.py` en las 29 unidades que quedaban; despues, para los floats que no salen en las diferencias
  (funciones sin escribir), alineado de los pools por valor (`.sdata2`/`.rodata` del obj partido frente a
  nuestro `.o`, difflib sobre los valores; script en `/home/claude/scratch-M/align.py`) en
  Em202ForestSpider, Em203GreaterDaemonRedMp, Em206CocoonMp, EnemyQuarkSecond, LastCocoonImago, PacketPack,
  GameCamera_unk, NewCampTask y GameUI. `lblmap.py` habia dejado nombres repetidos dentro de una unidad
  (p. ej. `@19835` tres veces en Imago): el alineado por valor los corrige.
- Unidades de otras colas tocadas **solo** en symbols.txt (renumerado de `@N`): BattleTask, AiTarget, GameTask,
  MiniMapTask (K), TacView y Event/* (L), atn/* (J) y en general las 199 de la lista. Si otra cola cambia
  cabeceras de esas unidades, al integrar habra que repetir pdbatch/align en ellas.
- `GameUI::fn_803DBD44`: con el argumento real bajaba de 97,87 a 97,62 % (el original recarga `lbl_8087F048`
  para `m3E2E0` y calcula `mF94[m1014[i]]` despues del argumento; el nuestro lo sube por encima). Se acepta
  por la regla "firma real primero" y se recupera en un segundo commit: `GetWidth_unk()/GetHeight_unk()`
  de `atn::GraphicsDevice` y `dist[i] = 1e16f` primero en el bucle de reinicio arreglan los registros del
  primer bloque: **97,89 %**.

## Probado sin exito

- GameUI, region de vfBC (8 variantes, ninguna mejora): argumento en una variable antes de la llamada,
  operandos de la comparacion al reves, accesores inline (`GetCursorChara_unk`, `fn_8013A194()->m3E2E0`,
  `IsCursorChara_unk(CharaTask*)`), puntero local a BattleTask, ternario directo, `== 0`. Quedan tambien
  los huecos de pila de los vectores del bucle de la vista cenital (0x40 bytes de temporales mas abajo en
  el original) y el registro del resultado de `IsWaiting_unk`.
- Ctor de LastCocoonImago: con una inline local de K asserts vacios llamada desde el ctor, K <= 2 sigue
  por debajo del presupuesto (todo inline, 0 %) y K >= 4 lo pasa (95,8 %, mismo tamano que el original +4;
  diferencias: la llamada a la inline nueva, que entonces sale con `bl`, y r30/r31 del cero y de
  `@stringBase0`). Al original le faltan ~3-4 unidades de coste de candidatas que no se ven en sus llamadas
  (todas las `bl` coinciden). Sin forma natural encontrada; misma familia que el ctor de LastCocoonChrysalis
  (alli faltaban 33-36).
- `fn_80458550` = `__ct__Q23atn11ColliSphereFv` (copia weak). No se renombra en symbols.txt: un global con
  ese nombre en el obj partido de una unidad no enlazada podria desplazar las copias weak de las enlazadas.

## Pendientes

- `RayObj_unk` (BattleTask.h, cola K) es `atn::ColliBase` (flags en 0x08, dueno en 0x0C): cambiar el tipo
  de `BattleTask::m3E2E8` y quitar el cast de GameUI. LastCocoonImago.cpp sigue con su vista local
  `Effect_unk` de BattleTask (`void* m3E2E8`, `fn_800F8548(Effect_unk*)`...): deuda de la regla 10 de
  BattleTask.
- Hallazgo: un parametro sin nombre en una funcion inline definida en una cabecera consume un `@N` en cada
  unidad que la incluye (como uno con nombre en una declaracion).
