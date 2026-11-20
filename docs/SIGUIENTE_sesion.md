# Siguiente sesion (tras mixto2-20261120)

> Sesion `mixto2-20261120` (hasta 4 agentes, 33 colas) del 2026-11-20 01:30 al ~13:40. Notas: `docs/mixto2-20261120.md` y `docs/mixto2-20261120-q*.md`;
> para el port: `claude/port-mixto2-20261120.md` (proyecto).

Estado: **16318 / 18113**, lost 0, rep100 9, labelcheck 10 lineas, `main.dol: OK`. Codigo 65,13 %, datos **76,77 %**
(1099358 B), **754** unidades completas. DRAFT 0. Funciones al 0 %: 45 (20 sin iostream/auto_03).

## Decisiones pendientes del usuario

- `D2AMpMenu::fn_804B3494` llega al 100 % con una relectura sin efecto al final (`if (D2ACommonDialog::sInstance->m4C != 0) { return; }`; el original relee y no usa): regla 2. Parche `docs/mixto2-20261120-qA3-d2ampmenu-3494.patch`.
- Bajadas aceptadas en la sesion: 42 de fuzzy por el push_back nuevo (regla de V; lista en `docs/mixto2-20261120-qV.md`, mas MenuCommonTask fn_804A3C24 94,41 -> 94,13 de V2) y GameTask vf10 95,62 -> 95,55 (cola M, alineamiento del diff; cualquier alternativa baja).
- `goto` anterior al lote en game/Em303KrakenFootMp vf104 (no esta en las excepciones aprobadas).
- `atn::D2A_Task::SetParam2_unk` quiza recibe el rectangulo UV por valor y no `const float*` (GameUI fn_803D2A6C, `docs/mixto2-20261120-qG1c.md`): cabecera muy compartida.
- `GameVoice::fn_803EB394` devuelve `void` (cola D3: arreglo de firma que solo sube fuzzy; integrado).

## Siguiente (decomp)

- A una funcion de enlazar con avance esta sesion: Event/Section (fn_8053C788 99,99: solo el `delete` del buffer, que el original lee a traves del `this` del buffer; Channel/Cut igual), Event/Label (`__sinit` 93,9: `lwz r31` del epilogo entre los dos ultimos stores; ~46 formas). PBoneBasic vf10 99,974 (solo el tamano del marco, parche qN1).
- push_back de `vector<std::string>`: parche `docs/mixto2-20261120-qG5b-vecstring.patch` (fn_803AB96C 99,90, PrintLog_unk 99,86, MapLift vf20 99,89): falta el registro del almacen nuevo tras `__nw` (qV3).
- push_back dentro de otra inline: NewCampPartsHint vf20/vf30, Np131LivingArmor vf10, ShaderManager_unk, ArrowSet_unk, GameConfig_unk AddLight_unk (qV2).
- "Cero copiado del contador" de GameEventScript (~8 funciones): lo causa cualquier `asm` dentro del bucle (la copia paired-single de Vec.h); el original carga `li` (qG5b).
- Unidades grandes: GameEventScript 46 que faltan, PacketPack 46, CharaTask 38 (objdiff), GameUI 18 (parches qG1c), BattleTask 32, GameTask 15, ControlStickByQuark 6, AI_archer_attack01 4 (fn_8001C088 al 0 % por desenrollado + datos extern).
- Muchos parches de solo fuzzy en `docs/mixto2-20261120-q*-*.patch`: utiles como punto de partida.
- Trucos que mas rindieron (seccion mixto2-20261120 de `docs/trucos-mwcc.md`): forma de QuadrupedTask vf20 en las gemelas, getter `static inline` para la segunda lectura de un miembro/global, enum para if/else 0/1, puntero a la copia local para forzar relecturas, local sin inicializar rellenado por la inline, objetos ptmf `lbl_` partidos en `@N`, `__sinit` con `@N` de `.bss` que faltaban, ctor/dtor inline sacados de la cabecera para el orden de `.sdata2`.

## Herramientas

- Integracion de colas: `tools/decomp/symmerge.sh X` (bloques en conflicto de symbols.txt con HEAD via `ourshunks.py` + `refmap2.py` con MAINREF = copia del agente, recompilando antes sus `.o`). `tools/decomp/dollarfix.py` para nombres `$N` tras renumerar.
- `tools/decomp/permrun.py <unidad> <simbolo> <funcion>`: prueba los ordenes de bloques cortos de asignaciones. `tools/decomp/stk.py <unidad> <simbolo>`: ranuras de pila nuestras frente al original.
- Re-emparejado: `MAINREF=<build de referencia> python3 tools/decomp/refmap2.py <unidades>` (por direccion). Una `static inline` nueva renumera los `@N` de la unidad; una inline de clase no plantilla en una cabecera consume 4 `@N` en cada unidad que la incluye.
- Agentes que piden permiso de Bash se quedan parados: pararlos y reanudarlos con un prompt de continuacion.

## Candidatas por datos (report.json de mixto2-20261120)

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
