# enemies-third, cola n2: Em202_01_BSpider, Em202ForestSpider, Em202MothersSpider

Rama `agent-n2`. Referencia: `/home/claude/ref/n2_start.json` (7425 funciones).

## Resultado por unidad (report.json, antes -> despues / total)

| Unidad | Antes | Despues | Total |
|---|---|---|---|
| `game/Em202_01_BSpider` (fuente nueva) | 0 | 14 | 19 |

### Em202_01_BSpider: NONMATCHING
- Constructor (99.29%): registros r7-r11 de la busqueda del punto de ruta en linea (12 formas).
- `vf0C` (89.34%): el original intercala los `ori` de los flags con las cargas de m500-m508 (10 formas).
- `vf10` (99.62%): r4/r5 del objetivo y de la copia de su posicion en el caso por defecto (8 formas).
- `vf14` (93.04%): mismo caso que `Em202SpiderBase::vf14` (huecos de pila del color).
- `fn_80271DAC` (99.76%): r4/r5 de la copia del vector cero y r29/r30 del contador (mismo caso que
  `Em202Spider::fn_80281924`).

## Decisiones tomadas sin el usuario
- `include/game/CharaStatus.h` (compartida): `RouteTable_unk::m98` pasa de `unsigned char[4]` a `unsigned int`
  (numero de puntos de `mPoints`, lo lee el ctor de BSpider). Mismo tamano y offset; nada baja.
- `symbols.txt` (BSpider): renombrados los 19 `fn_` y `fn_8027276C` -> `__sinit_\Em202_01_BSpider_cpp`;
  `lbl_8074495C` -> `@stringBase0` (local, string_table); los tres doubles de `.rodata` 0x80744930/38/40 ->
  `@6724`/`@6886`/`@6880` (locales, nombres de nuestro .o para que objdiff empareje el pool).
- Datos `.bss` 0x807C8338 (6 ints, puntos ocupados) y 0x807C8350 (6 `atn::Vector3`, su `__sinit`) como
  `static` en la unidad; la tabla de ids de puntos 0x807448D8 y la de sonidos 0x80744920 como `static const`.
- `src/game/SpecialEnemyBase.cpp`: caso 13 con `new Em202_01_BSpider(parent, data)` (quitado el extern
  `fn_8026FC0C`); el porcentaje de la factoria no cambia (73.25%).

## Hallazgos generales
- Los doubles de `.rodata` sin nombre (`lfd` de 2pi, 90.0, 0x4330...) solo empatan en objdiff si el simbolo
  del objetivo se renombra al `@NNNN` local que da nuestro .o (`scope:local`).
