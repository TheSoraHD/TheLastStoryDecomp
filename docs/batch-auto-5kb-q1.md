# Lote batch-auto-5kb - cola q1

Cola (de menor a mayor): `atn/Math_unk`, `atn/LipSync_unk`, `atn/Pad_unk`, `atn/LightUtil_unk`, `atn/Light_unk`,
`atn/PostFxBloom`, `atn/PostFxColorMatrix`, `atn/FileUtil_unk`, `atn/Geometry_unk`, `atn/BoneCalc_unk`.

## atn/Math_unk (commit 69e6097)

- Resultado: 1/1 al 100% (`__sinit`), `matching: true`.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**:
  - Split sin cambios (ya cubria `.text`, `.ctors`, `.bss` 0x807C7028-0x807C7090 y `.sdata2` 0x80880BC8-0x80880BD0).
  - `symbols.txt`: `fn_80081DC8` -> `__sinit_\Math_unk_cpp` (scope:local).
  - Los globales se mantienen como `extern "C" lbl_807C70xx` (otras unidades ya los declaran asi): vector 2D cero,
    `atn::Vector3` cero, dos `atn::Quaternion_unk` (cero e identidad), `atn::Matrix3x4` identidad (tipos inferidos por los stores).
  - `include/math/Vec.h`: anadidos ctores inline `Quaternion_unk(x, y, z, w)` y `Matrix3x4(12 floats)`.
  - Nombre real propuesto: `atn_math.cpp` (sin prueba directa).
- Hallazgos MWCC: objetos de `.bss` de mas de 8 bytes alineados a 8 dentro del fichero (Vector3 de 0xC ocupa 0x10).

## atn/LipSync_unk (commit eb61b3e)

- Resultado: 2/4 al 100%, `matching: false`.
- NONMATCHING:
  - `LipSyncData_unk::fn_8007A530` 99.22% - reparto de registros del bucle (offset, base del pool y clave intercambiados).
  - `Chara1154_unk::fn_8007A7BC` 99.70% - FPR del clamp (valor actual y `m2C` en f2/f3 intercambiados); ~12 variantes probadas.
- **Decisiones sin el usuario**:
  - Split sin cambios. Los dos `.sdata` 0x8087D760/0x8087D764 (palabras a cero que pasa `new[]` como nombre de fichero;
    tambien los usa `Event::Dialog`) quedan solo declarados `extern "C" char[4]`: MWCC manda a `.sbss` cualquier
    inicializador a cero (probado `char[4] = ""`, `{0}`, `int = 0`, `__declspec(section ".sdata")`), asi que la unidad no
    se puede enlazar aunque el codigo llegue al 100%. Pendiente: averiguar que construccion deja ceros en `.sdata`.
  - `symbols.txt`: `fn_8007A530` -> `fn_8007A530__15LipSyncData_unkFPCci`, `fn_8007A748` -> `__ct__14LipSyncKey_unkFv`,
    `lbl_80731900` -> `@stringBase0` (pool `"\0A\0I\0U\0E\0O"`, dtk lo veia como wstring). `pairnames.py` emparejo mal
    `fn_8007A748` con la copia debil de `CsvBase::vf08` (se corrigio a mano).
  - Cabecera nueva `include/atn/LipSync_unk.h` (`LipSyncKey_unk`, `LipSyncData_unk`, `Chara1154_unk` con campos);
    `Chara1154_unk` sale de `include/game/CharaTask.h` (que ahora incluye la nueva cabecera).
  - `include/atn/Model_unk.h`: `Track_unk::m14` pasa a `float` y se anade `float m1C` (antes relleno; sin otros usos).
  - Tipo inferido: `Event::DialogParam_unk` (src/Event/Dialog.cpp) es el mismo `LipSyncKey_unk` (ctor 0x8007A748 = {5, 0.0f},
    no {5, 1.0f} como dice su comentario); no se toca Dialog.cpp.
- Hallazgos MWCC: un objeto global con inicializador todo a cero va siempre a `.bss`/`.sbss`, incluso con
  `__declspec(section ".sdata")`; un pool de cadenas anchas de <= 4 bytes (`L""`) si va a `.sdata`.

## atn/Pad_unk (commit COMMIT_PAD)

- Resultado: 15/17 al 100%, `matching: false`.
- NONMATCHING: `fn_800A58D0` y `fn_800A58F8` 97% - solo registros: el original deja el gestor en r3 y la tabla en r5
  (como si r3 siguiera vivo al principio); ~8 variantes (local, accesor miembro/libre, puntero, referencia) sin exito.
- **Decisiones sin el usuario**:
  - `splits.txt`: anadido `.sbss 0x8087EF70-0x8087EF78` (el singleton `lbl_8087EF70`, solo lo escribe `fn_800A5508`).
    El hueco `.sbss 0x8087EF68` (singleton de NandManager, lo crea `fn_800A2C48`) sigue sin asignar: pendiente de NandManager.
  - `symbols.txt`: `lbl_80732930` (0x50) partido en `lbl_80732930`/`lbl_80732958` (0x28 cada uno; son dos tablas de ejes
    distintas: con un solo array se pierde la base comun de 3 estaticas y cambia el codigo); `lbl_80880CB8` tamano 0x8 -> 0x4
    (float + relleno, objdiff lo trataba como double); `lbl_80732980` -> `@stringBase0` (pool `""`).
  - Funciones como `extern "C"` libres con nombres `fn_` (otras unidades ya las declaran asi: D2AMpMenu*, MenuCommonTask);
    probablemente son metodos de una clase de mando vacia (`new (1, "", "", 0)` de 1 byte), `Pad_unk` inventada.
  - Tipos locales inferidos: `WPad_unk` (gestor de Wii Remote `lbl_8087F018`, banderas por mando en +0x3C00) y tablas
    `sPortMap`/`sButtonMap`(34)/`sAxisMap`/`sAxisMap2`.
- Hallazgos MWCC: nada nuevo (confirmado: tablas `static const` > 8 bytes alineadas a 8 en `.rodata`; base comun con >= 3 estaticas).
