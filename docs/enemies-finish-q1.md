# Lote enemies-finish, cola q1

Inicio: 2026-10-22 09:05 CEST. Rama agent-q1 (sale de main 64df7a6; referencia start-q1: 6806 funciones al 100%).
Cola: game/Em003LesserDaemon, game/Em005DoppelGanger, game/Em101ElderZesya, game/Em101ElderZesyaSecond,
game/Em101LittleZesya, game/Em101LittleZesyaSecond (todas con fuente empezada en close-units).

Estado inicial (report.json, funciones al 100% / total): Em003LesserDaemon 7/22, Em005DoppelGanger 5/17,
Em101ElderZesya 8/20, Em101ElderZesyaSecond 3/15, Em101LittleZesya 13/25, Em101LittleZesyaSecond 3/15.

Estado final (report.json, funciones al 100% / total): Em003LesserDaemon 7 -> 10/22, Em005DoppelGanger
5 -> 8/17, Em101ElderZesya 8 -> 10/20, Em101ElderZesyaSecond 3 -> 11/15, Em101LittleZesya 13 -> 18/25,
Em101LittleZesyaSecond 3 -> 8/15 (+26 en total; global 6806 -> 6832). Ninguna unidad enlazada: a todas les
quedan funciones grandes sin escribir (vf10 de 2-6 KB y estados de 1-4 KB) y varios NONMATCHING de registros.

Nota sobre los porcentajes: varias funciones dan 99.x% en objdiff de la unidad suelta y 100% en report.json
(solo difieren en etiquetas del pool .sdata2 / @stringBase0, que el report compara por posicion); esas cuentan
como 100% y no llevan NONMATCHING. Las que siguen por debajo de 100% en el report llevan NONMATCHING con el %
de objdiff.

## Unidades

- `game/Em003LesserDaemon` (7/22 -> 10/22): `fn_80250884` (cuenta jugadores y enemigos del otro bando dentro de
  un radio) al 100% (`radius * radius` escrito en cada comparacion, sin local). `fn_8024F9B8` (busca un hueco
  libre detras del objetivo con rayos y barrido) y `fn_8024E1C0` (arranca la accion encolada m1654) al 100%.
  NONMATCHING: `fn_802506AC` 99.49% (r28/r30 de puntero/indice del bucle, 12 variantes); ctor 99.74% (segundo
  std::string); `vf24` (fn_8024CD24, motion de andar segun la direccion, como CharaTask::vf24) 99.52% (f3/f4 del
  0.0f de ToRotation_unk; los double de .rodata los colocan funciones sin escribir); `fn_8024E7E4` (estado 6,
  desplazamiento del warp) 99.39% (r4/r5 de las copias de mWarpDest, familia __VecCopy); `fn_8024ED68`
  (estado 7, laser teledirigido de 3 balas) 99.61% (r27/r28 de los dos vectores y f20/f21, ~10 variantes).
  Quedan: fn_8024AD30 (vf0C?), fn_8024B4F0 (vf10, 2.4 KB con tabla de saltos), fn_8024BE94, fn_8024C46C (vf3C),
  fn_8024D120 (4.2 KB), fn_8024F1DC, fn_8024FFE8.
- `game/Em005DoppelGanger` (5/17 -> 8/17): vf0C, `fn_80252E6C` (estado 0) y `fn_80253884` (estado 1) al 100%.
  NONMATCHING: ctor 99.6% (segundo std::string), vf3C 98.98% (b inalcanzable tras el return), vf40 99.58%
  (r5/r6 de la copia del registro de 0x1470), vfF0 98% (li r4 / mr r3), `fn_80253C9C` 99.54% (bools del
  CanAct_unk inline), `fn_802531F4` 99.1% (slots de la matriz de rotacion). Quedan vf10 (5.7 KB) y la clase
  anonima `ControlCall` (vf0C fn_80253F00, dtor fn_802544F0): solo la construye vf10, asi que sin vf10 MWCC no
  la emite; el borrador esta en scratch (no commiteado) y se describe abajo.
- `game/Em101ElderZesya` (8/20 -> 10/20): `fn_8025941C` y `fn_80258574` al 100%. NONMATCHING: lector del CSV
  `fn_80256384` 99.85% (r24/r25 de la tabla de rutas y la barrera), `fn_80258F1C` (estado 12) 99.99% (solo el
  offset de "SE_SP016_001_2" en @stringBase0, depende de las funciones sin escribir). Quedan fn_80256C04 (3.6 KB),
  fn_80257B50, fn_80258A50, fn_8025922C (copia FPR de parametros de camara, bloqueo de EnemyTrista) y
  fn_80259AB4.
- `game/Em101LittleZesya` (13/25 -> 18/25): ctor 99.74% (bloqueo del segundo std::string, como Em101ElderZesya y
  Em201ZanGurg), vf0C y lector del CSV `fn_8025CFF4` al 100%, `fn_8025E930` al 100% (`(float)mStatus.m04` en
  HpRate_unk), `fn_8025F07C` (teletransporte, solo floats del pool) y `fn_8025E9E4` (GetTrans_unk inline: solo
  pool) cuentan al 100% en el report. `fn_8025F45C` 98.26% (registros del bucle del punto mas cercano, como
  Em101ElderZesya::fn_802598DC; 10 variantes). `fn_8025D90C` (elige la siguiente accion) 99.04% (la matriz 4x4
  de ToRotation_unk queda con las filas en orden de pila descendente en el original; 8 variantes).
  `fn_8025D7B0` (dispatcher) no se deja escrito: con -ipa file inlinea los estados escritos y sale al 0% hasta
  que existan fn_8025E100/fn_8025EAE4 (presupuesto de inline); borrador en scratch.
- `game/Em101LittleZesyaSecond` (3/15 -> 8/15): lector del CSV, vf3C, vf40, `fn_802620A8` (muerte) y
  `fn_80261948` al 100%; ctor 99.81% (segundo std::string); vf4C 99.83% (slots del vector negado; 9 variantes);
  `fn_80261A50` (balas teledirigidas, HomingParam_unk de game/HomingBullet.h) 97.9%. Quedan vf10 (fn_802604E0),
  fn_80261078, fn_80261DC0 (bloqueo de la camara) y fn_802621D4.
- `game/Em101ElderZesyaSecond` (3/15 -> 11/15): vf3C, vf40, `fn_8025C624` (rotura de barrera), `fn_8025C460`
  (teletransporte), `fn_8025C7A8` (ordena los puntos de warp y elige uno de los 3 mas cercanos), `fn_8025B9EC`,
  `fn_8025BD1C`, `fn_8025BF58`, `fn_8025C29C` al 100%. ctor 99.74% (segundo std::string) y vf0C 92.8% (push_back
  de vector en bucle, bloqueo documentado). Queda vf10 (`fn_8025ACA0`, 2.3 KB).

## Decisiones tomadas sin el usuario

- **symbols.txt**: los pools de cadenas `lbl_8074367C`, `lbl_80743AF4`, `lbl_80743D18`, `lbl_80743F78`,
  `lbl_807440F4`, `lbl_80744298` (uno por unidad de la cola) pasan a `@stringBase0` (scope:local, string_table).
- **symbols.txt**: `lbl_8087DC28`/`lbl_8087DC2C` (.sdata, auto_09) -> `@STRING@reserve__Q23atn26Vector_unk<Q23atn7Vector3>FUii@0`
  y `...FUii` (scope:weak): son los "" del `new (0, "", "", 0)` del reserve inline de `Vector_unk<Vector3>`, que
  usan Em101LittleZesya y Em101ElderZesya.
- **symbols.txt**: `lbl_80743F50` (4 bytes) + `lbl_80743F54` -> `sSounds_unk` (tabla local de 2 punteros a
  "SE_SP007_001"/"SE_SP007_003" de Em101ElderZesyaSecond).
- **symbols.txt (renombrados)**: Em003LesserDaemon fn_802506AC/fn_80250884; Em101LittleZesya fn_8025C998 (ctor),
  fn_8025CEF8 (vf0C), fn_8025CFF4, fn_8025D90C, fn_8025E100, fn_8025F07C, fn_8025F45C; Em101LittleZesyaSecond
  fn_80260EBC/F88 (vf3C/vf40), fn_802626E4 (vf4C), fn_802620A8, fn_80261948; Em101ElderZesyaSecond fn_8025A67C
  (vf0C), fn_8025B5D8/B7DC (vf3C/vf40), fn_8025B9EC, fn_8025BD1C, fn_8025BF58, fn_8025C29C, fn_8025C460,
  fn_8025C624, fn_8025C7A8. Ningun otro .cpp los llamaba.
- Cabeceras de la cola (solo las usan sus unidades y la factoria): campos sacados del relleno con nombres
  inventados (`// inferred:`); `ElderList_unk` pasa a `std::vector<RoutePlace_unk*>`; Em101LittleZesya incluye
  game/CharaStatus.h para RouteTable_unk/RoutePlace_unk.
- `fn_801C02F4` (ctor de chr::ControlMagicEnd, unidad chr/ControlChant en asm) se declara `extern "C"` en
  Em101ElderZesyaSecond.cpp y Em003LesserDaemon.cpp.
- **symbols.txt (renombrados, segunda pasada)**: Em003LesserDaemon fn_8024A650 (ctor), fn_8024CD24 ->
  `vf24__17Em003LesserDaemonFv` (es la entrada vf24 de la vtable), fn_8024E1C0, fn_8024E7E4, fn_8024ED68,
  fn_8024F9B8; Em005DoppelGanger ctor, vf0C, vf3C, vf40, vfF0, fn_80252E6C, fn_80253884, fn_80253C9C,
  fn_802531F4; Em101ElderZesya fn_80256384, fn_80258574, fn_80258F1C, fn_8025941C; Em101LittleZesyaSecond
  fn_80261A50. Ningun otro .cpp los llamaba.
- `fn_8020A81C` se declara en Em005DoppelGanger.cpp con `const char*` (CharaTask.cpp la declara con un int; mismo
  simbolo C, solo cambia el tipo del argumento en esta unidad).
- Em101ElderZesya: `Barrier_unk::m00` pasa a `Gimmick_unk*` (punto de warp) y m1500-m1518 a `HitInfo_unk*`
  (nombres de las claves del CSV); `Field_unk` local con la tabla de rutas en 0x10D8 (vista minima, como
  Em306GurgBeast).
- Em003LesserDaemon: m14D0 y m14CC pasan a `HitInfo_unk*` (magia de fn_8024E1C0 y bala del warp), m1610 a
  `atn::Vector3` (destino de la accion 11), y se declara la virtual vf24 en la cabecera. `ControlMagic_unk`
  (vista del control de magia: m14 objetivo, m1D ya disparado) y `ElderView_unk` (Em101LittleZesya: solo el
  flag 0x16AC de Em101ElderZesya) son vistas locales con nombres inventados.
- Em005DoppelGanger: m14FC/m1504 a `CharaTask*`, m1528/m152C a `RoutePlace_unk*`; `DoppelState_unk` (registro de
  0x1C bytes en 0x1470 de los personajes de batalla) definido localmente.

## Hallazgos MWCC

- `(float)mStatus.m04 / (float)max` (cast explicito aunque m04 ya es float) cambia el orden de f1/f2 del cociente.
- En un bucle de seleccion, un `float limit = K;` local antes del `if (a - b < limit)` carga la constante antes
  que los operandos (como en el original).
- `ToRotation_unk()` ya pone z = 0: un `rot.z = 0.0f` extra detras deja un frsp/stfs de mas.
- `CollResult_unk result;` declarado despues de los vectores from/to fija el orden de los stores del ctor.
- Con varias llamadas a `fn_8004ED34(lbl_8087EE98, ...)` en una funcion, el original carga el global en un
  registro no volatil antes de `fn_80179D44()`: sale con un local `void* colli = lbl_8087EE98;` justo antes de
  cada llamada (sin el, MWCC lo carga despues en r3).
- `m164C = m1624 == 0 ? 3 : 1;` en vez de un local con if: elimina un registro no volatil extra.
- En un bucle `for (i = 0; i < 3; i++)` con `(float)(3 - 1)`, MWCC pliega la constante; el original convierte
  un entero en tiempo de ejecucion (`int count = 3; int n = count - 1;`).
- Bucle que copia `fwd` a `dir` al principio: declarar `dir = fwd` antes del calculo del flotante `t` fija los
  registros del bucle (de 95% a 99.6%).
- `bool f() const { return !m; }` en un inline genera cntlzw/srwi. en vez de cmpwi (el original lo usa para
  `!(x->m16AC == 0)`).
- Una clase en un namespace anonimo que solo se construye desde una funcion sin escribir no se emite (ni vtable
  ni metodos): hay que escribir el constructor llamante a la vez.

## Pendientes (esta cola)

- Em005DoppelGanger: clase `ControlCall` en `namespace {}` (RTTI "@unnamed@em005_doppel_ganger_cpp@::ControlCall",
  vtable 0x80784340, 8 bytes, base chr::ControlBase): vf0C fn_80253F00 busca la bala del doppelganger
  (SkillEffectManager, m01 == 5) y, desde el frame 110, manda a los enemigos fuera del radio a un punto del borde
  (DoppelState_unk con rand() % 10, dos rayos y un barrido); dtor fn_802544F0. Necesita vf10 escrita para que
  MWCC la emita. Ojo: el nombre del fichero original es em005_doppel_ganger.cpp, asi que el nombre mangled y la
  cadena RTTI del namespace anonimo no coinciden con src/game/Em005DoppelGanger.cpp (probablemente quede
  NONMATCHING en datos o requiera renombrar el fichero, decision del usuario).
- Dispatchers (Em101LittleZesya fn_8025D7B0) y vf10 grandes de todas las unidades.

## Pendientes para otras colas

- chr/ControlChant: `fn_801C02F4` es el ctor de `chr::ControlMagicEnd` (lo llaman Em003LesserDaemon,
  Em101ElderZesya, Em101ElderZesyaSecond y Em001Necromancer como extern "C").
