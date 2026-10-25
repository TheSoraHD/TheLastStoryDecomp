# D2A_Texture — q1

Rama `codex/d2atexture-decomp` (2026-10-25). Se revisaron las 30 funciones de `atn/D2A_Texture.cpp`; 14/30 están al 100% y ninguna baja del 50%. El informe global pasó de 7550 a 7564 funciones al 100% (26,18 → 26,21% de código; 43,17 → 43,29% fuzzy). `game_units.json` conserva `matching: false`.

## Resultado

- Constructor `D2A_Texture` (`0x801F94F4`): 99,83%; dos movimientos de registros guardados mantienen orden distinto. Destructor al 100%; `vf0C` 97,96%; `vf10`, `vf14`, `vf1C` y `vf20` al 100%.
- Helper UV/escala (`0x801F9E70`): 100%. `vf18` (`0x801F9FB0`): 95,72%; reconstruye las rutas de renderizado, callback, conversión de color, contornos, teselado y mosaicos. El residuo es principalmente el orden de registros/carga de argumentos; no se añadió asm ni pragmas.
- Carga (`0x801FD5A0`, `0x801FDA20`): 56,67% y 55,69%. Reconstruyen basename, eliminación de extensión, sustitución del sufijo `_jp` por `_xx`, extensión `.texture`, selección de textura y flags de wrap. Ambas siguen `NONMATCHING` y requieren confirmación semántica.
- Layout: constructor y destructor al 100%; seis funciones de eliminación de punteros al 100%. Búsquedas por hash 97,96%; setters float 97,94–98,16%; setter int 96,76%; actualización de texto 95,57%; actualización animada 95,37%; reserva 99,59%.
- `fn_801FD530`, `fn_801FD534` y `fn_801FDE90` se renombran `vf10`, `vf1C` y `vf20` en `D2A_Texture`.

Validación: build limpio `main.dol: OK`; `regress.py` 7564/18215, lost: 0; `fuzzdrop.py` 0 bajadas; `poolcheck.py` en `atn/D2A_Texture`, `atn/D2A_Data`, `atn/D2A_Common` y `atn/D2Anime_unk`: 0 hallazgos. Para regress/fuzzdrop se usó el informe limpio disponible como referencia (el anterior se eliminó al hacer clean), por lo que la comparación no detecta regresiones fuera de esta unidad. No se añadió asm, `#pragma` ni excepción nueva de regla 2.

## Revisión de tipos y símbolos

- La vtable `__vt__Q23atn11D2A_Texture` tiene los slots `vf0C`, `vf10`, `vf14`, `vf18`, `vf1C` y `vf20` en los offsets esperados. `vf20` (`0x801FDE90`) es un salto a `fn_801EDCB8`; los nombres de los tres slots renombrados quedan confirmados.
- `D2A_Texture` contiene dos `D2A_Anim_unk<4>`, dos `Texture_unk` y dos arrays de cuatro `D2A_Expr_unk`. Sus offsets concuerdan con el constructor y el helper UV. Los nombres `mTexRect`, `mTexScale`, `mRectCode` y `mScaleCode` son inferidos por sus usos.
- `D2A_Layout_unk` contiene arrays en `+0x00`, `+0x0C`, `+0x18`, `+0x24`, `+0x30` y `+0x3C`. Los registros/eliminadores de `D2A_Texture` y los recorridos de actualización confirman `mTextureAnims` en `+0x18`, separado de `mAnims4` en `+0x24`.

## Decisiones a revisar

- Confirmar que los dos cargadores normalizan el basename, quitan la extensión, convierten `_jp` a `_xx` y añaden `.texture` tal como sugieren sus literales y el asm. Las firmas de los llamadores existentes pasan también enteros en el segundo argumento, lo que deja ambigua la semántica pública.
- Confirmar los nombres semánticos inferidos para los dos juegos de animaciones/expresiones y los flags de wrap `m2D`/`m2E`.
- `vf18` todavía no está al 100% (95,72%); el umbral actual de 50% sí se cumple en las 30 funciones.

## Pendiente

Quedan 16 funciones por igualar, incluida `vf18`; las dos cargas están por encima del 50% pero no están completamente validadas. No marcar la unidad `matching: true` hasta alcanzar 100% en las 30 funciones y verificar el enlazado.
