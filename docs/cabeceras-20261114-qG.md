# cabeceras-20261114, cola G: deuda de la regla 10 en cabeceras

Rama `agent-G`. Inicio 16:10. Referencia `$REF` = `/home/claude/ref-G` (15433 / 18109).

## Resultado

### 1. `fn_800F8C6C` como miembro de `BattleTask`

- `include/game/BattleTask.h`: `int fn_800F8C6C(HitResult_unk*, int, CharaTask*, HitInfo_unk*, int, float, int,
  int);` (sin nombres de parametro). Mismo orden de argumentos que la definicion (`out, outCount, chara, data, id,
  angle, time, max`).
- `game/BattleTask.cpp`: la definicion `extern "C" int fn_800F8C6C(BattleTask* mgr, ...)` pasa a
  `int BattleTask::fn_800F8C6C(...)` (`mgr` -> `this`). `symbols.txt`: renombrado
  `fn_800F8C6C` -> `fn_800F8C6C__10BattleTaskFP13HitResult_unkiP9CharaTaskP11HitInfo_unkifii`. Sigue NONMATCHING
  90,6 % (igual).
- 41 unidades llamadoras: fuera las 43 declaraciones `extern "C"` (y sus comentarios del orden del float en
  EnemyJill, EnemyTrista, QuadrupedTask, Em401_01_WhiteLeoPard); las llamadas pasan a
  `((BattleTask*)lbl_8087F048)->fn_800F8C6C(out, count, chara, data, id, angle, time, max)` (el `lbl_8087F048`
  de esas unidades sigue siendo su vista `Effect_unk`/`MountleEffect_unk`; mismo molde que ya usaba
  `Em309Ghoul` con `fn_80109828`). Las 21 que no la incluian incluyen ahora `game/BattleTask.h`.
  Em306GurgBeast: la inline C `fn_802F0964(Effect_unk* mgr, ...)` llama `((BattleTask*)mgr)->fn_800F8C6C`.
- Sube `BattleCharaTask::fn_800E589C` 97,22 -> 97,96 % y `fn_800E9ED8` 97,93 -> 98,50 % (el float en su
  sitio); EnemyJill (que tenia el float en octavo lugar "por el orden de carga") no baja.
- Re-emparejado: `pdbatch.sh` de las 80 unidades que incluyen `BattleTask.h` o llaman la funcion, `lblmap.py` en
  9 (Em202ForestSpider, Em203GreaterDaemonRedMp, Em206CocoonMp, Em305Vampire, EnemyQuarkFirst, EnemyQuarkSecond,
  LastCocoonImago, QuadrupedTask, Em306GurgBeast). `lbl_8087D6E8/EC` devueltos a mano (pdbatch los habia
  renombrado a `@STRING@Resize_unk...`, enlace con simbolos duplicados). 15433 -> 15433, lost 0, sin bajadas,
  labelcheck/rep100 igual, `main.dol: OK`. poolcheck de ControlMountle/EnemyJill/Em306GurgBeast igual que con el
  fuente de HEAD.

### 2. `Effect_unk` de LastCocoonImago y `MeleeTarget_unk`

- `game/LastCocoonImago.cpp`: fuera la vista `struct Effect_unk { m00000[0x3E2E8]; void* m3E2E8; }`;
  `lbl_8087F048` es `BattleTask*` (como en AiTarget) y `fn_800F8548`/`fn_800F8574`/`fn_800FAB80` se declaran con
  `BattleTask*` (como sus definiciones C de BattleTask.cpp). `lbl_8087F048->m3E2E8` es el `atn::ColliBase*` de
  `BattleTask` (comparado con `&m18FC`, un `atn::ColliSphere`). Neutro: 21/36 igual, sin bajadas, labelcheck igual.
- `MeleeTarget_unk` (y `MeleeTargetLess_unk`, `MeleeSort_unk`/`MeleeMedian3_unk`) se queda en
  `game/BattleTask.cpp`: no es una vista de un objeto compartido sino el tipo del `std::vector` local de
  `BattleTask::fn_800F8C6C` (ninguna otra unidad lo usa). Llevarlo a `BattleTask.h` (ctor inline vacio, plantillas)
  renumeraria las `@N` de las ~80 unidades que incluyen la cabecera sin desbloquear nada.

## Decisiones tomadas sin el usuario

- Llamadas por molde `(BattleTask*)lbl_8087F048` en vez de cambiar el tipo de `lbl_8087F048` en las 41 unidades:
  sus vistas `Effect_unk` se usan tambien con otras funciones C (`fn_800F8548`, `fn_800FAB80`, `fn_801092C8`...)
  que siguen siendo `extern "C"` con el `BattleTask*` delante; unificarlas es otra deuda.

## Para el port

- `fn_800F8C6C` es ahora `int BattleTask::fn_800F8C6C(HitResult_unk* out, int outCount, CharaTask* chara,
  HitInfo_unk* data, int id, float angle, int time, int max)` (aprobado por el usuario). Antes: `extern "C"` con
  el `BattleTask*`/`Effect_unk*` delante y, en 39 de las 43 declaraciones, el float al final. Ahora todas las
  llamadas pasan el float en sexto lugar (tras `id`): el problema de posicion del float en el ABI x64 desaparece.
  Unidades del port que cambian: `BattleTask` (definicion), `BattleCharaTask` (`fn_800E589C`, `fn_800E9ED8`).
  Resto de llamadores (no del port): ControlMountle, ControlUseSkill, Em003..Em403, EnemyJill/QuarkFirst/
  QuarkSecond/QuarkStand/Tasha/Trista, LastCocoonImago, LastCocoonQuadruped, QuadrupedTask, em005_doppel_ganger.

## Hallazgos

