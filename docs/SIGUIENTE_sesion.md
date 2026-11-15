# Siguiente sesion (tras enlazar-20261115)

> Sesion `enlazar-20261115` (10 h, hasta 4 agentes, 26 colas A-Z) del 2026-11-15, 13:07 -> ~23:15. Notas:
> `docs/enlazar-20261115.md` y `docs/enlazar-20261115-q*.md`. Para el port: `claude/port-enlazar-20261115.md` (proyecto).

Estado: **15631 / 18109**, lost 0, rep100 34, labelcheck 12 lineas (como antes), `main.dol: OK`. Datos enlazados
**1075250 / 1432078 B (75,08 %)**, 698 unidades completas (con KPAD).

## Decisiones pendientes del usuario

- (Hecho) KPAD enlaza con el parche de la cola O, aprobado por el usuario (no hay otro repo con licencia con el mismo
  cuerpo; Whippy tiene una version distinta). Datos enlazados 75,08 %, 698 unidades.
- Funciones inventadas que el enlazador quita (EventCommandDef_unk, EventLoader_unk, ControlCdnStone) y las
  especializaciones explicitas de D2A_TaskInstanceBase_64_8: revisar (ver "Decisiones tomadas sin el usuario").
- Subidas de fuzzy commiteadas sin 100 % (G: BreakArmor, Ottotto; Q: OpenBoard, LH_Handle, ShakeCamera).

## Siguiente

- `chr/ControlAppear`: dejar para el paso final de la decompilacion completa (decision del usuario; contador
  `@enum$24281control_appear_cpp`).
- Cerca de enlazar (una funcion o datos): chr/ControlDamage (parche `docs/enlazar-20261115-qZ.patch`: ctor 99,95 %,
  Knockdown vf0C 99,70 %, LastOne vf0C 99,64 %), GameSaveBanner `fn_803B9638` 99,18 %, OpenBoard vf10 99,76 %,
  LH_Handle 99,56 %, Event/ActionActorBehavior `fn_8054FBE0` 99,77 %, Event/MotionData `MovieData::vf08` 99,8 %,
  ControlStagger `ControlSlip::vf0C` 99,9 %, ControlShoot ctor 99,9 %, EnemySeaLizerd 99,96 %.
- Bloqueos comunes: `__sinit` de las tablas `ParamDef_unk` (Label, Channel, Cut, Sequence); `push_back` de
  `std/vector.h` (parche qA, Event/Section); `Loader::Get_unk<T>` (parche qC, 99,83 %); `strlen` intrinseca
  (global: 2 bajadas; por unidad con `std/msl_common.h` primero).
- Regla 10: quedan 22 casts a `D2A_TaskInstance32_unk` (ItemSlotAnime, parche qV; SelectMember); `lbl_8087F1E4`
  con varios tipos (MenuMsgTable_unk, MpCampMsgTable_unk).
- Tramos auto_ que quedan (49, 924 B): sus duenos no enlazan (GameTask, GameVoice, SoundManager, PopupTask...).
- Gecko_ExceptionPPC, ut_TextWriterBase y bte/bd siguen bloqueadas (2 bytes de mas en el original).
- Sincronizar el port con `claude/port-enlazar-20261115.md`.
