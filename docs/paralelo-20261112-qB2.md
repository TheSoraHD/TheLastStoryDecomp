# Lote paralelo 2026-11-12, cola B2 (vestuario y campamento, segunda pasada)

Inicio: 2026-11-12 21:50 CET (tope 23:35).

## Resultado por unidad (funciones al 0 % / sin escribir, antes -> despues)

- `game/NewCampTask`: 13 -> 8 (sin contar las copias debiles que dependen del presupuesto). `report.json`
  15200 -> 15204. Escritas `fn_8052CA6C` (cierre del campamento, 77,6 %), `NewCampView_unk::fn_8052E1A0`
  (guarda escena/camara/post efectos, 71 %) y `fn_8052FA44` (los aplica, 73 %; el original lo inlinea en
  fn_8052CA6C y aqui tambien). Con fn_8052E1A0 escrita, `vf0C` pasa el presupuesto R3 como el original:
  33 % -> 99,5 % y `RenderSetting_unk::ResetPostFx_unk` (0x8052B7F8) se emite: 96,6 %; al 100 % nuevas las
  copias debiles `fn_8052B7F0`, `ResHandle::Load` (0x8052BC90), `vector<NewCampElem_unk>::operator[]`/`size`.

- `game/NewCampPartsMapSelect`: 1 -> 0. `vf14` (0x8051D864, dibujo del mapa, 5 KB) escrita: 96,3 %. Declarada
  `virtual void vf14()` en `NewCampPartsMapSelect.h` (la vtable apuntaba a la de la base); simbolo
  `fn_8051D864` -> `vf14__21NewCampPartsMapSelectFv`. Tabla `sBackRanges_unk` (lbl_8075BE80, rangos de los
  tableros de fondo) definida en el fuente. El `.sdata2` de la unidad queda identico al original.

## NONMATCHING

- `fn_8052CA6C` 77,6 %, `fn_8052E1A0` 71 %, `fn_8052FA44` 73 %: el `atn::Scene::operator=` implicito pasa el
  presupuesto en el original (R3/R5) y llama a los `operator=` de Texture, GodRay, HazeParam_unk y
  ColorMatrixParam_unk (0x8052E8D4, 0x8052BBEC, 0x8047C7FC, 0x80453DBC); aqui se inlinean porque los de
  `vector<Light>`/`vector<Camera>`/`CharaLightData_unk` solo estan declarados (no son candidatos). Arreglarlo
  pide definir esos `operator=` en cabeceras de atn/std (otra cola): pendiente.
- `vf0C` 99,5 %: registros del grupo del menu de depuracion (r27/r28); probado declarar `sub` antes: igual.
- `ResetPostFx_unk` 96,6 %: registros de las constantes del Dof.

- `NewCampPartsMapSelect::vf14` 96,3 %: temporales del lerp de `MoveEntry_unk`, un registro float guardado de mas, orden de las sumas de la caja de depuracion y del offset de camara (mDC 2). Las marcas se escriben sin helper inline: las cadenas de un helper inline van a un pool `@STRING@` propio

- Pool de cadenas de NewCampPartsMapSelect: el original tiene un NUL de mas tras "map_board01_%04d",
  "map_board01.d2a", "map_be010_001_%04d" y "" de vf4C (bytes sin referencia). Se escribe `"...\0"` en esos
  `sprintf`: `@stringBase0` identico al original salvo el relleno final; `vf48` al 100 %, vf4C 94,8 -> 96,5 %,
  vf10 95,2 -> 94,9 % (registros, el comentario de cadenas desaparece). Decision a revisar (estilo).

## Parche de `MslSort_unk.h` (agente A2)

Probado `/home/claude/scratch-A2/MslSort_unk-fix.patch` (sOffset = -4, `n3` aparte, rama `i == first` con
`j = last; --j`): sube NewCampPartsItem (dos sorts 94,9 -> 100 %, medianas 91,5 -> 94,4 %), CosEditItemList_unk
(sorts con predicado inline 88,3 -> 89,9 %, 94,2 -> 95,6 %) y Event/Node (72,8 -> 78,4 %); GameUI igual. Pero
bajan los cuatro sorts de CosEditItemList_unk con predicado fuera de linea (2,89 -> 0 %, ya completamente
distintos: R3). Por la condicion del integrador (ninguna bajada) **no se aplica**; recomendable aplicarlo en la
integracion aceptando esa bajada cosmetica.

## Decisiones tomadas sin el usuario

- `symbols.txt`: renombrados `fn_8052BC90` -> `Load__9ResHandleFPCc`, `fn_8052BCE4` ->
  `__vc__Q23std25vector<15NewCampElem_unk>FUi`, `fn_8052BCF4` -> `size__Q23std25vector<15NewCampElem_unk>CFv`;
  dos etiquetas de `.sdata2` de NewCampTask que `pd.sh` no renumero (`@16438` -> `@17601`, `@16623` -> `@17786`).
- El color de borrado de `NewCampView_unk` (0x238) se lee con un cast `*(RsColor_unk*)m238`: con un miembro
  `RsColor_unk` el constructor de NewCampView_unk bajaba (llama al constructor de RsColor_unk).

- `symbols.txt`: `@14042`/`@14384` de NewCampPartsMapSelect renombradas a mano a `@15196`/`@15538` (pd.sh no
  las renumero). Despues, todas las etiquetas `@N` de `.sdata2` de NewCampPartsMapSelect renombradas por
  direccion a las de nuestro `.o` (el `.sdata2` es identico byte a byte; pairdata las emparejaba mal por valores
  repetidos).

## Hallazgos

- Escribir una funcion grande no inline del mismo fichero puede subir otras: con `-ipa file` es candidata a
  autoinline de sus llamadores y los pasa del presupuesto (vf0C 33 % -> 99,5 % al escribir fn_8052E1A0).
