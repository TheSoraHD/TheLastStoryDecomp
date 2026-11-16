# escribir-20261116 cola W2 (game, enemigos y gimmicks)

Inicio: 2026-11-16 23:58 CET (tope 2,5 h: 02:28).

## Tabla de la cola

| Funcion | Antes | Despues |
|---|---|---|
| Em206CocoonMp ctor (0x802BA92C) | 0 % + DRAFT | equivalente NONMATCHING 0 % (presupuesto de inline, bloqueo conocido) |
| Em206CocoonMp `ResHandle::ResHandle` (0x802BABC0) | 0 % (no emitida) | sin cambios: solo se emite si el ctor queda fuera de presupuesto |
| GimmickManager `LoadSeTableTask::vf0C` (0x803EEEDC) | 0 % | equivalente NONMATCHING 0 % (comprobada contra el asm; caso abierto de docs/inlining.md) |
| GimmickManager `fn_803EF3B0` (boost::function::operator()) | 0 % (no emitida) | sin cambios (se inlinea en vf0C) |
| LastCocoonChrysalis ctor (0x80452710) | 0 % | sin cambios (idea nueva probada: locales key/cmp/value) |
| Em302KrakenMp vf0C (0x802DE27C) | DRAFT 99,30 % | equivalente NONMATCHING (orden de IsLoading corregido; quedan registros) |
| Em302KrakenMp vf104 (0x802DF570) | DRAFT 100 % | 100 % (mnemcheck limpio, literales iguales) |
| EnemyTasha ctor (0x803559F8) | DRAFT 100 % | 100 % (mnemcheck limpio, literales iguales) |
| Em202ForestSpider ctor (0x8027278C) | DRAFT 98,40 % | equivalente NONMATCHING 99,6 % (faltaba `m14BC(0)`) |

## Para el port
- Em302KrakenMp::vf0C: el original comprueba `m15CC.IsLoading()` justo despues de `m152C` (antes de m1564/m1570/m157C); corregido.
- Em202ForestSpider ctor: el original pone `m14BC = 0` ("ReinforceCenter"); faltaba en el init list (miembro sin inicializar).

## Decisiones tomadas sin el usuario

## Hallazgos

## Bloqueos
- Em206CocoonMp ctor y LastCocoonChrysalis ctor: las locales anidadas key/cmp/value del bucle de tokens (lo que saco LastCocoonImago de presupuesto) no bastan; tampoco una local `rest` mas ni `file = path.c_str()` (escribir-20261116 qW2).

## Cambios de splits/symbols
- Ninguno.

## Scripts
- `/home/claude/scratch-W2/bin/fltcmp.py <unidad> <simbolo>`: valores de los literales (.sdata2/.rodata/.sdata) que carga una funcion, en orden, en el .o original y en el nuestro; para comprobar funciones al 100 % (objdiff no compara valores).
