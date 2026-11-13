# Lote paralelo 2026-11-13, cola G2 (eventos 2, segunda pasada)

Inicio: 2026-11-13 07:19 CET. Tope 09:49.

## Resultado por unidad (funciones al 0 % / sin escribir, antes -> despues)

- `Event/Puppeteer_CharaTask`: 14 -> ... (82 -> 83 al 100 %).
  - `Actor::fn_80010B68` (rotacion del actor; para modelo y dummy, del eje z de su matriz) 100 %: metodo
    `const` (renombrado en `symbols.txt` a `fn_80010B68__Q25Event5ActorCFv`), punteros locales por caso,
    `GetRow_unk(2).ToRotationZ_unk()` y `operator-` inline (como en gmk/Cannonball).
  - `Actor::fn_80010490` (pone la rotacion; modelo y dummy conservan la traslacion) 97,6 % y
    `Actor::fn_80011420` (rotacion en y) 99,0 %: el mismo cuerpo en el inline `Actor::SetRotation_unk`
    (el original lo inlinea en las dos).
  - `fn_8001011C` 99,2 -> 99,6 % (columnas de `SetModelMtx_unk` por `MtxCol_unk`, evaluadas z, y, x).

## NONMATCHING

- `fn_80010490` 97,6 %: el original guarda this/rot en r28/r29 y la matriz y el modelo del caso 3 en r31/r30
  (aqui comparten registro); matriz del caso 3 arriba del marco.
- `fn_80011420` 99,0 %: huecos de las matrices de rotacion (0x30 mas arriba aqui).

## Decisiones tomadas sin el usuario

- `include/Event/Actor.h` (33 unidades): `fn_80010B68` pasa a `const` y miembro nuevo
  `inline void SetRotation_unk(const atn::Vector3&)` sin nombre de parametro (no renumeran). Llamadores de
  `fn_80010B68` (Event/Path, Event/ActionActorPosition) sin cambios en el fuente; el nombre en `symbols.txt`.
- `@15509`/`@15510` (0.0f/1.0f de `.sdata2` de la unidad) renombrados a mano a `@16302`/`@16303` (pd.sh no
  los renombra).

## Hallazgos

- Primera carga de `m20` antes de guardar los FPR en el prologo: metodo `const`; con `const`, `if (m1C != 0)`
  se relee: usar un puntero local por caso.

## Pendientes
