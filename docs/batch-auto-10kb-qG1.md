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
