# Lote batch-auto-5kb - cola q2

Cola (de menor a mayor): `atn/Texture_unk`, `atn/Se_unk`, `atn/CompositeCache_unk`, `atn/PostFxDof`,
`atn/SequenceHolder`, `atn/PostFxMotionBlur`, `atn/TextureDraw_unk`, `atn/PostFxDamage`, `atn/PostFxRain`.

## Arreglo previo del enlace (commit 0adbc2c)

- La copia integrada (q1 + q4 + q5) no enlazaba: q1 renombro `fn_800698DC` a
  `fn_800698DC__17DebugMessages_unkFPCc`, pero `src/atn/LH_Core.cpp` (enlazada) y `src/atn/FileSystem_unk.cpp`
  (de q4) lo seguian llamando como `extern "C"`. **Cambio fuera de mis unidades**: ambas declaran ahora
  `struct DebugMessages_unk { void fn_800698DC(const char*); }` y llaman al metodo (mismo codigo). `main.dol: OK`, lost: 0.

## atn/Texture_unk (commit 8524856)

- Resultado: 8/9 al 100%, `matching: false`.
- NONMATCHING: `fn_800D5A3C` 99.69% - es `fn_800D5B58(ancho, alto, 4, 0)` inlineada; el original carga ancho y alto
  en el orden contrario (r30/r29) y deja el tamano del bufer en r31; ~8 variantes (locales, puntero al gestor, array,
  cuerpo sin la inline: este ultimo da otro reparto peor).
- **Decisiones sin el usuario**:
  - Split sin cambios.
  - `symbols.txt`: `lbl_80734638` -> `@stringBase0` (pool `""` que se pasa como fichero/funcion a `fn_800839EC`);
    `lbl_808811D8` tamano 0x8 -> 0x4 (float + relleno).
  - `fn_800D58A4`, `fn_800D5908`, `fn_800D594C`, `fn_800D5A3C` quedan `extern "C"` con el puntero a la textura
    como primer argumento (Dialog, StrapTask, LibraryTask, NewStaffRoll, AutomaticDoor y CompositeIntensityTintMulti
    ya llaman asi a `fn_800D5908`); `fn_800D59B8` mantiene su nombre `GimmickUvOwner_unk::fn_800D59B8` (struct local
    vacio, se convierte `this` a `Texture_unk*`). Probablemente todas son metodos de `Texture_unk`.
  - `include/atn/Texture_unk.h`: `m2D`/`m2E` pasan a `signed char` (modos de wrap S/T: se leen con `lbz`+`extsb`
    en el orden del original solo si el campo es con signo; los demas usos solo guardan 1);
    `fn_800D5B58` devuelve `int` (siempre 1).
  - Nombre real propuesto: ninguno (sin cadenas).
- Hallazgos MWCC:
  - `(signed char)campo_unsigned` no reproduce `lbz`+`extsb` en el mismo orden que un campo `signed char`: el tipo del campo importa.
  - Una funcion que llama a otra autoinlineable del fichero con argumentos fijos (`fn_800D5A3C` -> `fn_800D5B58(w, h, 4, 0)`)
    se reconoce porque los registros de los parametros se crean antes que los locales del cuerpo inlineado.

## atn/Se_unk (commit d113613)

- Resultado: 11/11 al 100%, `matching: true` (enlazada, `main.dol: OK`).
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**:
  - Split sin cambios (`.text` + `.sdata2` 0x80881138-0x80881140 con 1.0f/0.0f).
  - `symbols.txt` sin cambios: todas las funciones quedan `extern "C"` con su nombre `fn_` (Event, gmk y game ya las
    declaran asi: `fn_800C3094`/`fn_800C3124` cargan/liberan grupos, `fn_800C31F4`/`fn_800C344C` devuelven `atn::SoundRef`).
  - Tipos locales inventados con solo los campos usados: `SoundEngineSe_unk` (lbl_8087EE90), `SoundManagerSe_unk`
    (lbl_8087EFE8, banderas 0x34A0/0x34B4/0x34B8) y `SoundObjectSe_unk` (`mSound` 0x04, `mA8`).
  - `fn_800C31F4` recibe un tercer entero que no usa (los llamadores pasan -1; se llama a `fn_800C32E0` con -1 literal).
  - Nombre real propuesto: ninguno (sin cadenas); parece la API de efectos de sonido del motor (`atn_se.cpp`?).
- Hallazgos MWCC:
  - `strlen` de `<string.h>` es intrinseca: el compilador mantiene un global en registro a traves de la llamada;
    declarada a mano (`extern "C" unsigned long strlen(const char*)`) se recarga el global despues, como en el original.
  - `bool v = f() - 1 != -1; if (v)` en el cuerpo da `subfic/addi/or/srwi.`; dentro de una inline o en el `if` directo
    se pliega a `cmpwi r3,0`.
  - Para el orden de ramas: `if (a == 0 || b == 0 || c == 0) return r;` da `beq` a la salida en cada termino;
    con `c` en un `if` aparte el termino anterior sale como `bne`+`b`.

## atn/CompositeCache_unk (commit 4bcced8)

- Resultado: 5/7 al 100%, `matching: false`.
- NONMATCHING:
  - `CompositeCacheEntry_unk::fn_800D8458` (Release) 99.04% - iteradores begin/end de la lista intercambiados en r30/r31 (~10 variantes del bucle; el
    vaciado como inline miembro `Purge_unk` de la cache arreglo el resto).
  - `CompositeCache_unk::fn_800D8698` (crear entrada) 97.99% - registros del `push_back` inlineado: el mismo problema
    conocido de `std/list.h` en las 12 copias de `LoadManager_unk::Get_unk`.
- **Decisiones sin el usuario**:
  - `splits.txt`: `.text` de `atn/CompositeCache_unk` reducido de 0x800D8894 a **0x800D87A4**. Las 9 funciones
    0x800D87A4-0x800D8894 (ctor/AddRef/Release/textura del handle, ctor de color RGBA, accesores de textura) no
    pueden ser de este fichero: `fn_800D87B0` llama fuera de linea a `fn_800D8448` (una sentencia) y `fn_800D87C4` a
    `fn_800D8458`, cosa imposible con `-ipa file` en el mismo fichero. Quedan como tramo `auto_` sin asignar.
    **Pendiente**: probablemente son el principio de `atn/CompositeIntensityTintMulti` (su fuente ya las trata como
    copias debiles de sus inline, `fn_800D8808`...); ampliar esa unidad hacia atras es mover un limite ajeno.
  - `symbols.txt`: `lbl_80734758` -> `@stringBase0` (pool `""` del `new`); `lbl_80881218` tamano 0x8 -> 0x4
    (FLT_EPSILON + relleno); `pairnames`: `fn_800D81CC` -> `__ct__Q23atn16CompositeKey_unkFPCcPCQ23atn22CompositeLayerDesc_unki`,
    `fn_800D82BC`/`fn_800D8448`/`fn_800D8458`/`fn_800D8624`/`fn_800D8698` con nombre mangled (los de 8448/8458/8624/8698
    coinciden con las declaraciones de `CompositeIntensityTintMulti.cpp`). Los `@LOCAL@`/`@GUARD@` de `Get_unk` y el
    registro `lbl_807C77E8` sin cambios.
  - `include/std/list.h`: anadidos `erase(iterator)`, `erase(first, last)` y `clear()` (miembros de plantilla: no cambian
    a los demas usuarios). Sin ctor/dtor en `std::list` para no tocar `LoadManager`/`PacketPack`: el ctor y el dtor
    (`clear`) estan en una jerarquia local `CacheListImp_unk`/`CacheListMid_unk`/`CacheList_unk` (cuatro comprobaciones
    de `this` en `~CompositeCache_unk`, como el `__list_imp` de MSL).
  - `include/atn/Texture_unk.h`: `m2F` pasa a `signed char` (`fn_800D8870` lo lee con `extsb`).
  - Tipos inventados (duplicados de los locales de CompositeIntensityTintMulti, que no se toca): `CompositeKey_unk` (0x188:
    hash del fichero base, numero de capas y 6 capas {hash, 3 colores, 3 usos}), `CompositeCacheEntry_unk` (0x1BC:
    referencias, clave, `Texture_unk` en 0x18C).
- Hallazgos MWCC:
  - `b = b && x;` en sentencias separadas reutiliza el registro de `b`; el original `t = eq && x1; t = t && x2; ...;
    eq = t && x4;` (un temporal distinto) se reconoce por `li r5,0`/`li r5,1` intermedios y el ultimo en el registro de `eq`.
  - Una comparacion de floats dentro de una inline que devuelve `bool` sale con `mfcr`; escrita en linea, con `bge`.
  - Las clases plantilla derivadas vacias con dtor emiten sus dtors debiles aunque nadie los llame (el enlazador los elimina).

## atn/PostFxDof (commit 951459c)

- Resultado: 2/3 al 100% (ctor y `vf10`, 0x5DC), `matching: false`.
- NONMATCHING: `PostFxDof::fn_800ACCB8` (reconstruye la tabla de profundidad 256x256 IA8) 97.47% - reparto de FPR de la
  conversion distancia -> profundidad del principio (n/f en f9/f3 en el original); ~12 variantes (locales, inline,
  orden de operandos). El bucle de la tabla y la limpieza de la esquina ya coinciden.
- **Decisiones sin el usuario**:
  - Split sin cambios (ya tenia `.rodata`, vtable/RTTI en `.data` y `.sdata2`; el dtor es la copia debil 0x800A91D4 de otro fichero).
  - `symbols.txt`: `fn_800AC6DC` -> `vf10__Q23atn9PostFxDofFPv` (entrada 4 de la vtable); `fn_800ACCB8` ->
    `fn_800ACCB8__Q23atn9PostFxDofFv`; `lbl_80732C00` -> `@stringBase0` (pool `""` de `fn_800839EC`).
  - Clase `atn::PostFxDof : IPostFx` (de `include/atn/PostFx.h`) con campos inferidos: `m04` (ultimo `mE0`), tres
    `GXTexObj` (copia reducida, copia desenfocada, tabla), `mTable` y las cuatro profundidades de la tabla (`m6C`-`m78`, -1 al crear).
  - Structs locales por offset: `DofParams_unk` (lbl_8087EFA8: 0xD8-0x100) y `DofFrameBuffer_unk` (lbl_8087EFB4:
    near/far 0x1CC/0x1D0, texturas de profundidad 0x74C y color 0x77C). Funciones GX que faltaban declaradas en el
    `.cpp` (no en `GXDecl.h`: hay declaraciones locales incompatibles de `GXGetTexBufferSize`/`DCFlushRange` en otros ficheros).
- Hallazgos MWCC:
  - `float m[2][3]` rellenado elemento a elemento en orden reproduce `lfs f0/f1` + `stfs`; con inicializador de llaves
    MWCC copia un objeto de `.rodata` con `lwz`/`stw`.
  - Un `GXColor c = {..}` constante declarado dentro del `if` donde se usa va igualmente a `.sdata2` en el orden del primer uso.
  - Los `if/else` de saturacion escritos en linea (no en inline) cambian el reparto de FPR del bucle.

## atn/SequenceHolder (commit 88106bb)

- Resultado: 7/11 al 100%, `matching: false`.
- NONMATCHING / sin escribir:
  - `SequenceHolder::SequenceHolder` 99.44% - el `reserve(0x80)` inlineado: puntero nuevo y numero de elementos movidos
    intercambiados en r29/r30 (3 variantes).
  - `SequenceHolder::fn_800C3990` (anade una unidad) 97.25% - `push_back` inlineado (el problema conocido de
    `std/vector.h`: el original carga la capacidad antes que el tamano) y el pool del fichero tiene `""` antes que
    `"vector length error"` (en el original al reves).
  - `fn_800C3D74` (busqueda por nombre) y `fn_800C3E84` (por id, con `__ptmf_scall` a `fn_800C3D6C`) sin escribir:
    son `std::find_if` con predicados de **boost::lambda** (el predicado se copia 4 y 7 veces en la pila); boost::lambda
    no esta en `libs/boost`.
- **Decisiones sin el usuario**:
  - Split sin cambios. `fn_800C37FC` (al principio del split) es un metodo de `SequenceControl` (usa `mTable`; lo llaman
    Bullet y SequenceUnitParam1 con `SequenceControl::sInstance`): se define aqui como `SequenceControl::fn_800C37FC`
    y se declara en `include/atn/SequenceControl.h` (sin cambiar el layout). Podria ser la ultima funcion de
    `SequenceControl.cpp`, que ya esta enlazada: no se mueve.
  - `symbols.txt`: `fn_800C37FC__Q23atn15SequenceControlFPCc`, `__ct__Q23atn14SequenceHolderFv`,
    `__dt__Q33atn14SequenceHolder16SequenceUnitBaseFv`, `fn_800C3990__Q23atn14SequenceHolderFRCQ33atn14SequenceHolder12SequenceUnit`,
    `fn_800C3D6C__Q33atn14SequenceHolder16SequenceUnitBaseCFv`, `vf0C__Q33atn14SequenceHolder12SequenceUnitFPQ23atn4Task`,
    `__dt__Q33atn14SequenceHolder12SequenceUnitFv`, `__sinit_\SequenceHolder_cpp` (scope:local) y `lbl_8073385C` ->
    `@stringBase0` (tamano 0x15: `"vector length error"` + `""`). `fn_800C3D74`/`fn_800C3E84`/`fn_800C3FB0` quedan
    `extern "C"` (SequenceControl.cpp, enlazada, llama asi a las dos ultimas).
  - Tipos inferidos: `SequenceHolder` {contador, `std::vector<SequenceUnitBase*>`}; `SequenceUnitBase` {id, nombre
    char[0x40]} con `vf0C` que devuelve 0 (su copia debil es la 0x800C37D8 que `symbols.txt` llama
    `SequenceControl::fn_800C37D8`); `SequenceUnit` con `boost::function<Task*(Task*)>` en 0x48 (con `function1` el
    destructor tiene una comprobacion de `this` menos).
  - Los objetos `.sbss` 0x8087EFC8-0x8087EFDC son `free1..3` y las referencias `_1.._3` de boost::lambda: se reproducen
    con structs vacios locales (el `__sinit` coincide).
  - `include/std/vector.h`: `reserve` despacha a `__reserve_unk(n, U**)` para punteros, con un bufer nuevo
    `__vec_raw_buf_unk` {data, size, cap} que mueve los bytes y pone el tamano a 0 antes de liberar; el caso general
    no cambia (D2A_Message sin cambios, regress lost 0).
- Hallazgos MWCC:
  - `T& r = obj;` a nivel de espacio de nombres se inicializa en el `__sinit` (dos `li`/`stw` por referencia), como los placeholders de boost::lambda.
  - `boost::function<R(A)>` y `boost::function1<R, A>` no generan el mismo destructor: la forma portable anade un nivel de herencia.

## atn/PostFxMotionBlur (commit c984be9)

- Resultado: 2/3 al 100% (ctor y `fn_800AFEBC`), `matching: false`.
- NONMATCHING: `PostFxMotionBlur::vf10` 99.78% - solo registros: el bloque de parametros/el flag al principio (r4/r6)
  y los FPR del bucle de desvanecimiento; ~14 variantes. Estructura, pila y `.sdata2` ya coinciden.
- **Decisiones sin el usuario**:
  - Split sin cambios.
  - `symbols.txt`: `fn_800AFEBC` -> `fn_800AFEBC__Q23atn16PostFxMotionBlurFi` (reserva el anillo de texturas para un
    divisor), `fn_800AFFEC` -> `vf10__Q23atn16PostFxMotionBlurFPv`, `lbl_80732CE0` -> `@stringBase0` (pool `""`),
    `lbl_80880E58` tamano 0x8 -> 0x4 (0.7f + relleno).
  - Clase `PostFxMotionBlur : IPostFx` con campos inferidos: anillo de 4 `GXTexObj` + 4 imagenes, indice del ultimo
    fotograma (-1), numero de fotogramas viejos (max 3), exponentes de la curva de pesos, flag/factor de fundido y divisor.
  - Structs locales por offset: `MotionBlurParams_unk` (lbl_8087EFA8 0x240-0x260), `MotionBlurFrameBuffer_unk`
    (textura de color 0x74C) y `MotionBlurColor_unk` (con ctor de copia que carga los 4 floats antes de guardarlos,
    como `Color4f_unk` de CompositeIntensityTintMulti).
- Hallazgos MWCC:
  - Un color pasado por valor que se copia con `lfs`/`stfs` (no `lwz`/`stw`) indica un ctor de copia explicito.
  - `x / 4.0f` sale como `fmuls x, 0.25` igual que `x * 0.25f`, pero con otro orden de operandos (y de `fnmsubs`).
  - Declarar un local `float` del cuerpo del bucle antes del acumulador (`float a; ... float sum = 0.0f;`) intercambia sus FPR salvados.

## atn/TextureDraw_unk (commit 71dd1dc)

- Resultado: 3/4 al 100%, `matching: false`.
- Sin escribir: `fn_800D5F68` (0x930: matriz de proyeccion de textura de las pasadas de luz/sombra segun un modo 1-4,
  con copias `Matrix3x4` por psq, `PSVECCrossProduct`/`PSVECNormalize`/`PSMTXConcat` y una estatica local con guarda en
  `.bss` 0x807C75F4/`.sbss` 0x8087EFF8). No escrita por tiempo; es la unica que usa los `.bss`/`.sbss` del split.
- **Decisiones sin el usuario**:
  - Split sin cambios.
  - `symbols.txt` sin cambios: las cuatro funciones quedan `extern "C"` con su nombre (`fn_800D5C84` matriz UV 2D con
    escala/rotacion/desplazamiento; `fn_800D5D00` = `C_MTXLightPerspective`; `fn_800D5E18` = `C_MTXLightOrtho`, ambas
    con `memset` previo; las llaman GraphicsDevice, ModelDraw_unk, PostFxDamage, LayerShadow y RenderSystem_unk en asm).
  - Nombre real propuesto: ninguno (sin cadenas); el contenido es de matrices de textura, no de dibujo.
- Hallazgos MWCC: reutilizar un unico `tmp` (como el SDK) en vez de dos locales cambia el orden de planificacion de las divisiones y los stores.
