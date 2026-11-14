# Sesion cabeceras-20261114, cola A3 (forma original de `push_back` sin bajadas)

Inicio: 2026-11-14 17:25 CET. Fin: ~18:55.

Parte del parche de A2 (`docs/cabeceras-20261114-qA2.patch`). Medido en build completo contra `ref-A3` (15434):
A2 sube 36 funciones y baja `RenderManager::RenderManager` 88,71 -> 46,29, `ParsePATH_unk` 72,74 -> 16,47,
`ParseACID_unk` 64,42 -> 57,51, `DataList_unk<Actor>::Add_unk` 61,23 -> 52,56 (mas las dos de ControlAppear por
etiquetas y 9 bajadas pequenas).

**Resultado:** las cuatro bajadas grandes resueltas (y convertidas en subidas), pero quedan 7 bajadas pequenas
(<= 0,72, registros/huecos de pila heredados de la forma de A2) y una de 0,64 por la copia con `memmove`. Segun la
cola, **no se commitea en el codigo**: mejor parche en `docs/cabeceras-20261114-qA3.patch` (`git apply`, sobre
`main` actual; incluye las especializaciones locales de A2).

## Mejor variante (v6, `docs/cabeceras-20261114-qA3.patch`)

A2 mas tres cambios en `include/std/vector.h`:

1. **Ctor de RenderManager**: el arbol de `push_back<atn::Light>` pasaba del presupuesto (R3) por una sola expresion
   (quitar el `++this->mSize` o el `construct` de la rama de `slow` lo devuelve). El incremento va en el argumento:
   `construct(this->mData + this->mSize++, x)`. Coste: Event/Manager `fn_80481B9C` 95,97 -> 95,00 (el original
   incrementa tras la llamada) y Sequence `fn_80540A08` 92,40 -> 91,61; las dos siguen muy por encima de la ref.
2. **Copia de `std::vector` mas cara (20 asserts vacios `MSL_VEC_ASSERT10_UNK` en `vector(const vector&)`)**:
   arregla Path y Actor a la vez. `push_back<Path>` (ParsePATH 0x8057AEAC) en el original rechaza sus candidatas
   (llama a `construct` 0x8057DC38 y a `slow` 0x8057DB14) y el nuestro las aceptaba; la copia de `Actor` en el
   original llama a las copias de sus miembros (0x8054372C...) y la nuestra las metia todas. Las dos cosas salen de
   que nuestra copia de vector es mas barata que la de MSL. Con 13-50 asserts: ParsePATH 94,63, ParseACID 82,99,
   Add_unk<Actor> 83,92 (con 10 sale otra forma peor; con 1-2 a medias).
3. **Copia de vectores de palabras con `memmove`** (`__vec_word_unk<T>`: punteros, int, float, los de
   `__vec_word_alloc_unk`) en un inline `__construct_at_end_unk`: el original copia el `vector<int>` de Event::Path
   con `memmove(data + size, first, (last - first) * 4); size += last - first` (0x8057E244) y las estructuras POD
   (PathCubic_unk...) elemento a elemento. Sube Sequence `fn_8054204C` 10,27 -> 92,99, TextAnimation
   `push_back<vector<float>>` 69,13 -> 75,43, copia de Path +1,65, `vector<Camera>` (NewCampPartsGmk) +1,62. Baja
   `DebugInfoColumn_unk` (copia) 99,64 -> 99,00 (registros, por el nivel de inline extra).

Build completo v6: `main.dol: OK`, 15432 (las 2 perdidas son ControlAppear por `@enum$6929ControlAppear_cpp`:
se arreglan con el re-emparejado en lote, no hecho porque no se commitea). **45 suben**, entre ellas:
Sequence fn_8054204C +82,7; Manager fn_80481B9C 58,41 -> 95,00; Add_unk<Actor> 61,23 -> 83,92; Section/Cut/Channel
69 -> 91 (x4); ParsePATH 72,74 -> 94,63; ParseACID 64,42 -> 82,99; RenderManager ctor 88,71 -> 89,15; luces de
Manager +1,9-2,7; push_back debiles de MapPartsBase/GameScene/GimmickStateCommand/Em204/D2A_Manager +1,5-2,5.

Bajadas que quedan (fuzzdrop): ColorF push_back 95,56 -> 95,34, Em301LShade fn_802D6004 87,60 -> 86,88,
GameEventScript fn_8039F08C 99,97 -> 99,81, Manager fn_80484088 95,18 -> 94,93, NewCampPartsItem fn_80517B3C
99,40 -> 99,10, ShopBase fn_805849E8 99,77 -> 99,62, DebugInfoColumn copia 99,64 -> 99,00, MapPartsBase
MatColorInfo -0,00. Todas de registros u orden de huecos de pila: en la ColorF debil el `n` de `__grow_cap_unk` va a
0x10 y los temporales del max a 0x8/0xC (original: n a 0x8, temporal a 0x10), por el nivel de inline extra de
`slow`/`realloc` (ya lo vio A2).

Variante v10 (v6 sin el `memmove`): sin la bajada de DebugInfoColumn pero pierde fn_8054204C (+82) y TextAnimation;
peor.

## Probado sin exito (no repetir)

- Quitar el `else cap = max_size();` muerto de `__expand_by_unk`: arregla el ctor de RenderManager (89,11) pero
  baja Em301LShade fn_802D67F8 -6, Em302Kraken fn_802DD1B4 -5,5, CrowdManager fn_801EB188 -3,5, Np131LivingArmor
  fn_80546B84 -2,6, BattleTask fn_800F8C6C -1,7 (alli el `else` si deja codigo).
- Para el ctor: `else` en vez de `return`, coma (`construct(...), ++mSize;`), `construct` sin el accesor
  `__alloc_unk()`, `buf.push_back(x)` en vez de `__append_copies_unk`, `__vec_set_buf_unk`, despacho por `enum`
  en `__push_fast_unk`/`__append_copies_unk`, `push_front` con etiqueta: nada (el coste es por expresiones, no por
  sentencias; las etiquetas cuestan mas que el codigo muerto).
- `expand_by` = `__grow_by_unk(n); return false;` con la comprobacion `cap > max_size()` en la reserva del buffer
  (como 0x80544748/0x80545B90 del original): Em301LShade fn_802D6004 86,4 -> 92,75, pero el ctor de RenderManager
  vuelve a pasarse (tambien con el `mSize++`). Solo la reserva con comprobacion (sin `expand_by` nuevo): nada.
- Despacho de la forma vieja para tipos polimorficos (`__builtin_is_polymorphic`): con `if` constante el codigo
  muerto tumba lo demas; con etiqueta en `slow` el ctor se pasa y ParsePATH sale al 0 %.
- Para Path/Actor: accesores `begin()/end() const`, `distance`, partir `__init_unk` en ayudantes, `x.size()`:
  no cuentan. Coste en la copia de `std::string`: no influye. `memmove` con `__builtin_trivial_members & 7` (4
  `memmove` en la copia de Path, el original 1) o en linea en `__init_unk` (pasa de las ~13 sentencias y sale fuera
  de linea para int/float).
- Orden de huecos de pila en ColorF/ShopBase: argumento de `__grow_cap_unk` con `+ 1`, por una variable local o con
  `size()`/`capacity()`: igual o peor.

## Funciones al 100 % / 0 %

Sin cambios (nada commiteado en codigo).

## Para el port

Ningun cambio (el parche no se integra). Si se integra: solo `include/std/vector.h` y miembros nuevos en los
buffers de vector (A2); ningun layout ni firma de clases del juego.

## Decisiones tomadas sin el usuario

- No commitear el parche: quedan 8 bajadas pequenas (<= 0,72) de registros/pila frente a 45 subidas (cuatro de
  +18 a +83). Decision para el usuario: aceptarlas e integrar v6 (re-emparejado en lote completo por las etiquetas
  de ControlAppear y las inline nuevas), o seguir con el orden de huecos de pila de `grow_cap`.
- Los 20 asserts vacios en la copia de vector son la tecnica ya aceptada (asserts de release para el presupuesto);
  el numero (20) es el centro del tramo que funciona (13-50).

## Hallazgos (una linea)

- Nuestra copia de `std::vector` costaba menos que la de MSL para el presupuesto (R3): 13-50 asserts vacios en
  `vector(const vector&)` dan la forma del original en `push_back<Event::Path>` (rechaza) y en la copia de
  `Event::Actor` (llama a las copias de sus miembros).
- El presupuesto cuenta expresiones, no sentencias: `construct(p, x), ++size;` cuesta lo mismo que dos sentencias;
  `construct(data + size++, x)` cuesta menos.
- MSL copia los `vector` de punteros/int/float con `memmove(data + size, first, (last - first) * 4)` y los de
  estructuras elemento a elemento (copia de Event::Path, 0x8057E244).
- El `else cap = max_size();` del recommend muerto deja codigo en funciones grandes (CrowdManager, Em301, Em302):
  no se puede quitar.
