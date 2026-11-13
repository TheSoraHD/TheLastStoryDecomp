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

## Decisiones tomadas sin el usuario

- Renombrados en `symbols.txt`: `fn_80393BEC` -> nombre mangled de `boost::lexical_cast<std::string, int>`,
  `fn_80394318` -> `str__Q23std65basic_stringstream...CFv`, `fn_803B2780` -> `__sinit_\GameEventScript_cpp`;
  `pd.sh` puso `__RTTI__i` y `__RTTI__Q23std59basic_string...` en 0x8078A780/0x8078B190 (unicos).
- Los `@N` de GameEventScript se renumeraron (+7000 aprox.) por las plantillas nuevas: `pairdata --relabel`
  dejaba ambiguos los floats repetidos; se reetiquetaron alineando por contenido nuestro `.o` anterior y el
  nuevo (script local `remap.py`, 66 etiquetas) y luego `pd.sh`.
- `boost::throw_exception(const std::exception&)` declarada en GameEventScript.cpp (definida en
  atn/Util_unk.cpp, BOOST_NO_EXCEPTIONS).
- Cambio de tamano de `ctype<char>` en `locale_facets_unk.h` (cabecera MSL compartida con EffectManager): solo
  miembro de datos.

## Hallazgos

- MWCC no guarda las vtables en los dtors inline de `basic_stringbuf`/`basic_streambuf` dentro de
  `~basic_stringstream` (solo las ranuras de base virtual en +0x3C).
- Un parametro con nombre o un cuerpo inline nuevo en `ios_base.h`/`sstream.h` renumera los `@N` de las
  unidades que la incluyen (GameScene +19, WordWrap +1); declararlas sin nombres y definirlas en una cabecera
  propia lo evita.

## Pendientes

- AI/AI_archer_attack01: en curso (mapa de scripts: fn_8001E730, fn_8001EA18, fn_8003DEE0).
- `ordercheck` GameEventScript: 1 fuera de orden ya conocido (`__dt__15GameEventScriptFv`).
