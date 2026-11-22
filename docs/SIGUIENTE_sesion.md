# Siguiente sesion (tras bloqueos-20261122)

> Sesion `bloqueos-20261122` (secuencial, sin agentes; barridos de fondo en dos worktrees) del 2026-11-22 01:31 al ~12:20 (cierre desde las 11:31). Notas: `docs/bloqueos-20261122.md`. Port: `claude/port-bloqueos-20261122.md` (proyecto). Anterior: `barrido-20261121`.

Estado: **16400 / 18113**, lost 0, rep100 y labelcheck iguales a la referencia, `main.dol: OK`. Codigo **66,40 %**, datos 76,94 %, 387 avisos del compilador, **756** unidades completas (chr/ControlChaseCaught enlaza).

## Decisiones pendientes del usuario

- Identidad `Val_unk` (`template <class T> inline T Val_unk(const T& v) { return v; }`): solo se usa como sonda de los barridos y cada acierto se reescribe como accesor con nombre. Em403WarLion fn_80329ED8 solo llega al 100 % con `Val_unk(c)->m560 == 4` (ningun accesor con nombre lo da): se deja en 99,25 %. Si se aprobara la identidad como excepcion de la regla 2, cerraria.
- Copia debil `size()` 0x800E0AA8: sin tocar (decision anterior).

## Siguiente (decomp)

- Lo que mas rindio: leer un miembro por una inline con parametro (`FaceOf_unk(c)`, `TableOf_unk(task)`, `FirstOf_unk`/`NextOf_unk` en el arbol de tareas): la llamada se evalua antes (Sethi-Ullman) y cambia el orden de carga y los registros. Barrido: `mutsweep.py --only val,valb` (sonda `Val_unk`) y `patsweep.py`. Cubierto: >= 98,7 % de todo el juego, 97-98,7 % y 95-97 % solo en parte (logs val3/val4 en el scratch, se pierden); CharaTask entera (97-100 %).
- Huecos de pila: `tools/decomp/slotmap.py` (que objeto se movio) y `tools/decomp/mirrorscan.py` (grupos invertidos: el original los tenia en una inline o no). Lista de mirrorscan 97-100 %: unas 70 funciones; probadas solo 8. Siguientes: EventParser_unk Parse*ID_unk (9 hermanas, 99,26-99,91 %, objetos de 1 byte de las copias de `std::string` en push_back), Em203GreaterDaemon fn_8028BFE0, LastCocoonQuadruped fn_80462CCC / fn_80465B88, ReplayBuffer fn_8056C3D8 / fn_8056F5B4, Em204BigMulue push_back, main push_back de SimpleVector, gmk/PigeonManager y ChickenManager (gemelas).
- Cierres de la ultima hora que abren familia: los aciertos del barrido `val` en 95-98,7 % (val3/val4) solo se recorrieron en parte; seguir la banda 95-98,7 con `valdrv`/`mutsweep --only val,valb` y reescribir cada acierto como accesor (GameCamera x2 cerraron asi; GameUI fn_803DE89C 99,62, ControlCharge vf0C 99,71, RenderManager fn_800C06B0 99,93, Light fn_800795DC 99,32 quedaron cerca).
- Casi cerradas: Em403WarLion fn_80329ED8 (99,25, ver decision), EnemyQuarkStand fn_8034F788 (99,88: solo el par copia/objetivo de SetTarget_unk), Em101LittleZesya fn_8025D90C (99,81, registros del primer bloque), BattleTask fn_801018E4 (99,67), PassBoard vf0C (99,45; enlazaria la unidad), CompositeCacheEntry_unk fn_800D8458 (99,19).
- Bloqueos confirmados (no repetir sin idea nueva): el registro base de un miembro (`m4D8`/`m97C`, fase 2: no reproducido en fichero suelto, ver `docs/mwcc-blockers.md`), `Loader::Get_unk<T>`, la familia vf3C de NewCampParts (mismo codigo con todas las formas), atn/Task fn_800D32B8 (cualquier cambio cambia el inlining).
- Lista de candidatas: `claude/candidatas-bloqueos-20261122.md` (proyecto).

## Herramientas nuevas

- `tools/decomp/slotmap.py <unidad> <simbolo> [-v]`: grupos de pila movidos (original, nuestro, delta, tamano, primera fila).
- `tools/decomp/mirrorscan.py [--lo P] [--hi P] [--min B] [--units ...]`: funciones con grupos de pila en orden invertido.
- `tools/decomp/patsweep.py <regex> <reemplazo> [--units] [--min] [--max] [--helper] [--log] [--all]`: una reescritura en cada coincidencia de las funciones < 100 %.
- `tools/decomp/frontsweep.py`: el vector unitario girado por una inline (`FrontY_unk`).
- `mutsweep.py`: kinds `val`, `valb`, `lentmp` (tambien con una inline `DistV_unk`).

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
