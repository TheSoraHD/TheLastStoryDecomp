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
