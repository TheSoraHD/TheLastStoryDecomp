# Siguiente sesion (tras codigo-20261120)

> Sesion `codigo-20261120` (secuencial, sin agentes) del 2026-11-20 13:58 al ~22:20. Notas: `docs/codigo-20261120.md`;
> para el port: `claude/port-codigo-20261120.md` (proyecto). Anterior: `mixto2-20261120` (`docs/mixto2-20261120.md`).

Estado: **16347 / 18113**, lost 0, rep100 6, labelcheck 10 lineas, `main.dol: OK`. Codigo **65,50 %**, datos 76,77 %,
**755** unidades completas (enlaza atn/D2A_Data).

## Decisiones pendientes del usuario

- Ninguna nueva. Las de mixto2 quedaron resueltas: qA3 de D2AMpMenu y el `goto` de Em303KrakenFootMp aprobados (regla 2), `SetParam2_unk` por valor integrada.

## Siguiente (decomp)

- Seguir la idea que mas rindio al final: valores de un solo uso como expresion (`x.GetNormalize().ToRotation_unk()`, `VecAssign_unk(m, Neg(dir.GetNormalize()))`) en funciones al 98-99,9 % con huecos de pila; a veces baja (el original tenia el local): probar y revertir.
- Relecturas de globales que faltan (`tools`: odiff2 con `DIFF_DELETE lwz rX, lbl_...@sda21`; lista de la sesion en las notas): Event/Manager fn_8047FCC0/fn_80482ED8, GameEventScript fn_803A11D8/fn_803A8470, ModelInstance fn_8009E6EC, MapTask vf0C, BattleTask fn_80106878, PostFxGodRay fn_800AEF68, PacketPack fn_804E4EB0/fn_804EB934.
- Gemelas de familia por similitud de opcodes (twins): EnemyQuarkStand/First/Second, Em202Spider*, Em203GreaterDaemon*, Em101*Zesya*.
- Casi cerradas con pocas lineas de diff (ver `docs/mwcc-blockers.md` seccion codigo-20261120 para lo ya probado): em005 vf3C (un `b` inalcanzable), DrawHelper fn_800602E8 (`mr` frente a `li 0`), WaitObject vf40 y GameEventScript fn_803A26F4 (una instruccion movida), PPDATAU fn_8050DC58, atn_colli fn_80059264.
- Lo de mixto2 sigue valido: Event/Section/Channel/Cut (delete del buffer), Event/Label `__sinit`, push_back de `vector<std::string>` (qV3) y push_back dentro de otra inline (qV2).

## Herramientas

- Integracion de colas: `tools/decomp/symmerge.sh X` (bloques en conflicto de symbols.txt con HEAD via `ourshunks.py` + `refmap2.py` con MAINREF = copia del agente, recompilando antes sus `.o`). `tools/decomp/dollarfix.py` para nombres `$N` tras renumerar.
- `tools/decomp/permrun.py <unidad> <simbolo> <funcion>`: prueba los ordenes de bloques cortos de asignaciones. `tools/decomp/stk.py <unidad> <simbolo>`: ranuras de pila nuestras frente al original.
- Re-emparejado: `MAINREF=<build de referencia> python3 tools/decomp/refmap2.py <unidades>` (por direccion). Una `static inline` nueva renumera los `@N` de la unidad; una inline de clase no plantilla en una cabecera consume 4 `@N` en cada unidad que la incluye.
- Agentes que piden permiso de Bash se quedan parados: pararlos y reanudarlos con un prompt de continuacion.

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
