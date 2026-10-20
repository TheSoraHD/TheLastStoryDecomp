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
