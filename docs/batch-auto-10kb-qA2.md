# batch-auto-10kb qA2: atn (teclado en pantalla, shaders, mando, word wrap, cola iostream)

Cola: `atn/Shader_unk`, `atn/SoftKeyboardBase_wchar_t_0` (prioridad 1), `atn/WPad_unk`, `atn/WordWrap_unk`,
`std/allocator_char` (+ `std/char_traits_char`).

## atn/SoftKeyboardBase_wchar_t_0 (commit 42c8ee5)
- Resultado: 34/34 al 100%, `matching: true`, `main.dol: OK` (regress lost: 0).
- NONMATCHING: ninguna.
- Decisiones sin el usuario:
  - Split sin cambios (0x800C6128-0x800C7DC4 cubre el fichero: dos funciones libres, la plantilla y sus copias debiles).
  - Cabecera nueva `include/atn/SoftKeyboardBase.h`: `atn::SoftKeyboardBase<T, N>` (rejilla 8x32 de `atn::SoftKey_unk`
    de 0x1C bytes, texto `Text_unk<T, 0x400>`, 7 `boost::function` de callback en 0x2428-0x24A0). Nombres de tipos
    (`SoftKey_unk`, `Text_unk`) y miembros inventados con `inferred`. Firmas de callback deducidas de los invocadores:
    m2428 `void(int,int,Key*,int,int,Key*)`, m243C/m2450 `void(int,int,Key*)`, m2464/m2478/m248C `void()`,
    m24A0 `void(int,int,Key*,bool)`.
  - `template class atn::SoftKeyboardBase<wchar_t, 0>;` (instanciacion explicita de toda la clase, contra la norma
    general de la guia): es la unica forma probada que emite los miembros en orden de declaracion seguidos de sus
    copias debiles (safe_bool, destructores de `boost::function<...>`, invocadores) como en el original. Con
    instanciaciones por miembro, cada miembro arrastra justo detras sus dependencias y el orden no cuadra.
  - Funcion inventada `fn_SoftKeyboard_dead_unk(boost::function<void()>*)` (eliminada por el enlazador): solo para
    que `operator safe_bool` de function0<void> y `~function<void()>` salgan antes del ctor (0x800C61C0/0x800C622C).
    El enlazador la elimina junto con su constante ptmf (comprobado: `main.dol: OK`).
  - `SoftKey_unk::Contains_unk` (0x800C79DC) es una funcion normal definida tras la instanciacion explicita (como
    inline o plantilla salia en otra posicion).
  - symbols.txt: nombres de las 30 funciones de la unidad (mangled C++; `fn_800C6128__Fi`, `fn_800C6190__Fv` libres).
    Las copias debiles 0x800C7844/0x800C78CC/0x800C7954 son `~function<Sig>` de las firmas de 6, 3 y 4 argumentos
    (las llama SoftKeyboardU).
  - `fn_800DF154` (atn/WPad_unk) se declara `extern "C" Vector2_unk fn_800DF154(WPadMgr_unk*, int)` (posicion del puntero).
  - Nombre real propuesto del fichero: algo como `SoftKeyboard.cpp` (sin pistas).
- Hallazgos MWCC:
  - `bool ret = m2404; if (ret) { ret = f(a); if (!ret) ret = f(b); } return ret;` da la cadena neg/or/srwi. del
    original; `return m && (f(a) || f(b))` da cmpwi y li 0/1.
  - `mText[wcslen(mText) - 1] = 0` sube el 0 a un registro salvado; `unsigned long len = wcslen(...); mText[len - 1] = 0;` no.
  - Un miembro array cuyo `[0] = 0` va antes que la construccion de otro array miembro = struct con ctor propio.
  - Una copia debil se emite tras la primera funcion que la menciona aunque la haya inlineado; con `template class`
    los miembros salen primero en orden de declaracion y luego sus dependencias.
  - Inline de una funcion que devuelve un struct de 8 bytes POD -> r3/r4; con ctor de copia de usuario
    (`atn::Vector2_unk`) -> puntero oculto, como el original.
- Pendientes para otras unidades: `atn/SoftKeyboardU` (MP) puede reutilizar `include/atn/SoftKeyboardBase.h`.

## atn/Shader_unk (+ nueva atn/ShaderManager_unk) (commit 966a158)
- Resultado: Shader_unk 9/12 al 100%, ShaderManager_unk 2/4; ambas `matching: false`, `main.dol: OK`
  (regress lost: 0).
- NONMATCHING:
  - `ShaderTex_unk::ShaderTex_unk` 98.0%: carga de desc->mA48 antes de los stores de miembros y orden de
    registros del bucle.
  - `Sort_unk<ShaderTexEntry_unk>` (0x800C4444) 90.3%: MSL `__sort`; el original materializa como bool las
    comparaciones negadas de la particion y recarga la clave del pivote (misma reconstruccion que
    game/CharaTask Sort_unk, que tambien esta NONMATCHING).
  - `std::vector<ShaderLODEntry_unk>::__insert_one_unk` (0x800C507C) 66%: disposicion de bloques (la
    comprobacion de longitud de la via lenta va antes de la via rapida) y bucles de copia.
  - `fn_800C5644` (crea el ShaderManager) 95.5%: push_back inline de std::vector<T*> (desajuste conocido
    de include/std/vector.h: carga la capacidad antes que el tamano) y carga de la descripcion antes del new.
  - `ShaderManager_unk::fn_800C607C` 98.7%: registros indice/iterador del FindShader_unk inline cambiados.
- Decisiones sin el usuario:
  - **Split cortado**: el fichero real acaba en 0x800C5644 (dos pools de cadenas con "vector length error"
    duplicado y dos 0.0f en .sdata2 => dos ficheros). `atn/Shader_unk` pasa a .text 0x800C41D0-0x800C5644,
    .rodata 0x80733878-0x807338C8, .data 0x80779348-0x80779368, .sdata 0x8087D920-0x8087D930,
    .sdata2 0x80881140-0x80881150 (sin .bss). Unidad nueva `atn/ShaderManager_unk.cpp` (nombre inventado):
    .text 0x800C5644-0x800C6128, .rodata 0x807338C8-0x80733FF8 (nombres de los 76+4 shaders y su pool),
    .data 0x80779368-0x807794A8 (tablas), .bss 0x807C75A0-0x807C75B0, .sbss 0x8087EFE0-0x8087EFE8
    (absorbido del hueco auto_10: `lbl_8087EFE0`, la instancia), .sdata2 0x80881150-0x80881158.
    game_units.json: entradas nuevas `atn/Shader_unk.cpp` y `atn/ShaderManager_unk.cpp` (matching false).
  - Cabecera nueva `include/atn/Shader_unk.h` (ShaderDesc_unk, ShaderTexEntry_unk, ShaderTex_unk, ShaderLOD_unk,
    ShaderManager_unk; todo inventado). `ShaderTexEntry_unk` deriva de un struct POD con los campos: asi las
    copias pasan por la pila como en el original.
  - CABECERA COMPARTIDA `include/std/vector.h`: anadido `insert(T*, const T&)` y `__insert_one_unk` (NONMATCHING,
    nadie mas los instancia).
  - Reutiliza `atn::Buffer_unk<T>::Resize_unk` (include/atn/Array_unk.h) para los arrays del shader.
  - Funciones del motor de atn/MaterialParser_unk (0x8007AA30 ctor de etapa de textura, 0x8007E9EC, 0x8007AA64,
    0x8007C144 ctor del material, ...) llamadas como `extern "C"` (new + llamada explicita al ctor y
    `__construct_new_array` a mano) para no renombrar simbolos de esa unidad. ShaderMaterial_unk (0x54 bytes)
    parece el `Material_unk` de include/atn/Model_unk.h.
  - Funciones inventadas `ShaderManager_unk::LoadShaders_unk` / `CreateLODs_unk` y ctor no inline: el ctor
    del original se autoinlinea en fn_800C5644 con los literales en el pool del fichero (no @STRING@), lo que
    solo sale si el ctor no es `inline` y es corto; las dos funciones se autoinlinean en el ctor y el
    enlazador las elimina.
  - symbols.txt: nombres de las 16 funciones, `@stringBase0` (0x80733880 y 0x80733FD8), `sShaderFiles`,
    `sErrorShaders`, `sErrorMaterials`, `sSortOffset` (scope:local).
- Hallazgos MWCC:
  - Un swap de un struct con constructor de usuario se hace en registros; si los campos estan en una base
    POD (ctor en la derivada) el temporal deja stores muertos en la pila, como en el original.
  - Inicializar un struct POD local con `= {a, b, 0}` lo deja en la pila (se copia con lwz/stw) en vez de en registros.
  - `bool` pasado por referencia a un helper: `bool flag = false; f(..., flag);` da `li 0; stb` en la pila
    (con el literal `false` directo sale una constante en .sdata).
  - Un metodo `const` carga el primer miembro antes de guardar r31 (confirmado en fn_800C4F38/fn_800C5FF0).
- Pendientes: `atn/MaterialParser_unk` (otra cola): 0x8007AA30/0x8007E9EC/0x8007AA64 son ctor(int)/ctor()/dtor
  de la etapa de textura de 0x18 bytes y 0x8007C144 el ctor del material de 0x54 bytes.

## atn/WPad_unk (commit a9ccb6b)
- Resultado: 16/18 al 100%, `matching: false`, `main.dol: OK` (regress lost: 0).
- NONMATCHING:
  - `WPad_unk::WPad_unk` (0x800DDB4C) 99.8%: el contador del bucle de fn_806540A0 es r31 en el original.
  - `fn_800DDD18` (actualizacion por frame, 0xDEC bytes) 93.7%: mismos bloques; el original guarda un puntero
    base por array y por mando (this + port * 4, 0x64, 0x2C, 0x50, 8) y otro orden de registros/constantes.
- Decisiones sin el usuario:
  - Split: anadido `.sbss 0x8087F018-0x8087F020` (`lbl_8087F018`, la instancia, que estaba en el hueco
    auto_10_8087F000). Entrada nueva en game_units.json (`atn/WPad_unk.cpp`, matching false).
  - Cabecera nueva `include/atn/WPad_unk.h` (WPad_unk de 0x40F8 bytes, KPADStatus_unk de 0xF0 bytes;
    nombres inventados; helpers inline Press_unk/Release_unk de la logica de botones). No se toca
    atn/Pad_unk.cpp (tiene su propia vista local del struct).
  - Funciones como `extern "C"` con el gestor como primer parametro (como las llama atn/Pad_unk.cpp y
    SoftKeyboardBase: fn_800DF154 devuelve atn::Vector2_unk); solo el ctor es C++: fn_800DDB4C ->
    `__ct__8WPad_unkFv` en symbols.txt. `lbl_80734DD0` -> `@stringBase0`.
  - Funciones del SDK (KPAD/WPAD) sin nombre declaradas con su fn_ e `inferred` (KPADRead = fn_806572D0, ...).
  - Nombre real propuesto: un gestor de mando del motor (WPadManager o similar).
- Hallazgos MWCC:
  - `if (0 <= a && a <= 3) { if ((unsigned)a <= 1) ...` reproduce cmpwi/blt + cmpwi/bgt + cmplwi del original.
  - Un struct POD local con metodo Set(x, y) inline: los argumentos se evaluan de derecha a izquierda y el
    struct queda en la pila (copias lwz/stw del original).
  - `fabs` de math.h no es intrinseca si se declara a mano: usar `(float)__fabs((double)v)` para fabs + frsp.
- Pendientes: ninguno.

## atn/WordWrap_unk (commit 41c9d99)
- Resultado: 2/8 al 100%, `matching: false`, `main.dol: OK` (regress lost: 0).
- NONMATCHING: `fn_800DFB40` (word wrap, devuelve std::wstring) 75.8%: mismos bloques; el original carga el tag
  de iterador de cada caracter anadido antes de los bucles y no guarda direcciones de pila en registros.
- Sin decompilar: `fn_800E0000` (parte el texto en un std::vector<std::wstring>, push_back inline de wstring),
  la copia debil del ctor de subcadena de std::wstring (0x800E0908) y los helpers debiles de iostream de MSL
  0x800E0A64 (basic_ios::clear, usa "ios_base failure in clear" de la .data de std/allocator_char),
  0x800E0AA8 y 0x800E0AB0 (no hay fuente de iostream de MSL C++).
- Decisiones sin el usuario:
  - Split sin cambios; entrada nueva en game_units.json (`atn/WordWrap_unk.cpp`, matching false).
  - Funciones `extern "C"` (como las declaran D2A_Message, LibraryTask, MenuCommonTask...). Tabla de kinsoku
    `lbl_80734DD8` -> `sKinsoku` (scope:local; 138 entradas {wchar_t, no empieza linea, no acaba linea}).
  - CABECERA COMPARTIDA `include/std/string.h`: anadido `operator+=(T c)` (size() + __replace_unk de &c);
    regress sin perdidas.
  - Fusion con std/char_traits_char y std/allocator_char: NO hecha. Pruebas a favor de que es un solo fichero:
    la .data de las tres unidades es continua (RTTI de basic_ostream<char> en WordWrap, de ios_base en
    char_traits, vtables de stringbuf/streambuf/ostream y "ios_base failure in clear" en allocator_char) y el
    helper debil 0x800E0A64 de WordWrap usa la cadena de allocator_char. Pero el codigo de iostream (debil, sin
    fuente) ocupa la mayor parte y no se puede enlazar igualmente; se deja como pendiente fusionar las tres
    unidades (0x800DF22C-0x800E2A24) cuando haya una reconstruccion del iostream de MSL.
  - Nombre real propuesto: un fichero de utilidades de texto (word wrap + TextParser).
- Hallazgos MWCC:
  - Una expresion booleana con funciones inline evaluada como sentencia suelta (resultado sin usar) deja en el
    codigo sus bucles (aqui la busqueda binaria): el original tiene esa copia muerta antes del `return`.
  - El orden de registros de los locales de una inline (lo/hi de la busqueda binaria) depende de la profundidad
    de anidado: con la misma inline llamada desde distintas profundidades hay que aplanar un nivel.
  - `c == 9 || c == 13 || c == 0x20 || c == 0x3000` en contexto de valor sale con cntlzw; en un `if`, con li 0/1.
- Pendientes: fusion WordWrap_unk + std/char_traits_char + std/allocator_char (ver arriba).

## std/allocator_char (commit 98edb82)
- Resultado: 6/35 al 100% (TextParser_unk: ctor, fn_800E0D54, fn_800E0D70, fn_800E1374 y 2 NONMATCHING),
  `matching: false`, `main.dol: OK` (regress lost: 0).
- NONMATCHING: `fn_800E0F84` 99.7% (registros del puntero/tamano en el assign inline de std::string);
  `fn_800E114C` 93.1% (el original guarda la constante 1 del test de espacio en r31 y desplaza el resto).
- Sin decompilar: el resto son plantillas debiles de iostream de MSL (basic_stringbuf/basic_streambuf/
  basic_ostream, sentries 0x800E14A4/0x800E1630/0x800E1A48, dtors 0x800E0B18-0x800E0C8C, vtables) y el
  `__sinit` 0x800E29B4; sin fuente de iostream de MSL C++.
- Decisiones sin el usuario:
  - Split sin cambios (la unidad ya existia en game_units.json sin fuente). Fuente nueva
    `src/std/allocator_char.cpp` con TextParser_unk (struct local, misma vista que atn/EffectSequencer_unk.cpp;
    se conserva el mangling `__ct__14TextParser_unkFPvPv` que ya usaba esa unidad). Funciones `extern "C"`.
  - std/char_traits_char no se toca (sigue 1/1 al 100%).
- Hallazgos MWCC:
  - Un `char` local cargado de memoria se extiende con signo al cargarlo; si el original lee `p[i]` varias veces
    (CSE), el byte queda sin extender y el extsb aparece solo en la comparacion: escribir las lecturas directas.
  - `c == 9 || c == 10 || c == 13 || c == ' '` sale como `(c - 9) <= 23 && (1 << (c - 9)) & 0x800013`.
- Pendientes: ver la fusion propuesta en atn/WordWrap_unk.
