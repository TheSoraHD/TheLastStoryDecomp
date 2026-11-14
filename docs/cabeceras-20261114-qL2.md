# cabeceras-20261114, cola L2: resto de vistas de `lbl_8087F048` y `BattleChara_unk::fn_80108378`

## Resultado

### 2. `fn_80108378` es de `BattleChara_unk`

- En el `.dol` esta entre `BattleChara_unk::fn_801081D8` y `fn_801083D8`; no usa `this` (lee `lbl_8087F048`).
  Los once llamadores la invocaban como `((BattleTask*)w)->fn_80108378(p)` sobre la entrada que devuelve
  `BattleTask::fn_80103F60` (una `BattleChara_unk*`).
- `BattleTask.h`: la declaracion pasa de `BattleTask` a `BattleChara_unk` (con el mismo nombre de parametro, para no
  renumerar `@N`). BattleTask.cpp: `int BattleChara_unk::fn_80108378(CharaTask*)` (solo el nombre de clase).
  `symbols.txt`: `fn_80108378__10BattleTaskFP9CharaTask` -> `fn_80108378__15BattleChara_unkFP9CharaTask`.
- Fuera las vistas `{ m00[0x128]; float m128[]/mWeight[]; }` de las once unidades (GhoulWeights_unk x2,
  BattleSlots_unk x2, DaemonAi_unk, CrabWeights_unk, CocoonSlots_unk, AllyAi_unk, ElderWeights_unk,
  QuarkWeights_unk, DoppelAi_unk): ahora `BattleChara_unk*` y `m128`, sin moldes. Em309Ghoul, Em309GhoulSimple,
  Em301LShade, EnemyQuarkSecond, Em003LesserDaemon, Em308InvertCrab, Em206CocoonMp, Em403WarLion,
  Em101ElderZesyaSecond, EnemyQuarkStand, em005_doppel_ganger. Neutro: precommit OK, fnstat igual.

### 1. Vistas con cuerpo de `lbl_8087F048` -> `BattleTask*`

- GameScene (`atn::Task*`, ya incluia la cabecera): fuera los moldes `((BattleTask*)lbl_8087F048)`.
- ControlAppear (`Colli_unk`): `class BattleTask;` adelantada, `fn_8010A308(BattleTask*, ...)`; `lbl_8087EE98` sigue
  como `Colli_unk` (otro objeto). `@enum$6929ControlAppear_cpp` sin cambios (el aviso de poolcheck ya estaba).
- AiCommon_unk (`AiEnemyMgr_unk` + `Effect_unk` con `fn_8010C948`), ArenaTask (`ArenaEnemyMgr_unk`): incluyen
  `game/BattleTask.h`, `mCharas`/`mCharaCount`; AiCommon llama `BattleTask::fn_8010C948` (antes `Effect_unk::`, que
  no enlazaba).
- GameSoundControl (`BattleState_unk`): `m38C90` es el `BattleSel_unk` (`m00`, `m04`, `m0C`, `m10`), `m38CCC/m38CD0`.
- ControlWallUp (`WallEffect_unk`/`WallColor_unk`): `BattleTask::m3E304` con `EffectColor_unk(r, g, b, a)`; las tres
  funciones C (`fn_800F8548`, `fn_800FAB80`, `fn_800FB4B0`) con `BattleTask*`.
- CharaEquip_unk (`BulletMgr_unk::mMarkEfp[7]` en 0x3663C): `BattleTask::mEfp3663C[mark]`.
- PickTask (`PickBattle_unk`/`PickSlots_unk`): `mCharas`/`mCharaCount`; el inline `PickSlots_unk::GetChara_unk(i)`
  pasa a `GetBattleChara_unk(BattleChara_unk*, int)` en el fuente (con `lbl_8087F048->mCharas[i].mChara` directo
  vf10 bajaba 96,49 -> 94,61; con el inline tomando `BattleTask*` 95,30; tomando el array, igual que antes).
- `pdbatch.sh` de las ocho (GameScene 150 -> 153, AiCommon 33 -> 35 en fnstat). El re-emparejado volvia a poner
  `@STRING@Resize_unk...` en `lbl_8087D6E8/EC` y `sInstance` en 0x8087EE78 (y el enlace fallaba): devueltos por
  linea. Neutro: precommit OK, 15450, lost 0, sin bajadas, labelcheck/rep100 igual.
- DropItem (`DropBulletMgr_unk`/`DropBullet_unk`/`DropAttack_unk`/`DropBulletOwner_unk`): era una vista de
  `BattleTask::mBullets` (`Bullet`, 0xA0) contada desde `Bullet+4`. Ahora `Bullet*` con `m04` (flags), `m08`
  (posicion), `m20` (velocidad), `m88->mType` (dueno jugador) y `BulletShot_unk` copiado miembro a miembro de
  `m5C..m74`; `IsBlocked_unk(BulletShot_unk)` mira `HitInfo_unk::mAC`. Con `GetShot_unk()` vf10 bajaba
  (90,70 -> 90,22: un temporal mas); con la copia explicita sube 90,70 -> 91,74. pd + precommit OK.

### 3. Declaraciones `extern "C"` de BattleTask en `game/BattleTask.h`

- Al final de `BattleTask.h`, sin nombres de parametro y con el tipo de la definicion de BattleTask.cpp:
  `fn_800F8548`, `fn_800F8574`, `fn_800FAB80` (devuelve `int`), `fn_800FB4B0` (`int`), `fn_80108F38`, `fn_801092C8`,
  `fn_8010A308`. Las 67 unidades que incluyen la cabecera pierden sus declaraciones locales (script
  `scratch-L2/decl.py`); ajustes de llamadas: `fn_80108F38(fx, &chara->fn_801781B0(), ...)` (CharaStatus x18,
  DamageCalc_unk, Em308ParmCrab: antes por referencia), EnemyQuarkSecond `&mPos, &mRot` y `&(m164C + dir)`,
  FragileObject `CharaTask* chara` (antes `int` con molde). Codigo identico.
- Quitar las declaraciones locales (con nombres de parametro) renumera `@N`: `pdbatch.sh` de las 67 + BattleTask, y
  a mano lo que el re-emparejado dejo mal: GameEventScript (5 floats de .sdata2 0x80885B10-34 con el desplazamiento
  -18 de HEAD; `lblmap.py` lo habia empeorado a 66 sin nombre), QuadrupedTask (double .rodata 0x8075FCA0 @14270 ->
  @14269), Em206CocoonMp (0x808841C0 @18117 -> @18091). `lbl_8087D6E8/EC` y 0x8087EE78 devueltos tras cada pd.
- precommit: main.dol OK, 15450, lost 0, sin bajadas, labelcheck/rep100 igual. Unico FAIL: ordercheck de
  GameEventScript (1 fuera de orden `__dt__15GameEventScriptFv` + `FindWarpIn_unk` extra), igual con el fuente y la
  cabecera de HEAD: ya estaba.
- Segunda tanda: `fn_800FBA9C`, `fn_800FBE70` (`const atn::Vector3*`), `fn_80101434`, `fn_8010C00C`; fuera las locales
  de CharaTask, Em201ZanGurgFinal, Em206EggMonster, Em403WarLion, EnemyQuarkFirst, EnemyTasha, LastCocoonQuadruped.
  pd + `shiftfix.py` (EnemyQuarkFirst 0x808851B0 @10596 -> @10590). precommit OK.
- No hechas por tipos que no casan con la definicion: `fn_8010B250` (los llamadores pasan `int` donde la definicion
  tiene `BattleCalcParam_unk*`), `fn_800FDB20` (un `float` de mas en ZanGurgFinal/Em206EggMonster frente a un `int`
  de la definicion), `fn_800FB1BC` (SkillEffectManager sin el `float` final), `fn_80101524` (5 argumentos frente a
  6), `fn_8010CF28`, `fn_800FD658` (`void*` donde la definicion tiene `HitInfo_unk*`); y las de un solo llamador.
- No hechas: `fn_80108C10` (cada unidad tiene su vista del dano: `DaemonDamage_unk`, `QueenDamage_unk`...; unificar
  `BattleDamage_unk` es otra tarea) y las de 1-5 declaraciones. Las unidades que no incluyen `BattleTask.h` (`-` en
  `sigs.py`: Coffin, SparkCube, LandMine, WaitObject, GimmickStateCommand, Bullet, Em310Gargoyle...) siguen con su
  declaracion local (incluir la cabecera renumera); varias tienen tipos distintos (`int`/`void*` en vez de
  `CharaTask*`/`HitInfo_unk*`, Bullet con `fn_800FAB80` devolviendo `int` y los float antes de dos int).

### 4. `fn_80109950` -> `BattleTask` (pendiente de la cola L)

- BattleTask.cpp: fuera la vista `Effect_unk`, `atn::Vector3 BattleTask::fn_80109950(CharaTask*)` (declarada en
  `BattleTask.h` sin nombre de parametro); `symbols.txt` `fn_80109950__10Effect_unkFP9CharaTask` ->
  `fn_80109950__10BattleTaskFP9CharaTask`. CharaTask y Em315Coffin: fuera sus vistas `Effect_unk` y el molde
  (`fn_8013A194()->fn_80109950(this)`); Em315Coffin incluye ahora `game/BattleTask.h` (y pierde sus declaraciones
  locales de las funciones C del paso 3). Em312DarkServant: `class BattleTask;` en vez de `struct Effect_unk;`.
- `pdbatch.sh` de las cuatro; el re-emparejado de BattleTask dejo 14 floats de .sdata2 con el nombre de otro valor:
  `scratch-L2/shiftfix.py` (nombre de HEAD menos k, k = 0..3, el primero cuyo objeto en nuestro .o tiene los
  mismos bytes que el .dol) los arregla; Em315Coffin 0x80884D4C @9459 -> @9927 (+468, HEAD). precommit OK.

## Decisiones tomadas sin el usuario

- La cola pedia la declaracion sin nombres de parametro; la ya existente tenia `chara` y se ha movido tal cual:
  quitarlo renumeraria `@N` en todas las unidades que incluyen `BattleTask.h`.

## Para el port

- `game/BattleTask.h` declara `extern "C"` (sin nombres): `fn_800F8548`, `fn_800F8574`, `fn_800FAB80`, `fn_800FB4B0`,
  `fn_80108F38`, `fn_801092C8`, `fn_8010A308` (tipos de la definicion). Las unidades que incluyen la cabecera ya no
  las declaran; `fn_80108F38` recibe `const atn::Vector3*` en todas (antes `&` en CharaStatus, DamageCalc_unk,
  Em308ParmCrab) y `fn_800FAB80` punteros en EnemyQuarkSecond. Unidad del port tocada:
  BattleCharaTask (solo quitar sus declaraciones locales).
- `Effect_unk::fn_80109950(CharaTask*)` -> `BattleTask::fn_80109950(CharaTask*)` (BattleTask.cpp, solo el nombre de
  clase; no usa `this`).
- Vistas de `lbl_8087F048` quitadas (GameScene, ControlAppear, AiCommon_unk, ArenaTask, GameSoundControl,
  ControlWallUp, CharaEquip_unk, PickTask, DropItem): ahora `BattleTask*`; ninguna unidad del port salvo GameScene
  (no es del port). `PickTask` usa un inline `GetBattleChara_unk(BattleChara_unk*, int)` local.

- `BattleTask::fn_80108378(CharaTask*)` -> `BattleChara_unk::fn_80108378(CharaTask*)` (no usa `this`). Unidad del
  port tocada: BattleTask (solo el nombre de clase de la definicion).
