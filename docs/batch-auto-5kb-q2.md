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

## atn/PostFxDamage (commit 0ac9e10)

- Resultado: 1/3 al 100% (ctor), `matching: false`.
- NONMATCHING:
  - `fn_800AB774` (desenfoque giratorio: 5 pasadas con escala 1-4 y rotacion 0, 1, 2, -1, -2 veces el angulo; el reparto
    de pesos es el de `fn_800A981C` de PostFxBloom mas la correccion de redondeo `k0.r += 255 - 5*c0`) 97.93% -
    huecos de pila de los colores K y registros del bucle TEV; ademas `.sdata2` sale con 3.0f antes que -1.0f
    (el original al reves; 3 variantes del orden de argumentos sin exito).
  - `PostFxDamage::vf10` 98.11% - registros de tamanos/imagenes al principio, el `shift` del tinte sale con `mfcr`
    en vez de ramas y los huecos del color K intercambiados (~12 variantes).
- **Decisiones sin el usuario**:
  - Split sin cambios.
  - `symbols.txt`: `fn_800ABD94` -> `vf10__Q23atn12PostFxDamageFPv`; `lbl_80732BD4` -> `@stringBase0`
    (`"slime_edge.texture"`); estaticas locales de `vf10`: `lbl_807C7410` -> `@LOCAL@vf10__Q23atn12PostFxDamageFPv@sEdge@0`
    (`Texture_unk`), `lbl_807C7440` -> `@LOCAL@...@sBlur@1` (`GXTexObj`), `lbl_8087EF90` -> `@LOCAL@...@sLoaded`,
    `lbl_8087EF94` -> `@GUARD@...@sEdge@0` (todas `scope:local`); `lbl_80880DA0` tamano 0x8 -> 0x4.
    `fn_800AB774` queda `extern "C"` (solo la llama vf10).
  - Structs locales: `DamageParams_unk` (lbl_8087EFA8 0x378-0x394: debug, tinte RGBA, angulo/longitud del desenfoque,
    zoom del borde), `DamageColor_unk` con ctor de copia de floats.
  - `src/atn/LinearAllocator.cpp` tiene su copia local de la clase (`IRender` como base); aqui se usa `IPostFx` de PostFx.h (el RTTI lo exige).
- Hallazgos MWCC:
  - `static int sLoaded;` declarado antes del `static Texture_unk sEdge;` coloca `sLoaded` antes que la guarda de `sEdge` en `.sbss`.
  - Una matriz local con stores en orden inverso de los ceros se escribe como cadena `m01 = m02 = ... = 0.0f` (la cadena guarda de derecha a izquierda).

## atn/PostFxRain (commit HASH9)

- Resultado: 2/8 al 100% (`PostFxRainDrop_unk::PostFxRainDrop_unk` vacio y `fn_800B0A48`), `matching: false`.
- NONMATCHING (todas con la estructura y los datos ya correctos, `.sdata2` en el mismo orden):
  - ctor 97.74% - store muerto del puntero tras `__construct_new_array` y registros del bucle de limpieza.
  - `fn_800B0A70` (dibuja las gotas) 98.61%, `fn_800B1554` (Reset) 97.65% - registros de los bucles.
  - `fn_800B0CD4` (ondas de calor) ~76% - disposicion de la pila: el original deja los 4 colores en orden ascendente
    por encima de las copias de argumentos y el vector/matriz de escala por debajo, como locales de una inline;
    probado una inline `DrawGradient_unk` y el orden inverso de declaracion sin exito.
  - `vf10` 99.65% - `this`/tamano de pantalla en otros registros salvados y los dos colores de `GXSetCopyClear` en otros huecos.
  - `fn_800B1598` (update) 99.65% - desplazamiento y puntero de gota del ultimo bucle intercambiados en r6/r5.
- **Decisiones sin el usuario**:
  - Split sin cambios. Los dos `.sdata` 0x8087D870/0x8087D874 (palabras a cero pasadas a `new[]` como fichero/funcion,
    como los de LipSync_unk de q1) quedan solo declarados `extern "C" char[4]`: MWCC no deja un objeto a cero en
    `.sdata`, asi que la unidad no se podra enlazar aunque el codigo llegue al 100% (pendiente, igual que en q1).
  - `symbols.txt`: `fn_800B0A44` -> `__ct__18PostFxRainDrop_unkFv`, `fn_800B0A70`/`fn_800B0CD4` -> metodos de
    `atn::PostFxRain`, `fn_800B105C` -> `vf10__Q23atn10PostFxRainFPv`, `lbl_80732D00` -> `@stringBase0`
    (`"heat_waves01.texture"`, `"drop_warp.texture"`), `lbl_80880EC0` tamano 0x8 -> 0x4.
  - Layout inferido de `PostFxRain` (0xE8; coincide con la copia local de `src/atn/LinearAllocator.cpp`): flags de gotas
    y calor, intensidades suavizadas (0x0C/0x10/0x20), punteros a los ajustes activos (0x14 gotas, 0x54 calor:
    `lbl_8087EFA8`+0x2AC/+0x2BC o la sustitucion de `fn_800C1424`+0x1E4/+0x1F8), desplazamiento de las ondas,
    `GXTexObj` indirecta (0x60), array de 256 gotas de 0x1C (x, y, w, h, alfa, tiempo, estado) y dos `Texture_unk`.
  - `fn_800BFCB4` recibe tambien el ancho y alto de pantalla (r8/r9) tras los 4 colores.
- Hallazgos MWCC:
  - Los literales de `.sdata2` siguen el orden del texto del fuente, incluidas las de inlines plantilla (`Max_unk`).
  - Un bucle sobre `float m[2][3]` escrito como dos bucles anidados (2x3) se desenrolla por 3 con `ctr` = 2, igual que el original; un bucle plano de 6 se desenrolla entero.
