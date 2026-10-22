# batch-auto-20kb-atn qA9: atn (arranque por secuencias)

Cola: `atn/SequenceUnitParam1_const_int` (.text 0x805727A4-0x80575F0C).

## atn/SequenceUnitParam1_const_int (commit: ver git log de agent-qA9)
- Resultado: 34/37 al 100% (+34 en el total: 6260 -> 6294), `matching: false`, `main.dol: OK`, regress lost: 0
  (main e integracion), fuzzdrop 0.
- Contenido del fichero: el objeto que crea `main` (`SequenceBoot_unk`, 0x14 bytes: padre + 4
  `atn::SequenceHolder`, singleton lbl_8087F9AC) y que registra las secuencias BootTask/Title/Multiplayer/
  GameLoadMultiplayer/GameLoad/Game/Strap y los mapas de depuracion (tablas sDebugMaps1..3_unk, rellenadas por
  el `__sinit`), las plantillas `SequenceHolder::SequenceUnit(name, F)` y `SequenceUnitParam1<P>` con sus
  instanciaciones de `boost::function` (invoker/manager/stored_vtable por cada tipo de creador) y el callback de
  depuracion de "Noon Town" (fn_8057285C).
- NONMATCHING (los tres son el `push_back` de `std/vector.h` inlineado en `SequenceHolder::Regist_unk<T>`, el
  bloqueo conocido; aqui los temporales de `__grow_cap_unk` van en 0x8/0x10 al reves y r28/r31 cruzados):
  - `Regist_unk<SequenceUnitParam1<const GameLoadTask::Option>>` (0x8057344C) 99.62%.
  - `Regist_unk<SequenceUnitParam1<const GameTask::Option>>` (0x80573B14) 99.60%.
  - `Regist_unk<SequenceUnitParam1<const int>>` (0x80574174) 99.57%.
- **Decisiones sin el usuario**
  - Splits: sin cambios. Nombre propuesto del fichero real: desconocido (algo como `SequenceBoot`/`BootSequence`:
    registra las secuencias del arranque; lo llama `main` 0x8047A4C8). No se renombra la unidad.
  - Nueva cabecera `include/atn/SequenceHolder.h` (las clases salen de `src/atn/SequenceHolder.cpp`, que ahora la
    incluye; sus porcentajes no cambian: 7/11). Cambios: `~SequenceUnitBase` y `~SequenceUnit` pasan a ser
    inline vacios (en este fichero los destructores derivados no llaman al de la base y `SequenceBoot_unk`
    inlinea `~SequenceUnit`; su copia fuera de linea de SequenceHolder.cpp sigue al 100%); ctor plantilla
    `SequenceUnit(const char*, F)`; plantilla `SequenceUnitParam1<P>` (boost::function<Task*(Task*, P)> en 0x48,
    P en 0x5C); `Regist_unk(const SequenceUnit&)` (envoltorio inline de fn_800C3990, copia 0x80572F40) y
    `template <class T> int Regist_unk(const T&)` (no inline, fuera de la clase: su `""` va al @stringBase0);
    `SequenceParam_unk<P>` (como el parametro de la plantilla recibe P: referencia constante por defecto, por
    valor para `const int` y `const GameTask::Option`) y `SequenceArg_unk<P>` (P sin const para la firma del
    boost::function; no cambia el codigo).
  - Tipos inventados en el .cpp: `SequenceBoot_unk` (ctor no inline porque sus literales van al @stringBase0:
    siempre se inlinea y su copia la elimina el enlazador), `GameTask::Option` (0x24: tipo/mapa/sub/posicion/...
    /callback, con ctor de 7 argumentos 0x80572F44; el original lo pasa por valor),
    `GameLoadTask::Option` (0x44: secuencia siguiente + flag; ctor y ctor de copia `*this = other` de usuario: asi
    las copias de vf0C y del ctor salen desenrolladas en registros) y `GameLoadTask::OptionTmp_unk` (temporal
    derivado con el destructor fuera de linea 0x803B29A8, que las copias de Option no llaman),
    `MP::Manager2` (0x9A88, `Create_unk` inline con los `""` en .sdata), `NewTitleTask`, `BootSettings_unk`
    (lbl_8087F0A8, modo de arranque en 0x1B0), `DebugMap_unk` {nombre, Option}.
  - Los creadores tienen enlace C++ (los nombres typeid de los tipos de puntero a funcion no llevan
    `extern "C"`): `symbols.txt` `fn_8010EB58__FPQ23atn4Task`, `fn_80365708__FPQ23atn4TaskRCQ28GameTask6Option`,
    `fn_80531ED0__FPQ23atn4Taski`. `fn_803B27F8` (GameLoadTask) se deja `extern "C"` porque lo llaman asi
    `game/GameOverTask.cpp`, `game/CampSaveLoad.cpp` y `game/NewCampPartsTop.cpp` (alli `CampSaveData_unk` =
    GameLoadTask y `CampSaveArg_unk` = GameLoadTask::Option): su nombre typeid en .rodata sale con
    `extern "C"` (diferencia solo de datos). Pendiente: pasarlo a C++ en los cuatro ficheros.
  - `symbols.txt` (renombrados): todas las funciones de la unidad (invoker/manager de boost, ctores plantilla,
    Regist_unk, destructores y vf0C de SequenceUnitParam1, `__sinit` y fn_8057285C locales),
    `__dt__Q212GameLoadTask13OptionTmp_unkFv` (0x803B29A8; en `src/auto_03_803B27AC_text.cpp` se define ahora
    como ese destructor fuera del bloque extern "C", sigue al 100%), `__ct__Q22MP8Manager2FPQ23atn4Task` (0x804D0328), @stringBase0 (0x80760D18),
    `sSequenceBoot_unk` (0x8087F9AC, global: con scope:local dtk le anade la direccion), sDebugMaps1..3_unk (locales), los
    `@LOCAL@`/`@GUARD@` de los seis `stored_vtable`, los `__RTTI__PF...` de los seis tipos de creador y los dos
    pools `@STRING@Create_unk__Q22MP8Manager2FPQ23atn4Task[@0]` de .sdata.
  - Nueva cabecera `include/atn/SequenceBoot_unk.h` (clase y `sSequenceBoot_unk`): los llamadores `extern "C"`
    de `fn_80572B6C(lbl_8087F9AC, ...)` (game/GameOverTask, game/StrapTask, game/NewCampPartsTop y
    auto_03_803B27AC_text) pasan a `sSequenceBoot_unk->SetNext_unk(...)` y game/main llama a
    `SequenceBoot_unk::Create_unk` (antes `fn_805728BC`). Sin cambios de porcentaje.
  - La tabla sDebugMaps3_unk (dg999 tests) y los holders 2 y 3 no los usa ninguna funcion: el original tendria
    funciones de depuracion eliminadas por el enlazador (no se escriben).
- Hallazgos MWCC
  - Una copia de un struct de 0x44 bytes con `char[0x40]` va en bucle (`lwzu`/`bdnz`) cuando es argumento por
    valor o copia implicita, pero desenrollada en registros (`stmw`) al construir un objeto con nombre o cuando
    la clase tiene ctor de copia de usuario (`*this = other`).
  - El nombre typeid de un tipo puntero a funcion incluye `extern "C"` si la funcion se declaro asi, y el
    `const` de nivel superior de los parametros (`atn::Task *const`).
  - Un ctor de miembro definido fuera de la clase con `inline` sigue poniendo sus literales en `@STRING@`; sin
    `inline` van al @stringBase0 (y se sigue inlineando con -ipa file).
- Pendientes: los tres `push_back` (bloqueo de `std/vector.h`); `fn_803B27F8` con enlace C++ (ver arriba).
