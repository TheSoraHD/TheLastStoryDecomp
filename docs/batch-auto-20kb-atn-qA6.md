# batch-auto-20kb-atn qA6: atn (sonido, tareas, animacion de texto)

Cola: `atn/Sound3dListener_cont` (fusion con `atn/Sound3dListener`), `atn/Task`, `atn/TextAnimation`.

## atn/Sound3dListener (+ _cont fusionado)
- Resultado: 37/37 al 100%, `matching: true` (sigue enlazada), `main.dol: OK`.
- Contenido: `atn::SoundCore` completo (fades de pitch/pan/fx, arranque por etiqueta/id, 2D y 3D, start vs prepare,
  pausa por mascara, volumen por categorias, longitud/posicion en ms de streams y wave sounds, volumen de pistas).
- **Decisiones sin el usuario**
  - Splits: `atn/Sound3dListener_cont` fusionada en `atn/Sound3dListener`: `.text 0x800C7DC4-0x800CAD9C`,
    `.rodata 0x8073401C-0x80734180` (absorbe la tabla de errores 0x80734020-0x80734130 que estaba en auto y los
    doubles de conversion), `.data 0x807794F8-0x807795C0`, `.sdata2 0x80881160-0x80881180`.
    - `.rodata` empieza en 0x8073401C (4 bytes de relleno de SoftKeyboardBase): con start 0x80734020 dtk creaba
      `gap_06_8073401A_rodata` de tamano invalido y objdiff fallaba ("size out of section bounds").
    - `.rodata` acaba en 0x80734180, no en 0x80734188: los 8 bytes de ceros finales no los emite el .o y, dentro
      del split, el `.dol` salia desplazado 0x10 (atn/Sound3dEngine enlazada justo detras). Quedan en auto.
  - `symbols.txt`: pairnames (31 metodos `fn_XXXXXXXX__Q23atn9SoundCore...`; `fn_800CA844` es `const`, `CFv`),
    datos: `lbl_80734020..lbl_807340D8` -> `@689..@700` (cadenas de la tabla), `lbl_807340F0` -> `sSoundErrors`,
    `lbl_80734120/28` -> `@3670/@3792` (doubles de int->float), `lbl_8073416C` -> `@stringBase0`,
    `lbl_80881160` -> `@701` (""), `lbl_80881164` -> `sUnknownError` (todos locales).
  - Llamadores actualizados (antes `extern "C"`): `atn/Bgm`, `atn/Se_unk`, `atn/Sound3dEngine` (enlazadas, siguen al
    100%), `game/SongManager`, `game/GameBgm`: llaman con cast a `atn::SoundCore*`. Cabecera nueva
    `include/atn/SoundCore.h` solo con los metodos usados fuera (la clase completa sigue en el .cpp);
    `Sound3dEngine.cpp` amplia su vista local de `SoundCore` con `fn_800CA844() const`.
  - Tipos inventados en el .cpp: `SoundFade_unk` (ya existia; anadidos Start/Update/Lerp/Finish), `SoundCategory_unk`
    (punteros 0xA8/0xAC/0xB0: mute, volumen, pitch, pan, fx), `SoundStartInfo_unk` (= nw4r StartInfo con
    SeqSoundInfo), `SoundDataInfo_unk`, `StrmSoundHandle_unk`/`WaveSoundHandle_unk`, `HandleSound_unk` (player en
    0x110), `nw4r::snd::detail::BasicSound`/`SoundParam_unk` (bloque en 0x30). Layout de `SoundCore`: `mD8` pasa
    a `Vector3`, `mC4` es float, `mA8..mB0` punteros; `Sound3DActor` con `mUserParam_unk` 0x60, `mPosition` 0x64,
    `mStartOffset_unk` 0x80.
  - Nombres de error de NW4R (`"success"`...`"invalid seq start label"`) en una tabla estatica: dan el orden real de
    `SoundStartable::StartResult`.
- Hallazgos MWCC
  - `if (mSound == 0) return x; return mSound->Inline();` (con el metodo inline en la clase del sonido) da la
    comparacion en r0 y la recarga del puntero; `!IsAttachedSound()` invierte los bloques y lo reutiliza.
  - Un `float` guardado en un miembro y releido por una inline (`mB8 = v; Apply();`) sale como `stfs f1` +
    `frsp` del parametro: el original separa setter y "apply" (fn_800CA0AC/fn_800CA144).
  - Rellenar un struct local a traves de un helper `static inline` que recibe el struct por referencia cambia el
    reparto de registros respecto a escribir las mismas asignaciones en el llamador (StartInfo en 0x800C8780).
  - `(void)((cond) || (Panic(...), 0))` (assert NW4R) materializa el bool (`li r0,0 ... li r0,1; cmpwi r0`).
  - Interpolacion sin fmadds: `float d = t * (b - a); float v = a + d; m1C = v - m18; m18 = v;`.
  - `if (a && b && !c) X(); else if (!a && d && c) X();`: el else-if usa las copias en registros salvados.
- Pendientes: ninguno.

## atn/Task (+ atn/TaskBase nuevo)
- Resultado: `atn/Task` 11/25 al 100%, `atn/TaskBase` 3/4 al 100%, ambas `matching: false`, `main.dol: OK`.
- NONMATCHING (comentados en el fuente):
  - Recorridos recursivos inlineados (fn_800D2494/2868/2AD8/2C50/2D7C/32B8, dtor, 3FA4/4280: 90-99%): el
    original reparte los registros salvados de los niveles inlineados en otro orden (p. ej. r25/r26 en el 2o/3er
    nivel). fn_800D26A4, fn_800D37B8 y fn_800D3B74 si casan.
  - fn_800D4558/498C/4DB8/51EC (42.7%): el original inlinea un nivel mas de la recursion (assert del hijo en el
    llamador y luego la llamada); con el assert dentro de la funcion se llega a la profundidad pero cambia la
    entrada. Probado tambien un helper inline (assert + llamada): MWCC emite el helper fuera de linea.
  - fn_800D1D94 (95%): el recorrido hasta el ultimo hijo compara `*link` en r0 y lo recarga (lwz + lwzu).
  - `TaskBase::TaskBase` (58.86%): orden de cargas del contador de ids y de la etiqueta.
- **Decisiones sin el usuario**
  - Splits: `atn/Task` partida en `atn/Task.cpp` (`.text 0x800D1C60-0x800D5618`, `.data 0x807796B0-0x807796F0`,
    `.sbss 0x8087EFF0-0x8087EFF8`) y `atn/TaskBase.cpp` nueva (`.text 0x800D5618-0x800D5738`, `.ctors`, `.rodata
    0x80734628-0x80734638` "XTASKTAG", `.data 0x807796F0-0x80779700` vtable TaskBase, `.bss 0x807C75D0-0x807C75E8`).
    Prueba: Task.cpp conserva todas las llamadas a `IsValid` cuyo resultado se ignora (asserts de release);
    MWCC las elimina si ve la definicion en el mismo fichero (comprobado). El `__sinit` (etiqueta "XTASKTAG") va
    al final, detras de TaskBase.
  - fn_800D1C60/fn_800D1CD4 (tablas de modos que usan MaterialParser_unk/Shader_unk) se quedan en Task.cpp como
    `extern "C"`; pueden ser de otro fichero (estan al principio del split).
  - `include/atn/Task.h`: `TaskBase` con `IsValid`, ctor `TaskBase(int)` y `mTag`/`mId` de 64 bits guardados como
    `unsigned long[2]` (+ relleno `m04`/`m2C`): un `long long` alinea la clase a 8 y mueve miembros de
    CharaTask/ControlCannonLift (comprobado con regress). Acceso con la macro `TASK_U64`. `Task`: miembros con
    nombre (`m18` primer hijo, `m1C` hermano, `m20` padre (sigue `void*`: gmk/OnOff lo compara con otro tipo),
    `m24` raiz, `m28`/`m30` tarea avisada por vf18 y su id, `m3C` contador, `m40` espera, `m44` borrador),
    inlines `AddChild_unk`/`RemoveChild_unk`, metodos `fn_800D1D94`, `fn_800D4558`, `fn_800D4DB8`.
    `TaskStats_unk` (lbl_807C75D8: tareas vivas + ultimo id) se queda con nombre `lbl_807C75D8` (extern "C").
  - Flags de `mFlags` deducidos: 1 solo hijos, 2 listo (vf0C devolvio != 0), 4 parado, 0x10, 0x20 vf10 hecho,
    0x40 destruido, 0xDEAD0000 matado (fn_800D2338).
  - `symbols.txt`: pairnames (estaticas `fn_XXXXXXXX__3atnFPQ23atn4Task`, metodos, `__ct__Q23atn8TaskBaseFi`,
    `IsValid__Q23atn8TaskBaseCFv`, `__sinit_\TaskBase_cpp`), `lbl_80734628` -> `@stringBase0`, `lbl_807C75D0` ->
    `sTaskTag__3atn` (locales).
- Hallazgos MWCC
  - Con `-ipa file`, una llamada a una funcion sin efectos visible en el fichero cuyo resultado no se usa se
    elimina (assert de release): si el original la conserva, la funcion esta en otro fichero.
  - Los bucles `if (c) do {...} while (c)` del original (guarda + test al final) no salen de `for`/`while`.
  - Leer los flags en un local antes de varias pruebas evita las recargas; la funcion inline `IsKilled` (sin local)
    las fuerza cuando el original las tiene (fn_800D2338).
