# Lote enemies-finish, cola q7 (Em403WarLion, EnemyJill)

Inicio: 2026-10-22 09:05 CEST. Rama agent-q7 (sale de main 64df7a6). Referencia: /home/claude/ref/start-q7.json.

## Unidades

- `game/Em403WarLion` (unidad SIN FUENTE hasta ahora, 0/67 al inicio): **fuente nueva
  `src/game/Em403WarLion.cpp` y cabecera nueva `include/game/Em403WarLion.h`** (subclase de EnemyTask, layout
  0x14B0-0x1610, nombres inventados). En curso (ver abajo el recuento final).

## Decisiones tomadas sin el usuario

- `config/SLSP01/symbols.txt` (Em403WarLion): renombrados ctor/dtor y las virtuales de la vtable
  (vf0C, vf10, vf14, vf20, vf24, vf3C, vf40, vf50, vf54, vf90, vf94, vfE4, vfEC), los metodos no virtuales
  `fn_XXXXXXXX__12Em403WarLionF...`, `fn_80323C18` -> `IsRaging_unk__12Em403WarLionFv` (copia debil de un
  getter inline), los invokers/managers de los dos `boost::bind` (0x803234C8-0x803235D8), sus RTTI
  (0x80788BC0/BC8), `@LOCAL@`/`@GUARD@` de `assign_to` (0x807C8438/40, 0x8087F400/01) y `lbl_80749D10` ->
  `@stringBase0` (scope:local).
- `game/SpecialEnemyBase.cpp`: la factoria crea el tipo 48 con `new (3, "", "", 0) Em403WarLion(parent, data)`
  (incluye game/Em403WarLion.h; se quita el `extern "C" fn_80322C68`). fn_805A5374 sigue igual (73.27%).
- Clases inventadas: `WarLionPath_unk` (0x50 bytes en 0x1524, mismo layout y reset que `gmk::PathData_unk`
  sin su ultima palabra), `WarLionPathList_unk` (derivada de std::vector), `WarLionRes_unk` (como ResHandle,
  pero el original limpia el puntero en el cuerpo del ctor), `AllyAi_unk` (datos de IA de un aliado devueltos
  por fn_80103F60, pesos por enemigo en 0x128).

## Hallazgos MWCC

- vf0C de Em403WarLion: un `if (result != 0) { ...; return 1; } return 0;` (sin retorno temprano) da el `beq`
  directo del original; el `Vector3(a, b, c)` asignado a un miembro necesita un local con nombre para que el
  temporal vaya al final de la pila.
- `float frame; if (a > 5 || (frame = m, frame >= f())) {...}` (operador coma) reproduce la carga del frame
  en f31 antes de la llamada sin `mfcr`.

## Pendientes para otras colas

- (ninguno por ahora)
