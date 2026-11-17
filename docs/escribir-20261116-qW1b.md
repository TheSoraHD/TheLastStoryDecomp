# escribir-20261116, cola W1b (game, interfaz y sistema, segunda pasada)

- Inicio: 2026-11-17 02:00 (CET).

## Tabla de la cola

| Funcion | Antes | Despues |
|---|---|---|
| GameScene::vf14 (0x803C221C, dibujo de depuracion, 5192 B) | DRAFT stub vacio, 0,08 % | **100 %** |
| GameScene::fn_803CC5E4 (personaje de un punto) | 87,3 % | **100 %** |
| GameScene::fn_803CCCC0 (pares en cola) | 92,4 % | **100 %** |
| GameScene::fn_803CCA84 (posicion de un punto) | 96 % | **100 %** |
| TutorialPopup::vf10 | 67 % (escrita desde m2c, no equivalente: faltaban la espera del caso 1 y el salto de las lineas de tipo 1) | **100 %** |
| TutorialPopup::vf0C | 77,8 % (no equivalente: seguia tras la carga) | **100 %** |
| TutorialTask::vf0C | 91,2 % | equivalente NONMATCHING 99,8 % (registros) |
| TutorialTask::fn_805BA934 (arranque de la pagina) | 73,5 % (escrita desde m2c) | equivalente NONMATCHING 98 % |
| TutorialPopup::fn_805BCF04 (csv de las lineas) | 90 % (escrita desde m2c) | equivalente NONMATCHING 98,5 % (registros) |
| TutorialTask::fn_805BB654 (csv de las paginas) | 83,7 % (escrita desde m2c, sin pasar a minusculas) | equivalente NONMATCHING 90,4 % |
| TutorialTask::fn_805BA354 (inicio) | 92,4 % | **100 %** |
| GameScene::fn_803C0F48 (warp) | 93,2 % | equivalente NONMATCHING 99 % (registros) |
| TutorialTask::fn_805BC1C0 (camara libre) | equivalente 96,3 % | equivalente NONMATCHING 99 % (registros) |

## Para el port
- TutorialTask.cpp: `extern "C" fn_80097A9C(void* model, ...)` -> `atn::Model::fn_80097A9C` (via CharaTask::GetModel_unk).
- TutorialPopup::vf10 (codigo de m2c no equivalente -> el del original): el caso 1 espera al final de la animacion de la linea (D2A_Task::IsEnd_unk) antes de pasar al 2; en el 2 las lineas de mType 1 se cierran tambien cuando el personaje falta o tiene mFlag6, o con CharaTask::HasC48_unk. Cambio de comportamiento.
- TutorialPopup::vf0C: tras la carga del csv devuelve 0 en ese mismo frame (antes seguia con la inicializacion). Cambio de comportamiento.
- GameScene::vf14: stub vacio -> codigo real (dibujo de depuracion de puntos, enlaces y cajas de la escena con DrawHelper si GameConfig_unk::mSceneDebug.mEnabled; al final fn_8008CD60 de los modelos restringidos mDC, que tambien corre sin depuracion). Cambio de comportamiento.
- GameConfig_unk: los 12 int de 0x214-0x240 (mSceneDebugEnabled, mMovePoint ... mBgmBox) -> un struct SceneDebugConfig_unk mSceneDebug (mEnabled, mMovePoint ...); vf14 lo copia entero a un local. Solo GameConfig_unk.cpp los usaba.
- GameScene.cpp: vista local Settings_unk de lbl_8087F0A8 -> GameConfig_unk (m5B8 = mTitle.m34, m190 = mLoadTownSubScript).

- atn::Video (include/atn/VideoTHP.h): `float m14; float m18;` -> `Vector2_unk mPos;` (0x14, copia paired-single en TutorialTask fn_805BA934); VideoTHP.cpp usa mPos.x/mPos.y (sin cambios de codigo, enlaza).

- TutorialTask::fn_805BB654: el nombre del video (sin extension) se pasa a minusculas (std::transform con tolower de MSL) antes de buscarlo en sMovies; antes se comparaba tal cual. Cambio de comportamiento.

## Decisiones tomadas sin el usuario
- TutorialTask.cpp incluye `<ctype.h>` (tolower de MSL) y una inline LowerCase_unk(last, out, first) con la forma de std::transform (no hay `<algorithm>` ni begin() no const en std/string.h).
- Dos inlines inventadas con estaticas locales (IsSphereInView_unk: atn::Sphere en 0x807C8600, guarda 0x8087F478; IsBoxInView_unk: Frustum_unk::Bounds_unk en 0x807C8610, guarda 0x8087F479): el original comprueba la guarda en cada uso, sin ctor.

## Hallazgos
- Registros de una funcion grande con un puntero base global (RenderManager) usado en bucles: `atn::Camera& camera = instance_->mCamera;` en vez de `RenderManager* rm` (99,3 -> 99,84 %, GameScene vf14). (escribir-20261116 qW1b)
- Indice por defecto `int idx = i; if (x >= 0) idx = x;` frente a `int idx = x >= 0 ? x : i;`: el ternario cambia el orden de los registros (vf14 99,84 -> 100 %). (escribir-20261116 qW1b)

- Un Vector3 local del que solo se usa una componente (ToRotationZ_unk(...).y) cambia la pila: `float yaw = v.ToRotationZ_unk().y;` deja el temporal donde el original (TutorialTask fn_805BC1C0 98,1 -> 99 %). (escribir-20261116 qW1b)
- Bucle sobre un FixedArray_unk que el original recorre con un puntero desde `this` (desplazamiento del miembro en las cargas): `m190.mData[i].m00` en cada uso, sin referencia local ni operator[] (GameScene fn_803CCCC0 92 -> 100 %); y en SimpleArray `m80[i].mId`/`m80[i].m20` en vez de una referencia (fn_803CC5E4 87 -> 100 %). (escribir-20261116 qW1b)
- Inline que devuelve 0/1 usada en cada caso de un switch (`return Inline(...)`): MWCC pone un `li r3, 0` por caso; si el original salta a un solo `li r3, 0` final, el bucle va escrito en cada caso con `return 1;` y `break;` (GameScene fn_803CCA84 96 -> 100 %). (escribir-20261116 qW1b)
- Registro de un local que guarda un global para pasarlo como argumento (`void* font = lbl_8087EEC8;`): una inline con el global como parametro (`TextWidth_unk(lbl_8087EEC8, id)`) le da el registro del original (TutorialPopup vf0C 99,95 -> 100 %). Igual con el layout de fn_801FED24: inline `SetPaneValue_unk(layout, name, ...)` evalua el layout antes del hash. (escribir-20261116 qW1b)
- Retorno 0 compartido al final (un solo `li r3, 0`): `if (a) {...} else if (b) {...; return 1;} return 0;` en vez de dos `return 0;` tempranos (TutorialPopup vf0C). (escribir-20261116 qW1b)
- Argumentos de layout/pane (fn_801F4E8C, fn_801FED24, fn_801FEE08) que el original evalua antes del hash del nombre: inlines con el layout como parametro (GetPaneRect_unk, SetPaneValue_unk, SetPaneText_unk), como las de MP/D2AMpMenu_unk.h (TutorialTask). (escribir-20261116 qW1b)
- Condicion `a == "x" && f() != 0` con temporales `std::string` que se destruyen antes del if: el original la guarda en un `bool` (`bool ok = ... && ...; if (ok)`), sin un segundo flag (TutorialPopup fn_805BCF04 90 -> 98,5 %). (escribir-20261116 qW1b)
- Copia implicita grande (`mCamera = *camera`) seguida de una asignacion simple: escribir la simple antes (`mPage = 0;` delante de la copia) cambia el reparto de registros de toda la funcion (TutorialTask fn_805BA354 92 -> 100 %). (escribir-20261116 qW1b)
- Vector temporal construido y copiado (`dir = Vector3(0, 0, 1)` con psq_l/psq_st desde la pila): declarar sin inicializar y asignar; y un componente de un local ya cargado (`rot.y`) en vez de releer el miembro (GameScene fn_803C0F48 93 -> 99 %). (escribir-20261116 qW1b)

## Bloqueos
- GameChoiceWindow fn_803B6970/fn_803B6E04/vf10: el original deja materializados los tres bool anidados de HasVoice_unk (r28/r29/r30); probadas las inlines con `return a && b;` (sin cambio).
- TutorialTask fn_805BA934: `m5B4 = mAttackType != 0` sale con neg/or; el original lo hace con dos cntlzw. Probados `!(x == 0)`, `!!x` y una inline bool (MWCC lo pliega).
- GameScene fn_803CB618 (91 %), fn_803CAED0 (97,6 %), fn_803C1CAC (98,9 %): orden de cargas/pila; probados reordenar las referencias a/b, operator- en vez del ctor por componentes (91,4 %), bloque anidado y declarar sin inicializar en fn_803CAED0, y `mCamera != mDefaultCamera` en fn_803C1CAC.
- ReplayBuffer fn_80570A74: con las inlines reales `mEffects.begin()/end()/erase(p)` sube a 89,7 % pero desplaza dos `@N` (labelcheck peor); no commiteado. Sigue el reparto de registros (contador exterior en r31).
- GameChoiceWindow fn_803B5974/fn_803B710C: el pool ancho original (.sdata 0x8087DD80-0x8087DD98) tiene 0x18 bytes: L"" de fn_803B3D2C en +0, el de fn_803B4C38 en +8 y L"\xFF1F..." en +0xC; probado `L"\0"` en fn_803B4C38 (queda en +6). Falta saber que literales ocupan +2..+8.
- GameTask vf14: el original guarda MemoryManager* y recalcula +0x138/+0x150/+0x168 en cada llamada virtual; probados la inline con referencia y con puntero a miembro (sin cambio).
- ReplayBuffer fn_8056A9E4: las lfs de la columna z se intercalan en la copia paired-single de __MtxCopy en el original; mover las declaraciones no cambia nada.

## Cambios de splits/symbols
