# port-decomp-20261114, cola G: una sola `CollResult_unk`

Objetivo: las vistas locales del resultado de las consultas de colision (0x50 bytes; `CollResult_unk` y
otros nombres) pasan al tipo del motor `atn::ColliResult_unk` (`include/atn/ColliBase.h`), para que el
port tenga una sola definicion (punteros en m34/m38/m40).

## Resultado

- **Todas** las vistas pasan a `atn::ColliResult_unk`, salvo `game/BattleTask` (cola H) y
  `RayResult_unk` de `game/BattleCharaTask` (cola H): pendientes, ver abajo.
  - 43 `struct CollResult_unk` locales (CharaTask, ColliAttrManager, EnemyLizerdBlade, Em306GurgBeast,
    Em201ZanGurgFinal, Em315Coffin, FlamingArrow, ControlStickByQuark, ControlTurnQuadruped y las 34 de
    enemigos/gimmicks) y la de `gmk/Lemon.h` (usada por 14 unidades).
  - 13 vistas con otro nombre y el mismo constructor: `GhoulCollResult_unk` (Em309Ghoul), `EnvSeHit_unk`
    (EnvSeBase), `BigMulueColl_unk`, `KrakenColl_unk`, `GhoulColl_unk`, `MulueColl_unk`,
    `QuadCollResult_unk` (LastCocoonQuadruped), `KrakenMpColl_unk`, `GrappleCollResult_unk` (ControlJumpAt),
    `LadderCollResult_unk`, `WallCollResult_unk`, `DropCollResult_unk`, `MountleCollResult_unk`.
  - `CollObj_unk` (CharaTask) y `CollPoly_unk` (Em202Spider, Em202ForestSpider) = `atn::ColliBase`
    (`m08` -> `mFlags`; `fn_801A03F4`/`fn_801A0400` de ForestSpider toman `atn::ColliBase*`).
- Funciones: 15392 -> 15392 (lost 0, fuzzdrop 0), labelcheck/rep100 igual que `$REF`, `main.dol: OK`,
  poolcheck igual que la base en las 85 unidades tocadas. Ninguna unidad tocada esta enlazada salvo
  las que solo incluyen `gmk/Lemon.h` (GimmickCommon, MapTask, LemonBucket...: solo renumeracion `@N`).

## Decisiones (sin el usuario)

- **Constructor fuera de linea 0x80140500**: es la copia debil del constructor inline de
  `atn::ColliResult_unk`. Lo llaman con `bl` funciones de 12 unidades (CharaTask, Em316LastLaser,
  Em316LaserSphere, Em202ForestSpider, Em203GreaterDaemon, Em306GurgBeast, Em315Coffin, Em403WarLion,
  GameCamera_unk, Cannonball, FlamingArrow, ControlStickByQuark): cuando la funcion pasa del presupuesto
  de inline, MWCC emite la copia debil en la unidad y el enlazador se queda con la de CharaTask (la
  primera). Renombrado en `symbols.txt`: `__ct__14CollResult_unkFv` -> `__ct__Q23atn15ColliResult_unkFv`
  (sigue al 100 %, y `Em316LastLaser::fn_8031D350` etc. emparejan su `bl`). Se quita la definicion
  explicita de CharaTask.cpp.
  - **Pendiente (enlace de CharaTask):** nuestra copia debil sale antes de `fn_8013CB68` (su primer
    llamador no inlineado en nuestro fuente); en el original esta en 0x80140500, entre `fn_801404F8` y
    `fn_80140518`. Se movera cuando las funciones grandes de CharaTask (`fn_8013CB68`, `fn_80140588`,
    NONMATCHING) gasten el presupuesto de inline como el original. `ordercheck` ignora los simbolos
    debiles: 0 fuera de orden.
- **Campos**: los tipos de las vistas se leen con cast en el sitio (`*(unsigned int*)result.m34`,
  `*(int*)result.m34`, `(CharaTask*)result.m38->m0C`) y los nombres propios pasan a offsets (`mPos` ->
  `m10`, `mNormal` -> `m28`, `m10_unk` -> `m10`). **No** se anaden accesores a `ColliBase.h`:
  - un metodo inline nuevo en `ColliResult_unk` (`IsKind1A_unk`) renumera los `@N` de todas las unidades
    que incluyen la cabecera (el `@enum$6811` de ControlAppear paso a 6818 y se perdieron 2 funciones);
  - un accesor `AttrId_unk()` mete un nivel de inline que cambia el codigo de `fn_8013CB68`,
    `fn_80140588` y `fn_801426A4` de CharaTask (una instruccion mas).
  `IsKind1A_unk` queda como funcion inline libre en CharaTask.cpp, antes de `IsLanded_unk`.
- Probado y descartado: una derivada `struct X : atn::ColliResult_unk { X(); }` con el constructor fuera
  de linea (mantenia el nombre mangled, pero son dos tipos) y una derivada con constructor inline en
  `gmk/Lemon.h` (el inline de dos niveles consume mas `@N` y bajaba `GameCamera::fn_803903E0`
  98,54 -> 98,45; con el tipo del motor, fuzzy y etiquetas como antes).
- `ControlTurnQuadruped`: su vista media 0x48 (`m44` int); con 0x50 no baja nada.

## Cambios de firma (para el port; nombres mangled renombrados en `symbols.txt`)

- `Bullet::fn_80110858(atn::ColliResult_unk*)` (antes `CollResult_unk*`).
- `ColliAttrManager::fn_80184864(atn::ColliResult_unk*, CharaTask*, int, int)`.
- `gmk::Lemon::fn_80416268(atn::ColliResult_unk*, const atn::Vector3*, const atn::Vector3*, float)`.
- `__ct__Q23atn15ColliResult_unkFv` (0x80140500), ver arriba.
- `CharaTask::IsLanded_unk(const atn::ColliResult_unk& result)` (inline, sin simbolo; el nombre del
  parametro se queda: quitarlo renumera un `@N`).
- Cabeceras: `Bullet.h`/`ColliAttrManager.h` declaran `namespace atn { struct ColliResult_unk; }` en vez de
  `struct CollResult_unk;`; `gmk/Lemon.h` incluye `atn/ColliBase.h` y ya no define `CollResult_unk`.
- `extern "C" fn_8004ED34`/`fn_8004D388`/... siguen declaradas en cada fuente (79 ficheros), ahora con
  `atn::ColliResult_unk*` donde tenian la vista.

## Re-emparejado de etiquetas (como se hizo)

Quitar una vista con constructor inline renumera -4 los `@N` posteriores de la unidad (y de las ~30 que
incluyen `gmk/Lemon.h`). `pdbatch.sh` (pairdata por valor) deja etiquetas ambiguas en 6-10 unidades
(constantes repetidas). Solucion exacta: compilar la base (`git worktree` en e41cd3a, fuera del repo) y
renombrar cada `@N` de `symbols.txt` (rango de la unidad) al nombre que tiene en nuestro `.o` la etiqueta
del mismo offset y seccion (`maprelabel`: old .o -> new .o, solo si los tamanos de seccion coinciden), a
partir del `symbols.txt` de la base. 85 unidades, ~3000 renombrados, todos `@N`.

## Pendientes

- `game/BattleTask` (`struct CollResult_unk` local con `unsigned int* m34`) y `game/BattleCharaTask`
  (`RayResult_unk`): de la cola H. Pasarlos a `atn::ColliResult_unk` (`*(unsigned int*)r.m34`) y
  re-emparejar sus `@N` (-4 tras la vista).
- Posicion de la copia debil del constructor en CharaTask (arriba).
- `game/GameEventScript`: 1 fuera de orden (`__dt__15GameEventScriptFv`) ya en la base, no tocado.

## Hallazgos

- Un inline de dos niveles (constructor de una derivada vacia que llama al inline de la base) consume
  mas `@N` que el inline directo: sirve para decidir que tipo usaba el original.
- Tras un cambio de cabecera, mapear las etiquetas por offset entre el `.o` de la base y el nuevo es
  exacto; `pdbatch.sh` por valor falla con constantes repetidas.
