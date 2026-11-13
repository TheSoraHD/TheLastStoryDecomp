# Lote paralelo 2026-11-13, cola E (vestuario: CosEditItemList_unk, CosEditProtector, NewCampTask)

Inicio: 2026-11-13 03:05 CET. Tope 05:35.

## Resultado por unidad (funciones al 0 % / sin escribir, antes -> despues)

- `game/CosEditItemList_unk`: 41 -> 3 (`fn_801D80A0`, el ctor `0x801D8C90` y `push_back` de armas
  `0x801E6384` al 99,8 %: este ultimo ya no cuenta como 0 %). Al 100 % 15 -> 50 (+35): los sorts con predicado
  fuera de linea (`EquipLess` x2, `ArmorLess` x2), su mediana de ArmorLess, los dos juegos de operadores del
  iterador (`MslIter_unk` de protectores y de armas, 0x801DBAC0-0x801DC07C y 0x801E26B4-0x801E2D68), el swap
  de armas, `size`/`[]`/`clear`/begin/end de `vector<CosEditWeaponItem_unk>`. `fn_801E07A4` 0 -> 97,2 %.

## Cambios

- Predicados `CosEditEquipLess_unk` y `CosEditArmorLess_unk` (los dos con `L"%s"`): en un namespace anonimo,
  `operator()` en la clase (como ShopBase/ShopUpGrade). Asi el literal va a `@wstringBase0` y el predicado es
  candidato a inline cuando se instancian los sorts (antes, definido fuera de linea detras de su usuario, no lo
  era: el sort quedaba dentro del presupuesto y se inlineaba todo).
- Con eso los sorts siguen dentro del presupuesto: hacen falta ~19 asserts vacios (`COSEDIT_ASSERT_UNK`) en
  cada uno de esos dos predicados para pasar R3 como el original (13 sentencias muertas tambien bastaban;
  12 no). Con 20 el sort, el predicado y la seleccion salen fuera de linea con el tamano exacto del original.
  Margen medido de los otros sorts (predicado inline, deben seguir dentro): ArmourLess aguanta 13-20
  sentencias de mas, WeaponArmourLess 12-16.
- Tipo nuevo `CosEditWeaponItem_unk` (mismos campos, sin funciones) para las listas de armas `mB4C`, `mB58` y
  `mB64` (CosEditProtector.h) y para `CosEditBase::fn_801CF3C0` (solo recibe armas). Sus copias debiles son
  otras (0x801E26B4-, swap de 132 bytes por la pila). Sobrecargas de `CosEditBegin_unk`/`CosEditEnd_unk` para
  `vector<CosEditWeaponItem_unk>` con 120 asserts vacios (`COSEDIT_ASSERT10_UNK` x6 en cada una): ponen
  `fn_801E07A4` por encima del presupuesto como el original (con 80 no basta; con 110 si) y salen sus copias
  debiles de size/[]/clear/push_back/begin/end.

## NONMATCHING

- `MslSwap_unk<CosEditItem_unk>` (0x801DBB14) 64 %: el original no pasa el temporal por la pila. Pasa si
  `CosEditItem_unk` tiene constructor por defecto (y `0x801D8C90` es justo ese constructor: mItem 0,
  mIndex/mOwner -1, mEnabled 0, llamado fuera de linea por fn_801DD26C), pero entonces `std::vector` toma el
  camino no POD en push_back (prueba de null del placement new) y bajan push_back (96,9 -> 82,7 %) y
  fn_801D94D0 (95,3 -> 92,1 %). El original toma el camino POD con el constructor: la MSL despacharia por
  copia/destructor triviales y no por `__builtin_is_pod`. Probado `(__builtin_trivial_members(T) & 7) == 7` en
  los dos push_back de `std/vector.h`: push_back 94 %, fn_801D94D0 94,1 % (siguen por debajo); revertido.
  Pendiente (cabecera de todo el proyecto).
- Mediana de EquipLess 91,5 %, seleccion de EquipLess 92,2 %, seleccion de ArmorLess 97,3 %: el orden de
  carga de `first != last` y los huecos de pila de los dos buffers del predicado inlineado (al reves).
- `fn_801E07A4` 97,2 %: falta el accesor de `Inventory::mEquip` (0x801D80A0, sin declarar) y el original usa un
  solo hueco de pila para las dos entradas.

## Decisiones tomadas sin el usuario

- Asserts vacios en los dos predicados (22 cada uno) y en begin()/end() de las listas de armas (60 cada uno)
  para el presupuesto R3 (practica aprobada: regla 2, asserts vacios para el umbral de autoinline).
- `CosEditWeaponItem_unk` nuevo en `CosEditBase.h` (la incluyen 5 unidades) y cambio de tipo de `mB4C`,
  `mB58`, `mB64`, de `fn_801CF3C0`, `fn_801DCDAC`, `fn_801DD12C`, `fn_801E7098`.
- `symbols.txt`: solo renombrados (51 entradas de CosEditItemList_unk y `fn_801CF3C0` de CosEditBase).
  `0x801D8C90` renombrado a `__ct__15CosEditItem_unkFv` aunque nuestro tipo no tiene constructor (sigue sin
  emitirse).

## Hallazgos

- Un swap `T tmp = a; a = b; b = tmp;` de un struct POD de 6 palabras pasa `tmp` por la pila; si el struct
  tiene constructor por defecto (o de copia) escrito, no (100 frente a 132 bytes).
- Las copias debiles de una inline salen junto a la primera funcion que la referencia aunque alli se inlinee.

## Pendientes

- Accesor de `Inventory::mEquip` (0x801D80A0, tambien lo llama NewCampPartsStatus con un `extern "C"`):
  declararlo en `Inventory.h` renumera 58 unidades.
- Constructor de `CosEditItem_unk` + despacho de `std::vector` por copia trivial (ver NONMATCHING).
