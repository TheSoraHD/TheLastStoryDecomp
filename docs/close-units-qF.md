# Lote close-units, cola F (chr y game, 20 o mas funciones pendientes)

Inicio: 2026-10-22 05:39 CEST. Rama agent-F. La herramienta Projects no estaba disponible: reglas tomadas del
prompt del lote y de docs/.

Observacion general: de las 78 unidades de la cola solo 4 tienen fuente (game/AreaEffConfig,
game/GimmickStateCommand, game/SkillEffectManager, game/GameUI); el resto esta al 0% sin .cpp. Ninguna puede
cerrarse en el tiempo del lote; se prioriza (1) las funciones NONMATCHING >= 95% de las unidades con fuente y
(2) empezar las unidades sin fuente por la cabeza de la cola, escribiendo las funciones pequenas/medianas.

## Unidades

- game/AreaEffConfig: sin cambios. fn_80043014 99.57% (contador exterior / desplazamiento interior r27/r28):
  ~35 variantes (las 24 permutaciones de declaraciones de count/n/i/j, while, continue, unsigned, referencia
  local, mSize en la condicion). fn_80041D44 0%: presupuesto de inline (bloqueo documentado), no tocada.
- game/SkillEffectManager: sin cambios. fn_8059B66C 99.01% (parametros r29-r31/r25-r27 desplazados): 11
  variantes (bucle de FindFree_unk en la cabecera, retorno temprano, locales de user, m02 directo). vf10 y
  fn_8059B8A8 no intentadas (registros ya investigados).
- game/GimmickStateCommand: vf10 94.49% sin cambios (4 variantes del bucle de tablas: unsigned, continue,
  referencia, indice fuera). El resto: presupuesto de inline (documentado en la unidad).
- game/GameUI: no intentada (ver close-units-qC; ctor 98.1% de planificacion documentado).
- game/Em101ElderZesya (0/20 -> 7/20, NUEVA fuente `src/game/Em101ElderZesya.cpp` + cabecera
  `include/game/Em101ElderZesya.h`): al 100% Barrier_unk ctor/dtor (0x80255F2C/0x80255F68), dtor (0x80256014),
  vf0C (0x802561C4), vf3C (0x80259604), vf4C (0x80257A94), vf64 (0x80257A14) y `__sinit` (0x8025A03C).
  ctor 99.6% (r25/r26 del strlen de los parametros y del grupo del menu, igual que Em201ZanGurg), vf54 99.9%
  (el original recarga m624.mData antes de copiar el centro, mismo caso abierto que CharaTask::vf54).
  - symbols.txt: fn_802558B0 -> `__ct__15Em101ElderZesyaFPQ23atn4TaskP13EnemyInfo_unk`, fn_80255F2C/F68 ->
    `__ct__/__dt__Q215Em101ElderZesya11Barrier_unkFv`, fn_80256014 -> `__dt__15Em101ElderZesyaFv`,
    fn_802561C4 -> vf0C, fn_80257A14 -> vf64, fn_80257A94 -> vf4C, fn_80258368 -> vf54, fn_80259604 -> vf3C,
    fn_8025A03C -> `__sinit_\Em101ElderZesya_cpp` (scope:local).
  - src/game/SpecialEnemyBase.cpp: la factoria (case 7) usa `new (3, "", "", 0) Em101ElderZesya(...)` como con
    Em201ZanGurg (sin cambio de codigo generado).
  - Decision sin el usuario: el barrier guarda un `Gimmick_unk*` opaco (posicion en +4, rotacion en +0x14):
    incluir game/GimmickCommon.h choca con declaraciones locales de SpecialEnemyBase.cpp.
  - HitMsg_unk+0x40 se lee con un cast (`*(int*)&msg->m3C[4]`) para no tocar la cabecera compartida CharaTask.h.

- game/Em101ElderZesya (segunda pasada, 8/20): `fn_802598DC__15Em101ElderZesyaFv` (punto de canto mas cercano
  al centro del grupo) 99.22%: copias de vector r3/r4 (familia `__VecCopy`) + orden de incrementos del bucle.
  Con un inline `PartyCount_unk()` (carga de lbl_8087F8A0 repetida) el resto coincide.
- game/Em205Ogre (0/21 -> 14/21, NUEVA fuente `src/game/Em205Ogre.cpp` + `include/game/Em205Ogre.h`):
  al 100% ctor (0x802B77AC), dtor (0x802BA898), vf0C, vf10, vf14, vf40, vf54, vf7C, vfFC, vf100 (0x802B7F94),
  vf10C (0x802B7DEC), lector del CSV (0x802B7AF8), busqueda del circulo de curacion (0x802B9C5C) y el `get`
  debil del scoped_ptr (0x802B98A8). vf30 99.97% (desplazamiento de "Em205 Ogre[%d]" en el pool, cadenas de
  funciones aun sin escribir). fn_802B98B0 (capsula) 98.59%: familia `__VecCopy` + planificacion de FPR.
  - symbols.txt: fn_802B77AC/BA898/7970/7CE0/7DE8/9BC8/7E60/BA5E0/7F04/7F4C/7DEC/7F94/7AF8/9C5C/98B0 ->
    nombres de Em205Ogre; fn_802B98A8 -> `get__Q25boost27scoped_ptr<13ResLoader_unk>CFv`.
  - SpecialEnemyBase.cpp: factoria case 23 con `new (3, "", "", 0) Em205Ogre(...)`.
  - Pendientes: vf50 (0x802B9DB4, recrea m624 con push_back en linea: bloqueo documentado), vf104/vf108
    (0x802B8398/0x802B8AA4, grandes), slot 0xB0 (0x802BA64C: CharaTask::vfB0 esta declarada `void vfB0()`; su
    firma real devuelve int y recibe un Vector3*; no se cambia la cabecera compartida), vfB4 (0x802BA67C, bind
    de ControlWallSlash).

- game/Em303KrakenFootMp (0/21 -> 15/21, NUEVA fuente + cabecera): al 100% ctor (0x802E59E0), dtor
  (0x802E84C8), fn_802E5B30 (setter), lector del CSV (0x802E5CD0), vf14, vfFC, vf108 (vacia), vf3C, vf24, vf40,
  vf80, vfEC, vf110/vf114 (estado multijugador) y fn_802E8084 (reaccion al dano). vf0C 99.6% (orden de los
  flags 0x2A del estado / 0x200 de m54C, 8 variantes); vf10 ~75% (CanAct_unk en linea dos veces + copias de
  vector). Pendientes: vf100 (0x802E6228, 1556 B), vf104 (0x802E683C, 2272 B), vf54 (0x802E7298),
  fn_802E7740 (2372 B).
  - splits.txt (decision sin el usuario): `.rodata` de game/Em303KrakenFootMp ampliada de 0x80747E9C a
    0x80747F58: absorbe el tramo auto_06_80747E9C (relleno + pool de cadenas lbl_80747EA0, que solo usa esta
    unidad). Build limpio de comprobacion OK.
  - symbols.txt: fn_802E59E0/5B30/5B38/5CD0/5E94/61BC/61D0/711C/7120/71B0/8084/8250/82F0/8354/8438/8454/84C8 ->
    nombres de Em303KrakenFootMp.
  - SpecialEnemyBase.cpp: factoria con `new (3, "", "", 0) Em303KrakenFootMp(...)`.

- game/Em315Coffin (0/21 -> 7/21, NUEVA fuente + cabecera): al 100% dtor (0x80313848), vf0C (0x8031393C),
  vf28 (vacia), vf14, vfFC, fn_80315FE0 (para el efecto), vfA0 (3.6651914f). vf30 ~99% (solo desplazamientos
  del pool: faltan cadenas de funciones sin escribir); lector del CSV ~85% (las palabras 0x43300000 de las
  conversiones del color del material salen del bucle en el original); ctor ~35% (copia del CharaRes_unk de
  0x90 bytes por pares de palabras y orden de lectura de los estaticos de .sdata).
  - symbols.txt: fn_80313544/3848/393C/3AAC/3FC0/3FC4/40C0/5FE0/61F0/6034 -> nombres de Em315Coffin;
    lbl_8087DC40/44 -> sZero_unk/sOne_unk (scope:local, estaticos .sdata del fichero).
  - SpecialEnemyBase.cpp: factoria con `new (3, "", "", 0) Em315Coffin(...)`.
  - Clean build tras el cambio de splits de Em303KrakenFootMp: main.dol OK.

- game/Em312DarkServant (0/21 -> 11/21, NUEVA fuente + cabecera): al 100% dtor, vf0C (busca su amo con
  "master=", tipo 6), vf10, vf14, vf10C (switch 9-11 / 12), fn_8030E490/fn_8030E4AC (`extern "C"`, los llama
  Em006DarkMaster; cuerpos de los inline IsDown_unk/KnockDown_unk), vfFC, fn_8030FEAC (enemigos cercanos),
  fn_8030FFF0/fn_80310084 (efecto). Con NONMATCHING: ctor ~40% (bucles de arrays), vf40 ~95%, fn_8030E530 ~90%,
  fn_8030E5C8 ~85% (recursion en linea, bloqueo documentado), vf30 ~60%. Pendientes: lector del CSV
  (0x8030DEB0), vf100/vf104/vf108 (grandes).
  - symbols.txt: fn_8030DA20/DC9C/DD40/E160/E258/E26C/E36C/E5C8/E6C0/FEAC/FFF0/80310084/80310098 -> nombres de
    Em312DarkServant (fn_8030E490/4AC/530 siguen `extern "C"`).
  - SpecialEnemyBase.cpp: factoria con `new (3, "", "", 0) Em312DarkServant(...)`.

- game/Em306GurgBeast (0/22 -> 8/22, NUEVA fuente + cabecera): al 100% ctor, dtor, vf0C, lector del CSV
  (0x802EE9A8, lista "HomePoint" como FixedArray_unk<RoutePlace_unk*, 8>), vf14, vf10C (reenvia el golpe al
  jinete con `vf3C(hit, 1)`), vf40, vfFC. vf30 ~99.9% (pool: faltan cadenas). Pendientes: vf10/vf100/vf104/vf108
  y las copias debiles fn_802F0964/0988/0990/0998 (las usan FlamingArrow, Cannonball, NewCampTask).
  - Decision sin el usuario: la llamada al jinete usa una vista local `Rider_unk` con vf3C de dos argumentos
    (CharaTask::vf3C de la cabecera compartida tiene uno; no se toca).
  - symbols.txt: fn_802EE604/E7B8/E838/E9A8/EFE0/F01C/F0FC/F124/802F1490 -> nombres de Em306GurgBeast.

- game/Em309GhoulSimple (0/22 -> 8/22, NUEVA fuente + cabecera): al 100% dtor, vf0C (carga en dos pasos con
  m164C), vf14, vf10C, vfFC, vf54, vf64, vf90. ctor ~85% (bucle de ctores de GhoulPair_unk[7] y cero en r30).
  Pendientes: lector del CSV (0x80306078), vf10, vf40, vf100/vf104/vf108, vf20/vf30/vf50, vfB0/vfB4.
  - symbols.txt: fn_80305C74/5EDC/6078/66F0/66F4/6AF8/9698/9704/8030A074/8030A07C -> nombres de Em309GhoulSimple.

- game/Em209SmallGolem (2/22 -> 9/22, NUEVA fuente; cabecera existente ampliada): al 100% dtor, vf14, vf3C,
  vf40, vf84, fn_802CFAE4 (`extern "C"`, lo llama Em209BigGolem) y el `__sinit` (0x802CFFC8, tres colores
  estaticos). vf54 99.9% (recarga de m624.mData, caso abierto de CharaTask::vf54). Pendientes: ctor (std::string
  + boost::tokenizer), vf0C/vf10, vf1C/vf20/vf50/vf80/vf98/vf9C/vfEC.
  - CABECERA (solo la usan Em209SmallGolem/Em209BigGolem): include/game/Em209SmallGolem.h: m14B0 pasa a
    atn::LH_Binary y m1550 a ResHandle (mismo tamano).
  - symbols.txt: fn_802CE42C/E430/E504/ED80/EF0C -> vf14/vf3C/vf40/vf54/vf84; fn_802CFFC8 ->
    `__sinit_\Em209SmallGolem_cpp`; lbl_807C83C8 (0x30) partido en sColor0_unk/sColor1_unk/sColor2_unk
    (0x807C83C8/D8/E8, scope:local).

- game/Em303KrakenFoot (0/26 -> 10/26, NUEVA fuente + cabecera): al 100% ctor, dtor, fn_802E2328 /
  fn_802E2360 / fn_802E2468 (`extern "C"`, los llama Em302Kraken, que aun no tiene fuente), vf14, vf3C,
  fn_802E554C, vfEC, vf24. vf0C 99.7% (orden de los flags 0x2A/0x200, como en KrakenFootMp). Pendientes: lector
  del CSV (0x802E2640), vf10/vf40/vf54 y las funciones de estado grandes.
  - symbols.txt: fn_802E211C/2270/2470/2640/2E84/2E88/554C/58C8/5970 -> nombres de Em303KrakenFoot.
  - SpecialEnemyBase.cpp: factoria con `new (3, "", "", 0) Em303KrakenFoot(...)`.

- game/Em101LittleZesya (0/25 -> 9/25, NUEVA fuente + cabecera): al 100% dtor, vf10, vf3C, fn_8025D6D0 /
  fn_8025D704 / fn_8025FE30 (`extern "C"`, los llama Em101ElderZesya), fn_8025DE54, fn_8025E748, fn_8025F754.
  fn_8025E930 ~95% (f1/f2). Pendientes: ctor (std::string), vf0C, vf40, fn_8025D7B0 y estados grandes.
  - symbols.txt: fn_8025CDE0/D648/D7B0/DE54/E748/E930/F754/F7D0 -> nombres de Em101LittleZesya.

- game/Em401_01_WhiteLeoPard (0/25 -> 1/25, NUEVA fuente + cabecera minima): dtor al 100%; ctor 99.9% (la
  carga de info->m20 en el prologo solo sale con un puntero const, y el simbolo no es const). El resto de
  funciones son grandes (vf10 ~1.8 KB, etc.) o binds de boost::function (invoke/manage 0x8031F48C-0x8031F59C).
  - symbols.txt: fn_8031EB3C/EC5C -> ctor/dtor. SpecialEnemyBase.cpp: factoria case 47 con `new`.

- Segunda pasada: lectores de CSV al 100% en game/Em303KrakenFoot (fn_802E2640, 11/26) y game/Em309GhoulSimple
  (fn_80306078, 9/22; "SummonEnemy" llena un array {id, punto} de 8 con placement new, "SummonWarpPoint" lo
  recorre con puntero).

- game/Em312DarkServant: lector del CSV fn_8030DEB0 al 100% (12/21). symbols.txt: fn_8030DEB0 ->
  `fn_8030DEB0__16Em312DarkServantFPCci`.

- game/Em308ParmCrab (0/24 -> 2/24, NUEVA fuente + cabecera con relleno): dtor (dos vectores envueltos
  CrabList_unk, como ChaserGuard) y vf90 (3.5f) al 100%. El slot 0x60 (0x802FC848, devuelve 8.0f) no se escribe:
  CharaTask::vf60 esta declarada void en la cabecera compartida. Ctor (std::string) sin escribir: la factoria
  sigue llamando a fn_802F6E64.

- game/Em309Ghoul (0/33 -> 2/33, NUEVA fuente + cabecera con relleno): dtor (array de 2 LH_Xsca destruido con
  __destroy_arr) y vf64 al 100%. symbols.txt: fn_802FCF50/F4B4 -> dtor/vf64; fn_802FCF14 ->
  `__ct__Q23atn7LH_XscaFv` (copia debil del ctor, lo usa el ctor de la unidad, sin escribir).

- game/Em202Spider (0/25 -> 1/25) y game/Em202SpiderBase (0/23 -> 1/23): NUEVAS fuentes con cabeceras de
  relleno; los dtors (0x8027CD9C, 0x80282EA4) al 100% (vectores envueltos como ChaserGuard).

- game/Em003LesserDaemon (0/22 -> 4/22, NUEVA fuente + cabecera con relleno): dtor, vf88 (salta a
  fn_8024FCDC), vf14, vfEC al 100%. symbols.txt: fn_8024AB20/BE90/C468/E690/FCDC -> nombres de la clase.

- game/Em310Gargoyle (0/21 -> 2/21, NUEVA fuente + cabecera con relleno): dtor y vf54 al 100%. Las otras
  pequenas (0x8030B408 ctor de un color de 4 floats, 0x8030C190 accesor de nodo, 0x8030DA1C vacia) son copias
  debiles cuyo tipo no esta identificado; no se escriben.

- game/Em305Vampire (0/30 -> 9/30, NUEVA fuente; cabecera existente de Em305VampireMp): al 100% fn_802EABE4,
  fn_802EB97C, vfEC, fn_802EBABC, fn_802EBB14 y las cuatro vacias vf100/vf104/vf108/vfFC. vf14 ~95% (pool en el
  tramo auto). symbols.txt: fn_802E9D38 -> vf14, fn_802EA490 -> `fn_802EA490__12Em305VampireFv`,
  fn_802EC660/664/668/66C -> vf108/vf104/vf100/vfFC. CABECERA (solo Vampire/VampireMp): se declara fn_802EA490.

## Hallazgos MWCC
- vf0C de los enemigos especiales: la cadena de cargas en un unico `if (a == 0 && b == 0 && ...)` reproduce el
  `beq` comun al `return 0` final; con retornos tempranos sale un `li r3,0; b` por condicion.
- Un switch con casos {22, 23, 20} donde 23 cae en el bloque comun: el orden de comparaciones del original
  (22, 23, 20) sale con `case 22: goto reset; case 23: ...; case 20: reset:`.
- Un accesor inline que devuelve `const Vector3&` (`GetPos_unk`) en vez de `p->mDesc->mPos` cambia el orden de
  carga (lwz del puntero antes del lis de la constante).
- `if (p != 0) {...} else { vf100(6); break; }` en vez de `if (p == 0) { vf100(6); break; }` reproduce el orden
  de bloques del original (rama de exito primero).
- `m620 = r; m614 = base + off; m614.y += m620;` (miembro asignado antes de la suma de vectores) fija el orden
  de cargas de FPR del original en el calculo de la capsula.
- `fn_80097C08(..., 1.0f, 0.1f)`: cuando el original reutiliza el registro del 1.0f de la velocidad para el
  sexto argumento, el blend es 1.0f (no 0.0f); comprobar el valor de f1 en la llamada.
- Tres estaticos con constructor seguidos (`static C a(...), b(...), c(...)`) dan el `__sinit` con una base y
  desplazamientos 0/0x10/0x20; un array `static C x[3] = {...}` da otro orden de cargas.
- `bool r = false; if (a != 1 && a != 2) r = true; return r;` da la secuencia cmpwi/beqlr del original; con
  `if (a == 1 || a == 2) return 0; return 1;` MWCC lo convierte en aritmetica sin saltos (cntlzw).
- Un inline `InRange_unk`/accesor del frame que relee el miembro en cada comparacion fija el orden de carga de la
  constante frente al miembro (lfs constante antes del lfs del frame).
- `if (s - 11U > 2) { if (s == 10) {...; return;} } else {...}` coloca el bloque del rango despues, como el
  original; con `if (rango) {...} else if (s == 10)` el orden de bloques se invierte.
- `for (...; p++, i++)` frente a `i++, p++`: el orden de los incrementos del for decide el orden de los addi.
- `while (true) { int id = f(); if (id == 0) break; ... }` reproduce la prueba al principio del bucle; con
  `while ((id = f()) != 0)` MWCC rota el bucle y la prueba va al final.
- El orden de las definiciones en la TU decide el orden de las cadenas en el pool (@stringBase0): una funcion
  escrita al final del fichero pone sus literales despues; hay que colocarla en su sitio (por direccion) para
  que los desplazamientos coincidan.
