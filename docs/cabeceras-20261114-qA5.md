# Sesion cabeceras-20261114, cola A5 (las 8 bajadas pequenas del parche de push_back)

Inicio: 2026-11-14 19:26 CET. Fin: ~20:05 (reloj de la maquina).

Parte del parche de A4 (`docs/cabeceras-20261114-qA4.patch`, W2). Medido con build completo (`ninja -j1`) contra
`ref-A5` (15447) y, para iterar, compilando solo las unidades tocadas + `objdiff-cli report generate`
(comparacion del fuzzy por funcion con `ref-A5` y con W2).

**Resultado:** no hay variante sin bajadas. Mejor parche en `docs/cabeceras-20261114-qA5.patch` (`git apply` sobre
`main` actual, `--check` OK; incluye todo A3 + A4): **8 bajadas** (W2 tenia 9 + 1 de redondeo) y +2,6 de fuzzy
sobre W2. Segun la cola, **no se commitea en el codigo**.

## Mejor variante (`docs/cabeceras-20261114-qA5.patch`)

W2 de A4 mas dos cambios:

1. `vector::__construct_at_end_unk(const T*& first, const T* const& last)` (por referencia, sin copiar los
   parametros de `__init_unk`): TextAnimation `push_back<vector<float>>` 75,45 -> 77,93, copia de Event::Path
   97,56 -> 97,63. Sin efecto en el resto.
2. `__vec_grow_sel_unk<U16RunPair_unk>` con los dos niveles de los tipos palabra, en la especializacion local de
   `game/U16Table_unk.cpp`: fn_803CD598 y fn_803CCF68 vuelven exactamente a la ref (sin bajada).

Build completo: `main.dol: OK`, **15450** (ref 15447: +5 push_back al 100 % de A4, -2 de ControlAppear por
`@enum$6929ControlAppear_cpp`, que se arreglan con el re-emparejado en lote al integrar). Bajadas (fuzzdrop):
DebugInfoColumn copia 99,64 -> 99,00; NewCampPartsItem fn_80517B3C 99,40 -> 99,07; Manager fn_80484088
95,18 -> 94,97; GameEventScript fn_8039F08C 99,97 -> 99,83; WordWrap fn_800E0000 -0,02; CosEditProtector
fn_801D94D0 -0,01; Em305Vampire/Em305VampireMp vf0C -0,01.

## Analisis de las bajadas (por que no son retrocesos de forma)

- Em305 x2, CosEditProtector fn_801D94D0, NewCampPartsItem, GameEventScript: comparando el diff de la ref con el
  del parche, el parche **arregla** los huecos de pila de `__grow_cap_unk` (n y los dos temporales del max, que en
  la ref salen invertidos) y deja `cap` en un registro distinto de `mCap` como el original (`add r19, r18, r0`;
  la ref los fusionaba). Lo que queda es una rotacion de registros que ya estaba en la ref: el puntero al buffer
  de realloc (`addi rX, r1, off` sacado del bucle, solo lo usa el `cmpwi rX, 0` del destructor) recibe en el
  original el registro mas bajo y en el nuestro uno mas alto; con la forma nueva esa rotacion arrastra tres o
  cuatro instrucciones mas (cap / datos nuevos). En GameEventScript la rotacion es de los campos de `record`.
- Sin el nivel de `__vec_grow_sel_unk` (r6 = forma de A3) desaparecen las de Em305/CosEdit/WordWrap/U16Table pero
  se pierden los 4 push_back al 100 % (Kraken x2, CosEditItem x2, HideLink2) y bajan otras 12.
- DebugInfoColumn: el nivel de `__construct_at_end_unk` intercambia `n` (r25) y los datos (r26) de la copia de
  `vector<string>`; sin el nivel vuelve a 99,64 pero pierde Sequence fn_8054204C (-86, presupuesto) y vector<Camera>.

## Probado sin exito (no repetir)

- Asserts de la copia de vector fuera: sin efecto en ninguna de las 9 bajadas.
- Copia: selector `__vec_copy_sel_unk<T>` (generico / palabras / string con dos niveles): string 99,03 y Sequence
  fn_8054204C al 10 %; bucle generico en `__init_unk` con el `memmove` en un nivel solo para palabras: Sequence
  al 10 % y vector<Camera> -1,6; especializacion explicita de `vector<string>::__init_unk` en `vector_string.h`:
  99,39 pero la cadena pasa a `@STRING@__init_unk...` y bajan push_back y fn_80220168 de DebugInfo; local para
  los datos (`T* data = alloc(n)`), `p` como parametro, reserva+cap en otro nivel (con o sin la comprobacion de
  longitud), cinco formas del bucle (for/while, `++p` dentro, `p += size`): nada o peor.
- `slow`: `return` en vez de `else`, `push_back` con `else` en vez de `return`: identico.
- Un nivel mas en `expand_by` (`__expand_by2_unk`): identico; un nivel mas en realloc: peor (9 bajadas).
- `__vec_grow_sel_unk` solo alrededor de `__grow_cap_unk` (devuelve cap): igual que sin nivel (se pierden los 4
  al 100 %); con `buf.mStart` o `buf.mCap` fuera del nivel, o la reserva devuelta con `cap` por referencia: identico
  a W2; declarar el buffer despues de la reserva: cambia el orden del codigo (-306).
- Para la rotacion del puntero al buffer: ctor del buffer con lista de inicializacion en vez de `w[0..2]`, bucle
  muerto del destructor otra vez dentro: identico.

## Funciones al 100 % / 0 %

Sin cambios en el codigo (nada commiteado). Con el parche: +5 al 100 % (las de A4), 0 % sin cambios.

## Para el port

Ningun cambio (no se integra). Si se integra: solo `include/std/vector*.h` y especializaciones locales (miembros
nuevos de buffers y selectores internos de MSL; `__vec_grow_sel_unk<U16RunPair_unk>` en U16Table_unk.cpp);
ningun layout ni firma de clases del juego.

## Decisiones tomadas sin el usuario

- No commitear el parche: quedan 8 bajadas (4 de 0,14-0,64 y 4 de <= 0,02), todas de registros. Decision para el
  usuario: integrar `qA5.patch` (con re-emparejado en lote completo por ControlAppear y los inline nuevos) a
  cambio de +5 al 100 % y la forma de pila del original, o seguir con la rotacion de registros.
- La especializacion local de `__vec_grow_sel_unk` en U16Table es un ajuste por tipo (sin explicacion general);
  se puede quitar del parche (vuelve la bajada de 0,03).

## Hallazgos (una linea)

- Las bajadas pequenas del parche de push_back son una rotacion de registros heredada (puntero al buffer de
  realloc con registro mas alto que en el original) que la forma nueva (huecos de pila y `cap` separado de `mCap`,
  ya como el original) hace visible en mas instrucciones.
- `__construct_at_end_unk` con `first`/`last` por referencia sube TextAnimation `push_back<vector<float>>` +2,5.
