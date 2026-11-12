# Cola H (enemigos) - lote paralelo 2026-11-12

Inicio: 2026-11-12 17:20 CET. Rama `agent-H`.

## Resultado por unidad

- `game/Em316LaserSphere`: al 0 % 9 -> 6 (`fn_8031958C` y sus copias weak `fn_80318F90`, `fn_80319998`, las tres al 100 %); 22 -> 25/40 al 100 %.

- `game/EnemyQuarkSecond`: `fn_8033A7A0` (antes 0 %, escrita) al 100 % (43 -> 44/70).
- `game/Em316LastLaser`: al 0 % 5 -> 1 (falta `fn_8031CA30`); `fn_8031D350` y `LaserTrace::fn_8031E8BC` al 100 %
  (8 -> 10/19); `fn_8031D6DC` 98,4 % y `fn_8031E078` 98,9 % (NONMATCHING: marco 0x10 menor y offset de `""`).

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
  fn_8031D350 y fn_8031E8BC). `lbl_80884E34..E40` pasan a `@N` locales.

## Hallazgos

- `bge X; b Y` sobre un clamp: `int cur = index; if (cur < n - 1) { i = cur; } else { i = n - 1; }` (EnemyQuarkSecond).
- `static inline` deja los literales en `@stringBase0` (con `inline` solo van a `@STRING@`) y se inlinea en una
  llamadora dentro de presupuesto mientras la que esta fuera la llama (Em316LastLaser fn_8031E078).
- Quitar una declaracion de una cabecera renumera los `@N` de todas las unidades que la incluyen.

