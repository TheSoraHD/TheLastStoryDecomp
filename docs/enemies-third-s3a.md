# Lote enemies-third, cola s3a (Em309Ghoul, Em309GhoulSimple, Em310Gargoyle) - segunda pasada

Rama agent-s3a (sale de main 8d7faf7). Referencia: /home/claude/ref/s3a_start.json (7352 funciones).

## Unidades

- `game/Em310Gargoyle`: **5/21 -> 14/21**. Escritas las 21 (no queda ninguna sin fuente). Nuevas al 100%: vf14
  (0x8030AF10, pantalla de depuracion), fn_8030B74C (spline + alabeo), vf50, Set_unk (0x8030B408), fn_8030C190 y
  fn_8030CA38 (copias debiles inline), fn_8030CF84 ya estaba. NONMATCHING:
  - vf10 99.79% y fn_8030D0D8 99.71%: registros del inline de fn_8030D0D8 (el original lo expande en la rama de
    depuracion de vf10; solo se autoinlinea si fn_8030D0D8 queda bajo el umbral: con dos sentencias `id` sale).
  - fn_8030B92C 93.89%: copias de los cuatro puntos de ruta (el original relee la pila para la longitud).
  - fn_8030CBC0 99.63% (r3/r4 de las copias, familia `__VecCopy`).
  - vf3C 99.95%, vf40 99.85%, fn_8030C1A4 99.81%, fn_8030C490 99.92%: solo desplazamientos de `.sdata2`: el pool de
    nuestro .o lleva las constantes de las copias debiles (ToU32 255, fn_8028AC70 -3/3/5, WrapAngle_unk pi/2pi) que
    el enlazador del original quito con ellas (poolcheck las lista; no hay forma de quitarlas sin perder el inline).
  - ctor 99.93%, vf0C 99.91%, vf20 99.88%: sin cambios.
- `game/Em309GhoulSimple`: **12/22 -> 17/22**. Escritas las 22. Nuevas al 100%: vf50, vfB4 (0x803098D0, bind de
  chr::ControlThrustForGhoul), invoker/manager de boost::function, vf30 (al poner "spine1" en vf20). NONMATCHING:
  vf108 99.61% (pila y FPR f29/f30/f31; mismo codigo), vfB0 99.59% (r4/r5, `__VecCopy`), vf10 99.57%, vf20
  99.77%, vf100 99.96% (sin cambios).
- `game/Em309Ghoul`: **14/33 -> 19/33**. Escritas las 33 y ordenado el fuente como el .dol (vf14 al final).
  Nuevas al 100%: vf40, vf50 (0x802FE630, tres esferas de dano), fn_802FF6C4, fn_802FF800 (movimiento de los
  estados sin update), fn_80300654/fn_8030073C/fn_803009F4/fn_80300AD8/fn_80300DF4/fn_803010BC/fn_80301200
  (inicios de estado, al cuadrar el pool), fn_80302B84, fn_803057A8 (menu de depuracion) y vf14. NONMATCHING:
  - ctor 96.97%, vfEC (0x803012AC, muerte + camara xsca) 96.49%, fn_80302F94 (estado 10) 95.62%, fn_80304B40
    97.97%: reparto de registros/pila (copias implicitas de Camera_unk, matrices y temporales).
  - vf10 99.80% (marco 0xB0 mayor), vf3C 99.87%, vf54 99.57%, vf20 99.48%, fn_802FFFF8 99.82%,
    fn_80301DDC 99.59%, fn_803026F0 99.71%, fn_80304718 99.79%, fn_80304FC0 99.23%: registros.
  - fn_803041C8 99.96%: solo el orden de 8.0f y 7.99999f en `.sdata2`.

Unidades enlazables: ninguna.

## Errores corregidos (objdiff no los ve)

- Em310Gargoyle::vf3C compara con 900.0f (no 0.05f) y vf40 usa 0.05f (no 0.25f): los literales estaban
  cambiados; el pool del original lo confirma.
- Em309GhoulSimple::vf20 busca el nodo "spine1" (no "neck").
- Em309Ghoul::vf0C guarda 0.08f en m56C (no 35.0f); al corregirlo cuadra el pool y suben a 100% siete funciones.

## Decisiones tomadas sin el usuario

- symbols.txt (renombrados): Gargoyle fn_8030A848 -> vf10, fn_8030AF10 -> vf14, fn_8030D5C8 -> vf50, fn_8030B74C,
  fn_8030B92C, fn_8030C1A4, fn_8030C490, fn_8030CBC0 con firma; fn_8030B408 -> `Set_unk__Q23atn11Color4f_unkFffff`;
  **fn_800D8814 -> `__ct__Q23atn11Color4f_unkFffff`** (es el ctor de atn::Color4f_unk; gmk/FlamingArrow.cpp, unico
  otro llamador con fuente, actualizado: vf10 sube 97.34 -> 97.47). GhoulSimple fn_803073C4 -> vf108,
  fn_80308E6C -> vf50, fn_803097AC -> vfB0, fn_803098D0 -> vfB4, fn_80309D20/fn_80309D50 -> invoke/manage,
  lbl_80787DE8 -> RTTI del bind_t, lbl_80787F48 -> @STRING@allocate, lbl_807C8420/lbl_8087F3E8 ->
  @LOCAL@/@GUARD@ stored_vtable (como los de Em302Kraken); fn_801957F0 ->
  `__ct__Q23chr21ControlThrustForGhoulFP9CharaTaskP9CharaTaskPCc`.
- **Cabecera compartida include/chr/ControlStickByQuark.h**: anadida la clase `chr::ControlThrustForGhoul` (solo
  declaracion; nadie mas la usa).
- Em310Gargoyle.cpp: 50 asserts vacios (`GARGOYLE_ASSERT_UNK`, regla aceptada del umbral de autoinline) en el
  accesor inline fn_80121F00: el original llama fuera de linea todos los inline de fn_8030C490 (supera el
  presupuesto); con menos de 50 todo se inlinea. Atn::Color4f_unk local con ToU32 inline (como
  atn/D2A_Window.cpp) para que vf14 supere el presupuesto igual que el original.
- Em309GhoulSimple.h: m154C-m1554 pasan a `atn::Vector3 m154C` (rotacion de la estocada; solo la usa su .cpp).
- **Fabrica compartida src/game/SpecialEnemyBase.cpp**: `new (3, "", "", 0) Em309Ghoul(parent, info)` en lugar del
  extern del ctor (incluye game/Em309Ghoul.h). Ghoul: fn_802FC850 -> ctor, fn_802FD328 -> vf10, fn_802FDDAC ->
  vf3C, fn_802FE630 -> vf50, fn_803012AC -> vfEC, fn_802FF800/fn_802FFFF8/fn_80301DDC/fn_803026F0/fn_80302F94/
  fn_803041C8/fn_80304FC0 con firma `__10Em309GhoulFv`.
- Em309Ghoul.cpp: 50 asserts vacios (`GHOUL_ASSERT_UNK`, misma regla que Gargoyle) en el accesor inline
  fn_80121F00 para que fn_802FFFF8 llame fuera de linea a sus inline como el original.
- Em309Ghoul.h: m1638 pasa a `atn::Vector2_unk m1638[2][2]` y m1658 a `CharaTask*`. HitInfo_unk::m50 (angulo de
  0x64A) se lee con `*(float*)&m50[0]` como en Em202Spider (no se toca la cabecera compartida).
- fn_80304FC0 reproduce un fallo del original: las direcciones de las balas laterales se escalan desde d0, no
  desde las giradas (`d1 = d0 * t`).

## Hallazgos MWCC

- objdiff no compara literales: un `0.05f` en lugar de `900.0f` sale al 100%. Revisar el orden del pool del
  original (`awk '/^.obj lbl_/{n=$2} /\.float/{print n,$2}'` sobre el .s) frente al primer uso de cada constante.
- Una funcion propia que el original expande en otra (fn_8030D0D8 en vf10) se reproduce dejandola bajo el umbral
  de autoinline; cualquier sentencia de mas la saca.
- Al cambiar un literal mal puesto (aunque objdiff lo de por bueno) se corrige el orden del pool y suben a 100%
  todas las funciones cuyo unico residuo eran desplazamientos de `.sdata2`: comparar el pool antes de iterar.
- Un vector frontal `Matrix3x4 m; m.fn_8013A13C(a); PSMTXMultVec(m.m, ...)` en un inline por valor da otro
  reparto de registros que `Mtx` + PSMTXRotRad (vfB0 de GhoulSimple: 95.9 -> 99.5).
