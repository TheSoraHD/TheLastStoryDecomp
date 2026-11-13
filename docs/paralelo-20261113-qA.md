# Lote paralelo 2026-11-13, cola A (Batalla)

Inicio: 2026-11-13 01:01 CET. Rama `agent-A`. Unidades: `game/BattleTask`, `game/TacView`, `game/BattleCharaTask`.
Funciones al 0 % al empezar: BattleTask 51, TacView 6, BattleCharaTask 1.

## Resultado por unidad

- `game/BattleTask`:
  - `fn_800F29D0` (asigna el callback de nodos de un modelo, 100 %).
  - `fn_800FC1D0` (esfera de golpe mas cercana; miembro nuevo de `BattleTask`, renombrado en `symbols.txt`;
    99,97 %: solo la reubicacion del `1e16f` de `.sdata2`).
  - `fn_800FBE70` (igual con radio y capsula de colision; 99,9 %: solo constantes de `.sdata2`).
  - `fn_800FB1BC` (efecto, sonido, sub-habilidad y evento 15 de un golpe; 98 %).

## NONMATCHING

- `fn_800FC1D0`, `fn_800FBE70`: solo reubicaciones de constantes de `.sdata2` (pool del fichero incompleto).
- `fn_800FB1BC` 98 %: huecos de pila de la esfera y la luz cambiados (probado: esfera con nombre, referencia
  a la luz), registros del bucle de oyentes.

## Decisiones tomadas sin el usuario

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

- Un `if` de exclusion con su propia variable de flags en un inline `bool` (`IsSkipped_unk`) da el orden de
  registros del original donde el mismo codigo en linea los cambia (fn_800FC1D0/fn_800FBE70).

## Pendientes

- `fn_800FA4FC`/`fn_800FA9F4` (std::sort y mediana de tres, elementos de 0xC bytes por el float de +8) solo
  se emiten al escribir su llamador `fn_800F8C6C` (6 KB).
