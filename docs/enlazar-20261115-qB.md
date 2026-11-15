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

## Pendientes
