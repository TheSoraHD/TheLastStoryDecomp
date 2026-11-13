# Lote paralelo 2026-11-13, cola F (eventos 1)

Inicio: 2026-11-13 03:12 CET. Tope 05:42.

## Resultado por unidad (funciones al 0 % / sin escribir, antes -> despues)

- `Event/DebugInfo_unk`: 5 -> 0. Paginas 0 (`fn_80221FA4`, evento, corte, frames, MA, fps; 99,83 %) y 1
  (`fn_80222AD0`, estado del manager y de la secuencia; 99,31 %) escritas; `fn_80222A38` (copia del nombre
  del MA, 100 %), `fn_80223A08` (100 %, ahora sale como copia debil); pagina 4 `fn_80224184` 0 -> 99,92 %
  (deja de ser borrador). 40 -> 42 funciones al 100 %. Las paginas que quedan en 99,x % fallan solo por los
  desplazamientos de `@stringBase0` (ver NONMATCHING).

## NONMATCHING

- `Event/DebugInfo_unk`: falta en el pool el `""` que el original pone justo tras `"vector length error"`
  (lo usan las paginas 2-5, pero se crea antes de la pagina 0; no se encontro que funcion lo crea: ninguna
  funcion anterior lo referencia). Todas las paginas quedan un byte desplazadas. Pagina 0: ademas los huecos de
  pila de las dos etiquetas de iterador de los `+=` de cadena (probado `append`, una inline intermedia, una
  local). Pagina 1: registros de los tres booleanos de los canales de sonido (5 ordenes de declaracion).

## Decisiones tomadas sin el usuario

- `fn_80224184` (pagina 4): 30 asertos vacios (`SEQ_ASSERT10_UNK` x3, el macro de `Event/Sequence.h`) en el
  accesor inline `fn_802245C0`, para pasar el presupuesto de inlining R3 (el original llama a todas las
  inline de la pagina: `fn_8000DB1C`, `fn_80221CAC`, el ctor de la fila, `push_back`...). Con 20 no basta. Los
  accesores pierden el `__declspec(noinline)` (ya no hace falta).
- `fn_80221CF8` con `__declspec(noinline)` (la pagina 0 la llama con `bl`; caso aceptado de la regla 2).
- Nombres de estado y de tipo de la secuencia como inline con enlace externo (`GetSequenceStateName_unk`,
  `GetSequenceTypeName_unk`): sus literales van a `.data` en orden inverso, como en el original.

## Hallazgos

- Funcion que devuelve por valor una copia de un `std::string` miembro: con el parametro `const T*` sale el
  `lwz` de la primera palabra antes de guardar los registros, como el original (`fn_80222A38`).
- Pagina 1: el original evalua los argumentos de `sprintf` como llamadas (getters inline: `GetCurCam_unk`,
  `GetWaitMode_unk`, `GetStartCut_unk`): con miembros directos salen cargados antes.

## Pendientes
