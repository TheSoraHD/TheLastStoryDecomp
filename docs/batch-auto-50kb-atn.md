# batch-auto-50kb-atn (propuesta F, 2026-10-29)

Unidades `atn/` sin fuente de 20–50 KB (`targets.py --max-unit 51200 --limit 1000`, > 20480), en orden de dirección, trabajo secuencial. Rama `batch-auto-50kb-atn` desde `main` (556dddc). Referencia: `/home/claude/ref/report-F-start.json` (8571).

**Resultado:** 8571 → **8614** funciones al 100% (+43), código 30,79 → 30,96%, fuzzy 52,67 → 52,99%, 372 enlazados, `main.dol: OK` (build limpio), `regress.py` lost: 0, `fuzzdrop.py` sin bajadas, `mnemcheck.py` y `poolcheck.py` sin hallazgos.

El lote son solo cuatro unidades. Dos tienen fuente nueva; las otras dos se aplazan por una decisión que es del usuario (ver abajo).

## Por unidad

### `atn/ColliTest_unk` (0x80050900, 25,8 KB): 16/28 al 100%

Pruebas de forma del motor de colisión: distancias punto/recta/segmento (`fn_80050900`, `fn_80050A1C`, `fn_80050C38`, `fn_80050E18`), pruebas de volumen (esfera, cápsula, caja orientada) y pruebas con resultado (`ColliResult_unk`) de esfera, segmento y esfera barrida contra esfera, cápsula y triángulo. Todas son funciones libres `extern "C"` (no hay clase; las llaman `atn_colli`, `ColliCalc_unk` y el juego con el mismo nombre).

- Al 100%: `fn_80050900`, `fn_80050A1C`, `fn_80050C38`, las seis de volumen `fn_80051A88`–`fn_80051CD8`, `fn_80052760` (esfera/triángulo, 0xAF4), `fn_80053254`, `fn_80053BD0`, `fn_800551BC`, `fn_80055810`.
- NONMATCHING (comentario con el motivo en el fuente): `fn_80050E18` 98,6%, `fn_80051684` 90,1% (orden de pila de los locales de la prueba de lados), `fn_80051E90`/`fn_800520F0` 99,9% y `fn_8005256C` 99,7% y `fn_8005361C` 99,9% (cruce r3/r4 de `__VecCopy`, bloqueo conocido), `fn_8005339C` 79,1% (el original realinea la pila a 64 bytes: algún local está sobrealineado), `fn_80054038` 99,5%, `fn_8005691C` 99,6% y `fn_80056AE8` 87,2% (registros y orden z-primero de copias).
- Sin escribir: `fn_8005454C` (segmento contra caja, 0xC70) y `fn_80055C40` (esfera barrida contra triángulo, 0xCDC; usa la tabla `int` `lbl_80731048`).
- Pool `.sdata2`: nuestro orden pone 1e-5 antes que 1.1920929e-7; el original, al revés (probablemente por las funciones sin escribir).
- `AABB_unk` pasa de `atn/Geometry_unk.cpp` a `include/atn/AABB_unk.h` (mismo objeto, `.o` idéntico).

### `atn/ComDrawTexture` (0x8005DED8, 44,8 KB): aplazada

Sus 66 funciones son métodos de `atn::DrawHelper` (cola de comandos de dibujo de depuración y sus `vf10` con GX), con 12 clases `ComDraw*` con vtable. 57 ficheros del juego las llaman como `extern "C" fn_800616C0(lbl_8087EEB0, ...)` con nombres sin mangling en `symbols.txt`. La regla 10 pide escribirlas como miembros, lo que obliga a renombrarlas en `symbols.txt` y cambiar esos 57 ficheros (posible choque con Codex). **Decisión del usuario.**

### `atn/MaterialParser_unk` (0x8007A994, 29,7 KB): aplazada

Parser de los ficheros de material (`Name`, `Shader`, `MatColor`, `TexColor1`...; `fn_8007C46C` mide 0x2580). Los constructores, destructor y métodos de `TexStage_unk` y `ShaderMaterial_unk` (`fn_8007E9EC`, `fn_8007AA30`, `fn_8007AA64`, `fn_8007AB38`, `fn_8007C018`, `fn_8007B100`, `fn_8007C144`, `fn_80081D00`...) se llaman como `extern "C"` con `self` desde `atn/Shader_unk.cpp`, que además construye el array a mano con `__construct_new_array`. Pasarlos a miembros cambia ese fichero ya trabajado. Mismo tipo de decisión que `ComDrawTexture`.

### `atn/EffectSystem_unk` (0x8022B850, 37,4 KB): 27/58 al 100%

Sistema de efectos (instancias en lista y partículas `EffectParticle_unk`). Escritas las funciones pequeñas y medianas:

- Al 100%: los tres constructores (`EffectInst_unk`, `EffectSlot_unk`, `EffectFade_unk`), las variables estáticas de recurso/dueño/grupo/handle y sus accesores (`fn_802328DC`, `fn_80232AC4`, `fn_80232B7C`, `fn_80232B88`, `fn_80232B90`, `fn_802346AC`, `fn_802346B4`) y los métodos de búsqueda y ajuste por dueño/grupo, handle o efecto (`fn_80232ACC`, `fn_80232B98`, `fn_80232BEC`, `fn_80232D04`, `fn_80232E48`, `fn_80232F84`, `fn_80232FF4`, `fn_802330A4`, `fn_80233158`, `fn_802331B0`, `fn_80233260`, `fn_802346BC`, `fn_802346F0`, `fn_80234764`, `fn_802347DC`, `fn_802348E4`, `fn_80234958`).
- NONMATCHING: `fn_8023329C` (muestreo de curva) 98,0%; `fn_80233348` y sus cuatro copias (`fn_802334E0`, `fn_80233678`, `fn_80233810`, `fn_802339A8`) 99,9%: el `fcmpu` contra 0.0f lleva los operandos al revés y ninguna forma probada (`!=`, `0.0f != x`, `==` con ramas cambiadas) lo da.
- Sin escribir (25): la inicialización y liberación (`fn_8022B850`, `fn_8022BEAC`, `fn_8022C2A0`), la actualización (`fn_8022C3F0`, `fn_8022D078`...), el dibujo (`fn_8022D478`, `fn_8022DD1C` de 0x1DA8...), la creación (`fn_8022FAC4`), `fn_802328EC` (avanza una instancia), `fn_80232ED4` (el nombre del RSO dice `Fv` pero usa `r4`), `fn_80233B40`, `fn_80233D74` y el `__sinit` `fn_80234A60`.
- Cabecera nueva `include/atn/EffectSystem_unk.h` con `EffectColor_unk`, `EffectKey_unk`/`EffectCurve_unk`, `EffectParticle_unk`, `EffectInst_unk`, `EffectSlot_unk`, `EffectFade_unk` y `EffectSystem_unk`; salen de `atn/EffectManager.cpp`, `atn/EffectRes_unk.cpp` y `atn/EffectDesc_unk.cpp` (que la incluyen). El código de los tres `.o` es idéntico (solo cambian los números `@NNNN`, que no están nombrados en `symbols.txt`).

## Decisiones tomadas sin el usuario

1. Aplazar `atn/ComDrawTexture` y `atn/MaterialParser_unk` (motivo arriba) en vez de escribirlas con `extern "C"` y `self` o renombrar en masa.
2. `EffectSystem_unk::mEnabled` (0x0C) pasa a `EffectInst_unk* mInsts` y `m20`/`m24` a `mParticleCount`/`EffectParticle_unk* mParticles`; `EffectParticle_unk::m30` pasa a `unsigned int` (el original compara con `cmplw`). `fn_80232ACC` pasa a `void` y `fn_80232BEC`/`fn_80232FF4`/`fn_802330A4` a `int` (el original devuelve 1/0); los llamadores no usan el valor y su código no cambia.
3. Unificar las dos `EffectColor_unk` (la de `EffectManager.cpp`, con constructores, se queda) y las `EffectKey_unk`/`EffectCurve_unk` de `EffectRes_unk.cpp` en la cabecera: `.o` idénticos.
4. Renombrados en `symbols.txt`: `fn_802328EC` → `fn_802328EC__16EffectSystem_unkFP14EffectInst_unki`, `fn_8022BDB4`/`fn_8022BE1C`/`fn_8022BE8C` → constructores de `EffectInst_unk`/`EffectSlot_unk`/`EffectFade_unk` (solo se llaman desde la unidad), y los `lbl_` de las estáticas a `sCurrentRes`, `sCurrentOwner`, `sCurrentGroup`, `sCurrentHandle`, `sFrame`, `sFrameBase` (`pairdata`).
5. Las pruebas de `ColliTest_unk` que el resto del código declara con `Segment_unk`/`ColliCapsuleShape_unk` toman aquí `const ColliCapsuleShape_unk*` (inicio, fin; el radio no se usa), y el segmento de dos puntos local es `ColliSegment_unk`.
6. Herramienta nueva `tools/decomp/resetlocal.py <unidad>`: devuelve el nombre `lbl_` a los `@NNNN` de los datos de la unidad antes de repetir `pairdata.py --apply` (el paso "resetlocal" de la receta no existía en el repo).

## Hallazgos (una línea cada uno en `docs/trucos-mwcc.md`)

- Dentro de una función inline, los temporales y locales se colocan en la pila al revés que en la función que la llama (el primero, en la dirección más baja): el orden de pila del original delata qué cálculo estaba dentro de una inline (`Advance_unk` en `fn_8005361C`, `ColliPlane_unk::Intersect_unk` en `fn_80054038`).
- Un local que el original llena con una copia `psq` desde un temporal (en vez de construirse en su sitio) es una asignación tras la declaración (`Vector3 nd; nd = a - b;`) o el `return r` de `GetNormalize()`; uno que se llena por dos caminos sin copia viene del `return` de una inline con dos salidas.
- Una variable escrita antes de una copia de matriz/llamada y releída después (`stfs` temprano y `lfs` desde la pila) es un miembro de un local asignado antes de esa copia (`local.mRadius = s->mRadius;` antes de `inv = box->mMtx;`).
- `a || b || c` dentro de una inline que devuelve `bool` da la forma con valores (`cntlzw`/`srwi.` y dos banderas); en un `if` directo da `cmplwi`/`beq` (`IsInfNan_unk`).
- Una comprobación de nulo justo antes de escribir en un elemento de lista (`if (p) p->mKill = 1;` dentro de un bucle que ya sabe que `p` no es nulo) es una inline que recibe el puntero.
