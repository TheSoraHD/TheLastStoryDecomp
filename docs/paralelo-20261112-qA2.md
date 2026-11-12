# Lote paralelo 2026-11-12, cola A2 (Tiendas, segunda pasada)

Inicio: 2026-11-12 21:32 (CET). Rama `agent-A2`.

## Resultado por unidad

- `game/ShopBase`: funciones al 0 % **6 -> 0**. El `std::sort` de la lista (0x80582598-0x80584174) usa
  `game/MslSort_unk.h` con `Iter = ShopEntry5C_unk*` y el predicado nuevo `ShopEntryLess_unk::operator()`
  (0x805828B0). Al 100 % nueva: `MslSwap_unk<ShopEntry5C_unk>` (0x80582EFC). NONMATCHING: sort 95,9 %
  (x2, codigo de la cabecera), predicado 98,5 %, mediana 98,9 %, seleccion 94,3 %.

## NONMATCHING (motivo)

- `ShopEntryLess_unk::operator()`: `L"%s"` es un `@STRING@` debil aqui (miembro inline) y parte de
  `@wstringBase0` (+0x12) en el original, como en ShopUpGrade; registros de los nombres en la rama de los
  objetos nuevos (r25 frente a r30). Probado definir el `operator()` fuera de linea (no inline) tras
  fn_80581FD8: la cadena entra en el pool, pero cambia el orden y el inlining (sort se come el predicado).
- Sort/seleccion: diferencias de planificacion y registros del codigo de `MslSort_unk.h` (cabecera de la cola B).

## Pendientes

- `include/game/MslSort_unk.h` (cola B): `static int sOffset;` deberia ser `static int sOffset = -4;`
  (en el original todos los contadores estan en `.sdata` con 0xFFFFFFFC: ShopBase lbl_8087E688/8C,
  CosEditItemList_unk 0x8087DA70..). No lo toco (cabecera ajena); afecta a Event/Node, NewCampPartsItem,
  CosEditItemList_unk y ShopBase (ninguna enlazada).

## Decisiones tomadas sin el usuario

- `symbols.txt`: solo renombrados de las 6 funciones del sort de ShopBase y de sus dos contadores
  (`@LOCAL@MslSort_unk<...>@sOffset`).
- `ShopWeapon_unk` (local de ShopBase.cpp) gana `m78` (clave de orden de las armas), sin cambiar el tamano.

## Hallazgos

- En el predicado, las comparaciones `a < b` de enteros salen como `xor/srawi/and/subf/srwi`; el orden de los
  registros en el `xor` distingue `a.x < b.x` de `a.x > b.x`.
