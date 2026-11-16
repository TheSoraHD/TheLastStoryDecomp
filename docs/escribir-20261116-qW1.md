# escribir-20261116 cola W1 (game, interfaz y sistema)

- Inicio: 2026-11-16 23:58 (CET). Tope: 2026-11-17 02:28.

## Tabla de la cola

| funcion | antes | despues |
|---|---|---|
| MapTask::fn_804A0E18 | DRAFT (100 %) | 100 % (DRAFT quitado: igual al asm) |
| MapTask::fn_804A0E90 | DRAFT (100 %) | 100 % (DRAFT quitado: igual al asm) |
| GameChoiceWindow fn_803B3D2C (etiquetas `$X[n]`) | DRAFT 0,3 % | equivalente NONMATCHING 99,5 % |
| GameChoiceWindow::fn_803B4C38 (opciones) | DRAFT 0,8 % | equivalente NONMATCHING 88,6 % |
| GameMessageWindow::fn_803B5974 (textos) | DRAFT 0,2 % | equivalente NONMATCHING 99,99 % |
| GameTextMessageWindow::vf10 | DRAFT 0,25 % | equivalente NONMATCHING 91,3 % |
| GameTextMessageWindow::fn_803B6970 | DRAFT 0,5 % | equivalente NONMATCHING 89,8 % |
| GameTextMessageWindow::fn_803B6E04 | DRAFT 0,5 % | equivalente NONMATCHING 88,4 % |
| GameTextMessageWindow::fn_803B710C | DRAFT 0,2 % | equivalente NONMATCHING 99,99 % |
| GameMessageWindow::vf10 (efecto colateral, pool) | 99,86 % | 100 % |

## Para el port

- GameChoiceWindow (stubs -> codigo real, cambio de comportamiento): fn_803B3D2C sustituye `$V[n]` (valor), `$I[n]` (nombre de objeto, fn_80211480), `$T[n]` (texto del primer objeto de la tabla, fn_8021E444/fn_80444CF8), con `[Ln]`/`[Gn]` leidos de los contadores locales/globales de GameTask; ahora devuelve `int` (1). fn_803B4C38 reparte el texto en las cuatro opciones; fn_803B5974/fn_803B710C ponen nombre y tres lineas (ajuste de linea fn_800DFB40 con textWidth/fontSize_text, por defecto 640/20); vf10/fn_803B6970/fn_803B6E04 ya hacen la cola de mensajes, la voz (GameVoice) y abren la ventana de opciones.
- GameTextMessageWindow: `int m4C` -> `GameMessage* mMessage`; 0x7C-0xB3 tiene la forma de un GameMessage (se pasa como `(GameMessage*)&m7C` a GameMessageWindow::fn_803B5774). Nuevos inline Start_unk/Choose_unk/Pop_unk.
- Texto del hablante desconocido: L"\xFF1F\xFF1F\xFF1F" (tres signos de interrogacion de ancho completo); `wchar_t lbl_8087F468[1]` (.sbss, texto vacio).
- `std::basic_string::replace(pos, n, const basic_string&)` nuevo, definido en `include/std/string_replace_unk.h`.

## Decisiones tomadas sin el usuario

- `__declspec(noinline)` en GameTextMessageWindow::fn_803B6970: el original la llama con bl desde vf10 y MWCC la inlinea.
- La definicion de `replace(pos, n, const basic_string&)` va en una cabecera aparte: en `std/string.h` renumeraba `@enum$` de chr/ControlAppear (lost 2).

## Hallazgos

- Una inline nueva (con locales) en `std/string.h` desplaza el contador de `@enum$NNNN<fichero>` de las unidades que la incluyen y cambia los nombres mangled de sus plantillas (chr/ControlAppear lost 2): definirla fuera de la clase en una cabecera propia, como string_find_last_of.h. (escribir-20261116 qW1)

## Bloqueos

- GameTextMessageWindow HasVoice_unk: el original guarda tres bools (li 0 al principio y li 1 de cada uno); `return a && b`, `bool r = false; if (...) r = true;` y `bool r = a && b;` en inlines anidadas dan el mismo codigo (MWCC pliega los dos interiores).

## Cambios de splits/symbols

- symbols.txt: pd.sh de game/GameChoiceWindow (30 etiquetas `@N` de .sdata2/.rodata renombradas; lbl_80885C38/3C/48/4C/50 pasan a `@N` locales: son literales).
