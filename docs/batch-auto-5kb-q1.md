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

## atn/Pad_unk (commit b8d3240)

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

## atn/LightUtil_unk (commit 14a12c7)

- Resultado: 4/7 al 100%, `matching: false`.
- NONMATCHING:
  - `LightList_unk::fn_8007A154` 98.73% - el original vuelve a leer `mCount` tras la comprobacion de rango (aqui se reutiliza); ~8 variantes.
  - `LightSet_unk::fn_8007A36C` / `fn_8007A41C` 99.50% - orden de los dos incrementos del bucle (indice antes que offset).
- **Decisiones sin el usuario**:
  - `splits.txt`: anadido `.sbss 0x8087EEF8-0x8087EF00` (singleton `lbl_8087EEF8` de la lista de luces, lo crea `fn_8007A100`).
  - `symbols.txt`: `lbl_807318F0` -> `@stringBase0` (pool `""`); funciones con nombre mangled por `pairnames`
    (`__ct__12LightSet_unkFv`, `fn_80079EFC__12LightSet_unkFP9Light_unkf`, `fn_8007A154__13LightList_unkFi`,
    `fn_8007A36C__12LightSet_unkFP13LightList_unkRCQ23atn7Vector3fi`, `fn_8007A41C__12LightSet_unkFP13LightList_unk`,
    `fn_8007A4CC__13LightList_unkFv`); `fn_8007A100` queda como `extern "C"`.
  - Cabecera nueva compartida `include/atn/Light_unk.h`: `Light_unk` (0x44, color RGBA en +0x34 copiado como struct),
    `LightList_unk` {datos, cuenta, capacidad}, `LightSet_unk` (4 luces ordenadas por distancia, 0x24). `LH_Scene.cpp` y
    `NewCampPartsGmk.cpp` tienen copias locales de `Light_unk` que no se han tocado.
  - `fn_8007A36C` recibe 3 argumentos mas que no usa (posicion, radio, entero: los pasa `ModelDraw_unk`).
- Hallazgos MWCC: la copia implicita de un struct con `atn::Vector3` sale con la z cargada primero (`lfs` antes del `psq_l`),
  aunque el `operator=` de Vector3 use `__VecCopy`.

## atn/Light_unk (commit d2f4d39)

- Resultado: 7/10 al 100%, `matching: false`.
- NONMATCHING:
  - `Light_unk::fn_80079280` 99.74% - FPR de la formula del radio (b2/a2 intercambiados, orden de operandos de `(4*c2)*k`).
  - `Light_unk::fn_800795DC` 96.52% - planificacion de los argumentos de la llamada tras convertir el color.
  - `Light_unk::fn_80079994` 76% - el original niega `mDir` con `fneg` (valores tratados como double) pero `-1.0f` sigue
    en `.sdata2` antes de `1000000.0f`; escrito `-1.0f * x` se respeta el orden del pool (y `fn_80079BC8` llega al 100%)
    pero sale `fmuls`. Ademas el prologo va intercalado con la conversion del color.
- **Decisiones sin el usuario**:
  - Split sin cambios.
  - `symbols.txt`: `lbl_807318A0` -> `@stringBase0`; `fn_80079090` -> `__ct__9Light_unkFP13LightData_unk` (es un ctor:
    devuelve `this`) y los demas metodos con nombre mangled de `Light_unk`.
  - **Cambio fuera de la unidad**: `src/atn/LH_Scene.cpp` llamaba a `extern "C" Light_unk fn_80079090(void*)`; ahora declara
    el ctor `Light_unk(LightData_unk*)` y hace `Light_unk light(...); mLights[i] = light;` (sigue al 100%
    `LoadScene::fn_804778DC`, comprobado). `src/atn/LightUtil_unk.cpp` llama ya a `Light_unk::fn_800795DC`.
  - `include/atn/Light_unk.h` ampliada: `SceneTrack_unk` (duplicada de la local de LH_Scene.cpp), `LightData_unk`
    (pistas de la luz en el fichero de escena), `LightColor_unk` con ctor, campos de `Light_unk` con nombre
    (`mType`, `mPos`, `mDir`, `mAttnType`, `mRadius`, `mRefDist`, `mRefBright`, `mAngle`, `mColor`).
  - `include/atn/GXDecl.h`: anadidos `GXLightObj` y `GXInitLight{Attn,Spot,DistAttn,Pos,Dir,Color}`.
- Hallazgos MWCC:
  - El resultado de una llamada que devuelve `float` se trata como double si se guarda en un local `float`: usarlo en una
    resta mete `frsp`, guardarlo en memoria no (`stfs` directo).
  - `x = (double)f;` (f float) genera `frsp` + `stfs`; sirve para un `frsp` aislado antes de un store.
  - Un temporal devuelto por una inline (`ToGXColor_unk()`) va por debajo de la copia del argumento por valor; un local con nombre por encima.
  - `!(a != 2 && a != 3)` evita la fusion en `cmplwi` de `a == 2 || a == 3` (dos `cmpwi` + `beq`/`bne`).

## atn/PostFxBloom (commit COMMIT_BLOOM)

- Resultado: 1/3 al 100% (ctor), `matching: false`.
- NONMATCHING:
  - `fn_800A981C` (pasada de desenfoque direccional, `extern "C"`: la llama tambien Layer2DBlur) 96.87% - registros
    (el color K1 se queda en registro durante la primera llamada; contadores del bucle TEV en r25/r26).
  - `PostFxBloom::vf10` 85.65% - registros y las expresiones u16 de los tamanos de copia; el original guarda mas valores en f28-f31.
- **Decisiones sin el usuario**:
  - Split sin cambios (ya incluia RTTI/vtable y el RTTI debil de `atn::IPostFx`).
  - `symbols.txt`: `fn_800A9E64` -> `vf10__Q23atn11PostFxBloomFPv` (entrada 4 de la vtable).
  - Cabecera nueva `include/atn/PostFx.h` (IRender con `vf14`/`vf18` no puros como en la vtable, `IPostFx`,
    `PostFxManager::fn_800A96AC`, globales de render). Jerarquia inferida del RTTI: `PostFxBloom : IPostFx : IRender`.
  - `include/atn/GXDecl.h`: anadidas funciones TEV/K-color/`GXLoadTexMtxImm`/`GXInvalidateTexAll`.
  - Parametros de bloom (`lbl_8087EFA8`) con nombres inferidos en un struct local (`BloomParams_unk`).
- Hallazgos MWCC: la cadena `m[..] = ... = 0.0f` de `Matrix3x4::SetIdentity` reproduce el orden de stores del original
  (de la ultima a la primera); `float s = 0.0f; for (5) s += w[i];` mantiene el `0.0f +` que una expresion unica pliega.
