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
- `BattleTask::fn_80103FD4` (5436) y `BattleCharaTask::fn_800E4278` (5668): sin escribir (ver pendientes).

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
- `pairdata.py --relabel` da parejas ambiguas si la unidad tiene funciones grandes NONMATCHING: filtrar por
  `match_percent >= 99`.

## Pendientes

- `fn_80103FD4` (odio por personaje, 5,4 KB, 18 registros de coma flotante): asm leido entero, sin escribir.
  Esquema: `HateConfig_unk` (vista local de GameConfig desde 0x350) necesita `mPlayerMinusHate` (+0x24),
  `mIgnoreLongHate` (+0x28), `mIgnoreLongHateValue[10]` (+0x2C) y `mIgnoreLongHateRand[10]` (+0x54), indexados por
  `(signed char)chara->m5C->m121`; `fn_80210220(cfg->mParamIndex)` sin usar el resultado; `m363B4` es un contador
  (`int`, decrementa si > 0). Bucle por `mCharas[i]` con `mMarkers.mItems[i]`: si el personaje no puede actuar
  (CanAct o flag 0x2000 de m54C) limpia `mCharas[k].m08[idx]` (k < 72), resetea la entrada si `m92F`, limpia el
  marcador conservando el bit 1 de `mFlags`, `m808 = 60` si esta agarrado y `fn_8017A5D8(0)`; si puede, recorre los
  rivales (`m6E4[j]` con m6DC/m6E0, distancias en `m24C[j]` con PSVECMag, `m36C[j]` del odio con fn_80164EF4,
  `mF8`/`m48C`/`m5AC`/`m80C[j]` con los bonus 0x7D0, (n+1)*200, 70/35, 0xAF, 3000), elige objetivo (r30) y
  `m930 = target->mFD8`; al final ajusta `m248` con 1/15, 1/75 segun tipo y estado. Constantes (sdata2):
  0, 1, 0.8, 1000, 2000, 240, 60, 12000, 15000, 0.05, 1/300, 32767, 70, 1.5, 50, 100, 25, 0.4, 0.7, 10, 0.5.
- `BattleCharaTask::fn_800E4278` (5,6 KB): sin empezar.
- `HitInfo_unk::m50/m54` como floats (ver arriba).
- `CharaC58_unk::m140` -> `float[24]`, `m1A0` -> `int[4]` y `CharaRes_unk::m64-m84` como 9 floats en
  `game/CharaTask.h` (cabecera de otra familia, muy incluida): con eso fn_80017128 deberia acercarse al 100 %.
- `AI/AI_archer_attack01.cpp` deberia usar `AiScriptInput_unk`/`AiScriptOutput_unk` de `game/AiTarget.h`
  (regla 10: un objeto, un tipo).
- Tras `pd.sh game/AiTarget` hay que devolver `lbl_8087D6E8`/`lbl_8087D6EC` (los empareja con las cadenas
  `@STRING@Resize_unk` de `SimpleArray<int>` y el enlace falla).
