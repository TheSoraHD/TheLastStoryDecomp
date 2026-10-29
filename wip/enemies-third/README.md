# Trabajo a medias de enemies-third (no se compila)

Lo avanzado de las colas que se cerraron antes de escribir fuente. Nada de esto está en `src/` ni `include/`: al retomar una unidad, mover la cabecera a `include/game/`, crear `src/game/<Unidad>.cpp` y borrar su carpeta de aquí. Los volcados de asm no se guardan (salen del `.dol` original): regenerarlos con `python3 tools/decomp/fasm.py game/<Unidad> <addr>`.

- `Em202ForestSpider/` (cola n2): `Em202ForestSpider.h` con el layout deducido del ctor, dtor y `vf0C` (hilos `SpiderThread_unk` de 0xEC en 0x1538, listas 0x14B0/0x14C0/0x1908, campos CSV 0x1504-0x152C); `borrador_v*.txt`, fragmentos de C++ (búsqueda de punto de ruta, etc.). Pendientes y no virtuales que usa MothersSpider: `docs/enemies-third-n2.md`.
- `Em203GreaterDaemonRedMp/` (cola n3b): `Em203GreaterDaemonRedMp.h` (layout 0x14D4-0x16CC), `mp_head.cpp` (inicio del fuente) y `borradores/` (intentos de funciones; varios son de `Em203GreaterDaemonRed`, ya en `src/`). Resumen para retomarla: `docs/enemies-third-n3b.md`.
- `Em206CocoonMp/` (cola n5): `cocoon_decode.txt`, análisis del asm (layout 0x1938, vtable, mapa de estados de la tabla de saltos, tablas de `.rodata`, helpers inline, orden de `.sdata2`). Em206EggMonster no tiene nada.
