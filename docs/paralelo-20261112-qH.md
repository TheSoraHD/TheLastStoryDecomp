# Cola H (enemigos) - lote paralelo 2026-11-12

Inicio: 2026-11-12 17:20 CET. Rama `agent-H`.

## Resultado por unidad

- `game/Em316LaserSphere`: al 0 % 9 -> 6 (`fn_8031958C` y sus copias weak `fn_80318F90`, `fn_80319998`, las tres al 100 %); 22 -> 25/40 al 100 %.

## Decisiones tomadas sin el usuario

- Em316LaserSphere `fn_8031958C`: el original lo llama todo fuera de linea (presupuesto R3); 300 asserts vacios
  (`SPHERE_ASSERT100_UNK`) en la inline `fn_80267B20` (copia weak de otra unidad, solo la usa esta funcion).
- Renumeracion `@N` en `symbols.txt` de la unidad (sdata2pair + relocpair + a mano); `lbl_80884DC8..DD4` pasan a `@N` locales
  (solo los usa esta unidad).

## Hallazgos

