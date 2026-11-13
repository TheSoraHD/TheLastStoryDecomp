# Cola N3 (paralelo-20261113): cerca de enlazar, tercera pasada

## Resultado por unidad

- `game/GameSaveLoadCommon`: fn_803B8B08 99,9 -> 100 % (9 -> 10/11). Los formatos `sprintf` del
  pool llevan un NUL extra (`"%s/%s\0"`) y dos funciones libres sin llamador
  (`SaveSlotPath_unk`, `SaveSysPath_unk`) ponen `"%s/save%02d.dat\0"` y `"%s/save_sys.dat\0"` en su
  sitio del `@stringBase0`. Queda fn_803B85EC 98,79 (`beq`+`b` del fn_803B83A4 inlineado en el bucle;
  probados else-if, resultado en local, `if (!...)`, ifs separados en la funcion externa: sin cambio).
- `gmk/Brazier`: sin cambio. fn_80407D24 99,83 (mRadius en f3 por f1; probados local antes/despues,
  `y = y + r`, radio primero, temporal de centro, orden de locales) y vf40 (r27/r28 cruzados).

## Decisiones tomadas sin el usuario

- GameSaveLoadCommon: dos funciones libres no llamadas (el enlazador las quita) solo para sus
  literales; son las que el original tenia y el enlazador elimino (el pool lo prueba). ordercheck las
  da como `extra`. Revisar si se acepta este patron.

## Hallazgos

- Literales de una funcion que el enlazador elimino: una tabla `static` sin usar no los emite; una
  funcion libre no `static` sin llamador si, en su orden del fuente.
