# Siguiente sesion (tras grandes-20261120)

> Sesion `grandes-20261120` (secuencial, sin agentes) del 2026-11-20 23:59 al 2026-11-21 ~10:45 (cierre desde las 09:58). Notas: `docs/grandes-20261120.md`;
> para el port: `claude/port-grandes-20261120.md` (proyecto). Anterior: `codigo-20261120` (`docs/codigo-20261120.md`).

Estado: **16366 / 18113**, lost 0, rep100 6, labelcheck 10 lineas, `main.dol: OK`. Codigo **65,94 %**, datos 76,77 %,
**755** unidades completas.

## Decisiones pendientes del usuario

- Ninguna nueva. Usadas las aprobadas del prompt: funciones inventadas que el enlazador quita (SequenceHolderAddUnit_unk, NewCampAddElem_unk) y bajadas de fuzzy por bytes (lista en las notas).

## Siguiente (decomp)

- **Barrido automatico primero**: `tools/decomp/mutsweep.py <unidad> <simbolo> <Clase::metodo>` sin `--only` (intercambio de sentencias independientes, local de un solo uso como expresion, ternarios, `fswap`, `cswap`) cerro o desatasco 5 funciones y subio otras 0,5-1,3 puntos. Las listas barridas estaban en el scratch de la sesion (no persisten): regenerar con `bigcands.py`; barridas las de 97-99,7 % de 500-4000 B y las de 99-99,6 %. Quedan sin barrer las de 90-97 %.
- **Familias**: la de "mover hacia la rotacion" quedo cerrada salvo LastCocoonQuadruped fn_80462038 (99,63, `li` de keepRot) y CharaTask fn_8013CB68 (14,7 KB, 98,62 con los mismos retoques: un GPR guardado mas). Buscar otras familias con `tools/decomp/twins.py`.
- Casi cerradas (ver `docs/mwcc-blockers.md` seccion grandes-20261120): GameTask fn_803729B4 (99,99: solo el nombre de la copia debil `size()` 0x800E0AA8, ver la nota), CharaStatus fn_8012F440 (15,5 KB, 99,85), ChaserGuard fn_8017D13C (99,99), D2AMpMenu fn_804B1528, DamageCalc fn_8003F538/fn_8003EA3C, EnemyQuarkSecond fn_8033D750.
- Al probar variantes que anaden inlines, el porcentaje baja por los `@N` renumerados: contar `odiff2.py | grep -v @` (`tools/decomp/varnl.sh`).

## Herramientas nuevas

- `tools/decomp/bigcands.py`, `unitcmp.py`, `odctx.py`, `twins.py`, `mutsweep.py` (kinds `fswap`, `cswap`, `splitor`, `nest`...), `hoistsweep.py`, `constsweep.py`, `constsweep2.py`, `getsweep.py`, `symonly.py`.
- Re-emparejado: `MAINREF=<build de referencia> python3 tools/decomp/refmap2.py <unidades>` tras compilar la unidad (si no, renombra 0) y antes de `precommit.sh`.

## Candidatas por datos (report.json de mixto2-20261120; sin cambios en codigo-20261120)

Datos totales 1432078 B; enlazados 1099358 (76.77 %); 1 % = 14320 B.

## A: 0-3 funciones (>= 300 B)

| Unidad | Datos | Faltan | % de las que faltan | B/func | Z |
|---|---|---|---|---|---|
| game/GameConfig_unk | 2944 | 1/15 | 98.3 | 2944 |  |
| chr/ControlChaseCaught | 2456 | 1/39 | 100.0 | 2456 |  |
| atn/ShaderManager_unk | 2192 | 1/4 | 99.1 | 2192 |  |
| atn/RenderSetting_unk | 2168 | 1/15 | 97.1 | 2168 |  |
| atn/D2A_Manager | 6076 | 3/21 | 97.5, 98.6, 99.2 | 2025 |  |
| game/Em204MulueMp | 1596 | 1/31 | 98.7 | 1596 |  |
| gmk/PassBoard | 1552 | 1/13 | 98.7 | 1552 |  |
| game/Em309GhoulSimple | 1448 | 1/22 | 99.6 | 1448 |  |
| RVL_SDK/src/revolution/net/NETAES | 2800 | 2/9 | 61.3, 62.4 | 1400 |  |
| chr/ControlAppear | 1360 | 1/24 | 98.8 | 1360 |  |
| dwc/src/base/dwc_transport | 1256 | 1/24 | 99.8 | 1256 |  |
| atn/SoundManager | 1228 | 1/76 | 99.5 | 1228 |  |
| atn/Util_unk | 1176 | 1/16 | 92.7 | 1176 |  |
| game/CosEditProtector | 3452 | 3/25 | 97.1, 97.4, 99.2 | 1150 |  |
| chr/ControlDive | 1096 | 1/25 | 85.0 | 1096 |  |
| Event/Section | 1076 | 1/6 | 100.0 | 1076 |  |
| Event/ActionActorBehavior | 976 | 1/14 | 99.8 | 976 | 98 |
| PowerPC_EABI_Support/src/Runtime/Gecko_ExceptionPPC | 896 | 0/14 |  | 896 |  |
| game/Em209SmallGolem | 876 | 1/22 | 99.7 | 876 |  |
| chr/ControlCannonLift | 844 | 1/17 | 99.3 | 844 |  |
| game/NewCampPartsTop | 816 | 1/12 | 98.9 | 816 |  |
| game/Em101LittleZesyaSecond | 784 | 1/15 | 98.2 | 784 |  |
| chr/ControlDamage | 2280 | 3/41 | 99.4, 99.9, 99.9 | 760 |  |
| chr/ControlInteractionBase | 1464 | 2/21 | 97.6, 97.7 | 732 |  |
| game/NewCampPartsStatus | 1392 | 2/29 | 98.4, 99.7 | 696 |  |
| game/ShopBase | 1360 | 2/48 | 95.3, 97.1 | 680 |  |
| game/Em204Mulue | 1352 | 2/39 | 99.3, 99.6 | 676 |  |
| game/Em303KrakenFootMp | 632 | 1/21 | 99.8 | 632 |  |
