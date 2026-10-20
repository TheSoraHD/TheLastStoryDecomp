# batch-auto-10kb qG1: AI/game

Cola: `AI/AiCommon_unk`, `game/AreaEffConfig`, `game/CharaC58_unk`, `game/CharaFaceControl`, `game/MotionCommand_unk`.

## game/AreaEffConfig (commit 961ee2d)

- Resultado: 10/35 al 100%, `matching: false`, `main.dol: OK`, regress lost 0.
- NONMATCHING:
  - `AreaEffConfig::fn_80041D44` (carga del CSV) 0%: el original supera el presupuesto de inlining y llama
    fuera de linea a todas sus inline (de ahi salen las copias debiles 0x8004203C-0x80042EEC y 0x80043130);
    el nuestro las inlinea todas. Medido con una candidata artificial al principio: hacen falta ~81
    sentencias mas para que rechace, y entonces el cuerpo coincide salvo nombres. Probado sin exito: bucle
    para el color, ternario para el grupo, mas/menos sentencias propias, constructor de copia explicito de
    la fila, `FixedString` plantilla, `push_back` dentro de la clase (todo sube o no cambia el umbral). Es el
    mismo caso abierto que `LoadSeTableTask::vf0C` (docs/inlining.md). Mientras no se resuelva, las 21
    copias debiles que solo emite la carga no existen en nuestro .o.
  - `AreaEffConfig::fn_80043014` 99.57%: registros (la cuenta del bucle exterior y el desplazamiento del
    interior intercambian r27/r28; probados tipos de indice, orden de declaraciones y tamano en la condicion).
- **Decisiones sin el usuario**
  - `splits.txt`: `.sdata` de la unidad 0x8087D6F8-0x8087D700 -> 0x8087D6F8-0x8087D708 (los pools `""`
    0x8087D700/0x8087D704 del `new[]` de `Buffer_unk<MotionCmdElemD_unk>` estaban en el hueco).
  - `symbols.txt`: 28 `fn_` renombradas a nombres mangled (create `fn_80041A80__13AreaEffConfigFPQ23atn4TaskPCc`,
    `__dt__16AreaEffEntry_unkFv`, `__dt__13AreaEffConfigFv`, `vf0C__13AreaEffConfigFv`,
    `fn_80041D44__13AreaEffConfigFv`, `fn_80043014__13AreaEffConfigFiPCc`, `__ct__18MotionCmdElemD_unkFPCc`
    y las copias debiles de vector/Buffer/basic_string/D2A_DrawColor con los nombres que da nuestro
    compilador); `lbl_80730CB4` -> `@stringBase0`. `vf10`/`vf14` 0x8004312C/0x80043128 son de verdad
    `atn::Task::vf10/vf14` (otras vtables las usan): se quedan con ese nombre.
  - `fn_80043014` pasa a ser miembro (`AreaEffConfig::fn_80043014(int type, const char* name)`, devuelve
    `const atn::D2A_DrawColor*`): actualizado el llamador `src/game/MapManager.cpp` (antes `extern "C"`;
    sigue igual en report).
  - Cabecera nueva `include/game/AreaEffConfig.h` (clase 0x98: `LH_Binary` 0x48, `Buffer_unk<AreaEffEntry_unk>`
    0x50, 4 colores por defecto 0x58) y `AreaEffEntry_unk` (fila de 0x20; nombre inventado).
  - Cabeceras compartidas ampliadas (solo anadidos): `std::basic_string::empty()`,
    `atn::CsvBase::GetField_unk()` (inventado), `atn::Buffer_unk::Alloc0_unk(int)` (inventado) y
    `operator=`, `MotionCmdElemD_unk(const char*)`, `operator=(const MotionCmdElemD_unk&)`,
    `operator==(const char*)`.
- Hallazgos MWCC nuevos
  - La asignacion implicita de un struct de floats (`atn::D2A_DrawColor`) es una funcion inline mas: en una
    funcion sobre presupuesto sale como copia debil `__as__...` (0x80042108).
  - `atn::Task::vf10/vf14` son inline vacias en el original (copias debiles 0x8004312C/0x80043128 emitidas
    aqui), aunque `include/atn/Task.h` las declara fuera de linea.
  - El `push_back` generico del original (copias 0x800422D4 y 0x800427C8) mueve los elementos con un bucle
    contado: `--last; new (--p) T(*last);` (no `T(*--last)` dentro del placement, que deja `last` en la rama
    del `new` y no da `mtctr`), y tras el bucle hace `this->mSize = buf.mSize; buf.mSize = 0;` sin el
    `this->mSize = 0` previo. Con esos dos cambios en `__vec_split_unk::__push_front_unk` y
    `vector::__swap_out_unk` solo quedan diferencias de registros; no aplicado a `include/std/vector.h`
    (compartido) porque aqui no se emite.
- Pendientes: el presupuesto de `fn_80041D44` (ver arriba).

## game/CharaC58_unk (commit 0ed6c68)

- Resultado: 31/33 al 100%, `matching: false` (dos NONMATCHING), `main.dol: OK`, regress lost 0.
- NONMATCHING:
  - `CharaC58_unk::fn_8011CAE4` 99.46%: registros del bucle que busca el punto por `CharaTask::m58`.
  - `CharaD74_unk::fn_8011CD84` 97.97%: registros (copia del estado temporal, bucle del tipo 6, orden de
    carga de los dos radios) y un salto muerto tras el `MAX` del radio del tipo 4.
- **Decisiones sin el usuario**
  - `game_units.json`: entrada nueva `game/CharaC58_unk.cpp` (`CharaC58_unk`, `CharaD74_unk`).
  - `symbols.txt` (pairnames + a mano): las 25 `fn_8011BF94..fn_8011CD1C` pasan a miembros
    `CharaC58_unk::fn_8011C074(CharaTask* chara, int mode)` y las ordenes `fn_8011C2D4..fn_8011CD1C`
    (todas con `CharaTask* chara` primero; firmas en `include/game/CharaTask.h`);
    `fn_8011CD84` -> `fn_8011CD84__12CharaD74_unkFP9CharaTaski`. Llamadores `extern "C"` actualizados:
    `src/game/PickTask.cpp`, `src/game/Em002Skelton.cpp` (sin cambio en report).
  - `include/game/CharaTask.h` (compartida): `CharaC58_unk` gana `int mB8`, `int mD4`, `unsigned int mD8`
    (flags) y parte `m140` en `m140[0x60]`, `m1A0[0x10]`, `m1B0[0x100]` (los tres `memset`);
    `CharaD74_unk` gana constructor inline (lista de inicializacion, inlineado en el de `CharaC58_unk`),
    `short m06` (antes 2 bytes sin tipo) y accesores `IsFlag_unk/SetFlag_unk/ClearFlag_unk` de `m20`
    (recargan la palabra como el original).
  - `include/game/CharaStatus.h` (compartida): `RouteTable_unk::m80` pasa a `RouteSpots_unk m80[3]`
    (`{count, RouteSpot_unk*}` por tipo de personaje 1/4, 2, 3; `RouteSpot_unk` de 0x148 bytes, nombres
    inventados).
  - Tipos locales: `LabelState_unk`/`LabelPlayer_unk` (registro 0x1C en 0x1470 de los jugadores, como
    `PlayerState_unk` de `CharaTask.cpp`).
  - Nombre real del fichero: sin pistas (el struct de 0xC58 son las ordenes de la IA; las llama el
    despachador `fn_8001AEDC` de `AI/AiCommon_unk`).
- Hallazgos MWCC nuevos
  - `return 1 <= x;` (constante a la izquierda) da `srawi/subfc/adde` en vez de `neg/andc/srwi` de `x > 0`.
  - Un `Vector3` temporal elegido por rama (`const Vector3* p = cond ? &tmp : &member`) evita la copia
    que mete el ternario de objetos; el temporal debe construirse dentro del bloque (no asignarse).
- Pendientes: `CharaTask::mStatus.m2CC` se compara sin signo (`cmplwi`/`cmplw`): aqui con cast; quiza
  deberia ser `unsigned int` en `CharaStatus.h`.

## AI/AiCommon_unk (commit 1fe1adf)

- Resultado: 25/31 al 100%, `matching: false`, `main.dol: OK`, regress lost 0. Funciones libres
  (`extern "C"`, nombres `fn_` sin cambio); la 31.a (`fn_8001BEE0`, 0x64) no estaba en la lista de
  `fasm` por el orden del .s.
- NONMATCHING:
  - `fn_8001A464` 92.79%: el original recarga `lbl_8087EE64` dentro del bucle de copia.
  - `fn_8001A65C` 98.87%, `fn_8001A788` 98.10%: registros.
  - `fn_8001AEDC` (despachador de ordenes de la IA, switch de 23 casos) 98.96%: personaje y su
    `CharaC58_unk` en r30/r31 cambiados y uso de r4/r5 frente a sus copias salvadas.
  - `fn_8001B508` 99.88%, `fn_8001B5B4` 99.50%: orden de `.sdata2` (el original tiene -1.0f antes del 1.5f
    del despachador; ningun uso anterior visible: probados helper inline y local `float`).
- **Decisiones sin el usuario**
  - `game_units.json`: entrada nueva `AI/AiCommon_unk.cpp` (sin clases).
  - `symbols.txt`: `lbl_8072FF58` ("neck") -> `@stringBase0` (`scope:local data:string_table`).
  - `include/game/HitInfo_unk.h` (compartida): `m1C[0x48-0x1C]` partido en `m1C[0x40-0x1C]`,
    `float m40`, `float m44` (alcances de la habilidad).
  - `include/game/CharaTask.h` (compartida): `CharaRes_unk` gana `float m34`, `float m38` (antes relleno).
  - Tipos locales (inventados): `AiList_unk` (`lbl_8087EE64`), `AiEnemyMgr_unk` (`lbl_8087F048`, como en
    ArenaTask), `AiTeam_unk`/`AiTeamMember_unk` (`lbl_8087FA00`, miembros de 0x37C), `AiSettings_unk::m468`.
  - El orden de los casos del switch del despachador en el fuente es 0-9, 15, 10-14, 16, 17, 20-22, 18, 19
    (el del codigo del original).
- Hallazgos MWCC nuevos
  - El codigo de un `switch` con tabla de saltos sale en el orden de los `case` del fuente, no en el
    numerico: sirve para deducir el orden original.
  - Un resultado `float` con valor por defecto (`float d = 0.0f; if (...) d = f(); return d;`) carga el 0.0f
    al principio; con `return 0.0f;` al final sale despues.
  - `int count` que continua en el bucle de relleno (`for (; count < 16; count++)`) frente a un indice
    nuevo cambia todo el desenrollado del relleno.
