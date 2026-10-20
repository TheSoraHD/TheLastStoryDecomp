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

## atn/CompositeCache_unk (commit HASH3)

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
