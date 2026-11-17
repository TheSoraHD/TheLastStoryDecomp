# Siguiente sesion (tras escribir-20261116)

> Sesion `escribir-20261116` (12 h, hasta 4 agentes, 22 colas) del 2026-11-16 23:26 al 2026-11-17 ~11:45. Notas:
> `docs/escribir-20261116.md` y `docs/escribir-20261116-q*.md`. Para el port: `claude/port-escribir-20261116.md` (proyecto).

Estado: **15974 / 18109**, lost 0, rep100 24, labelcheck 11 lineas, `main.dol: OK`. Codigo 61,12 %, datos 75,95 %,
732 unidades completas. DRAFT 0. Funciones al 0 %: 45 (20 sin iostream/auto_03; PacketPack 1). PacketPack 307/380 al 100 %.

## Decisiones pendientes del usuario

- "Para revisar" de `docs/escribir-20261116.md`: bajada aceptada de fn_804EE040 (99,88 -> 99,21, argumentos reales),
  macros min/max de ReplayBuffer y WPad_unk, `(player->mFlagWord >> 28) & 1` en PacketPack fn_804EB26C, funciones inventadas
  sin llamadores (`LayerOcclusionPool_unk`, `SwingUpdate_unk`, `DataList_unk<Path>::Add_unk`), instanciacion explicita del
  ctor de subcadena en Event/Sequence, cambios de comportamiento (GameScene ctor, ShopBuySell sufijo, ForestSpider vfFC).
- Parches sin integrar: `docs/escribir-20261116-qN6.patch` (`Traits::length` en `std/string.h`, +2, re-emparejar ~26
  unidades sin `datafix.py` en CharaTask), `-qW4.patch` (vf0C de SmallAllocator 87 -> 96 %), `-qN7.patch`, `-qW3.patch`.
- `atn::Abs_unk` de dos niveles (`float` que llama a `double`) en `include/math/Vec.h`: sube varias (N4, N7) pero renumera
  casi todo.

## Siguiente

- Los 20 al 0 % restantes son bloqueos (presupuesto de inline, asm/GQR6, iostream): solo con ideas nuevas.
- Cerca de enlazar: SwingObject 17/20, RotaryRoom 15/18, PassBoard 12/13, RandomMerchant 21/23, Em315Coffin (ctor, `.sdata`
  de 0.0f), EnemyTasha (5), FileList (push_back), las de `tabla.py` con `faltan` 1-2.
- PacketPack: relectura del tamano de `mPlayers` en bucles (G6), fn_804DE884 99,99, fn_804EE040; helpers copiados
  `HitCenter_unk`/`MakeRotZYX_unk`/`HitEvent_unk` a una cabecera comun con BattleTask/DamageCalc_unk.
- Trucos que mas rindieron (grep `escribir-20261116` en `docs/trucos-mwcc.md`): orden de declaracion de los locales para los
  registros volatiles, modelo de pila (locales con nombre, temporales, cada nivel de inline invierte), bools en `||` escritos
  en el `if`, dtor no inline tras el cargador, tamano de `@stringBase0` (`strend.py`), una forma cambiada en todas las
  funciones de una unidad a la vez (G5).
