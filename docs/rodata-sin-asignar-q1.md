# `.rodata` sin asignar — q1

- Rama local: `codex/rodata-unassigned-2026-10-24`; sin push.
- Amplié cuatro rangos contiguos en `config/SLSP01/splits.txt`, según las referencias del ensamblado y los objetos `auto_`:
  - `game/CampSaveLoad`: `0x80736AA8–0x80736AB0`, double de conversión usado por dos funciones de la unidad.
  - `chr/ControlGmkCoffinOpening`: `0x8073AD70–0x8073AE54`, string de `boost::bind` referenciado por la unidad.
  - `chr/ControlJumpAt`: `0x8073AEE0–0x8073B110`, double y strings de `boost::bind` referenciados por la unidad.
  - `chr/ControlRide`: `0x8073B810–0x8073B8BC`, string de `boost::bind` referenciado por la unidad.
- No cambié fuentes ni amplié las excepciones de la regla 2. La cantidad de funciones al 100% no cambia.
- Candidatos revisados y dejados auto tras probar rangos adicionales: `game/AiTarget` (`0x8072FD8C–0x8072FF50`) causa un ciclo `game/AiTarget.cpp -> std/runtime_error.cpp`; el fragmento de `atn/ComDrawTexture` (`0x80731140–0x80731160`) causa `atn/DrawHelper.cpp -> atn/ComDrawTexture.cpp`. Los pools intercalados de `nw4r/DvdSoundArchive`/`nw4r/SoundArchive` causan `nw4r/DvdSoundArchive.cpp -> nw4r/SoundArchive.cpp`. Se retiraron los tres intentos; los datos permanecen en `auto_`.
- Validación: `ninja -t clean` quitó 714 artefactos, pero informó que no pudo retirar el directorio no vacío `build/compilers`. Los objetos se recompilaron desde cero usando los compiladores locales; build final `main.dol: OK`. `regress.py`: lost 0; `fuzzdrop.py`: 0 bajadas; `poolcheck.py` en las cuatro unidades tocadas: 0 hallazgos; `git diff --check`: limpio.
- Decisión a revisar: si conviene probar unidades de datos independientes para los tres casos cíclicos; la asignación directa a los ficheros propietarios no funciona con el orden actual del enlazador. No se intentó forzar `order:` ni reordenar objetos.
