# Lote enemies-finish, cola q1

Inicio: 2026-10-22 09:05 CEST. Rama agent-q1 (sale de main 64df7a6; referencia start-q1: 6806 funciones al 100%).
Cola: game/Em003LesserDaemon, game/Em005DoppelGanger, game/Em101ElderZesya, game/Em101ElderZesyaSecond,
game/Em101LittleZesya, game/Em101LittleZesyaSecond (todas con fuente empezada en close-units).

Estado inicial (report.json, funciones al 100% / total): Em003LesserDaemon 7/22, Em005DoppelGanger 5/17,
Em101ElderZesya 8/20, Em101ElderZesyaSecond 3/15, Em101LittleZesya 13/25, Em101LittleZesyaSecond 3/15.

## Unidades

- `game/Em003LesserDaemon` (7/22 -> 8/22, primera pasada): `fn_80250884` (cuenta jugadores y enemigos del otro
  bando dentro de un radio) al 100% (`radius * radius` escrito en cada comparacion, sin local). `fn_802506AC`
  (teletransporte al punto mas lejano del objetivo) NONMATCHING 99.49%: r28/r30 de puntero/indice del bucle
  intercambiados (12 variantes). Cabecera: m14B0 (objetivo), m1590 (contador) + m1594 (8 puntos), m15F4.
- `game/Em101LittleZesya` (13/25 -> 18/25): ctor 99.74% (bloqueo del segundo std::string, como Em101ElderZesya y
  Em201ZanGurg), vf0C y lector del CSV `fn_8025CFF4` al 100%, `fn_8025E930` al 100% (`(float)mStatus.m04` en
  HpRate_unk), `fn_8025F07C` (teletransporte, solo floats del pool) y `fn_8025E9E4` (GetTrans_unk inline: solo
  pool) cuentan al 100% en el report. `fn_8025F45C` 98.26% (registros del bucle del punto mas cercano, como
  Em101ElderZesya::fn_802598DC; 10 variantes). `fn_8025D7B0` (dispatcher) no se deja escrito: con -ipa file
  inlinea los estados escritos y sale al 0% hasta que existan fn_8025D90C/fn_8025E100 (presupuesto de inline).
- `game/Em101LittleZesyaSecond` (3/15 -> 8/15): lector del CSV, vf3C, vf40, `fn_802620A8` (muerte) y
  `fn_80261948` al 100%; ctor 99.81% (segundo std::string); vf4C 99.83% (slots del vector negado; 9 variantes).
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
  Em101ElderZesyaSecond.cpp.

## Hallazgos MWCC

- `(float)mStatus.m04 / (float)max` (cast explicito aunque m04 ya es float) cambia el orden de f1/f2 del cociente.
- En un bucle de seleccion, un `float limit = K;` local antes del `if (a - b < limit)` carga la constante antes
  que los operandos (como en el original).
- `ToRotation_unk()` ya pone z = 0: un `rot.z = 0.0f` extra detras deja un frsp/stfs de mas.
- `CollResult_unk result;` declarado despues de los vectores from/to fija el orden de los stores del ctor.

## Pendientes para otras colas

- chr/ControlChant: `fn_801C02F4` es el ctor de `chr::ControlMagicEnd` (lo llaman Em003LesserDaemon,
  Em101ElderZesya, Em101ElderZesyaSecond y Em001Necromancer como extern "C").
