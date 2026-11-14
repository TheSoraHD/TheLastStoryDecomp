# cabeceras-20261114, cola L: vistas de `lbl_8087F048` y `MovieState_unk`

Rama `agent-L`. Inicio 19:37. Referencia `$REF` = `/home/claude/ref-L` (15447 / 18109).

## Resultado

### 1. Vistas `Effect_unk` de `lbl_8087F048` -> `BattleTask*`

- 64 unidades llamadoras (las 62 con `struct Effect_unk;` adelantada o vacia, mas CharaTask y Em315Coffin):
  `extern "C" BattleTask* lbl_8087F048; // inferred: the battle manager`, fuera `struct Effect_unk;` y las
  vistas vacias (ControlCharge, ControlChant), y `Effect_unk` -> `BattleTask` en las declaraciones `extern "C"` de
  las funciones C de BattleTask (`fn_800F8548`, `fn_800FAB80`, `fn_801092C8`...), en locales y en inline locales
  (`GetEffect_unk`, `fn_8013A194`, `fn_802F0964` de Em306GurgBeast). Fuera los moldes `((BattleTask*)lbl_8087F048)`,
  `((BattleTask*)fn_8013A194())` y los de locales que ya son `BattleTask*` (CharaStatus `fx`, Em314Ivy `effect`).
  Las seis unidades que no incluian `game/BattleTask.h` (Em310Gargoyle, Em316LaserSphere, ControlShoot,
  Em305VampireMp, ControlStickByQuark, Em315Coffin) usan `class BattleTask;` adelantada (sin `@N` nuevas).
- Las funciones C siguen `extern "C"` con su declaracion local en cada unidad (no aprobado pasarlas a miembros).
  No se ha movido ninguna declaracion a `BattleTask.h`: los tipos de los demas argumentos difieren entre unidades
  (`void*`/`HitInfo_unk*`, `Color4f_unk&`/`EffectColor_unk*`...) y una sola declaracion obligaria a moldes en las
  llamadas. Deuda: unificar esas declaraciones.
- ControlChant: `fn_801070C8`/`fn_80107168` ya eran miembros de `BattleTask` (definidas como tales en
  BattleTask.cpp) y la unidad las declaraba `extern "C"` (no enlazaban): ahora `lbl_8087F048->fn_801070C8(...)`
  con `&Color4f_unk(...)` como ControlDamage.
- CharaTask: fuera `EffectCharaSlot_unk` y la vista con `mCBF0`/`m3E304`/`m3E39C`; `GetEffectSlots_unk` devuelve
  `BattleChara_unk*` (`mCharas`); `m3E304` es el `EffectColor_unk` de BattleTask (se pasa a `__SetFloat4_unk` con
  molde a `atn::Model::Float4_unk`, mismo layout; `fn_80145334` 98,25 -> 98,27 %). `Effect_unk::fn_8013A19C` pasa a
  `BattleTask::fn_8013A19C(int)` (declarada en `BattleTask.h` sin nombre de parametro; `symbols.txt` renombrado
  `fn_8013A19C__10Effect_unkFi` -> `fn_8013A19C__10BattleTaskFi`, sigue al 100 %).
- Queda `struct Effect_unk { atn::Vector3 fn_80109950(CharaTask*); };` en CharaTask y Em315Coffin, con molde
  `((Effect_unk*)fn_8013A194())->fn_80109950(this)`: BattleTask.cpp (cola K) la define todavia como
  `Effect_unk::fn_80109950`. Pendiente: pasarla a `BattleTask::fn_80109950` alli y quitar las dos vistas.
- Visto y no tocado: varias unidades llaman `((BattleTask*)w)->fn_80108378(p)` con `w` un `*Weights_unk`/`slots`
  (Em003LesserDaemon, Em101ElderZesyaSecond, Em206CocoonMp, Em301LShade, Em308InvertCrab, Em309Ghoul,
  Em309GhoulSimple, Em403WarLion, EnemyQuarkSecond, EnemyQuarkStand, em005_doppel_ganger): `fn_80108378` es de
  `BattleChara_unk` (la entrada de `mCharas`), no de `BattleTask`. Deuda de otra cola.

### 1b. Vistas similares (solo declaracion adelantada o `void*`)

- 15 unidades con una vista adelantada de otro nombre pasan a `BattleTask*` del mismo modo: `BulletMgr_unk`
  (CharaListeners_unk, gmk/Coffin, EnemyLizerdBlade, Np131LivingArmor, Em316LastLaser, HomingBullet, SparkCube,
  LandMine, GimmickStateCommand, EnemySeaLizerd, game/Bullet), `BulletPool_unk` (BulletPoint), `ArmsEffectMgr_unk`
  (Arms), `MountleEffect_unk` (ControlMountle), `WaitBullets_unk` (WaitObject).
- 5 con `void*` que ya incluian `BattleTask.h` y moldeaban en cada llamada: LottaSummon, Em002_01BlackSkelton,
  Em006DarkMaster, ControlDive, Event/Manager (fuera los moldes; `fn_800F8548(BattleTask*)` en Em002).
- Quedan (vistas con cuerpo o `void*` sin la cabecera; para otra pasada): CannonBullets_unk (Cannonball),
  BrazierBullets_unk (Brazier), BattleState_unk (GameSoundControl), ArenaEnemyMgr_unk (ArenaTask), PickBattle_unk
  (PickTask), WallEffect_unk (ControlWallUp), DropBulletMgr_unk (DropItem), AiEnemyMgr_unk (AiCommon_unk),
  BulletMgr_unk con cuerpo (FragileObject, CharaEquip_unk), Colli_unk (ControlAppear), `atn::Task*` (GameScene).
- Segundo commit: los `void*` sin la cabecera (ShopUpGrade, ControlDropDown, RandomMerchant, RandomItem,
  CosEditBase, ItemManager_unk, ItemSlotAnime, ControlInteractionBase) con `class BattleTask;` adelantada y
  `BattleTask*` como primer argumento de las funciones C de BattleTask que declaran (`fn_801092C8`, `fn_8010A308`,
  `fn_8010C00C`, `fn_80108C10`). Neutro, precommit OK.

### 2. `Event/MovieState_unk.h` -> `atn::VideoTHP`

- Borrada `include/Event/MovieState_unk.h`. `Event/Manager.h`: `namespace atn { class VideoTHP; }`,
  `void fn_8048E9A4(atn::VideoTHP* movie, int frames);` (con nombres, como antes, para no renumerar) y
  `atn::VideoTHP* m1F70`. `symbols.txt`: `fn_8048E9A4__Q25Event7ManagerFPQ25Event14MovieState_unki` ->
  `fn_8048E9A4__Q25Event7ManagerFPQ23atn8VideoTHPi`.
- Manager.cpp y ActionEffectPlay.cpp incluyen `atn/VideoTHP.h` (ya incluian `IRender.h` por otras cabeceras).
  ActionEffectPlay: fuera `MovieRes_unk` (sin uso) y los moldes; `MovieTask::m4C` ya era `atn::VideoTHP*`;
  `vf3C(volume, frames)` con el orden de `VideoTHP` (float, int).

## Decisiones tomadas sin el usuario

- Declaraciones locales `extern "C"` con `BattleTask*` en vez de una sola en `BattleTask.h` (ver arriba).
- Molde `(Effect_unk*)` para `fn_80109950` mientras BattleTask.cpp la defina como miembro de `Effect_unk`.

## Para el port

- `BattleTask` gana el metodo `void fn_8013A19C(int)` (antes `Effect_unk::fn_8013A19C`, definido en
  game/CharaTask.cpp; pone `m3E39C`).
- `Event::Manager::fn_8048E9A4(MovieState_unk*, int)` -> `fn_8048E9A4(atn::VideoTHP*, int)`;
  `Manager::m1F70` `MovieState_unk*` -> `atn::VideoTHP*` (mismo offset). `MovieState_unk::vf3C(int, float)` era
  `VideoTHP::vf3C(float, int)`: en ActionEffectPlay los argumentos van ahora en el orden de VideoTHP.
- Las declaraciones `extern "C"` de las funciones C de BattleTask en los llamadores tienen `BattleTask*` como primer
  argumento (antes `Effect_unk*`). Unidades del port tocadas: BattleCharaTask (solo esas declaraciones y moldes;
  ninguna firma propia cambia).

## Comprobaciones

- Primer commit: `pdbatch.sh` de Event/ActionEffectPlay, Event/Manager, chr/ControlChant y game/CharaTask (la
  inclusion de `atn/VideoTHP.h` y los cambios de CharaTask renumeraban `@N`: rep100 peor antes, igual despues).
  `lbl_8087D6E8/EC`, `sInstance`, `lbl_80885CB0`, las cadenas 0x80783CA8/0x8078CB00-10 y
  `@enum$...ControlAppear_cpp` sin cambios. 15447 / 18109, lost 0, sin bajadas, labelcheck/rep100 igual,
  `main.dol: OK`. poolcheck: solo los avisos de floats ya conocidos (15.0, pi...) en las unidades tocadas (los
  cambios son de tipos de puntero, no tocan constantes).

## Hallazgos

- `report.json` no ve las `@N` renumeradas (CharaTask seguia "383 al 100 %" con las etiquetas corridas); `fnstat.py`
  (objdiff-cli diff) y el rep100 de `precommit.sh` si: mirar siempre el rep100 tras tocar includes o tipos.
- Quitar o cambiar moldes y tipos de puntero en las llamadas no renumera; si lo hace incluir una cabecera con
  clases nuevas (aqui `atn/VideoTHP.h`) o, en CharaTask, el conjunto de cambios (no aislado: `pdbatch` lo arregla).
