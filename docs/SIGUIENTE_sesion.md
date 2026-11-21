# Siguiente sesion (tras barrido-20261121)

> Sesion `barrido-20261121` (secuencial, sin agentes; barrido en segundo plano en dos worktrees) del 2026-11-21 12:22 al 2026-11-22 ~01:00 (cierre desde las 00:23). Notas: `docs/barrido-20261121.md`. Sin cambios de logica ni firmas (no hay doc de port). Anterior: `grandes-20261120`.

Estado: **16370 / 18113**, lost 0, rep100 6, labelcheck 10 lineas, `main.dol: OK`. Codigo **65,99 %**, datos 76,77 %, **755** unidades completas, 387 avisos del compilador.

## Decisiones pendientes del usuario

- Ninguna nueva. El renombrado de la copia debil `size()` 0x800E0AA8 (GameTask fn_803729B4) se probo y se revirtio: es la copia que dejo el enlazador al fusionar `size()` de vectores distintos; ponerle el nombre de `vector<EnemyTask*>` baja 12 funciones en 10 unidades. Solo se cerraria con casts entre vectores en esas unidades (decision del usuario si interesa).

## Siguiente (decomp)

- El barrido ya paso por todas las funciones del juego de >= 200 B por debajo de 100 % (al menos con los kinds nuevos); rindio 4 cierres en 12 h. Lo que mas rindio: barrido + un retoque a mano mirando el asm (VecAssign_unk, setter por valor, valor de verdad) y `--plateau` en funciones que el barrido ya habia mejorado.
- Casi cerradas (un registro o un `cmpw`, ver `docs/mwcc-blockers.md` seccion barrido-20261121): Em204Mulue fn_802A8BBC (99,974 con `float ry = rot.y;`), Em203GreaterDaemonRed fn_80296DB8 (99,92 con VecAssign_unk), LastCocoonQuadruped fn_80468534 (99,92), CosEditBase fn_801CDE14 (99,80), Em301LShade fn_802D57B8 (99,72, parche de una inline para la rama del efecto).
- Bloqueos que se repiten en lo que queda: huecos de pila de temporales y numeracion de registros guardados (uno de mas o de menos); la direccion base de un miembro en registro (`m4D8`/`m97C` del modo de camara: inline miembro).
- Lista de candidatas grandes: `claude/grandes-candidatas-20261122.md` (proyecto).

## Herramientas nuevas

- `tools/decomp/mutsweep.py`: kinds mulsplit, divsplit, idx, ifid, ld, swret, cref, cassign, vassign, dtop, byval; `--plateau K` (sigue desde una variante con el mismo porcentaje), `--diff F`, `--list`. Busca tambien `Metodo` sin el espacio de nombres.
- `tools/decomp/rotld.py [regex]`: en todas las funciones < 100 %, el angulo de `PSMTXRotRad(m, 'y', a.b.y)` en un `float` local (o el patron que se pase).

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
