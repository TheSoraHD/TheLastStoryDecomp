# Cola N3 (paralelo-20261113): cerca de enlazar, tercera pasada

## Resultado por unidad

- `game/GameSaveLoadCommon`: fn_803B8B08 99,9 -> 100 % (9 -> 10/11). Los formatos `sprintf` del
  pool llevan un NUL extra (`"%s/%s\0"`) y dos funciones libres sin llamador
  (`SaveSlotPath_unk`, `SaveSysPath_unk`) ponen `"%s/save%02d.dat\0"` y `"%s/save_sys.dat\0"` en su
  sitio del `@stringBase0`. Queda fn_803B85EC 98,79 (`beq`+`b` del fn_803B83A4 inlineado en el bucle;
  probados else-if, resultado en local, `if (!...)`, ifs separados en la funcion externa: sin cambio).
- `gmk/AutomaticDoor`: vf0C 99,81 -> 100 % (17 -> 18/19). La prueba de la textura en una
  `static inline int IsTextureReady_unk(atn::Texture_unk&)` con su local `ready`: el flag comparte
  r31 con el puntero del `new` de CreateCamera_unk. Queda fn_803F53E0 93,78.
- `chr/ControlTurnQuadruped`: sin cambio. ctor 99,92 (el `stfs` de mAngle reutiliza el `frsp` de la
  comparacion; probados local antes/despues, `mAngle < 0.0f`, inline, lista de inicializacion).
- `game/PlayerManager`: sin cambio. fn_80549F14 99,66 (id y head comparten r30 en el original;
  probados ordenes de declaracion, for, ?:, unsigned).
- `game/CostumeEditChara`: sin cambio. vf14 98,90 (`addi 0x680` + `lwzx`: probados `*(p + i)`,
  puntero local, inline, `(int)i`); vf10 99,98 ademas usa `lbl_8087F580` como vista parcial de
  `MenuCommonTask::sInstance` (m1EC/m1F4 como `Vector2_unk`): pendiente, toca `MenuCommonTask.h`.
- `gmk/Brazier`: sin cambio. fn_80407D24 99,83 (mRadius en f3 por f1; probados local antes/despues,
  `y = y + r`, radio primero, temporal de centro, orden de locales) y vf40 (r27/r28 cruzados).

## Decisiones tomadas sin el usuario

- GameSaveLoadCommon: dos funciones libres no llamadas (el enlazador las quita) solo para sus
  literales; son las que el original tenia y el enlazador elimino (el pool lo prueba). ordercheck las
  da como `extra`. Revisar si se acepta este patron.

## Hallazgos

- Literales de una funcion que el enlazador elimino: una tabla `static` sin usar no los emite; una
  funcion libre no `static` sin llamador si, en su orden del fuente.
