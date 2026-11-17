# escribir-20261116, cola G3 (atn/PacketPack, tercera pasada)

Inicio: 2026-11-17 04:05 CET (tope 06:35). Fin: 05:50.

Commits: 4e58b5b, 5d0cb60, 5571ff3, 0b7d5ee, f046aff, e4d9076, caaa94a (notas), f62e555 y el de estas notas (precommit OK en los dos primeros;
desde 5571ff3 el unico aviso es el fuzzdrop de fn_804EE040 99,54 -> 99,21, ver Decisiones; lost 0, labelcheck y
rep100 igual, poolcheck solo los floats conocidos de las copias debiles de CharaTask, ver Hallazgos).

## Tabla de la cola

| funcion | antes | despues |
|---|---|---|
| fn_804DE884 | 0 % (parche de qG, 96,3 %) | equivalente NONMATCHING 99,58 % (sin tocar CharaTask.h) |
| fn_804E4EB0 | sin escribir | equivalente NONMATCHING 98,80 % |
| fn_804EFF74 | sin escribir | equivalente NONMATCHING 99,12 % |
| fn_804EB934 | sin escribir | equivalente NONMATCHING 96,78 % |
| vf10 de Manager2 | DRAFT 95,05 % | equivalente NONMATCHING 95,16 % (comprobada; 5 errores corregidos) |
| payloads pack(1) | bytes sueltos | 0x1014/0x101A/0x101B, 0x1035 y fn_804F47FC con structs empaquetados |
| fn_804F3EEC (de rebote) | 75,99 % | 93,87 % (fn_804DB1C4 se inlinea otra vez) |
| fn_804F47FC (de rebote) | 98,25 % | 98,80 % |

PacketPack sin escribir 4 -> 0 en la cola; DRAFT 1 -> 0. Funciones al 100 %: sin cambios (293/380).

## Para el port

- fn_804DE884 escrito (antes sin cuerpo): arranque de la partida (GameTask del modo, resultados, jugadores de la
  CPU, personajes `MP::Chara<EnemyTask>`, popups, efectos y grupos de sonido). Cambio de comportamiento.
  `MP::Chara<T>` se define antes (con su ctor inline y `Create_unk`); `fn_80365708` declarada como la de
  game/GameTask (devuelve `atn::Task*`, antes `extern "C"` con `GameTask*`).
- fn_804E4EB0 escrito (antes sin cuerpo): cuenta atras de los derribados (mD38[4]), mensajes 0x103B/0x103A/0x1002,
  puntos, revivir (golpe 0x2711, punto de aparicion) o derrota del jugador humano en el modo 2. Cambio de
  comportamiento. `IsSameTeam_unk` devuelve `int` (antes `bool`; mismo codigo en sus usos).
- fn_804EFF74 escrito (antes sin cuerpo): mensajes 10/11 (golpe calculado en esta maquina). **Firma**
  `fn_804EFF74(PPPlayer_unk*, unsigned int type)` (antes sin `type`); fn_804EE040 le pasa `type`.
  `fn_804F1060` con `__declspec(noinline)` (el original la llama en cada vuelta). Copias locales de
  `HitCenter_unk` y `MakeRotZYX_unk` (las de game/BattleTask.cpp; pendiente: llevarlas a una cabecera).
- fn_804EB934 escrito (antes sin cuerpo): listener de los personajes (eventos 5-15 -> mensajes 0x1021-0x1049).
  Cambio de comportamiento. Tabla nueva `lbl_80790FF0[10]` (VO_ARCOM_010..090) antes de `lbl_80791018`.
  `HitEvent_unk` copiada de game/DamageCalc_unk.cpp (pendiente: cabecera comun).
- fn_804F5A98 devuelve `bool` (antes `int`; mismo codigo).
- fn_804DB1C4: el popup recibe `&chara->vf58()` (direccion del temporal) en vez de un local con nombre; mismo
  comportamiento (ver Hallazgos).
- vf10 de Manager2 (cambios de comportamiento, eran errores): `D2AMpMenu::fn_804AF520(2)` (antes 4); tiempos del
  lobby y del resultado `/ 30` (antes `/ 60`); el equipo propio sale de `mD4Flags.m22` (antes se copiaba la palabra
  `mD4` entera en el bitfield); una sola llamada a `fn_8013C504`; `fn_804C2268` como metodo de D2AMpMenuLobby (antes
  `extern "C"` con el lobby de argumento).
- Payloads empaquetados (`#pragma pack(push, 1)` con `// inferred:`): `PPMsgCharaFull_unk` {PackedChara_unk, indice}
  y `PPMsgSpecialFull_unk` {PackedChara_unk, indice, SpecialState_unk} (antes `unsigned char m00[0x15/0x25]`);
  `PPMsgAnswer_unk` {m00; union {PPAnswerGimmick_unk mGimmick; mTeam; mTime}} sustituye a `PPMsgTime_unk`
  (bytes) y a `PPMsgGimmickAnswer_unk` (`out->mGimmick.mId/mKind`). fn_804F47FC lee `PPMsgRoom_unk` (antes
  desplazamientos de bytes). Structs nuevos sin pragma (tamanos con el N de `NetMsgI_unk`): `PPMsgDown_unk`,
  `PPMsgAttack_unk`, `PPMsgCalc_unk` (15 B), `PPMsgValue_unk` (5 B), `PPMsgShotTarget_unk`, `PPEventShot_unk`,
  `PPEventShotTarget_unk`, `PPEventDamage_unk`; `PPMsgShot_unk`, `PPMsgHit_unk`, `PPMsgDamage_unk`,
  `PPMsgCharaRef_unk` y `PPMsgGimmick_unk` movidos antes (mismo layout).
- Literales: 0.7f (musica 0x53D), 120 s del limite del modo 2, 150 frames de invencibilidad, efectos 0x4FC1 y
  0x2711, 10.0f/0.1f/2.0f/100.0f de fn_804EFF74, 90/-1 del odio, voces VO_ARCOM.

## Decisiones tomadas sin el usuario

- Punto 1 de la cola: **no** se sacan `vfE0`/`vfA0`/`vf70` de game/CharaTask.h. El original las tiene inline
  (copias debiles en game/BattleCharaTask, 0x800E8FA4/0x800EF080/0x800EF368, y la vtable de MP::Chara en PacketPack
  apunta a ellas); sus floats en nuestra `.sdata2` los quita el enlazador (deadstrip de la copia debil
  descartada): Em209BigGolem enlaza con `main.dol: OK` y tiene los mismos avisos de poolcheck (0x3C B de `.sdata2`
  en el .o frente a 0x20 en el split). El aviso de poolcheck en PacketPack es un falso positivo.
- fn_804EE040 99,54 -> 99,21 %: pasa a fn_804EFF74 el tipo del mensaje (el original lo pasa en r4; sin el argumento
  la funcion no seria equivalente). Probados `(unsigned char)type` en uno o en los cuatro manejadores y un local
  `unsigned int`: peor (98,5-98,9 %). Commits 5571ff3 en adelante con ese unico fuzzdrop.
- fn_804DB1C4 pasa `&player->mRef.mChara->vf58()` (direccion de un temporal, MWCC lo acepta): con el local con
  nombre MWCC no la auto-inlinea; el original si (su `L""` sigue en `@wstringBase0`, asi que no era inline).
- Cadenas VO_ARCOM: `fixlbl.py` propio (scratch-G3/bin) que reconoce la tira de 45 cadenas de 13 B y las dos tablas
  de saltos nuevas (0x807912B0 de fn_804EB934 y 0x807912DC de fn_804EE040).

## Hallazgos

- Una funcion normal que el original auto-inlinea y la nuestra llama: un local de clase con nombre (aqui el
  `atn::Vector3` devuelto por una virtual) puede impedir el auto-inline; pasar la direccion del temporal
  (`f(&obj->vf58())`) lo arregla sin tocar la funcion (PacketPack fn_804DB1C4 en fn_804E4EB0 85 -> 98,8 %,
  fn_804F3EEC 75 -> 93,9 %) (escribir-20261116 qG3)
- Los floats de copias debiles de virtuales inline descartadas por el enlazador desaparecen del `.dol` (deadstrip):
  un poolcheck con esos floats de mas no impide enlazar (Em209BigGolem: 0x3C B de `.sdata2` en el .o, 0x20 en el
  split, `main.dol: OK`) (escribir-20261116 qG3)
- MWCC compara con `cmplwi` el resultado de una funcion `bool` (`== 1`) y con `cmpwi` uno `int`: el tipo de retorno
  real se ve en el llamador (PacketPack fn_804F5A98, IsSameTeam_unk) (escribir-20261116 qG3)
- Para comprobar una funcion grande "casi igual": diff de mnemonicos normalizados (registros y pila fuera, offsets de
  campos y constantes dentro) mas diff de la secuencia de `bl`: salen los errores reales (constantes, bitfields,
  llamadas de mas) entre el ruido de registros (`/home/claude/scratch-G3/bin_sdiff.py`; vf10 de Manager2)
  (escribir-20261116 qG3)

## Bloqueos

- PacketPack fn_804EE040: pasar `type` a fn_804EFF74 (como el original) rota registros (99,54 -> 99,21 %); con
  `(unsigned char)type` (el `clrlwi r4` del original) peor (escribir-20261116 qG3)
- PacketPack fn_804DB1C4: el original lee `mDCScore` dos veces (suma y valor anterior); leer la suma por
  `mDC.mChara` sube la funcion (98,87 %) pero baja fn_804E4EB0 (98,1 %) (escribir-20261116 qG3)
- PacketPack fn_804EB934 y fn_804F3EEC: `points >>= 1` de la constante 10 (`li 10; srawi`) que el original no pliega
  (escribir-20261116 qG3)

## Cambios de splits/symbols

- Re-etiquetado `@N` (pd.sh) y por direccion: 0x807596F0, 0x80759810.. (10 VO_ARCOM, antes `lbl_80759810..`),
  0x807598B0.. (35 VO_AR), `.bss` 0x807C8AE8/AF4/B00, tablas de saltos 0x807911B4, 0x8079120C, 0x807912B0 (antes
  `jumptable_807912B0`) y 0x807912DC (antes `jumptable_807912DC`); `lbl_808875FC`/`lbl_80887600` -> `@N` (floats
  del ctor de MP::Chara). Splits sin cambios.

## Scripts

- `/home/claude/scratch-G3/bin/fixlbl.py` (desde la copia, tras `pd.sh atn/PacketPack`): el de G2 mas las 10
  VO_ARCOM y las tablas 0x807912B0/0x807912DC. Sustituye al de /home/claude/agentes/bin.
- `/home/claude/scratch-G3/bin/sdiff.py L.s R.s` (salidas de `ours.py ... left/right`): diff de mnemonicos
  normalizados para buscar diferencias reales en funciones grandes.
- `/home/claude/scratch-G3/bin/suspect.py <unidad> <simbolos...>`: lista las diferencias con constantes o llamadas
  distintas (barrido rapido de errores reales; ruidoso con funciones de registros).

## Pendientes

- `HitCenter_unk`/`MakeRotZYX_unk` (game/BattleTask.cpp y PacketPack) y `HitEvent_unk` (game/DamageCalc_unk.cpp y
  PacketPack) a una cabecera comun.
- fn_804EE040: recuperar el 99,54 % pasando el tipo.
- vf10: los tres bloques de -1 en 0x99F4 (punteros que avanzan) y la cola comun 60/80 (registros).
- fn_804EFF74/fn_804EB934: las entradas de estado se copian por la base de CharaStatus (0x7D4) en el original;
  probado un push a mano por `GetStatus_unk()` con la comprobacion por `mStatus`: peor (99,08 %).
- `suspect.py` sobre las 86 funciones no al 100 % de PacketPack: 23 con constantes/llamadas distintas, sin revisar
  (ruido de registros en su mayoria; fn_804E4B44 revisada: conocida).
