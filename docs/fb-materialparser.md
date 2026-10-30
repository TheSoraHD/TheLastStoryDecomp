# Propuesta F-b, primera parte: `atn/MaterialParser_unk` (opción A)

Rama `fb-materialparser`, sale de `batch-auto-50kb-atn` (6b8cf3c). 2026-10-30.

## Resultado

- **8678 → 8696** funciones al 100% (+18), `main.dol: OK`, `regress` lost: 0 y `fuzzdrop` sin bajadas contra el `report.json` del inicio.
- `atn/MaterialParser_unk`: 35 funciones escritas en orden del `.dol`; **18 al 100%**. La unidad no enlaza todavía (`matching: false`).
- Las funciones de la unidad son ahora métodos (`Material_unk`, `TexStage_unk`, `TexAnim_unk`; `TexUv_unk::s*`), renombrados en `symbols.txt`. Los llamadores (`Shader_unk`, `ModelDraw_unk`, `LH_Material`, `PassBoard`, `GimmickCommon`) usan los métodos. Siguen con `extern "C"`, por ser funciones libres: `fn_8007B078` (id de textura por nombre) y `fn_80081C34` (ruta de la textura por defecto).

| Función | % | Motivo |
|---|---|---|
| `fn_8007A994` (tokenizador) | 98,1 | registros de `s`/`delims`/`len` |
| `TexStage_unk::fn_8007AB38` | 91,7 | registros float y orden de cargas de los `Set*` |
| `TexStage_unk::fn_8007B100` | 95,2 | `obj`/`minFilter` cruzados (r30/r31) |
| `TexStage_unk::fn_8007B260` | 99,5 | registros |
| `Material_unk::Material_unk` | 87,5 | orden de los stores (la tabla de swap `m38` va en el init del original: necesita un tipo con constructor, pero eso rompe `fn_8007B260`) |
| `Material_unk::fn_8007C46C` (parser) | 97,0 | forma del bucle de la macro `FIND_NAME` (ver abajo) y registros |
| `Sort_unk` / `Swap_unk` / `SelectionSort_unk` | 95,9 / 99,4 / 98,2 | registros |
| `fn_8007FE5C` | 91,8 | el original junta las tres escrituras del campo de bits en una (no supone alias con `desc`) |
| `fn_80080048` | 90,0 | registros float (constantes subidas fuera del bucle) |
| `fn_80080528`, `fn_80080B34`, `fn_80080584`, `fn_80080C20`, `fn_80081080`, `fn_800816B4` | 98,5 / 98,3 / 94,8 / 96,6 / 99,8 / 99,0 | registros |

## Decisiones tomadas sin el usuario

1. **Nombre del tipo único:** `Material_unk` (global, el nombre que ya usaban `Model_unk.h` y los enemigos). `ShaderMaterial_unk` desaparece; renombrados sus símbolos (`fn_800C4F38__13ShaderLOD_unkCFPP12Material_unk` y los `@LOCAL@`/`@GUARD@` de `fn_8009D1A4`).
2. **Cabecera nueva `include/atn/Material_unk.h`**, que incluye `std/string.h` y `boost/function.hpp` (por `fn_80080C20`, que recibe un `boost::function<Texture_unk*(const std::string&)>`). Solo la incluyen los ficheros que tocan campos; `Model_unk.h` deja una declaración adelantada. Las vistas `GolemMaterial_unk` (`Em209SmallGolem`) y `LeoMaterial_unk` (`Em401_01_WhiteLeoPard`) siguen como estaban.
3. **`String_unk`** (cadena con `new[]` del heap 1): `lbl_8087EF00` pasa a `sEmpty_unk__10String_unk` (`char`, 1 byte). Por orden del usuario (2026-10-30), `splits.txt` da a la unidad el `.sbss` `0x8087EF00-0x8087EF08` (antes era un hueco entre `LightUtil_unk` y `SmallAllocator_8`) y el fuente lo define.
4. **Valores por defecto de la UV** (`lbl_80880B88`–`B98`, que `ModelDraw_unk` ya leía): miembros estáticos `TexUv_unk::sScaleU`… (explican el `1.0f` duplicado del pool).
5. **`__declspec(noinline)` en el tokenizador** `fn_8007A994`: es la excepción aprobada en la regla 2 (función fuerte que el original llama con `bl` y MWCC inlinearía). Sin ella, `fn_8007B260` se pasa del presupuesto de inline.
6. **`SafeDelete_unk`** (plantilla en el fuente) en el destructor y en `operator=` de `TexStage_unk`. Así MWCC los auto-inlinea como el original y el quicksort sale con la misma estructura: `Swap`, `Median3` y el comparador fuera de línea; `SelectionSort` con todo dentro. `operator=` no está en el `.dol` (el enlazador lo quita).
7. **Macro `FIND_NAME`** para las búsquedas de nombre del parser. Con la función inline, el parser supera el presupuesto (regla R3) y llama a todo; con la macro, las llamadas son idénticas a las del original. Coste: un `cmpwi`/`bne` de más tras cada bucle.
8. `m24`/`m28`/`m2C` pasan a `GXColor mTevColor[3]` (RegColor1-3 se indexan). Código de `Shader_unk` y `ModelDraw_unk` sin cambios.
9. `ShaderDesc_unk` gana los campos que lee el parser (`m074`, `mTexInfos`, `mIndUv`, `mA3C`, `mA40`; floats de `ShaderTexMtx_unk`), sin cambiar el tamaño: `GraphicsDevice` sale idéntico.
10. `include/std/string.h`: constructor `(const T*, size_type)`, `compare(const basic_string&)` (data y size en una sola rama) y `operator==` de dos cadenas. Todos inline y sin otros usuarios.
11. `ShaderTex_unk::ShaderTex_unk` (`Shader_unk.cpp`) construye la etapa con `new TexStage_unk(-1)`. Con `name` declarado dentro del bucle sube a 98,3% (antes 97,8%).

## Hallazgos

- Un miembro con destructor inline hace que MWCC emita una copia débil de ese destructor al definir un constructor de la clase, aunque nadie la llame. El enlazador la quita, así que no indica que el original fuera distinto.
- El auto-inline de MWCC (`-inline auto`) mide el tamaño en sentencias del fuente: un destructor con tres `if (p) { delete p; p = 0; }` no se inlinea; con un ayudante de una línea, sí. Los literales `""` del `new` siguen en `@stringBase0` porque la función no es `inline`.
- Pool de cadenas: `"%s"`, `ScaleX`…`WrapV` (con las erratas del original `TranxX`/`IndTranxX`) van antes que las del parser. Son de un ayudante `static` del menú de depuración de la etapa, inlineado en `fn_800816B4`.
- `fn_8007C0D4` (`b fn_8007B100`) y `fn_80081BCC` (vacía) son callbacks del menú de depuración (`void (*)(void*)`).
