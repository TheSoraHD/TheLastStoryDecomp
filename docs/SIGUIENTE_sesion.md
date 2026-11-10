# Siguiente sesion (tras arranque2-20261110)

Rama `arranque2-20261110` (sin mergear; inicio 2026-11-10 00:52, cierre 12:52): **14895 -> 14956 / 18109**, lost 0,
sin bajadas de fuzzy, build limpio y clon limpio de la rama con `main.dol: OK`. Notas: `docs/arranque2-20261110.md`;
ruta `main()` -> pantalla de titulo: `docs/ruta-arranque.md` (581/666 al 100 %, enlazan 9 de 27).

## Hecho en arranque2-20261110

- Ruta: 557 -> 581 / 666; enlazan `game/GameSystem_unk` y `atn/AudioDevice`. Ninguna funcion de la ruta sin
  escribir ni borrador.
- Fuera de la ruta enlazan `atn/Camera_unk`, `chr/ControlUseSkill`, `game/Em209BigGolem`, `gmk/LottaSummon`,
  `gmk/Fireworks`.
- **Cruce r3/r4 de `__VecCopy` resuelto** (bloqueo 3 de `docs/mwcc-blockers.md`): la copia a traves de
  `static inline void XAssign_unk(atn::Vector3& dst, const atn::Vector3& src) { dst = src; }`.
  `tools/decomp/asgwrap.py <unidad> <simbolo>` prueba cada asignacion (`--declonly`: `Vector3 v = expr;`; `--ops`:
  `x++`, `x op= y`); `--only=i --apply --name N --type atn::Vector3` lo deja escrito y quita el `NONMATCHING`.
- Otros niveles de inline: `Wrap26A4_unk` (Task), `ClampU16_unk` (RenderSystem), cadena como parametro de una inline
  (GameConfig), `FwDec_unk` (Fireworks), resultado de llamada como argumento (DialogConfig).

## Pendiente

- Pasar `asgwrap.py` (modo normal y `--declonly`) a las funciones al 97-99,9 % con diferencias en `psq_` que no
  llegaron a probarse (unas 45 de `scratch`: regenerar con un barrido de `odiff2.py | grep psq_` sobre las
  funciones al 97-99,9 % de `report.json`). El modo `--ops` y las funciones sin `psq_` no sacaron nada.
- Mejoras parciales de `asgwrap` sin commitear (BattleCharaTask fn_800E3F5C 99,96; Em003-like casos en
  ChaserGuard fn_8017F4A0, CharaTask vfD8, Em101ElderZesya fn_802598DC, Em315Coffin fn_80315654): falta lo otro.
- Ruta: bloqueos de registros (lista en "Que falta" de `docs/ruta-arranque.md`); `Loader::Get_unk<T>` cerraria los
  ocho `atn/LH_*` de golpe (99,7 %).
- De sesiones anteriores: cabecera unica de `IRenderLayer`; `fn_800F8C6C` (orden de parametros); `ordercheck` de
  atn/SmallAllocator_8; deuda de la regla 10 en PacketPack; ReplayBuffer; MapPartsGimmickLoc; ShopUpGrade.

## Decisiones a revisar

- Seccion "Decisiones tomadas sin el usuario" de `docs/arranque2-20261110.md` (inlines de copia por unidad en lugar
  de una cabecera comun; prioridad fuera de la ruta desde las 08:15).
- Las de `docs/arranque-20261109.md` siguen abiertas.
