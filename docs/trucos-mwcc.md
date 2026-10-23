# Trucos de MWCC (una línea cada uno)

Referencia para consultar con `grep` cuando una función se atasca; no hace falta leerla entera. Ejemplos completos y contexto por lote: `docs/<lote>.md` y `docs/archivo/CLAUDE_completo_2026-10-22.md` (sección "Trucos y hallazgos conocidos"). Bloqueos ya investigados (no repetir): `docs/mwcc-blockers.md`.
Al añadir un hallazgo: una línea, con el ejemplo (unidad o dirección) entre paréntesis, en la sección que toque.

## Inlining (`-ipa file`, ver `docs/inlining.md`)

- No se inlinea nunca: función que devuelve por valor un objeto con destructor, o que recibe por valor un parámetro con destructor.
- Cada llamadora inlinea todas sus candidatas o ninguna (presupuesto ~200 sentencias); una candidata única se inlinea siempre; destructores vacíos se eliminan.
- Superado el presupuesto se llaman fuera de línea **todas** las inline, incluso setters de una sentencia; no cuentan las sentencias propias, `((void)0)` ni los getters triviales; un `if` con local cuesta mucho más que un ternario.
- R5: si una inline que rechaza sus candidatas se inlinea en otra, sus candidatas de ≤ 4 sentencias se inlinean igualmente y el resto se llaman.
- Funciones no inline de ≤ ~13 sentencias se autoinlinean; `((void)0)` (asserts de release) cuenta para ese umbral.
- El umbral de autoinline cuenta sentencias del fuente: partir `(m = f())->g()` en dos sentencias o añadir `if`/locales saca una función del autoinline.
- Añadir helpers inline a una función puede hacer que esa función entera pase a inlinearse en sus llamadoras del mismo fichero: comparar `report.json` antes/después.
- Si el original llama fuera de línea a algo que aquí se inlinea, suele faltar otra candidata del mismo llamador, o es `static` (no `inline`), o es plantilla miembro definida fuera de la clase (`ArenaFloor::fn_8041E480`, `Get_unk`).
- Una función débil de otro fichero que el original llama fuera de línea: declararla sin cuerpo en este fichero.
- Una función no inline grande definida más abajo que el original inlinea en una anterior: su cuerpo es un miembro inline y la función fuerte es un envoltorio (`CharaStatus::ClearFlags_unk`).
- La copia débil de una inline se emite tras la primera función que la menciona; destructores implícitos al final en orden de clase; los inline declarados, en orden inverso.
- Un constructor con literales en `@stringBase0` (no `@STRING@`) no es `inline` aunque se inlinee.
- Si un cargador supera el presupuesto, sus inline salen como copias débiles con su nombre real justo detrás: sirve para nombrar `fn_` de 8 bytes.
- Un destructor `{}` inline sale como copia débil al final del `.o` y puede romper el `.dol`: definirlo fuera de línea tras el ctor (`gmk/Cannon`).
- `this` de un objeto local en registro salvado durante su ctor inline (`CsvU`/`CsvTabU`): un destructor vacío `~CsvBase() {}` en la base lo reproduce.
- Dos cadenas inline iguales en una función (dos `fn_80178208`): la segunda va a través de un envoltorio inline más (`static StartX_unk`).
- Helper que el original inlinea pero cuyos literales están en el pool del fichero: `static`, no `inline`.
- Funciones de 3–4 sentencias con un inline de 5 dentro necesitan ~12 `((void)0)` para dejar de autoinlinearse (con un bucle, 6–8). Una miembro inline definida en la clase pone sus literales en `@STRING@`; para mandarlos al `@stringBase0`, definirla fuera de la clase.
- Un llamador con varias llamadas a una candidata grande (`push_back`) saca de línea también `boost::bind` y el ctor plantilla de `boost::function` (todo o nada). `atn::Task::vf10/vf14` son inline vacías (copias débiles 0x8004312C/0x80043128).
- El original inlinea un nivel más de recursión en recorridos de árbol (`atn::Task`) y en `Destroy` de mapas MSL (`AudioDevice`, TownTask): sin solución.
- `Task.cpp` conserva llamadas sin usar a `IsValid`: MWCC las borra si la definición está en el mismo fichero (prueba de que `TaskBase` es otro fichero).
- Un despachador de estados inlinea las funciones de estado de una sola llamada aunque estén definidas antes: sacarlas del autoinline (`((void)0)`, Em101LittleZesya).
- Un miembro inline virtual usado solo por la vtable puede emitirse en mitad del fichero; si el original lo tiene al final, definirlo fuera de línea al final (`SkeltonsMirror::vf38`).
- La vtable (y el destructor inline) solo se emite si se define la primera virtual no inline (normalmente `vf0C`).

## Registros y pila

- Registros salvados por orden de creación: lo creado después recibe el más bajo; mover la declaración de un local cambia r31/f31.
- Pila: locales con nombre en orden inverso a su declaración, luego temporales (cuerpo antes que helpers inline); los grandes (> 0xC) antes que los pequeños. Los locales de una función **inlinada** van en orden ascendente (al revés).
- Bloques de un método con el orden de pila invertido = helper inline del original.
- Un valor que el original guarda en registro salvado y aquí se recalcula: pasarlo por un local o un accesor inline. Lo inverso (recarga): accesor inline o referencia.
- Un local con nombre que guarda una dirección no se propaga como constante (queda en registro salvado); los de funciones inline sí (`addi` en cada uso).
- Global que el original carga antes que los demás argumentos: copiarlo a un local (`T* p = g;`) justo antes. Los argumentos no triviales se evalúan de derecha a izquierda; cargas de miembros simples, de izquierda a derecha.
- Primera carga de `this->mX` (o de un parámetro puntero) antes de guardar r31 en el prólogo = método `const` (o `const T*`).
- Cualquier llamada en el cuerpo de un destructor (aunque se inline) repone el puntero a la vtable.
- `cmplw rA, rB` sigue el orden de la comparación del fuente; `mulhwu` = índice `unsigned`, `mulhw` = con signo.
- Rangos `i >= 0 && i < N` / `x >= a && x <= b` se fusionan en `cmplwi`; con la constante a la izquierda en una mitad (`0 <= i`, `N > i`) quedan dos comparaciones con signo. `a == 2 || a == 4 || a == 5` sale como máscara de bits; `!(a != 2 && a != 3)` evita la fusión.
- Inline que devuelve `bool` usado en un `if` = `mfcr`/`extrwi`; `x > 0 ? x : 0` da `neg/andc/srawi/and` y `x < 0 ? 0 : x` da `srawi/andc`.
- Un argumento calculado pero no usado en la llamada = la función recibe ese argumento (vale con valor por defecto). Un argumento que el original no pasa (registro con basura) = la función externa se declara con uno menos.
- Un parámetro que la función no lee no deja rastro en el asm: si una subclase de la misma virtual sí lo usa, la firma base lo lleva. Leer `mChara` en un local antes del primer store puede intercambiar r4/r5 (`ControlOneMotion`).
- `frsp` antes de `stfs` = valor `double`; si el original guarda `f1` directo, la función devuelve `float`. Un `float` devuelto por una llamada se opera como double.
- Bloques largos: se planifican por ventanas (~51 nodos); añadir o quitar un nodo antes desplaza cargas de constantes y FPR salvados.
- Miembro releído justo tras escribirlo (`stb` + `lbz`) = `volatile`.
- Dos lecturas seguidas del mismo campo sin fusionar: una pasa por un accesor inline. Leer un bit con un accesor `bool` hace recargar la palabra antes de modificarla.
- `strlen` de `<string.h>` es intrínseca y deja globales en registro a través de la llamada; declarada a mano, el global se recarga.
- `sin`/`cos`: nuestro compilador los trata como puros (no recarga globales tras la llamada); el original sí. Sin resolver (`atn/Layer2DBlur`).
- `total += f() * n` invierte los operandos de `mullw`; `int p = f(); total += p * n;` da el orden del original.
- Un struct con un array miembro no vive en registros: construirlo como temporal y copiarlo deja los stores muertos en la pila.
- `A*B + C*D` = `fmuls C*D` + `fmadds A,B`. Matrices de trabajo de una inline: orden de pila = orden de declaración.
- `x + 1 >= N ? 0 : x + 1` da `xoris/addc/subfe/andc`; `x + 1 < N ? x + 1 : 0` da `subfc/srwi/subfe/and`. `(int)c == 6` para comparar un `unsigned char` con `cmpwi`.
- `x ? -1 : 0` da `neg/or/srwi/neg`; `x != 0 ? -1 : 0` da `srawi`. `if (!(x < eps))` da `blt`; `x >= eps` da `cror`.
- `__attribute__((aligned(32)))` en un global grande desactiva optimizaciones (selects sin saltos, desenrollado). Wii/1.3 no desenrolla bucles con `__stwbrx`/`__lwbrx`.
- Reutilizar la misma variable local en dos `if ((p = Find()) != 0 ...)` la mantiene viva entre ambos. `x = k * x` y `x = x * k` reparten distinto los registros volátiles.
- `atn::Abs_unk(x) < k` reutiliza el registro del fabs; `(float)__fabs(x) < k` redondea a uno nuevo.
- `if/else` en lugar de `flag ? 1 : 0` dentro de un setter inline reproduce una rama muerta del original.
- Rama muerta que sobrevive con su `stw` saltado con `b`: probar `!= 0` en lugar de `> 0`.
- `bne X; b Y; X:` en vez de `beq Y` = un `return` dentro de una función inline.
- `if (a || b) X else Y` frente a `if (!a && !b) Y else X` decide qué bloque va primero. `(flags & 0x80) == 0x80` con `cmplwi`: convertir a `unsigned int`.
- Una comparación guardada en `cr1` y reutilizada en un `else if`: el objeto en un local (`CharaTask* chara = mChara;`).
- `int state = mState; switch (mState) {...} if (state == 3)` reutiliza los `cr` del switch. `default: return;` escrito primero quita dos `b` sobrantes.
- `x *= f()` calcula la dirección de `x` después de la llamada; para calcularla antes, pasar `x` por referencia a un inline.
- Bucles: `new (--p) T(*--last)` conserva el bucle del original; un contador `int` reduce a puntero y uno `unsigned` recalcula base+offset; `buf.mData[i]` en lugar de un `operator[]` inline cambia el orden de los incrementos (`LoadModel`).
- En el contador de efectos, un índice `unsigned` y el puntero del elemento reutilizado fuera del `for` conservan `cmplwi` y el orden r30/r31 (`AI/AI_archer_attack01::fn_8001BF64`).
- `x - x` tras propagar: restar a través de una referencia (`unsigned int& size = mSize; size -= n;`).
- Un local agregado desplazado 4 bytes: probar un tamaño que no sea múltiplo de 8 (`LoadColliObject::vf0C`).
- Registros float de varias sumas: el orden de declaración de los locales fija `f31`, `f30`...
- Bucle de partes de `ClearFlags_unk` con `ctr` donde el original no lo usa: sin resolver (lista de lo probado en el archivo).

## Datos y secciones

- Estáticas: inicializadas primero, no inicializadas por orden de primer uso; `= NULL` explícito cambia el orden en `.bss`.
- Base común para `.bss` solo si la función usa ≥ 3 estáticas ya definidas en ese punto (un struct cuenta como una).
- Cada fichero alinea `.data`/`.rodata` a 8 y tiene su propia copia de constantes en `.sdata2`: misma constante en dos direcciones ⇒ ficheros distintos.
- objdiff compara `.sdata2` por posición (no por valor): los floats deben emitirse en el mismo orden de primer uso. Las constantes pasadas como argumento a una inline salen antes que las de su cuerpo; locales `float` con nombre reordenan el pool sin cambiar el código.
- `.sdata2` sigue el orden en que el compilador ve los literales (ctores inline de las fábricas, funciones autoinlineadas definidas antes). Un `float` de 8 bytes en `symbols.txt` (float + relleno) hace que objdiff lo trate como `double`: tamaño 4.
- En unidades a medias, escribir las funciones grandes que faltan (aunque queden NONMATCHING) y poner el fuente en el orden del `.dol` cuadra `.sdata2`/`@stringBase0` y sube muchas funciones del 99,9% al 100%. Tablas estáticas al principio del fichero.
- dtk descarta un hueco de ceros menor que la alineación de la unidad siguiente aunque tenga símbolo: `align:4` en esa sección.
- Vtables en orden inverso a la declaración de clases (incluye `#include`); RTTI débil, la emite el primer fichero.
- Pool de cadenas `@stringBase0` al final de `.rodata`; literales de funciones inline en pools `@STRING@<func>`; cadenas anchas `L"..."` en `@wstringBase0` de `.data` (un `L""` de ≤ 4 bytes, en `.sdata`). Un pool que empieza por `""` = creador `new(1, "", "", 0)` antes del ctor.
- La constante `double` de conversión int→float (`0x4330000080000000`) va en `.rodata` del propio fichero, antes de los nombres RTTI.
- Formatos de `sprintf` con NUL extra en el pool: escribir `"...\0"`. Literal `00 00` sin fusionar con `""` = `"\0"`. Dos `""` sin fusionar tras otro `""` = `strcmp(x, "\0")`.
- Cadenas/constantes de funciones eliminadas por el enlazador siguen en `.data`/`.sdata2` en su posición: a veces hay que reconstruir la función (stubs `Stripped*_unk`, ptmf `00000000 ffffffff 00000000` de `scoped_ptr`).
- Static local de una inline usada en otro fichero: el `.o` emite su registro de destructor `@NNNN` (0xC en `.bss`), que debe entrar en el split.
- `__sinit_*` es siempre la última función de su fichero. Las referencias a nivel de namespace (`T& r = obj;`) se inicializan en el `__sinit`.
- Nombre de enum anónimo `@enum$N<fichero>_cpp` o RTTI `@unnamed@<fichero>_cpp@` da el nombre real del fichero (renombrar la unidad).
- Todo inicializador a cero va a `.bss`/`.sbss`. Ceros en `.sdata` del original: pools `@STRING@` de `""` en una inline; si son floats globales, `#pragma explicit_zero_data` (aprobado solo en PostFxColorMatrix).
- `static const char* const p = "";` pone el literal en `.sdata2`; en tablas de punteros, los literales > 4 bytes van a `.rodata` y los ≤ 4 a `.sdata2`. Tablas `static const char* t[]` van a `.data`. Una tabla sin usar no se emite pero sus literales compartidos salen en su orden.
- Un `static const char* const` en `.sdata2` cuyo literal no está en el pool es un `static const char x[]` aparte.
- Un ctor que llama a `basic_string::clear()` sobre una cadena recién puesta a cero deja la rama larga muerta tras el `blr`: dtk la parte como función propia.
- Un `long long` miembro alinea la clase a 8: guardar 64 bits como dos `unsigned long` (`TASK_U64`).
- Meter una clase nueva en una cabecera usada por una unidad enlazada puede cambiar el orden de sus copias débiles y romper el `.dol`: cabecera aparte.
- Un ctor inline de struct con miembros `Vector3` generó un bucle que rompió `gmk/WaterLevel`: struct plano de floats.
- Constantes de más en el pool que el original no tiene en esa unidad: una inline o ctor inline que el original no expande en ese fichero (`poolcheck.py` las lista).

## Enlazado y símbolos

- objdiff da < 100% si una llamada apunta a un símbolo con otro nombre: dar nombre real al ctor/dtor de la base antes de pelear con las subclases.
- Tras integrar ramas, muchas funciones bajan solo porque su pool sigue como `lbl_`: renombrar a `@stringBase0`/`@wstringBase0` (`scope:local data:string_table`). En unidades `auto_`, `scope:local` añade `_ADDR`: dejarlos globales.
- Invokers/managers de `boost::bind`: casi siempre basta con `pairnames` + emparejar sus datos; `pairnames` se salta los nombres con `@` y se equivoca con inlines de 8 bytes.
- Instanciaciones explícitas por miembro (`template void C<T>::f();`), no `template class C<T>;` (salvo que todos los miembros se usen, como `SoftKeyboardBase<wchar_t,0>`).
- Función sin referencias que el original conserva → `force_active` en `config.yml`. Relocalizaciones falsas de dtk → `block_relocations` en `config.yml`.
- Unidad enlazada seguida de una en asm puede romper el `.dol` por alineación → `align:N` en la sección de la unidad siguiente en `splits.txt`.
- Una unidad al 100% puede romper el SHA-1 si define funciones que están en otro objeto o le falta una segunda `.text`: mirar el MAP (`configure.py --map`).
- Varias TU en un tramo: funciones pequeñas nunca inlineadas entre grupos + cadenas en objetos alineados a 8 por grupo.
- Funciones al principio de un split que llaman fuera de línea a funciones de una sentencia: son de otro fichero.
- No crear cabeceras del juego bajo `include/revolution/` (u otra ruta de librería): las librerías del SDK la tomarían.
- Nombres de tipo de `boost::bind`/`boost::function` en `.rodata` dan nombres reales de clase y firmas de callbacks.
- Nombres de estáticas locales (`IsInitialized`, `Callback`…) se repiten entre ficheros: no copiarlos a `symbols.txt` como globales.
- Relocalizaciones `@ha`/`@l`: el offset apunta a la mitad de la instrucción (+2); alinear a 4.

## Splits

- dtk exige que los datos de las unidades sigan el orden de `.text` ("Cyclic dependency"); un `_cont` no puede llevar datos previos a los de su unidad enlazada.
- Final de split dentro de un símbolo: dtk lo rechaza. Huecos de 4 bytes entre unidades: extender la anterior. Una guarda compartida en un hueco ≤ 8 bytes sale indefinida: absorberla en la vecina.
- `configure.py` no borra los `.s` de unidades que ya no existen: borrar `build/SLSP01/asm` antes de inventariar.
- `"Memory allocation failure"` está deduplicada por el enlazador: no indica el fichero.
- El corte más fiable: una constante o cadena con una copia por fichero + el pool de cadenas al final de `.rodata`. Sin pruebas, mejor una unidad grande que cortes inventados.
- Las clases `atn::` siguen el orden alfabético de fichero.
- Más método: `docs/splits-plan.md`.

## Patrones del juego

- `new (heap, "", "", 0) T(...)` = `__nw__FUliPCcPCci`; singletons `if (!sInstance) sInstance = new ...; return sInstance;` y `sInstance = 0;` en el destructor.
- Factorías con el constructor fuera de línea se definen antes que el constructor.
- `if (p != 0) delete p;` explícito si hay `cmpwi` antes del destructor; `delete[]` sin `__destroy_new_array` = elemento sin destructor. `delete (T*)p` crea un temporal.
- Destructores: la vtable no se guarda si el cuerpo está vacío; miembros con `-1`, bases con `0`.
- Conversión float→u8 saturada: `(f >= 1.0f) ? 255 : (f <= 0.0f) ? 0 : (unsigned int)(f * 255.0f + 0.5f)`. `x / 4.0f` ≠ `x * 0.25f`; `x / 2.0f` da `fmuls x, 0.5`. El 2π de `WrapAngle` es `(double)6.2831855f`.
- `__fabs(x)` para `fabs` en línea; `(float)` de un `u16` sale con signo: `(float)(int)x`.
- Clamp: `template<class T> inline T __Clamp_unk(T v, const T& lo, const T& hi)` reproduce la doble lectura de los límites. `MIN`/`MAX` de `macros.h`: el orden de operandos cambia el código.
- `std::vector` con N comprobaciones de `this` en el destructor = miembro de una clase derivada de `std::vector`.
- `push_back` (aplicado en `std/vector.h`, residuo en bucles): ver `docs/mwcc-blockers-q1.md`.
- `Vector3`: un solo `operator=`; el orden z-primero lo decide el scheduler; el cruce r3/r4 con origen temporal no tiene solución conocida (`docs/mwcc-blockers-q3.md`).
- `Vector3 v; v = a - b;` deja temporal + copia psq; `Vector3 v = a - b;` construye en sitio. Un vector que se recarga tras escribir otro miembro = operador con referencia (`m6C += mVel`).
- `clear()` en el cuerpo del ctor deja el store del tamaño del vector al final. Ctor vacío `T() {}` inline: `new[]` sin `__construct_new_array`. `new T[n]` con argumentos por defecto emite `__defctor__T`.
- `a = b = c = 0.0f` guarda de derecha a izquierda; en un ctor inline, la lista de inicialización da el orden de stores.
- Campos de bits entre dos miembros del constructor: van en un miembro `struct` con su propio ctor. `rlwinm; srawi.` al leer un bit = `int x : 1`.
- Intercambio de bytes: `sthbrx`/`stwbrx` solo con la forma exacta de la expresión con máscaras; `slwi` + tres `rlwimi` solo con `__rlwimi`.
- `boost::shared_ptr` es el de Boost 1.35.0 (`libs/boost`); `boost::function` es de un trunk de 2007 (`libs/boost/README.md`).
- Lanzadores de control: `new (0, "", "", 0) chr::ControlX(this, ...)` + `fn_80178208(control)`; la clase sale de la vtable que guarda el constructor.
- Enemigos especiales: factoría de `game/SpecialEnemyBase.cpp` (`fn_805A5374`) con `new (3, "", "", 0) EmXXX(parent, info)`. Bloqueo común sin resolver: copia de parámetros de cámara en registros float.
- Psq: ningún compilador genera `psq_l`/`psq_st` para `a = b`; el asm de `Vec.h` usa `fp1`/`fp2` fijos para que los temporales queden en `f7`+.

## Librerías de red y DWC

- Fuentes en `libs/RVL_SDK/src/revolution/<lib>/`, `libs/dwc/src/` y `libs/gamespy/src/` (`configure.py` compila `gamespy/X` desde ahí). GameSpy: `assert` vacío, macros de depuración que evalúan sus argumentos, `atof` = `strtod` de un argumento.
- `Gecko_ExceptionPPC.cp`: aparcada por decisión del usuario (tamaño de extab).
- Umbral de autoinline con `((void)0)` también en DWC (`dwc_match`); `DWCi_SetMatchStatus` inlineada ~54 veces numera `oldState`/`prev` al revés.
- Funciones eliminadas por el enlazador dejan cadenas en `.data`: en dwc_ghttp se reconstruyó `DWC_GetGHTTPData`.
