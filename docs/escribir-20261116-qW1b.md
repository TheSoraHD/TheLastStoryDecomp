# escribir-20261116, cola W1b (game, interfaz y sistema, segunda pasada)

- Inicio: 2026-11-17 02:00 (CET).

## Tabla de la cola

| Funcion | Antes | Despues |
|---|---|---|
| GameScene::vf14 (0x803C221C, dibujo de depuracion, 5192 B) | DRAFT stub vacio, 0,08 % | equivalente NONMATCHING 99,3 % (solo registros) |

## Para el port
- GameScene::vf14: stub vacio -> codigo real (dibujo de depuracion de puntos, enlaces y cajas de la escena con DrawHelper si GameConfig_unk::mSceneDebug.mEnabled; al final fn_8008CD60 de los modelos restringidos mDC, que tambien corre sin depuracion). Cambio de comportamiento.
- GameConfig_unk: los 12 int de 0x214-0x240 (mSceneDebugEnabled, mMovePoint ... mBgmBox) -> un struct SceneDebugConfig_unk mSceneDebug (mEnabled, mMovePoint ...); vf14 lo copia entero a un local. Solo GameConfig_unk.cpp los usaba.
- GameScene.cpp: vista local Settings_unk de lbl_8087F0A8 -> GameConfig_unk (m5B8 = mTitle.m34, m190 = mLoadTownSubScript).

## Decisiones tomadas sin el usuario
- Dos inlines inventadas con estaticas locales (IsSphereInView_unk: atn::Sphere en 0x807C8600, guarda 0x8087F478; IsBoxInView_unk: Frustum_unk::Bounds_unk en 0x807C8610, guarda 0x8087F479): el original comprueba la guarda en cada uso, sin ctor.

## Hallazgos

## Bloqueos

## Cambios de splits/symbols
