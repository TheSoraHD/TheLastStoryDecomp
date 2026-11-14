# port-decomp-20261114 - cola H (combate, segunda pasada)

Inicio: 2026-11-14 04:41 CET. Rama `agent-H`. Unidades: `game/BattleCharaTask`, `game/BattleTask`, `game/AiTarget`.

## Resultado por funcion

- `BattleCharaTask::fn_800E4278` (0x800E4278, 5668 bytes): **escrita, NONMATCHING 99,5 %** (antes 0 %, sin version
  funcional en el port). Firma final `void BattleCharaTask::fn_800E4278(GameTask*)` (antes `extern "C"
  fn_800E4278(PlayerTask*, GameTask*)` en GameTask.cpp). Entrada del lider: bloqueo (`Input_unk::fn_80122330`),
  objetivo mFC4, acciones de cobertura (lado `fn_80153B44`, salto/ataque desde la pared con `fn_8010089C`),
  gimmicks (`GameTask::fn_8036E098`) y rutas (`fn_8036DAF4`) a mano, enemigo agarrable (`fn_8036DDD4`: 0x2711/0x4FBF
  y el texto "hips" del popup, o items 400), pista del HUD (`GameUI::m764`, `m7C0` = 0xD / 0x11, `m728`) y la entrada
  del arma (`fn_800E62AC`/`fn_800E7650` con `GameUI::m748.m00`/`m748.m14`). Resto: r0/r3 permutados en las
  comprobaciones del objetivo, la entrada 0x11 recarga `lbl_8087F0A8` (aqui usa `config`), orden de stores del
  vector al objetivo, cargas de `m7FC`/`m800` de la camara y la recarga de la palabra de flags antes de `mFlag5 = 0`.
- `Effect_unk::fn_80109950` (0x80109950, 1580 bytes, unidad BattleTask): **escrita, NONMATCHING 97,5 %** (antes 0 %).
  Firma `atn::Vector3 Effect_unk::fn_80109950(CharaTask*)` (sin cambio; `this` no se lee). Puesto de `chara` en el
  anillo de su lider (`mC58.mC4`, `m125C` = {angulo, personaje}): busca el mas cercano (o el de tipo 0 / tipo 2
  con `m50 < 100`), reparte la vuelta completa por los anchos `vfA0` y recorre el anillo hacia un lado y el otro hasta
  `chara`; devuelve el centro mas `(0, 0, m64->m30)` girado, con la y de `chara`, o `mPos` si no esta. Resto:
  asignacion de registros (centro/anillo, punteros de los bucles, floats `best`/`offset`).
- `BattleTask::fn_800FDF14` (0x800FDF14, 8020 bytes): **escrita, NONMATCHING 92,3 %** (antes 0 %). Firma `int
  fn_800FDF14(BattleCalcParam_unk*)` (sin cambio; devuelve 0 sin objetivo/datos, con el objetivo intocable 0x2000
  sin el bit 0x20000 del bullet o si `fn_8010B180` falla, y 1 si se aplica). Aplica el golpe `m38A5C`: espalda
  (m0C 0x10, coseno de 120 grados), tiro a la cabeza (nodo "head", radio `m5B4 * 0.25`, salvo los seis jefes
  `em002_bs00_03`...`np131_bs00_18`), robo del objeto del objetivo (estado 0x3C, `Inventory::mDB10`), criticos
  (`fn_80134168(4)` + `mParamB.m18`, x1,5), estados 0x13/0x18/0x1A/0x23, `vf3C`, drenajes (bit 8 del bullet:
  la mitad; `mHateAbsorbHpRate` con el texto "hpUpNum"/"hpUpSubNum"/"hpRate" de `m3E2F4`; 0x23; 7/8/9/0x47 a la
  lista de 16), popup de dano, efecto y sonido, odio (`fn_80102EAC`, valores 100/200/20/10), tension
  (`fn_8012F034`, tabla por tipo de personaje con el jumptable), la arena y la moral `m38A04`. Resto: el original
  usa el ancla de `.rodata` (`lbl_807355C8`, r31) porque `lbl_80735DD0` aun es `extern` (deuda de la regla 10),
  `vf3C` recibe un segundo argumento (1) que `CharaTask.h` no declara, y 0x100 bytes de pila / registros.

- Sin escribir: el constructor y el destructor de `BattleTask` (0x800F1430, 0x800F1F94; con ellos saldrian las
  copias debiles `__dt__12HomingBullet` y `__dt__Q210BattleTask9BetoModel`): necesitan tipar los `atn::EffFile` /
  `atn::Texture` que `BattleTask.h` guarda como bytes, y eso renumera los `@N` de todas las unidades que incluyen la
  cabecera (ver el comentario de `BattleMarkers_unk`); el orden de destruccion no se puede imitar a mano.
  `fn_800F8C6C` (6 KB, con `fn_800FA4FC`/`fn_800FA9F4` de `std::sort`) usa `std::vector<{CharaTask*, int, float}>`
  con el `push_back` de `std/vector.h` que sigue pendiente (docs/SIGUIENTE_sesion.md). `vf2C__Q23chr11ControlBaseFv`
  (8 bytes) sale con alguna de ellas.
- No tocadas: las NONMATCHING de la cola B (`vf14` 85 %, `AIManager::fn_80017128` 89,9 %, `vf10` 98,5 %,
  `fn_80103FD4` 98,3 %): no dio tiempo tras las tres funciones nuevas.

## Cambios de firma / layout (para el port)

- `BattleCharaTask::fn_800E4278(GameTask*)` nuevo miembro en `include/game/BattleCharaTask.h`; `GameTask::fn_8036CC90`
  llama `leader->fn_800E4278(this)` (antes `extern "C"`).
- `BattleCharaTask::fn_800E7650(Field_unk*, int, int)`: el original pasa `a`/`b` (r5/r6) tambien a esta funcion
  (como a `fn_800E62AC`); la funcion no los usa. Simbolo renombrado a `fn_800E7650__15BattleCharaTaskFP9Field_unkii`.
- `fn_80108C10` declarado en BattleCharaTask.cpp con `const atn::Vector3&` (como en Em203GreaterDaemon), no puntero.

## Decisiones y hallazgos

- `BattleTask::m3E2F8` pasa de `unsigned char[4]` a `int` (ultimo valor de recuperacion mostrado; el port lo
  pone a 0 con memset, sigue valiendo). Sin nombres nuevos en cabeceras: labelcheck sin cambios en otras unidades.
- `BattleMsg_unk`/`lbl_8087F1E4` y `BattleItem_unk`/`fn_80211480` suben en BattleTask.cpp por encima de
  `fn_800FDF14` (solo se movieron las definiciones). Nuevas: `HitDamage_unk` (BattleDamage_unk con constructor),
  `SetHitFlag_unk`/`ClearHitFlag_unk`, `IsNoHead_unk`, `IsSpecialHit_unk`, `HitPos_unk` y la tabla `sNoHead_unk`.
- Pendientes de cabeceras ajenas: `CharaTask::vf3C` tiene dos argumentos (`(param, 1)` en fn_800FDF14);
  `lbl_80735DD0` (sonidos) deberia definirse en BattleTask.cpp.
- Re-emparejado de BattleTask tras fn_800FDF14: deltas +1468 / +2100 (y @13868 -> @14031 por las cabeceras nuevas
  GameVoice.h/ArenaTask.h); labelcheck "missing 2" como la referencia.

- `Effect_unk` se define como vista minima en BattleTask.cpp solo para `fn_80109950` (CharaTask.cpp y Em315Coffin
  la llaman con ese nombre mangled); pendiente unificar con `BattleTask` cuando se toquen esas unidades.
- Re-emparejado de BattleTask: `pd.sh` (pairdata --relabel) rompe ~45 funciones (rep100 +45). Con pairdata99 hay que
  iterar y aun quedan 32 etiquetas de funciones NONMATCHING: el desplazamiento de los `@N` es constante (+328, y
  +427 a partir de ~@33000), aplicado a mano por direccion (labelcheck vuelve a "missing 2" como la referencia).

- Un parametro con nombre en la cabecera consume un `@N` en todas las unidades que la incluyen: se deja el nombre
  `field` en `fn_800E7650` para no mover las etiquetas de ~40 unidades; la nueva declaracion va sin nombre.
- `@28783` -> `@28781` (0x808857A8) en symbols.txt a mano tras quitar la declaracion `extern "C"` con dos
  parametros con nombre de GameTask.cpp (pd.sh no la re-emparejo).
- precommit: `labelcheck/rep100: peor` solo por `fn_800EBD00` (report 99,97 -> 100, objdiff 99,95: la etiqueta global
  `lbl_808812D8` frente al `@N` local del 1.0f; el valor del pool ya coincide). No es una bajada; se acepta.
- Temporales de Vector3 en inlines (`Dot_unk`, `Cross_unk`, `Neg_unk`, `enemy->mPos + Vector3(...)` por referencia)
  colocan los huecos de pila detras de los locales con nombre, como el original.
- `bool b = IsX(); int v = 4; if (b) v = 2;` reproduce `cntlzw/srwi.` (bool materializado).
- Bucle sobre `FixedArray_unk`: `items.mData[i]` da base `&items` + 4 con puntero incrementado; `items[i]` (int) no.

## Resumen

Funciones al 0 % escritas: 3 (`fn_800E4278` 0 -> 99,5 %, `fn_80109950` 0 -> 97,5 %, `fn_800FDF14` 0 -> 92,3 %).
Funciones al 100 %: +1, `BattleCharaTask::vf2C` (0x800EBF1C, 99,87 -> 100 %: el pool `@stringBase0` ya tiene
"hips" de fn_800E4278; quitado su NONMATCHING). report.json 15392 -> 15394 (la otra es `fn_800EBD00`, 100 en el
report con objdiff 99,95 %). lost 0, sin bajadas de fuzzy.
precommit: `labelcheck/rep100: peor` solo por `fn_800EBD00` (ver arriba).
