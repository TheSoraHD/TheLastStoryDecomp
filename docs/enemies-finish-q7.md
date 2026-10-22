# Lote enemies-finish, cola q7 (Em403WarLion, EnemyJill)

Inicio: 2026-10-22 09:05 CEST. Rama agent-q7 (sale de main 64df7a6). Referencia: /home/claude/ref/start-q7.json.
Ultima actualizacion: 2026-10-22 13:20 CEST.

## Unidades

- `game/Em403WarLion` (unidad SIN FUENTE hasta ahora): **0/67 -> 39/67** al 100%. Fuente nueva
  `src/game/Em403WarLion.cpp` y cabecera nueva `include/game/Em403WarLion.h` (subclase de EnemyTask, layout
  0x14B0-0x1610, nombres inventados). No enlazada (quedan NONMATCHING y los helpers de vector/spline sin
  escribir). Pool de .sdata2 alineado con el original salvo la cola (1000.0f, 0.015625f de los helpers de spline).
  NONMATCHING:
  - fn_80324A38 98.22% (movimiento, version propia de CharaTask::fn_8013CB68): FPR del giro y del clamp de m584.
  - vf24 99.75%: dt en f28 en vez de f29.
  - fn_803256F8 97.82% (callback de pose): un GPR guardado mas en el original y orden de los quaternion.
  - fn_803258D4 98.79% (callback de matriz): r30/r31 del nodo y huecos de pila de MakeRotationZYX_unk.
  - vf20 97.76%: r3/r4 de copias de Vector3 (__VecCopy) y offset de "tale" en @stringBase0.
  - vf50 99.87%: orden de cuatro stfs del item.
  - fn_803267E8 99.93%: f30/f31 y huecos de pila de los CollResult_unk.
  - fn_803278B4 99.72%: r5/r6 de la copia a m150C (__VecCopy).
  - fn_80327A64 99.95%: offset de "" en @stringBase0 (cadenas de funciones sin escribir).
  - fn_80327EF0 99.96%: operandos de un fmuls.
  - fn_8032943C 99.22%: orden de carga m590/lbl_8087F048 y dos stfs del rayo.
  - fn_80329744 99.29%: temporales de Lerp en la pila; fn_80329A20 99.34%: stfs tras el ternario.
  - fn_80329ED8 98.28%: asignacion de GPR; fn_8032A3D4 98.99%: r28/r29 de move/ofs.
  - fn_8032A9F4 7.68% (borrador): el original llama fuera de linea a ctores de Vector3 y FindPlace_unk.
  - vfE4 99.55%, vf90 97.50%: solo la posicion de su constante en .sdata2 (falta el final del pool).
  - ctor 95.5%: planificacion de las stores de los vectores de WarLionPath_unk.
  - Sin escribir: fn_8032AB90, fn_8032ABA4, fn_8032AC1C, fn_8032AF90, fn_8032B314, fn_8032B6F8, fn_8032B7F0,
    fn_8032BE88, fn_8032C1EC (copias debiles de std::vector<Vector3> y del spline; tambien las llaman
    gmk/CharaPathMove, Event/Manager y game/LastCocoonQuadruped).

- `game/EnemyJill` (unidad SIN FUENTE hasta ahora): **0/39 -> 30/39** al 100%. Fuente nueva
  `src/game/EnemyJill.cpp` y cabecera nueva `include/game/EnemyJill.h` (subclase de EnemyTask, layout
  0x14B0-0x17C8, nombres inventados; parecida a EnemyTrista). No enlazada. NONMATCHING:
  - vf0C 98.26%: r4/r5 en la copia de la matriz de color.
  - fn_8032E3DC 99.87%: registros dentro de la copia inlineada de std::string.
  - vf10 99.79%: r29/r30 del evento actual.
  - fn_8032F314 99.89%: r3/r4 de la copia a m14C0 (__VecCopy).
  - fn_8032F828 93.8%: registros y orden del buffer circular de voces.
  - fn_8032FBF4 86.3%: el original guarda this, el indice y el offset en tres registros.
  - fn_8032FDE8 98.2%: registros del arranque del contador de guardia.
  - fn_80330D74 90.8%: direccionamiento de la camara con registros base.
  - fn_80331898 53.9%: interpolacion de la matriz de color (planificacion de FPR muy distinta).

Unidades enlazadas: ninguna.

## Decisiones tomadas sin el usuario

- `config/SLSP01/symbols.txt` (Em403WarLion): renombrados ctor/dtor y las virtuales de la vtable
  (vf0C, vf10, vf14, vf20, vf24, vf3C, vf40, vf50, vf54, vf90, vf94, vfE4, vfEC), los metodos no virtuales
  `fn_XXXXXXXX__12Em403WarLionF...` (incluidos fn_80329ED8, fn_8032A3D4, fn_8032A650, fn_8032A894, fn_8032A9F4 y
  fn_8032C3CC), `fn_80323C18` -> `IsRaging_unk__12Em403WarLionFv`, `fn_80323C34` -> `TransMtx_unk__Ffff` (copias
  debiles de inlines), los invokers/managers de los dos `boost::bind` (0x803234C8-0x803235D8), sus RTTI
  (0x80788BC0/BC8), `@LOCAL@`/`@GUARD@` de `assign_to` (0x807C8438/40, 0x8087F400/01) y `lbl_80749D10` ->
  `@stringBase0` (scope:local).
- `config/SLSP01/symbols.txt`: `fn_80197528` -> `__ct__Q23chr17ControlMatHoldPreFP9CharaTaskP9CharaTask` (ctor de
  un control de chr/ControlStickByQuark, nombre de clase sacado de la vtable que guarda); declarado en
  `include/chr/ControlStickByQuark.h` (solo la declaracion, sin tocar otras clases). Nadie lo llamaba por
  `extern "C"`.
- `game/SpecialEnemyBase.cpp`: la factoria crea el tipo 48 con `new (3, "", "", 0) Em403WarLion(parent, data)`
  y el tipo 63 con `new (3, "", "", 0) EnemyJill(parent, info)` (se quitan los `extern "C"` fn_80322C68 y
  fn_8032DEA0). fn_805A5374 sigue igual (73.27%).
- `config/SLSP01/symbols.txt` (EnemyJill): ctor (`__ct__9EnemyJillFPQ23atn4TaskPC13EnemyInfo_unk`, con
  `const EnemyInfo_unk*` como CopyRobotTask para que la carga de info->m20 vaya al prologo), dtor implicito, vf0C,
  vf10, vf14, vf3C, vf44, vfA8 y los 27 metodos no virtuales `fn_XXXXXXXX__9EnemyJillFv`; `lbl_80749F88` ->
  `@stringBase0` (scope:local).
- Clases inventadas de EnemyJill: `JillEvent_unk`, `JillColMat_unk`/`JillVec4_unk`, `JillRing_unk`,
  `JillVoices_unk`, `JillControl_unk`, `PlayerInput_unk`, `JillKeyboard_unk`, `JillArena_unk`, `JillField_unk`.
  Accesores de otras unidades escritos como `extern "C" inline` en el .cpp (fn_80122550, fn_8026616C,
  fn_80266174, fn_802A49BC, fn_802A4968, fn_8000D9E8, fn_8000DCF4, fn_80121F00) y fn_8032EC94/ECA0/ECA8.
- Clases inventadas de Em403WarLion: `WarLionPath_unk` (0x50 bytes en 0x1524, mismo layout que
  `gmk::PathData_unk` sin su ultima palabra; con `GetPos_unk`/`Segment_unk` inline como `GetLocal_unk` de
  game/PathData_unk.cpp), `WarLionCubic_unk`, `WarLionLen_unk` (struct de un float: con `float` el vector
  emitia el bucle de destruccion y rompia el dtor), `WarLionPathList_unk`, `WarLionRes_unk`, `AllyAi_unk`,
  `WarLionPlace_unk` (tabla del circuito `lbl_80749B00`, 10 lugares 0x258-0x261, definida en el .cpp como
  `extern "C" const` al principio del fichero para que quede primera en .rodata), `CollHit_unk`.
- Em403WarLion: funcion inline inventada `WarLionStripped1_unk` (no se emite) solo para colocar el literal
  -100.0f del pool en su sitio (el original lo tiene justo detras de 57.295776f; probablemente de una copia
  debil eliminada por el enlazador). Mismo criterio que `Stripped1_unk` de EnemyJill.
- Em403WarLion: `DegToRad_unk__3atnFf` y `fn_8008CD1C` (asignacion de Matrix3x4 de atn/Model) declarados como
  `extern "C"` con su nombre: vf10 supera el presupuesto de inline y el original enlaza las copias de otras
  unidades (sus constantes no estan en el pool de esta).
- Prototipo de fn_800F8C6C en EnemyJill.cpp con el float antes del ultimo int (`int time, float f, int c`): fija
  el orden de carga de m594 (orden de evaluacion de argumentos de derecha a izquierda). En Em403WarLion.cpp
  se queda `int c, float f` (con ese orden ya casan sus llamadas).

## Hallazgos MWCC

- vf0C de Em403WarLion: un `if (result != 0) { ...; return 1; } return 0;` (sin retorno temprano) da el `beq`
  directo del original; el `Vector3(a, b, c)` asignado a un miembro necesita un local con nombre para que el
  temporal vaya al final de la pila.
- `float frame; if (a > 5 || (frame = m, frame >= f())) {...}` (operador coma) reproduce la carga del frame
  en f31 antes de la llamada sin `mfcr`.
- La cola circular de voces de EnemyJill: `mHead = Next_unk(mHead); mCount--;` reproduce el orden de cargas.
- El bit `0x20` de 0x12A4 es `mFlag5` (MSB primero), no `mFlag26`.
- Intercambio r5/r6 al copiar `chara->mPos - mPos` a un temporal: copiar antes el puntero a un local
  (`CharaTask* target = mC58.mC4; target->mPos - mPos`) lo arregla (EnemyJill::fn_80331DAC,
  Em403WarLion::fn_80327EF0).
- FindPlace_unk inlineado con r5/r6 cambiados: el original pasa por los accesores
  `fn_80121F00()->fn_8013C504()->FindPlace_unk(id)` (Field_unk::fn_8013C504 devuelve m10D8).
- Escalar un Vector3 por `1/n`: `v *= 1.0f / n` recalcula por componente; con un local `inv` y tres `*=` sale
  como el original (fn_8032A650).
- Una condicion inline `a != 0 && b != 0` en un `if (!...)`: con un `bool` local (true, y false en el `||`) sale
  el `li r3, 1 / li r3, 0` del original (vf10).
- ToRotation_unk vs ToRotationZ_unk (version con local `fz`): el original de varias funciones usa la segunda
  (`fmr f2, f4` en vez de `frsp f2, f2`).

## Pendientes para otras colas

- Los helpers de std::vector<atn::Vector3> y del spline (fn_8032AB90..fn_8032C1EC) estan en esta unidad pero los
  usan tambien gmk/CharaPathMove, Event/Manager y game/LastCocoonQuadruped; game/PathData_unk.cpp ya modela el
  spline. Escribirlos permitiria cerrar el pool y enlazar Em403WarLion.
