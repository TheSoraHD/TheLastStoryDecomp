# port-decomp-20261114 - cola B (Combate)

Inicio: 2026-11-14 02:59 CET. Rama `agent-B`. Unidades: `game/BattleTask`, `game/BattleCharaTask`, `game/AiTarget`.

## Resultado por funcion

- `BattleTask::vf10` (0x800F3EF4, 5116 bytes): **escrita, NONMATCHING 98,5 %** (antes 0 %). Firma `void vf10()`.
  Resto: registros del bucle del anillo de disparos (el original recarga el iterador de la pila), registro base
  del modelo de mSlots (r29) y huecos de pila de los temporales de las columnas.
  Correcciones frente a la version del port: el enemigo entra en `mTargets` cuando `fn_8016F3D0(leader, 1)` es
  **0**; los bucles de amigos descartan al personaje cuando `fn_8016F368(type)` es **distinto de 0**; el
  indicador `mE0_12` vale `!leaderOn && !aiOn && !gaugeOn` y se limpia si `m098 == 0` o si `fn_8016E484()` es 0;
  la moral baja solo si **queda** algun enemigo que puede actuar (`MAX(m38A04 - m38A10 * rate, 0)`);
  los temporizadores de mCBB0 se compactan en su sitio (sin copia local).
- `BattleTask::vf14` (0x800F5314, 7996 bytes): **escrita, NONMATCHING 85 %** (antes 0 %). Firma `void vf14()`.
  Resto: asignacion de registros de los bucles de depuracion y 0x100 bytes de pila. Correcciones frente al port:
  en el modo 3 de "DrawHateInfo" se cuentan/dibujan los personajes con `fn_8016F3D0(chara, 1)` **distinto de 0**;
  `fn_8010C948` va inline (nueva inline `GetHated_unk`); el texto de rango y el de "HateAtt" son `char[256]`, el de
  los disparos `wchar_t[256]`; `57.295776f` (0x42652EE0).
- `BattleTask::fn_80103FD4` (5436), `BattleCharaTask::fn_800E4278` (5668), `AIManager::fn_80017128` (2076): sin
  escribir (ver pendientes).

## Cambios de firma o de layout (para el port)

- `BattleChara_unk::m04`: `int` -> `CharaTask*` (rival mas cercano; ya lo era en el port).
- `BattleTask::mTargetCount`/`mTargets[64]` -> `BattleTargets_unk mTargets` (0x48, `{mSize, mData[64]}` con
  `size()`, `clear()`, `push_back()` inline en BattleTask.cpp). No es `atn::FixedArray_unk` porque instanciar la
  plantilla en la cabecera renumera los `@N` de las ~57 unidades que incluyen BattleTask.h. Los bucles de
  fn_800FFE68/fn_801002DC usan `mTargets.mData[i]` (con `operator[]` pierden la induccion de puntero).
- `BattleTask::mCBAC` pasa a `unsigned int`, `mCBB0` a `BattleTimer_unk mCBB0[8]` (`{float m00; int m04;}`).
- `BattleTask::mCB5C` -> `float` (alfa de la flecha), `mCB64` -> `atn::Vector3` (direccion suavizada).
- `BattleShots_unk`: iterador (`iterator`, `begin`, `end`, `operator[]`, `erase`) declarado en la cabecera sin
  nombres de parametro y definido inline en BattleTask.cpp.
- Nuevas inline de BattleTask (declaradas sin nombres): `UpdateAlert_unk(CharaTask*, PlayerTask*)`,
  `GetHated_unk(CharaTask*)`, `BetoModel::SetMtx_unk(const atn::Matrix3x4&)`.
- `symbols.txt`: `fn_800F3EF4` -> `vf10__10BattleTaskFv`, `fn_800F5314` -> `vf14__10BattleTaskFv`, y etiquetas
  `@N` de BattleTask re-emparejadas.

## Decisiones tomadas sin el usuario

- `HitInfo_unk::m50` se queda como bytes (game/Em202Spider, de otra cola, lo lee con `*(float*)&m50[0]`);
  BattleTask usa la misma vista. Pendiente: `float m50; float m54;` en `HitInfo_unk.h` y cambiar Em202Spider.
- El re-emparejado de etiquetas se hizo con pairdata limitado a funciones >= 99 % (las NONMATCHING grandes
  daban parejas ambiguas) y un renombrado a mano de `@26501` -> `@27401` (50.0f).

## Hallazgos

- `CanAct_unk()` inline sobre un `EnemyTask*`/`NpcTask*` sale con los booleanos plegados en saltos; sobre un
  `CharaTask*` (o `PlayerTask*`) sale materializado (`li rX, 0`/`li rX, 1`) como en el original: declarar la
  variable del bucle como `CharaTask*` (vf10, vf14).
- Los locales de una inline se colocan en pila despues de los del llamador: un `Vector3` que en el original
  queda por debajo de los de bucles posteriores indica una inline (UpdateAlert_unk en vf10).
- Instanciar `atn::FixedArray_unk<T, N>` como miembro en una cabecera compartida renumera los `@N` de las
  unidades que la incluyen (los parametros con nombre de la plantilla).
- `pairdata.py --relabel` da parejas ambiguas si la unidad tiene funciones grandes NONMATCHING: filtrar por
  `match_percent >= 99`.

## Pendientes

- `fn_80103FD4` (odio por personaje, 5,4 KB, 18 registros de coma flotante; lee mas campos del bloque "Hate" de
  GameConfig desde 0x350 de los que tiene `HateConfig_unk`: +0x28, +0x2C + 4 * tipo, +0x54 + 4 * tipo).
- `HitInfo_unk::m50/m54` como floats (ver arriba).
