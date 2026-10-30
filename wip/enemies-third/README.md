# Trabajo a medias de enemies-third (no se compila)

Lo avanzado de las colas que se cerraron antes de escribir fuente. Nada de esto está en `src/` ni `include/`: al retomar una unidad, mover la cabecera a `include/game/`, crear `src/game/<Unidad>.cpp` y borrar su carpeta de aquí. Los volcados de asm no se guardan (salen del `.dol` original): regenerarlos con `python3 tools/decomp/fasm.py game/<Unidad> <addr>`.

- `Em202ForestSpider/`: superado; la unidad está en `src/game/Em202ForestSpider.cpp` y `src/game/Em202MothersSpider.cpp` (cabeceras en `include/game/`). Quedan los borradores por si sirven para las funciones NONMATCHING.
- `Em203GreaterDaemonRedMp/` (cola n3b): `Em203GreaterDaemonRedMp.h` (layout 0x14D4-0x16CC), `mp_head.cpp` (inicio del fuente) y `borradores/` (intentos de funciones; varios son de `Em203GreaterDaemonRed`, ya en `src/`). Resumen para retomarla: `docs/enemies-third-n3b.md`.
- `Em206CocoonMp/` (cola n5): `cocoon_decode.txt`, análisis del asm (layout 0x1938, vtable, mapa de estados de la tabla de saltos, tablas de `.rodata`, helpers inline, orden de `.sdata2`). Em206EggMonster no tiene nada.
