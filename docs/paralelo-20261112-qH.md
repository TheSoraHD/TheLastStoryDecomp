# Cola H (enemigos) - lote paralelo 2026-11-12

Inicio: 2026-11-12 17:20 CET (reinicio del servidor 18:33-20:16; cierre previsto 21:30). Rama `agent-H`.

## Resultado por unidad

- `game/Em316LaserSphere`: al 0 % 9 -> 6 (`fn_8031958C` y sus copias weak `fn_80318F90`, `fn_80319998`, las tres al 100 %); 22 -> 25/40 al 100 %.

- `game/EnemyQuarkSecond`: `fn_8033A7A0` (antes 0 %, escrita) al 100 % (43 -> 44/70); `fn_80342674`, `fn_80342BC8`
  y `fn_8034315C` escritas (no existian; NONMATCHING 90-93 %, huecos de pila de los temporales del salto, como
  fn_8034228C). `Field_unk` (local del fichero) gana `m10D8` (tabla de rutas).
- `game/Em316LastLaser`: al 0 % 5 -> 0; `fn_8031CA30`, `fn_8031D350` y `LaserTrace::fn_8031E8BC` al 100 %
  (8 -> 11/19); `fn_8031D6DC` 98,4 % y `fn_8031E078` 98,9 % (NONMATCHING: marco 0x10 menor).
- `game/Em206CocoonMp`: al 0 % 6 -> 5; `vf104` escrita (NONMATCHING 99,9 %: nombres de las relocalizaciones del
  ctor/dtor de la lista y un registro de float). 27/39 al 100 % (sin cambios).
- `game/Em202ForestSpider`: `fn_80279280` (antes DRAFT al 0 %, no equivalente) reescrita al 100 % (39 -> 40/64).

## Decisiones tomadas sin el usuario

- Em316LaserSphere `fn_8031958C`: el original lo llama todo fuera de linea (presupuesto R3); 300 asserts vacios
  (`SPHERE_ASSERT100_UNK`) en la inline `fn_80267B20` (copia weak de otra unidad, solo la usa esta funcion).
- Renumeracion `@N` en `symbols.txt` de la unidad (sdata2pair + relocpair + a mano); `lbl_80884DC8..DD4` pasan a `@N` locales
  (solo los usa esta unidad).

- EnemyQuarkSecond `fn_8033A7A0`: `NextInTable_unk` pasa a macro (`QUARK_NEXT_IN_TABLE_UNK`; el original la inlinea
  pero llama fuera de linea sus accesores) y 300 asserts vacios en `fn_800E0AA8` (copia weak de otra unidad). La
  declaracion `NextInTable_unk` se queda en `EnemyQuarkSecond.h` sin uso: quitarla renumera `@N` de EnemyQuarkFirst y
  SpecialEnemyBase (incluyen la cabecera).
- Em316LastLaser: `fn_8031D6DC`/`fn_8031E078` son funciones `static` del fichero con el `LaserTrace*` como primer
  parametro (simbolos locales sin nombre y literales en `@stringBase0`); renombradas en `symbols.txt` a su nombre
  mangled con `scope:local`. `LaserTrace::m10..m24` pasan de seis `float` a dos `atn::Vector3` (ctor sigue al 100 %).
  300-600 asserts vacios en `fn_802657FC` y `fn_800F7FA8` (copias weak de otras unidades, solo las llaman
  fn_8031D350 y fn_8031E8BC). `lbl_80884E34..E40` pasan a `@N` locales. `TargetDist_unk` (distancia a "spine1"
  de vf10) pasa a `static inline SpineDist_unk` (su literal va en `@stringBase0`); la declaracion del miembro se queda
  sin definir en la cabecera para no mover los `@N` de SpecialEnemyBase/SpecialEnemyFactory_unk.
- Em206CocoonMp `vf104`: lista de candidatos como `std::vector<EnemyTask*>` (el original llama a `fn_802A4170` y a
  `__dt__18NecroPlaceList_unkFv` 0x80244674; ese nombre de Em001 parece erroneo, lo llaman tambien GameTask y
  Em203GreaterDaemonRedMp con `push_back` de `vector<EnemyTask*>`: **pendiente** renombrarlo, toca otras colas).
  Con el indice en un local sale todo igual salvo esos nombres, pero `report.json` la cuenta al 100 % y objdiff-cli
  no (REP+): se deja la forma con `CharaTask* t` (99,9 %). `fn_802BEE44`/`fn_802BF5C0` declaradas como miembros
  (renombradas en `symbols.txt` a su nombre mangled); se incluye `game/PlayerManager.h`. 300 asserts vacios en
  `fn_800E0AB0` (copia weak).
- Em202ForestSpider `fn_80279280`: las copias weak 0x80279788-0x802797D4 pasan de funciones fuertes a
  `extern "C" inline` definidas antes (salen detras en orden de primer uso); 0x802797A8 es el constructor de
  `GimmickMsg_unk` (renombrado en `symbols.txt` a `__ct__14GimmickMsg_unkFv`). `fn_800C344C` y `fn_80276AE4` toman
  referencias en vez de punteros (mismo codigo). 900 asserts vacios en `fn_80267B20` (copia weak).

## Hallazgos

- `bge X; b Y` sobre un clamp: `int cur = index; if (cur < n - 1) { i = cur; } else { i = n - 1; }` (EnemyQuarkSecond).
- `static inline` deja los literales en `@stringBase0` (con `inline` solo van a `@STRING@`) y se inlinea en una
  llamadora dentro de presupuesto mientras la que esta fuera la llama (Em316LastLaser fn_8031E078).
- Temporales pasados por referencia a una funcion `extern "C"`: declararla con `const atn::Vector3&` en vez de
  puntero evita el aviso "not an lvalue" y deja el temporal en la zona baja de la pila como el original.
- Quitar una declaracion de una cabecera renumera los `@N` de todas las unidades que la incluyen.

