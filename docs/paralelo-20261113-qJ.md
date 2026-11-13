# paralelo-20261113, cola J (juego varios 2)

## Resultado por unidad

- `game/RouteWalker_unk` 6 -> 2 al 0 %. Al 100 %: `fn_80128CF8` (meta a la vista), `fn_80129670` (segmento entre
  dos puntos). Escritas: `fn_80128090` (97,05 %, replanifica el modo actual), `fn_80129144` (98,97 %, va directo a
  la meta). Quedan `fn_801255C8` (3 KB) y `fn_80126214` (6 KB).

## NONMATCHING

- `RouteWalker_unk::fn_80129144` 98,97 %: solo registros (tabla en r5 y `m44` en r4 en el original). Probado: local
  para `m44` antes y despues de la tabla, tabla pedida en el caso 3, getter inline del sitio, orden de los casos,
  bandera de `m48` tras el switch.
- `RouteWalker_unk::fn_80128090` 97,05 %: solo registros de los planificadores inlineados (el destino en r26).

## Decisiones tomadas sin el usuario

- `__declspec(noinline)` en la definicion de `RouteWalker_unk::fn_801286F0` (excepcion aceptada de la regla 2): el
  original la llama con `bl` desde `fn_80128090`, que inlinea los otros planificadores (`fn_80128818`...). Sigue al
  100 %.
- Cuatro miembros nuevos de `RouteWalker_unk` en `game/CharaStatus.h` (269 unidades), sin nombres de parametro:
  `fn_80128090`, `fn_80128CF8`, `fn_80129144`, `fn_80129670(int, int)`. Precommit sin bajadas.
- `RouteWalker_unk.cpp` incluye `game/GameConfig_unk.h` (`lbl_8087F0A8->m060`) y `math/VecAssign_unk.h`: los `@N`
  de la unidad cambian (renombrados en `symbols.txt` con `pd.sh`).

## Hallazgos

- Producto vectorial en una inline `Cross_unk(a, b)` que devuelve por valor y comprobacion de linea
  `LineHit_unk(const Vector3&, const Vector3&)` con temporales (`from + side`) dan los huecos de pila del original
  (RouteWalker_unk::fn_80128CF8).
- `RoutePoint_unk* GetPoint_unk(table, i)` inline (`&mPoints[i - 1]`) arregla el reparto de registros frente a
  indexar `mPoints[i - 1]` directamente (fn_80129670).

## Pendientes

- RouteWalker_unk `fn_801255C8`, `fn_80126214`.
