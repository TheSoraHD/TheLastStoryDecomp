# Lote batch-auto-5kb - cola q7

Cola (de menor a mayor): `game/FriendManager_cont`, `game/BattleCalc_unk`, `game/TargetPos_unk`,
`Event/BehaviorGesture`, `AI/AiUtil_unk`, `game/HomeButtonMenu`, `Event/BehaviorMoveTo`, `game/EnemyTask`,
`game/GameSaveTask`, `game/MenuCommon_unk`, `chr/ControlMoveCursor`, `game/U16Table_unk`,
`game/GameSaveLoadBackgroundCommon`, `game/BattleEfp_unk`, `gmk/Cannon`.

## game/FriendManager_cont (commit 2391d39)

Fusionada en `game/FriendManager`.

- Resultado: 1/1 al 100% (`fn_8036554C__13FriendManagerFv`); `game/FriendManager` sigue `matching: true`, `main.dol: OK`.
- **Decisiones sin el usuario**
  - `splits.txt`: `game/FriendManager.cpp` .text 0x80365464-0x8036554C -> 0x80365464-0x8036555C (absorbe el `_cont`, contiguo y sin datos); borrada la entrada `game/FriendManager_cont.cpp`.
  - La funcion no usa `this`: devuelve `lbl_8087F8A0->mHead->mNext` (segundo jugador de `CharaManager<PlayerTask>`, el primer "amigo" tras el lider); firma existente en `include/game/FriendManager.h` sin cambios.

## game/BattleCalc_unk (commit bfef61e)

- Resultado: 1/1 al 100% (`fn_800E2A24`), `matching: true`.
- **Decisiones sin el usuario**
  - `symbols.txt`: `lbl_808812D0` (1.0f) tamano 0x8 -> 0x4 (con 8 bytes objdiff lo trata como double y da 96%; el relleno lo pone la alineacion).
  - Tipo inferido `BattleCalcParam_unk` (0x98 bytes, local al .cpp): bloque de parametros que BattleTask reinicia en +0x38A5C antes de cada calculo; nombres de campos desconocidos.
  - La funcion se deja `extern "C"` (nombre de symbols.txt sin clase).

## game/TargetPos_unk (commit 50562cb)

- Resultado: 2/2 al 100%, `matching: true`.
- **Decisiones sin el usuario**
  - `symbols.txt`: `fn_8003E918` -> `fn_8003E918__8AiTargetFv`, `fn_8003E950` -> `fn_8003E950__8AiTargetCFv` (las llama `AIManager` sobre sus `AiTarget` en +0x424/+0x458 justo tras poner `__vt__8AiTarget`); `lbl_808807C0` (0.0f) tamano 0x8 -> 0x4 (objdiff lo leia como double).
  - Cabecera nueva `include/game/AiTarget.h` (clase `AiTarget`, 0x34 bytes, layout inferido de estas dos funciones; `AiTargetObj_unk` para el objetivo de tipo 2).
  - Nombre real propuesto del fichero: probablemente parte de `game/AiTarget.cpp` o un `AiTarget.cpp` propio (el split inferido esta lejos de AiTarget en .text, asi que es otro fichero; nombre sin pruebas).

## Event/BehaviorGesture (commit f9c5123)

- Resultado: 2/2 al 100%, `matching: true`. Splits ya correctos (vtable/RTTI/nombres y 1.0f/0.0f propios).
- **Decisiones sin el usuario**
  - `symbols.txt`: `fn_8010E3F0` -> `vf08__Q25Event15BehaviorGestureFPQ25Event13ActionArg_unk`, `fn_8010E604` -> `vf0C__...ActionArg_unki` (por la vtable); `fn_800119C0` -> `fn_800119C0__Q25Event9Actor_unkFiiiiPviiff` (metodo de `Event::Actor_unk`, r3 = actor; firma inferida de los registros, el 5o argumento se usa como puntero).
  - `include/Event/Behavior.h`: declarado `Actor_unk::fn_800119C0`.
- Hallazgos MWCC: objdiff compara `.sdata2` por posicion, no por valor: con dos floats pasados como argumento (1.0f, 0.0f) en orden cambiado da 100% igualmente; mirar `cmp.py -v` (muestra el valor del literal).

## AI/AiUtil_unk (commit 4798db2)

- Resultado: 4/4 al 100%, `matching: true`.
- **Decisiones sin el usuario**
  - `splits.txt`: anadida `.sbss 0x8087EE60-0x8087EE68` (`lbl_8087EE60`/`lbl_8087EE64`, antes en `auto_10_8087EE60_sbss`): los escriben solo los setters `fn_800142C4`/`fn_800142CC` de esta unidad y los lee `AI/AiCommon_unk`; se definen aqui como globales `void*` (tipo desconocido). `lbl_8087EE68` (lo usan AiCommon/AI_archer/Em006) se queda en `auto_`.
  - `symbols.txt`: `lbl_808806B0` (60.0f) tamano 0x8 -> 0x4.
  - Tipo local `AiSettings_unk` para `lbl_8087F0A8` (solo `mA4`). `CharaStatus::m14` se lee con cast a `unsigned` (el original usa `cmplwi`); no se cambia la cabecera.
- Hallazgos MWCC: un clamp que reevalua la llamada en cada rama (`f()` llamada hasta 3 veces) sale de macros `MIN(MAX(e, 0), 1)` sin locales.

## game/HomeButtonMenu (commit 303f02c)

- Resultado: 4/5 al 100%, `matching: false`.
- NONMATCHING: `fn_80440994__14HomeButtonMenuFv` 83.4% - planificacion del calculo de la relacion de pantalla (el original copia el tamano 64 con `fmr` y calcula `sy` tras `k * sx`; mismo problema que `StrapTask::vf14`). Con un helper inline que recibe el tamano, la funcion pasa a inlinearse en `fn_8044073C`.
- Nota: el .o emite la RTTI debil de `atn::IRender` (y su cadena) que el original no tiene aqui; con `game/EnemyTask` (mismo caso con 5 RTTI debiles) el enlace da `main.dol: OK`, asi que no deberia impedir `matching: true`.
- **Decisiones sin el usuario**
  - `symbols.txt`: `fn_804405F8` -> `fn_804405F8__14HomeButtonMenuFPv` (crea el singleton), `fn_804406C4` -> `__dt__14HomeButtonMenuFv`, `fn_8044073C` -> `fn_8044073C__14HomeButtonMenuFv`, `fn_80440990` -> `vf10__14HomeButtonMenuFPv`, `fn_80440994` -> `fn_80440994__14HomeButtonMenuFv`, `lbl_8087F4E8` -> `sInstance__14HomeButtonMenu`, `lbl_807544E0` -> `@stringBase0` (tamano 0x20 -> 0x1E, `scope:local data:string_table`).
  - Clase inferida `HomeButtonMenu : atn::IRender` (0xD4 bytes; `IRender` declarada localmente como en otras unidades). El constructor no es `inline` (su literal va en `@stringBase0`) y se define despues de `fn_804405F8` para que el pool empiece por `""`.
  - `game/HomeButton_unk` (siguiente unidad, 0x80440B24-0x80441590) contiene las funciones HBM que usa esta clase (`fn_80440F64`, `fn_804410C0`, `fn_80441170`, `fn_804411DC`, `fn_804413D4`); sigue como unidad aparte (1.0f propio).

## Event/BehaviorMoveTo (commit 48a2786)

- Resultado: 1/2 al 100%, `matching: false`.
- NONMATCHING: `vf0C__Q25Event14BehaviorMoveToFPQ25Event13ActionArg_unki` 97.8% - en `case 6` el desplazamiento y el puntero de la busqueda inline del actor intercambian r4/r5 (mismo problema que `BehaviorFollow::vf0C`; probadas 7 variantes).
- **Decisiones sin el usuario**
  - `symbols.txt`: `fn_8010E60C` -> `vf0C__Q25Event14BehaviorMoveToFPQ25Event13ActionArg_unki`, `fn_8010E7B8` -> `vf08__Q25Event14BehaviorMoveToFPQ25Event13ActionArg_unk` (por la vtable; mismo orden de fuente que BehaviorFollow).

## game/EnemyTask (commit f8f045c)

- Resultado: 8/8 al 100%, `matching: true`.
- **Decisiones sin el usuario**
  - `include/game/CharaTask.h`: firmas de las virtuales `vf68` -> `int vf68(CharaTask* attacker)` y `vf6C` -> `void vf6C(int kind, CharaTask* other)` (deducidas de EnemyTask; nadie mas las define en `src/`). `symbols.txt`: `vf68__9CharaTaskFv` -> `vf68__9CharaTaskFP9CharaTask`, `vf6C__9CharaTaskFv` -> `vf6C__9CharaTaskFiP9CharaTask` (copias de 0x800E8FAC/0x800E8FB4) y lo mismo para `EnemyTask`.
  - `symbols.txt`: `fn_8035B534` -> `fn_8035B534__9EnemyTaskFPQ23atn4TaskP13EnemyInfo_unki` (creador estatico), `fn_8035B854` -> `fn_8035B854__9EnemyTaskFv` (enemigo anterior, como `NpcTask::fn_80547E44`), `fn_801AFAC0` -> `__ct__17EnemyReaction_unkFP9CharaTaskPCQ23atn7Vector3i` (ctor de un control de 0x24 bytes de clase desconocida, declarado local), `lbl_8074B390` -> `@stringBase0` (0x8 -> 0x1), `lbl_808855F8` (0.0f) 0x8 -> 0x4.
  - `include/game/CharaStatus.h`: `chr::CharaData_unk` gana `mCC`/`mD0` (antes relleno `mCC[0x1C]`); `include/game/EnemyTask.h`: declarados `fn_8035B534` y `fn_8035B854`.
  - `vf6C` construye la direccion con `mPos.y` y luego pone `y = 0` (unica forma encontrada de que los stores salgan z, x, y).
- Hallazgos MWCC: una unidad enlazada que emite RTTI debiles (y sus cadenas) de clases cuya RTTI vive en otro fichero anterior enlaza bien (`main.dol: OK`): el enlazador las descarta.

## game/GameSaveTask (commit 8b1b777)

- Resultado: 7/7 al 100%, `matching: true`.
- **Decisiones sin el usuario**
  - Clase `GameSaveTask : GameSaveLoadCommon` declarada en el .cpp (menu de guardado de depuracion: lista de 17 ranuras, "上書きしますか？", "はい"/"いいえ", "セーブ中です").
  - `include/game/GameSaveLoadBackgroundCommon.h`: nuevas `GameSaveLoadCommon` (base, ctor 0x803B82C0 / dtor 0x803B834C en la unidad siguiente), `GameSaveSlot_unk` (cabecera de ranura 0xB8), `GameSaveData_unk` (estado serializado 0x134A8), `GameSaveFile_unk` (0x13560), `SaveTime_unk` (0x28, como OSCalendarTime) y `GameSaveLoadBackgroundCommon::fn_803B8160(int)`.
  - `symbols.txt` (renombrados): `fn_803B7944` -> `fn_803B7944__12GameSaveTaskFPQ23atn4TaskP18GameSaveBackground`, `fn_803B7A08` -> `__dt__12GameSaveTaskFv`, `fn_803B7A60/7AD0/7E20` -> `vf0C/vf10/vf14__12GameSaveTaskFv`, `fn_803B7FC8`/`fn_803B807C` -> `...__12GameSaveTaskFi`; en la unidad siguiente `fn_803B82C0` -> `__ct__18GameSaveLoadCommonFPQ23atn4Task`, `fn_803B834C` -> `__dt__18GameSaveLoadCommonFv`, `fn_803B8160` -> `fn_803B8160__28GameSaveLoadBackgroundCommonFi`, `fn_803B85EC/8758/87A8/8AC4/8B08` -> metodos de `GameSaveLoadCommon`; en `game/GameSaveData_unk`: `fn_803BE8B4` -> `__ct__16GameSaveSlot_unkFv` (se construye en bucle sin `__construct_array`), `fn_803BE8C0`, `fn_803BEAB4` (`GameSaveSlot_unk`), `fn_803BDEE4` (`GameSaveData_unk`); `lbl_8074FF58` -> `@stringBase0` (0x30 -> 0x2B); `lbl_80885C70` (260.0f) 0x8 -> 0x4.
  - Declaracion local de `fn_800616C0` con el float `f6` despues del color (`..., text, color, f6, a, b, c)`): mismo ABI que las otras declaraciones, pero cambia el orden de evaluacion.
- Hallazgos MWCC: el orden de los parametros float respecto a los enteros no cambia los registros (bancos separados) pero si el orden de evaluacion de argumentos: si un literal float se carga tras la union de un ternario del argumento entero anterior (sin `fmr` con otro 0.0f), declarar ese float despues del entero.
- Hallazgos MWCC: `return f() == 0;` da `cntlzw`; con saltos (`li r3,1 / li r3,0`) es `if (f() == 0) return 1; return 0;`.

## game/GameSaveLoadBackgroundCommon (commit 178cc6c)

(Hecha antes de su turno en la cola porque comparte cabecera y simbolos con `game/GameSaveTask`.)

- Resultado: 12/16 al 100%, `matching: false`.
- NONMATCHING:
  - `fn_803B8160__28GameSaveLoadBackgroundCommonFi` 14.9% - el original llama a `fn_803B83A4` fuera de linea; aqui se inlinea (en `fn_803B85EC`/`fn_803B87A8` el original tambien la inlinea). Probado: helper `static inline` + envoltorio, llamada a `fn_803B8200(1)`; el resto de la funcion es identico.
  - `fn_803B85EC__18GameSaveLoadCommonFv` 98.8% - en la copia inline de `fn_803B83A4` dentro del bucle el original compara `NANDGetHomeDir` con `beq cuerpo; b salto` en vez de `bne salto` (la misma inline da `bne` en `fn_803B87A8`). Arriba se resolvio con `if (m198 != 0 || NANDGetHomeDir(home) != 0) return;` (ver hallazgo en chr/ControlMoveCursor) y el `cmpw` con `int pos = NANDSeek(...)`.
  - `fn_803B87A8__18GameSaveLoadCommonFv` 99.9% - solo el nombre de la tabla de saltos (`jumptable_8078B568` frente a `@NNN`).
  - `fn_803B8B08__18GameSaveLoadCommonFP16GameSaveSlot_unk` 99.9% - offsets en `@stringBase0`: el pool original tiene `""`, `"%s/save%02d.dat"`, `""`, `"%s/save_sys.dat"`, `""` entre `"%s/%s"` y sus cadenas (literales de funciones eliminadas por el enlazador; no reconstruidas para no inventar).
- **Decisiones sin el usuario**
  - `symbols.txt`: `lbl_8074FFB8` -> `@stringBase0` (0x68 -> 0x64), `lbl_8074FF88` -> `sEmpty_unk` (`scope:local`; objeto de 8 bytes a cero que el original pasa como nombre/fichero a `new[]`: se reproduce con `static const char sEmpty_unk[8] = ""`), `lbl_80885C90` (16.0f) 0x8 -> 0x4; `fn_803B8AC4` se queda con nombre C (`extern "C"`, la usa NewTitleTask). `lbl_80885C78`/`lbl_80885C7C` son `const char* const` a "savedata.dat"/"banner.bin" (mantienen el nombre).
  - `include/game/GameSaveLoadBackgroundCommon.h`: `m150` pasa de `int` a `unsigned char*` (buffer de `new[]`), incluye `atn/Memory.h` y declara `memset` y el inline `AllocBuffer_unk`; `fn_803B8AC4` no es miembro.
  - `src/auto_03_803B27AC_text.cpp` (GameLoadTask, sin configuracion, no se compila) declara en C `fn_803B82C0`, `fn_803B834C`, `fn_803BE8B4`, `fn_803B85EC`, `fn_803B8758`, `fn_803B87A8`, `fn_803B8B08`, `fn_803BEAB4`, `fn_803B8160`, que ahora tienen nombre C++: habra que actualizarlo cuando se decompile.
- Hallazgos MWCC: un `char buf[8] = ""` estatico const va a `.rodata` (8 bytes) y se direcciona con `lis/addi`; un `const char* const p = ""` va a `.sdata2` (1 byte) y con `-sdata2 4` sale `li rX, sym@sda21`.

## game/MenuCommon_unk (commit b52c507)

- Resultado: 7/10 al 100%, `matching: false`.
- NONMATCHING: `fn_80116FC0` 99.7% (y `fn_80116EB8`/`fn_801170C8`, que la inlinean, 99.7%) - en la busqueda de la tabla de mensajes (`lbl_8087F1E4`, entradas de 8 bytes) el original deja la tabla en r0 y el indice en r3 (`add r3, r0, r3`); aqui salen al reves (5 variantes probadas).
- **Decisiones sin el usuario**
  - `splits.txt`: `.bss` de la unidad 0x807C78E4-0x807C7AF0 -> 0x807C7898-0x807C7AF0: absorbe `auto_08_807C7898_bss` (los 4 colores que inicializa el `__sinit` de este fichero y el registro de destructor del `std::wstring` estatico; el `__sinit` direcciona todo desde 0x807C7898).
  - `symbols.txt`: `fn_801175EC` -> `__sinit_\MenuCommon_unk_cpp` (`scope:local`); `lbl_807369E0` 0xC8 -> 0xC4 (tabla de 7 categorias; el resto es relleno); `lbl_807C78C8` 0x1C -> 0x10 y nuevo `@237` (0x807C78D8, 0xC, `scope:local`: registro de `__register_global_object`, como los `@NNN` de otras unidades). Es el unico simbolo anadido.
  - Funciones `extern "C"` con los nombres de symbols.txt (las usan unidades MP con esas firmas). Tipos locales inferidos: `MenuCategory_unk` (tabla 0x807369E0: id de texto, "acepta todo", orden, 4 tipos de objeto), `MenuItem_unk` (solo `m04` id y `mBC` tipo), `MenuColor_unk`, `MenuMsgTable_unk`.
  - Tablas como `const char* const[]`/`const int[]`: sus cadenas salen como objetos sueltos alineados a 8 en `.rodata` (igual que el original). `fn_80117228` devuelve `atn::SoundRef` por valor (como la declaran `include/MP/D2AMpMenu_unk.h`).
- Hallazgos MWCC: un bucle de 4 iteraciones con `return` se desenrolla (2x2) solo si esta escrito en la propia funcion o en una inline; si la inline devuelve el resultado y el llamador hace `if (inline()) return x;`, MWCC materializa el bool (`li r0,1; b; li r0,0; cmpwi`).
- Hallazgos MWCC: un `std::wstring` estatico con `= L""` y un `const wchar_t* const p = L""` dejan `L""` en `.sdata` (`@wstringBase0`) y en `.sdata2` respectivamente.

## chr/ControlMoveCursor (commit 2a1af24)

- Resultado: 3/6 al 100%, `matching: false`.
- NONMATCHING:
  - `fn_801C346C` 97.6% - el original no llama a ningun constructor para los dos `atn::Matrix3x4` locales (no existe `__ct__Q23atn9Matrix3x4Fv` debil en el juego: su constructor por defecto era trivial); aqui se llama al `Matrix3x4() {}` de `include/math/Vec.h` (la funcion supera el presupuesto de inline). No se toca la cabecera compartida.
  - `fn_801C3944` 96.6% - orden de los stores del punto final del rayo alrededor del ctor de `CollResult_unk`.
  - `fn_801C3D1C` 89.3% - planificacion (el original carga `mChara` antes de las constantes).
- **Decisiones sin el usuario**
  - `symbols.txt`: `fn_801C346C`/`fn_801C3944` -> `...__Q23chr17ControlMoveCursorFPCQ23chr16CursorTarget_unkfff` (los llama `fn_801C3458` con los mismos argumentos), `fn_801C3D1C` -> `fn_801C3D1C__Q23chr17ControlMoveCursorFv` (metodo; lo llaman ControlCharge/ControlChant con `this`).
  - `include/chr/ControlMoveCursor.h`: declarados `fn_801C346C`, `fn_801C3944`, `fn_801C3D1C`.
  - Los getters minusculos que el original llama fuera de linea (`fn_801156BC` pad, `fn_8008B964` camara, `fn_800F7258`, `fn_80116E6C`, `fn_801C1060` cos, `fn_801A03E8` producto escalar, `fn_801C3910` longitud XZ, `fn_801C0738` setter de `mRot`) se declaran `extern "C" inline` (son copias debiles); `fn_801C3910` es la copia debil emitida aqui tras su primer uso.
  - Tipos locales: `CursorField_unk` (`lbl_8087F430`, estado 0x868/0x86C y dos `CursorView_unk` en 0x74 y 0x268 con `GetDir_unk()` inline, deducido del orden de los temporales), `CollResult_unk` (copiado de gmk/MotionObject.cpp, con `fn_8004ED34` tomando `from`/`to` por referencia).
- Hallazgos MWCC: `if (a != 4 || a != b) { return; }` genera `bne fin; cmpw; beq cuerpo; b fin` (el patron `beq/b` que no salia con `if` anidados ni con `&&`).
- Hallazgos MWCC: un `if (x) A(); else B();` con `A`/`B` metodos no inline de la misma clase definidos despues: MWCC autoinlinea la llamada si la funcion tiene <= ~13 sentencias, aunque genere mucho codigo (`fn_801C3944`); para que el original la llame hace falta que tenga mas sentencias.
- Hallazgos MWCC: los temporales de una inline anidada (p. ej. `a - b` dentro de `GetDir_unk()`) van por debajo de los temporales del cuerpo en la pila.

## game/U16Table_unk (commit 5317039)

- Resultado: 2/5 al 100%, `matching: false`.
- NONMATCHING:
  - `fn_803CCF68` 89.6% y `fn_803CD598` 88.5% - `std::vector::push_back` inlineado: el original carga el tamano antes que la capacidad (problema abierto de `include/std/vector.h`) y difiere el reparto de registros salvados.
  - `fn_803CD958` 98.0% - reparto de registros (mRuns en r6 y los datos en r5 en el original); 4 variantes probadas.
- **Decisiones sin el usuario**
  - Tipos inferidos (locales): `U16Table_unk` (cuenta, numero de tramos, puntero), `U16Run_unk` (POD, lo que guarda la tabla: `new[]` sin cookie) y `U16RunPair_unk` (el mismo par con constructor, el elemento del `std::vector` temporal). Especializaciones locales `std::__vec_imp_unk<U16RunPair_unk>` (insercion rapida como la de `int`) y `std::__vec_split_unk<U16RunPair_unk>` (insercion por asignacion, traslado con `new` de copia) en el .cpp.
  - Funciones `extern "C"` con los nombres de symbols.txt (las declaran asi ChaserNpc, CrowdSimulation, Augur y MeetingSpot).
  - `symbols.txt`: `lbl_80750518` -> `@stringBase0` (0x14 -> 0x15, "vector length error" + ""), `lbl_8078BCB8` -> `@STRING@__allocate__Q23std24vector<14U16RunPair_unk>FUi` ("Memory allocation failure", literal de la inline `__allocate`; el nombre lleva el tipo inventado).
- Hallazgos MWCC: `delete[]` de un tipo con constructor (aunque sea POD de facto) mete la cookie de 16 bytes y la comprobacion de nulo; `new[]` sin `__construct_new_array` ni cookie y `__dla` directo = tipo sin constructores.
- Hallazgos MWCC: `while (*(p = f()) != 0)` sale rotado (salto al test del final); `for (;;) { p = f(); if (*p == 0) break; ... }` deja el test arriba como el original.

## game/BattleEfp_unk (commit cae2cd9)

- Resultado: 17/17 al 100%, `matching: true`.
- **Decisiones sin el usuario**
  - Tipos inferidos (locales): `BattleEfpData_unk` (fila de la tabla de efectos de batalla: id, cuatro nombres de efecto/recurso, tres nombres en 0x84/0xA4/0xC4, dos animaciones de camara `.xsca`, bandera 0x124) y `BattleEfpEntry_unk` (0x48 bytes: fila + 3 `atn::EfpHandle_unk`, `ResLoader_unk`, 2 `atn::LH_Xsca`, bandera de cargado) con `IsLoading_unk()` y `Unload_unk()` inline.
  - `symbols.txt`: `fn_800EF6BC` -> `__ct__18BattleEfpEntry_unkFv`, `fn_800EF7F8` -> `__dt__18BattleEfpEntry_unkFv` (copias debiles que usan `__construct_new_array`/`__destroy_new_array`), `lbl_80735428` -> `@stringBase0`. El resto mantiene sus nombres C (`extern "C"`; las llama GameTask).
  - Globales `lbl_8087D968` (modo, = 2), `lbl_8087F030/34/38` (entradas, cuantas, nivel) definidas aqui con esos nombres.
- Hallazgos MWCC: un `bool` inline escrito como `if (a()) return true; ... return f();` deja el valor en r0 (`li r0,1; b` por rama y `neg/or/srwi` en la ultima) y el llamador lo prueba con `cmpwi`; con `return a() || b() || ...` MWCC lo convierte en saltos directos.
- Hallazgos MWCC: un literal dentro de una funcion inline va a un pool `@STRING@` en `.sdata`/`.data`; si el original lo tiene en `@stringBase0`, el codigo estaba en la propia funcion (aqui bloques con un local `name` para que el puntero quede en un registro salvado).

## gmk/Cannon (commit cd4dd46)

- Resultado: 13/13 al 100%, `matching: true`.
- **Decisiones sin el usuario**
  - Cabecera nueva `include/gmk/Cannon.h` (clase `gmk::Cannon : GimmickCommon`, 0x930 bytes; campos deducidos de las claves "MODEL", "MODEL_CRUSH", "MOTION", "EFP_SHOOT", "EFP_CRUSH", "PARAM", "SE_PARAM" y del menu de depuracion "shootWait", "shootWaitRand", "windDirection"; los "SE_PARAM" son `atn::FixedString_unk<0x20>`).
  - `symbols.txt`: los 13 `fn_8040xxxx` -> nombres de `gmk::Cannon` segun la vtable (`fn_80408324` creador, `__ct__`, `__dt__`, `vf0C`, `vf10`, `vf14`, `vf1C`, `vf20`, `vf28`, `vf38`, `vf40`, `vf6C`, `vf70`); `lbl_80752954` -> `@stringBase0`.
  - **Cambio fuera de mi unidad:** `src/game/GimmickCommon.cpp` (enlazada) declaraba `extern "C" fn_80408324`; ahora incluye `gmk/Cannon.h` y llama a `gmk::Cannon::fn_80408324` (sin cambio de codigo; necesario para enlazar con el nombre nuevo). `regress.py`: lost 0.
  - El destructor no es inline: se define tras el constructor (el original lo tiene en 0x80408464; con `virtual ~Cannon() {}` la copia debil sale al final del .o y el `.dol` no coincide).
- Hallazgos MWCC: `if (mModel.mTracks[0].m08 >= f())` carga el campo despues de la llamada; el original lo carga antes (en un FPR salvado): `float frame = ...; if (frame >= f())`.
