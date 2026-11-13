# Lote paralelo 2026-11-13, cola D (Efectos)

Inicio: 2026-11-13 01:18 CET. Rama `agent-D`. Unidades: `atn/EffectSystem_unk`, `atn/EffectManager`.

## Resultado por unidad

- `atn/EffectSystem_unk` (commit 1): 0 % 1 -> 0; escrita NONMATCHING fn_8022DD1C (dibujado de un slot,
  7592 bytes, 84,8 %). Toda la unidad queda escrita; al 100 % sin cambios (43/58). No enlaza (15 NONMATCHING).
- `atn/EffectSystem_unk` (commit 2): fn_8022D478 al 100 % (`EffectSystem_unk::mUpdating` volatile: la espera
  `while (mUpdating != 0)` relee la palabra en cada vuelta). 44/58.

- `atn/EffectManager` (commit 3): 0 % 29 -> 16 (13 emitidas), al 100 % 28 -> 38: copias debiles de MSL con
  instanciaciones explicitas al final del fichero en el orden del DOL (`include/std/locale_facets_unk.h`, nueva):
  `locale::facet::~facet`, `num_put::~num_put`, `num_put::do_put(double)`, `numpunct<char>` (truename, falsename,
  do_decimal_point, grouping, do_thousands_sep, ~numpunct) y el `__sinit` (guardas de `num_put::id` y
  `numpunct::id`). NONMATCHING 55 %: `numpunct::do_truename/do_falsename/do_grouping` (copia corta del string).
  `fn_8023A680` llama a `atn::LoadHandle::fn_80473F18` como miembro (99,03 -> 99,06 %).

- `atn/EffectManager` (commit 4): escrita NONMATCHING `basic_ostream<char>::operator<<(int)` (0x8023BA10, 27 %:
  el original inlinea el ctor y el dtor del sentry; aqui se llaman, ver docs/mwcc-blockers.md del sentry). 0 % 16 -> 15.
  `std::locale::imp` pasa a llamarse `std::__locale_imp` (nombre del DOL, `replace__Q23std12__locale_imp...`).

- `atn/EffectManager` (commit 5): escrita NONMATCHING `num_put::do_put(bool)` (0x8023C0C0, 88 %: huecos de pila de
  los dos nombres del ctor inline de `numpunct`); con ella sale `std::__literal` (0x80243514) al 100 %. 0 % 15 -> 13,
  al 100 % 38 -> 39. `ios_base::mWidth` (0x2C, antes relleno) y `basic_streambuf::sputc` (inline, solo WordWrap).

## Decisiones tomadas sin el usuario

- Empece por fn_8022DD1C y no por EffectManager: la cola de EffectManager son 26 instanciaciones de MSL
  (`num_put::do_put` x8, ~20 KB, `numpunct`, `__put_grouping_float`...) que solo se emiten si se reconstruye
  la plantilla entera de MSL; fn_8022DD1C cierra la unidad.
- `EffectParam_unk::m60` pasa de relleno (`unsigned char[4]`) a `float m60` (distancia hacia la camara) en
  `atn/EffectSystem_unk.h` (67 unidades): solo un miembro de datos, no renumera (precommit OK, fuzzy drops 0).
- `EffectFileEntry_unk::m110[8]` pasa a `float m110, m114` (pivote u/v del quad) en `atn/EffectFileEntry_unk.h`.
- Cabecera nueva `include/atn/EffectVertex_unk.h` con `EffectVertex_unk` (0x14 bytes). `atn/EffectDesc_unk.cpp`
  sigue con su propia definicion en el .cpp (no es de mi cola): pendiente que incluya la cabecera.
- `fn_80236B84`/`fn_80236FFC` declaradas en EffectSystem_unk.cpp con la misma firma que en EffectDesc_unk.cpp
  (ultimo argumento `int`: es la escena; se pasa `(int)scene`).
- `pd.sh` empareja mal 5 floats de `.sdata2` de la unidad (2.0 y los coeficientes -3, 4, 3, 5 de la spline):
  renombrados a mano en `symbols.txt` por valor (0x808830D8, 0x808830F0-0x808830FC).

- `EffectSystem_unk::mUpdating` pasa a `volatile int` (cabecera de 67 unidades; solo el tipo de un miembro de
  datos; EffectManager, que tambien lo usa, sigue igual).
- Tras cada cambio de la unidad, `pd.sh` empareja mal los floats de `.sdata2` (el 0.5 y pi de fn_8022D790 estan
  cruzados en el pool desde antes): los renombro por valor con un script propio
  (`/home/claude/scratch-D/fixsd2.py`, compara el `.sdata2` del `.o` original y el nuestro).

- `include/std/locale.h`: declaraciones `class facet; class id;` dentro de `std::locale` (definidas en
  `std/locale_facets_unk.h`, que solo incluye EffectManager); WordWrap_unk (unica otra unidad) sin cambios de `@N`.
- `symbols.txt`: `dtor_8023B9D0` -> `__dt__Q33std6locale5facetFv` (tambien lo usa la extab de MSL locale.cpp) y
  `fn_80243700` -> `__sinit_\EffectManager_cpp`; las guardas `lbl_8087F3CC/CD` pasan a `@GUARD@id__...` (pd).
  GameScene y HColliData_unk (fuera de las colas) las escribian como `extern "C"` con una clase falsa: ahora
  instancian `num_put<char>::id`/`numpunct<char>::id` con la cabecera nueva y su `__sinit` sale igual (100 %;
  GameScene 149 -> 150, HColliData_unk 4 -> 5).
- La declaracion `extern "C" fn_80473F18(void*)` de `atn/EffectManager.h` (regla 10, es `LoadHandle::fn_80473F18`)
  queda sin usar pero no se borra: la cabecera la incluyen 11 unidades de otras colas (renumeraria sus `@N`).

## NONMATCHING

- `fn_8022DD1C` 84,8 %: marco 0x90 bytes mayor (huecos de pila de las matrices temporales; el original
  parece declarar todas las matrices al principio), registros y orden de la aritmetica de indices del anillo
  de la estela. Probado: angulos por referencia (`const float&`, 81 -> 84,8 %: el original los relee del slot).

## Hallazgos

- Las `@N` de los literales de toda la unidad se renumeran con cualquier cuerpo nuevo (aunque este al final del
  fichero): en EffectSystem_unk, +1201 con fn_8022DD1C; los parametros y locales de las inline cuentan.
- El orden de `.sdata2` de los coeficientes de la spline (-3, 4, 3, 5) sale del orden de las sentencias en el
  fuente (c2 antes que c1).

## Pendientes

- Orden de las copias debiles de EffectManager (ordercheck no las mira): con `operator<<` el dtor de
  `locale::facet` sale detras de el (en el DOL va delante), y salen fuera de linea `~locale` y el dtor del
  `__shared_ptr_base_unk` (el original los inlinea).

- EffectManager: MSL de `ostream << int` (fn_8023BA10) y `num_put`/`numpunct` (ver notas qF2).
- `atn/EffectDesc_unk.cpp`: usar `atn/EffectVertex_unk.h`.
- Orden 0.5/pi del pool (0x808830DC/E0) ya estaba cruzado antes del lote (fn_8022D790).
