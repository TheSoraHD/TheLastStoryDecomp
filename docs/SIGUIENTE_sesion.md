# Siguiente sesion (tras s12h-20261109)

Rama `s12h-20261109` (desde `main` f8995cf, **sin mergear**): **14596 -> 14837 / 18109**, lost 0, la unica
bajada de fuzzy es el stub conocido de `MiniMapTask::vf10` (0,33 -> 0; ahora es borrador con +10 copias).
Clon limpio de la rama: `main.dol: OK`. Notas: `docs/s12h-20261109.md`.

## Pendiente

- Revisar y mergear la rama (decisiones abajo).
- game/ReplayBuffer (14/28): quedan fn_8056A9E4 (borrador), fn_8056B558, fn_8056C3D8, fn_8056CB20, fn_8056F5B4
  (extraccion de angulos con atan2 y PSMTX44MultVec), fn_8056DACC, fn_8056E918, fn_8056FC24 (como fn_8056BD34:
  falta 0x20 de pila). Las funciones de 0x80570A14-0x80570A74 son de ReplayBuffer pero estan en el split de
  game/GameSystem_unk (frontera de split a revisar).
- Bloqueos de registros sin resolver: buscador fn_804DD158 de PacketPack (~10 funciones), `Alloc_unk` de
  ItemSlotAnime (5), `SetNext_unk` de GameEventScript (62 comandos).
- MapPartsGimmickLoc: 6 copias (operator[], Resize_unk, ctor/dtor del registro) saldran con el dtor
  (0x804970D4; hace falta `SwingPart_unk` de gmk/SwingObject.cpp en cabecera) o el borrador de fn_804985E4.
- Unidades sin fuente: ShopUpGrade (57 funciones, deriva de ShopBase), NewTitleTask (8 grandes restantes).

## Decisiones a revisar

- `__declspec(noinline) inline` en accesores de borradores de PacketPack; `__declspec(noinline)` en los
  borradores fn_8056A9E4 y fn_8056D60C de ReplayBuffer.
- Nombres del RTTI: `ReplayBuffer` (antes `GameTaskData_unk`) y `atn::EffectManagerCallback` (antes
  `EffectListener_unk`); `EffectEvent_unk` movido a `include/atn/EffectManagerCallback.h`.
- Macros `REPLAY_MIN`/`REPLAY_MAX` en game/ReplayBuffer.cpp.
