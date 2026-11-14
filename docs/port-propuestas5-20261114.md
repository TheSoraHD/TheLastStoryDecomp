# port-propuestas5-20261114: arreglos del port de PC (fase 3) que valen para la decomp

Rama desde `main` (`efc67f6`, 15389 / 18109). Salen de la fase 3 del port (`TheSoraHD/TheLastStory-PortTest`,
rama `fase3-20261113`, notas en `docs/port-fase3-notas.md` de ese repo). Comprobado con build completo:
`main.dol: OK`, `regress.py` lost 0 y `fuzzdrop.py` sin bajadas contra el `report.json` de `main`.

## Cambios

1. **Vistas locales `CollResult_unk`** (39 unidades): los campos `m34`, `m38` y `m40` que la vista declaraba
   `int` y la unidad nunca lee pasan a `void*`. En `atn::ColliResult_unk` (`atn/ColliBase.h`) son punteros; en
   MWCC el código es el mismo (solo los ceros del constructor), y en PC la vista tiene así el tamaño real
   (0x68): con `int`, la consulta de colisión desbordaba la pila al andar. Los campos que alguna unidad lee
   como entero se dejan (lista en las notas del port); lo limpio sería una sola definición.
2. **`AIScriptMapNode_unk::mParentColor`** (`AI/AI_archer_attack01.cpp`): `__msl_size_t` en vez de
   `unsigned long` (el mismo tipo en la Wii). Guarda un puntero con el color en el bit 0: en Windows
   `unsigned long` mide 32 bits.
3. **`fn_8009D58C`** (`atn/ModelDraw_unk.cpp`): las máscaras de pasada se leen de la capa real
   (`atn::IRenderLayer::m0C/m10`, `atn/Layer3D.h`) en vez de la vista `DrawPass_unk` (regla 10). Con el
   `(unsigned int)` de la vista el fuzzy no cambia (83,59 %); sin él bajaba a 83,53. `DrawPass_unk` queda
   solo para `m7C` (el ojo de `LayerShadow`).

## Lo que no se ha subido (y por qué)

- La palabra de estado GX del material (`fn_80076A28(rs, (char*)mat + 0x14)`), el proyector de luz
  (`TexProjector_unk` = `CharaLightData_unk`) y las texturas de `LayerShadow` (`RenderTextures_unk`): cambian
  el código o necesitan mover clases a cabeceras; sin probar el matching.
- `ModelNodeDesc_unk::mName` como `PTR32`: la decomp no tiene `port_types.h`.
- Unificar `IRender` (`atn/PostFx.h`, `LensFlare.h`) y la firma de 4 argumentos de `ModelBase::vf14`.
