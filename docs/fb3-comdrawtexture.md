# F-b3: `atn/ComDrawTexture` como métodos de `atn::DrawHelper` (opción A)

Rama `fb3-comdrawtexture` desde `main` (4e8111f1), 2026-10-30. Paso 1 escrito de madrugada sin compiladores y verificado a las 08:00 con el PC enlazado: **`main.dol: OK`, 8696 / 18215 (igual que `main`), `regress.py` lost: 0, `fuzzdrop.py` 0 bajadas**. Sube el fuzzy de `MindowsCommon` (vf18, +0,2-0,3), `D2A_Message::vf18` (+1), `DebugMessages_unk::fn_80069E50` (+1,8) y `auto_03 fn_803B30D8` (80,5 -> 90,9).

## Hecho (paso 1 de la propuesta: firmas y llamadores)

- Firmas fijadas para las 31 funciones de la unidad que se llaman desde fuera: `include/atn/DrawHelper.h` (`void fn_<addr>(...)` miembros de `atn::DrawHelper`).
  - Orden de parámetros: el de la declaración más repetida entre los llamadores. Los `int`/punteros y los `float` van en registros distintos (r4.. y f1..), así que el orden entre clases no cambia la ABI; el orden dentro de cada clase sí, y se ha conservado.
  - **Pero MWCC evalúa los argumentos en el orden del fuente**: al compilar, `game/GameSaveTask` y `game/EnvSeBase` bajaron porque el color (una expresión) se evaluaba antes que los floats. Eso fija el orden real: `fn_800616C0(x, y, z, w, h, text, color, f6, a, b, c)` y floats antes del color en `fn_80063D3C(pos, radius, arg, color)`, `fn_80063764(a, b, width, color)` y `fn_800629F0(center, size, arg, color, mtx)`. Las demás siguen con el orden mayoritario (ningún llamador emparejado lo contradice).
  - `color` siempre `unsigned int`. Vectores por `const Vector3&` (varios llamadores pasan temporales: `game/Em310Gargoyle`, `game/TeamAIManager`); la matriz de `fn_800629F0` sigue siendo puntero porque `atn/Geometry_unk` pasa `0`.
  - `fn_800629F0`: (centro, medio tamaño, color, matriz, float) sacado del asm (`r4`+4 leído como vector, `r7` a `PSMTXMultVec`).
- `symbols.txt`: las 31 renombradas a su nombre mangled (`fn_80063D3C__Q23atn10DrawHelperFRCQ23atn7Vector3Uiff`...). Mangling calculado a mano con las reglas de CW (sin sustituciones); comprobar contra el `.o` al compilar.
- 302 llamadas en 67 ficheros reescritas con `tools/decomp/drawhelper/` (`rewrite.py`, `canon.json`, `mangle.py`) (copia del script usado): `fn_X(lbl_8087EEB0, ...)` → `lbl_8087EEB0->fn_X(...)`, con los argumentos reordenados por clase de registro según la declaración local que tenía cada fichero. Fuera las declaraciones `extern "C"` locales y los `extern "C" void* lbl_8087EEB0;`; se incluye `atn/DrawHelper.h`.
- Vistas parciales de `DrawHelper` eliminadas (regla 10): `Renderer2D_unk` (D2A_Task, D2A_Frame, D2A_Message, D2A_Texture, D3Anime, D3AnimeInstance, FadeCommon), `StaffRollDraw_unk` (NewStaffRoll), `HbmDraw_unk` (HomeButtonMenu), `DrawTextures_unk` (ModelDraw_unk), y la clase local de `game/main.cpp`. `DrawHelper` gana `mD4` (`Matrix3x4`, matriz de vista) y comentarios en `m10`/`mA0`/`mA4`.
- Llamadores con el receptor en una variable `void*` (TeamAIManager, RenderSystem_unk, D2A_Window, Em310Gargoyle): de momento `((atn::DrawHelper*)draw)->...`; conviene tiparlos como `atn::DrawHelper*`.

## Decisiones tomadas sin el usuario (revisar)

1. `atn/MindowsCommon`: las dos llamadas a `fn_80060D58` pasaban 4 floats; el comentario del propio fichero dice que el original lleva 16.0f en f5, así que ahora pasan `h = 16.0f`. Puede subir sus `vf18` (NONMATCHING 90-97%).
2. `game/EnvSeBase` y `game/EnvSeManager` declaraban `lbl_807C7030` como `Matrix3x4` (identidad); es un `.bss` de 0x10 bytes que el resto del juego usa como vector cero. Cambiado a `atn::Vector3`.
3. `atn/atn_colli`: pasaba la caja entera (`ColliBoxShape_unk`) como tamaño; ahora `m3C.mSize` (offset 0, mismo código).
4. `std/string.h` incluido desde `atn/DrawHelper.h` (para `fn_8005DED8(int, const std::string&)`).

- Los nombres mangled calculados a mano enlazan (ningún `undefined`). `std::string` va con declaración adelantada en `DrawHelper.h` (incluir `std/string.h` chocaba con `stdio.h` en `MindowsCommon`).
- Receptores tipados: `fn_800F7250` (accesor débil de `Em310Gargoyle`) devuelve `atn::DrawHelper*`; las variables `draw`/`renderer`/`debugDrawer` de TeamAIManager, D2A_Window y RenderSystem_unk también.
- `tools/decomp/drawhelper/`: `rewrite.py` (paso de `extern "C"` a miembro), `mangle.py`, `canon.json` (firmas). Para cambiar el orden de una función ya convertida hubo un script de permutación de llamadas miembro; si hace falta otra vez, mismo esquema que `rewrite.py`.

## Paso 2 (2026-10-30, mañana): `atn/DrawHelper` con ComDrawTexture dentro

- **Fusión aprobada por el usuario:** `atn/DrawHelper.cpp`, `atn/ComDrawTexture_rodata.cpp` y `atn/ComDrawTexture.cpp` son una sola unidad `atn/DrawHelper.cpp` (texto 0x8005DD60-0x80068DB0, `.rodata` 0x80731140-0x80731390, `.data` 0x807777C0-0x80777B08, `.sbss` 0x8087EEB0 con el singleton, `.sdata2` 0x808809A0-0x808809E8). Comparten el pool de cadenas (el `""` del principio es de `fn_8005DD60`) y los 0.0f/1.0f. La unidad queda `matching: false` hasta tener las 68 funciones; DrawHelper deja de enlazarse desde fuente (sus funciones siguen al 100% en objdiff).
- **30 / 68 al 100%** (8724 / 18215 en total, +28 frente a `main`; `main.dol: OK`, `regress` lost: 0, sin bajadas de fuzzy, `ordercheck` 0 fuera de orden, `poolcheck` sin avisos). Casi: `fn_8005E838` 98,9, `fn_8005EEE0` 99,8, `fn_8005F36C` 99,1, `fn_8005F90C` 99,1, `fn_8005FD80` 98,9, `fn_800602E8` 99,7, `fn_800607C0` 98,8 (asignación de registros de la comprobación de textura o del bucle rotado) y `fn_80063484` 99,95 (orden de operandos de un `fnmsubs`).
- Esquema: `DrawArray_unk::New_unk<T>()` y `NewArray_unk<T>(n)` (cabecera `+0x10`, redondeo `4 - size % 4`) en `DrawHelper.h`; comandos `DrawHelper::ComDraw*` declarados en la cabecera y definidos en el `.cpp`; vértices `DrawVertex_unk` (0x10), `DrawTexVertex_unk` (0x18), `DrawMaskVertex_unk` (0x20). Las esquinas se escriben `p0 = DrawTransform_unk(mA4, Vector3(...))` con `Vector3 p3, p2, p1, p0;` declarados antes (así salen las tres copias y el orden de pila).
- Firmas corregidas al decompilar (y renombradas en `symbols.txt`): `fn_8005E7F4(color, void* tex0, void* tex1, ...)` (antes `int` en medio), `fn_80061AE4(text, color, a, b, c, d, color2, ...)`, `fn_80062C8C`/`fn_80063200`/`fn_80063484` con `unsigned int segments` (la conversión a float usa la constante sin signo). `DrawHelper::m104` es el búfer `wchar_t[0x200]` de `fn_800616C0`.
- `libs/boost/include/boost/scoped_array.hpp` (Boost 1.35.0): el búfer de texto largo de `fn_800616C0` (`reset` llama a `delete[]` del nulo anterior). `include/std/new.h` gana el `operator new[]` de colocación.
- Script de ayuda para variantes: `/home/claude/scratch/try_variant.sh` (no está en el repo).

## Pendiente

1. Paso 3: el resto de `src/atn/DrawHelper.cpp` (comentario al final del fichero): `fn_800638B0`, `fn_80063D3C`, `fn_800641CC`, `fn_8006471C`, `fn_80064AC4`, `fn_80061E2C` (ventana, 0xADC), los 12 `vf10` (0x80065060-0x800685E4, GX) y los destructores débiles; luego los casi-100%.
2. Paso 2 original: escribir `src/atn/ComDrawTexture.cpp` (66 funciones, 12 clases `ComDraw*` con vtable: `vf08` destructor, `vf10` dibujo). Esquema común de cada comando: reserva en el búfer `m04` (`{data, capacidad, usado}`), construye la clase con su vtable y un array de vértices de 0x10 bytes con `__construct_new_array(fn_800610A0 = ctor vacío)`.

