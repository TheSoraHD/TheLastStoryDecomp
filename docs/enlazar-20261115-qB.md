# enlazar-20261115, cola B (juego, varias unidades a una funcion)

Inicio 13:22 (servidor reiniciado a las 13:43; seguido desde el estado de la copia).

## Resultado por unidad

- `db/CharaIdTable_unk`: fn_80208BE8 99,62 -> 100 % (15/16 -> 16/16); **enlaza**.
- `game/SystemErrorMessage_unk` fn_803646C4: 99,86 % sin cambio (5 variantes del bucle de la fuente del OS: y fuera
  del for, `i * 30 + 50`, do/while, orden de incrementos, `int i`).
- `game/Em201ZanGurg` ctor: 99,77 % sin cambio (`name = std::string(...)`, dos en una declaracion, literal en local,
  `info->m2C` en local). El mismo patron (std::string dir/name + atn::Csv) esta al 99,8 % en Em201ZanGurgFinal,
  Em202SpiderBase, Em204Mulue, Em308InvertCrab.
- `game/GameSaveLoadCommon` fn_803B85EC: 98,79 % sin cambio (cuerpo de fn_803B83A4 anidado, else vacio, bool,
  ternario, goto; inline escrita a mano en el bucle en 4 formas: 98,67 %).
- `game/CrossbowMenuTask` fn_801E9544: 99,82 % sin cambio (continue, defs en local, while, puntero, if anidado).
- `db/MapEnvSeTable_unk` fn_80214F80: 99,21 % sin cambio (operator[], mData[j] dos veces, referencia).
- `game/SystemErrorMessage_unk` (segunda pasada): `i` compartida por los dos bucles, cuerpo de
  DrawFontMessage_unk escrito en la funcion (99,57), puntero `*(... + i)`, `lang` local (98,88), `y = 50, i = 0`
  (99,87, no se guarda), 6 ordenes de declaracion, indice de linea pasado a DrawString_unk (98,4), `y < 170`.
  Con `i * 30 + 50` el orden de los `li` (i, offset, y) ya es el del original (99,82) pero los colores siguen
  invertidos (i r29 / offset r28 / y r27 frente a r27/r29/r28), con 11 ordenes de declaracion.
- `db/MemberList_unk` fn_80217D9C: 98,39 % sin cambio (tabla local, referencia al ultimo, first/last locales,
  `n = count - 1`, `i <= count - 1`, `sCount` en el bucle, `level <` invertido).
- `game/SelectMember` ctor: 99,64 % sin cambio; 108 combinaciones (lista antes/despues/literal, `1 + i`/`i + 1`/
  `(&mAnims[1])[i]`, for/while/do/1..8, int/unsigned/short) y temporal/unsigned: ninguna cambia r30/r31.
- `MP/D2AMpMenuSetRule` fn_804CECEC: 99,82 % sin cambio (una sola variable `text` reasignada en todas las llamadas).
- `game/GameSaveBanner` fn_803B9638: 99,18 % sin cambio (tamano literal, `&`, fuente local, `shift` local, indice
  int; argumentos de NANDInitBanner: `&mC4[0]`, `this->`, puntero al banner, title/comment locales, casts).
- `game/CrossbowMenuTask` (segunda pasada): `i` compartida por los dos bucles (99,70), `inventory`/`found` arriba.
- `game/BattleResult` fn_800F0D14: 99,33 % sin cambio (un solo contador para los tres bucles, int/unsigned).
- `game/MapManager` FindArea: 98,0 -> 98,16 % con el primer bucle sin `end` local (como el segundo); no se guarda
  (quedan la recarga de mAreaIndex y los registros del resto).
- `game/EnemyLizerdBlade` fn_80334460: el clamp `bge/b/fmr` sale con una inline `Min_unk(1.0f, t)` de dos returns
  (truco 42), pero el 1.0f usa otra constante y el resto no cambia (93,39); ademas `poolcheck` ya da floats de mas
  en su `.sdata2` (0.5, 0.7, 15, 2.44, 20): la unidad no enlazaria aunque la funcion llegara al 100 %. No se guarda.
- `db/TreasureBoxTable_unk` fn_8021E5A4: 98,91 % sin cambio (`(int)flags` en el primer bucle 96,9, parametro int /
  `const&` / `unsigned long`, producto invertido). Las mascaras salen en orden inverso al original en los dos bucles.
- Tercera pasada (barridos automaticos): MapEnvSeTable 20 combinaciones de bucle exterior/interior (mejor la
  actual), CrossbowMenuTask 54 (inventario fuera/dentro/inicializado, indice/puntero/clave, int/unsigned, orden
  de la comparacion): ninguna pasa de 99,82; Em201ZanGurg: copy-init, `const std::string`, `static const char*`.
- Sin intentar (ya documentadas o con datos que no enlazan): `db/ArrowSet_unk` (push_back de vector.h compartido),
  `db/WeaponParam_unk` (`__pp` de boost::token_iterator, plantilla de cabecera), `MP/CryptMP` (double 0.0 en .bss),
  `game/StrapTask` (registros FPR), `game/PickTask`, `game/CrowdSimulation`, `game/MapPartsRain` (poolcheck 1),
  `game/Em201ZanGurg` (ademas poolcheck 2).

## Unidades que enlazan (bytes)

- `db/CharaIdTable_unk`: 192 bytes de datos (`total_data`).

## NONMATCHING con motivo

(ver Resultado por unidad; comentarios de los fuentes sin cambiar)

## Cambios de splits

Ninguno.

## Para el port

Nada.

## Decisiones tomadas sin el usuario

Ninguna.

## Hallazgos

- Dos resultados de un mismo contador en registros distintos (r27 y r29) donde el original usa uno: el original
  reutiliza como posicion de escritura la variable de tamano del bucle anterior (`wideSize = 0;` tras el `new`),
  no una variable nueva (CharaIdTable_unk fn_80208BE8).

- Inline de dos returns `if (a < b) return a; return b;` reproduce el `bge X; b Y; X: fmr` de un minimo de floats
  aunque `MIN`, el ternario y los `if` con local den `ble` (ya en trucos-mwcc linea 42; confirmado en LizerdBlade).
- Unidades de la cola con `poolcheck` sucio (no enlazarian con todas sus funciones al 100 %): Em201ZanGurg (2),
  MapPartsRain (1), EnemyLizerdBlade.

## Pendientes

- Patron comun de 5 ctores al 99,8 % (`std::string dir(...)`, `std::string name(...)`, `atn::Csv csv(info->m2C, ...)`:
  Em201ZanGurg, Em201ZanGurgFinal, Em202SpiderBase, Em204Mulue, Em308InvertCrab): el literal del segundo string y su
  objeto cambian r26/r27. Si se resuelve en `std/string.h` sube en las cinco (re-emparejado en lote).
