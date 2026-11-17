# escribir-20261116, cola W4 (atn, chr, gmk, AI y SDK)

Agente W4, inicio 2026-11-17 02:08 (tope 04:38). Rama `agent-W4`.

## Tabla de la cola

| Funcion | Antes | Despues |
|---|---|---|
| atn/SmallAllocator_8 `~SmallAllocator<6>` (0x80082B54) | 0 % (escrita, inlinea el ctor del gestor) | igual: equivalente NONMATCHING 0 % (ciclo de inlining, ver Bloqueos) |
| atn/SmallAllocator_8 `operator delete` (0x80084684) | 0 % (escrita) | igual: equivalente NONMATCHING 0 % (mismo ciclo) |
| atn/LayerOcclusionCPU `fn_800B4390` | 0 % (C sin comprobar) | equivalente NONMATCHING 0 % (asm a mano; comprobada contra el asm) |
| atn/WordWrap_unk `ios_base::setstate`, `basic_string(const basic_string&)`, thunk `@8@60@~basic_ostream` | no emitidas | igual (ver Bloqueos) |
| atn/LH_Binary `fn_80471FD0` | no escrita | equivalente NONMATCHING 0 % (paired-single sin GQR6) |
| atn/LH_Binary `SceneTrack_unk::fn_8047202C` | no escrita | equivalente NONMATCHING 0 % |

## Para el port

- `SceneTrack_unk` pasa de `unsigned char m00[0xC]` a campos (`m00`, `mType`, `mCount`, `mScale`, `mKeys`) en
  la cabecera nueva `include/atn/SceneTrack_unk.h` (la incluye `atn/Light_unk.h` donde estaba el struct);
  `BinTrack_unk` de LH_Binary es ahora un typedef de `SceneTrack_unk`. Parametros de `fn_8047202C`
  renombrados `(const short& key, float frame)` (el `short` es la cache del indice de clave; se escribe).
- `SceneTrack_unk::fn_8047202C` y `fn_80471FD0` pasan de no existir a codigo real (evaluacion de pistas:
  constante, lineal, una muestra por frame y hermite). Los valores s16 se escalan por `2^-mScale` (el
  original lo hace con GQR6); `fn_80471FD0` recibe la escala como sexto argumento. Base de hermite
  `lbl_8078FBF0` = {2, 1, -3, -2, -2, 1, 3, -1} definida en LH_Binary.
- LayerOcclusionCPU `fn_800B4390`: misma semantica; la distancia al plano se suma en el orden del
  original y ya no multiplica `mDist` por `lbl_807790A8[1]` (el 1 no se usa en el original).

## Decisiones tomadas sin el usuario

## Hallazgos

## Bloqueos

## Cambios de splits/symbols

- LH_Binary (pd.sh): `lbl_80755C20` -> `@5283`, `@4836` -> `@5020`, `lbl_80886EE0/EE4` -> `@5277`/`@5390`.
