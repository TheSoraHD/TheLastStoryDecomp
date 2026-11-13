# Cola M (paralelo-20261113, agente M)

Cola: restos de game/GameEventScript (12 al 0 %) y AI/AI_archer_attack01 (6 al 0 %).

## Resultado por unidad

- `game/GameEventScript`: 0 % 12 -> 0; al 100 % 110 -> 120/224 (lost 0, fuzzdrop 0).
  - `boost::lexical_cast<std::string>(const int&)` (0x80393BEC, antes `extern "C" fn_80393BEC`): escrita como
    Boost 1.34 (`detail::lexical_stream` sobre `std::stringstream`, `precision(10)` = `digits10 + 1` de int,
    `bad_lexical_cast(typeid(int), typeid(std::string))` con los dos punteros). 89,34 %, NONMATCHING (ver abajo).
  - Al 100 %: `~basic_iostream`, `~basic_istream`, `~basic_stringstream` (0x803940E8-0x80394208),
    `basic_stringstream::str() const` (0x80394318), `__sinit_\GameEventScript_cpp` (0x803B2780: solo las guardas
    de `num_put<char>::id` y `numpunct<char>::id`, como GameScene) y los cinco thunks de base virtual
    (0x803B27AC-0x803B27F8).
  - `basic_stringbuf::str() const` (0x803AD668): 88,53 %, NONMATCHING (ver abajo).
- `AI/AI_archer_attack01`: 0 % 5 -> 0; al 100 % 480 -> 482/510 (lost 0, fuzzdrop 0).
  - Mapa de scripts (`std::map<std::string, factory>` en 0x807C6B84): `AIScriptMap_unk` (tamano, raiz con el
    nodo final en `&mRoot`, primero; como `ScriptTreeData_unk`), y `lbl_807C6B84` pasa a ser ese objeto (antes
    `unsigned char[]`; `fn_8001E1D0` y la `__sinit` a mano usan `&lbl_807C6B84`, sin cambios en su codigo).
  - `AIScriptMap_unk::Destroy_unk` (0x8003DEE0, antes `fn_8003DEE0`) al 100 %: recursiva con los asserts de
    los padres evaluados (dos niveles de inline, como AudioMap_unk) y `destroy` + `operator delete` del
    nodo (sin test de nulo; con `delete node` sale el test).
  - `AIScriptMap_unk::InsertNode_unk` (0x8003DD50, antes `fn_8003DD50`) 99,05 %: como
    `ScriptTreeData_unk::InsertNode_unk`; los flags de `fn_8001E1D0`/`fn_8003E530` pasan a `bool` (con
    `unsigned char` `fn_8001E1D0` bajaba de 47,6 a 39,3 por la conversion). NONMATCHING: la cadena del pool
    (+0x454) va como base + `addi` en el original y el tag del iterador con las ranuras de pila cruzadas.
  - `fn_8003E6B8` (destructor del mapa registrado por la `__sinit` a mano `fn_8003E688`) al 100 %: borrado
    con test de `this`, `~AIScriptMapL1_unk()` explicito (capas L1-L3 sobre `AIScriptMap_unk`: cuatro tests
    mas) y devuelve el objeto. Escrito como `extern "C"` porque la `__sinit` ya lo esta: con un objeto global
    de verdad (`AIScriptMapL1_unk lbl_807C6B84;`) el compilador da el destructor al 100 % pero la `__sinit`
    baja a 67,5 % (la primera escritura va por el simbolo, `stw rX, lo(rHA)`; probadas 11 variantes en
    `try.sh`: ctor en lista o cuerpo, base, miembro, `static`, miembro estatico, ctor no inline).
  - Parcial `fn_8001E1D0` (`map::insert`) 47,60 -> 94,07 %: reescrita con `fn_8003E530`,
    `basic_string::compare(const basic_string&)` de `std/string.h` y `InsertNode_unk` (los locales sin
    inicializar: los pone el llamado). Queda el hueco de pila del predecesor.
  - Parcial `fn_8003E530` (posicion de insercion) 88,17 -> 92,93 %: `compare` de `std/string.h`, extremo y
    raiz del `AIScriptMap_unk`, `*parent = 0` antes de leer la raiz. Quedan los huecos de pila de los
    temporales del compare.
  - `fn_8001E730` (vacia el mapa: borra las factorias en orden y luego `clear()`) 93,82 % y `fn_8001EA18`
    (`find` por nombre: `AIScriptMap_unk::Find_unk(std::string(name))` inline con el `compare` de
    `std/string.h`) 88,02 %: NONMATCHING (ver abajo).
- `game/GameScene`: llama a `boost::lexical_cast<std::string, int>` con una declaracion de la plantilla (sin
  definicion: la copia debil es la de GameEventScript) en vez de `extern "C" fn_80393BEC`. Codigo y etiquetas
  identicos.

## Cabeceras nuevas o tocadas

- `include/std/istream.h` (nueva, solo la incluye GameEventScript): `basic_istream` (vbase 0x00, cuenta 0x04,
  vtable 0x08, `basic_ios` 0x0C), `basic_iostream` (parte `basic_ostream` en 0x0C, `basic_ios` en 0x14),
  `basic_stringstream` (buffer en 0x14, `basic_ios` en 0x4C) y las definiciones inline de lo que se declara en
  las cabeceras compartidas (`ios_base::unsetf/precision`, `locale::locale()`, ctor de `basic_streambuf`,
  `basic_ios::init/widen`, ctor y `str()` de `basic_stringbuf`, dtors de `basic_ostream/streambuf/stringbuf`).
- `ios_base.h` (13 unidades): `skipws = 0x1000`, declaraciones `unsetf(fmtflags)`, `precision(streamsize)`,
  `ios_base::init(void*)` (0x8068B610); en `basic_ios` `widen(char)`, `init(basic_streambuf*)` y el miembro de
  datos `mFill` (0x38, antes `m38[4]`). `locale.h`: `locale()` y `static const locale& __global()`
  (0x806926D4). `streambuf.h`: `basic_streambuf()`. `sstream.h`: ctor `basic_stringbuf(openmode)`, `str()`,
  `mHigh` `mutable`. Todo sin nombres de parametro ni cuerpos en esas cabeceras: definidas en `istream.h`.
  Comprobado: GameScene, WordWrap_unk sin cambios en el `.o` (ni en los `@N`).
- `locale_facets_unk.h`: `this->m38[0]` -> `this->mFill`; `ctype<char>` con `char m08[0x10]` (el facet es de
  0x18 bytes, `new` de `use_facet`): EffectManager sube 0,01 en dos `do_put`.

## NONMATCHING

- `lexical_cast` 89,34 %: (1) `use_facet<ctype<char>>` en `basic_ios::init`: el original llama a `new` y al
  ctor del facet antes de leer el id y la tabla del locale; la cabecera lee la tabla antes (orden del caso
  `numpunct` de EffectManager, que tiene ctor inline). Probado escribir `loc.mImp.mPtr->replace(new Facet,
  id.get())`: sube unas funciones de EffectManager y baja otras (fuzzdrop), revertido. (2) `unsetf(skipws)`: el
  original hace `and` con 0xEFFF en registro (`lis`/`subi`); probados `&=`, `= x & ~mask`, mascara en
  `unsigned short`: salen `andi.` o `rlwinm`.
- `basic_stringbuf::str()` 88,53 %: el original usa un constructor de rango de `basic_string` (`reserve(last -
  first)` + `do_replace`); con el declarado en `std/string.h` da 98,97 % (first/last cruzados r30/r31 en las dos
  ramas; probados orden de ramas, tag temporal, cast del tamano, ctor plantilla). No se deja en `string.h`
  porque recompila todo el juego; queda con `basic_string(const T*, size_type)`.

- `fn_8001E730` 93,82 %: la direccion del mapa queda en r30 (el original la vuelve a cargar con lis/addi tras
  el bucle y en cada llamada) y un registro guardado mas; probado `clear()` como miembro.
- `fn_8001EA18` 88,02 %: el tamano y los datos de la clave se cargan una vez antes del bucle (el original los
  relee en cada nodo y guarda 4 registros, aqui 7); probados clave local, temporal en `Find_unk`, helper de
  compare propio (57 %).

## Decisiones tomadas sin el usuario

- Renombrados en `symbols.txt`: `fn_80393BEC` -> nombre mangled de `boost::lexical_cast<std::string, int>`,
  `fn_80394318` -> `str__Q23std65basic_stringstream...CFv`, `fn_803B2780` -> `__sinit_\GameEventScript_cpp`;
  `pd.sh` puso `__RTTI__i` y `__RTTI__Q23std59basic_string...` en 0x8078A780/0x8078B190 (unicos).
- Los `@N` de GameEventScript se renumeraron (+7000 aprox.) por las plantillas nuevas: `pairdata --relabel`
  dejaba ambiguos los floats repetidos; se reetiquetaron alineando por contenido nuestro `.o` anterior y el
  nuevo (script local `remap.py`, 66 etiquetas) y luego `pd.sh`.
- `boost::throw_exception(const std::exception&)` declarada en GameEventScript.cpp (definida en
  atn/Util_unk.cpp, BOOST_NO_EXCEPTIONS).
- AI_archer: renombrado `fn_8003DD50` -> `InsertNode_unk__15AIScriptMap_unkFP19AIScriptMapNode_unkbbRC20AIScriptMapValue_unk`.
- AI_archer: renombrado `fn_8003DEE0` -> `Destroy_unk__15AIScriptMap_unkFP19AIScriptMapNode_unk`; `pd.sh`
  reetiqueto los `@N` de la unidad (481 -> 481).
- AI_archer: `__declspec(noinline)` en `fn_8001EA18` (con `Find_unk` inline MWCC la metia en `fn_8001C1C0`, que bajaba de 100 a 0).
- AI_archer: `fn_8003E6B8` como funcion `extern "C"` que hace de destructor con borrado (decision a revisar:
  sustituye a la que generaria el compilador; ver arriba).
- Cambio de tamano de `ctype<char>` en `locale_facets_unk.h` (cabecera MSL compartida con EffectManager): solo
  miembro de datos.

## Hallazgos

- `basic_string::compare(const basic_string&)` de `std/string.h` reproduce el compare inline de MSL de los
  `std::map<std::string, ...>` (datos y tamano del argumento en una sola prueba del bit largo); un helper
  propio con `compare(pos, n1, s, n2)` lo separa en dos.
- Un objeto global con destructor da en la `__sinit` `stw rX, sym@l(rHA)` para la primera escritura; el
  original (AI_archer 0x8003E688) la hace con el puntero en r3.

- MWCC no guarda las vtables en los dtors inline de `basic_stringbuf`/`basic_streambuf` dentro de
  `~basic_stringstream` (solo las ranuras de base virtual en +0x3C).
- Un parametro con nombre o un cuerpo inline nuevo en `ios_base.h`/`sstream.h` renumera los `@N` de las
  unidades que la incluyen (GameScene +19, WordWrap +1); declararlas sin nombres y definirlas en una cabecera
  propia lo evita.

## Pendientes

- GameEventScript: `poolorder.py --funcs` da 8 funciones que solo fallan por el pool de cadenas (fn_803A064C,
  fn_803A1B20, fn_803A24A4, fn_803A4568, fn_803A47A4, fn_803AA5E4 y los dos `InsertNode_unk`): al original le
  sobran 7 cadenas vacias `""` sin referencias (no es alineacion), cada una justo detras del formato de un
  `sprintf` de depuracion: el log de flags (fn_8039C??? "...%d -> %d,"), "ScriptError: ExecComAimGimmick...",
  "Event not found! ev%04d", "START Page%d", "END" (pero no "Page%d -> %d" del mismo `if`), y los dos logs
  "%d,%s,%d,..." de cambio de pagina y de flag externo. Parece una llamada inline con un `""` que se elimina
  (p. ej. un `strcat(buf, "")` o un parametro por defecto no usado); no se ha probado.

- AI_archer: pasar el mapa a objeto global (destructor y `__sinit` del compilador) cuando se encuentre la
  forma de la `__sinit` original.
- `ordercheck` GameEventScript: 1 fuera de orden ya conocido (`__dt__15GameEventScriptFv`).
