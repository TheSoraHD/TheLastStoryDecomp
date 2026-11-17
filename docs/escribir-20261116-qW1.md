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
| ReplayBuffer fn_8056A9E4 (pose de la repeticion) | DRAFT 13,9 % | equivalente NONMATCHING 97,4 % |
| TutorialTask::fn_805BC1C0 (camara libre) | DRAFT 2,5 % | equivalente NONMATCHING 96,3 % |
| GameScene::fn_803CB820 (interacciones) | DRAFT 0,2 % | equivalente NONMATCHING 99,5 % |
| GameTask::vf14 | DRAFT 96,3 % | equivalente NONMATCHING 98,2 % (casos 1/3 antes del 0, `!(a != x && a != y)`) |
| MapPartsArea::ParseScene | DRAFT 34 % | equivalente NONMATCHING 34 % (secuencia de lecturas/strcmp/llamadas externas igual; presupuesto de inlining) |
| GameTask::vf10 | DRAFT 95,6 % | equivalente NONMATCHING 95,6 % (revisadas las diferencias de saltos/llamadas de odiff2, no linea a linea) |

## Para el port

- GameChoiceWindow (stubs -> codigo real, cambio de comportamiento): fn_803B3D2C sustituye `$V[n]` (valor), `$I[n]` (nombre de objeto, fn_80211480), `$T[n]` (texto del primer objeto de la tabla, fn_8021E444/fn_80444CF8), con `[Ln]`/`[Gn]` leidos de los contadores locales/globales de GameTask; ahora devuelve `int` (1). fn_803B4C38 reparte el texto en las cuatro opciones; fn_803B5974/fn_803B710C ponen nombre y tres lineas (ajuste de linea fn_800DFB40 con textWidth/fontSize_text, por defecto 640/20); vf10/fn_803B6970/fn_803B6E04 ya hacen la cola de mensajes, la voz (GameVoice) y abren la ventana de opciones.
- GameTextMessageWindow: `int m4C` -> `GameMessage* mMessage`; 0x7C-0xB3 tiene la forma de un GameMessage (se pasa como `(GameMessage*)&m7C` a GameMessageWindow::fn_803B5774). Nuevos inline Start_unk/Choose_unk/Pop_unk.
- Texto del hablante desconocido: L"\xFF1F\xFF1F\xFF1F" (tres signos de interrogacion de ancho completo); `wchar_t lbl_8087F468[1]` (.sbss, texto vacio).
- ReplayBuffer fn_8056A9E4 (stub -> codigo real): guarda los callbacks mB8/m35C del modelo, pone el de giro del torso (o vacio) y vacia m35C, copia la matriz, m54 = mayor escala de los ejes, rellena las pistas (medio float / byte), quita el bit 0x40 de mFlags, fn_80097E80(1), pose del nodo "reference", CalcMatrixStack sin callback, velocidad y peso de las pistas y devuelve los callbacks. Firma: `void* data` -> `ReplayTrack_unk* tracks`.
- GameScene::fn_803CB820 (stub que devolvia 0 -> codigo real): firma `()` -> `(const Vector3* pos, const Vector3* move, CharaTask* chara)` (el llamador GameTask fn_8036DBC0 ya se los pasaba en r4-r6). Recorre las conexiones mCC de los puntos de interaccion mC4, filtra por tipo/estado, direccion y capsula (fn_8005361C) y arranca la accion del personaje (fn_80161E58, fn_80162140, fn_801698E4, fn_80162428, fn_80165F54, fn_80162720, fn_80162CF0 con el caso fijo de la escalera del mapa 2-4-1, fn_80162FE8, fn_801632D0, fn_801635B8, fn_80162A08, fn_801638A0); devuelve 1 si arranca una.
- TutorialTask::fn_805BC1C0 (stub -> codigo real): posicion libre de la camara: mira desde el personaje (y su punto de ruta mas cercano, GameScene::fn_803C1560) en la direccion de la pagina (ToRotationZ_unk de target - pos); si el rayo de 20 unidades (5 arriba) choca con el mapa (fn_8004ED34, mascara 0x80000008) gira un cuarto de vuelta (cuatro intentos). Antes solo copiaba mPos/mRot.
- atn::Model::Track_unk: `m19[3]` -> `unsigned char m19` (la copia de una pista no copia 0x1A-0x1B; atn/Model.cpp fn usa `t.m19`). ReplayChara_unk::m07 y ReplayTrack_unk::mFlag pasan a `signed char` (extsb).
- `std::basic_string::replace(pos, n, const basic_string&)` nuevo, definido en `include/std/string_replace_unk.h`.

## Decisiones tomadas sin el usuario

- `__declspec(noinline)` en GameTextMessageWindow::fn_803B6970: el original la llama con bl desde vf10 y MWCC la inlinea.
- La definicion de `replace(pos, n, const basic_string&)` va en una cabecera aparte: en `std/string.h` renumeraba `@enum$` de chr/ControlAppear (lost 2).

## Hallazgos

- Casos de un switch que solo hacen `x->MeltTask(); break;` y que el original emite antes del caso 0: escribirlos primero en el fuente (GameTask vf14). (escribir-20261116 qW1)

- Vector3::ToRotation_unk frente a ToRotationZ_unk: la de z en un local deja un `frsp` de z antes de atan2; sin `frsp` es ToRotation_unk (GameScene fn_803CB820). (escribir-20261116 qW1)
- `ColliResult_unk` declarado dentro del bucle: el original lo pone a cero al principio de la funcion (declarado arriba). (escribir-20261116 qW1)

- Una inline nueva (con locales) en `std/string.h` desplaza el contador de `@enum$NNNN<fichero>` de las unidades que la incluyen y cambia los nombres mangled de sus plantillas (chr/ControlAppear lost 2): definirla fuera de la clase en una cabecera propia, como string_find_last_of.h. (escribir-20261116 qW1)

## Bloqueos

- GameTextMessageWindow HasVoice_unk: el original guarda tres bools (li 0 al principio y li 1 de cada uno); `return a && b`, `bool r = false; if (...) r = true;` y `bool r = a && b;` en inlines anidadas dan el mismo codigo (MWCC pliega los dos interiores).

## Cambios de splits/symbols

- symbols.txt: `fn_803CB820__9GameSceneFv` -> `fn_803CB820__9GameSceneFPCQ23atn7Vector3PCQ23atn7Vector3P9CharaTask`; pdbatch de GameScene y GameTask (renumerados +1; 0x808857E0/E4 a mano: @31595/@31596).
- symbols.txt: pd.sh de game/TutorialTask.
- symbols.txt: pd.sh + lblmap.py de game/ReplayBuffer (@12998 -> @13644, @13104 -> @13412).
- symbols.txt: pd.sh de game/GameChoiceWindow (30 etiquetas `@N` de .sdata2/.rodata renombradas; lbl_80885C38/3C/48/4C/50 pasan a `@N` locales: son literales).
