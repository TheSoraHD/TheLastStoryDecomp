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
- `AIManager::fn_80017128` (0x80017128, 2076 bytes): **escrita, NONMATCHING 89,9 %** (antes 0 %). Firma
  `int fn_80017128(CharaTask*)` (devuelve 0 sin escena o sin datos del personaje, 1 si no). Paso del script de IA:
  rellena el bloque de entrada (`fn_8001C260`), ejecuta el script de `CharaRes_unk::m08` (`fn_8001C1C0`) y lee el de
  salida (`fn_8001C26C`): estado, parte en mano, valores guardados. Resto: el original indexa los valores guardados
  desde el puntero a `CharaC58_unk` (en CharaTask.h `m140`/`m1A0` son bytes) y desenrolla distinto. Sale con ella
  la copia debil `vf5C__13GimmickCommonFv` (+1 funcion al 100 %: 15390).
- `BattleTask::fn_80103FD4` (0x80103FD4, 5436 bytes): **escrita, NONMATCHING 98,3 %** (antes 0 %). Firma
  `void fn_80103FD4()`. Odio de cada entrada de mCharas hacia las demas: limpia las que no pueden actuar
  (`m08[idx]` de todas, marcador conservando el bit 1, `m808 = 60` si esta agarrado); en las demas el temporizador
  "IgnoreLongHate" (m6DC/m6E0/m6E4), distancias (m24C), odio por distancia con el tiempo de sombra (m36C, m6CC
  aleatorio), bonus del personaje enlazado, de los atacantes a distancia (m48C/m5AC) y de las ordenes de la IA
  (m80C: 2000, (n+1)*200, 70/35, 175, 3000), objetivo (`fn_8017A5D8`, `fn_8012F188` con `mExplsnAddHateAtt`,
  `m930 = target->mFD8`) y visibilidad m248. Resto: registros de los indicadores de CanAct_unk al principio del
  bucle, recarga de mCharaCount en FindChara_unk y orden de las cargas del personaje enlazado.
- `BattleCharaTask::fn_800E4278` (5668): sin escribir (sin tiempo).

## Cambios de firma o de layout (para el port)

- `BattleChara_unk::m04`: `int` -> `CharaTask*` (rival mas cercano; ya lo era en el port).
- `BattleTask::mTargetCount`/`mTargets[64]` -> `BattleTargets_unk mTargets` (0x48, `{mSize, mData[64]}` con
  `size()`, `clear()`, `push_back()` inline en BattleTask.cpp). No es `atn::FixedArray_unk` porque instanciar la
  plantilla en la cabecera renumera los `@N` de las ~57 unidades que incluyen BattleTask.h. Los bucles de
  fn_800FFE68/fn_801002DC usan `mTargets.mData[i]` (con `operator[]` pierden la induccion de puntero).
- `BattleTask::mCBAC` pasa a `unsigned int`, `mCBB0` a `BattleTimer_unk mCBB0[8]` (`{float m00; int m04;}`).
- `BattleTask::m363B4` -> `int m363B4; int m363B8;` (contador de fn_80103FD4), `BattleChara_unk::m930` ->
  `unsigned int` (se compara sin signo con `CharaTask::m138C`/`mFD8`; esos dos se comparan con cast porque
  CharaTask.h es de otra familia: candidatos a `unsigned int`).
- Vista local `HateConfig_unk` (BattleTask.cpp): `mPlayerMinusHate`, `mIgnoreLongHate`,
  `mIgnoreLongHateValue[10]`, `mIgnoreLongHateRand[10]` (como GameConfig_unk desde 0x350).
- `BattleTask::mCB5C` -> `float` (alfa de la flecha), `mCB64` -> `atn::Vector3` (direccion suavizada).
- `BattleShots_unk`: iterador (`iterator`, `begin`, `end`, `operator[]`, `erase`) declarado en la cabecera sin
  nombres de parametro y definido inline en BattleTask.cpp.
- Nuevas inline de BattleTask (declaradas sin nombres): `UpdateAlert_unk(CharaTask*, PlayerTask*)`,
  `GetHated_unk(CharaTask*)`, `BetoModel::SetMtx_unk(const atn::Matrix3x4&)`.
- `symbols.txt`: `fn_800F3EF4` -> `vf10__10BattleTaskFv`, `fn_800F5314` -> `vf14__10BattleTaskFv`, y etiquetas
  `@N` de BattleTask re-emparejadas.

- `AiTarget.h`: `AIManager::m9C` pasa de `unsigned char[0x40]` a `float m9C[16]` (valores guardados entre pasos
  del script). Tipos nuevos `AiScriptInput_unk` (0x180) y `AiScriptOutput_unk` (0xF0) con arrays; son el mismo
  objeto que `AiInput_unk`/`AiOutput_unk` de `AI/AI_archer_attack01.cpp` (otra cola): pendiente unificar.

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
- MWCC no pliega `15000.0f - 12000.0f` si vienen de locales, y no elimina `x * rate` con `float rate = 1.0f`: el
  original los tenia asi (fn_80103FD4).
- Inicializar los locales al principio del bucle exterior (target/guard) aunque solo se usen en una rama coloca
  los `li rX, 0` como el original (fn_80103FD4, +0,5 %).
- `pairdata.py --relabel` da parejas ambiguas si la unidad tiene funciones grandes NONMATCHING: filtrar por
  `match_percent >= 99`.

## Pendientes

- `BattleCharaTask::fn_800E4278` (5,6 KB): sin empezar.
- `HitInfo_unk::m50/m54` como floats (ver arriba).
- `CharaC58_unk::m140` -> `float[24]`, `m1A0` -> `int[4]` y `CharaRes_unk::m64-m84` como 9 floats en
  `game/CharaTask.h` (cabecera de otra familia, muy incluida): con eso fn_80017128 deberia acercarse al 100 %.
- `AI/AI_archer_attack01.cpp` deberia usar `AiScriptInput_unk`/`AiScriptOutput_unk` de `game/AiTarget.h`
  (regla 10: un objeto, un tipo).
- Tras `pd.sh game/AiTarget` hay que devolver `lbl_8087D6E8`/`lbl_8087D6EC` (los empareja con las cadenas
  `@STRING@Resize_unk` de `SimpleArray<int>` y el enlace falla).
