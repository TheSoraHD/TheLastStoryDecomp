Inicio: 2026-11-26 23:10 (Europe/Madrid). Sesion secuencial de 12 h (sin agentes), prompt `claude/prompt-12h-accesores.md`.

# accesores-20261126

Objetivo: subir `matched_code_percent` cerrando funciones al 100 % con accesores con nombre (aciertos de la sonda `Val_unk`) y la pila (`slotmap.py`/`mirrorscan.py`). Partida: main fdac202, 16400 / 18113, 756 unidades, codigo 66,40 %, datos 76,94 %, lost 0, 387 avisos. Referencia de este entorno (`mkref.sh`): rep100 10, labelcheck 15 lineas. MAINREF en /home/claude/mainref (symbols.txt y .o de fdac202).

## Fase 0

- Driver `tools/decomp/valdrv.py` en la rama (el del apendice; el tercer argumento admite `min-max` de tamano y escribe `DONE` al acabar).
- Worktrees /home/claude/scratch/sweep (v97: 97,0-98,0 % >= 200 B, 159 funciones; despues vs60: 95-100 % de 60-200 B, 28) y sweep2 (v90: 90-96,29 % >= 200 B, 306), kinds `val,valb`.

## Fase 1

- RenderManager fn_800C06B0 (99,93 -> **100**): el color de la niebla en una inline (`FogColorOf_unk(&m8A0)`, su temporal queda al fondo de la pila) y el bloque de reinicio como llamada a `fn_800C08B0()` (que se inlinea: su `color` queda debajo de `arg`, como en el original). Sin segunda declaracion de GXSetFog.
- GameUI fn_803DE89C (99,62 -> **100**): `HeadOf_unk(player)` (referencia a mF6C) en las dos copias del centro, `AimPointOf_unk(field->mCamera)` (m8A4) y el radio de la esfera en un local (`float radius = m5C * m28;`).
- Sin cierre (tope): EnemyQuarkStand fn_8034F788 (17 variantes: el temporal por valor de SetTarget_unk queda encima de `pos`; inlines con `pos` por valor o referencia, AimDirV_unk por valor, sin local: igual o peor), Em101LittleZesya fn_8025D90C (17: bool local, if/else, inline IsEnraged_unk, RangeOf_unk, orden de la suma: igual), ControlCharge ctor/vf0C (los dos bools anidados de IsCursorPlayer_unk/IsCursorMode_unk r4/r5: return directo, bool local, if anidado, if/else: igual o peor), BattleTask fn_801018E4 (LevelOf_unk, AttackerOf_unk, orden de locales, const: igual), PassBoard vf0C (mutsweep plateau: nada), CompositeCacheEntry_unk fn_800D8458 (iterador `end` con nombre antes/despues: igual).

## Fase 2

- Aciertos del barrido convertidos en accesores con nombre (solo fuzzy): `RotOf_unk(nodes[i])` (copia de la rotacion por valor) en las callbacks de pose de Em401_01_WhiteLeoPard/Em403WarLion/QuadrupedTask (97,87 -> 99,97/99,97/99,89; queda el orden de `q` del cuello y `rot` de la columna en la pila); `mListBase->MeltTask()` en ShopColor fn_805880F0 (97,90 -> 99,95; queda r24/r27 de la direccion de la capa); CharaStatus fn_8012F188 `CharaOf_unk(this)` (95,30 -> 99,29: la inline fn_8012F3D8 repite su prueba como el original); ModelInstance fn_8009E6EC `DataOf_unk`/`DrawOrderOf_unk` (97,53 -> 99,11).
- BattleTask fn_80106878 (97,99 -> 99,56 con `Val_unk(chara)->m648`): ninguna forma con nombre pasa de 98,74 (`SpeedOf_unk(model)` si; `GetPart648_unk()`, `PartsOf_unk`, inline de toda la condicion: no); queda ademas el marco 0x10 mayor: sin aplicar.
- CosEditBase fn_801CE148: `FaceOf_unk` 97,40 -> 97,89 (la sonda llega a 98,25 con `Val_unk(prev)->mChara`): sin aplicar.

## Fase 3

- Regla nueva (confirmada 3 veces): si mirrorscan da TODOS (o casi) los grupos de un bloque invertidos, el original tenia el bloque entero en una inline. Model fn_8008B978 (99,47 -> **100**: el bucle de enlazado de texturas en `BindTextures_unk(this)`; aplicado tambien en fn_8008B140, 99,29 -> 99,44, pila ya igual), DialogHead fn_80227434 (99,37 -> 99,79: el nombre de la cara en `FaceMotion_unk(kind, arg)`; pila igual, quedan registros de la copia de SimpleVector en `std`), gmk/OnOff vf0C (99,86 -> 99,95: `MaxScale_unk(mtx)` de gmk/ArenaFloor), BattleCharaTask fn_800E9ED8 (`SideDir_unk(mRot)`, 98,50: arregla 4 grupos, quedan saltos).
- Objetos de 1 byte invertidos en push_back (FileList fn_8006BD78, EventParser Parse*ID, Em204BigMulue): son los `random_access_iterator_tag` de las copias de `std::string` inlineadas; el orden depende de la profundidad de inline. Probado `__construct_unk(p, x)` (un nivel mas) en el camino rapido y en el movimiento de vector.h: FileList queda con la pila de cada copia bien pero el orden entre copias no, y en todo el juego +31 / -26 y lost 2 (ControlAppear: presupuesto de inline). Revertido. Sube mucho `vector<MatColorInfo>::push_back` (94,95 -> 99,99) y `vector<ColorF_unk>` (95,6 -> 99,6): idea para una sesion de cabeceras.

- Mas cierres de la fase 2/3: DrawEntry_unk ctor (96,75 -> **100**: longitudes de columna con `ColLen_unk(m, i)` empezando por z, como `MaxScale_unk` de gmk/ArenaFloor, y las callbacks copiadas miembro a miembro en el cuerpo); Em203GreaterDaemon fn_8028F1D0 (94,77 -> **100**: `FrameOf_unk(mMotion)` del barrido y argumentos -1, -1, 1 del efecto, que el fuente tenia como -1, 1, -1: cambio de logica para el port); Em203GreaterDaemonRedMp fn_802A17CC (99,34 -> **100**: `FrameOf_unk` en la prueba de 70/80, encontrado con patsweep del accesor anterior).
- Solo fuzzy: EnemyJill fn_8032F828 93,88 -> 99,59 (`CountOf_unk(m15E4)` del barrido y `Pop_unk()`), LastCocoonEgg fn_80454F08 98,95 -> 99,60 (`TableOf_unk`, `EggLerpV_unk`, `VecAssign_unk`), CharaEquip_unk fn_80565B98 99,25 -> 99,40 (`MaxScale_unk(mtx)`: queda el registro base del miembro), gmk/LandMine Start 98,33 -> 98,67 (`RotYOf_unk`).
- Lista nueva `/home/claude/scratch/stackonly.py 97 100` (funciones cuyas diferencias son solo desplazamientos de pila o etiquetas): 24. Probadas sin cierre: el trio WhiteLeoPard/WarLion/Quadruped (q como temporal o en inline: peor), LastCocoonQuadruped fn_80468534 y RandomItem vf14 (dos direcciones de pila invariantes del bucle se emiten en otro orden, mismos registros: declaraciones, copia con operator=, __MtxCopy: igual), SkillEffectManager fn_8059D3B0 (bloque comun en una inline: peor), Em206CocoonMp vf114 (dano en inline: igual), EffectParticle fn_80238588 (el `zero` muerto se parte en escalares; el original lo deja entero en 0x48), LayerShadow fn_800B6C70 y CharaStatus fn_8013354C (el original reutiliza las ranuras de las conversiones int/float), LayerOcclusionCPU fn_800B3978 (orden de los stores: peor).
- Barridos: v97 y vs60 terminados; v90b (90-93 %) lanzado en sweep a las 01:50 (sweep2 sigue con v90 desde arriba; pararlo al llegar a 93).

## Decisiones

## Commits

## Cambios de splits
