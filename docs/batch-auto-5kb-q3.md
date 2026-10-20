# Lote batch-auto-5kb - cola q3

Cola (de menor a mayor): `atn/EffectManager_cont`, `atn/D2A_ParamTable_unk`, `atn/EfpHandle_unk`,
`atn/NumberFormat_unk`, `atn/Util_unk`, `atn/D2A_MessageTable_unk`, `atn/EffectSequencer_unk`, `atn/EffectRes_unk`,
`atn/EffectLoader_unk`, `atn/LoadManager_unk`.

## Arreglo previo fuera de la cola (commit d0c4289)

- La copia integrada (q1+q4+q5+q7+q9) no compilaba: `src/MP/SaveLoad.cpp` (q9) declaraba `fn_8004B20C` con otro tipo
  que `include/game/GameSaveLoadBackgroundCommon.h` (otra cola) y llamaba a `fn_803BE8C0` como `extern "C"`, que la
  otra cola habia renombrado a `GameSaveSlot_unk::fn_803BE8C0`. Arreglo: `typedef SaveTime_unk CalendarTime_unk` y
  llamada como metodo sobre la cabecera de 0xB8 de `SaveWork_unk`. `MP/SaveLoad` sigue 8/8 al 100%.

## atn/EffectManager_cont (commit bc7b982)

- Resultado: 2/2 al 100%, unidad propia `matching: true` (no fusionada). `main.dol: OK`.
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**
  - No se fusiona con `atn/EffectManager`: entre ambas esta `std/char_traits_char_8023C080` (0x8023B9D0-0x8024372C,
    unidad ajena en `game_units.json`), que segun `splits-auto-B` es parte del mismo fichero real. Ademas
    `atn/EffectManager` esta en `matching: false` (no esta enlazada). **Pendiente para el usuario**: fusionar las
    tres unidades en `atn/EffectManager` (0x80239798-0x80243758) cuando esa unidad y la parte std esten al 100%.
  - Las dos funciones son los thunks de ajuste de `this` de `EffectManager` para su base `IRender` en 0x48
    (`@72@vf10__Q23atn13EffectManagerFP10Camera_unk` y `@72@__dt__Q23atn13EffectManagerFv`; nuestro
    `EffectManager.o` los emite debiles al final). Como unidad aparte se escriben como llamadas de cola normales
    (`extern "C"`, nombres `fn_8024372C`/`fn_80243734` sin cambiar en `symbols.txt`); al fusionar habra que borrarlas.
  - Splits: `atn/EffectManager_cont` .text 0x8024372C-0x80243758 -> 0x8024372C-0x8024373C; los 0x1C bytes de
    `EfpData_unk::EfpData_unk()` (0x8024373C) pasan a `atn/EffectSequencer_unk` (0x80243758 -> 0x8024373C): los
    thunks cierran el fichero de EffectManager y `EfpData_unk` tiene mas metodos dentro de EffectSequencer
    (`fn_802437B4__11EfpData_unkFPCc`).
  - `game_units.json`: entrada nueva `atn/EffectManager_cont.cpp` tras `std/char_traits_char_8023C080.cpp`.
- Hallazgos MWCC
  - MWCC convierte en salto de cola (`addi r3,r3,-0x48; b f`) una funcion que solo llama a otra con el puntero
    ajustado: sirve para reproducir thunks `@N@` fuera de su fichero. Un parametro `short` reenviado anade `extsh`:
    usar `int`.

## atn/D2A_ParamTable_unk (commit d6e5655)

- Resultado: 6/6 al 100%, `matching: true`, `main.dol: OK`.
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**
  - Splits: sin cambios (el tramo ya cubre `.text`, el double int->float de `.rodata` y su `.sdata2`).
  - `symbols.txt`: `lbl_80882C28` (0.0f) pasa de `size:0x8` a `size:0x4` (los 4 bytes siguientes son relleno;
    con 8 objdiff lo trataba como double y no casaba el `lfs`).
  - Contenido: tablas `{clave, valor}` de los elementos D2A con setter (busca la clave exacta) y getter interpolado
    linealmente entre las claves vecinas, para `int` (`fn_801F0364/03AC`), `float` (`D2A_ParamTable::fn_801F04FC`,
    `fn_801F0544`) y `std::wstring` (`fn_801F0670/0758`, sin interpolar: valor de la clave anterior). Tipos
    inferidos locales `D2A_IntParamTable_unk` y `D2A_StringParamTable_unk`. Los nombres `extern "C"` se mantienen
    (los llaman asi D2A_Common, D2A_Message, D2A_Data, D2A_Texture y MenuCommonTask); `fn_801F04FC` conserva su
    firma `(int key, int arg, float value)` aunque por los llamadores parece `(key, value, flag)` (mismos registros).
  - **Cabecera compartida `include/std/string.h`**: `operator=` copia dos cadenas cortas con `mRaw = str.mRaw` en vez
    de `mLong = str.mLong` (el original carga la palabra 1 antes de guardar la 0, como el ctor copia). Efecto en el
    resto (report.json): suben `db/CharaParam_unk` tokenizer::assign (95.9 -> 99.7) y `LibraryTask`
    `BookTask::fn_8046B798` (96.2 -> 97.0); bajan un poco `D2A_Message` ctor (66.54 -> 66.31) y `vf18`
    (91.59 -> 91.58), ambas ya NONMATCHING. Ninguna funcion al 100% se pierde.
  - Nombre real propuesto: entre `atn_d2a_frame` y `atn_d2a_line` (ver splits-auto-B), p. ej. `atn_d2a_param.cpp`.
- Hallazgos MWCC
  - Locales de un bucle de busqueda (`prev`, `next`, `found`): el orden de declaracion decide r7/r8/r9; probar las
    6 permutaciones con un script es mas rapido que razonarlo.
  - Temporales (tags de iterador) en orden ascendente en la pila = el codigo esta en una inline (aqui la seleccion
    del valor anterior/siguiente): sacarlo a un helper inline los reordena.

## atn/EfpHandle_unk (commit 9fdd1e7)

- Resultado: 14/15 al 100%, `matching: false`. `main.dol: OK`.
- NONMATCHING: `fn_802371BC` 74.74% - intercambiador de bytes guiado por una tabla `{tipo, cuenta, sub}`
  (tipos 0 saltar, 1/2/3 invertir 16/32/64 bits, 4 recursion). Conseguidos: la estructura del `switch`, el
  desenrollado (`while (count-- > 0)`), `stwbrx` en el bucle de 32 bits con las mismas direcciones y el bucle de
  64 bits desenrollado x2 con los stores muertos en pila (struct con array). Falla: el bucle de 16 bits sale con
  `lhzu` (forma con actualizacion) en vez de desplazamientos + direcciones aparte, el orden del cuerpo de 64 bits y
  los registros salvados (r26-r31 frente a r25-r31). ~20 variantes probadas.
- **Decisiones sin el usuario**
  - Splits: sin cambios (el tramo cubre `.text`, `.ctors`, `.bss` de los registros de destructor y `.sbss`).
  - `symbols.txt`: `fn_802378E0` -> `__sinit_\EfpHandle_unk_cpp` (pairnames). Renombrados a mano (necesarios
    para que el `__sinit` reproducido enlace con nombre): `fn_8068B770`/`fn_8068BC30` ->
    `__ct__/__dt__Q33std8ios_base4InitFv` (construye cin/cout/cerr sobre `__files`: es el `ios_base::Init` de
    `<iostream>` de MSL) y `fn_8068BD60`/`fn_8068C240` -> `__ct__/__dt__Q23std11__wInit_unkFv` (nombre INVENTADO
    para el equivalente ancho). El mismo `__sinit` aparece en CryptMP, NetGlobals_unk, func_enter_setting,
    EffectSequencer_unk...: el fichero incluye `<iostream>`.
  - `include/atn/ResLoader_unk.h`: `m04[8]` pasa a `atn::LH_Effect mLoader` (mismo tamano 0xC; incluye
    `atn/LH_Effect.h`). Comprobado en report.json: ningun otro porcentaje cambia.
  - Contenido: `ResLoader_unk` (recurso + `LH_Effect`) y `atn::EfpHandle_unk` (recurso + `LH_Efp`): ctor, dtor
    (debil de `LH_Effect` emitido aqui), cargar, comprobar carga, liberar (avisando a `EffectManager`). Los
    accesores `fn_80237784`/`fn_8023779C`/`fn_802378C4` se quedan `extern "C"` (asi los llaman ReplayBuffer,
    EffectSequencer y MotionObject). Declaracion minima local de `atn::EffectManager` (dos metodos).
  - Nombre real propuesto: desconocido (contiene los cargadores de efectos; quizas `atn_effect_handle.cpp`).
- Hallazgos MWCC
  - MWCC reconoce como `sthbrx`/`stwbrx` algunas expresiones de inversion de bytes con mascaras (16 bits:
    `((v & 0xFF) << 8) | ((v >> 8) & 0xFF)` sin signo; 32 bits: `((v & 0xFF) << 24) | ((v & 0xFF00) << 8) |
    ((v & 0xFF0000) >> 8) | ((v & 0xFF000000) >> 24)` con `int`), y entonces desenrolla el bucle; con los
    intrinsecos `__sthbrx`/`__stwbrx` el bucle no se desenrolla nunca. La de 64 bits no se reconoce.
  - Desenrollado: `while (count-- > 0)` / `for (; count > 0; count--)` con `count` entero da el esquema
    `srwi. ctr,count,3` + resto `andi.`; `for (i = 0; i < count; i++)` da el esquema generico (comprobacion de
    desbordamiento con 0x7FFFFFFE y `count - 8`).

## atn/NumberFormat_unk (commit 87693a3)

- Resultado: 3/4 al 100%, `matching: false`. `main.dol: OK`.
- NONMATCHING: `fn_800DD3FC` 99.39% - solo reparto de registros (`text`/`num`/`number`); el resto (estructura del
  `switch`, pila, constantes) igual.
- **Decisiones sin el usuario**
  - Splits: sin cambios (cubre `.text`, el double int->float de `.rodata` y el 1e7f de `.sdata2`).
  - Contenido: un `swprintf` reducido del juego (`fn_800DD3FC`: `%s %c %b %o %x %d %f %%`, flags `+`/espacio y
    digitos de ancho/precision; coma decimal si `fn_8006BA30()` (idioma) esta entre 2 y 5), el relleno de ancho
    (`fn_800DD1EC`) y de decimales (`fn_800DD2A8`), y `fn_800DD8A8` (sustituye `%s<n>` por la n-esima cadena).
    Nombres `extern "C"` sin cambiar (los llaman asi D2AMpMenuCockpit, RandomMerchant...).
  - Externos inferidos: `fn_80686AC4` = `wcscat`, `fn_800DC1DC` = cadena ancha a entero (Util_unk),
    `fn_8068B2A0` = entero a texto (`itow(value, buf, radix)`, devuelve `buf`).
  - Nombre real propuesto: desconocido (formateo de texto; `atn_wsprintf.cpp`?).
- Hallazgos MWCC
  - Funcion con muchas variables en registros salvados: declararlas todas al principio (estilo C) en el orden de
    los registros del original (de r31 hacia abajo) ordena la asignacion mucho mejor que declararlas en su ambito.
  - `float x = __fabs((double)expr)` da `fabs` + `frsp` antes de `fctiwz` (con `__fabs(float)` no sale `frsp`).
  - Un helper inline de pocas sentencias dentro de una funcion pequena la hace autoinlinable en sus llamadoras;
    escribir el bucle directamente (sin helper) la dejo fuera de linea sin necesitar `((void)0)`.

## atn/Util_unk (commit 8a6bc87)

- Resultado: 6/12 al 100%, `matching: false`. `main.dol: OK`.
- NONMATCHING:
  - `fn_800DC12C` 97.73%, `fn_800DC1DC` 90.81%, `fn_800DC288` 94.56%, `fn_800DC3CC` 94.87% (atoi/atof char y
    anchas): el original empieza con un `b` a la instruccion siguiente (bloque de entrada vacio) que no he
    reproducido (etiquetas, bucles, do/while, helpers inline...); ademas en atof el cuerpo vuelve a comparar
    '.' (el original salta desde la condicion del bucle a cada caso) y en la ancha de atoi el bucle usa otro
    registro puntero.
  - `fn_800DC500` 81.56% y `fn_800DC6B4` 82.78% (CRC-32 por tabla): reparto de registros y planificacion del
    bucle desenrollado (el original guarda el CRC en r0).
- **Decisiones sin el usuario**
  - Splits: sin cambios.
  - `symbols.txt`: `lbl_807348A0` pasa de `size:0x460` a `size:0x400` (la tabla CRC) y se ANADE
    `@stringBase0 = .rodata:0x80734CA0` (0x59, los mensajes de `basic_string` del pool del fichero: el fichero
    incluye `std/string_members.h`). `lbl_80734870` (seis doubles en un objeto de 0x30) se deja como esta.
  - Contenido: atoi/atof (char con punteros, anchas con indice; la prueba de digito ancha sale como
    `c + (wchar_t)-L'0'`, `addis`+`subi`), envoltorios de `wcstok` (`fn_80686B90`), `fn_8068B1A4` y `itoa`
    (`fn_8068B29C`), CRC-32 (tabla y bit a bit, polinomio 0x04C11DB7), `fn_800DC980` = printf a `std::string`
    (`vsprintf` a un bufer de 0x200) y `boost::throw_exception` vacio (BOOST_NO_EXCEPTIONS). Nombres `extern "C"`
    sin cambiar (los llaman asi LibraryTask, D2A..., FileList).
  - Nombre real propuesto: desconocido (`atn_util.cpp`?).
- Hallazgos MWCC
  - `x ^ 0xFFFFFFFF` da `li r0,-1; xor`; `~x` da `not`.
  - El mismo cuerpo escrito como funcion normal o como inline llamada desde un envoltorio `extern "C"` asigna
    los FPR en orden distinto (atof: solo la version inline casa los registros de `value`/`scale`).
