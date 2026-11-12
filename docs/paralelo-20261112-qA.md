# Lote paralelo 2026-11-12, cola A (Tiendas)

Agente A, rama `agent-A`. Inicio: 2026-11-12 14:54 CET (tope ~2,5 h).

## Resultado por unidad

- `game/ShopUpGrade` (unica unidad sin fuente): fuente nuevo `src/game/ShopUpGrade.cpp` + cabecera
  `include/game/ShopUpGrade.h` (mismo patron que ShopIdentify). Funciones al 0 %: **57 -> 2**
  (quedan `__cl__20ShopUpGradeLessC_unk` 0x80590448 y `SortSelect_unk<..LessC..>` 0x80591504: MWCC las
  inlinea en `Sort_unk<..LessC..>`, que por eso da 0 % por tamano). **30 / 57 al 100 %**; el resto
  escritas con `// NONMATCHING` (ctor 94,7 %, vf20 98,7 %, fn_8058DB10 94 %, fn_805969D4 90 %,
  fn_80597554 91 %, fn_80598530 96 %, sort 93-99,9 %).
- `std::sort` de la unidad reconstruido (`Sort_unk`/`SortMedian_unk`/`SortSelect_unk`/`MinElement_unk`,
  iterador `ShopUpGradeIter_unk` con sus miembros fuera de linea, cuatro predicados `ShopUpGradeLessA-D_unk`):
  quicksort con mediana de tres en `*(last-1)`, desplazamiento rotatorio `static int` de -4 a 4 por
  instanciacion (los `.sdata` 0xFFFFFFFC), seleccion por debajo de 21, recursion por la copia `Cmp&`.
  Sirve de guia para el sort de `game/ShopBase` (0x80582598-0x80584174) y `game/CosEditItemList_unk`.
- `game/ShopBase`: sin cambios (6 al 0 % = su `std::sort` de punteros crudos; no dio tiempo).

## Decisiones tomadas sin el usuario

- Ultimo commit con `PRECOMMIT FAIL` solo por `rep100`: 4 funciones nuevas (dos `SortMedian_unk` y dos
  predicados) que `report.json` cuenta al 100 % y objdiff al 99,8-99,9 % (el literal `L"%s"` de un inline
  sale como `@STRING@` debil; en el original esta en `@wstringBase0`). Estan marcadas `NONMATCHING` y no se
  cuentan; main.dol OK, lost 0, sin bajadas, ordercheck/mnemcheck/poolcheck limpios.

- `symbols.txt`: solo renombrados de las funciones de la clase (`__ct__/__dt__/vfXX/fn_X__11ShopUpGrade...`)
  y de `lbl_8087F9E0` = `ShopUpGrade::sInstance`.
- `src/game/GameTask.cpp` (llamador de otra familia): cambia `extern "C" lbl_8087F9E0` y `fn_8058ABF4` por
  `ShopUpGrade::sInstance` / `ShopUpGrade::fn_8058ABF4` e incluye `game/ShopUpGrade.h` (obligado por el
  renombrado; GameTask sin cambios en el `.o`, comprobado con precommit).
- `ShopUpGrade.cpp` define `inline WeaponParamEntry_unk::~WeaponParamEntry_unk() {}` como
  `db/WeaponParam_unk.cpp` (la cabecera `db/` no se toca).
- Las plantillas de `std::sort` de la unidad (0x8058E0C8-0x80595F78, ~30 funciones) se declaran
  `extern "C"` y no se reconstruyen (como en ShopBase y CosEditItemList_unk).

- Incluir `game/ShopUpGrade.h` en GameTask renumera sus etiquetas `@N`: tras cada cambio de la cabecera hay que
  pasar `precommit.sh --pd game/ShopUpGrade game/GameTask`; `pairdata` no reempareja un float de `.sdata2`
  (0x808857A8, usado por `GameTask::fn_8036D2D0`): renombrarlo a mano al `@N` de nuestro `.o` (`nm`).
- `ShopUpGradeEntry_unk` lleva constructor (sus copias son miembro a miembro como en el original,
  fn_8058CA44), aunque el `push_back` original de fn_805969D4 va por la rama POD de MSL: contradiccion sin
  resolver, fn_805969D4 queda en NONMATCHING.
- `fn_80597268`/`fn_80597554` usan la vista parcial local de `atn::D2A_Task` (SetNumber_unk/SetColor_unk), como
  ShopIdentify, porque `atn/D2A_Task.h` choca con las declaraciones `extern "C"` de las tiendas.

## Hallazgos

- ShopUpGrade: `mMode` 0 mejora, 1 Rotta (flag 0xD7), 2-3 mejora extra; pestanas `mTab` 0 armas, 1 protectores,
  2 armas especiales; lineas de 0x58 bytes POD (`push_back` por la rama POD de MSL).
- Un argumento `const T&` evita la copia extra de un local pasado a una funcion tras otra llamada
  (fn_80597164: el original pasa la direccion del local).
- Array con contador sueltos (`unsigned int mReloadNum; T mReload[7]`) en vez de `atn::FixedArray_unk`
  cuando el original indexa con la base `this` reducida (fn_8058CF60).
