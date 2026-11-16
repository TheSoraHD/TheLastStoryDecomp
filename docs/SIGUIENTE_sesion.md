# Siguiente sesion (tras cierre99-20261115)

> Sesion `cierre99-20261115` (10 h, hasta 4 agentes, 19 colas) del 2026-11-15 23:24 al 2026-11-16 ~09:45. Notas:
> `docs/cierre99-20261115.md` y `docs/cierre99-20261115-q*.md`. Para el port: `claude/port-cierre99-20261115.md` (proyecto).

Estado: **15732 / 18109**, lost 0, rep100 29, labelcheck 11 lineas, `main.dol: OK`. Codigo `matched_code_percent`
58,37 %, datos enlazados **1078186 / 1432078 B (75,29 %)**, 707 unidades completas. PacketPack 0 % 54, 100 % 250/380.

## Decisiones pendientes del usuario

- Revisar "Para revisar" de `docs/cierre99-20261115.md`: literales float corregidos (cambian el comportamiento), funciones
  inventadas que el enlazador quita (`MapManager::AddArea_unk`), commits que solo suben fuzzy a una funcion de enlazar.
- Parches sin aplicar (solo suben fuzzy): qI-controldamage (Knockdown 99,94), qJ (push_back de shared_ptr, Section
  99,58), qE-bloom, qE2-lheffect, qL-lightutil.

## Siguiente

- Unidades a una funcion de enlazar (tabla `tabla.py`): GameBgm vf10 99,94, Brazier fn_80407D24 99,83, GmkCoffinOpening
  (RotZ_unk), GmkBreakArmor ctor 99,95, CostumeEditChara vf14, Leaf vf10, Em309GhoulSimple, Em315Coffin (ctor: 0.0f en
  `.sdata`, necesitaria `explicit_zero_data`), ControlDamage (qI).
- PacketPack: 54 al 0 %; dos `""` sin referencia en `@stringBase0` bloquean 7 funciones por offset; vf10 sin escribir
  (arrastra fn_804FB674 y las pequenas que solo llama ella); `vector<PPPlayer_unk>` y los `sort`.
- Bloqueos que no conviene repetir sin una idea nueva: `Loader::Get_unk<T>` (~110 variantes; 7 unidades LH_*), `__sinit`
  de ParamDef_unk, el 0 copiado de otro registro (ParseUtil_unk, ActionActorBehavior), pares texto/layout de menus MP.
- Las ~740 funciones al 99 % que quedan son casi todas de reparto de registros puro: los barridos automaticos dan
  0-1 aciertos por cada 30-40 funciones. Rinden mas las colas que sustituyen vistas/lecturas a mano por el tipo o la
  inline reales (ver `grep cierre99-20261115 docs/trucos-mwcc.md`).
- Sincronizar el port con `claude/port-cierre99-20261115.md`.
