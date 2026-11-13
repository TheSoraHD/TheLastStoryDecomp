# Lote paralelo 2026-11-13, cola F (eventos 1)

Inicio: 2026-11-13 03:12 CET. Tope 05:42.

## Resultado por unidad (funciones al 0 % / sin escribir, antes -> despues)

- `Event/DebugInfo_unk`: 5 -> 0. Paginas 0 (`fn_80221FA4`, evento, corte, frames, MA, fps; 99,83 %) y 1
  (`fn_80222AD0`, estado del manager y de la secuencia; 99,31 %) escritas; `fn_80222A38` (copia del nombre
  del MA, 100 %), `fn_80223A08` (100 %, ahora sale como copia debil); pagina 4 `fn_80224184` 0 -> 99,92 %
  (deja de ser borrador). 40 -> 42 funciones al 100 %. Las paginas que quedan en 99,x % fallan solo por los
  desplazamientos de `@stringBase0` (ver NONMATCHING).

- `Event/Node`: 2 -> 0. `Action::fn_805377F0` (pone la accion al comando `id`: atributos por defecto y curva
  (0,1)-(1,1); 94,2 %) y `Action::fn_805384CC` (posicion relativa al nodo de un actor, girada por su yaw; 99,37 %).
  `fn_805382BC`/`fn_805384CC` llaman ahora a `Actor::fn_80011964` como miembro (antes `extern "C"`).

- `Event/Sequence`: el borrador `fn_8054204C` (anade la curva `id` a la lista 0x184) escrito y equivalente (0,25 ->
  10 %). Los otros dos borradores (`fn_80542684`, `fn_805428A8`) no son metodos de `Sequence`: son las copias
  debiles de la construccion y de la reallocacion del `push_back` de `vector<EffectEntry24_unk>` (comentado).
  Las 19 plantillas al 0 % (0x8054372C-0x80545C3C) siguen sin escribir (ver Pendientes).

## NONMATCHING

- `Event/DebugInfo_unk`: falta en el pool el `""` que el original pone justo tras `"vector length error"`
  (lo usan las paginas 2-5, pero se crea antes de la pagina 0; no se encontro que funcion lo crea: ninguna
  funcion anterior lo referencia). Todas las paginas quedan un byte desplazadas. Pagina 0: ademas los huecos de
  pila de las dos etiquetas de iterador de los `+=` de cadena (probado `append`, una inline intermedia, una
  local). Pagina 1: registros de los tres booleanos de los canales de sonido (5 ordenes de declaracion).

- `Action::fn_805377F0` 94,2 %: el `resize` inline de los atributos (plantilla de `std/vector.h`): el original
  rellena con `{0x28, 0}` (constructor de `ActionParam_unk`; puesto en `Event/Action.h` da 92,1 %, no se deja:
  16 unidades incluyen la cabecera) y sus temporales de pila ocupan 8 bytes menos.
- `Action::fn_805384CC` 99,37 %: orden del store de `r.z` dentro de `Vector3::RotZ_unk` (`math/Vec.h`).

- `Sequence::fn_8054204C` 10 %: con `std/vector.h` la funcion pasa el presupuesto R3 y llama a todas sus inline;
  el original las inlinea todas salvo la construccion y la reallocacion del `push_back` de m184 (como si su
  `push_back` fuera una inline pequena sobre dos ayudantes). `poolcheck` avisa ahora de 3.0 y 1/64 en
  Sequence.o (la copia debil de `EffectLists_unk::fn_8048A968` que sale aqui; la unidad no se enlaza).

## Decisiones tomadas sin el usuario

- `fn_80224184` (pagina 4): 30 asertos vacios (`SEQ_ASSERT10_UNK` x3, el macro de `Event/Sequence.h`) en el
  accesor inline `fn_802245C0`, para pasar el presupuesto de inlining R3 (el original llama a todas las
  inline de la pagina: `fn_8000DB1C`, `fn_80221CAC`, el ctor de la fila, `push_back`...). Con 20 no basta. Los
  accesores pierden el `__declspec(noinline)` (ya no hace falta).
- `fn_80221CF8` con `__declspec(noinline)` (la pagina 0 la llama con `bl`; caso aceptado de la regla 2).
- Nombres de estado y de tipo de la secuencia como inline con enlace externo (`GetSequenceStateName_unk`,
  `GetSequenceTypeName_unk`): sus literales van a `.data` en orden inverso, como en el original.

- `Event/Node.cpp`: fuera la declaracion `extern "C" float fn_80011964(Event::Actor*)` (regla 10: es un
  metodo de `Actor`, ya declarado en `Event/Actor.h`).

## Hallazgos

- Funcion que devuelve por valor una copia de un `std::string` miembro: con el parametro `const T*` sale el
  `lwz` de la primera palabra antes de guardar los registros, como el original (`fn_80222A38`).
- Pagina 1: el original evalua los argumentos de `sprintf` como llamadas (getters inline: `GetCurCam_unk`,
  `GetWaitMode_unk`, `GetStartCut_unk`): con miembros directos salen cargados antes.

- Direccion de un nodo a angulos con `Vector3::ToRotationZ_unk()` (la variante que lee z en una local): el
  original redondea z una sola vez (`fn_805384CC`).

## Pendientes

- `Event/Sequence` 0x8054372C-0x80545C3C (19 plantillas, y `EventParser_unk` fn_8057DB14/DC38/E274, y `Manager`
  fn_80482400/8049003C/80490098): todas salen de `push_back` con otra forma de la de `std/vector.h`. Lo visto:
  (1) el `push_back` del original parece una inline pequena `size < cap ? construct : realloc(x)`, cuyos dos
  ayudantes se inlinean o no segun el presupuesto del llamador (Add_unk<Actor> inlinea el realloc con
  `__grow_cap` sin la comprobacion final, `__vec_set_buf(buf, n, cap)` con ella, `__append_n` y
  `__push_front` fuera de linea; fn_8054204C llama a construct y realloc); (2) la copia implicita de `Actor`
  rechaza sus candidatas (las copias de sus miembros salen como copias debiles) porque
  `atn::SimpleVector` tiene en el original un constructor copia profundo (fn_80544070: `new[]` +
  `__construct_new_array` + copia), que `atn/Array_unk.h` (352 unidades) no declara: la copia actual es
  superficial. Las dos cosas tocan cabeceras muy compartidas.

- `ActionParam_unk` probablemente tiene un constructor `m00(0x28), m04(0)` (resize de `fn_805377F0`); ponerlo
  en `Event/Action.h` exige revisar las 16 unidades que la incluyen.
