# Siguiente sesion (tras cabeceras-20261114)

> Sesion `cabeceras-20261114` (10 h, hasta 4 agentes, colas A-N + segundas pasadas) cerrada el 2026-11-14 hacia las
> 23:30 (inicio 13:20). Notas: `docs/cabeceras-20261114.md` y `docs/cabeceras-20261114-q*.md`. Para el port:
> `claude/port-cabeceras-20261114.md` (proyecto).

Estado: **15424 -> 15450 / 18109**, lost 0, sin bajadas, rep100 138, labelcheck 19, `main.dol: OK`. Funciones al 0 %:
**199**. 34 DRAFT (0 en la ruta). Enlazan 143 unidades del juego y del motor (como antes) y, entre las librerias, HBMBase.

## Decisiones pendientes del usuario

- **Parche de `push_back`** (`docs/cabeceras-20261114-qA5.patch`, colas A2-A5): forma original de MSL; +5 al 100 %,
  +45 funciones de fuzzy (Event/Manager fn_80481B9C 58 -> 96 %, Sequence::fn_8054204C 10 -> 93 %), 8 bajadas de
  registros (<= 0,64). Si se acepta: aplicar, `pdbatch.sh` completo, `@enum$...ControlAppear_cpp`.
- **Cola M** (rama `cabeceras-20261114-m`): `Frustum_unk::mBounds` subobjeto, +5 al 100 %, bajan NewCampTask
  fn_8052E1A0 (-0,5) y fn_8052FA44 (-0,1). Arreglo real: `operator=` de `vector<atn::Light>`/`vector<atn::Camera>`.
- Split de la cola E ("Event::ActionProcessor" a Behavior_unk).

## Siguiente

- Sincronizacion del port con esta rama (lista de cambios en `claude/port-cabeceras-20261114.md`).
- Con el parche de push_back: las 26 copias debiles de Event/Sequence, EventParser_unk y Event/Manager;
  `PathLengths_unk` (= `vector<float>`) en WindyItem/PathData_unk/ActionActorPosition; `BattleTask::fn_800F8C6C`.
- Funciones C de BattleTask que aun no estan en `BattleTask.h` (tipos distintos por unidad: `fn_80108C10` con
  `BattleDamage_unk`, `fn_800FDB20`, `fn_800FB1BC`, `fn_80101524`, `fn_8010CF28`, `fn_80102EAC`, `BattleAim_unk`); pasar a
  miembros todas las funciones C de BattleTask necesita aprobacion (unidad del port).
- Firmas del port pendientes de aprobacion: `GameUI::fn_803E43B8/fn_803E4434` con segundo argumento (GameTask
  fn_80370320), `CharaTask::vf3C` con segundo argumento (BattleTask fn_800FDF14), `static Matrix3x4 fn_8013A13C(float)`
  (GameCamera fn_80382A1C).
- `ordercheck` de GameEventScript (`__dt__15GameEventScriptFv` fuera de orden, ya en `main`).
- Bloqueos nuevos en `docs/mwcc-blockers.md` (seccion cabeceras-20261114).
