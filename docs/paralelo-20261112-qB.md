# Lote paralelo 2026-11-12, cola B (vestuario y campamento)

Inicio: 2026-11-12 14:55 CET. Tope ~17:25.

## Resultado por unidad (funciones al 0 % / sin escribir, antes -> despues)

- `game/CosEditItemList_unk`: 65 -> 37. `CosEditProtector::fn_801D94D0` (estado 13, 3,6 KB) escrita
  (95,3 %); las copias de `std::sort` (6 predicados x {sort, sort recursivo, mediana}) escritas (87-98 % con
  predicado inline; 3 % las dos con predicado fuera de linea); `__sinit` (sHidePos). 100 % nuevas: `size`/
  `operator[]` de vector, `clear`, begin/end (`CosEditBegin_unk`/`CosEditEnd_unk`), push_back 97 %.
  `fn_801DD26C` 81,3 -> 94,7 % (begin/end como iteradores).
- `game/NewCampPartsItem`: 22 -> 0. `fn_80517B3C` (99,4 %) y todas las copias de `std::sort`: 15 -> 30
  funciones al 100 % (operadores del iterador, swap, mediana del orden de la bolsa); el resto 91-98 %.
- `game/NewCampPartsMapSelect`: 4 -> 1. `fn_805212BC` (cursor del mapa) 91,8 %, `vf10` (0x8051CF78) 94,7 %,
  `vf28` (0x8051F138) 96 %; falta `vf14` (0x8051D864, 5 KB).
- `game/NewCampTask`: 16 -> 15. `vf0C` (carga y menu de depuracion) 91 %.
- `game/CosEditProtector`: 1 -> 1 (`vf64`, 9 KB, sin tocar).

## NONMATCHING

- `fn_801D94D0` 95,3 %: registros de los primeros bucles; el indice del `erase` se calcula desde la direccion.
- `MslSort_unk`/`MslMedian3_unk` (`include/game/MslSort_unk.h`): 87-98 % con predicado inline; con los
  predicados fuera de linea (`CosEditEquipLess_unk`, `CosEditArmorLess_unk`) el original pasa el presupuesto
  R3 y llama a los operadores del iterador, a la seleccion y a la mediana (aqui se inlinean): 3 %. Probado el
  predicado `inline` (dentro y fuera de la clase): lo inlinea todo; revertido.
- `NewCampCategoryLess_unk::operator()` 98,1 %, `fn_80517B3C` 99,4 % (registros del push_back).
- `fn_805212BC` 91,8 %, `vf10` 94,7 %: el bucle de los puntos guarda el indice (`lwzu`/`stfsu`); `vf10` con
  offsets de `@stringBase0` desplazados hasta escribir `vf14`. `vf28` 96 %: el anime de la miniatura se
  relee tras el `sprintf` (como `vf20`).
- `NewCampTask::vf0C` 91 %: el original pasa el presupuesto R3 y llama fuera de linea a los accesores (gamma,
  `ResHandle::Load`, `size`/`operator[]` de `sElems_unk`); `fn_8052B7F8`, `fn_805303D0`, `fn_805305D0` sin
  escribir (declaradas).

## Decisiones tomadas sin el usuario

- `CosEditProtector::m1C0C`/`m1C30`: `FixedArray_unk<CosEditSet_unk,4>` -> `FixedArray_unk<NewCampPair_unk*,8>`
  (mismo tamano 0x24; solo los usa fn_801D94D0; `CosEditProtector.h` la incluyen 4 unidades).
- Cabecera nueva `include/game/MslSort_unk.h`: iterador de clase de `std::vector` (`MslIter_unk`: constructor
  desde `T* const&`, `operator+`/`-` devuelven `T*`, `operator=` escrito; con eso los pesos y el orden de las
  copias debiles de NewCampPartsItem salen exactos) y el `__sort` de MSL. La comparten las dos unidades.
- La lista de armas `mB4C` sigue con `CosEditItem_unk`: el original tiene otras copias debiles para ella
  (0x801E26B4-0x801E2D68, 0x801E6374-0x801E6700: otro tipo de entrada); quedan sin escribir.
- `symbols.txt`: solo renombrados (copias de sort, iterador, begin/end, `__sinit_\CosEditItemList_unk_cpp`);
  `sHidePos` (0x807C7D38) con `scope:local` (pd le puso el nombre global de CosEditPalette_unk y el enlazador
  daba duplicado).

- `include/game/NewCampTask.h`: declarados `fn_805303D0()`/`fn_805305D0()` (los llama vf0C; 12 unidades la
  incluyen; labelcheck igual). `include/game/NewCampPartsMapSelect.h`: `vf10`/`vf28` declaradas (eran
  `fn_8051CF78`/`fn_8051F138` en la vtable) y `m18FC` pasa de `unsigned char[4]` a `float`.
- `NewCampElem_unk` (`sElems_unk`, NewCampTask.cpp) con campos: carpeta del menu de depuracion y colores
  base/base_sdw/after/after_sdw.
- Aviso previo de `poolcheck` en NewCampTask (0.0174533), ya estaba antes del lote.

## Hallazgos

- Un struct de un puntero sin constructor de copia escrito se devuelve en r3; con constructor de copia
  escrito, por puntero oculto (MWCC). El iterador de `std::vector` del juego es una clase: `std::sort` lo
  recibe por direccion.
- `std::sort(v.begin(), v.end(), Pred())`: el temporal del predicado (`stb 0`) se crea antes que los
  iteradores; con locales para begin/end se pierde ese orden (usar funciones que devuelvan el iterador).
