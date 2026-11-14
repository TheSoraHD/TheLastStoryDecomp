# Sesion cabeceras-20261114, cola A4 (bajadas pequenas del parche de push_back)

Inicio: 2026-11-14 18:43 CET. Fin: ~19:30.

Parte del parche de A3 (`docs/cabeceras-20261114-qA3.patch`, v6). Medido en build completo contra `ref-A4`
(15446). Herramienta propia: build de las unidades tocadas + `objdiff-cli report generate` (8 s) y comparacion
del fuzzy con `ref-A4` y con v6 (el `match_percent` de `objdiff-cli diff` no sirve: cuenta etiquetas renumeradas).

**Resultado:** no hay variante sin bajadas; quedan 4 bajadas de registros (0,14-0,64) y 5 minimas (<= 0,03).
Segun la cola, **no se commitea en el codigo**: mejor parche en `docs/cabeceras-20261114-qA4.patch` (`git apply`
sobre `main` actual; incluye todo A3 + lo de A4; `git apply --check` OK).

## Mejor variante (W2, `docs/cabeceras-20261114-qA4.patch`)

v6 de A3 mas cuatro cambios en `include/std/vector.h`:

1. **Huecos de pila de `__grow_cap_unk` (la "bajada de pila" de A3 era en realidad un fallo de siempre)**: el
   orden de `n` y de los dos temporales del max salia invertido frente al original en TODOS los `push_back`
   (ColorF debil: original n 0x8 / tmp1 0x10 / tmp2 0xC, nuestro al reves; GameEventScript inline: al reves en los
   dos). Cada nivel de inline invierte los huecos (truco ya conocido): para tipos genericos `realloc` llega a
   `__grow_cap_unk` a traves de un nivel mas (`__vec_grow_sel_unk<T>::__alloc_split_unk`); para punteros, int y
   float (que ya estaban bien, `vector<int>::push_back` 100 %) a traves de dos (`vector::__alloc_split_unk`), para
   conservar la paridad. El argumento de `__grow_cap_unk` tiene que ser la expresion (no una copia de un parametro:
   entonces la comparacion usa el registro y no recarga `n`).
2. **`__expand_by_unk` con el recommend muerto en un inline aparte con `const size_type& n`**
   (`__recommend_ref_unk`): da la forma del original en bucles (sin huecos de pila, `cmplwi r0,1`, `b; b`):
   Em301LShade fn_802D6004 87,60 -> 93,00; las luces de Event/Manager siguen igual o mejor.
3. **Bucle muerto de `~__vec_split_unk` (elementos sin destructor) en `__destroy_front_unk()`**: abarata el arbol
   de `push_back<atn::Light>` para el presupuesto (R3); sin esto 2. tumba el ctor de RenderManager (46 %). Mover
   tambien el del destructor de `__vec_split_gbase_unk` baja 7 funciones (EventParser x5, TextAnimation, Em204).
4. Con ese margen, **`construct(...); ++mSize;`** como el original en `slow` (A3 tuvo que meter el `mSize++` en el
   argumento): Manager fn_80481B9C 94,96 -> 95,97, Sequence fn_80540A08 91,61 -> 92,20; el ctor de RenderManager
   sigue en 89,13.

Build completo W2: `main.dol: OK`, **15449** (ref 15446; las 2 perdidas son las de ControlAppear por
`@enum$6929ControlAppear_cpp`, se arreglan con el re-emparejado en lote) => **5 funciones nuevas al 100 %**:
push_back de `CosEditItem_unk`, `CosEditWeaponItem_unk` (CosEditItemList_unk), `KrakenCommand_unk` (Em302Kraken),
`KrakenMpCommand_unk` (Em302KrakenMp) y `gs::HideLink2_unk` (GameScene). Fuzzy +28,5 sobre v6 (+346 sobre ref en
las unidades tocadas, sin ControlAppear). Push_back debiles genericos +0,1-0,25 cada uno (MapPartsBase,
Em204BigMulue, GimmickStateCommand, Em203...), ShopBase fn_805849E8 99,77 -> 99,85, MatColorInfo 94,95.

Bajadas que quedan (fuzzdrop contra ref-A4):
- Event/DebugInfo_unk copia de `DebugInfoColumn_unk` 99,64 -> 99,00 (de A3: el nivel del `memmove`; registros).
- game/NewCampPartsItem fn_80517B3C 99,40 -> 99,07 (registros).
- Event/Manager fn_80484088 95,18 -> 94,97 (registros; ya con el marco y el recommend muerto del original).
- game/GameEventScript fn_8039F08C 99,97 -> 99,83 (registros de los campos de `record`; la pila ya cuadra).
- Minimas (registros): atn/WordWrap_unk fn_800E0000 -0,02, game/U16Table_unk fn_803CD598 -0,03,
  CosEditItemList_unk fn_801D94D0 -0,01, Em305Vampire/Em305VampireMp vf0C -0,01.

Mediciones intermedias (suma del fuzzy en las 33 unidades frente a v6 / bajadas):
v6 0 / 8; E8 (nivel extra para todos) +4,6 / 14 (int, float y punteros bajan: necesitan paridad par);
E9 (selector por tipo) +6,4 / 6; Y2 (+recommend por referencia) Em301 +6 pero ctor RenderManager 46 %;
Y2+S1 (los dos bucles muertos fuera) +18,6 / 11; Y2+S1b (solo el de `~__vec_split_unk`) +20,6 / 4;
W2 (+`++mSize` separado) +25,9 / 4 (+ las 5 minimas fuera de la lista, vistas en el build completo).

## Probado sin exito (no repetir)

- `buf.push_back(x)` en vez de `__append_copies_unk(n, x)` en realloc: pierde Sequence fn_8054204C (-82).
- `realloc` con `1` literal, parametros del selector en otro orden, condicion de `slow` invertida
  (`if (!expand_by) realloc else construct`: Manager fn_80481B9C -1,4): nada para los registros.
- `__grow_cap_unk(const size_type&)`: `n` queda en registro (el original lo recarga de la pila).
- Recommend en un inline por valor (`__recommend_unk(n)` o comprobacion+recommend juntos): orden de huecos bien
  pero `n` es una copia y la comparacion usa el registro.
- Copia de vector: `memmove` solo por especializaciones explicitas de `vector<int/float>::__init_unk` (el resto sin
  nivel): pierde fn_8054204C (-82), `vector<Camera>` -1,6, TextAnimation -1,1 (los genericos quieren el nivel);
  especializacion de `vector<string>::__init_unk` sin nivel: DebugInfoColumn 99,39 pero bajan push_back y
  fn_80220168; con dos niveles: nada; despacho por etiqueta (`(T*)0`) en vez del `if` constante: pierde
  fn_8054204C (el codigo muerto cuenta para su presupuesto).
- El orden de registros de tamano/capacidad en el rapido (NewCampPartsItem, Em305: el original carga el tamano en
  el registro mayor) ya estaba mal antes del parche: no es la causa de las bajadas.
- Biseccion en NewCampPartsItem fn_80517B3C: cabecera vieja + un nivel `slow` envolviendo el codigo viejo da
  99,57 (mejor que la ref); quitar solo `expand_by` (99,37) o solo `realloc` (99,27) recupera a medias: las
  dos funciones inline nuevas cambian la coloracion, pero las dos las exige Event/Manager fn_80481B9C.

## Funciones al 100 % / 0 %

Sin cambios en el codigo (nada commiteado). Con el parche: +5 al 100 % (ver arriba), 0 % sin cambios.

## Para el port

Ningun cambio (no se integra). Si se integra: solo `include/std/vector.h`, `vector_string.h`, `vector_wstring.h`
y las especializaciones locales de A2 (miembros nuevos en buffers y selectores internos de MSL); ningun layout ni
firma de clases del juego. Unidades del port afectadas solo en fuzzy (Puppeteer_CharaTask ColorF, GameUI, TacView,
BattleTask): matching neutro.

## Decisiones tomadas sin el usuario

- No commitear W2: quedan 4 bajadas de registros (<= 0,64) y 5 minimas (<= 0,03), frente a +5 funciones al 100 %
  y +28,5 de fuzzy sobre v6. Decision para el usuario: aceptarlas e integrar W2 (con re-emparejado en lote
  completo por las etiquetas de ControlAppear y los inline nuevos) o seguir con la coloracion de registros.

## Hallazgos (una linea)

- Los huecos de `n` y de los temporales de `__grow_cap` en `push_back` salen invertidos con el numero de niveles
  de inline del original: tipos genericos con un nivel mas entre `realloc` y `grow_cap`, punteros/int/float con
  la paridad de antes (dos niveles); 5 `push_back` debiles al 100 %.
- El recommend muerto de `expand_by` con el `n` por `const&` en un inline aparte desaparece entero en bucles,
  como en el original (Em301LShade fn_802D6004 +5,4).
- Un bucle muerto (constante de `__builtin_trivial_members`) dentro de un destructor inline cuenta para el
  presupuesto R3: sacarlo a un inline aparte (una llamada muerta) libera margen (ctor de RenderManager).
