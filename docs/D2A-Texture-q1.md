# D2A_Texture — q1

Rama local `codex/d2atexture-decomp` (2026-10-25). Recorrí las 30 funciones del split `atn/D2A_Texture.cpp`; 6/30 están al 100%. El informe global pasó de 7550 a 7556 funciones al 100% (26,18 → 26,19% de código; 43,17 → 43,26% fuzzy). `game_units.json` conserva `matching: false`.

## Resultado

- Constructor `D2A_Texture` (`0x801F94F4`): 99,13%, `NONMATCHING`; queda una diferencia de asignación de GPR en los bucles.
- Destructor (`0x801F97A8`): 100%; `vf0C` (`0x801F9860`): 97,96%; `vf14` (`0x801F9E6C`): 100%.
- Helper UV/escala (`0x801F9E70`): 100%; `vf18` (`0x801F9FB0`): 1,50%, con la ruta de dibujo y teselado todavía parcial.
- Funciones auxiliares de carga (`0x801FD5A0`, `0x801FDA20`): placeholders `NONMATCHING` que devuelven 0; no representan la carga real y deben sustituirse antes de enlazar la unidad.
- Layout: constructor `0x801FDE94` al 100%; `fn_801FE090` al 99,59%; destructor al 51,28%; actualización de valores `fn_801FEF40` al 95,37%. Los helpers de búsqueda, setters y eliminación se recorrieron y conservan `NONMATCHING` cuando no alcanzan el 100%.
- `fn_801FD530` y `fn_801FDE90` al 100%.

Validación: build limpio con `main.dol: OK`; `regress.py` lost: 0; `fuzzdrop.py` 0 bajadas; `poolcheck.py` en `atn/D2A_Texture`, `atn/D2A_Data`, `atn/D2A_Common` y `atn/D2Anime_unk`: 0 hallazgos. Sin asm, `#pragma` ni excepción nueva de regla 2. No se subió la rama.

## Decisiones a revisar

1. El layout de `D2A_Texture` se modeló como dos grupos `D2A_Anim_unk<4>`, dos `Texture_unk` y ocho punteros de expresión. Los offsets y el tamaño concuerdan con los usos observados; los nombres semánticos de los dos grupos siguen inferidos.
2. Añadí `mTextureAnims` en `D2A_Layout_unk +0x18`, separado de `mAnims4` en `+0x24`. Los accesos de `fn_801FE99C` y el registro de `D2A_Texture` apuntan al primer array; validar que su nombre y tipo sean adecuados.
3. Los símbolos `0x801F97A8`, `0x801F9860`, `0x801F9E6C` y `0x801F9FB0` se renombraron a destructor/virtuales de `D2A_Texture` según sus cuerpos y referencias de vtable.
4. `fn_801FD5A0` y `fn_801FDA20` siguen siendo placeholders. No cambiar `matching` a `true` hasta reconstruirlas, completar `vf18` y llevar todas las funciones al 100%.
