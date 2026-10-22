# Lote enemies-finish, cola q7 (Em403WarLion, EnemyJill)

Inicio: 2026-10-22 09:05 CEST. Rama agent-q7 (sale de main 64df7a6). Referencia: /home/claude/ref/start-q7.json.

## Unidades

- `game/Em403WarLion` (unidad SIN FUENTE hasta ahora, 0/67 al inicio): **fuente nueva
  `src/game/Em403WarLion.cpp` y cabecera nueva `include/game/Em403WarLion.h`** (subclase de EnemyTask, layout
  0x14B0-0x1610, nombres inventados). En curso (ver abajo el recuento final).

- `game/EnemyJill` (unidad SIN FUENTE hasta ahora, 0/39 al inicio): **fuente nueva `src/game/EnemyJill.cpp` y
  cabecera nueva `include/game/EnemyJill.h`** (subclase de EnemyTask, layout 0x14B0-0x17C8, nombres inventados;
  parecida a EnemyTrista: lector de csv con "Event", voces "Voice" y matrices de color "ColMat"). En curso.

## Decisiones tomadas sin el usuario

- `config/SLSP01/symbols.txt` (Em403WarLion): renombrados ctor/dtor y las virtuales de la vtable
  (vf0C, vf10, vf14, vf20, vf24, vf3C, vf40, vf50, vf54, vf90, vf94, vfE4, vfEC), los metodos no virtuales
  `fn_XXXXXXXX__12Em403WarLionF...`, `fn_80323C18` -> `IsRaging_unk__12Em403WarLionFv` (copia debil de un
  getter inline), los invokers/managers de los dos `boost::bind` (0x803234C8-0x803235D8), sus RTTI
  (0x80788BC0/BC8), `@LOCAL@`/`@GUARD@` de `assign_to` (0x807C8438/40, 0x8087F400/01) y `lbl_80749D10` ->
  `@stringBase0` (scope:local).
- `game/SpecialEnemyBase.cpp`: la factoria crea el tipo 48 con `new (3, "", "", 0) Em403WarLion(parent, data)`
  (incluye game/Em403WarLion.h; se quita el `extern "C" fn_80322C68`). fn_805A5374 sigue igual (73.27%).
- `config/SLSP01/symbols.txt` (EnemyJill): ctor (`__ct__9EnemyJillFPQ23atn4TaskPC13EnemyInfo_unk`, con
  `const EnemyInfo_unk*` como CopyRobotTask para que la carga de info->m20 vaya al prologo), dtor implicito, vf0C,
  vf10, vf14, vf3C, vf44, vfA8 y los 27 metodos no virtuales `fn_XXXXXXXX__9EnemyJillFv`; `lbl_80749F88` ->
  `@stringBase0` (scope:local).
- `game/SpecialEnemyBase.cpp`: la factoria crea el tipo 63 con `new (3, "", "", 0) EnemyJill(parent, info)`
  (incluye game/EnemyJill.h; se quita el `extern "C" fn_8032DEA0`). fn_805A5374 sigue igual (73.27%).
- Clases inventadas de EnemyJill: `JillEvent_unk` (linea "Event", 0x10 bytes), `JillColMat_unk`/`JillVec4_unk`
  (matriz de color de 0x50 bytes, la misma que lbl_8087EFA8 + 0x324), `JillRing_unk` (cola circular de 10 ints
  de las voces, 0x30 bytes), `JillVoices_unk` (FixedArray_unk<std::string, 5> derivado: su destructor comprueba
  `this` dos veces), `JillControl_unk` (control con una bandera en 0x1C), `PlayerInput_unk`, `JillKeyboard_unk`,
  `JillArena_unk`, `JillField_unk`. Accesores de otras unidades escritos como `extern "C" inline` en el .cpp
  (fn_80122550, fn_8026616C, fn_80266174, fn_802A49BC, fn_802A4968, fn_8000D9E8, fn_8000DCF4, fn_80121F00) y
  fn_8032EC94/fn_8032ECA0/fn_8032ECA8 (copias debiles que el original emite en esta unidad).
- Clases inventadas de Em403WarLion: `WarLionPath_unk` (0x50 bytes en 0x1524, mismo layout y reset que `gmk::PathData_unk`
  sin su ultima palabra), `WarLionPathList_unk` (derivada de std::vector), `WarLionRes_unk` (como ResHandle,
  pero el original limpia el puntero en el cuerpo del ctor), `AllyAi_unk` (datos de IA de un aliado devueltos
  por fn_80103F60, pesos por enemigo en 0x128).

## Hallazgos MWCC

- vf0C de Em403WarLion: un `if (result != 0) { ...; return 1; } return 0;` (sin retorno temprano) da el `beq`
  directo del original; el `Vector3(a, b, c)` asignado a un miembro necesita un local con nombre para que el
  temporal vaya al final de la pila.
- `float frame; if (a > 5 || (frame = m, frame >= f())) {...}` (operador coma) reproduce la carga del frame
  en f31 antes de la llamada sin `mfcr`.

- La cola circular de voces de EnemyJill: `mHead = Next_unk(mHead); mCount--;` (avance de la cabeza antes del
  decremento, con un helper estatico) reproduce el orden de cargas del original.
- Lectura de una bandera de bits en un `if (m1590 == 0) mFlag5 = 1;`: el bit `0x20` de 0x12A4 es `mFlag5`
  (MSB primero), no `mFlag26`.

## Pendientes para otras colas

- (ninguno por ahora)
