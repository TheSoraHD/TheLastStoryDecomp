# port-decomp-20261114, cola F: oclusion y restos del motor

Agente F, rama `agent-F`. Unidades: `atn/LayerOcclusionCPU`, `atn/LH_Motion_cont` (y `atn/LH_Motion`).

## Resultado por funcion

| Funcion | Antes | Despues | Firma final |
|---|---|---|---|
| `Loader::Get_unk<LoadMotion>` (0x804766B0, antes `fn_804766B0`) | sin emparejar (`-`) | escrita, NONMATCHING 97,1 % | `template<> LoadMotion* atn::Loader::Get_unk<LoadMotion>(const char*, int)` |
| `fn_800B47AC` | sin escribir | escrita, NONMATCHING 77,6 % | `extern "C" void fn_800B47AC(atn::LayerOcclusionCPU*, const unsigned short* indices, int count, int type, int frac, const void* vertices, int clip, const Mtx* mtx)` |
| `fn_800B5994` | sin escribir | escrita, NONMATCHING 99,9 % (codigo igual; solo los limites de `.sbss`) | `extern "C" int fn_800B5994(atn::LayerOcclusionCPU*, const atn::AABB_unk*)` (1 = oculta) |
| `fn_800B4390` (fuera de la cola) | sin escribir (asm a mano) | escrita en C++, NONMATCHING (objdiff 0 %: el original es asm a mano) | `extern "C" void fn_800B4390(const Plane_unk*, const atn::Vector3* in, int count, atn::Vector3* out, int* outCount)` |
| `LayerOcclusionCPU::vf10` | `vf10(int)` | `vf10(void*)`, sigue al 100 % | `virtual void vf10(void*)` |

## Detalle

- `Get_unk<LoadMotion>`: el simbolo `fn_804766B0` pasa a `Get_unk<Q23atn10LoadMotion>__Q23atn6LoaderFPCci_PQ23atn10LoadMotion`
  (renombrado) y objdiff ya la empareja con la instanciacion de `LH_Motion_cont.cpp`. `LH_Motion::vf0C` deja el
  `extern "C" fn_804766B0(void* manager, ...)` (metodo con `self`, regla 10) y llama a
  `lbl_8087F518->Get_unk<LoadMotion>(path, 1)`; para no instanciar la plantilla en `LH_Motion.o` (unidad enlazada)
  se declara la especializacion explicita `template <> LoadMotion* Loader::Get_unk<LoadMotion>(const char*, int);`
  en `LH_Motion.cpp` (mismo simbolo, sin cuerpo). `LH_Motion.o` sigue identico. Diferencias que quedan: las de
  registros de todas las copias (`atn/LoadManager.h`) y el `addi r5, r5, 0x1b` del `""` de `new`, que en el original
  va detras de los mensajes de `std::string` del mismo pool (estan en `LH_Motion.cpp`, que no se puede fusionar sin
  desenlazarla).
- `fn_800B47AC`: rasterizado de los triangulos de un oclusor. `type` 4 = vertices de tres floats, 1 = tres s16 con
  `frac` bits de fraccion (el original pone GQR6 con `mtspr` y carga con `psq_l`; asm no aprobado: se escribe como
  escala `LoadS16Vtx_unk`, por eso el marco y los registros cambian). Transforma por `mtx`, descarta los triangulos
  de espaldas a la camara (`(v1-v0)x(v2-v0) . (v0-camPos) < 0`, lineas verdes de depuracion), recorta por los 6
  planos del frustum (`clip`, `fn_800B4390` alternando dos buffers de la cache bloqueada 0xE0000000/0xE00000C0) o usa
  una cache de 16 vertices proyectados (`lbl_807C7560` indices, `lbl_807C74A0` posiciones), proyecta con
  `RenderManager::GetScreenPosition` y dibuja con `fn_800B5528`. `m28` (spans) = 0xE0000180. Colores con
  `atn::Color::ToU32` (verde/blanco), lineas con `DrawHelper::fn_80063764`.
- `fn_800B5994`: test de oclusion de una caja: si `m2C == 0` devuelve 0; esquinas a espacio de vista
  (`ViewBox_unk`, misma forma que `ProjectBox_unk` de `atn/Geometry_unk.cpp` con `PSMTXMultVecArray` y
  `__VecMinMax`), fuera de near/far -> 0; rectangulo de pantalla (`fn_800704DC`) acotado con `lbl_8087D898..A4`;
  si ancho, alto y area < `m1F4` -> 1; si cubre >= 31 celdas -> 0; si no `fn_800B64DC(..., 4)`. Cajas y rectangulos
  de depuracion (`fn_8006FE08`, `DrawHelper::fn_800610A4`) segun `m1DC`/`m1E4`. 93 % -> 99,9 %: las esquinas se
  construyen leyendo los campos de la caja directamente salvo `max.z`/`max.y` en locales (deciden los FPR guardados),
  `w` antes que `h`, area en variable y `m1F4` leido en cada condicion. Lo que queda es solo de datos: los limites a
  cero `lbl_8087D898..A4` van a `.sbss` (el original los tiene en `.sdata`, igual que `fn_800B6218`).
- `fn_800B4390`: paso de Sutherland-Hodgman (parte con `n.p + d < 0`). El original divide con `fres` (aproximado); el
  C++ divide exacto. Se define `lbl_807790A8 = {0.0f, 1.0f}` (`.data` propio, umbral y w) y el `.data` queda en el
  orden del original.
- `.bss`: `lbl_807C74A0` y `lbl_807C7560` se definen antes de `sTexReady` y quedan en el orden del original
  (0x0, 0xC0, `sTexObj` 0xE0).

## Cambios de firma o de layout (para el port)

- `atn::LayerOcclusionCPU` usa ya `atn::IRenderLayer` de `atn/Layer3D.h` (se quita la copia local); `vf10(int)` ->
  `vf10(void*)` (el asm da `cmpwi r4, 0` tambien con puntero). Renombrado en `symbols.txt`:
  `vf10__Q23atn17LayerOcclusionCPUFi` -> `vf10__Q23atn17LayerOcclusionCPUFPv`.
- `RenderSettings_unk` local de LayerOcclusionCPU sustituida por `RenderSetting_unk` (`atn/RenderSetting_unk.h`;
  usa `m1D8`, `m1DC`, `m1E0`, `m1E4`, `m1F4`).
- `fn_800B4390` declarada con `const Plane_unk*` y `atn::Vector3*` (antes `float*`); sigue `extern "C"`.
- `fn_800B47AC`: el cuarto argumento es el tipo de vertice (4/1) y el quinto los bits de fraccion; `vertices` es
  `const void*`, `mtx` `const Mtx*`.
- `LH_Motion.cpp`: llama a `Loader::Get_unk<LoadMotion>` (sin `extern "C"`).

## Decisiones sin el usuario

- `fn_800B4390` (no estaba en la cola) escrita en C++ equivalente: el original es asm a mano, no aprobado.
- `fn_800B5994` se queda `extern "C"` (sin clase) para no tocar `atn/RenderSystem_unk.cpp`, que la declara asi.
- Etiquetas de `.sdata2`: `pd.sh` no empareja el `0.0f` (0x80880F48); se renombra a mano a nuestra etiqueta
  (renombrado) para que `labelcheck` no empeore. Repetirlo tras cada `pd.sh` de la unidad.

- `precommit.sh` da `FAIL` solo por `REP+ atn/LayerOcclusionCPU fn_800B5994 99,9`: `report.json` la cuenta al
  100 % (15392 -> 15393) y `objdiff-cli diff` no, por la seccion de los limites (`.sbss`/`.sdata`), como
  `fn_800B6218`, que ya estaba en `rep100` de la referencia. Es una entrada nueva de rep100, no una bajada: se acepta
  y se deja apuntado (main.dol OK, lost 0, sin bajadas de fuzzy, labelcheck igual, ordercheck 0).

## Hallazgos

- El pool `.sdata2` del original empieza por 0.5, 255, 0, 1 = orden inverso de las constantes de
  `atn::ColorToU8_unk`; aqui salen en orden de primer uso (1, 0, 255, 16, 32, 128, 0.5). Sin resolver.
- `vf10`/`vf1C` dan 100 % en objdiff con las etiquetas emparejadas aunque el offset de 1.0f en `.sdata2` difiera.

- Probado en `fn_800B4694` (90,6 %): el ternario del ultimo argumento en la llamada, `bool`, la forma sin local de
  la forma; en todas MWCC la inlinea en `vf14` (62 %): se deja como estaba.

## Pendientes

- `OccObj_unk`/`OccRenderNode_unk` son vistas locales de `RenderObj_unk`/`RenderNode_unk` (`atn/Layer3D.h`), cuyo
  `vf0C` alli devuelve `void` y aqui `int`: unificar exige tocar `atn/Layer3D.h`.
- El comentario de `atn/RenderSystem_unk.cpp` (linea 72, "vf10 takes an int there") queda desfasado (otra unidad).
