# escribir-20261116, cola W1b (game, interfaz y sistema, segunda pasada)

- Inicio: 2026-11-17 02:00 (CET).

## Tabla de la cola

| Funcion | Antes | Despues |
|---|---|---|
| GameScene::vf14 (0x803C221C, dibujo de depuracion, 5192 B) | DRAFT stub vacio, 0,08 % | **100 %** |
| GameScene::fn_803CC5E4 (personaje de un punto) | 87,3 % | **100 %** |
| GameScene::fn_803CCCC0 (pares en cola) | 92,4 % | **100 %** |
| TutorialTask::fn_805BC1C0 (camara libre) | equivalente 96,3 % | equivalente NONMATCHING 99 % (registros) |

## Para el port
- GameScene::vf14: stub vacio -> codigo real (dibujo de depuracion de puntos, enlaces y cajas de la escena con DrawHelper si GameConfig_unk::mSceneDebug.mEnabled; al final fn_8008CD60 de los modelos restringidos mDC, que tambien corre sin depuracion). Cambio de comportamiento.
- GameConfig_unk: los 12 int de 0x214-0x240 (mSceneDebugEnabled, mMovePoint ... mBgmBox) -> un struct SceneDebugConfig_unk mSceneDebug (mEnabled, mMovePoint ...); vf14 lo copia entero a un local. Solo GameConfig_unk.cpp los usaba.
- GameScene.cpp: vista local Settings_unk de lbl_8087F0A8 -> GameConfig_unk (m5B8 = mTitle.m34, m190 = mLoadTownSubScript).

## Decisiones tomadas sin el usuario
- Dos inlines inventadas con estaticas locales (IsSphereInView_unk: atn::Sphere en 0x807C8600, guarda 0x8087F478; IsBoxInView_unk: Frustum_unk::Bounds_unk en 0x807C8610, guarda 0x8087F479): el original comprueba la guarda en cada uso, sin ctor.

## Hallazgos
- Registros de una funcion grande con un puntero base global (RenderManager) usado en bucles: `atn::Camera& camera = instance_->mCamera;` en vez de `RenderManager* rm` (99,3 -> 99,84 %, GameScene vf14). (escribir-20261116 qW1b)
- Indice por defecto `int idx = i; if (x >= 0) idx = x;` frente a `int idx = x >= 0 ? x : i;`: el ternario cambia el orden de los registros (vf14 99,84 -> 100 %). (escribir-20261116 qW1b)

- Un Vector3 local del que solo se usa una componente (ToRotationZ_unk(...).y) cambia la pila: `float yaw = v.ToRotationZ_unk().y;` deja el temporal donde el original (TutorialTask fn_805BC1C0 98,1 -> 99 %). (escribir-20261116 qW1b)
- Bucle sobre un FixedArray_unk que el original recorre con un puntero desde `this` (desplazamiento del miembro en las cargas): `m190.mData[i].m00` en cada uso, sin referencia local ni operator[] (GameScene fn_803CCCC0 92 -> 100 %); y en SimpleArray `m80[i].mId`/`m80[i].m20` en vez de una referencia (fn_803CC5E4 87 -> 100 %). (escribir-20261116 qW1b)

## Bloqueos
- ReplayBuffer fn_80570A74: con las inlines reales `mEffects.begin()/end()/erase(p)` sube a 89,7 % pero desplaza dos `@N` (labelcheck peor); no commiteado. Sigue el reparto de registros (contador exterior en r31).
- GameChoiceWindow fn_803B5974/fn_803B710C: el pool ancho original (.sdata 0x8087DD80-0x8087DD98) tiene 0x18 bytes: L"" de fn_803B3D2C en +0, el de fn_803B4C38 en +8 y L"\xFF1F..." en +0xC; probado `L"\0"` en fn_803B4C38 (queda en +6). Falta saber que literales ocupan +2..+8.
- GameTask vf14: el original guarda MemoryManager* y recalcula +0x138/+0x150/+0x168 en cada llamada virtual; probados la inline con referencia y con puntero a miembro (sin cambio).
- ReplayBuffer fn_8056A9E4: las lfs de la columna z se intercalan en la copia paired-single de __MtxCopy en el original; mover las declaraciones no cambia nada.

## Cambios de splits/symbols
