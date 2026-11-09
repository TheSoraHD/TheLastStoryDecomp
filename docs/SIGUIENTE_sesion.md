# Siguiente sesion (tras arranque-20261109)

Rama `arranque-20261109` (sin mergear; inicio 2026-11-09 12:41, cierre ~00:41): **14837 -> 14893 / 18109**, lost 0,
sin bajadas de fuzzy, build limpio y clon limpio de la rama con `main.dol: OK`. Notas: `docs/arranque-20261109.md`;
la ruta `main()` -> pantalla de titulo, con tablas por unidad, llamadas al SDK/NW4R y asm aprobado:
`docs/ruta-arranque.md`.

## Hecho en arranque-20261109

- Ruta de arranque (27 unidades): 523 -> 557 / 666 funciones al 100 %, enlazan 7 de 27 (nuevas: `game/BootTask`,
  separada de `game/Bullet`, y `atn/MindowsCommon`; frontera `ReplayBuffer`/`GameSystem_unk` movida). 0 borradores en la ruta;
  `game/NewTitleTask` escrita entera.
- Borradores: 56 -> 42 (los doce lectores de `Event/EventParser_unk`; `Data48_unk` es `Event::DummyData`).
- boost::lambda 1.35.0 vendorizado (`atn::SequenceHolder`); `CharaTask::fn_80170A20(target, radius, arg)`.
- Herramientas: `drafts.py`, `ruta_arranque.py`, `offscan.py`, `blorder.py`, `lblmap.py`, `declperm.py`.
- Trucos nuevos en `docs/trucos-mwcc.md` (orden de declaracion = orden de registros, inlines que cortan el CSE,
  `result` bool, `s *= ...; x += s`, orden de parametros deducido del orden de evaluacion).

## Pendiente

- Enlazar la ruta: las 21 unidades que no enlazan tienen 1-6 funciones al 97-99,9 % por registros (lista en
  "Que falta" de `docs/ruta-arranque.md`): GameSystem fn_80570CF8 (99,93), Pad fn_800A58D0/58F8 (97), NandManager
  vf10 (99,85) y fn_800A4244 (98,0), SoundManager fn_800CB800 x3 (99,90), StrapTask vf14 (85,9), main fn_8047A4C8
  (76, constructores de boost::function fuera de linea), SequenceHolder fn_800C3E84 (98).
- Una cabecera unica para las capas `IRenderLayer` (aplazado: renombra vf10 en 6 unidades).
- `fn_800F8C6C` (124 llamadas): probablemente el mismo orden de parametros que fn_80170A20 (Em308ParmCrab vf10,
  Em401 fn_8031FE4C).
- Fallo de `ordercheck` anterior en atn/SmallAllocator_8 (`__ct__MemoryManager`).
- De sesiones anteriores, sin tocar: deuda de la regla 10 en PacketPack; ReplayBuffer; MapPartsGimmickLoc;
  ShopUpGrade sin fuente.

## Decisiones a revisar

- Seccion "Decisiones tomadas sin el usuario" de `docs/arranque-20261109.md` (boost::lambda, orden de parametros de
  fn_80170A20, DummyData, constructores de Event, asignacion muerta en Input_unk fn_80122330, nombres inventados).
