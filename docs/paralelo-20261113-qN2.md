# Cola N2 (lote paralelo-20261113): segunda pasada de cerca de enlazar

Lista: `/home/claude/scratch/n-candidates.txt` (98 % y menos, y las de 99 % que la cola N no probo).

## Resultado por unidad

- `gmk/MeetingSpot`: fn_803FD328 95,29 -> 100 %, fn_803FD0A0 99,97 -> 100 %; **enlaza** (`matching: true`).
  Dos causas: (1) el motion del estado 7 es un ternario de constantes `enum` en el argumento
  (`PlayMotion_unk(i, i == 0 ? kMeetMotion76_unk : kMeetMotion77_unk, 1)`); (2) `PlayMotion_unk` no es
  `inline`: definida entre vf10 y fn_803FD0A0, `-inline auto` la expande en fn_803FD328 y el enlazador quita
  su copia; sus 1.0f/0.1f quedan en `.sdata2` antes del 6400.0f de fn_803FD0A0, como en el original.
- `gmk/Augur`: vf10 99,22 -> 100 %; **enlaza**. Estado 5: `int r = rand() % 100;` y
  `fn_8041F6A4(r > mDesc->m24 ? kAugurState6_unk : kAugurState7_unk)` (ternario de `enum` en el argumento:
  `this` se prepara antes de la comparacion, como en el original).
- `chr/ControlStickByQuark` (fuera de las colas A-M y de la ruta; no enlaza, 29 parciales): ctor de
  ControlThrustForQuarkStand 99,87 -> 100 % con `int motion = mKind == 2 ? kThrustMotion216_unk :
  kThrustMotion215_unk;` (`enum`).
- `chr/ControlDamage` (fuera de colas): ctor de ControlGuarded 90,16 -> 100 % (`a ? kGuardMotion1DD_unk :
  kGuardMotion1DE_unk` en el argumento).
- `game/PlayerInput_unk` (fuera de colas): fn_80122C70 99,3 -> 100 % (`return ... ? kInput07_unk :
  kInput00_unk;`: tambien con 7/0 MWCC quitaba el salto). Con pd.sh la unidad pasa de 27 a 30/31; queda
  fn_801231D0 99,46 (el `return 0` de las acciones 39/40 antes de la llamada; probados if invertido y ?:).
- `game/EnemyQuarkStand` (fuera de colas, K tenia EnemyQuarkSecond): fn_80351AB8 99,23 -> 99,97 (el motion
  con ?: de `enum`; queda la direccion de los slots por r3).
- `game/ChaserNpc` (fuera de colas): fn_80180AE0 98,73 -> 99,90 (`MAX(0, m14D0 - 1)` en vez de
  `MAX(m14D0 - 1, 0)`); queda el orden de los temporales en la pila.
- `game/Em306GurgBeast` (fuera de colas): vf104 99,60 -> 99,85 (estado 8/9 con ?: de `enum`); queda CanAct_unk.
- `chr/ControlAttackBase` (fuera de colas): ctor de ControlAttack 98,95 -> 99,63 (`kind` con ?: de `enum`
  tras rand()); queda la pila del vector frontal.
- `game/Em206EggMonster` (fuera de colas): fn_802C38E4 99,32 -> 99,93 (`next = rand() % 2 == 0 ? 2 : 4;`:
  con constantes que difieren en 2 basta un ?: de `int`); quedan las ranuras de pila.
- `game/EnemyTrista` (fuera de colas): fn_8035CF78 97,99 -> 100 %. El float de fn_800F8C6C va antes de
  `time` y `count` en la declaracion (el orden de evaluacion del original es 0.0f, 30, m594).
- `game/Em401_01_WhiteLeoPard` (fuera de colas): fn_8031FE4C 98,63 -> 100 % con la misma firma de
  fn_800F8C6C (float antes). `game/QuadrupedTask`: fn_80566FB4 86,11 -> 87,52 con la misma firma.
- `game/Em202SpiderBase` (fuera de colas): fn_80285FA0 99,65 -> 100 % (`float dist;` declarada antes y
  `float rate = 1.0f;` asignada primero: el 1.0f se carga en el prologo y dist queda en f31).
- `game/EnemySeaLizerd` (fuera de colas): fn_80355390 98,87 -> 99,96 (copia del puntero `CharaTask* c = chara;`
  al principio de GetFrontPos_unk); queda f4/f6 del frsp de fn_80139F60.
- `chr/ControlShoot`: ctor 98,76 -> 99,90 %. El motion de los casos 0x25/0x26 con el ternario de `enum` y
  `mFB4 = mData->m48 + mFB4`. Queda el registro del cero de mFB0/mFB4 (r6 por r4).
- Sin cambio (intentos revertidos, 2-8 cada una): `db/MapEnvSeTable_unk` fn_80214F80 (operator[] e inline
  de busqueda), `gmk/FallingTower` fn_8041044C (inline de mezcla Blend_unk(a, b, t): igual), `game/MapManager`
  FindArea (orden begin/end), `Event/EventCommandDef_unk` fn_800081C0 (inline de busqueda, referencia),
  `atn/LightUtil_unk` fn_8007A154 (`(int)size()` en la comprobacion da la recarga de mSize, pero cruza
  src/dst; helpers de copia), `db/CharaIdTable_unk` fn_80208BE8 (temporal con nombre; inline de nombres no
  se expande por el presupuesto), `gmk/InteractionSpot` Start (found/break, inline estatica por referencia).
- No probadas por bloqueo conocido o notas previas: RandomItemTable dtor (doble beq, `docs/trucos-mwcc.md`),
  Event/Label `__sinit`, ControlDraggingStart vf10 (mismo residuo que ControlCdnSleep), LandMine Start,
  HealPoint, Texture_unk fn_800D5A3C, CharaLight vf6C, Section/EfpHandle.

## Hallazgos

- **Ternario de constantes `int` que difieren en 1 -> MWCC lo hace sin salto** (`cntlzw`/`subfic`+`nor`+`srawi`
  + `addi`), tambien dentro de una inline o con `!=`/`<`. Con constantes de un **`enum`** queda el salto del
  original: `li r4, 0; li r5, C2; bne; li r5, C1` evaluado en el orden de los argumentos (MeetingSpot,
  ControlShoot).
- Constantes de `.sdata2` antes de las de una funcion anterior: un metodo **no inline** que `-inline auto`
  expande en una funcion posterior y que el enlazador quita deja sus constantes en la posicion de su
  definicion (MeetingSpot::PlayMotion_unk).

- `MAX(x, 0)` (macros.h) da `neg/andc/srawi/and`; `MAX(0, x)` da `srawi/andc` (ChaserNpc).
- `enumscan2.py` (cualquier par de constantes) encuentra 369 funciones; las que tienen en nuestro lado
  `neg`/`cntlzw` donde el original salta son pocas (MaterialParser fn_8007C46C, EffectDesc fn_80234CD8,
  D2AMpMenuLobby fn_804C18A8): pendientes.

- **Orden de evaluacion de argumentos = orden de la declaracion**: un `lfs` de un argumento float antes
  que los `li`/`lwz` de enteros posteriores indica que el float esta antes en la lista de parametros
  (EnemyTrista, fn_800F8C6C). `limove.py` (scratch-N2) busca `li` movidos en las parciales.

## Decisiones tomadas sin el usuario
- `fn_800F8C6C` en EnemyTrista.cpp declarada con el float en 7a posicion; las otras ~40 unidades la declaran
  con el float al final (cada una su propia `extern "C"`, deuda de BattleTask): no se tocan.

- `include/gmk/MeetingSpot.h`: `PlayMotion_unk` pasa de `inline` a declaracion normal (la incluyen
  MeetingSpot y GimmickCommon; sin cambio de `@N`, labelcheck/rep100 igual).
- `enum` anonimos con nombres inventados (`kMeetMotion*_unk`, `kShootMotion*_unk`) en los `.cpp`.
- `matching: true` en `gmk/MeetingSpot` y `gmk/Augur` (con `main.dol: OK`).
- Con la cola casi agotada, aplico el ternario de `enum` a funciones de unidades que no son de ninguna cola
  A-M ni de la ruta (salida de `enumscan.py`), empezando por chr/ControlStickByQuark.

## Pendientes

- `GameBgm::fn_8037D4C0`: con la firma `(int id, float volume, int fade, int b, int c)` SongManager
  fn_805A2FD4 y fn_805A32C8 pasan a 99,95 (solo el nombre mangled; con el renombrado en symbols.txt
  quedarian al 100 %). Hay ~45 llamadas en 14 ficheros (PacketPack, GameTask, Event/Manager, Em203*,
  Em206CocoonMp, GameEventScript...): no lo hago (otras colas y fuera de alcance). Revertido.

- `/home/claude/scratch-N2/bin/enumscan.py` lista funciones <100 % con `li rX, C; bxx; li rX, C+-1`
  (sin pares 0/1): 52 en todo el juego, la mayoria de otras colas (Event/Action*, MP/D2AMpMenu*, chr/Control*,
  GameUI, CharaTask...). Probar en ellas el ternario de `enum`.
- ControlShoot ctor: el cero de mFB0/mFB4.
