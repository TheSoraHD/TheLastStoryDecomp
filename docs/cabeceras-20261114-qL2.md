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
- Queda DropItem (`DropBulletMgr_unk`: vista de `Bullet` desplazada 4 bytes y con `DropAttack_unk` = `BulletShot_unk`).

## Decisiones tomadas sin el usuario

- La cola pedia la declaracion sin nombres de parametro; la ya existente tenia `chara` y se ha movido tal cual:
  quitarlo renumeraria `@N` en todas las unidades que incluyen `BattleTask.h`.

## Para el port

- `BattleTask::fn_80108378(CharaTask*)` -> `BattleChara_unk::fn_80108378(CharaTask*)` (no usa `this`). Unidad del
  port tocada: BattleTask (solo el nombre de clase de la definicion).
