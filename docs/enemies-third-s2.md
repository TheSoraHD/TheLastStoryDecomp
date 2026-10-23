# Lote enemies-third, cola s2 (Em306GurgBeast, Em308InvertCrab, Em308ParmCrab) - segunda pasada

Rama agent-s2 (sale de main be98a47). Referencia: /home/claude/ref/s2_start.json (7347 funciones).
Final: 7352 (+5), regress lost 0, fuzzdrop sin bajadas, poolcheck sin avisos, `main.dol: OK` en cada commit.

## Unidades

- `game/Em306GurgBeast`: **13/22 -> 17/22**. Escritas vf104 (0x802EF5FC, maquina de estados) y fn_802F09D4
  (0x802F09D4, movimiento hacia una rotacion, como CharaTask::fn_801426A4); vf30 movida al final del fuente
  (orden del .dol) y al 100%. Al 100% ademas fn_802F1120 (`if (a == 0 || b == 0 || m58C == 17) return;` y un
  local `rider` para la copia del asiento) y fn_802F1320 (vector frontal como inline `BeastFront_unk`).
  Ya no faltan funciones por escribir. NONMATCHING:
  - vf104 99.60%: solo registros (CanAct_unk en linea r0/r4, bloqueo conocido; r29/r30 y r28/r30 del bucle de
    los HomePoint; ~25 formas).
  - fn_802F09D4 99.72%: f3/f4 de m574.z y rot.y en el paso de amortiguacion y giro (~20 formas).
  - vf108 97.99% (antes 97.84): orden r5/r7 de las dos llamadas a fn_802F0964 (6 formas mas: local, ref,
    const, helper).
  - vf10 97.62% y fn_802F0998 56%: sin cambios (4 formas mas en fn_802F0998).
- `game/Em308InvertCrab`: **16/29 -> 17/29**. Escrita la huida fn_802F693C (100%): la distancia horizontal va
  por un inline que toma el vector **por valor** (`CrabLengthXZ_unk(atn::Vector3)`) y la rotacion como
  temporal `dir.GetNormalized_unk().ToRotation_unk().y`. Sigue sin escribir el slot 0x60 (fn_802F6E5C, ver
  pendientes). fn_802F469C (6 formas mas) y los gemelos fn_802F5BCC/5F20 (6 formas mas) sin mejora.
- `game/Em308ParmCrab`: **10/24 -> 11/24**. Escrita la huida fn_802FC2E0 (100%, misma forma que InvertCrab).
  Sigue sin escribir el slot 0x60 (fn_802FC848). vfEC (5 formas mas) y fn_802FA2F4 (4 formas mas) sin mejora.

Unidades enlazables: ninguna (las tres tienen NONMATCHING; los cangrejos ademas el slot 0x60).

## Pendiente de decision del usuario

- `CharaTask::vf60` deberia devolver `float`: la base (0x800EF38C, unidad sin fuente game/BattleCharaTask) es
  `lfs f1; blr`, y en todo el DOL solo dos vtables derivadas de CharaTask sobrescriben la ranura 0x60:
  Em308InvertCrab (0x802F6E5C, 8.0f) y Em308ParmCrab (0x802FC848, 8.0f). Cambiar la firma subiria **2
  funciones** (las dos copias de los cangrejos; la base quedaria escribible cuando se haga BattleCharaTask). No
  hay llamadores en el fuente actual que dependan del `void`. No se ha cambiado.

## Decisiones tomadas sin el usuario

- symbols.txt (renombrados): fn_802F693C -> `fn_802F693C__15Em308InvertCrabFP9CharaTask`, fn_802FC2E0 ->
  `fn_802FC2E0__13Em308ParmCrabFP9CharaTask`, fn_802EF5FC -> `vf104__14Em306GurgBeastFv`, fn_802F09D4 ->
  `fn_802F09D4__14Em306GurgBeastFPCQ23atn7Vector3f`. Ningun otro .cpp los llamaba como `extern "C"`.
- Cabeceras propias: Em308InvertCrab.h y Em308ParmCrab.h declaran la huida como miembro; Em306GurgBeast.h
  declara fn_802F09D4 y el inline `RideMove_unk` (movimiento con el stick). Sin cambios en cabeceras compartidas.
- Em306GurgBeast.cpp: structs locales BeastCamera_unk (lbl_8087EFB4, m134 = giro de la camara),
  BeastTutorial_unk/BeastMessage_unk (lbl_8087F490, peticion de tutorial solo si m764.m14 == 0),
  CollResult_unk::m34 pasa a `int*` (atributo del suelo copiado a m634). Helpers inline inventados (`_unk`):
  ShowTutorial_unk, BeastFront_unk, LengthSq_unk, RideMove_unk.
- Sin cambios de splits.

## Hallazgos MWCC

- Un inline que toma un `atn::Vector3` por valor (`f(a - b)`) deja el temporal en el hueco de pila del
  original donde un local con nombre o un parametro por referencia lo desplazan (huidas de Em308).
- Un vector frontal `Vector3(0,0,1)` rotado por `mRot.y` de otro personaje escrito como inline que devuelve el
  vector (`BeastFront_unk(const Vector3& rot)`) coloca la carga de `rot.y` despues de los stores de (0,0,1),
  como el original (Em306GurgBeast fn_802F1320 y vf104).
- `if (a == 0 || b == 0 || c == 17) return;` reproduce el orden de bloques del original donde un `if` anidado o
  returns separados invierten los saltos (Em306GurgBeast fn_802F1120).
- Copia de un vector a un miembro de otro objeto (`m152C->mPos = ...`): pasar el puntero a un local
  (`CharaTask* rider = m152C;`) arregla r3/r4/r5 (fn_802F1120).
