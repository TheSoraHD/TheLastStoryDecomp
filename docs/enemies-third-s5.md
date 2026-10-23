# Lote enemies-third, cola s5 (Em403WarLion, EnemyJill) - segunda pasada

Rama agent-s5 (sale de main d51a317). Referencia: /home/claude/ref/s5_start.json. El contenedor se reinicio a
mitad de cola; se retomo desde los ficheros sin commit.

## Unidades

- `game/Em403WarLion`: **39/67 -> 45/67** al 100%. Escritos los helpers que faltaban (0x8032AB90-0x8032C1EC:
  copias debiles de `std::vector<atn::Vector3>`, `std::vector<float>` y del spline cubico natural), el pool de
  cadenas queda completo (funcion de menu de depuracion eliminada por el enlazador, como en Em201ZanGurg) y con
  ello pasan al 100% fn_8032A9F4, fn_8032AB90, fn_8032ABA4, el destructor, vfE4, vf90 y fn_80327A64.
  Poolcheck sin avisos. No enlazable aun. NONMATCHING nuevos o que siguen:
  - fn_8032AC1C 94.58%: push_back inlineado de std/vector.h (capacidad en r31 en el original; familia conocida).
  - fn_8032AF90 97.80%: r30/r31 de los puntos y la cuenta (mismo residuo que PathData_unk.cpp fn_8054875C).
  - fn_8032B7F0 87.22%: registros del bucle de sustitucion hacia atras desenrollado y de los tres vectores.
  - fn_8032B314 99.96%, fn_8032B6F8 99.84%, fn_8032BE88 99.86%: solo el mensaje de `__allocate`
    ("Memory allocation failure" de otro fichero: lbl_80779F68 / lbl_80775A88); BE88 ademas r28/r31.
  - fn_8032C1EC 99.83%: r27/r28 de los dos puntos.
  - fn_80327EF0 99.96%: operandos de un fmuls (8 variantes probadas).
  - ctor 95.5%: planificacion de los cuatro `__shrink_unk(0)` de Reset_unk (sin cambios).
  - Siguen los de la q7 (fn_80324A38, vf24, fn_803256F8, fn_803258D4, vf20, vf50, fn_803267E8, fn_803278B4,
    fn_8032943C, fn_80329744, fn_80329A20, fn_80329ED8, fn_8032A3D4).
- `game/EnemyJill`: **30/39 -> 30/39**. Se probo el prototipo coherente de fn_800F8C6C (`int c, float f`):
  baja fn_80330218 (100 -> 99.45) y fn_8033135C (100 -> 96.87), asi que se mantiene `float f, int c` con un
  comentario (mismos registros r10/f1; solo cambia el orden de evaluacion). fn_8032FDE8: 4 variantes mas, sin
  mejora (98.22).

Unidades enlazables: ninguna.

## Decisiones tomadas sin el usuario

- `include/std/vector.h` (cabecera compartida; full build sin bajadas, lost 0, fuzzy drops 0):
  - `resize` para elementos de clase y float: `__append_unk` reescrito (comprobacion `n > cap || size > cap - n`,
    suma por `__vec_sum_unk`, recomendacion muerta por `capacity()` en `__recommend_cap_unk`), `__append_n_unk`
    en `__vec_imp_unk`/`__vec_split_unk` (sustituye a `__append_zero_unk` de punteros), `__push_front_resize_unk`
    y `__swap_out_resize_unk` (el resize separa los dos decrementos del bucle; push_back no).
  - Especializaciones `float` (`__vec_imp_unk<float>`, `__vec_split_unk<float>`, `__shrink_unk` sin bucle y un
    `__append_unk` propio con `__append_realloc_unk`); con ellas la copia debil 0x8000D844 (~vector<float>)
    compila identica.
  - `vector(size_type n)` y `__init_n_unk` (fuera de la clase, mensaje al pool del fichero), `capacity()`, y
    `__allocate` publico.
- `config/SLSP01/splits.txt`: `.data` de game/Em403WarLion.cpp hasta 0x80788D20 (antes 0x80788D00): el
  "Memory allocation failure" de 0x80788D00 es el `@STRING@__allocate` debil de `vector<Vector3>` de esta unidad
  (lo usan tambien PathData_unk, Event/*: deduplicado por el enlazador).
- `config/SLSP01/symbols.txt`: `lbl_80788D00` -> `@STRING@__allocate__Q23std22vector<Q23atn7Vector3>FUi`.
- `include/game/Em403WarLion.h`: `WarLionPathList_unk` pasa a derivar de `std::vector<float>` (se quita
  `WarLionLen_unk`); `WarLionCubic_unk` con constructor vacio; `WarLionPath_unk::Eval_unk`; `WarLionRes_unk::Load`
  y `Em403WarLion::fn_DebugMenu_unk` (inventada, eliminada por el enlazador; solo coloca "em403_war_lion" y
  "DebugDisp" en el pool).
- Em403WarLion.cpp: los helpers son `extern "C" inline` (copias debiles con nombre C, porque gmk/CharaPathMove,
  game/PathData_unk, Event/* los llaman asi), definidos antes de fn_8032A9F4; los puntos y los floats temporales
  son structs POD (`WarLionPoints_unk`, `WarLionFloats_unk`) como `PathVec_unk` de PathData_unk.cpp.
  `fn_8032AC1C` lleva ~80 asserts vacios (`WARLION_ASSERT_UNK`, regla del umbral de autoinline): el original
  supera el presupuesto en fn_8032A9F4 y llama a todos los helpers; es la forma aceptada en CLAUDE.md, pero el
  numero es grande (anotado).

## Hallazgos MWCC

- Un `extern "C" inline` cuyos literales deben ir a `@stringBase0`: delegar en un miembro definido fuera de la
  clase (`vector<T>::__init_n_unk`); en la propia funcion inline van a `@STRING@`.
- `new (p) std::vector<T>()` en una funcion suelta anade `cmpwi r3,0; beqlr`; escribir los tres campos a mano.
- El destructor borrador de un vector POD-raw: llamar al destructor de la base (`__vec_imp_unk::~...`) da las
  tres comprobaciones de `this` del original; el de `vector` da cuatro.
- Un bucle `for (j = i + 1 ...) ... j++` reutilizado cambia el registro del indice; con un segundo indice `k`
  sale el del original (fn_8032A9F4).

## Pendientes para otras colas

- gmk/CharaPathMove, game/PathData_unk, Event/Manager, Event/ActionActorPosition, gmk/WindyItem y
  game/LastCocoonQuadruped pueden llamar ya a los helpers por nombre C (o reconstruir su spline con el mismo
  codigo). PathData_unk.cpp declara `fn_8032B314`/`fn_8032BE88` con `int n` (aqui `unsigned int`): mismo ABI.
