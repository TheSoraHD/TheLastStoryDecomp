# escribir-20261116 cola W1c (game, interfaz y sistema; tercera pasada: al 100 %)

- Inicio: 2026-11-17 06:32 (CET). Tope: 09:02.

## Tabla de la cola

| Funcion | Antes | Despues |
|---|---|---|
| GameScene::GameScene (0x803BEC40) | 96,55 % | **100 %** |
| GameChoiceWindow::fn_803B4C38 (choices) | 88,63 % | equivalente NONMATCHING 99,93 % (r18/r19 y el pool ancho) |
| GameTextMessageWindow::vf10 | 91,30 % | equivalente NONMATCHING 93,98 % (bools de HasVoice_unk) |
| GameTextMessageWindow::fn_803B6970 | 89,77 % | equivalente NONMATCHING 92,35 % |
| GameTextMessageWindow::fn_803B6E04 | 88,37 % | equivalente NONMATCHING 90,97 % |
| ReplayBuffer::fn_80570A74 | 89,43 % | equivalente NONMATCHING 91,42 % (FixedArray::erase real) |
| GameMessageWindow::fn_803B5974 / GameTextMessageWindow::fn_803B710C | 99,99 % | sin cambios (pool ancho, ver Bloqueos) |
| GameChoiceWindow fn_803B3D2C | 99,46 % | sin cambios |
| TutorialTask::vf0C | 99,83 % | sin cambios (10 variantes, ver Bloqueos) |
| GameTask fn_8036DDD4 | 99,7 % | sin cambios (11 variantes) |
| GameTask::fn_8036CC90 | 99,97 % | sin cambios |
| GameScene::fn_803C1E3C | 98,23 % | sin cambios (7 -> 2 diferencias con `bgm`/`volume` locales, no commiteado) |
| MapPartsArea::vf1C | 99,90 % | sin cambios |
| MapTask::fn_804A0F80 | 5 % | sin cambios (cuerpo en una inline: 30,8 %, no commiteado) |

## Para el port

## Decisiones tomadas sin el usuario

## Hallazgos

## Bloqueos

## Cambios de splits/symbols
