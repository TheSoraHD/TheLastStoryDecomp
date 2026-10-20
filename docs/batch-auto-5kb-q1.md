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
    se puede enlazar aunque el codigo llegue al 100%. Explicacion probable: el original se compilo con `#pragma explicit_zero_data on` (o equivalente); ver PostFxColorMatrix.
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

## atn/PostFxBloom (commit ed4d408)

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

## atn/PostFxColorMatrix (commit da4ffaa)

- Resultado: 2/4 al 100% (ctor, `fn_800AAE20`), `matching: false`.
- NONMATCHING:
  - `fn_800AA8DC` (actualizacion/interpolacion) 96.85% - registros FPR de los factores y planificacion de la desaturacion.
  - `PostFxColorMatrix::vf10` 76% - estructura aproximada (busqueda de escala y colores K con temporales Vec4); no se ha iterado.
- **Decisiones sin el usuario**:
  - Split sin cambios.
  - `symbols.txt`: `fn_800AAE2C` -> `vf10__Q23atn17PostFxColorMatrixFPv`.
  - Los 10 floats de `.sdata` 0x8087D828-0x8087D850 (pares 0.0/1.0 usados como limites de clamp) quedan solo declarados
    (`extern "C"`): MWCC solo deja un 0.0f en `.sdata` con `#pragma explicit_zero_data` (prohibido); la unidad no se
    podria enlazar aunque el codigo llegase al 100%. Pendiente de decision del usuario.
  - Tipos locales inventados: `ColorVec4_unk` (copia paired-single con `__Vec4Copy`), `ColorMatrix_unk` (0x50),
    `ColorMatrixOwner_unk` (devuelto por `fn_800C1424`).
- Hallazgos MWCC:
  - Orden de inicializacion en una ctor inline (lista de inicializacion vs cuerpo) cambia la planificacion de los stores
    de los miembros escalares respecto a las copias psq: con la lista de inicializacion sale el orden del original.
  - `float a[4] = {x, y, z};` inicializa a cero con `stw` enteros las 4 palabras antes de escribir x, y, z.
  - Un clamp contra variables globales escrito como expresion (no inline con parametros) carga cada limite justo antes de su comparacion.

## atn/FileUtil_unk (commit dcf2d82)

- Resultado: 7/16 al 100%, `matching: false`.
- NONMATCHING: codificador CMPR `fn_80068DB0` 92.9%, `fn_80069060` 78.8%, `fn_80069288` 77.9%, `fn_8006945C` 73.7%
  (registros/planificacion; no iterado a fondo); `fn_8006969C`/`fn_800696B4` 99% (solo el tamano del objeto de `.sdata2`);
  `DebugMessages_unk::fn_800698DC` 99.71% y `fn_80069E50` ~90% (registros); `basic_string(const basic_string&, pos, n)`
  82% (ya NONMATCHING en `string_members.h`).
- **Decisiones sin el usuario**:
  - `splits.txt`: anadido `.sbss 0x8087EEB8-0x8087EEC0` (singleton `lbl_8087EEB8` de la lista de mensajes, solo lo crea `fn_800696C8`).
  - `symbols.txt`: pool `lbl_80731398` -> `@stringBase0`; metodos de `DebugMessages_unk`/`DebugMessage_unk` con nombre
    mangled (`fn_800697D8__17DebugMessages_unkFPCc`, `fn_800698DC...`, `fn_80069D48...`, `fn_80069E50__17DebugMessages_unkFi`,
    ctor/dtor de `DebugMessage_unk`). Se mantiene el nombre `__dt__Q23gmk18MerchantString_unkFv` (copia debil que usa RandomMerchant).
  - **Cambios fuera de la unidad**: `src/gmk/RandomMerchant.cpp` y `src/game/CharaTask.cpp` llamaban a `extern "C" fn_800697D8(void*, const char*)`;
    ahora declaran `struct DebugMessages_unk { void fn_800697D8(const char*); }` y llaman al metodo (regress: lost 0).
  - `include/std/string_members.h`: anadida la definicion de `basic_string::find(const T*, pos, n)` (0x8006A004, al 100%).
  - Nombre real propuesto: no hay; el fichero mezcla el codificador CMPR, la prueba de orden de bytes y la lista de mensajes
    de depuracion: puede que sean 2-3 ficheros (sin pruebas de corte).
- Hallazgos MWCC:
  - Un `static const` escalar se pliega siempre; para conservar el `lhz` del original hubo que leerlo por un puntero `volatile`.
  - Los objetos de mas de 4 bytes van a `.rodata` (no a `.sdata2`) con `-sdata2 4`: el tamano 8 de dtk es relleno.

## atn/Geometry_unk (commit 36c31b8)

- Resultado: 4/10 al 100%, `matching: false`.
- NONMATCHING: `fn_8006FAB0` (rayo contra caja) 98.09% (FPR de constantes izadas); `fn_80070D04` (cilindro contra esfera)
  97.41%; `fn_80070B60` (escalar caja) ~58%; `AABB_unk::Merge_unk` 0% y `fn_8006FEBC`/`fn_800704DC` (proyeccion de la caja
  a pantalla) sin escribir (solo declaradas).
- **Decisiones sin el usuario**:
  - Split sin cambios.
  - `symbols.txt`: `lbl_80880A68` tamano 0x8 -> 0x4 (float + relleno).
  - Las funciones se dejan `extern "C"` con su nombre `fn_` (las declaran asi AABBTree, MapPartsModel, MapManager,
    LensFlare, ModelInstance); `AABB_unk` local como en `atn/AABBTree.cpp`.
  - **Pendiente**: `Merge_unk`, `fn_8006FEBC` y `fn_800704DC` usan un min/max paired-single en asm (`psq_lu`/`ps_sub`/
    `ps_sel`/`fsel`, tipo `VECMin`/`VECMax`) que no existe en `include/math/Vec.h`; anadirlo seria ampliar la excepcion
    de asm de Vec.h (necesita permiso del usuario).
- Hallazgos MWCC: `(float)__fabs(x) < eps` da `fabs` + `frsp` + `fcmpo` con el eps en `.sdata2` (sin el cast compara en double y lo saca a `.rodata`).

## atn/BoneCalc_unk (commit 76c9816)

- Resultado: 1/3 al 100% (`fn_8009B938`, funcion vacia), `matching: false`.
- NONMATCHING / sin escribir: `fn_8009A5CC` (0x7D8) y `fn_8009ADA4` (0xB94): calculo recursivo de matrices de huesos con
  `boost::function<>` locales (vtable de `boost::bad_function_call`), punteros a miembro (`__ptmf_null` y las tres
  constantes `{0, -1, fn_8009B938}` de `.data`), angulos de Euler via `fn_8072D210` (128/pi y medio angulo) y
  `PSMTXConcat`/`PSMTXMultVec`. No se escribieron por tiempo (son las dos funciones mas grandes de la cola).
- **Decisiones sin el usuario**: split sin cambios; `fn_8009B938` queda `extern "C"` (su clase es desconocida).
