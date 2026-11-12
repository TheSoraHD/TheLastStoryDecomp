# Lote paralelo 2026-11-12, cola G (eventos)

Inicio: 2026-11-12 16:35 CET. Tope ~19:05.

Total: funciones al 0 % de la cola tocadas 45 -> 26 (DebugInfo, Node, Puppeteer); `report.json` 15117 -> 15123
(+6, lost 0, sin bajadas). Ninguna unidad pasa a enlazar.

## Resultado por unidad (funciones al 0 % / sin escribir, antes -> despues)

- `Event/DebugInfo_unk`: 17 -> 4. Pagina 2 de la depuracion (`fn_8022354C`, lista de cortes) 96,3 %. Tabla de la depuracion (`DebugInfoTable_unk`): `fn_80220168` (fija las
  columnas, 98,2 %), `fn_8022051C` (anade una fila arriba, 100 %), `fn_802214F8` (sustituye la fila de una
  clave, 55 %), `fn_802219CC` (dibuja la tabla, 88,9 %); copias debiles nuevas: `fn_80220620` (llena, 100 %),
  `size`/`back`/`operator[]` del vector de filas (100 %), `push_back` (98,6 %), constructor copia de la fila
  (99,6 %), `operator=` de la fila (53 %). 35 -> 40 funciones al 100 %.

- `Event/Node`: 6 -> 2. `Action::fn_805393F8` (fija una clave de la curva, la
  ordena y devuelve su indice) 96,7 %; copias de `std::sort` con `game/MslSort_unk.h` (cola B): sort 72,8 %,
  sort recursivo 72,6 %, mediana 56 %. Quedan `fn_805377F0`, `fn_805384CC` (0 %).

- `Event/Puppeteer_CharaTask`: 22 -> 20. `Actor::fn_8001011C` (mueve el actor, 99,2 %; la matriz del modelo
  y su escala mayor con un `max` evaluado dos veces, como una macro); `Actor::fn_800112E0` (frente del actor, 84 %, ahora miembro;
  `Event/ActionCameraMode` lo llamaba con un `extern "C"`); `atn::Vector3::fn_80011410` movida antes de su
  primer uso (sigue al 100 %).

## NONMATCHING

- `DebugInfoTable_unk::fn_802214F8` 55 % y `DebugInfoColumn_unk::operator=` 53 %: orden de bloques de
  `vector<string>::operator=` inline (el original pone el camino de asignacion entre el crecimiento y la
  reasignacion). Probado el crecimiento escrito dentro (literal en `@STRING@`, no en `@stringBase0`: peor).
- `fn_80220168` 98,2 %: registros del bucle de `clear()`.
- `fn_802219CC` 88,9 %: planificacion de la primera llamada de dibujo.

- `Action::fn_805393F8` 96,7 % y sus `std::sort` 56-73 %: el original copia las claves miembro a miembro
  (floats), como si `ActionKey_unk` no fuera POD. Con un constructor vacio en `Event/Action.h` la mediana da
  100 % y los sorts 87 %, pero renumera las `@N` de todas las unidades de Event: no se pone.

## Decisiones tomadas sin el usuario

- `src/Event/DebugInfo_unk.cpp`: especializacion inline de `vector<string>::operator=` y su crecimiento
  (`__vec_grow_assign_unk`) definidos en el propio fuente, no en `std/vector.h`/`std/vector_string.h`: anadir
  plantillas a esas cabeceras (aun sin usarlas) renumera las `@N` de todas las unidades que las incluyen
  (probado: labelcheck peor en ~300 unidades). Especializacion local de `vector<DebugInfoColumn_unk>::reserve` (copia las
  filas en vez de moverlas como bytes, como el original); `__declspec(noinline)` en `fn_80220168` (el
  constructor la llama con `bl`; regla 2, caso aceptado).
- `DebugInfoColumn_unk::m00` pasa a `const void*` (se compara sin signo) y `DebugInfoTable_unk::m00` a
  `const DebugInfoColumnDef_unk*`.

## Hallazgos

- `fn_8022354C` (lista de cortes): el original dibuja la marca `>` con `m04 + m0C * fila` como y (usa la x).

- Declarar una plantilla nueva (aunque no se instancie) o un constructor inline en una cabecera muy incluida
  renumera las etiquetas `@N` de todas las unidades que la incluyen (labelcheck/rep100 peor en todas).

- `fn_8022051C` pasa el presupuesto R3 (por el `operator=` de `vector<string>` inline): todas sus llamadas
  quedan fuera de linea y salen las copias debiles en el orden del original.
