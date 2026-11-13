# Lote paralelo 2026-11-13, cola J2 (segunda pasada: juego varios 2 y enemigos)

Inicio 07:35.

## Resultado por unidad (funciones al 0 % antes -> despues)

- `game/MapPartsGimmickLoc` 15 -> 0; 27 (varias falsas) -> 41/46 al 100 %. Escritas `vf10` (99,64 %) y
  `fn_804985E4` (100 %); las 28 copias weak que salen de `fn_804985E4` (funcion fuera del presupuesto de inline:
  llama a todo con `bl`) se rehicieron con sus tipos reales y se renombraron en `symbols.txt`:
  `SimpleArray<SwingPart_unk*>/<Record>::operator[]`, `Resize_unk(n, heap, name)` de los dos arrays,
  constructor del registro, `vector<MapLocator_unk*>` y `vector<GimmickLocSwing_unk>` (ctor, dtor, `empty`,
  `size`, `operator[]`, `push_back`), `GimmickLocTable_unk::GetLocator_unk` (trozo 'locs'/'loct'), setters del
  gimmick (0x6C/0x78/0x90/0x58/0xA4), matriz de escala, `ModelInstance` (luz 0x7C, LOD 0x70-0x78),
  `static_string::c_str`, `make_pair`, `Csv::m630` y la matriz del registro (`UpdateRecordMatrix_unk`, 0x80498354).
  `vf2C` pasa a 100 % (la cadena "vector length error" va ahora delante de "<all>").

- `game/Em206CocoonMp` 5 -> 2 (quedan ctor y `ResHandle::ResHandle`, bloqueados como el ctor de
  LastCocoonChrysalis); 27/39 al 100 % (sin cambios). Escritas: `fn_802BC4EC` (96,25 %, portada de
  `LastCocoonImago::fn_8045ABB4`), `fn_802BEE44` (estado 13, 95,05 %) y `fn_802BF5C0` (estado 14, 93,92 %).

- `game/GameSaveData_unk` 2 -> 0; 26/37 al 100 % (sin cambios). Escritas: `GameSaveInventory_unk::fn_803BB584`
  (recoge el inventario, 94,37 %) y `fn_803BC2D4` (lo restaura, 87,31 %).

- `game/Em203GreaterDaemon` (parciales): `vf3C` al 100 % (lectura de `down` antes de anular el empuje y
  `m14B4 = 0` antes de `m151C`); 46 -> 47/66. El `vector<int>::operator[]` sigue sin salir (presupuesto).
- `game/Em203GreaterDaemonRedMp`: `fn_802A3D80` sin cambios (probado otra vez con los helpers como `extern "C"
  inline` delante: sigue inlineandolo todo).

## NONMATCHING

- MPGL `vf10` 99,64 %: orden de las direcciones de pila izadas del modo 2 y de la direccion del registro en la
  prueba del recuento.
- `SetLod_unk` 88,57 %: el original hace `frsp` de la distancia antes de dividir (probado releer el miembro, double).
- `vector<MapLocator_unk*>::~vector` 96,77 %: un `beq` de mas en el original (mismo caso que
  `vector<MapModelCache_unk*>` de atn/Motion_unk); viene de `include/std/vector.h` (no tocado).
- `vector<GimmickLocSwing_unk>::push_back` 86,71 %: el original mueve los elementos POD con memcpy al crecer;
  `include/std/vector.h` los copia en bucle (no tocado: cabecera de 350 unidades).

- Em206CocoonMp `fn_802BC4EC` 96,25 %, `fn_802BEE44` 95,05 %, `fn_802BF5C0` 93,92 %: huecos de pila
  (matrices de `ToRotation_unk`, temporales), registros flotantes; en las dos ultimas los MIN/MAX de las
  velocidades salen con la comparacion invertida y el `MIN(1.0f, 1.0f)` del original no se pliega alli.

- GameSaveInventory `fn_803BB584` 94,37 % y `fn_803BC2D4` 87,31 %: asignacion de registros y direccionamiento
  de las copias.

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

- Em206CocoonMp: `#include "gmk/Lemon.h"` (para `CollResult_unk`) y `GameConfig_unk.h`; seis `@N` de
  `.sdata2` renombrados a mano por valor (pairdata los daba por ambiguos).

- GameSaveInventory: los conjuntos de equipo del inventario (`Inventory::Equip_unk`) se copian a/desde los
  `GameSaveEquipExtra_unk` del guardado a traves de una vista (`AsEquip_unk`), y `MP::SaveElemB_unk` con un
  `CopyUpgrade_unk` local (el original usa su `operator=` con un `static_string` ancho en 0x294: tocaria
  `MP/SaveElemB_unk.h` y `atn/static_string.h`, de otras colas). El orden del grupo se lee como 19 palabras desde
  `Inventory::mOrder` (que sigue en `m64C8`) y las armas como 4 huecos desde `mWeapons` (`Equip_unk::m30`):
  pendiente ampliar esos miembros en `game/Inventory.h` (cola de ItemManager_unk).
  En `GameSaveData.h` `m92B4` pasa a `mIdentify[0x100]`, `mOrder[3][6]` (signed char) y `mOrder18`.
- pd.sh en GameSaveData_unk renombro `lbl_80885CB0` a `sInstance__9SetConfig` (enlace roto): devuelto a mano.

## Hallazgos

- Funcion fuera del presupuesto de inline: sus inline salen como copias weak; las no plantilla en el orden en que
  se usan, justo detras; las plantillas (vector, make_pair, Csv) despues; las que ya tenia otra unidad (ctor de
  `Vector3`, `DegToRad_unk`, `GetMapId`...) se llaman en su direccion de la otra unidad.
- Un inline que devuelve una `Matrix3x4` por valor (`ScaleMatrix_unk`) evita la llamada al constructor vacio
  que sale con `Matrix3x4 m; Set(m, ...)` cuando la funcion esta fuera de presupuesto.

- Copia de un array de structs: `for (i < 16) a[i] = b[i];` con elementos de 0x40 sale como en el original
  (bucle desenrollado de 8 en 8: 2 x 0x200); copiar el bloque entero como una struct da un bucle de 8 bytes.

- Huecos de pila de rotaciones inline al reves: el original pone las tres rotaciones en un inline propio
  (`RotateEulerZYX_unk`, como `RotateEuler_unk` de MapInstance_unk) dentro del de la matriz: un nivel de inline
  mas invierte el orden. Con eso `UpdateRecordMatrix_unk` y `fn_8049994C` salen al 100 % y `vf10` 99,64 %.

## Pendientes

- `GimmickLocTable_unk` repite `BinLocs_unk` (atn/LH_Binary.cpp) y `BuildingFile_unk` (MapPartsLocatorSet):
  unificar en una cabecera (otras colas).
- poolcheck: el `.o` lleva el 0.0174533 de la copia weak de `DegToRad_unk` (en el original esta en otra unidad).
- `game/Bullet` `fn_80110858` (6 KB, ~1550 instrucciones) sin escribir: no cabia en el tiempo que quedaba
  (empieza con el impacto sobre personaje `m38->m08 & 2` / gimmick `& 8` y su `GimmickHitMsg` por `vf44`).
- Em206CocoonMp ctor y `ResHandle::ResHandle` (copia weak): el original esta fuera de presupuesto (llama hasta
  a los ctores de `Vector3` y `LH_Binary`); el nuestro lo inlinea todo, igual que LastCocoonChrysalis.
- Em203GreaterDaemonRedMp `fn_802A3D80` y Em203GreaterDaemon `vector<int>::operator[]`: presupuesto de inline.
- `game/Inventory.h` (cola ItemManager_unk): `mOrder` tiene 19 palabras (0x64A0-0x64EC) y `Equip_unk` 4 huecos de
  arma (`m30`); `MP/SaveElemB_unk.h`: el nombre de 0x294 es un `static_string` ancho (asignacion con
  wcslen/wmemcpy). Con eso `fn_803BB584`/`fn_803BC2D4` podrian usar los `operator=` del original.
- `atn::Keyboard::Press` (copia weak en RedMp, 90 %): el original carga `mD90` despues del `li r31, 0`
  (cabecera atn/Keyboard.h, de otra familia).
