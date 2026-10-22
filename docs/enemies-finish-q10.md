# Lote enemies-finish, cola q10 (segunda pasada sobre la cola q1)

Inicio: 2026-10-22 14:51 CEST. Rama agent-q10 (sale de e407184; referencia start-q10: 7110 funciones al 100%).
Cola: game/em005_doppel_ganger, game/Em003LesserDaemon, game/Em101ElderZesya, game/Em101ElderZesyaSecond,
game/Em101LittleZesya, game/Em101LittleZesyaSecond (todas con fuente empezada).

Estado inicial (report.json, funciones al 100% / total): em005_doppel_ganger 8/17, Em003LesserDaemon 10/22,
Em101ElderZesya 10/20, Em101ElderZesyaSecond 11/15, Em101LittleZesya 18/25, Em101LittleZesyaSecond 8/15.

## Unidades

- `game/em005_doppel_ganger` (8/17 -> 9/17): escrita vf10 (5.7 KB) y la clase anonima `ControlCall`
  (ctor inline en vf10, vf0C y dtor). El fuente se reordena al orden del .dol (ctor, dtor, vf0C, vf10, vf54, vf14,
  vf3C, vf40, vf44, fn_80252E6C, fn_802531F4, vfF0, fn_80253884, fn_80253C9C, ControlCall::vf0C). Al 100%: dtor de
  ControlCall. NONMATCHING: vf10 96.3% (las pruebas `(m54C & 8) != 8` del original salen materializadas con
  subfic/or/srwi., mismo bloqueo que RouteWalker_unk::fn_80127EF4 y CharaTask::fn_80149A30; ~25 formas probadas
  en ficheros sueltos: bool/int, inlines, ternarios, mascara por parametro, xor, resta...); ControlCall::vf0C
  99.44% (r7/r8 en el bucle de busqueda y r26/r27 de dos vectores de pila; 6 variantes).

- `game/Em003LesserDaemon` (10/22 -> 14/22): escritas todas las funciones que faltaban: vf0C (lector del CSV,
  100%), vf10 (99.74%), vf3C (100% al completar el pool), fn_8024BE94 (luces de material, 99.89%),
  fn_8024D120 (eleccion de accion por fase, 100%), fn_8024F1DC (estado 8, abanico de laseres, 98.58%) y
  fn_8024FFE8 (cambio de fase, 100%). Fuente reordenado al orden del .dol. NONMATCHING: vf10 99.74% (huecos de
  pila de dos efectos y del dash, f9/f10; 8 variantes), fn_8024BE94 99.89% (f2/f3 del seno; 8 variantes),
  fn_8024F1DC 98.58% (registros salvados del bucle del abanico y de la busqueda; 10 variantes). Siguen como
  estaban: ctor 99.74%, vf24 99.90%, fn_8024E7E4 99.66%, fn_8024ED68 99.84%, fn_802506AC 99.53% (registros).

- `game/Em101ElderZesya` (10/20 -> 12/20; ganada de verdad solo vf50): escritas vf10 (3.6 KB), vf50,
  fn_8025922C y fn_80259AB4. Al 100%: vf50. fn_80258F1C sale al 100% en report.json pero 99.95% en el objdiff
  directo (etiquetas del pool): no se cuenta. NONMATCHING: vf10 97.39% (registros salvados del bucle de las
  barreras y de la lista de jugadores; 12 variantes), fn_8025922C 94.9% (parametros de camara direccionados con
  otro registro base, mismo bloqueo que EnemyJill), fn_80259AB4 97.67% (rama muerta `b` de ElderLook_unk::Set_unk
  que no se reproduce; ~12 formas).

- `game/Em101ElderZesyaSecond` (11/15 -> 12/15): escrita vf10 (2.3 KB, 99.84%); ya no queda ninguna funcion sin
  escribir. NONMATCHING: vf10 99.84% (solo desplazamientos de pila de los temporales: vectores del bucle,
  colores, esfera y copia del ptmf; registros e instrucciones iguales; 12 variantes). Siguen: ctor 99.74%,
  vf0C 92.78% (push_back de std::vector inline, caso abierto conocido).

- `game/Em101LittleZesya` (18/25 -> 19/25 en report, todas escritas): escritas el despachador fn_8025D7B0 (100%),
  fn_8025EAE4 (espadas teledirigidas, 94.30%) y vf40 (fn_8025F8C8, 99.53%). Fuente reordenado al orden del .dol
  (pool de .sdata2 identico al original). Al 100% de verdad: fn_8025D7B0, fn_8025DE7C, fn_8025E930,
  fn_8025E9E4 (las tres ultimas estaban al 99.8-99.9% solo por el pool). NONMATCHING: fn_8025EAE4 94.30% (el
  original guarda las palabras 0x43300000 de las dos conversiones int->float en el prologo y usa un FPR mas;
  11 variantes), vf40 99.53% (r27/r29 del anciano y del temporal de la rotacion; 9 variantes). Siguen: ctor
  99.74%, fn_8025D90C 99.04%, fn_8025E100 99.89%, fn_8025F45C 98.21%.

- `game/Em101LittleZesyaSecond` (8/15 -> 9/15, todas escritas): escritas vf10 (fn_802604E0, 99.96%),
  fn_80261078 (eleccion de accion, 98.51%), fn_80261DC0 (estado 11 con camara, 98.17%) y fn_802621D4 (destino
  del warp, 100%). Pool de .sdata2 identico al original. NONMATCHING: vf10 99.96% (huecos de pila del tajo del
  estado 12; 12 variantes), fn_80261078 98.51% (registros de las dos pruebas de estado y huecos de pila; 8
  variantes), fn_80261DC0 98.17% (parametros de camara por registro base, bloqueo conocido; 10 variantes).
  Siguen: ctor 99.81%, fn_80261A50 98.11%, vf4C 99.83%.

- Repaso de `game/Em101ElderZesya`: `ElderLook_unk::Set_unk` con if/else reproduce la rama muerta del original:
  fn_80259AB4 97.67% -> 98.70% y vf10 97.39% -> 97.84%.

## Decisiones tomadas sin el usuario

- **symbols.txt (renombrados)**: fn_80253F00 -> `vf0C__Q233@unnamed@em005_doppel_ganger_cpp@11ControlCallFv`
  (scope:local), fn_802544F0 -> `__dt__Q233@unnamed@em005_doppel_ganger_cpp@11ControlCallFv`, lbl_80784340 ->
  `__vt__Q233@unnamed@em005_doppel_ganger_cpp@11ControlCall` (scope:local), lbl_807843B0 -> `__RTTI__...ControlCall`.
  Ningun otro .cpp los usaba.
- Cabecera `include/game/Em005DoppelGanger.h` (solo la usan su unidad y la factoria): m14FC pasa a `CopyRobotTask*`
  (vf10 llama `CopyRobotTask::fn_801CCD00` y escribe su m14B8); el relleno 0x1530-0x1540 pasa a `atn::Vector3 m1530`
  (posicion de la trampa, duena de los efectos) y `float m153C` (radio); nueva inline `HpRate_unk`.
- Em005: vistas locales con nombres inventados: `DoppelAi_unk` (pesos por miembro del grupo de fn_80103F60, como
  AllyAi_unk de Em403WarLion), `Settings_unk` (solo m80 de lbl_8087F0A8), `ControlTime_unk` (m08 del control en
  curso), inline `IsShown_unk` / `IsControlTimeUp_unk`; `Color4f_unk` gana un ctor de 4 floats.

- Em003LesserDaemon: `mPoints` (0x1590) y `mLinks` (0x16D8) pasan a `atn::FixedArray_unk<atn::Vector3, 8>` y
  `atn::FixedArray_unk<Em003LesserDaemon*, 2>` (el original hace push_back con placement new y comprobacion de la
  direccion); m14C8/m14D4 a `HitInfo_unk*`, m14DC a `CharaTask*`, m14E0/m14E4/m14E8 y m1600/m160C sacados del
  relleno con nombres inventados; vistas locales `DaemonAi_unk`, `FrameTime_unk`, `MatColor_unk` (colores de
  material devueltos por fn_80094B6C), `ColorF_unk`. symbols.txt: fn_8024AD30/B4F0/C46C -> vf0C/vf10/vf3C y
  fn_8024BE94/D120/F1DC/FFE8 -> metodos de Em003LesserDaemon (ningun otro .cpp los llamaba).
- Em003LesserDaemon::vf10: un `*(volatile int*)&m1624` (dentro de la inline `ShowHits_unk`) reproduce la
  segunda lectura del original, variante ya usada en la cola q5.

- Em101ElderZesya: `Barrier_unk::Colli_unk` contiene ahora mDesc (antes en Barrier_unk); `Field_unk` local
  ampliado (m4D8 camara, m588 parametros, m8A0, m96C..m978, m97C ElderLook_unk, m10D8); m14D8/m14E4 a
  `atn::Vector3`; externs con nombres inventados (ElderRedirect_unk, ElderDamage_unk, fn_8025D6D0/D704...);
  incluye `game/Em101LittleZesya.h` para el companero. symbols.txt: fn_80256C04 -> vf10, fn_80257B50 -> vf50,
  fn_8025922C / fn_80259AB4 -> metodos de Em101ElderZesya (sin otros llamadores en .cpp).
- Em101ElderZesyaSecond: symbols.txt fn_8025ACA0 -> `vf10__21Em101ElderZesyaSecondFv`; cabecera: m1558 pasa de
  relleno a int, `vf10` declarada, inline `CountNear_unk` (cuenta de jugadores cerca, nombre inventado; como
  inline salen los f28-f31 del original). Locales: `EffectMgr_unk::mB8` (capa del efecto, como game/Arms.cpp),
  `ElderWeights_unk` (pesos por jugador, como GhoulWeights_unk), `ControlMagic_unk::m1D`, inlines
  `PlayEffectM_unk` / `PlayColorEffect_unk` / `GetTrans_unk`.

- **Em101LittleZesya: `ASSERT_UNK(this != 0)` (`((void)0)`) al principio de las diez funciones de estado**
  (3-12 por funcion, el minimo que da el 100%) para que fn_8025D7B0 no las autoinlinee con -ipa file: el
  original las llama fuera de linea. Es la variante aceptada de la guia (asserts de release vacios para pasar
  el umbral de autoinline), pero es codigo sin efecto: a revisar.
- Em101LittleZesya: symbols.txt fn_8025F8C8 -> `vf40__16Em101LittleZesyaFP11HitInfo_unk` (esta en el hueco
  vf40 de la vtable); inlines nuevas con nombres inventados `ZesyaRotZYX_unk`, `ZesyaHoming_unk` (parametros
  teledirigidos con sus valores por defecto), `WarpNear_unk` (miembro, cabecera) y externs fn_80370A78/AE4.
- Em101LittleZesyaSecond: symbols.txt fn_802604E0 -> `vf10__22Em101LittleZesyaSecondFv`, fn_80261078 /
  fn_80261DC0 / fn_802621D4 -> metodos (ningun otro .cpp los usaba); cabecera: vf10, los tres metodos, inlines
  `ResetAction_unk` / `ReleaseCamera_unk` / `Cast_unk`, m1544 pasa a float. `Field_unk` local con la camara,
  sus parametros y el punto de mira (copias `ZesyaCamera_unk` / `ZesyaCamParam_unk` / `ZesyaLook_unk` de las
  vistas de game/Em101ElderZesya.cpp), `Neg_unk`, `PlayEffect_unk`, `PlayEffectAt_unk`.
- Em101LittleZesya::vf40 calcula la rotacion hacia el anciano y no la usa (el original llama a atan2 igual).

## Hallazgos MWCC

- `bool r = (x & 8) != 8` no se materializa con subfic/or/srwi. en ninguna forma de C++ probada (bool/int/inline/
  ternario/mascara variable); `(8 - x) | (x - 8)) >> 31` escrito a mano si lo da, pero seria codigo inventado.
- En un argumento de llamada, calcular `m153C / 15.0f` en un `float` local antes del color temporal coloca el
  fdivs donde el original.

- Dos busquedas del punto mas lejano iguales en una unidad pueden necesitar dos inlines distintos: con
  `unsigned int i;` declarado antes del bucle sale el reparto de registros de una llamadora y con el `for`
  normal el de la otra (Em003LesserDaemon fn_8024D120 / fn_8024E1C0).
- `if (a == 0) return; if (b == 6) return;` (dos if) evita el `bne/b` invertido que da `if (a == 0 || b == 6)`.
- `mLinks.size() == 0` (accesor) en vez de `mLinks.mSize == 0` cambia el registro que recibe el contador.
- Colores de material leidos en una llamada: `ColorF_unk color(...)` con nombre y los otros dos como
  temporales en los argumentos dan el orden de pila del original.
- Un bucle con FPRs salvados escrito como inline miembro (`CountNear_unk(m14BC)`) da f28-f31 en el orden
  del original; escrito en linea dentro de vf10 los rota (Em101ElderZesyaSecond::vf10).
- Color elegido con ternario: guardarlo en un `Color4f_unk color = c ? A : B;` y pasarlo por `const&` a la
  inline da la copia por floats del original; pasar el ternario directo copia por words.
- Un despachador cuyas funciones de estado solo se llaman desde el (una vez cada una) las inlinea todas con
  -ipa file aunque esten definidas antes; el original las llama: hacen falta ~3-12 `((void)0)` en cada una
  (Em101LittleZesya::fn_8025D7B0).
- `m04 = flag ? 1 : 0;` en un setter inline no deja la rama muerta del original; `if (flag) m04 = 1; else
  m04 = 0;` si (`b` al siguiente + `li r0, 0`, ElderLook_unk/ZesyaLook_unk::Set_unk).
- El orden de .sdata2 delata el orden real del fuente: con todas las funciones escritas y en orden del .dol el
  pool sale identico y los NONMATCHING "solo pool" pasan a 100%.
- Dos consultas de estado seguidas (`IsState_unk(8) || IsState_unk(0x200)`) con un float calculado antes en una
  variable aparte (`reach`) acercan el orden del original (Em101LittleZesyaSecond::fn_80261078).

## Pendientes (esta cola)

- Ninguna funcion queda sin escribir en las seis unidades. Ninguna unidad enlazada (todas tienen algun
  NONMATCHING). Bloqueos principales: `(m54C & 8) != 8` materializado (em005 vf10), parametros de camara por
  registro base (fn_8025922C, fn_80261DC0), palabras 0x43300000 en el prologo (fn_8025EAE4), copias
  __VecCopy y huecos de pila.

## Pendientes para otras colas

- El truco de `ElderLook_unk::Set_unk` con if/else deberia servir a los demas usuarios del punto de mira de
  lbl_8087F430 + 0x97C (EnemyJill, Em203GreaterDaemon).
- `fn_80261A50` de Em101LittleZesyaSecond y los ctor de las tres unidades Zesya siguen con el bloqueo del
  segundo std::string (r26/r27), comun a Em201ZanGurg.

Resultado (report.json, funciones al 100%): em005 8 -> 9/17, Em003LesserDaemon 10 -> 14/22, Em101ElderZesya
10 -> 12/20 (una de ellas, fn_80258F1C, solo en el report), Em101ElderZesyaSecond 11 -> 12/15, Em101LittleZesya
18 -> 19/25, Em101LittleZesyaSecond 8 -> 9/15. Global 7110 -> 7120.
