# cabeceras-20261114, cola G: deuda de la regla 10 en cabeceras

Rama `agent-G`. Inicio 16:10, fin ~17:55. Referencia `$REF` = `/home/claude/ref-G` (15433 / 18109). Resultado:
**15433 -> 15433** (cola de deuda de la regla 10: sin funciones nuevas al 100 %), lost 0, sin bajadas,
labelcheck/rep100 igual, `main.dol: OK` en cada commit. Suben de fuzzy `BattleCharaTask::fn_800E589C`
(97,22 -> 97,96), `fn_800E9ED8` (97,93 -> 98,50) y `fn_8032AF90` de Em403WarLion (97,73 -> 97,80).

## Resultado

### 1. `fn_800F8C6C` como miembro de `BattleTask`

- `include/game/BattleTask.h`: `int fn_800F8C6C(HitResult_unk*, int, CharaTask*, HitInfo_unk*, int, float, int,
  int);` (sin nombres de parametro). Mismo orden de argumentos que la definicion (`out, outCount, chara, data, id,
  angle, time, max`).
- `game/BattleTask.cpp`: la definicion `extern "C" int fn_800F8C6C(BattleTask* mgr, ...)` pasa a
  `int BattleTask::fn_800F8C6C(...)` (`mgr` -> `this`). `symbols.txt`: renombrado
  `fn_800F8C6C` -> `fn_800F8C6C__10BattleTaskFP13HitResult_unkiP9CharaTaskP11HitInfo_unkifii`. Sigue NONMATCHING
  90,6 % (igual).
- 41 unidades llamadoras: fuera las 43 declaraciones `extern "C"` (y sus comentarios del orden del float en
  EnemyJill, EnemyTrista, QuadrupedTask, Em401_01_WhiteLeoPard); las llamadas pasan a
  `((BattleTask*)lbl_8087F048)->fn_800F8C6C(out, count, chara, data, id, angle, time, max)` (el `lbl_8087F048`
  de esas unidades sigue siendo su vista `Effect_unk`/`MountleEffect_unk`; mismo molde que ya usaba
  `Em309Ghoul` con `fn_80109828`). Las 21 que no la incluian incluyen ahora `game/BattleTask.h`.
  Em306GurgBeast: la inline C `fn_802F0964(Effect_unk* mgr, ...)` llama `((BattleTask*)mgr)->fn_800F8C6C`.
- Sube `BattleCharaTask::fn_800E589C` 97,22 -> 97,96 % y `fn_800E9ED8` 97,93 -> 98,50 % (el float en su
  sitio); EnemyJill (que tenia el float en octavo lugar "por el orden de carga") no baja.
- Re-emparejado: `pdbatch.sh` de las 80 unidades que incluyen `BattleTask.h` o llaman la funcion, `lblmap.py` en
  9 (Em202ForestSpider, Em203GreaterDaemonRedMp, Em206CocoonMp, Em305Vampire, EnemyQuarkFirst, EnemyQuarkSecond,
  LastCocoonImago, QuadrupedTask, Em306GurgBeast). `lbl_8087D6E8/EC` devueltos a mano (pdbatch los habia
  renombrado a `@STRING@Resize_unk...`, enlace con simbolos duplicados). 15433 -> 15433, lost 0, sin bajadas,
  labelcheck/rep100 igual, `main.dol: OK`. poolcheck de ControlMountle/EnemyJill/Em306GurgBeast igual que con el
  fuente de HEAD.

### 2. `Effect_unk` de LastCocoonImago y `MeleeTarget_unk`

- `game/LastCocoonImago.cpp`: fuera la vista `struct Effect_unk { m00000[0x3E2E8]; void* m3E2E8; }`;
  `lbl_8087F048` es `BattleTask*` (como en AiTarget) y `fn_800F8548`/`fn_800F8574`/`fn_800FAB80` se declaran con
  `BattleTask*` (como sus definiciones C de BattleTask.cpp). `lbl_8087F048->m3E2E8` es el `atn::ColliBase*` de
  `BattleTask` (comparado con `&m18FC`, un `atn::ColliSphere`). Neutro: 21/36 igual, sin bajadas, labelcheck igual.
- `MeleeTarget_unk` (y `MeleeTargetLess_unk`, `MeleeSort_unk`/`MeleeMedian3_unk`) se queda en
  `game/BattleTask.cpp`: no es una vista de un objeto compartido sino el tipo del `std::vector` local de
  `BattleTask::fn_800F8C6C` (ninguna otra unidad lo usa). Llevarlo a `BattleTask.h` (ctor inline vacio, plantillas)
  renumeraria las `@N` de las ~80 unidades que incluyen la cabecera sin desbloquear nada.

### 3. `AiInput_unk`/`AiOutput_unk` de AI_archer_attack01

- `AiScriptInput_unk`/`AiScriptOutput_unk` salen de `game/AiTarget.h` a la cabecera nueva
  `include/game/AiScriptIO_unk.h` (solo datos; `AiTarget.h` la incluye). AI_archer_attack01 incluye solo esa
  cabecera, define `AiScriptInput_unk lbl_807C68C0; AiScriptOutput_unk lbl_807C6A40;` y pierde sus vistas
  `AiInput_unk`/`AiOutput_unk` (todo `int` por desplazamiento). ~2600 accesos traducidos por desplazamiento
  (`m7C` -> `m7C[0]`, `m80` -> `m7C[1]`, `m18`..`m74` de la salida -> `m18[i]`, `m00` -> `mChara`/`mValid`,
  `m20` -> `mTarget`, `m28` -> `mFollow` (ya `CharaTask*`, fuera los moldes), `m64` -> `m60[1]`...); en
  `fn_8001C088` `&in->mA4` -> `&in->mA0[1]`, `&in->m16C` -> `&in->m14C[8]`, `&out->mDC` -> `&out->mBC[8]`.
  La palabra `mE0[0]` de la salida guarda un `CharaTask*` en los scripts: moldes `(CharaTask*)`/`(int)`.
- Neutro: fnstat de AI_archer_attack01 igual funcion por funcion (482/510), sin bajadas, labelcheck igual.
- Probado y descartado: incluir `game/AiTarget.h` entero en AI_archer_attack01 (los `@N` de los dobles de
  conversion se renumeran: hace falta `pd.sh`) y una `union { int mE0[4]; CharaTask* ...; }` en la estructura
  (una union anonima renumera las `@N` de las ~25 unidades que incluyen `AiTarget.h`: BattleTask, CharaTask,
  GameEventScript, GameUI...).

### 4. `OccObj_unk`/`OccRenderNode_unk` de LayerOcclusionCPU

- `atn/Layer3D.h`: `RenderObj_unk::vf0C` devuelve `int` (antes `void`; distinto de cero si el objeto esta
  oculto, como la leia LayerOcclusionCPU). Nadie mas la llamaba.
- `atn/LayerOcclusionCPU.cpp`: fuera `OccObj_unk` y `OccRenderNode_unk`; `vf14` recorre la lista como
  `RenderNode_unk` y `OccModel_unk` deriva de `RenderObj_unk` (relleno 0x04-0x3C, mismo layout), con
  `static_cast<OccModel_unk*>(node->mObj)`. Neutro (fnstat igual, `vf14` sigue 99,4 %), labelcheck igual.

### 5. Copias locales de `IRender`

- Fuera las cinco copias de `class IRender` (Rain, TextureComposite, VideoTHP, LinearAllocator,
  CompositeIntensityTintMulti): incluyen `atn/IRender.h`. Las derivadas toman las firmas de la cabecera:
  - `Rain::vf14(const Matrix3x4*, const Vector3*, const Frustum_unk* frustum, AABB_unk*)` (antes
    `vf14(void*, void*, void* frustum)`; el frustum sigue siendo el tercero) y `Rain::vf18(RenderManager*)`.
  - `TextureComposite::vf14(...4...)`/`vf18(RenderManager*)` (antes sin argumentos), tambien en la copia de
    `TextureComposite` de CompositeIntensityTintMulti (cuyo `vf10` pasa de `Camera_unk*` a `RenderManager*`).
  - `VideoTHP::vf10(RenderManager*)`, `vf14(...4...)`, `vf18(RenderManager*)` (antes sin argumentos).
  - LinearAllocator: las declaraciones `vf10` de las ocho `PostFx*` locales pasan de `Camera_unk*` a
    `RenderManager*` (el `.o` da las mismas funciones; `main.dol: OK`). Fuera el `typedef Camera_unk`.
- `symbols.txt`: renombrados los 7 simbolos con firma nueva (`vf14__Q23atn4RainFPvPvPv`, `vf18__Q23atn4RainFv`,
  `vf14/vf18__Q23atn16TextureCompositeFv`, `vf10/vf14/vf18__Q23atn8VideoTHPFv`) a los nombres nuevos.
- `pdbatch.sh` de las cinco (las inlines de `IRender.h` renumeran las `@N` de CompositeIntensityTintMulti:
  `vf14` 99,8 -> 100 % tras el re-emparejado). Neutro, labelcheck igual, `main.dol: OK` (LinearAllocator enlaza).
- Pendiente (no es de esta cola): `PostFxBloom.cpp` y demas `PostFx*` declaran `vf10(void*)`, que no sobrescribe
  `IRender::vf10(RenderManager*)` (el vtable original dice `vf10__Q23atn11PostFxBloomFPv`); las vistas locales de
  `PostFx*`/`PostFxManager` en LinearAllocator y de `TextureComposite` en CompositeIntensityTintMulti; y
  `include/atn/VideoTHP.h` (vista sin bases de `VideoTHP` para Event::MovieTask/TutorialTask).

### 6. `PathLengths_unk`: destructor 0x8000D844 y constructor 0x8032B6F8

- `include/Event/PathSpline.h`: `Event::PathLengths_unk` (derivada de `std::vector<float>`) recibe
  `PathLengths_unk() {}`, `~PathLengths_unk()` con cinco asertos vacios `((void)0)` (umbral de autoinline, como los
  de Em403WarLion: sin ellos el destructor se inlinea en el constructor del trazado y el original lo llama) y
  `PathLengths_unk(unsigned int n) : std::vector<float>(n) {}`. `symbols.txt`: `fn_8032B6F8` ->
  `__ct__Q25Event15PathLengths_unkFUi` (0x8000D844 ya era `__dt__Q25Event15PathLengths_unkFv`).
- `game/Em403WarLion.cpp`: `WarLionBuild_unk` usa `Event::PathLengths_unk xs(n), ys(n), zs(n)` (fuera
  `WarLionFloats_unk`, la definicion C `fn_8032B6F8` y las llamadas a `fn_8000D844`); el constructor debil sale de
  la unidad con su nombre real (100 %), `fn_8032AF90` 97,73 -> 97,80 %. Escritura por `xs.begin()[i]`
  (`xs[i]` da 76 %; el `operator[]` cambia el desenrollado).
- **No hecho** en gmk/WindyItem, game/PathData_unk y Event/ActionActorPosition: con locales `PathLengths_unk`
  sus constructores del trazado bajan mucho (95,6 -> 43 %, 98,7 -> 19,9 %, 94,7 -> 62,7 %): alli el
  constructor y el destructor son los unicos candidatos a inline (el resto de ayudantes son `extern "C"` sin
  cuerpo), caben en el presupuesto y se inlinean; en el original todos los ayudantes (`resize`, `solve`...) eran
  inline y el presupuesto se pasaba. Siguen con sus `extern "C" fn_8032B6F8`/`fn_8000D844` (nombres que ya no
  existen en `symbols.txt`). Las versiones probadas quedan en `scratch-G/*_pl.cpp`.
- Re-emparejado: `pdbatch.sh` de las 38 unidades que incluyen `PathSpline.h` (directa o indirectamente) y
  `lblmap.py` en 9 (ActionCameraMode, DebugInfo_unk, Node, Sequence, GameEventScript, GameTask, GameUI,
  NewCampPartsMapSelect, NewCampTask); `lbl_8087D6E8/EC` devueltos a mano otra vez. Neutro: 15433, lost 0, sin
  bajadas, labelcheck/rep100 igual, `main.dol: OK`; poolcheck de Em403WarLion/ActionActorPosition igual que HEAD.

## Decisiones tomadas sin el usuario

- Llamadas por molde `(BattleTask*)lbl_8087F048` en vez de cambiar el tipo de `lbl_8087F048` en las 41 unidades:
  sus vistas `Effect_unk` se usan tambien con otras funciones C (`fn_800F8548`, `fn_800FAB80`, `fn_801092C8`...)
  que siguen siendo `extern "C"` con el `BattleTask*` delante; unificarlas es otra deuda.

## Para el port

- `fn_800F8C6C` es ahora `int BattleTask::fn_800F8C6C(HitResult_unk* out, int outCount, CharaTask* chara,
  HitInfo_unk* data, int id, float angle, int time, int max)` (aprobado por el usuario). Antes: `extern "C"` con
  el `BattleTask*`/`Effect_unk*` delante y, en 39 de las 43 declaraciones, el float al final. Ahora todas las
  llamadas pasan el float en sexto lugar (tras `id`): el problema de posicion del float en el ABI x64 desaparece.
  Unidades del port que cambian: `BattleTask` (definicion), `BattleCharaTask` (`fn_800E589C`, `fn_800E9ED8`).
  Resto de llamadores (no del port): ControlMountle, ControlUseSkill, Em003..Em403, EnemyJill/QuarkFirst/
  QuarkSecond/QuarkStand/Tasha/Trista, LastCocoonImago, LastCocoonQuadruped, QuadrupedTask, em005_doppel_ganger.

- `AiScriptInput_unk`/`AiScriptOutput_unk` viven ahora en `game/AiScriptIO_unk.h` (incluida por
  `game/AiTarget.h`; layout sin cambios). En el port, `mE0[0]` de la salida guarda un `CharaTask*` en
  AI_archer_attack01 (`(int)` puntero): en x64 se trunca; AiTarget copia las cuatro palabras a
  `CharaC58_unk::m1A0`. Pendiente en el port: un tipo de puntero para esa palabra (una union aqui renumera `@N`).
- `atn::RenderObj_unk::vf0C`: `void` -> `int`. `LayerOcclusionCPU::vf14` (unidad del port) usa
  `atn::RenderNode_unk`/`RenderObj_unk`; `OccModel_unk` ahora deriva de `RenderObj_unk` (mismo tamano y offsets).

- Firmas de virtuales de `atn::IRender` en Rain, TextureComposite, VideoTHP (ver punto 5): `vf14` con los cuatro
  argumentos de `IRender.h` y `vf18(RenderManager*)`. Ninguna es unidad del port.

- `Event::PathLengths_unk` tiene ahora constructor por defecto, destructor y `PathLengths_unk(unsigned int)`
  (= 0x8032B6F8, antes `extern "C" fn_8032B6F8` definido inline en Em403WarLion). Layout igual. **El port pierde
  la definicion C `fn_8032B6F8`**: WindyItem, PathData_unk y ActionActorPosition la siguen llamando (y
  `fn_8000D844`, que ya era un stub): en el port, cambiar esos tres trazados a locales
  `Event::PathLengths_unk xs(n)` (mismo comportamiento; aqui no se hace porque baja el emparejado).

## Hallazgos

- Un destructor inline de una sola linea se inlinea (R5) donde el original lo llama: unos asertos vacios
  `((void)0)` en su cuerpo lo sacan del umbral sin cambiar el codigo (Em403WarLion, `PathLengths_unk`).

- Una `union` anonima nueva dentro de un `struct` de una cabecera compartida renumera las `@N` de todas las
  unidades que la incluyen (un miembro de datos normal no).
- `fnstat.py --nobuild` puede diferir del `report.json` de `$REF` en una unidad (AI_archer_attack01: 11
  funciones); para medir un cambio, comparar fnstat con fnstat del fuente de HEAD.

