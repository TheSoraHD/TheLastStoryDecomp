# Lote paralelo 2026-11-12, cola A2 (Tiendas, segunda pasada)

Inicio: 2026-11-12 21:32 (CET). Rama `agent-A2`.

## Resultado por unidad

- `game/ShopBase`: funciones al 0 % **6 -> 0**. El `std::sort` de la lista (0x80582598-0x80584174) usa
  `game/MslSort_unk.h` con `Iter = ShopEntry5C_unk*` y el predicado nuevo `ShopEntryLess_unk::operator()`
  (0x805828B0), este en un namespace anonimo. Al 100 % nueva: `MslSwap_unk<ShopEntry5C_unk>` (0x80582EFC).
  NONMATCHING: sort 95,9 % (x2, codigo de la cabecera), predicado 99,93 %, mediana 99,92 %, seleccion 99,4 %.
- `game/ShopUpGrade`: funciones al 0 % **2 -> 0** (`__cl__..LessC_unk` 0x80590448 y `SortSelect_unk<..LessC..>`
  0x80591504). Al 100 % **30 -> 43 / 57**: predicados LessB/LessC/LessD, medianas LessA/B/C/D (las 4 de
  `L"%s"` que `report.json` ya daba al 100 %) y los seis `Sort_unk` de LessB/C/D (LessC desde 0 %).
  Sort LessA 96,6 -> 98,3 %, seleccion 93,4 -> 98,9 %.

## NONMATCHING (motivo)

- `L"%s"` de los predicados: con el predicado en un **namespace anonimo** (enlace local) la cadena va al
  pool del fichero `@wstringBase0` como en el original, no a un `@STRING@` debil. Nombres en `symbols.txt`
  con `Q2<n>@unnamed@<Fichero>_cpp@...` (ShopBase y ShopUpGrade).
- ShopUpGrade `Sort_unk<..LessC..>`: el original lo compila por encima del presupuesto de inlining (llama
  al predicado y a la seleccion) y `Sort_unk<..LessA..>` no, con predicados casi iguales: seis asserts
  vacios `SHOPUP_ASSERT_UNK` en LessC (5 no bastan). Predicado y mediana con `atn::Abs_unk` y locales
  `ia`/`ib` para los nombres: al 100 % (LessA tambien).
- ShopUpGrade fn_80598530 (95,96 %): falta en `@wstringBase0` un `L""` sin referencias en +6 (entre
  `L"%s"` y `L"+99"`); el pool original es `%s, "", +99, "", ???, --, -`. No encontrado de donde sale.

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

- Predicados de ShopBase y ShopUpGrade en un namespace anonimo (cambia sus nombres mangled y los de las
  plantillas instanciadas con ellos; renombrados en `symbols.txt`).
- Asserts vacios (excepcion aprobada para el umbral de autoinline) en `ShopUpGradeLessC_unk`.

- `symbols.txt`: solo renombrados de las 6 funciones del sort de ShopBase y de sus dos contadores
  (`@LOCAL@MslSort_unk<...>@sOffset`).
- `ShopWeapon_unk` (local de ShopBase.cpp) gana `m78` (clave de orden de las armas), sin cambiar el tamano.

## Hallazgos

- `std::sort` de MSL: segundo pivote con `int n3 = n * 3 / 4;` en una sentencia aparte (como atn/Loader y
  GameTask) y `while (i != --j && ...)` (el `!=` con `i` a la izquierda): Sort_unk de ShopUpGrade al 100 %.
  `MslSort_unk.h` ya usa `i != --j` pero no `n3` (pendiente de la cola B).

- Una cadena literal en una funcion inline/plantilla con enlace externo sale como `@STRING@` debil; con la
  clase en un namespace anonimo va al pool del fichero (`@stringBase0`/`@wstringBase0`).

- En el predicado, las comparaciones `a < b` de enteros salen como `xor/srawi/and/subf/srwi`; el orden de los
  registros en el `xor` distingue `a.x < b.x` de `a.x > b.x`.
