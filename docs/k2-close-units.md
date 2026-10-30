# K2: cerrar unidades cerca de enlazar (2026-10-30)

Rama `k2-close-units` desde `main` (5878b26), mergeada en `main` el 2026-10-30 junto con `vector-push-back`. Trabajo secuencial, commit por unidad. Referencia de `regress.py`/`fuzzdrop.py`: el `report.json` de `main` antes de empezar.

**Resultado:** 8752 → 8759 funciones al 100%, **4 unidades enlazadas más** (`db/EventIdTable_unk`, `game/SpecialEnemyFactory_unk`, `Event/BehaviorMoveTo`, `Event/BehaviorFollow`), `lost: 0`, sin bajadas de fuzzy, `main.dol: OK`.

## Por unidad

| Unidad | Antes | Después | Qué hizo falta |
|---|---|---|---|
| `db/EventIdTable_unk` | 7/7, no enlazaba | **enlazada** | El pool original acaba con los mensajes de `basic_string`: `Stripped_unk` (devuelve un `std::string`; la elimina el enlazador) + `std/string_members.h`. |
| `game/SpecialEnemyFactory_unk` | 1/2 (99,97%) | **enlazada** | `atn::TaskBase`/`Task` con ids `unsigned long long` (ver abajo); 9 cabeceras mínimas de enemigos sin decompilar (constructor + relleno) y sus `fn_` renombrados a `__ct__…`; `Em310Gargoyle`, `Em003LesserDaemon`, `Em101LittleZesya` por su clase. |
| `Event/BehaviorMoveTo`, `Event/BehaviorFollow` | 1/2 cada una (97,8%) | **enlazadas** | Índice `unsigned` en `EventManager_unk::FindActor_unk` (`include/Event/Behavior.h`). |
| `atn/LightUtil_unk` | 4/7 | 6/7 | Índice `unsigned` en los dos bucles de `LightSet_unk`. Queda `fn_8007A154` (relee `mCount` tras la comprobación). |
| `atn/ShaderManager_unk` | 2/4 | 3/4 | `sErrorShaders[(unsigned int)i]`. Queda `fn_800C5644` (`push_back`). |
| `MP/CryptMP` | 9/11 | 10/11 | `__sinit` (0x804AE7C4) de las estáticas de iostream: `#include "std/ios_init.h"` y renombrado del símbolo. Queda `vf0C` (doble cero en `.bss`). |
| `atn/TextureDraw_unk` | 3/4 (`fn_800D5F68` sin escribir) | 3/4, `fn_800D5F68` al 98% | Escrita la matriz de proyección de textura (4 modos). `RenderView_unk` gana `mScene` (0x2FC). `NONMATCHING` con los restos de registros. |

## Decisiones tomadas sin el usuario (revisar)

1. **`include/atn/Task.h`: `mTag`, `mId` y `m30` pasan a `unsigned long long`** (antes, pares de palabras con `TASK_U64`). Motivo: los 11 `new` de la fábrica de enemigos que pedían 4 bytes más tenían tamaño ≡ 4 mod 8, y todos los demás tamaños del original son múltiplos de 8. Con el cambio solo se descuadraron tres rellenos `[0x520 - sizeof(GimmickCommon)]` (`CharaTask.cpp` ×2, `ControlCannonLift.cpp`), porque MWCC coloca los miembros de la derivada tras el tamaño sin relleno de la base; ahora son literales. `regress.py` lost: 0 en todo el proyecto.
2. **9 cabeceras nuevas** (`Em202ForestSpider`, `Em202MothersSpider`, `Em203GreaterDaemonRedMp`, `Em206CocoonMp`, `Em206EggMonster`, `LastCocoonEgg`, `LastCocoonImago`, `LastCocoonQuadruped`, `QuadrupedTask`) con solo el constructor y un relleno hasta el tamaño del `new`, para no llamar a constructores como `extern "C"` con `self` (regla 10). Sus `fn_` se renombraron a `__ct__…` en `symbols.txt`.
3. **`__declspec(noinline)` en `fn_800D5D00`/`fn_800D5E18`** (`atn/TextureDraw_unk`): funciones fuertes que el original llama con `bl` desde `fn_800D5F68` y MWCC inlinearía (excepción ya aceptada en `CLAUDE.md`).
4. `db/EventIdTable_unk::Stripped_unk`: cuerpo y nombre inventados, como los `Stripped*_unk` de `CharaTask`.

## No enlazables sin tocar `splits.txt`

- `std/bad_cast`, `std/bad_cast_what`, `std/runtime_error`: todas sus funciones al 100%, pero son copias débiles de otra unidad. La vtable de `bad_cast` está en el `.data` de `game/GameEventScript` (enlazarla da `multiply-defined __vt__Q23std8bad_cast`), y `runtime_error` ya lo indica en su comentario. Se cierran al fusionarlas con su unidad real.

## Pendiente / hallazgos

- `game/Arms` `fn_800457A4` (fábrica de `Sword`/`Gun`, sin escribir): el original pasa **5** argumentos (r4–r8) a `__ct__5SwordFPviii`/`__ct__3GunFPviii`, que según el RSO tienen 4 (y `Arms::Arms` solo lee r4–r7). O el constructor europeo tiene un quinto parámetro sin usar o el emparejamiento del RSO falla ahí; resolverlo cambia firmas compartidas.
- `gmk/CharaLight` `fn_80423340` (`__sinit`): tres estáticas de 4 bytes y tres referencias de namespace a ellas (`T& r = obj;`); `vf10` (888 bytes) sin decompilar.
- `Loader::Get_unk<T>` (8 unidades `atn/LH_*` a un paso): `core = cached ? Find_unk(hash) : 0` iguala el código pero hace que MWCC lo inlinee; con asserts vacíos fuera de línea baja al 91,8%. Sigue abierto.
- Dos barridos automáticos sobre las 185 unidades con 1–3 pendientes: índice `unsigned` línea a línea (1 acierto: `fn_800C607C`) e inversión de operandos de comparaciones (0 aciertos). El resto son restos de asignación de registros y scheduling ya documentados en sus `NONMATCHING`.
- `fn_800D5F68`: compilado aislado, el caso 2 da el reparto de registros del original; la diferencia viene de la interacción entre casos (un GPR guardado menos aquí).
