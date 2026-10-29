# Lote `batch-auto-20kb-game-b` (propuesta E, segunda mitad)

Rama creada desde `origin/batch-auto-20kb-game` en `acb3f6d8` el 2026-10-29. Informe de referencia guardado en `build/SLSP01/report-batch-auto-20kb-game-b-baseline.json`; build inicial de la rama: `main.dol: OK`.

## Alcance

Lista de `targets.py --max-unit 20480 --limit 1000`, filtrada a más de 10240 bytes, primera función posterior a `game/FinishCamera_unk` (0x80378474), sin fuente y sin unidades `atn/` o de enemigos. Se excluyó `game/QuadrupedTask` porque su RTTI deriva de `EnemyTask`.

| Unidad | Inicio | Bytes | Funciones | Estado |
|---|---:|---:|---:|---|
| `game/GameChoiceWindow` | 0x803B35FC | 17224 | 38 | en curso |
| `game/GameVoice` | 0x803E836C | 14552 | 29 | pendiente |
| `gmk/FragileObject` | 0x803F1BB4 | 11112 | 26 | pendiente |
| `RecycleBuffer_gmk/ItemSlotAnime` | 0x8044D710 | 20480 | 30 | pendiente |
| `game/MapPartsBase` | 0x80491EA0 | 17792 | 30 | en curso |
| `game/MapPartsGimmickLoc` | 0x80496EE8 | 12592 | 46 | pendiente |
| `game/MapTask` | 0x8049D264 | 19320 | 27 | pendiente |
| `game/MiniMapTask` | 0x804A7EA4 | 17804 | 52 | pendiente |
| `game/NewCampPartsBase` | 0x8050F93C | 12164 | 41 | pendiente |
| `game/NewCampPartsItem` | 0x805169F8 | 20264 | 41 | pendiente |
| `game/NewCampPartsOption` | 0x80521E1C | 19116 | 30 | pendiente |
| `Event/Node` | 0x80537708 | 11732 | 29 | pendiente |
| `Event/Sequence_cont` | 0x80543108 | 12024 | 35 | pendiente |
| `Event/ActionActorBehavior` | 0x8054E574 | 11156 | 14 | en curso |
| `Event/ActionCameraMode` | 0x8055A49C | 14168 | 20 | pendiente |
| `chr/CharaEquip_unk` | 0x805634D0 | 12112 | 26 | pendiente |
| `game/TutorialTask` | 0x805B9EC8 | 13808 | 31 | pendiente |

## Avance

- `game/GameChoiceWindow`: `fn_803B3AA0` (callback que inicializa los campos de `GameMessage`), `fn_803B4C24` (consulta si el estado en `+0x78` es 3), `fn_803B57B0` (transición de estado y actualización del task asociado), `fn_803B57EC` (completa estados 2/4), `fn_803B5830` (guarda dos parámetros y delega), `fn_803B5950` (reproduce sonido solo en estado 2) y `fn_803B78B4` (inserta en una cola circular de 64 valores) al 100%. `fn_803B78E8` queda en 98,48% por asignación de registros del puntero al administrador de sonido. El resto de la unidad sigue pendiente.
- `Event/ActionActorBehavior`: trece funciones emparejadas y en orden; una ausente (`fn_8054E574`, factoría). `fn_8054FAD4` y `fn_805501DC` al 100%; `fn_8054F380` 17,26% (libera 49 acciones estáticas y limpia la tabla de dispatch), `fn_8054F9DC` 89,19% (diferencias en el plegado de flags y ramas), `fn_8054FB10` 95,65% (MWCC reutiliza cargas de cero entre escrituras), `fn_8054FBC8` 81,67% (forma record de MWCC difiere), `fn_8054FBE0` 28,33% (selección de acción y temporización; falta reconstruir el borrado de lista intrusiva), `fn_8054FFE0` 59,98% (transición de cámara/evento reconstruida), `fn_80550254` 62,84% (dispatcher principal: gating, ventanas de acciones, cambios de corte y timeout; dispatch de estados y guardas de diálogo aún difieren), `fn_80550A78` 52,41% (recorrido de tabla de acciones con vista local inferida), `fn_80550CE0` 35,87% (dispatch temporal de acciones y rutas de corte); `fn_805510F8` y `fn_80551100` al 100%.
- `game/MapPartsBase`: `ordercheck` actual: 29 emparejadas, 1 ausente (`__dt__12MapPartsBaseFv`, destructor inline que no se emite en este objeto), 0 extra, 0 fuera de orden. Constructor base, helpers `vfXX`, helpers CSV/lista y constructor `fn_80494964` están presentes; `fn_80494964` ahora llega al 100% con una vista local inferida de `MapPartsAreaInit_unk` y `NewCampScene2_unk`. Los cuatro `push_back` siguen parciales (92,69–96,56%) por diferencias en las rutas rápidas/de crecimiento de `std::vector`. `fn_80491EA0`, `fn_80491F94`, `fn_8049248C`, `fn_80494354`, `fn_804962B0`, destructor `MapPartsArea` y virtuales de `MapPartsArea` están presentes como stubs `NONMATCHING`; no contar la unidad como terminada. Hacer el destructor base out-of-line provoca seis bajadas fuzzy de destructores derivados, así que se conserva inline.
- `game/MapPartsBase::fn_80494D78`: implementación de `std::vector<void*>::push_back`; se deja `NONMATCHING` porque MWCC cambia el orden de registros en la ruta rápida (el mismo bloqueo está documentado en `include/std/vector.h`).
- `ordercheck` de `game/MapPartsBase`: 29 emparejadas, 1 destructor inline ausente, 0 extra y 0 fuera de orden. `Event/ActionActorBehavior`: 13 emparejadas, 1 factoría ausente, 0 extra y 0 fuera de orden.

## Restricciones de integración

No editar las cabeceras compartidas ni los ficheros `SongManager.cpp`, `GameBgm.cpp`, `ChaseManager.cpp` o `EnvSe*` enumerados en la solicitud. Si una implementación necesita esas declaraciones, usar una vista local `_unk` y documentarla aquí. En `symbols.txt`, limitar los cambios a renombrados de este rango de direcciones. No actualizar la sección `Estado` de `CLAUDE.md` ni `docs/historial.md`.

