# Lote enemies-finish, cola q3 (Em301LShade, Em302Kraken, Em302KrakenMp)

Inicio: 2026-10-22 09:05 CEST. Rama agent-q3 (sale de main 64df7a6). Referencia: /home/claude/ref/start-q3.json.

## Unidades

### game/Em301LShade (0/31 -> 16/31)
- fn_802D5D18 (refuerzos, sellado de miembros del grupo) escrita: NONMATCHING 92.9% (relectura del estado
  dentro del inline "libre" y r26/r29; 8 variantes).
- Unidad SIN FUENTE hasta ahora: fuente nueva `src/game/Em301LShade.cpp` y cabecera nueva
  `include/game/Em301LShade.h` (subclase de SpecialEnemyBase; layout 0x14D4-0x16B0 deducido del ctor, del dtor
  implicito y de las claves del csv; nombres inventados).
- Al 100%: vf14, vf20, vf40, vf64, fn_802D404C, fn_802D4238, fn_802D4E9C, fn_802D5674, fn_802D7058,
  vf100/vf104/vf108/vfFC (vacias) y el dtor (implicito, ultima funcion del fichero).
- vf10 (5.9 KB) escrita (NONMATCHING ~90%: pila de los temporales de color y registros); con ella el pool de
  .sdata2 sale en el orden del original y suben vf64/fn_802D404C/fn_802D4E9C/fn_802D5674 al 100%.
- vf0C 0%: el original llama fuera de linea a todas las inline (presupuesto de inline superado); con una
  candidata extra de ~50 sentencias el cuerpo da 99.16%, asi que al original le sobra codigo inline que no
  sabemos reconstruir (caso abierto, como LoadSeTableTask::vf0C). vf50 99.90% (solo los "" de
  Vector_unk::reserve, que en el original son lbl_8087D9E4/lbl_8087D9E0, como CharaTask::vf50).
- NONMATCHING >= 99% (casi todo es la posicion de las constantes de .sdata2 y de las cadenas del pool, que
  fija vf10 (5.9 KB, sin escribir) y vf0C): ctor, vf54, vf64, fn_802D404C, fn_802D4378, fn_802D4E9C,
  fn_802D5020, fn_802D5674.
- Segunda pasada: fuente ordenada por direccion; fn_802D3370 (rondas del sombra) al 100% con dos
  `ASSERT_UNK` vacios en fn_802D5674 (sin ellos MWCC la inlinea; el original la llama). fn_802D4378 y
  fn_802D5674 al 100%. Escritas: vfEC (99.1%, r5/r6 de la copia de "dead_camera"), fn_802D57B8 (99.2%,
  pila), fn_802D3C74 (99.9%, orden del contador y la constante int->float lbl_80747090, que esta en el
  split de Em209SmallGolem: pendiente), fn_802D6004 (85%, push_back inlineado en bucle: familia conocida),
  fn_802D4628 (85.3%, absorcion; mismo caso abierto que Em302Kraken::fn_802DAA38). Sin escribir:
  fn_802D67F8 (refuerzos, push_back inlineado).
- vf54/vf50 99.92%: la cadena "head" de vf54 y los "" de Vector_unk::reserve dependen de vf0C (0%).

### game/Em302Kraken (0/49 -> 36/49)
- Unidad SIN FUENTE hasta ahora: fuente nueva `src/game/Em302Kraken.cpp` y cabecera nueva
  `include/game/Em302Kraken.h` (subclase directa de EnemyTask: el ctor llama a `__ct__9EnemyTask` y la vtable
  tiene 0xF8 bytes; layout 0x14B0-0x16B8 del ctor/dtor; nombres inventados).
- Al 100%: dtor, vf14, vf3C, vf40, vfEC, fn_802DC5B4, vfD4 (copia debil).
- vf10 escrita (97.5%: registros del bloque de la camara y pila); fija el orden del pool de .sdata2.
- vf0C escrita (99.4%: rotacion de registros salvados del bucle de lectura; 8 variantes). Al 100% las copias
  debiles que usa: fn_802D73E4 (busqueda de comando), fn_802D8540 (limpia un flag del modelo) y
  fn_802D8550/fn_802D8B6C (elemento i de un vector, plantilla `At_unk<T>`).
- Estados 12-15 (fn_802DB534/B7C8/BA3C/BC68) escritos: 98.2-99.9% (solo pool de cadenas/pila).
- Las funciones del .cpp se ordenan por direccion (el pool de cadenas sigue el orden de definicion).
  symbols.txt: fn_802DA718/AA38/B2D0/B534/B7C8/BA3C/BC68/C3D0 -> metodos `__11Em302KrakenFv` (estados).
- NONMATCHING >= 97% (pool de cadenas y .sdata2, que ordenan vf0C/vf10 sin escribir): ctor (98.3%: registros del
  segundo std::string, como Em201ZanGurg), vf20, vf54, vfD0, vfD8, fn_802DDA90 (el original relee el flag de
  0x12A4 del pie; 6 variantes), fn_802DDBB0 y las virtuales inline vf90/vf94/vfC8/vfD4/vfDC (copias debiles).
- La tabla de comandos de "act_command" (`sCommands_unk`, {nombre[0x40], id}) y su busqueda fn_802D73E4 son
  estaticas del fichero.
- Segunda pasada: vfB0 (99.9%), __sinit (99.4%, vector estatico sOffset_unk), estados 2/6/7/9/11/12
  (fn_802DC618, fn_802DBE70, fn_802DC014, fn_802DB2D0, fn_802DC1C0, fn_802DC3D0) y fn_802DC8B0 (siguiente
  comando de las patas, 99.95%): todos 98.7-99.95%, solo offsets del pool de cadenas/.sdata2 (dependen de
  las funciones aun sin escribir). fn_802DBA3C sube a 99.88% con el inline PlayEffectAt_unk.
- fn_802DA718 (estado 4, sombras): NONMATCHING 90.2% - el original redondea x/z con frsp tras el +0.3 y
  guarda las constantes de los dos efectos en registros salvados; 8 variantes.
- Tercera pasada (pool ordenado): fuente ordenada por direccion, instanciacion explicita de los dos
  push_back tras vf0C y la constante 15.0f de vf10 (caso 11, valor sin usar): 36/49. Al 100% vf20, vf54,
  vfB0, vfB4 (+ sus copias boost invoke/manage), vfC8, vfD0, vfD8, fn_802DA174, fn_802DB2D0, los estados
  6/7/11/12/13/14/15, la muerte fn_802DC618, fn_802DC8B0, __sinit. Escritas: vfCC (96.6%, programacion
  del z del temporal; 6 variantes), fn_802DAA38 (86.5%, absorcion: borrador). Sin escribir: fn_802DCA34 y
  fn_802DD1B4 (refuerzos, push_back inlineado). fn_802DB534 98.8% (bucle de pilares en inline; queda la
  programacion de la carga de g->m54).

### game/Em302KrakenMp (0/39 -> 31/39)
- Unidad SIN FUENTE hasta ahora: fuente nueva `src/game/Em302KrakenMp.cpp` y cabecera nueva
  `include/game/Em302KrakenMp.h` (subclase de SpecialEnemyBase, vtable 0x11C; layout 0x14D4-0x1660; nombres
  inventados). Misma tabla de comandos que Em302Kraken pero con ids 8/9/10 ({"swing","pound","wait"}).
- Al 100%: dtor, vf14, vf40, vf108, vfEC, vfFC, vf110.
- NONMATCHING >= 97%: ctor (98.1%, std::string), vf20, vf54, vf100, vf114, fn_802E1A0C (pool/.sdata2),
  vfAC (99.5%, slots de los dos temporales intercambiados; 7 variantes) y las copias debiles vf90/vf94/vfC8.
- vf0C escrita (99.3%, mismo lector que Em302Kraken). Al 100% fn_802DDD1C (busqueda de comando) y
  fn_802DEE18 (`At_unk<Em303KrakenFootMp*>`). symbols.txt: fn_802DE80C/fn_802DEB00 -> push_back de
  vector<Em303KrakenFootMp*> (99.85%) y vector<KrakenMpCommand_unk> (85%, push_back de elementos de 8 bytes:
  familia conocida del push_back de MSL).
- vf10 escrita (98.8%); vf114 sube al 100% con el pool ordenado. Decision: m15DC/m15E0/m15E4 corregidos
  (int, punto de aparicion actual, indice) tras ver los accesos de vf10.
- Segunda pasada: vf3C y fn_802E0890 (espera de refuerzos) al 100%; vfB0 (99.9%), __sinit (99.4%), estados
  2/6/7/9/10 (fn_802E177C, fn_802E10D8, fn_802E1290, fn_802E1414, fn_802E1624) y fn_802E0EC8 (estado de
  ira): 99.8-99.95%, solo offsets del pool.
- Tercera pasada (pool ordenado como en Kraken): 31/39. vf104 al 100% a la primera; fn_802E03F0 al 100%
  con inlines NextAttack_unk/CountActiveFeet_unk; fn_802E0C60, vf100, vf114, vfB4, vfC8 y todos los
  estados al 100%. Quedan: ctor (98.1%, std::string), vf0C (99.3%), vf10 (98.8%), vf50 (99.9%, "" de
  reserve), vfAC (99.8%, 13 variantes), fn_802E094C (81.9%, mismo frsp que Kraken::fn_802DA718) y los
  push_back (99.95% nombre de @STRING@ de otra unidad; 85.5% familia conocida).

## Estado final (2026-10-22 13:46 CEST)
- Em301LShade 16/31, Em302Kraken 36/49, Em302KrakenMp 31/39 funciones al 100% (antes 0/0/0).
- Ninguna unidad enlazada: a todas les quedan NONMATCHING (ctor con std::string, vf0C/vf10 grandes,
  push_back inlineados en bucles y los borradores de absorcion/sombras).
- Sin escribir: Em301LShade fn_802D67F8, Em302Kraken fn_802DCA34/fn_802DD1B4 (refuerzos; los tres son
  push_back de std/vector.h inlineados en bucles, familia que no sale).

## Decisiones tomadas sin el usuario
- **splits.txt**: `.rodata` de game/Em301LShade ampliada de 0x807470A4 a 0x807473F8: absorbe el tramo
  auto_06_807470A4 (pool de cadenas de la unidad, `lbl_807470A4`, que solo usa Em301LShade).
- **symbols.txt**: los 31 simbolos de 0x802D0038-0x802D73E4 renombrados a metodos de Em301LShade (ctor, dtor,
  vf0C, vf10, vf14, vf20, vf3C, vf40, vf50, vf54, vf64, vfEC, vfFC, vf100-vf108 por la vtable; el resto
  `fn_<addr>__11Em301LShadeF...`); fn_802D0B6C -> `IsSet_unk__13ShadeFlag_unkFv` (copia debil de un inline:
  `mGlobal != -1`); lbl_807470A4 -> `@stringBase0` (scope:local).
- game/SpecialEnemyBase.cpp: la factoria (case 29) crea `new (3, "", "", 0) Em301LShade(parent, info)` en lugar
  del `extern "C"` fn_802D0038 (mismo tamano 0x16B0; la factoria sigue en 73.27%).
- **CABECERA COMPARTIDA `include/game/CharaTask.h`**: `vfC8` pasa de `void` a `atn::Vector3` y `vfD4`/`vfDC`
  de `void` a `float` (la base 0x800EF2F0 copia mPos al resultado; 0x800EF2B4/0x800EF270 devuelven f1; las
  copias debiles de Em302Kraken tambien). El tipo de retorno no entra en el nombre y ninguna otra fuente
  las usaba: regress/fuzzdrop 0.
- **symbols.txt (Em302Kraken)**: ctor, dtor, vf0C, vf10, vf14, vf20, vf3C, vf40, vf50, vf54, vfEC, vfCC, vfD0,
  vfD8, vf90, vf94, vfC8, vfD4, vfDC (por la vtable) y fn_802DA174/C618/DA90/DBB0/C5B4 como metodos;
  fn_802D73E4 scope:local; lbl_807473F8 -> `sCommands_unk` (local); lbl_80747654 -> `@stringBase0` (local).
- **symbols.txt (copias debiles de push_back)**: fn_80244CCC -> `push_back__Q23std25vector<P14RoutePlace_unk>...`,
  fn_80244FC0 -> `push_back__Q23std19vector<P9EnemyTask>...`, fn_8000E18C -> `push_back__Q23std9vector<i>FRCi`,
  fn_802D8560/8854 -> push_back de vector<Em303KrakenFoot*>/vector<KrakenCommand_unk>. Son copias debiles
  sin fuente (las otras referencias estan en unidades en asm: Em001Necromancer, Em203GreaterDaemon*,
  GameTask, Em206CocoonMp); con el nombre real objdiff empareja las llamadas. regress/fuzzdrop 0.
- fn_800E0AA8/fn_800E0AB0 (size/empty de un vector, definidas en atn/WordWrap_unk.cpp como Count4_unk) se
  llaman `extern "C"` desde Em302Kraken::vf0C (no se renombran: las usa una unidad enlazada).
- game/SpecialEnemyBase.cpp: la factoria (case 30) crea `new (3, "", "", 0) Em302Kraken(parent, info)`.
- **symbols.txt (Em302KrakenMp)**: ctor, dtor y las 21 virtuales de la vtable renombradas; fn_802E1290/1414/
  1624/177C/1A0C como metodos; fn_802DDD1C scope:local; lbl_807479E8 -> `sCommands_unk` (local);
  lbl_80747B10 -> `@stringBase0` (local). Factoria (case 32) con `new (3, "", "", 0) Em302KrakenMp(...)`.
- **CABECERA COMPARTIDA `include/game/CharaTask.h`**: `vfB0()` pasa a `int vfB0(atn::Vector3* out)` (GameUI
  0x803D3C80 pasa un vector y prueba el resultado; la base 0x800EF31C devuelve 0; los krakens lo rellenan).
  symbols.txt: `vfB0__9CharaTaskFv` -> `vfB0__9CharaTaskFPQ23atn7Vector3` (ninguna fuente la definia; las
  vtables en asm la referencian por el nombre de symbols.txt). regress/fuzzdrop 0.
- **symbols.txt (Em302Kraken/Mp, segunda pasada)**: fn_802DDCFC/fn_802E20FC -> `__sinit_\Em302Kraken[Mp]_cpp`
  (local); fn_802D9938/fn_802E0100 -> vfB0; lbl_80786DC0 -> `sDefaultCommand_unk` (local, comando por
  defecto {8, 0} de fn_802DC8B0); estados como metodos (`fn_802DBE70__11Em302KrakenFi`,
  `fn_802DCA34__11Em302KrakenFii`, `fn_802DD1B4__11Em302KrakenFi`, `fn_802E10D8__13Em302KrakenMpFi`, ...).
- **symbols.txt (segunda/tercera pasada)**: fn_802D9C10/fn_802D9C40 y lbl_807C8408/lbl_8087F3E0/
  lbl_80786E18/lbl_80786F6C con los nombres de boost::bind<..., chr::ControlThrustForKraken*> (copias
  debiles de Em302Kraken::vfB4); lbl_80786F88/lbl_80787114 -> `@STRING@vfC8__11Em302KrakenFv` /
  `__13Em302KrakenMpFv`; lbl_80786DA0 -> `@STRING@__allocate__Q23std25vector<15ShadeTarget_unk>FUi`.
- Layouts cambiados en las cabeceras de la cola: Em302Kraken m14FC y m1518, Em302KrakenMp m1504 y
  Em301LShade m1570 pasan de tres campos sueltos a atn::Vector3 (se acceden como vector).
- Em301LShade::fn_802D5674 lleva dos `ASSERT_UNK(...)` vacios (macro `((void)0)`, patron aceptado de
  game/ColliAttrManager.cpp) para que no se autoinlinee en fn_802D3370.
- Valores sin usar `frame / 15.0f` en Em302Kraken::vf10 y Em302KrakenMp::vf104: el original convierte el
  contador a float y deja 15.0f en el pool en ese punto; es la unica forma encontrada de ordenar .sdata2.
- Em302Kraken::fn_802DC618 llama a fn_802E55B0 (metodo de Em303KrakenFoot, unidad de otra cola) como
  `extern "C"`; no se renombra.
- HitMsg_unk+0x40 (indice de la esfera de dano) se lee con un cast (`*(int*)&msg->m3C[4]`), como en
  Em101ElderZesya, para no tocar CharaTask.h.

## Hallazgos MWCC
- `mPos += m15A0; mPos.y += m14F0;` (operador de Vec.h y suma aparte) reproduce el orden de cargas del
  original en vf10 de los krakens; escrito componente a componente cambia el reparto de FPR.
- Un `int index = m15E4;` leido antes de la comprobacion de tamano fija el orden de cargas (tamano, indice)
  del original.
- Un `CharaTask* target = m1620;` local antes de construir el vector de offset (no despues) evita la segunda
  carga de m1620 en la rotacion (fn_802D4378).
- `float r = m620;` antes de las tres copias a mColli fija el reparto de FPR del original en vf20 de los
  krakens (Em302Kraken/Em302KrakenMp).

- Un inline `PlayEffectAt_unk(owner, id, efp, pos, scale)` (fn_80232B7C + color + fn_8023A8B4) reproduce el
  orden de pila del original (pos antes que color) donde escribirlo en linea no lo hace.
- Leer `EffectMgr_unk* mgr = lbl_8087F3C0;` despues de rellenar los vectores/color (en el inline) mueve la
  carga del manager al final, como en los estados 9/11 de los krakens.
- `Item14_unk* it = m624.mData;` y luego `it->m04.mCenter` fija el reparto r5/r6 del original en vfB0.
- El pool de cadenas sigue el orden de definicion tambien para las copias debiles: instanciar
  explicitamente los push_back justo detras de vf0C (`template void std::vector<T>::push_back(...)`) coloca
  "vector length error" donde el original.
- Un bucle de recuento metido en un inline (`CountFreeFeet_unk`) reproduce el reparto de registros del
  original en las maquinas de estados de los krakens; escrito en linea salen intercambiados.
- En las copias de vfC8 el original carga la traslacion del nodo en orden z, y, x (locales en ese orden).
- La camara de eventos (Field_unk+0x568) se copia con el patron de CameraMgr_unk::SetParam_unk
  (dos Vector3 a traves de un puntero) en Em301LShade::vfEC/fn_802D57B8.

## Pendientes para otras colas
- game/Em209SmallGolem (cola q2): sus `.rodata` terminan en 0x80747098 pero los tres double
  lbl_80747080/88/90 (2*pi y las constantes int->float) solo los usa Em301LShade; probablemente son de
  Em301LShade (split a 0x80747080). No lo toco (limite de otra cola); impide el 100% de fn_802D3C74,
  fn_802D5020 y la parte de vf10 que convierte enteros.
- Em303KrakenFoot: fn_802E55B0 (llamada por Em302Kraken::fn_802DC618 como extern "C") deberia ser un metodo
  (`fn_802E55B0__15Em303KrakenFootFi`); al nombrarlo hay que actualizar el extern de Em302Kraken.cpp.
