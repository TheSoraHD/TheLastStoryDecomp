# Lote paralelo 2026-11-13, cola A (Batalla)

Inicio: 2026-11-13 01:01 CET. Rama `agent-A`. Unidades: `game/BattleTask`, `game/TacView`, `game/BattleCharaTask`.
Funciones al 0 % al empezar: BattleTask 51, TacView 6, BattleCharaTask 1.

## Resultado por unidad

- `game/BattleTask`:
  - `fn_800F29D0` (asigna el callback de nodos de un modelo, 100 %).
  - `fn_800FC1D0` (esfera de golpe mas cercana; miembro nuevo de `BattleTask`, renombrado en `symbols.txt`;
    100 % tras `pd.sh`).
  - `fn_800FBE70` (igual con radio y capsula de colision; 100 % tras `pd.sh`).
  - `fn_800FB1BC` (efecto, sonido, sub-habilidad y evento 15 de un golpe; 98 %).
  - `fn_800FBA9C` (esfera alcanzada por el arco cuerpo a cuerpo; 99,8 %).
  - `fn_800FB4B0` (explosion sobre jugadores y enemigos en el radio, sube la barra m38A04; 99 %).
  - `fn_800FD658` (rellena `m38A5C` con el punto y la direccion del golpe y llama a `fn_800FDF14`; 95 %).
  - `fn_800FDB20` (golpe de bala: popup de guardia o `m38A5C` con el nivel de la bala; 99,95 %).
  - `BattleTask::fn_80103600` (eleccion del objetivo por odio, 100 %) y `BattleTask::fn_80103B58` (el objetivo es
    valido, 100 %): miembros nuevos, renombrados en `symbols.txt`.
  - `BattleTask::fn_801055A8` (lineas de objetivo del frame segun `SetConfig::mBattleDrawHateLine`, 100 %).
  - `fn_801061BC` (subida de nivel de un estado: efecto, sonidos y popup; 99,95 %: solo el offset de "neck" en el
    pool de cadenas).
  - `fn_800FAB80` (prueba de impacto de una bala contra personajes y gimmicks, efectos, barra y anillo de depuracion
    `mShots`; 96,9 %). Con ella salen las copias debiles `GimmickCommon::vf58`/`vf44` (0x800FA4EC/0x800FA4F4, 100 %).

## NONMATCHING

- `fn_800FBA9C` 99,96 %: registro de la altura absoluta (f3/f4).
- `fn_800FDB20` 99,95 %: registro de la palabra del campo de bits del registro de dano (r3/r10).
- `fn_800FB4B0` 97,6 %: el original guarda las direcciones de posicion y rotacion del atacante en registros antes
  del efecto final (probado: inline con punteros).
- `fn_800FAB80` 96,9 %: registros float; el original limpia x/y de los vectores del registro (constructor) y llega
  al anillo por su direccion.
- `fn_800FD658` 91,4 %: huecos de pila de las matrices temporales de las rotaciones (orden inverso).
- `fn_800FB1BC` 98 %: huecos de pila de la esfera y la luz cambiados (probado: esfera con nombre, referencia
  a la luz), registros del bucle de oyentes.

## Decisiones tomadas sin el usuario

- `fn_800FBA9C` lee `HitInfo_unk::m50`/`m54` como floats con un cast sobre `m50[8]` (como game/Em202Spider);
  cambiar el tipo en `HitInfo_unk.h` rompe Em202Spider (otra cola). **Pendiente**: `float m50; float m54;`
  en la cabecera y el cambio en Em202Spider.
- `BattleTask.h`: `m38A1C` y `m38A38` (miembros de datos) y `fn_800FDF14` devuelve `int` (fn_800FD658 devuelve
  su resultado; el tipo de retorno no cambia el nombre).
- `fn_800FD658` guarda el atacante en `BattleCalcParam_unk::m00` como `int` (cast), como `fn_800FDE60`, cuyo
  nombre mangled lleva `int`.
- `fn_800FB4B0`, `fn_800FD658`, `fn_800FDB20`, `fn_800FAB80` siguen `extern "C"` (llamadores `extern "C"` en
  otras colas).
- `fn_800FBA30` y `fn_800FAB80` devuelven `int` (si se golpeo a alguien), no `void*`.
- `BattleTask.h`: `m38A18`, `BattleShot_unk`/`BattleShots_unk mShots` (0x38CD4, anillo de 32 registros) con
  `Push_unk` declarada sin nombre de parametro y definida inline en el .cpp (como los marcadores).
- `BattleDamage_unk` se sube antes de `fn_800FDB20` con la palabra `m1C` (4 bits altos que se limpian).
- `GimmickHitMsg_unk::m2C` recibe el puntero de la bala con cast (`unsigned char m2C[4]` en GimmickCommon.h,
  cabecera ajena).
- `BattleTask.h`: `int m36290[72]` (veces que se eligio cada entrada como objetivo); `fn_80103600` lee
  `BattleCharaTask::m1454` y llama a `BattleCharaTask::fn_800EEFDC` con cast del `CharaTask*` (la firma del
  miembro ya era `CharaTask*`); `CharaC58_unk::mC8` (int en CharaTask.h) se usa como `CharaTask*` con cast.
- `BattleTask.h`: `m38A00` (dibuja las lineas de todos los amigos). `fn_801055A8` recorre la lista de
  FriendManager con una vista local (`BattleFriendNode_unk`, como game/AiTarget.cpp).
- `fn_801061BC` lee la matriz del nodo "neck" como `atn::Matrix3x4` con cast para `GetTrans_unk`.
- `fn_800FDB20` usa `atn::VecAssign_unk` para la copia de `m28` en `m10` (cruce r3/r5).

- `fn_800FC1D0` como miembro de `BattleTask` (sin llamadores C++; game/Bullet lo llama solo desde asm),
  declarada en `BattleTask.h` sin nombres de parametro.
- `fn_800FBE70` y `fn_800FB1BC` siguen `extern "C"` con `BattleTask*` (Em403WarLion, Bullet, DamageCalc_unk
  las llaman asi; deuda de la regla 10 fuera de esta cola).
- `fn_800F29D0` escribe `ModelBase::mB8` con un cast a `NodeCallback_unk` (en `atn/Model_unk.h` es
  `boost::function<void()>`; cambiar el tipo toca cabeceras ajenas).
- `fn_800FBE70`: la capsula de `CharaTask::mColli` se copia como un `ColliCapsuleShape_unk` con cast (como
  `atn/atn_colli.cpp`); con el constructor de tres argumentos el radio se carga antes.
- `fn_800FB1BC`: la luz del mapa (`MapColor_unk`) se pasa como `EffectColor_unk` con cast (dos vistas del
  mismo color RGBA); `fn_8014F8F4` inline local con la estatica (como gmk/Item.cpp); includes nuevos
  `game/MapManager.h` y `atn/SoundManager.h` en BattleTask.cpp.

## Hallazgos

- `x = a >= 0 ? a : (b >= 0 ? b : -1)` da el `blt/b` del original donde el `if (a < 0) {...}` sale invertido
  (fn_80103600).

- Una funcion nueva con instrucciones desalineadas que cargan constantes distintas en la misma linea hace
  ambiguo el emparejado de `pairdata.py` (rep100 sube en toda la unidad): alinear esas cargas antes de `pd.sh`.

- Un `if` de exclusion con su propia variable de flags en un inline `bool` (`IsSkipped_unk`) da el orden de
  registros del original donde el mismo codigo en linea los cambia (fn_800FC1D0/fn_800FBE70).

## Pendientes

- `fn_800FA4FC`/`fn_800FA9F4` (std::sort y mediana de tres, elementos de 0xC bytes por el float de +8) solo
  se emiten al escribir su llamador `fn_800F8C6C` (6 KB).
