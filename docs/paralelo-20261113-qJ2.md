# Lote paralelo 2026-11-13, cola J2 (segunda pasada: juego varios 2 y enemigos)

Inicio 07:35.

## Resultado por unidad (funciones al 0 % antes -> despues)

- `game/MapPartsGimmickLoc` 15 -> 0; 27 (varias falsas) -> 39/46 al 100 %. Escritas `vf10` (98,96 %) y
  `fn_804985E4` (100 %); las 28 copias weak que salen de `fn_804985E4` (funcion fuera del presupuesto de inline:
  llama a todo con `bl`) se rehicieron con sus tipos reales y se renombraron en `symbols.txt`:
  `SimpleArray<SwingPart_unk*>/<Record>::operator[]`, `Resize_unk(n, heap, name)` de los dos arrays,
  constructor del registro, `vector<MapLocator_unk*>` y `vector<GimmickLocSwing_unk>` (ctor, dtor, `empty`,
  `size`, `operator[]`, `push_back`), `GimmickLocTable_unk::GetLocator_unk` (trozo 'locs'/'loct'), setters del
  gimmick (0x6C/0x78/0x90/0x58/0xA4), matriz de escala, `ModelInstance` (luz 0x7C, LOD 0x70-0x78),
  `static_string::c_str`, `make_pair`, `Csv::m630` y la matriz del registro (`UpdateRecordMatrix_unk`, 0x80498354).
  `vf2C` pasa a 100 % (la cadena "vector length error" va ahora delante de "<all>").

## NONMATCHING

- MPGL `vf10` 98,96 %: los huecos de pila de las rotaciones inline salen en orden inverso (en el original la z
  queda abajo; en la copia suelta 0x80498354 el orden es el nuestro) y f26/f27 del aleatorio cruzados. Probado
  escribir la rotacion a mano con `Matrix3x4::RotateAxis_unk` (95 %) y con el `RotateAxis_unk` del fichero (89 %).
- `UpdateRecordMatrix_unk` 99,91 %: huecos de pila de las tres rotaciones.
- `SetLod_unk` 88,57 %: el original hace `frsp` de la distancia antes de dividir (probado releer el miembro, double).
- `vector<MapLocator_unk*>::~vector` 96,77 %: un `beq` de mas en el original (mismo caso que
  `vector<MapModelCache_unk*>` de atn/Motion_unk); viene de `include/std/vector.h` (no tocado).
- `vector<GimmickLocSwing_unk>::push_back` 86,71 %: el original mueve los elementos POD con memcpy al crecer;
  `include/std/vector.h` los copia en bucle (no tocado: cabecera de 350 unidades).

## Decisiones tomadas sin el usuario

- Los inline del original sobre clases muy incluidas (setters de `GimmickCommon`, `ModelInstance`,
  `static_string::c_str`, `CsvBase::m630`, `make_pair`) se escriben como funciones `inline` libres en
  `MapPartsGimmickLoc.cpp` (con un nombre `_unk`) para no renumerar los `@N` de ~130 unidades; sus copias weak
  llevan ese nombre en `symbols.txt`.
- `std::pair<MapLocator_unk*, int>` es una struct POD local `GimmickLocSwing_unk` (no hay `std::pair` en
  `include/std`).
- `push_back` de los dos vectores con instanciacion explicita tras `fn_804985E4` (como AreaEffConfig) para que
  salgan en su sitio y su cadena vaya antes de "<all>".
- Cabecera `MapPartsGimmickLoc_unk.h`: fuera las vistas falsas (`MapPartsGimmickLocObj_unk`,
  `MapPartsGimmickLocArray8/4_unk`, metodos `fn_80498E7C`... de MapPartsGimmickLoc); el registro tiene
  `static_string<32>` (nombre), `m44` escala, `mMtx` (0x48) y los enteros 0x88-0x9C; `m124` pasa a
  `atn::Vector3 m124` + `float m130`. Renumeraba MapTask, ArenaFloor, MapLift, OnOff y RotaryRoom:
  re-emparejadas con `pdbatch.sh` (RotaryRoom 9 -> 12).
- `fn_800E0AA8` (size de un vector de punteros, copia weak de otra unidad) se llama como `extern "C"`, igual
  que en GameScene, CharaPathMove, Em203GreaterDaemonRed y Em302Kraken.

## Hallazgos

- Funcion fuera del presupuesto de inline: sus inline salen como copias weak; las no plantilla en el orden en que
  se usan, justo detras; las plantillas (vector, make_pair, Csv) despues; las que ya tenia otra unidad (ctor de
  `Vector3`, `DegToRad_unk`, `GetMapId`...) se llaman en su direccion de la otra unidad.
- Un inline que devuelve una `Matrix3x4` por valor (`ScaleMatrix_unk`) evita la llamada al constructor vacio
  que sale con `Matrix3x4 m; Set(m, ...)` cuando la funcion esta fuera de presupuesto.

## Pendientes

- `GimmickLocTable_unk` repite `BinLocs_unk` (atn/LH_Binary.cpp) y `BuildingFile_unk` (MapPartsLocatorSet):
  unificar en una cabecera (otras colas).
- poolcheck: el `.o` lleva el 0.0174533 de la copia weak de `DegToRad_unk` (en el original esta en otra unidad).
