# `.rodata` sin asignar — q1

- Rama local: `codex/rodata-unassigned-2026-10-24`; sin push.
- Amplié cuatro rangos contiguos en `config/SLSP01/splits.txt`, según las referencias del ensamblado y los objetos `auto_`:
  - `game/CampSaveLoad`: `0x80736AA8–0x80736AB0`, double de conversión usado por dos funciones de la unidad.
  - `chr/ControlGmkCoffinOpening`: `0x8073AD70–0x8073AE54`, string de `boost::bind` referenciado por la unidad.
  - `chr/ControlJumpAt`: `0x8073AEE0–0x8073B110`, double y strings de `boost::bind` referenciados por la unidad.
  - `chr/ControlRide`: `0x8073B810–0x8073B8BC`, string de `boost::bind` referenciado por la unidad.
- Para evitar ciclos al asignar otros pools directamente a sus unidades de código, añadí tres unidades de datos sin fuente C++:
  - `game/AiTarget_rodata.cpp`: `0x8072FD8C–0x8072FF50` (452 bytes); `align:4` refleja el alineamiento real y elimina el nuevo aviso de dtk.
  - `atn/ComDrawTexture_rodata.cpp`: `0x80731140–0x80731160` (32 bytes).
  - `nw4r/SoundArchiveFamily_rodata.cpp`: `0x80762328–0x807626F8` (976 bytes). Agrupa RTTI/strings del bloque continuo compartido por `DvdSoundArchive` y `SoundArchive`, evitando el ciclo que aparece al repartir los fragmentos alternos entre ambos.
- No cambié fuentes ni amplié las excepciones de la regla 2. La cantidad de funciones al 100% no cambia.
- La asignación directa de `AiTarget` produce ciclo con `std/runtime_error`; la de `ComDrawTexture`, con `DrawHelper`; la partición de los pools nw4r por propietario, entre `DvdSoundArchive` y `SoundArchive`. Las unidades de datos separadas resuelven los tres casos sin mover los bytes originales.
- Validación final: `ninja -t clean` limpió 714 artefactos y `ninja` recompiló 716 pasos; `main.dol: OK`. `regress.py`: lost 0; `fuzzdrop.py`: 0 bajadas; `poolcheck.py` en las ocho unidades afectadas: 0 hallazgos; `git diff --check`: limpio.
- Decisión a revisar: confirmar que `nw4r/SoundArchiveFamily_rodata.cpp` es un nombre y agrupación adecuados para el bloque común de Dvd/SoundArchive. No se forzó `order:` ni se amplió ninguna excepción.
