# Lote paralelo 2026-11-12, cola A (Tiendas)

Agente A, rama `agent-A`. Inicio: 2026-11-12 14:54 CET (tope ~2,5 h).

## Resultado por unidad

- `game/ShopUpGrade` (unica unidad sin fuente): fuente nuevo `src/game/ShopUpGrade.cpp` + cabecera
  `include/game/ShopUpGrade.h` (mismo patron que ShopIdentify). Ver tabla al final.
- `game/ShopBase`: pendiente.

## Decisiones tomadas sin el usuario

- `symbols.txt`: solo renombrados de las funciones de la clase (`__ct__/__dt__/vfXX/fn_X__11ShopUpGrade...`)
  y de `lbl_8087F9E0` = `ShopUpGrade::sInstance`.
- `src/game/GameTask.cpp` (llamador de otra familia): cambia `extern "C" lbl_8087F9E0` y `fn_8058ABF4` por
  `ShopUpGrade::sInstance` / `ShopUpGrade::fn_8058ABF4` e incluye `game/ShopUpGrade.h` (obligado por el
  renombrado; GameTask sin cambios en el `.o`, comprobado con precommit).
- `ShopUpGrade.cpp` define `inline WeaponParamEntry_unk::~WeaponParamEntry_unk() {}` como
  `db/WeaponParam_unk.cpp` (la cabecera `db/` no se toca).
- Las plantillas de `std::sort` de la unidad (0x8058E0C8-0x80595F78, ~30 funciones) se declaran
  `extern "C"` y no se reconstruyen (como en ShopBase y CosEditItemList_unk).

## Hallazgos

- ShopUpGrade: `mMode` 0 mejora, 1 Rotta (flag 0xD7), 2-3 mejora extra; pestanas `mTab` 0 armas, 1 protectores,
  2 armas especiales; lineas de 0x58 bytes POD (`push_back` por la rama POD de MSL).
- Un argumento `const T&` evita la copia extra de un local pasado a una funcion tras otra llamada
  (fn_80597164: el original pasa la direccion del local).
- Array con contador sueltos (`unsigned int mReloadNum; T mReload[7]`) en vez de `atn::FixedArray_unk`
  cuando el original indexa con la base `this` reducida (fn_8058CF60).
