Inicio: 2026-11-12 02:29 CET. Funciones al 0 % al empezar (report.json): 972. Al 100 %: 15030 / 18109.

# fueraruta-20261112

Sesion autonoma de 12 h sobre unidades fuera de la ruta de arranque (prompt `claude/prompt-12h-fuera-ruta.md`).

## Resultado

## Cambios de splits

## Decisiones tomadas sin el usuario

- `game/Em314Ivy`: con vf0C escrita, `fn_803132D0` paso del 100 % de objdiff al 99,95 % por el pool de `.sdata2`
  (REP+); al escribir vf100 el pool vuelve a cuadrar y vuelve al 100 % (PRECOMMIT OK).
- `PopupTask::fn_8054E528` recibe un segundo argumento que no lee (`gmk::RandomItem` lo pasa en r5): firma
  `(unsigned int, int)`, simbolo renombrado; las unidades que incluyen `game/PopupTask.h` renumeran etiquetas
  (`lblmap` en ControlStickByQuark, BattleCharaTask, GameTask, LastCocoonEgg, FlyObjManager).
- `Inventory`: los bytes 0xDB18-0xDB2B pasan a campos (`mDB18`..`mDB28`: mapa del objeto que se anade, escrito por
  `gmk::RandomItem`).
- `SwingPart_unk` (nueva cabecera `include/gmk/SwingPart_unk.h`): la parte de mapa que se balancea; la
  `ModelInstance` es un miembro (el ctor no guarda vtable). `fn_803FFD10` = su ctor y `fn_803FFF40`,
  `fn_804003E0`, `fn_804006E0` = miembros (simbolos renombrados). `MapPartsGimmickLoc::mScenes` pasa a
  `SimpleArray<SwingPart_unk*>` (antes `MapPartsGimmickLocScene_unk`), y su vf14 sube al 100 %.
- `ModelInstance` (vista de `gmk/Lemon.h`): `m80` es el puntero a las matrices de nodo (antes relleno).
- `MP/Text.cpp`: `MpTextEntry_unk` describe el mensaje de la tabla del lobby con sus campos de bits; la tabla
  se lee con una vista `MpTextTable_unk` (mismo objeto que `MpMessageTable_unk` de `MP/MpMessage_unk.h`): deuda
  de la regla 10 apuntada en el fuente.

## Hallazgos

## Pendientes

- Bloque 1 (las cuatro unidades a las que solo les faltan funciones al 0 %), sin avance, todas bloqueadas:
  - `atn/LH_Motion_cont` `fn_804766B0` = copia debil de `Loader::Get_unk<LoadMotion>`, ya instanciada en el fuente
    (99,7 %, mismo problema de registros que las doce copias, `include/atn/LoadManager.h`). Renombrar el simbolo al
    mangled no sirve: `atn/LH_Motion.cpp` (enlazada) la llama como `extern "C" fn_804766B0` y dejaria de enlazar;
    llamarla con `lbl_8087F518->Get_unk<LoadMotion>` haria que LH_Motion.o emitiese la copia (99,7 %) y rompiera el
    `.dol`. Fusionar `_cont` con `LH_Motion` exige antes el 100 % de `Get_unk`.
  - `game/LastCocoonChrysalis` ctor: el original esta por encima del presupuesto de inlining (R3) y llama todo fuera
    de linea; el nuestro lo inlinea todo. Mismo caso que el ctor de `LastCocoonImago`; probado ya en sesiones
    anteriores (ver el `NONMATCHING`). Sin candidato legitimo que falte.
  - `game/GimmickManager` `LoadSeTableTask::vf0C` + `fn_803EF3B0` (`boost::function::operator()`, que solo se emite
    si vf0C no lo inlinea): es el caso abierto de `docs/inlining.md`.
  - `atn/BoneCalc_unk` `fn_8009A5CC`/`fn_8009ADA4`: paired-single (88 y 116 instrucciones `ps_`/`psq_`), asm no
    aprobado en esta unidad. Apuntadas.
