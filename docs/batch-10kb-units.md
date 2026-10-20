# Lote batch-10kb-units (2026-10-20): notas de los agentes

Notas completas de las 9 colas del lote de unidades de 5-10 KB (resultado por unidad, NONMATCHING con motivo, decisiones tomadas sin el usuario y hallazgos de MWCC). Resumen en CLAUDE.md, seccion "Estado actual".

---

# Cola q1_chr

## q1_chr - notas del agente (rama agent-q1_chr, copia /home/claude/work/q1_chr)

## chr/ControlCannonLift (commit cda5c4b)
- Resultado: 13/17 funciones al 100%, matching: no (la unidad define la estatica local
  `fn_8014F8F4()::sListeners` inlineada, igual que gmk/Item; no puede enlazar).
- NONMATCHING:
  - `ControlCannonLift::vf10` 97.8% - asignacion de registros del ctor de ControlCannonTarget inlineado + slots de pila.
  - `ControlCannonThrow::vf0C` 95.8% - slots de pila de los vectores punto/offset/diferencia.
  - `ControlCannonTarget::ControlCannonTarget` 97.1% - slot de pila del vector direccion; recarga de mChara para m1208.
  - `ControlCannonTarget::vf0C` 98.4% - slots de pila y orden de registros salvados.
- Decisiones sin el usuario:
  - splits.txt: ControlCannonLift ampliada sobre huecos auto (sin tocar vecinas):
    .text hasta 0x801A2290 (incluye `__sinit`), .ctors 0x8072D2DC-0x8072D2E0, .rodata 0x80739F70-0x8073A090
    (cadena boost bind_t de ControlCannonTarget, nombres RTTI, ControlMoveCursor RTTI debil, @stringBase0 ""),
    .data 0x8077EFB0-0x8077F178 (ptmf, type_info boost, vtables, RTTI ControlMoveCursor, "Memory allocation failure"),
    .bss 0x807C7BD0-0x807C7BF0, .sdata2 hasta 0x80882168. Motivo: sin ello las funciones que referencian esos datos no llegan al 100%.
  - symbols.txt (renombrados): fn_801C3414 -> ctor ControlMoveCursor, fn_801C3458 -> ControlMoveCursor::fn_801C3458,
    fn_80191960/fn_8019198C -> invoke/manage del bind mf0<CharaTask> (fn_8017518C), lbl_807C7B90/lbl_8087F0BE y
    lbl_807C7BE8/lbl_8087F0C8 -> @LOCAL@/@GUARD@ stored_vtable, lbl_807C7BD0 -> sQueenOffset_unk (local),
    lbl_8073A088 -> @stringBase0, lbl_8077EFC8 -> __RTTI__ bind_t, lbl_8077F158 -> @STRING@allocate.
  - Cabeceras nuevas: include/chr/ControlMoveCursor.h (clase base; vf58/dtor inline, ctor fuera de linea) y
    include/chr/ControlCannonLift.h. CharaTask.cpp sigue con sus declaraciones locales (no tocado).
  - Tipos inferidos: Cannon_unk (m520 slot, m524 peticion arena), Queen_unk (enemigo id 4, m1544), CursorTarget_unk.
- Hallazgos MWCC:
  - Ctor no inline que se autoinlinea en otra funcion del fichero: reducir sentencias con un helper inline (PlayMotion_unk) lo consigue.
  - Una condicion `a && b && ...` repetida: un helper inline que devuelve puntero/bool anade un test extra; escribirla en linea.
  - `GimmickMsg_unk msg; msg.mType = N;` (ctor por defecto + asignacion) en vez de `msg(N)` reproduce el orden de stores.
  - Locales con nombre: en nuestras pruebas quedan en orden ascendente de declaracion y por encima de los temporales.

## chr/ControlCliffhang (commit e9ed7de)
- Resultado: 3/8 al 100% (ctor, vf14, dtor), matching: no.
- NONMATCHING: vf0C 99.3% (slots de pila estados 0/2, FPR), vf10 98.5% (m94 cargado antes de guardar r31),
  fn_801AB6B8 99.9% (r3/r4), fn_801AB9E4 96.4% (slots de pila de vectores), fn_801AC14C 99.1% (rama vacia deja `li r4,-1`).
- Decisiones:
  - splits.txt: .rodata empieza en 0x8073A958 (double 2pi de fmod) y .data en 0x8077FB48 (ptmf vf8C y fn_801696E0); solo absorbe huecos auto.
  - symbols.txt: renombrados de las funciones de la unidad; lbl_8077CF44 (en el rango de ControlAppear) -> @STRING@allocate del bind
    mf1<CharaTask,const Vector3&> (la cadena "Memory allocation failure" que referencia el manage debil).
  - include/chr/ControlCliffhang.h (compartida con CharaTask.cpp): miembros con nombre en lugar de array; mismo tamano 0x9C; regress sin perdidas.
- Hallazgos MWCC:
  - Un `float t = 0.0f;` con nombre (luego reasignado) basta para poner 0.0f primero en .sdata2 (arreglo 2 funciones).
  - Lerp con el producto primero `(b - a) * t + a` (fadds prod, a) frente a Vec.h Lerp_unk (a + prod).
  - `if (!(x < eps))` genera `blt`; `x >= eps` genera cror+bne.

## chr/ControlDive (commit 39e3932)
- Resultado: 19/25 al 100%, matching: no.
- NONMATCHING: ControlDive::vf0C 85.0% (args de fn_803C1560 cargados antes de fn_80179D44, r27-r31), ControlSwatTurn::vf0C 99.5%,
  ControlCoverSlip::vf0C 98.9% y ControlBackStep::vf0C 97.4% (`ret` en r31 en vez de r30), ControlSwapPos::vf0C 99.8% (r3-r6),
  ControlBackStep ctor 91.6% (r30/r31 y orden de restas del vector negado).
- Decisiones:
  - splits.txt: .rodata de ControlDive empieza en 0x8073AB90 (double 2pi del fmod de SwapPos, estaba en hueco auto).
  - Fuera de mi unidad (solo renombrado/tipo, regress sin perdidas): src/game/CharaTask.cpp ya no declara localmente
    Control801B11BC_unk/Control801B151C_unk/Control801B0D88_unk/ControlBackStep/ControlSwapPos: usa include/chr/ControlDive.h
    (nombres reales ControlSwatTurn/ControlCoverSlip/ControlDive por la vtable). include/game/CharaTask.h:
    fn_80154EC4(int,int) -> fn_80154EC4(const atn::Vector3&, int) (el ctor de SwatTurn lee un Vector3; BattleCharaTask pasa una direccion);
    symbols.txt fn_80154EC4__9CharaTaskFii -> fn_80154EC4__9CharaTaskFRCQ23atn7Vector3i. include/chr/ControlStool.h ahora solo incluye ControlDive.h.
  - symbols.txt: ctors/vf/dtors de las 7 clases con nombre real (ControlWarp ctor 0x801B2480 incluido).
- Hallazgos MWCC:
  - Orden de stores del ctor: un miembro asignado en el cuerpo (no en la lista de init) cambia el orden; se ve en que miembro se guarda antes.
  - `--x <= 0` da `cntlzw + rlwnm r3, 1, r0, 31,31`; `--x == 0` da `cntlzw + srwi 5`.
  - Bloque grande en `else { ...; return 0; }` con el codigo comun al final reproduce el layout de ramas.
  - Helper inline con la tabla como parametro (`Find(lbl->m10D8, chara)`) carga la tabla antes de la llamada interna.

## chr/ControlInteractionBase (commit baf1d61)
- Resultado: 13/21 al 100% (incluye 9 dtors, ctors DuckUnderA/DuckJoist, DuckUnderA::vf0C, DiveIntoWater::vf14), matching: no.
- NONMATCHING: base vf0C 99.4% (r6/r7), DuckUnderB ctor 91.9% (registros salvados), Constrict/BumpHead/StepOn/ThroughRope ~93-95%
  (frsp de dir.z antes del fabs en el ToRotation inline: otra forma de la inline), DiveIntoWater ctor 97.6% y vf0C 97.7% (registros).
- Decisiones:
  - splits.txt: la unidad real es .text 0x801B7CD0-0x801B9A54 (dtor de DiveIntoWater estaba en hueco auto), .rodata 0x8073B200-0x8073B308
    (double de conversion int->float, nombre RTTI de DiveIntoWater, @stringBase0 "neck/spine1/hips/reference"), .data desde 0x80780FC0
    (vtable de ControlDiveIntoWater). No toca unidades vecinas (0x8073B308 = double de ControlLadder queda en hueco auto).
  - symbols.txt: lbl_8073B2E8 -> @stringBase0 (local) y nombres de vf0C/vf14/dtors.
  - include/chr/ControlInteractionBase.h (compartida con CharaTask.cpp): layout real (m08..m3C), mismo tamano 0x40; regress sin perdidas.
  - Tipo inferido: el ctor base, Setup_unk y GetBodyPos_unk se definen NO inline en el .cpp (se autoinlinean); sus copias fuera de linea
    las elimino el enlazador original: es lo que explica el orden del pool .sdata2 (1, 0, 0.1 antes que el 3 de vf0C, eps/pi/2 antes que 0.5).
- Hallazgos MWCC:
  - Constantes de .sdata2 "adelantadas" sin uso visible = funcion definida antes en el .cpp, autoinlineada y eliminada por el enlazador.
  - Un parametro `Vector3& out` de un helper autoinlineado mantiene &local en un registro salvado (como el original).
  - `if (x >= y) return 1; return 0;` da ramas; `return x >= y;` da mfcr/extrwi.

## chr/ControlWallUp (ver commit "Decomp chr/ControlWallUp (16/24 funciones)")
- Resultado: 16/24 al 100% (invokers/managers de 3 bind, WallDown vf0C/vf1C, dtors, vf1C/vf28/vf54...), matching: no.
- NONMATCHING: WallUp ctor 97.8%, vf0C 99.8%, vf10 99.8%, vf40 98.8% (registros); WallSlash ctor 99.6% (frsp en ToRotation),
  vf0C 99.7% (registros salvados); WallSlash vf14/vf18 60% (copia del Vector3 devuelto: z antes del psq, como GimmickCommon::vf54).
- Decisiones:
  - splits.txt: .rodata 0x8073B9A8-0x8073BC20 (double 2pi, 3 nombres boost bind, @stringBase0 "" en 0x8073BC18) y .data 0x807822D0-0x807824D8
    (3 ptmf, 3 type_info boost, 3 "Memory allocation failure"); solo huecos auto.
  - include/game/CharaTask.h (compartida): `virtual void vfB4()` -> `virtual boost::function<void()> vfB4(CharaTask* chara)` (la copia debil
    0x800EF310 escribe 0 en r3 = devuelve boost::function vacia; ControlWallUp::vf10 la llama con mChara) y se anade #include <boost/function.hpp>;
    symbols.txt vfB4__9CharaTaskFv -> vfB4__9CharaTaskFP9CharaTask. regress sin perdidas.
  - symbols.txt: lbl_8078249C/lbl_807824B8/lbl_80782480 -> @STRING@allocate de los 3 bind (lbl_8078249C tambien lo usan Em205Ogre y Em302KrakenMp
    en asm); stored_vtable/guards y __RTTI__ de los bind; lbl_8073BC18 -> @stringBase0.
  - include/chr/ControlWallUp.h (compartida con CharaTask.cpp): clases completas WallUp/WallDown/WallSlash, mismo tamano.
- Hallazgos MWCC:
  - MIN/MAX tipo macro con reevaluacion: el operando se repite tal cual (tambien una llamada inline `ratio()` dos veces).
  - `x > 0 ? x : 0` da neg/andc/srawi/and; `x < 0 ? 0 : x` da srawi/andc.
  - Ratio `frame / fn()` con la carga antes de la llamada: helper inline con `float f = m->frame; return f / m->fn(0);`.

## chr/ControlPushedDown (ver commit "Decomp chr/ControlPushedDown (10/14 funciones)")
- Resultado: 10/14 al 100% (PlaySe_unk, CanComplain_unk, ctor de PushedDown y de Ottotto, fn_801C59D4, Avoid vf1C/vf10, 3 dtors), matching: no.
- NONMATCHING: PushedDown vf0C 98.7% (registros salvados r31/r30 y f31/f30/f29, slot del vector de distancia),
  Ottotto vf0C 99.1% (contador en r5 en vez de r0, slots del vector negado), Avoid ctor 99.9% (slots up/front cambiados),
  Avoid vf0C 99.3% (registros de temporales, orden de los fneg, direccion del final del segmento).
- Decisiones:
  - splits.txt: unidad real .text desde 0x801C4738 (PlaySe_unk y CanComplain_unk estaban en hueco auto), .rodata 0x8073B778-0x8073B810
    (nombre SE, doubles 2pi e int->float, RTTI, @stringBase0 con los 8 modelos y ""), .data desde 0x80781D58 (ptmf de fn_80178208),
    .bss 0x807C7C28-0x807C7C30 (stored_vtable del bind de ControlChaseFollow), .sdata2 desde 0x808827A8 (tabla sSe). Solo huecos auto.
  - symbols.txt: simbolo nuevo `pad_8073B774` (.rodata, 4 bytes, local): relleno de alineacion a 8 entre ControlOneMotion (fin 0x8073B774)
    y esta unidad; sin el, dtk alarga el gap_06_8073B772 de ControlOneMotion fuera de su seccion y objdiff falla (no se toca su split).
  - symbols.txt: nombres de vf0C/vf10/vf1C/dtors, fn_801C4738 -> PlaySe_unk__FiPCQ23atn7Vector3f, fn_801C4750 -> CanComplain_unk__FP9CharaTask,
    lbl_8073B7DC -> @stringBase0 (local); del bind de ControlChaseFollow: fn_801A86C0/fn_801A86F0 -> invoke/manage (estan en el hueco de
    ControlChaseCaught), lbl_8077FAF0 -> @STRING@allocate, lbl_807C7C28 -> @LOCAL@ stored_vtable, lbl_8087F0E2 -> @GUARD@.
  - include/chr/ControlPushedDown.h (compartida con CharaTask.cpp): layout completo de las 3 clases, mismos tamanos (0x3C/0x18/0x3C). regress sin perdidas.
- Hallazgos MWCC:
  - Orden de slots de pila: primero los locales con nombre (en orden de declaracion, direcciones descendentes) y despues los temporales;
    un `Vector3 r = v.ToRotation_unk()` con nombre cambia todos los slots: usar `v.ToRotation_unk().y` directamente cuando el original lo hace.
  - Constantes de un inline (p.ej. el 1000.0f de Rand01) entran en el pool antes que las de la expresion que lo llama: escribir la expresion
    `(float)(rand() % 1001) / 1000.0f` a mano cuando el pool las tiene en orden lexico.
  - `if (strstr(...)) return 0; return 1;` da cntlzw y deja las cadenas en orden; `return strstr(...) == 0;` adelanta esa cadena en @stringBase0.
  - Variable `lim` asignada despues de la llamada (no al principio) evita que se quede en un registro salvado; `-lim` da fneg, `-0.17f` literal da otra constante.
  - El intercambio r3/r4 de `mChara->m570 = 0; mChara->m574 = zero;` (el NONMATCHING de ControlOneMotion) se arregla con
    `Chara_unk* c2 = mChara; c2->m570 = 0.0f; mChara->m574 = ...;`.
  - Bitfield leido dos veces (test y borrado) = accesor inline que devuelve `unsigned int` (con bool sale cntlzw).

---

# Cola q2_enemy

## q2_enemy - notas (rama agent-q2_enemy, copia /home/claude/work/q2_enemy)

## game/EnemySeaLizerd - 10/13 al 100%, matching: no (commit 266a0ab)
- NONMATCHING: vf10 99.9% (r28/r29 entre gestor de balas y datos de bala); fn_80354EE4 98.4% (orden de
  stores del primer vector, scheduling de dir.y += 8, f0/f5 al final); fn_80355390 98.9% (carga temprana
  de mC58.mC4 en r6, f4/f6).
- Decisiones:
  - splits.txt: .rodata de EnemySeaLizerd ampliada a 0x8074AF38-0x8074B088 (dobles de fmod/int->float
    al principio y @stringBase0 de 0x130 al final; el split inferido solo cubria el nombre RTTI).
  - symbols.txt: lbl_8074AF58 -> @stringBase0 (scope:local, string_table); nombres de las 13 funciones.
  - include/game/BattleCharaTask.h: vfE4 pasa de void a int (EnemySeaLizerd::vfE4 devuelve 1 o hace tail
    call a BattleCharaTask::vfE4). Ningun otro fuente lo usaba.
  - include/game/CharaTask.h: accesores IsFlag2_unk()/IsFlag3_unk() (inline const, no cambian layout).
  - Tipo nuevo include/game/EnemySeaLizerd.h (miembros 0x14B0-0x14F0 inferidos del ctor y del csv).
- MWCC:
  - Recarga del word de bitfields entre dos tests: accesor bool const + comparacion explicita
    (`if (IsFlag2_unk() == false) { if (IsFlag3_unk() == true)`); con `!acc()`/`acc()` no recarga.
  - `path = s` (operator=(const char*)) en vez de `path.assign(s)` cambia el orden de los temporales
    del iterator tag en la pila.
  - `float t = a * b;` sigue fusionandose en fmadds al usarse; `t = a; t *= b;` lo evita.
  - Un puntero intermedio (`Vector3* v = &node->m04`) o un local `CharaTask* target = m14C0;` intercambia
    r3/r4 entre un puntero y la direccion de un temporal.
  - Vector desde inline que recibe `CharaTask* const&` recarga el puntero del miembro tras la llamada.
  - fmod/WrapAngle_unk usa un double 6.2831855 en .rodata del propio fichero.

## game/Np131LivingArmor - 8/11 al 100%, matching: no (commit 088976b)
- NONMATCHING: vf10 96.8% y fn_80546B84 96.4% (std::vector<GimmickCommon*>::push_back inlinado: el
  original carga size antes que capacity, caso abierto de include/std/vector.h; huecos de pila;
  en fn_80546B84 `this` va en r18); fn_80547468 99.9% (orden de stores z,x,y del vector dir).
- Decisiones:
  - splits.txt: .text hasta 0x805477C8 (fn_80547468 y fn_80547708 eran de esta unidad: usan su pool y
    sus miembros); .rodata 0x8075E300-0x8075E380 (dobles + @stringBase0); .sdata2 hasta 0x80887D38;
    .sdata 0x8087E4D8-0x8087E4E8 (4 floats estaticos del fundido) y .sbss 0x8087F888-0x8087F890
    (static int sChecked) anadidos.
  - symbols.txt: nombres de funciones, @stringBase0 (0x8075E324), sFade* y sChecked (scope:local).
    fn_80547708 se queda con nombre C: chr/ControlGmkCoffinOpening.cpp la declara extern "C".
  - include/game/CharaTask.h: vfC0() -> vfC0(void* draw); symbols.txt vfC0__9CharaTaskFv ->
    vfC0__9CharaTaskFPv (renombrado). regress sin perdidas.
  - Cabecera nueva include/game/Np131LivingArmor.h.
- MWCC:
  - `mStatus.m18 |= 0x80; |= 0x100; |= 0x10000; |= 0x40; |= 0x1;` (5 sentencias) reproduce el
    ori/oris/ori del original; agrupar constantes cambia el scheduling.
  - Ctor con `const EnemyInfo_unk* info`: la carga de info->m20 sube al prologo (ya visto en
    CopyRobotTask; Em002_02SkeltonKing tiene ese NONMATCHING, probablemente se arregla igual).
  - Struct por valor como argumento (fn_803E2A30(..., atn::Vector3 pos, ...)) explica una copia en
    un temporal de la pila justo antes de la llamada.
  - Un local `Vector3` muerto (pos.y += 8 sin uso) se mantiene en el codigo por el asm de las copias.

## game/EnemyLizerdBlade - 12/13 al 100%, matching: no (commit cd2adf8)
- NONMATCHING: fn_80334460 91.4% (orden de stores de `dir`, scheduling de las copias de m14E8 y del
  clear de CollResult, que el original hace tras fn_80179D44; registros float de la interpolacion).
- Decisiones:
  - splits.txt: .rodata 0x8074A138-0x8074A298 (doble 2*pi de fmod + nombre RTTI + @stringBase0).
  - symbols.txt: nombres de las 13 funciones y @stringBase0 (0x8074A154).
  - include/game/CharaTask.h: CharaC58_unk gana `int m2B0` (0xF08 en CharaTask) partiendo el relleno
    m140 (layout igual).
  - Cabecera nueva include/game/EnemyLizerdBlade.h.
- MWCC:
  - Vector local inicializado `v.z = ...; v.x = ...; v.y = 0.0f;` reproduce el orden de stores z,x,y
    cuando el original resta z primero (fn_80333D68); no siempre (fn_80334460, Np131LivingArmor).
  - `m14D4 = m14D4 + g->m3A4` en vez de `+=` cambia los registros f0/f3 de la suma.
  - Un `x++` de miembro antes de stores float se programa antes; moverlo detras de esos stores
    reproduce el orden original.
  - Vector devuelto por una inline (frente de un personaje: `GetFront_unk(this) * 14.0f`) coloca la
    Mtx y el vector en los huecos de pila del original.
  - `ToRotation_unk().y` como temporal (sin local) deja el temporal debajo de los locales con nombre.

## game/EnemyQuarkStand - no empezada (aplazada)
- El split (0x80352EC8-0x803545E0, 20 funciones) es solo la cola del fichero real: el bloque auto
  0x8033C3D8-0x80352EC8 (102 funciones, 0x16AF0 bytes) usa el mismo pool de cadenas lbl_8074AA58
  (0x4E0 bytes, 63 referencias), las constantes .sdata2 desde 0x80885298 y la .data desde 0x807894B8
  (jumptables de vf10/vf14, ptmf, cadenas "Memory allocation failure" en 0x80789680/0x8078969C), y
  la vtable apunta a funciones del bloque auto (vf0C 0x80345BA8, vf10 0x80346384...). Las funciones
  de la cola usan floats/cadenas de esos pools y miembros hasta 0x1CD4, asi que no pueden llegar al
  100% sin decompilar el fichero entero (~98 KB). Sin cambios en splits.

## game/EnemyFieldMagician - 8/11 al 100%, matching: no (commit 12ebc46)
- NONMATCHING: fn_8032CF10 96.3% (huecos de pila: el offset de las matrices de colision queda
  debajo de los locales de PlayFieldEffect_unk); fn_8032D1F4 98.8% (mRot.y leido por r3 antes de
  pasar this a r31); vf3C 98.9% (registros volatiles de la copia del segmento, orden de mr).
- Decisiones:
  - splits.txt: .rodata 0x80749D70-0x80749F58 (tres cadenas SE_SP007_00x de 0xD alineadas + tabla
    de punteros lbl_80749DA0 + nombre RTTI + @stringBase0 de 0x198).
  - symbols.txt: nombres de las 11 funciones, @stringBase0 (0x80749DC0) y fn_80059550 ->
    __dt__Q23atn11ColliObjectFv (destructor de atn::ColliObject en atn_colli, usado por
    __construct_array). main.dol OK y regress sin perdidas.
  - include/atn/ColliBase.h: vf10() -> int vf10(void* result, int mask, const void* segment, int, int)
    (test de segmento; nadie lo llamaba ni definia en src).
  - Cabecera nueva include/game/EnemyFieldMagician.h (atn::FixedArray_unk<RoutePlace_unk*, 8>).
- MWCC:
  - Las cadenas de un `static const char* const tabla[] = {"..", ..}` salen como objetos .rodata
    separados alineados a 8 (no en @stringBase0), seguidos de la tabla.
  - Locales de una inline van en orden ascendente; el mismo bloque escrito a mano en la funcion va en
    orden inverso (efecto con pos/rot/color en vf10 vs fn_8032D48C).
  - Un argumento temporal `Color4f_unk(1, 1, 1, 1)` queda encima de los locales de la inline.
  - bool locales: el que se declara antes recibe el registro mas alto.

## game/Em316LastLaser - no empezada
- El fichero real es .text 0x8031B0A8-0x8031EB3C (incluye la clase LaserTrace, bloque auto de 10
  funciones), .rodata 0x80749718-0x80749898, .data 0x80788890-0x807889D8, .sdata2
  0x80884DF0-0x80884E48. El ctor llama fuera de linea a ctores de std::string/tokenizer/Vector3
  (caso abierto del presupuesto de inline). Se probo extender el split y se revirtio; sin cambios.

## game/Em305VampireMp - no empezada
- Deriva de Em305Vampire (SpecialEnemyBase), sin decompilar; vf0C usa muchos campos de la base.
  Requiere decompilar antes Em305Vampire. Sin cambios.

## game/EnemyTrista - no empezada (solo analisis)
- Fichero real: .text hasta 0x8035DF48 (fn_8035DF08 = dtor de @unnamed@enemy_trista_cpp@::
  ControlShingan), .rodata 0x8074B398-0x8074B528, .data 0x80789B00-0x80789CB8, .sdata2
  0x80885600-0x80885670. Layout de la clase anotado (0x14B0 LH_Binary ... 0x15B4 ResHandle).
- Bloqueo: el target pasa f1=3.0 a CharaTask::fn_8015783C, cuya definicion en
  src/game/CharaTask.cpp no tiene parametro float (habria que cambiar firma compartida). Sin cambios.

---

# Cola q3_gmk

## Notas q3_gmk (rama agent-q3_gmk, /home/claude/work/q3_gmk)

## gmk/Lemon (commit 3642079)
- Resultado: 12/14 al 100%, matching: no (entrada ya existia en game_units.json con matching false).
- NONMATCHING:
  - fn_80415F6C (79.6%): reparto de registros/planificacion; el original copia el 2o vector del segmento (sin uso) con su propio registro de direccion (0xB4) y calcula la velocidad reflejada en otro orden.
  - fn_80416268 (85.7%): pila 16 bytes mayor y tres stores muertos de floats (0x98, 0xC, 0x1C) de temporales desconocidos; registros distintos.
- Decisiones:
  - splits.txt: .rodata de gmk/Lemon ampliada a 0x80752E90-0x80752EC0 (incluye el double de conversion int->float y su @stringBase0, que estaban sin asignar).
  - symbols.txt: lbl_80752EA4 -> @stringBase0 (scope:local); nombres de funciones Lemon por pairnames + fn_80415F6C/fn_80416268 a mano.
  - symbols.txt + src/atn/ModelInstance.cpp: fn_8009E6EC__Q23atn13ModelInstanceFv -> ...Fi (recibe un int sin usar: los llamadores pasan 0/1 en r4, y fn_80490EA0 reenvia su r4). Solo cambia la firma.
  - src/game/GimmickCommon.cpp: incluye gmk/Lemon.h y llama a gmk::Lemon::fn_8041537C (antes extern "C" fn_8041537C).
  - include/gmk/Lemon.h nuevo: contiene una vista minima de atn::ModelInstance (0x88 bytes, ctor/dtor/fn_8009E6EC/fn_8009EE30) y CollResult_unk con ctor inline (el de CharaTask.cpp es fuera de linea 0x80140500).
  - fn_8041524C (fabrica por id) no tiene referencias: cuando Lemon se enlace hara falta force_active.
- Hallazgos MWCC:
  - Ctor no inline definido en el .cpp despues de las fabricas: se autoinlinea y sus literales van a @stringBase0 tras el "" de new.
  - fneg de componentes de un Vector3 copiado con psq sale con frsp (lo da Vector3(-p.x, -p.y, -p.z)).

## gmk/Coffin (commit 511924f)
- Resultado: 14/14 al 100%, matching: si (main.dol OK, regress sin perdidas).
- Decisiones:
  - splits.txt: .rodata de gmk/Coffin ampliada a 0x80752CC0-0x80752D90 (tabla de 8 pares arg->contador, double de fmod, nombre RTTI y @stringBase0).
  - symbols.txt: fn_80059550 -> __dt__Q23atn11ColliObjectFv (copia debil del dtor inline de ColliObject, usada por __destroy_arr); lbl_80752CC0 -> sSlots (scope:local); lbl_80752D14 -> @stringBase0; resto por pairnames.
  - src/game/GimmickCommon.cpp: incluye gmk/Coffin.h y llama a gmk::Coffin::fn_80411440.
  - include/gmk/Coffin.h nuevo.
- Hallazgos MWCC:
  - Ctor no inline en el .cpp justo tras la fabrica: si no se inlinea sale en orden de fuente (en Lemon se inlineaba).
  - Locales de un helper inline (PlayEfp_unk) van en orden ascendente de declaracion y por encima de los locales con nombre del llamador; el global pasado como argumento al inline se carga antes que el resto.
  - `Vector3 v; v = a - b;` (asignacion) deja temporal + copia psq; `Vector3 v = a - b;` construye en sitio.
  - Una busqueda en tabla dentro de un helper inline (return en el bucle) evita que la funcion llamadora se compile antes que su llamador.
  - Variable de puntero declarada al inicio de la funcion (usada en un case) recibe registro mas alto que una declarada en el bloque de otro case.

## gmk/RandomItem (commit siguiente a 511924f, "Decomp gmk/RandomItem (12/14)")
- Resultado: 12/14 al 100% dentro del split actual, matching: no.
- NONMATCHING:
  - vf14 (99.9%): dos direcciones invariantes del bucle (temporal de PSMTXConcat y matriz) se izan en el otro orden.
  - vf20 (98.8%): reparto de registros en las copias inline de Vector_unk (reserve/Assign_unk): origen/destino de los bucles de copia intercambiados.
- AVISO split: la unidad real es mas grande. Las funciones 0x8043465C-0x80435CE4 (fn_8043465C, fn_804348C4, fn_80435684, fn_80435908, fn_80435CBC; hoy en auto_03_8043465C_text) usan las constantes .sdata2 de RandomItem y son metodos suyos (this en r3, llamados desde vf10). Tambien .rodata 0x807541A0 (8 bytes) y .sdata2 0x808868A8-0x808868C8 y las 6 @STRING@ de .sdata 0x8087DFD8-0x8087DFEC. No he ampliado .text (no estan decompiladas); si se hace, ampliar text/rodata/sdata2 y anadir .sdata.
- Decisiones:
  - splits.txt: .rodata de gmk/RandomItem ampliada hasta 0x807541E0 (@stringBase0).
  - symbols.txt: lbl_807541B8 -> @stringBase0; fn_8043465C/fn_804348C4/fn_80435684 -> metodos de gmk::RandomItem (mangled); 6 lbl_8087DFD8..EC -> @STRING@reserve/Resize_unk de Vector_unk<...>; resto por pairnames (incluye ctor/dtor de RandomItemDrop_unk y ctor de RandomItemLoc_unk).
  - CABECERA COMPARTIDA include/atn/Array_unk.h: anadido Vector_unk::operator= que llama a Assign_unk (la copia implicita del elemento de 0x830 bytes copia el FixedArray en bloque solo asi). regress: sin perdidas.
  - src/game/GimmickCommon.cpp: incluye gmk/RandomItem.h y llama a gmk::RandomItem::fn_80432CF0.
- Hallazgos MWCC:
  - Copia implicita de una clase con miembro de clase con operator= de usuario + FixedArray: el FixedArray se copia en bloque (bucle lwzu/stwu); con operator= de usuario en la clase exterior, miembro a miembro.
  - Vector_unk<T> con T con ctor y sin dtor: new[] con cookie (0x10) y delete[] con subi 0x10 + __dla; con T sin ctor, sin cookie.
  - Puntero declarado fuera del bucle (asignado dentro) recibe registro mas alto que el contador declarado despues.

## gmk/Brazier ("Decomp gmk/Brazier (11/16)")
- Resultado: 11/16 al 100%, matching: no (ademas, como gmk/Item, el .o define el static local sListeners de fn_8014F8F4 y su guard; no se podria enlazar tal cual).
- NONMATCHING: vf10 99.96% y vf14 99.94% (orden de creacion de las constantes 0.5f/2.5f en .sdata2), vf40 99.82% (r27/r28 de player/gestor de balas; objeto de registro lbl_807C8878 local), fn_80407D24 99.83% (f3 en vez de f1), fn_80407E8C 99.36% (el original recarga lbl_8087F610 en el bucle de la arena).
- Decisiones:
  - splits.txt: .rodata de gmk/Brazier ampliada a 0x807528C0-0x80752948 (dos cadenas SE, tabla sSe de 2 punteros, double, nombre RTTI, @stringBase0).
  - symbols.txt: lbl_807528E0 -> sSe (size 0x8, scope:local; borrada la entrada lbl_807528E4 que era su 2o elemento); lbl_80752900 -> @stringBase0; funciones por pairnames + vf40/vf44/fn_80407E8C a mano.
  - CABECERA COMPARTIDA include/game/GimmickCommon.h: GimmickHitMsg_unk::m1C pasa de bytes a atn::Vector3 mDir (+ m28[8]); solo nombres, mismo layout. regress sin perdidas.
  - src/game/GimmickCommon.cpp: incluye gmk/Brazier.h y llama a sus dos fabricas.
  - CollResult_unk (con ctor inline) se toma de include/gmk/Lemon.h.
- Hallazgos MWCC:
  - `static const char* t[] = {...}` va a .data; con `const char* const` va a .rodata (las cadenas como objetos sueltos de 4 alineados antes de la tabla).
  - Copia de Vector3 con __VecCopy directo en vez de constructor copia invierte los registros de direccion de origen/destino.
  - Accesor miembro `if (i >= 0 && i < n) return &a[i]; return 0;` da la secuencia blt/bge; con ternario sale un bool intermedio (li 1).
  - `!(x < eps)` da fcmpo+blt; `x >= eps` da cror.

## gmk/OnOff ("Decomp gmk/OnOff (15/18)")
- Resultado: 15/18 al 100%, matching: no.
- NONMATCHING: vf0C 99.86% (huecos de temporales del caso 5 y orden de direcciones izadas), fn_803F8324 99.16% (registros de la copia de mMtx/mBoxPos). vf3C (0x803F940C) sin decompilar: recibe un argumento y GimmickCommon::vf3C esta declarado sin argumentos (cambiarlo renombraria vf3C__13GimmickCommonFv en todas las vtables).
- Decisiones:
  - splits.txt: gmk/OnOff empezaba tarde: .text 0x803F744C (antes 0x803F75B8; las dos fabricas estaban en un hueco auto), .sdata2 desde 0x80886018 (2 floats del hueco), .rodata hasta 0x807524F0 (@stringBase0 de 0x104).
  - symbols.txt: lbl_807523EC -> @stringBase0; nombres por pairnames corregidos a mano (pairnames emparejo vf10 con fn_803F8324 y vf3C con vf54__13GimmickCommonFv; restaurado fn_803F940C).
  - include/gmk/Lemon.h: la vista de atn::ModelInstance gana mMtx en 0x30 (mismo tamano).
  - src/game/GimmickCommon.cpp: incluye gmk/OnOff.h y llama a sus dos fabricas.
- Hallazgos MWCC:
  - `cond ? 2 : 1` sale sin saltos; con `int x = 1; if (cond) x = 2;` sale li/b/li (si la variable no cruza llamadas).
  - Inline con un local (color) llamado despues de otros inlines: sus locales quedan por debajo (ultimo inline expandido = direcciones mas bajas).
  - Asignar un struct a un miembro via helper con puntero (this en r3) da stores `0x3c(r3)`; directo, `0x590(r31)`.
  - Un local con nombre declarado antes (aunque se rellene despues) queda por encima de los declarados luego: sirve para ordenar la pila.

## gmk/Cannonball ("Decomp gmk/Cannonball (22/27)")
- Resultado: 22/27 al 100%, matching: no.
- NONMATCHING: vf10 99.92% (el accesor de m9C se llama IsShown_unk y el original llama a la copia CharaObj1208_unk::fn_8013A18C; el destructor de boost::function es fn_8000D3A8 en el original; registros de la suma del radio en los bucles del caso 7), vf40 98.79% (.sdata2 tiene un 1.1920929e-7f de mas por la copia debil de Vector3::ToRotation_unk; registros del caso 6), vf48 99.86% (mismo desplazamiento de .sdata2), fn_803FBC34 99.72% (r5/r6 en la copia a msg.mVec), DropItem::vf60 98.12% (r4/r5).
- Decisiones:
  - splits.txt: .rodata de gmk/Cannonball 0x80752510-0x807525A8 (3 cadenas SE, tabla sSe de 4, double, nombre RTTI, @stringBase0); nueva .bss 0x807C8750-0x807C8760 (objeto de registro del static sListeners, como gmk/ReinforcePoint).
  - symbols.txt: lbl_80752568 -> @stringBase0, lbl_80752540 -> sSe; las 27 funciones renombradas a mano (pairnames no empareja: el .o tiene mas debiles); fn_8008CCE8 -> __ct__Q23atn9Matrix3x4FPA4_Cf.
  - CABECERA COMPARTIDA include/math/Vec.h: nuevo constructor atn::Matrix3x4(const Mtx) (copia debil 0x8008CCE8). Nadie lo usaba antes.
  - CABECERA include/gmk/DropItem.h: DropItem::vf60 inline + IsOut_unk y campo m168 (su copia debil 0x803FB788 esta en Cannonball). include/gmk/Lemon.h: CollResult_unk::m28 pasa de bytes a Vector3 (normal).
  - vf4C de Cannonball (0x803FB800) recibe CharaTask*; GimmickCommon::vf4C esta declarado sin argumentos, asi que se declara como miembro no virtual fn_803FB800 (la vtable propia apunta a GimmickCommon::vf4C: .data 98%). REVISAR: cambiar la firma base renombraria vf4C de GimmickCommon/OnOff/Item/SavePoint.
  - Atn::Vector3::operator-() (unario) se redefine `inline` en Cannonball.cpp (en el original es inline: copia debil 0x8013C3B4; CharaTask.cpp la define fuera de linea). Sin eso vf10 no supera el presupuesto de inline.
  - regress: perdidas 0. main.dol OK.
- Hallazgos MWCC:
  - Presupuesto: vf10 estaba a 4 unidades del limite; probar con un candidato ficticio de N sentencias al principio da el margen rapido (sin dejarlo en el codigo).
  - Hacer `if`+flag en IsGrappled_unk/IsActive_unk (CharaTask.h) tambien lo superaba, pero cambia muchas funciones de CharaTask/Em*: no usar.
  - Un objeto con constructor vacio `T() {}` (Matrix3x4) se llama fuera de linea si el llamador esta sobre presupuesto; usar Mtx plano.
  - `3 <= x && x <= 5` da dos cmpwi; `x >= 3 && x <= 5` da subi+cmplwi.
  - `(int)u8 == 3` da cmpwi; `u8 == 3` da cmplwi.
  - Constante usada antes y despues de una llamada: declararla como local (`float speed = 5.0f;`) justo despues de la llamada la deja en f31 cargada en ese punto.
  - Locales: un Vector3 con nombre del llamador pasado a un inline queda entre los locales con nombre; el Color local del inline va abajo.

## gmk/Leaf ("Decomp gmk/Leaf (7/15)")
- Resultado: 7/15 al 100%, matching: no. El resto entre 93.7% y 99.9%.
- NONMATCHING: vf0C 99.42%, fn_8040161C 99.24%, fn_80401E50 99.22%, fn_804023E0 98.79%, fn_80400D08 99.90% (orden de registros indice/offset/this de los bucles sobre mLeaves; en el original `this` y el indice quedan por encima del offset), vf20 99.35% (r29/r30 de datos/tamano del binario), vf10 95.08% (la matriz de rotacion queda arriba en la pila; en el original esta en 0x18, por debajo de los temporales de los tres __MtxRotate), vf14 93.73% (el 0 de m64 sale de una copia del registro del indice).
- Decisiones:
  - splits.txt: .rodata de gmk/Leaf 0x80752798-0x80752830 (tabla de punteros a miembro sStates, double, nombre RTTI, @stringBase0); nueva .sdata 0x8087DF58-0x8087DF68 (sUpdate, sBlow = 1 y los dos pools @STRING@ de Buffer_unk<LeafEntry_unk>::Resize_unk).
  - symbols.txt: renombres de las 15 funciones a mano, sStates__3gmk, sUpdate, sBlow, @stringBase0 y los @STRING@.
  - CABECERA include/gmk/Lemon.h: la vista de atn::ModelInstance tiene ahora destructor virtual (Leaf borra sus instancias por la vtable; en atn/ModelInstance.cpp ya es virtual) y declara fn_8009EB20; se quita el campo mVtbl_unk. Lemon/Brazier/OnOff/Cannonball sin cambios de porcentaje.
  - src/game/GimmickCommon.cpp: llama a gmk::Leaf::fn_80400728.
  - Rand01_unk como macro y Sin_unk/Cos_unk como inline (ver hallazgos): nombres inventados.
- Hallazgos MWCC:
  - Orden de evaluacion de `a * b` con llamadas en ambos lados: una llamada inline se expande antes que el resto; con `rand()` dentro de un inline se evaluaba antes que `sin`; el original evalua `sin` (inline float) antes que el macro de rand.
  - `(a - k < b) ? b : a - k` da fcmpo+bge sin cror; con `>=` sale cror.
  - En un bucle con delete por la vtable, usar mData[i] directo (no operator[]) da el orden de incrementos del original.
  - Una tabla `static void (C::*const t[])()` va a .rodata y se llama con __ptmf_scall; el simbolo queda dentro del namespace (sStates__3gmk).

## gmk/WindyItem ("Decomp gmk/WindyItem (9/16)")
- Resultado: 9/16 al 100% (fabrica, ctor, ctor/dtor de WindySlot_unk, dtor, vf0C, vf1C, vf40, vf70), matching: no.
- NONMATCHING: vf28 92.84% (registros de las constantes izadas: el original crea 1.0f antes; orden de operandos de la mitad del tamano), vf20 99.98% (offset de 0.017453292f en .sdata2: faltan antes las constantes de vf10/vf14).
- Sin decompilar: vf10 (0x804375C0, 0x1254 bytes: rafagas, caminos cubicos de los objetos), vf14 (0x80438814, dibujo de depuracion) y los tres metodos fn_804392C0/fn_80439574/fn_8043A08C (0x804392C0-0x8043A770).
- Decisiones:
  - splits.txt: gmk/WindyItem empieza en 0x80436E34 (la fabrica estaba en un hueco auto) y termina en 0x8043A770 (incluye los tres metodos de WindyItem del hueco auto_03_804392C0, sin decompilar); .rodata 0x80754240-0x807542B0 (cadenas SE, tabla, double de esos metodos + nombre RTTI + @stringBase0); .sdata2 0x80886900-0x80886958 (antes 0x80886904-0x80886934: el 0.0f de 0x80886900 y las constantes de los tres metodos son de esta unidad). REVISAR: la unidad cubre ahora codigo sin decompilar.
  - symbols.txt: lbl_80754288 -> @stringBase0 (incluye "vector length error" de std::vector); 13 funciones renombradas.
  - src/game/GimmickCommon.cpp: llama a gmk::WindyItem::fn_80436E34.
- Hallazgos MWCC:
  - Un miembro de 3 floats sueltos se copia con lfs/stfs (y propaga valores ya en registro); un struct POD de 3 floats se copia con lwz/stw; atn::Vector3 con psq.
  - std::vector<float> deja un bucle vacio en el destructor (pseudo-destructor); con un struct de un float desaparece. Un nivel mas de herencia sobre std::vector anade un null-check mas.
  - Un Mtx que el original tiene por debajo de los temporales de un bloque {Mtx r; Mtx t;} indica que todo el codigo esta en un inline (la misma funcion inline aparece en vf28 y vf14): sus locales van abajo y los de sus bloques encima.
  - Bucle con puntero (`slot++`) en vez de indice conserva la base del elemento y suma el offset del miembro en cada uso.

---

# Cola q4_gmk

## Notas q4_gmk (rama agent-q4_gmk, /home/claude/work/q4_gmk)

## gmk/MapLift (commit c6f4eaf)
- Resultado: 10/15 al 100%, matching: no.
- NONMATCHING:
  - ctor 99.90% / dtor 99.84%: el original destruye el array de LH_Motion con fn_800970CC (dtor con doble comprobacion de null, tambien usado por Model_unk::Model_unk) en vez de __dt__Q23atn9LH_MotionFv. No he encontrado que clase lo genera.
  - vf0C 96.65%: solo reparto de registros (this en r31 y group en r21 en el original).
  - vf20 83.37%: registros + push_back de std::vector<std::string> inline (vector.h ya conocido como no matching).
  - fn_8042AB58 88.73%: orden de slots de pila de las 3 columnas temporales y planificacion del primer bloque.
- Decisiones:
  - splits.txt: gmk/MapLift ampliada: .text 0x80429668-0x8042AFE4 (fabrica antes y dos metodos despues, antes en autos), .rodata hasta 0x80753800 (@stringBase0), anadidas .sdata 0x8087DFB0-0x8087DFB8 (@STRING@ de Buffer_unk::Resize_unk) y .sdata2 0x80886720-0x80886728.
  - symbols.txt: nombres por pairnames + vf10/vf20/fn_8042AB58 a mano; fn_800A08E0 -> __ct__14MotionPose_unkFv (ctor de la pose de nodo, nombre inventado); lbl_80886720 size 0x8 -> 0x4 (0.0f + relleno); lbl_80753768 -> @stringBase0; lbl_8087DFB0/B4 -> @STRING@Resize_unk__Q23atn36Buffer_unk<Q33gmk7MapLift9Entry_unk>Fii(@0).
  - src/game/GimmickCommon.cpp: incluye gmk/MapLift.h y llama a gmk::MapLift::fn_80429668 (antes extern "C").
  - include/gmk/MapLift.h nuevo (MotionPose_unk global, Entry_unk igual que ArenaFloor).
  - "Memory allocation failure" del push_back inline es lbl_80778910 (.data compartido, @STRING@ de allocator<string>::allocate en otro fichero); no renombrado.
- Hallazgos MWCC:
  - `i >= 0 && i < 7` se fusiona en `cmplwi i,6`; `i >= 0 && 7 > i` deja los dos cmpwi con signo (vf10 al 100% asi).
  - Maximo de 3 con macro anidada (MAX(a, MAX(b,c))): la interior se evalua dos veces (recarga de memoria en la rama falsa) y lleva frsp.
  - Temporales devueltos por un inline (columnas de matriz) van por debajo de los temporales del cuerpo; el Vector3 resultado se reserva antes que sus argumentos.

## gmk/MeetingSpot (commit e420af2)
- Resultado: 8/10 al 100%, matching: no.
- NONMATCHING:
  - fn_803FD0A0 99.72%: operandos de las 4 comparaciones de id intercambiados (cmpw r0,r3) y 6400.0f emitido antes que 1.0f/0.1f.
  - fn_803FD328 95.29%: eleccion de motion 0x76/0x77 y 0x7B/0x7C (el original la hace justo antes de la llamada, un registro salvado menos) y orden de 1.0f/0.1f en .sdata2.
  - Orden del pool .sdata2: en el original 1.0f/0.1f (usados solo en fn_803FD328) van antes que 6400.0f (fn_803FD0A0, que va antes en el .dol): como si fn_803FD328 se compilara antes. No resuelto.
- Decisiones:
  - splits.txt: .rodata de gmk/MeetingSpot ahora 0x807525B8-0x807525D8 (double 2pi de fmod al principio, @stringBase0 al final); .data ahora desde 0x8078CFC0 (jumptable de vf10).
  - symbols.txt: nombres por pairnames + vf10/fn_803FD0A0/fn_803FD328 a mano; lbl_807525D4 -> @stringBase0.
  - src/game/GimmickCommon.cpp: incluye gmk/MeetingSpot.h y llama a gmk::MeetingSpot::fn_803FC8D8.
  - include/gmk/MeetingSpot.h nuevo (gemelo de InteractionSpot). En el .cpp se define struct RoutePath_unk {int,int} (declarado incompleto en game/CharaStatus.h).
- Hallazgos MWCC:
  - Un inline que devuelve bool usado en un if sale con mfcr/extrwi; para un `frame >= len()` con el frame cargado antes de la llamada: puntero local + float local + if directo.
  - Un inline que devuelve int (envolviendo una funcion que devuelve unsigned short) conserva `clrlwi.` + `ble` en `> 0`; con la funcion directa sale `beq`.
  - Un inline miembro que usa mChara[i] recarga el puntero tras cada store (como el original); con un parametro CharaTask* lo guarda en un registro.
  - Un `int` local para el resultado de `a || b` deja el patron cntlzw/srwi. + mfcr/extrwi del original (con bool salen ramas).

## gmk/CharaPathMove (commit siguiente a e420af2, "Decomp gmk/CharaPathMove (12/22)")
- Resultado: 12/22 al 100%, matching: no.
- NONMATCHING: dtor 99.44% y fn_8043E07C 83.52% (registros de los clear), vf6C 99.41% (orden .sdata2: 1.0f va antes en el original), fn_8043E770 89.59% y fn_8043EA64 93.10% (marco/slots de pila de la rotacion), vf20 (el original supera el presupuesto de inline y llama fuera de linea a todos los helpers de string/vector, al ctor de CSV fn_803EFCA8, a la creacion de GmkChara 0x8043E134 y al push_back 0x8043E1F0). Sin definir aqui (inlinadas en nuestro .o): fn_8043D6E4 (clear de vector<Vector3>), fn_8043D930 (operator[] de vector<PathChara*>), fn_8043E134, fn_8043E1F0.
- Decisiones:
  - splits.txt: .rodata de gmk/CharaPathMove ahora 0x807543A0-0x80754498 (double 2pi de fmod delante, @stringBase0 de 0xDC detras).
  - symbols.txt: nombres de 18 funciones a mano; lbl_807543BC -> @stringBase0.
  - src/game/GimmickCommon.cpp: incluye gmk/CharaPathMove.h y llama a gmk::CharaPathMove::fn_8043D594.
  - include/gmk/CharaPathMove.h nuevo: PathChara_unk (0x30), PathCharaList_unk (vector<PathChara*> derivado + int, 0x10), PathData_unk (0x54, 4 vectores de tipo desconocido; uso Vector3).
  - ((void)0) en fn_8043E770 (2) para que no se autoinlinee en vf28/vf10 (precedente aceptado de asserts vacios).
- Hallazgos MWCC:
  - Stores de size de un vector al final del ctor = clear() en el cuerpo del ctor (el store de la init list se elimina).
  - clear() de vector<float> deja un bucle vacio desenrollado (bdnz); con Vector3/punteros desaparece.
  - Bucle `for (i < v.size()) v[i]->f()` con i en registro mayor que el desplazamiento: meterlo en un inline miembro (orden ascendente de locales inlinados).

## gmk/Chain ("Decomp gmk/Chain (7/13)")
- Resultado: 7/13 al 100%, matching: no.
- NONMATCHING: vf10 99.95% (solo la posicion de 90000.0f en .sdata2, depende de fn_8043B9E0).
- Sin decompilar (declaradas en include/gmk/Chain.h): fn_8043B9E0 (0x954, construye las juntas con un std::vector local de elementos de 0x5C y push_back inline), fn_8043C508 (0x550), fn_8043CA58 (0x500), fn_8043CF58 (0x610), y por tanto el ctor de ChainJoint_unk (0x8043C334, se emitiria tras fn_8043B9E0).
- Decisiones:
  - splits.txt: gmk/Chain ampliada: .text desde 0x8043B778 (fabrica), .rodata 0x807542E8-0x807543A0 (8 nombres de nodo como objetos sueltos + RTTI + @stringBase0 con "it087_00.model", "joint%d", "vector length error"), .data 0x8078F0A0-0x8078F178 (tabla sNodes antes de la vtable y "Memory allocation failure" del vector despues).
  - CABECERA COMPARTIDA include/game/GimmickCommon.h: vf3C pasa a recibir un argumento (void* lens); symbols.txt vf3C__13GimmickCommonFv -> vf3C__13GimmickCommonFPv. gmk::Chain::vf3C guarda el argumento en Model_unk::m1DC. regress sin perdidas y main.dol OK.
  - symbols.txt: nombres de 13 funciones a mano; lbl_80754370 -> @stringBase0; lbl_8078F0A0 -> sNodes (scope:local).
  - src/game/GimmickCommon.cpp: incluye gmk/Chain.h y llama a gmk::Chain::fn_8043B778 (la fabrica devuelve 0 siempre).
- Hallazgos MWCC:
  - `static const char* t[] = {"a", ...}` (tabla en .data): los literales salen como objetos sueltos en .rodata alineados a 4 con nombres @NNNN, antes del RTTI.
  - `delete[] p` sin if explicito: una sola comprobacion de null (la del compilador); con `if (p) delete[] p;` salen dos.

## gmk/PigeonManager ("Decomp gmk/PigeonManager (6/15)")
- Resultado: 6/15 al 100%, matching: no.
- NONMATCHING: vf0C 88.73% (marco 0x10 menor, un registro salvado menos), vf10 99.52% y vf14 99.45% (orden de los incrementos del bucle: el original incrementa el contador antes que el desplazamiento; probado referencia, puntero, mData directo), fabrica 99.97% hasta corregir el tamano (ya 0x640).
- Sin decompilar (declaradas): los 6 metodos de Pigeon_unk 0x804048CC-0x80405DD4 (actualizacion, reinicio, vuelo; mucho calculo de angulos).
- Decisiones:
  - splits.txt: gmk/PigeonManager ampliada: .text desde 0x804047A4 (ctor/dtor de Pigeon_unk), .rodata 0x80752858-0x807528C0 (2 cadenas SE + tabla sSe, 3 doubles, RTTI, @stringBase0), .sdata 0x8087DF70-0x8087DF78 nueva (@STRING@ de Buffer_unk::Resize_unk), .sdata2 desde 0x808861F0. Nota: el pool de gmk/Bat (0x80752844-0x80752858) sigue fuera del split de Bat (no tocado).
  - symbols.txt: 15 funciones a mano; lbl_80752878+lbl_8075287C -> sSe (size 0x8, scope:local); lbl_807528AC -> @stringBase0; lbl_8087DF70/74 -> @STRING@Resize_unk__Q23atn30Buffer_unk<Q23gmk10Pigeon_unk>Fii(@0).
  - src/game/GimmickCommon.cpp: incluye gmk/PigeonManager.h y llama a gmk::PigeonManager::fn_80405DD4 (casos 14 y 15).
  - ((void)0) en el ctor para que no se inlinee en la fabrica.

## gmk/RotaryRoom ("Decomp gmk/RotaryRoom (7/18)")
- Resultado: 7/18 al 100%, matching: no.
- NONMATCHING: vf10 95.59% (el original recarga los flags del ColliTree antes de cada escritura; registros), vf20 91.50% (constante 1 en registro salvado, como ArenaFloor::vf20), vf40 99.83%, vf6C 99.54% y __sinit 99.71% (solo orden .sdata2).
- Sin decompilar (declaradas): vf0C (0x638; ademas emite la copia debil de MapPartsBase::vf24 y el ctor de RotaryEntry_unk 0x804248E0 tras ella), fn_80424AD4, fn_80424FB4, fn_80425498.
- Decisiones:
  - splits.txt: .rodata de gmk/RotaryRoom ahora 0x80753498-0x80753670 (tabla sTurns_unk [24][4], double de int->float, RTTI, @stringBase0); .bss nueva 0x807C88F8-0x807C8A28 (sFar y sRots[24], inicializados por __sinit).
  - symbols.txt: 17 funciones a mano (fn_80425FB0 -> __sinit_\RotaryRoom_cpp scope:local); lbl_80753498 -> sTurns_unk; lbl_80753630 -> @stringBase0; lbl_807C88F8 -> sFar (size 0xC); lbl_807C8908 -> sRots.
  - src/game/GimmickCommon.cpp: incluye gmk/RotaryRoom.h y llama a gmk::RotaryRoom::fn_80423F64.
- Hallazgos MWCC:
  - Rango `x >= 1 && x <= 24` tambien se fusiona (subi + cmplwi); `x >= 1 && 24 >= x` lo evita.

## gmk/WaitObject ("Decomp gmk/WaitObject (11/27)")
- Resultado: 11/27 al 100%, matching: no.
- NONMATCHING: ctor 98.84% (r31/r30), vf14 97.56% (patron bne+b para m54 == 0), vf1C 99.85% (puntero a miembro safe-bool de boost::scoped_ptr = lbl_8078DC64, local aqui), vf60 93.33% (el original recarga m54 en el bucle), vf48 99.94% (orden .sdata2), vf50 97.50% (recarga del puntero de colision al devolverlo).
- Sin decompilar (declaradas): vf28, vf10, vf20 (0x888), vf40, vf44, vf6C, fn_8041AAC0 y las tres funciones de 0x24C que llama (0x8041B28C, 0x8041B4D8, 0x8041B724).
- Decisiones:
  - splits.txt: gmk/WaitObject ampliada: .text 0x80418EAC-0x8041B970 (dos fabricas delante; scoped_ptr::get y 3 helpers detras), .rodata 0x80753038-0x80753110 (double, RTTI, @stringBase0), .data desde 0x8078DC40 (4 punteros a miembro {0,-1,get} del safe-bool de scoped_ptr), .sdata2 desde 0x808864F8.
  - symbols.txt: 23 funciones a mano (fn_8041B284 -> get__Q25boost31scoped_ptr<Q23atn11ColliObject>CFv); lbl_80753050 -> @stringBase0.
  - src/game/GimmickCommon.cpp: incluye gmk/WaitObject.h y llama a sus dos fabricas.
- Hallazgos MWCC:
  - `if (scoped_ptr)` = operator unspecified_bool_type: carga de un puntero a miembro {0,-1,&get} de .data + __ptmf_test.

## gmk/ChickenManager ("Decomp gmk/ChickenManager (5/15)")
- Resultado: 5/15 al 100%, matching: no. Gemelo de gmk::PigeonManager (cabecera adaptada).
- NONMATCHING: ctor 99.94% (orden .sdata2), vf0C 88.71% (como PigeonManager), vf10 98.54% (`s == 3 || s == 4` se fusiona en cmplwi aqui; probado operandos invertidos y switch), vf14 99.41% (orden de incrementos del bucle).
- Sin decompilar (declaradas): los 6 metodos de Chicken_unk 0x8040B668-0x8040D1D0.
- Decisiones:
  - splits.txt: gmk/ChickenManager ampliada: .text desde 0x8040B560 (ctor/dtor de Chicken_unk, antes en la auto de Cannon/DropItem), .rodata 0x80752AA8-0x80752B10 (2 cadenas SE + sSe, 3 doubles, RTTI, @stringBase0), .sdata 0x8087DF78-0x8087DF80 nueva, .sdata2 desde 0x80886318. fn_8040B4B8 (sin referencias) queda en la auto anterior.
  - symbols.txt: 15 funciones a mano; lbl_80752AC8+ACC -> sSe (scope:local); lbl_80752AFC -> @stringBase0; lbl_8087DF78/7C -> @STRING@Resize_unk__Q23atn31Buffer_unk<Q23gmk11Chicken_unk>Fii(@0).
  - src/game/GimmickCommon.cpp: incluye gmk/ChickenManager.h y llama a gmk::ChickenManager::fn_8040D1D0.

## Resumen final
- Cola completa (8 unidades tocadas, ninguna matching). Funciones al 100% ganadas: 76 (report: 3997 -> 4073), regress sin perdidas, main.dol OK, arbol limpio (ultimo commit 89612bc).
- Cambios fuera de src/ y game_units.json: splits.txt (8 unidades ampliadas a su tamano real, ver cada seccion), symbols.txt (renombrados + tamanos lbl_80886720 y sFar, fusion de sSe), include/game/GimmickCommon.h (vf3C(void*)), src/game/GimmickCommon.cpp (8 fabricas por clase). game_units.json no se toco (las entradas ya existian, matching false).

---

# Cola q5_gmk

## Notas q5_gmk (rama agent-q5_gmk, /home/claude/work/q5_gmk)

Nota general: tras tocar splits/symbols, `configure` re-divide todo (~90-150 s).
El "main.dol: OK" se ve con `dtk shasum -c config/SLSP01/build.sha1` (ninja sin trabajo no lo imprime).

## gmk/SparkCube (commit f7eee29)
- Resultado: 16/19 al 100%, matching: no (la entrada ya existia con matching false).
- NONMATCHING:
  - vf0C 99.52%: orden de creacion de las variables de induccion del bucle de fades (offset de nombres, indice, offset de fades).
  - vf10 99.87%: el bucle de fades incrementa el offset antes que el indice (mismo problema).
  - vf40 99.22%: bucles de mReverse de los casos 3 y 4 (el valor guardado va en un registro creado tras las variables del bucle).
- Decisiones:
  - splits.txt: gmk/SparkCube .text empieza en 0x80426108 (la fabrica estaba en un hueco auto), .rodata 0x80753670-0x80753700 (double de conversion, nombre RTTI y @stringBase0), nueva .bss 0x807C8A28-0x807C8A38 (objeto de registro del static sListeners de fn_8014F8F4, como gmk/Item).
  - symbols.txt: lbl_80753688 -> @stringBase0 (scope:local); lbl_8087DF48/DF4C -> @STRING@Resize_unk__Q23atn37Buffer_unk<23GimmickMaterialFade_unk>Fii(@0) (son de .sdata en la zona de FragileObject, compartidos con LemonBucket/MotionObject); fn_803F3334 -> __ct__23GimmickMaterialFade_unkFv (copia debil en FragileObject); funciones por pairnames + vf28/vf10/vf40 a mano.
  - CABECERA COMPARTIDA include/game/GimmickCommon.h: GimmickMaterialFade_unk gana ctor inline (0,0,0,1.0f,1.0f,-1,0). Nadie construia el tipo antes. regress sin perdidas.
  - src/game/GimmickCommon.cpp: incluye gmk/SparkCube.h y llama a gmk::SparkCube::fn_80426108.
  - include/gmk/SparkCube.h nuevo. Tipos de vista locales (SparkRes_unk: m16C del modelo con mCount 0x30 y nombres 0x50; SparkHitMsg_unk: m28 origen, m2C skill).
- Hallazgos MWCC:
  - Literal pasado a una inline miembro de la clase (helper en el .h) va a @STRING@; si se escribe en el cuerpo de cada llamador va a @stringBase0.
  - `if (p != 0) return p; return def;` en una inline da el patron `beq L; b fin; L:` del original (if/ternario directos dan bne).
  - Vector3 devuelto: locales `float z, y, x` (en ese orden) + `Vector3 v; v.x=...` reproduce la carga z->f0 del original; el constructor (x,y,z) da f2.
  - Valor aleatorio con indice: inline miembro `GetWait_unk(int state)` deja la direccion (state*4) en registro salvado y carga los floats tras rand(); pasar los floats como argumentos los carga antes.

## gmk/LemonBucket (commit ad736cf)
- Resultado: 14/18 al 100%, matching: no.
- NONMATCHING:
  - vf0C 98.71%, vf40 98.61%: registros del bucle inline HideLemons_unk (elemento y offset intercambiados).
  - vf10 99.01%: registros de los bucles de fades/switches (indice y offset intercambiados).
  - vf20 99.13%: reparto de registros (puntero del campo y base del pool de cadenas).
  - (Mismo problema de bucles que en SparkCube: no he encontrado la forma del bucle; probadas int/unsigned, puntero, referencia, accesor, setter inline, while, declaracion fuera.)
- Decisiones:
  - splits.txt: .text desde 0x8042F7EC (fabrica en hueco auto), .rodata 0x80753F98-0x80754070 (2 cadenas SE, tabla sSe, double, nombre RTTI, @stringBase0).
  - symbols.txt: lbl_80753FDC -> @stringBase0; lbl_80753FB8 -> sSe (size 0x8, scope:local; borrada lbl_80753FBC); lbl_8087DF40/44 -> @STRING@Resize_unk__Q23atn37Buffer_unk<23GimmickMotionSwitch_unk>Fii(@0); lbl_8087DFC8/CC -> @STRING@Resize_unk__Q23atn28Buffer_unk<P13GimmickCommon>Fii(@0); fn_803F3360 -> __ct__23GimmickMotionSwitch_unkFv; funciones por pairnames + vf10/vf40 a mano.
  - REVISAR: GimmickMotionSwitch_unk::fn_803EC0A4 recibe tambien Model_unk* (todos los llamadores -FragileObject, MotionObject, LemonBucket- pasan el modelo en r5). Cambiada la firma en GimmickCommon.h/.cpp y renombrado el simbolo a fn_803EC0A4__23GimmickMotionSwitch_unkFPQ23atn6CsvTabP9Model_unk (el codigo de GimmickCommon.cpp no cambia).
  - CABECERA COMPARTIDA GimmickCommon.h: ctor inline de GimmickMotionSwitch_unk (mHash 0, mFrame -1).
  - src/game/GimmickCommon.cpp: incluye gmk/LemonBucket.h y llama a gmk::LemonBucket::fn_8042F7EC.
  - La fabrica de gmk::Lemon se usa como extern "C" fn_8041524C (en main sigue sin nombre; q3 la llama gmk::Lemon::fn_8041524C en su rama: al integrar, cambiar a la de Lemon.h).
  - El destructor de LemonBucket es implicito (sale el ultimo en el .dol).
- Hallazgos MWCC:
  - `(float)__fabs(x) < c` y atn::Abs_unk(x) dan registros distintos; el original usa Abs_unk (inline que devuelve float).
  - Rotaciones condicionales z/y/x: una inline RotateZYX_unk que llama a Rotate_unk (locales `Mtx r; Mtx t;`) da la pila del original (cada pareja t<r en orden z, y, x ascendente); escribirlo en el cuerpo o llamar a Rotate_unk directamente invierte el orden.
  - `x / 2.0f` (fmuls x, 0.5) vs `x * 0.5f` (fmuls 0.5, x): el orden de operandos de fmuls lo delata.

## gmk/MotionObject (commit 9451b2c)
- Resultado: 25/29 al 100% (incluye vf20 de 0x754 y las copias debiles de los elementos de los arrays), matching: no.
- NONMATCHING: vf10 97.5% (registros: this en r24 en vez de r25; pila del rayo del bucle de personajes; el original carga 0.0f en f1 antes de CharaTask::fn_80148B38, que quiza recibe un float pero su simbolo es Fv), fn_804137F4 97.0% (pila de los temporales de la esfera), fn_804139D8 99.17% (registros del bucle, como LemonBucket), vf24 99.88% (registros de los temporales del CSV).
- Decisiones:
  - splits.txt: .text desde 0x80412AFC (fabrica en hueco auto), .rodata 0x80752D90-0x80752E58 (2 doubles, nombre RTTI, @stringBase0).
  - symbols.txt: lbl_80752DB8 -> @stringBase0; fn_803F1DA4/fn_803F1E18 -> __dt__Q23atn37Buffer_unk<23GimmickMaterialFade_unk|23GimmickMotionSwitch_unk>Fv (copias debiles en FragileObject); funciones por pairnames (incluye __defctor__9Model_unkFv, ctores/dtores de los elementos) + vf10 y fn_804137F4 a mano.
  - CABECERA COMPARTIDA include/atn/Model_unk.h: el constructor gana argumentos por defecto (8, 32): un array `Model_unk m[8]` genera el helper __defctor__9Model_unkFv (0x80412E94). regress sin perdidas.
  - vf4C (slot 0x4C, recibe CharaTask*) declarado como miembro no virtual fn_8041482C (como Cannonball en q3): la vtable propia apunta a GimmickCommon::vf4C. REVISAR junto con Cannonball/OnOff: GimmickCommon::vf4C deberia recibir un argumento.
  - ((void)0) x3 en fn_804139D8 (patron aceptado de asserts vacios) para que no se autoinlinee en vf10 (el original lo llama fuera de linea y vf10 sube de 86% a 97.5%).
  - TARGET_COLLI: struct de 0x5C con atn::ColliSphere como primer miembro (new de 0x5C con la vtable de ColliSphere); BOUND_CENTER_NODE es una cadena fija con ctor (String32_unk: `if (s != buf) memcpy`).
  - src/game/GimmickCommon.cpp: incluye gmk/MotionObject.h y llama a gmk::MotionObject::fn_80412AFC.
- Hallazgos MWCC:
  - Array de objetos con ctor de argumentos por defecto: helper debil __defctor__<clase>Fv con salto de cola al ctor; el dtor del array es el de la clase.
  - Array de structs con ctor inline y sin dtor: bucle inline en el ctor (el primer elemento a veces se escribe aparte); con dtor: __construct_array con copias debiles del ctor/dtor del elemento tras el ctor de la clase.
  - Un `if/else` cuyo else llama a otra inline con un local (color) deja ese local por debajo de los de la primera inline: hay que meterlo en otra inline (con el argumento float evaluado antes).
  - `int s = m58; if (s > 0) t = s; else { if (x >= 0) s = x; t = s; }` reproduce el reparto de registros; la version con m58 repetido no.

## gmk/AutomaticDoor (commit 2d4da7c)
- Resultado: 17/19 al 100%, matching: no.
- NONMATCHING: vf0C 99.81% (el flag de textura cargada va en r30 y no en r31), fn_803F53E0 ~66% (proyeccion de la textura con una camara: pila de la camara/vectores y registros; traduccion aproximada).
- Decisiones:
  - splits.txt: .text desde 0x803F471C (fabrica en hueco auto), .rodata 0x807522E0-0x807523A0 (nombre RTTI y @stringBase0).
  - symbols.txt: lbl_807522F4 -> @stringBase0; lbl_8087D9F0/F4 -> @STRING@CreateCamera_unk__FP9Model_unk(@0) (literales del inline CreateCamera_unk, compartidos con game/CharaTask.cpp, que no esta enlazado); funciones por pairnames + fn_803F53E0 a mano.
  - vf3C (slot 0x3C, recibe Lens_unk*) como miembro no virtual fn_803F69C8 (como OnOff en q3; misma REVISION pendiente de GimmickCommon::vf3C).
  - Texture_unk se trata como GimmickUvOwner_unk para llamar a fn_800D59B8 (el simbolo esta nombrado como metodo de GimmickUvOwner_unk; parecen la misma clase: m20 cargador, m28 imagen).
  - El codigo del plano de reflexion es el de gmk::WaterLevel (Plane_unk de WaterLevel.h; aqui m120 = 1 y comprueba lbl_8087DF50).
  - src/game/GimmickCommon.cpp: incluye gmk/AutomaticDoor.h y llama a gmk::AutomaticDoor::fn_803F471C.
- Hallazgos MWCC:
  - `strncmp(f, "X", strlen("X"))` y `f += strlen("X")` llaman dos veces a strlen (no se pliega).
  - Struct sin ctor de usuario con array de miembros con ctor/dtor: ctor/dtor implicitos debiles (__ct__/__dt__ de la struct) tras el ctor de la clase; los ints del struct se inicializan en el cuerpo del ctor de la clase (bucles 3x3 desenrollados).
  - `a == b` frente a `b == a` cambia el orden de subf en el cntlzw.

## gmk/SwingObject (commit 2d44b0c)
- Resultado: 9/20 al 100%, matching: no.
- NONMATCHING: LoadSwing_unk 90.09% (registros; el parseo de la letra del eje X/Y/Z sale sin ramas), fn_803FE390 99.88%, fn_803FE570 94.04%, vf0C 99.98%, vf10 91.55%, fn_803FEF78 95.29%, vf6C 96.65%. Casi todo es el orden de las constantes de .sdata2 (el original: 0, 1, 100, 0.0174, 20, pi/2, ...; el nuestro empieza 0, 0.5, 45, 10 por el ctor inline de SwingCore_unk en las fabricas).
- Sin escribir: fn_803FFD10, fn_803FFF40, fn_804003E0, fn_804006E0 (otra clase que reutiliza la logica de balanceo; la usa MapPartsGimmickLoc).
- Decisiones:
  - splits.txt: .text desde 0x803FDFC0 (LoadSwing_unk estatica + fabricas en hueco auto), .rodata 0x807525D8-0x80752798 con `align:4` (SIN align:4 dtk trata el objeto de ceros de 4 bytes lbl_807525D4 de MeetingSpot como relleno, lo descarta y el enlace falla con "undefined lbl_807525D4"), .data desde 0x8078D080.
  - symbols.txt: LoadSwing_unk (scope:local), @stringBase0 (lbl_807526BC), @LOCAL@assign_to<...SwingObject...>@stored_vtable (lbl_807C8870), su @GUARD@ (lbl_8087F4A8), __RTTI__ del boost::bind_t (lbl_8078D090); pairnames (revertido fn_804006E0 -> vf34, emparejado mal) + vf10/fn_803FEF78/vf40/fn_80400714 a mano.
  - CABECERA COMPARTIDA include/atn/Model_unk.h: bitfields m20C_31_16:16 -> m20C_31_24:8 (unsigned) + m20C_23_16:8 (int con signo), y m20C_7_6:2 -> m20C_7:1 + mFlag6_unk:1 (el codigo lee esos campos por separado con extension de signo). regress sin perdidas.
  - SwingPath_unk: funcion no estatica que solo existe para emitir el literal "/database/gimmick/gm%03d_%03d.gmk" delante de los de LoadSwing_unk en el pool (si es estatica se elimina y el pool queda desordenado). REVISAR.
  - vf3C (Lens_unk*) como miembro no virtual fn_80400714 (misma revision pendiente).
  - src/game/GimmickCommon.cpp: incluye gmk/SwingObject.h y llama a gmk::SwingObject::fn_803FE390/fn_803FE570.
- Hallazgos MWCC:
  - Helpers de matriz como `static` no inline dejan de inlinearse (vf10 baja a 42%); deben ser inline.
  - El orden de .sdata2 sigue el orden en que el compilador ve los literales (incluidos los de ctores inline usados en las fabricas), no el orden de .text.
  - dtk descarta como relleno un hueco de ceros menor que la alineacion de la unidad siguiente aunque tenga simbolo; `align:4` en la seccion lo evita.

## gmk/Bat (commit f2e760f)
- Resultado: 7/15 al 100% (fabrica, ctor/dtor del murcielago, dtor, vf40, los dos estados vacios), matching: no.
- NONMATCHING: vf0C 99.49, vf14 99.35, vf20 99.80 (indice/desplazamiento del bucle intercambiados, el problema de siempre), vf10 99.69 (los dos Vector3 locales de la busqueda del mas cercano quedan por debajo de sus copias), fn_80403308 99.97 (orden de .sdata2: 1000 antes que 40), fn_80403BFC 93.98 y fn_80404064 88.68 (registros float de la curva inline: un f27 de mas), fn_80404290 88.47 (registros de la mezcla del punto de control y de la rotacion del final).
- Decisiones:
  - splits.txt: .rodata 0x80752830-0x80752858 (double de conversion int->float, nombre RTTI y @stringBase0 "\0MODEL\0MOTION\0NUM"), .data desde 0x8078D1C8 (las cuatro constantes PTMF de los estados, anonimas).
  - symbols.txt: lbl_80752844 -> @stringBase0; lbl_8087DF68/6C -> @STRING@Resize_unk__Q23atn31Buffer_unk<Q23gmk11BatUnit_unk>Fii(@0) (literales del new[] inline de Buffer_unk::Resize_unk, en el hueco auto de .sdata); las 15 funciones a mano (pairnames emparejaba vf14 con el dtor de boost::function).
  - ((void)0) x6 en fn_80404290 para que no se inlinee en vf40 (con 5 se inlinea).
  - Constantes PTMF {0, -1, fn} de .data (lbl_8078D1C8...) se dejan sin nombre (MWCC les da @NNNN).
  - src/game/GimmickCommon.cpp: incluye gmk/Bat.h y llama a gmk::Bat::fn_80402978.
- Hallazgos MWCC:
  - El orden de pila de las variables de una funcion inline se invierte segun la profundidad de inline: el mismo codigo de matriz en fn_80403308 (llamado directo) y en vf10 (a traves de un inline mas, Update_unk) sale con el orden inverso; anidar un inline mas es la forma de reproducirlo.
  - `x *= f()` calcula la direccion de x despues de la llamada; para que se calcule antes (como en el original) hay que pasar x por referencia a un inline (RandScale_unk(float&)).
  - Una sentencia con una llamada (cos) usa la base calculada antes de la llamada para todos sus lvalues; para que el original recargue `mData` solo para uno de ellos hace falta una referencia local tomada antes de la llamada (BatUnit_unk& b = mBats.mData[i]).
  - Un inline que devuelve bool usado en `if` deja mfcr/extrwi; comparar en el llamador dos llamadas inline (DistSq_unk(...) > DistSq_unk(...)) da el fcmpo/ble del original.
  - Un parametro por valor de un inline (Vector3 q) genera la copia psq; con dos llamadas en la misma expresion ambas copias se hacen antes de la primera llamada.
  - `Vector3(m[0][i], m[1][i], m[2][i])` devuelto por un inline (GetCol_unk) da el orden de registros f0/f7/f8 del original; __Vec3Set_unk da el orden inverso.

## gmk/RandomMerchant (commit 9a99c7e)
- Resultado: 20/23 al 100% (fabrica, ctor, dtor, struct de entrada y su cadena, vf28/vf14/vf1C/vf40/vf70/vf60/vf48/vf4C/vf64/vf38 y las cuatro funciones debiles de RandomMerchantChara), matching: no.
- NONMATCHING: vf0C 99.77 (std::string y el Vector3 temporal intercambian hueco de pila), vf10 98.28 (huecos de pila de varios Vector3, la comprobacion fn_8017C974 sale como beq sobre b en el original, algun registro), vf20 99.32 (registros del puntero de entradas y de la base de posiciones).
- Decisiones:
  - splits.txt: .text desde 0x804310B0 (fabrica en hueco auto), .rodata 0x80754070-0x80754180 (double de conversion, nombres RTTI y @stringBase0 con los mensajes de basic_string).
  - symbols.txt: lbl_807540A0 -> @stringBase0; las 23 funciones a mano (pairnames emparejaba mal las funciones debiles de RandomMerchantChara).
  - symbols.txt FUERA DE LA UNIDAD: fn_80069758 (copia debil en el hueco auto 0x80068BF0, solo la usa esta unidad) -> __dt__Q23gmk18MerchantString_unkFv, el destructor vacio que se pasa a __construct_array para el array de 3 cadenas. REVISAR el nombre del tipo real.
  - Tipos nuevos en include/gmk/RandomMerchant.h: RandomMerchantChara (como gmk::GmkChara, tipo 1, m137C |= 1), gmk::MerchantString_unk (cadena de 0x80 con dtor vacio; no se reutiliza atn::FixedString_unk porque no tiene dtor y anadirselo cambiaria otras unidades), MerchantEntry_unk (0x29C), MerchantPos_unk.
  - Field_unk declarado localmente (m10D0 loteria de objetos, m10D8 tabla de mensajes, m54E4) como en gmk/SavePoint.cpp.
  - lbl_8087DFD0/D4 ("" del new del personaje, .sdata de otro archivo) como extern "C", igual que CharaObj.
  - src/game/GimmickCommon.cpp: incluye gmk/RandomMerchant.h y llama a gmk::RandomMerchant::fn_804310B0.
- Hallazgos MWCC:
  - `if (2 <= x && x <= 4)` da el par cmpwi/bltlr/cmpwi/bgtlr del original; `x >= 2 && x <= 4` se pliega a subi/cmplwi.
  - Un bucle que recorre un array de miembros con indice (mEntries[i]) da la base this+desplazamiento; un puntero local da la base this+0x4F8: hay que mirar cual usa el original en cada bucle (aqui conviven los dos en la misma funcion).
  - `int r = rand(); a = b; t = r % n;` reproduce que el original guarde `a` despues de la llamada a rand.

---

# Cola q6_atn

## q6_atn - notas (rama agent-q6_atn, copia /home/claude/work/q6_atn)

## atn/ParseTree - 24/25 al 100%, matching: no (commit 7124ca5)
- NONMATCHING: fn_800A6E8C 95.3% - el original calcula idx - 1 antes de escalar (subi; slwi; lwzx); MWCC aqui lo pliega en el offset -4. Probados int/unsigned/puntero, accesor inline, --idx (sale un stw extra), lector inline.
- Falta en el pool "tree::insert length error" (0x1C bytes al final de @stringBase0): viene de un insert de std::map/tree instanciado en el fichero y eliminado por el enlazador; no se reconstruye (no se sabe el tipo). Impide el enlace exacto aunque se arreglara fn_800A6E8C.
- Decisiones:
  - splits.txt: ParseTree ampliado al fichero real: .text 0x800A5B40-0x800A73A4 (el tokenizador fn_800A5B40 usa el pool del fichero; los virtuales de VariableNode/ValueNode estaban fuera), .rodata 0x80732988-0x80732A20 (double 0x4330.. + nombres RTTI de los nodos + pool), .data 0x80778AD8-0x80778BB8 (jumptable de fn_800A6BDC + vtables/RTTI de los nodos), nuevo .sbss 0x8087EF78-0x8087EF88. Todo eran tramos auto sin unidad.
  - fn_800A73A4 (0x4D8, usa .sdata2 0x80880CC8-0x80880CDC) NO es de ParseTree: la llama PBoneBasic y su .sdata2 precede al de PBoneBasic -> se trata con PBoneBasic.
  - include/atn/ParseTree.h (compartida con db/GrowthParseGlobal y CharaStatus): nodos completos (BaseNode/ValueNode/VariableNode/OperatorNode con virtuales inline), firmas de fn_800A5F18/fn_800A6198 como miembros, m04 = BaseNode* raiz, Clear_unk inline; declara pow/floor/memset extern "C". regress: 0 perdidas (GrowthParseGlobal sigue OK).
  - symbols.txt: renombrados por pairnames (funciones y sNodeOffset/sFirstLeaf/sArgument locales).
- Hallazgos MWCC:
  - `c == 'a' || c == 'b' || ...` (5 casos en 0x25..0x2F) se compila como rango + mascara de bits (addi 0xdb; clrlwi; cmplwi 0xa; slw; andi.); un switch no.
  - `c >= 'A' && c <= 'z'` se convierte en resta + cmplwi sin signo; escrito `'A' <= c && c <= 'z'` sale cmpwi/blt/ble como el original.
  - En una inline `if (a) return true; ... return strncmp(..) == 0;` la cadena del return final entra en el pool ANTES que las de los if anteriores; con `if (...) return true; return false;` se respeta el orden.
  - `x ? -1 : 0` da neg/or/srwi/neg; `x != 0 ? -1 : 0` da srawi.
  - Parametro que en el original recibe un registro salvado mas alto que los locales: copiarlo a un local declarado el primero (`const char* expr = str;`).

## atn/SinusCurve - 11/23 al 100%, matching: no (commit dcc33a1)
- Clases: Curve (abstracta; boost::scoped_ptr<Curve> m00 integral, m04 derivada, vptr en +8), SinusCurve, CosinusCurve, ConstantCurve, Polynomial1Curve, Polynomial2Curve. vf08 integral, vf0C derivada, vf10 valor (const), vf14 escala.
- NONMATCHING: los 10 vf08/vf0C (~55%), checked_delete<Curve> (22.5%) y ~Curve (26.8%): solo difiere la profundidad de inline del ciclo ~scoped_ptr -> checked_delete -> ~Curve (el original expande 3 niveles de ~Curve en el reset y llama a ~Curve; checked_delete mas corto). ~scoped_ptr<Curve> ya sale identico. Probado: ~Curve explicito inline / fuera de linea / definido tarde / virtual, variantes propias de scoped_ptr. Hipotesis: el orden en que MWCC procesa las funciones del ciclo (cuerpos ya expandidos) difiere; en el original checked_delete se procesa antes que vf08.
- Comportamientos "raros" copiados tal cual con // inferred: Sinus/Cosinus derivada usa la frecuencia como amplitud; Polynomial2 integral = Constant(0); Polynomial2 derivada = Polynomial1(a * 0.5, b).
- Decisiones: splits SinusCurve .rodata hasta 0x80731140 ("atn::Curve" + pool "") y .data hasta 0x807777C0 (RTTI de Curve). En el mismo commit: splits PBoneBasic ampliado a .text 0x800A73A4 (fn_800A73A4, la llama PBoneBasic y usa .sdata2 0x80880CC8+), .rodata hasta 0x80732AC0 (nombre RTTI PBoneInterface + pool), .data hasta 0x80778C68 (RTTI PBoneInterface), .sdata2 desde 0x80880CC8. Todo tramos auto.
- Hallazgos MWCC:
  - Con miembros declarados antes de la primera virtual, MWCC pone el vptr detras de ellos (Curve: vptr en +8).
  - Funcion virtual inline con primera carga de miembro antes del stw r31 del prologo: era const (confirmado otra vez).
  - `m0C * sin(...)` con sin double: hace falta `(float)sin(...)` para frsp + fmuls.

## atn/LayerOcclusionCPU - 2/11 al 100% (dtor, fn_800B448C), matching: no (commit 8a4028f)
- Casi al 100%: ctor 98.8, vf10 99.9, vf1C 99.9, vf20 98.3 (todas por el offset de las constantes .sdata2, ver abajo), vf14 99.4 (registros), helper fn_800B3978 96.9 (FPR), fn_800B4694 90.6 (orden de evaluacion del argumento matriz).
- No decompiladas: fn_800B4390 (asm paired-single escrito a mano: ps_madds0, fres, mcrf) y fn_800B47AC (rasterizador con mtspr GQR6 y locked cache). Declaradas extern "C" con NONMATCHING.
- Constantes: el .sdata2 original empieza por 0.5, 255, 0 antes del 1.0 del ctor -> vienen de una funcion anterior eliminada por el enlazador (no reconstruida). Todo lo que carga 1.0/16/32/128 apunta a otro offset.
- El fichero real es mas grande: .text hasta 0x800B66A4 (6 funciones de debug en 0x800B543C-0x800B6684 que usan su .sdata2 y el double de 0x80732E28, y __sinit 0x800B6684 que construye Vector3[16] en 0x807C74A0), .sdata con muchas variables (0x8087D890...), .bss 0x807C74A0-0x807C75A0, .data desde 0x807790A8. No ampliado (fuera de la cola); anotado en el fuente.
- Decisiones: splits LayerOcclusionCPU .text desde 0x800B3978 (helper estatico que solo llama vf20), .rodata hasta 0x80732E58 (pool "OcclusionCPU"), nuevos .bss 0x807C7580-0x807C75A0 (sTexObj) y .sbss 0x8087EFA0-0x8087EFA8 (sTexReady, sTexData). symbols.txt: renombrados + pools de ParseTree/SinusCurve/LayerOcclusionCPU como @stringBase0 (scope:local).
- IRenderLayer: copia local en el .cpp con vf10(int) (la cabecera compartida atn/Layer3D.h la declara sin argumento; esta clase lee r4). No se toco la cabecera para no renombrar simbolos de unidades matching.
- Hallazgos MWCC:
  - Bucle con `src[w * (y * 2) + x * 2]` conserva los mullw del original; con un indice de fila acumulado (row += 2) MWCC hace strength reduction y cambia todo.
  - Parametro puntero cargado antes del stmw del prologo -> el parametro es `const T*` (igual que el truco de metodo const). Ademas el local que se pasa (resultado de la funcion) tambien debe ser const para que el inline en el llamador no cambie.
  - Una funcion no inline que el original llama fuera de linea y aqui se autoinlinea: anadir sentencias reales (if/else en lugar de ternario, locales intermedios) hasta que deje de inlinearse.
  - `int c = (int)(k * v); if (v == 1.0f) c = 0;` reproduce fmuls/fctiwz antes del fcmpu; el ternario no.

## atn/LensFlare - 7/10 al 100%, matching: no (commit 1a2082e)
- 100%: ctor, LensFlareElem ctor (fuera de linea, definido tras el ctor), dtor, fn_800774D4, fn_80077534, vf14, vf18.
- NONMATCHING: vf10 90.2% (registros/orden de cargas en clamps), fn_8007786C 75.3% (mismo tamano; el original termina cada vertice antes del siguiente). fn_80078938 (0x700) no decompilada: mucho calculo muerto de vectores; se dejo nota.
- Decisiones: splits .rodata desde 0x80731838 (double 0x4330...) hasta 0x807318A0 (pool), nuevo .sdata 0x8087D758-0x8087D760 (los dos "" de LensFlareElems_unk::Resize_unk, renombrados a @STRING@Resize_unk__Q23atn18LensFlareElems_unkFUi[@0]). symbols: funciones renombradas a mano (pairnames emparejaba mal), pool -> @stringBase0.
- Cabecera compartida include/atn/LensFlare.h reescrita: base atn::IRender (definida aqui, igual que las copias locales de Rain/VideoTHP), LensFlareElem m18 float + ctor fuera de linea, mElems pasa a ser LensFlareElems_unk {mCount, mData} con operator[], Texture_unk m20[6], ResHandle m140. regress sin perdidas; ademas game/MapPartsFlare (no es de mi cola) pasa a 6/6 al 100% (su comentario NONMATCHING 99.7% queda obsoleto; no lo toque).
- fn_800D59B8 se llama con el nombre existente fn_800D59B8__18GimmickUvOwner_unkFv (es un metodo de Texture_unk; no renombrado para no romper unidades matching).
- Hallazgos MWCC:
  - Un ctor vacio `T() {}` inline hace que new[] no use __construct_new_array; si el original pasa una funcion vacia (blr), el ctor estaba definido fuera de linea en el .cpp (se emite donde se define).
  - 40 conversiones float->u8 en una funcion: con `if` en la inline se supera el presupuesto (todo fuera de linea); con ternarios encadenados se inlinea todo.
  - Un literal "" dentro de una inline va a .sdata como @STRING@<func>@N (1 byte), uno por literal.
  - Nombre de grupo de menu calculado antes de la condicion = la creacion esta en una inline que recibe el nombre como parametro.

## atn/SmallAllocator_8 - 27/35 al 100%, matching: no (commit f5c605b)
- Es el gestor de memoria del motor (MemoryManager_unk, nombre inventado): SmallAllocator<8>/<6> (IAllocator), 5 DLAllocator, mutex, callbacks MEMAllocator, __sys_alloc/__sys_free y los operator new/new[]/delete/delete[].
- 100%: callbacks fn_80081E54/820BC/82260, __sys_alloc, __sys_free, fn_800827E0 (instancia), ctor, dtor SA<8>, ~MM, Alloc fn_800832AC, 38AC, 38B8, 39EC, Free fn_80083AD4, SA vf10 x2, vf18/vf1C x2, ctor MemoryMode, new/new[] x2, fn_800848B4, delete[].
- NONMATCHING: dtor SA<6> y operator delete (0%: el original llama al ctor fuera de linea dentro de fn_800827E0 inlineada; ciclo ctor -> vtables -> dtors -> delete -> instancia; el orden en que MWCC lo resuelve decide quien inlinea el ctor; probado mover delete/ctor/instancia de sitio), dtor MemoryMode 42.5% (mismo motivo), Init fn_80082CAC 85.6% (registros), SA vf0C x2 87% (registros volatiles), SA vf14 x2 97.9%.
- Decisiones:
  - splits SmallAllocator_8 ampliado al fichero real: .text desde 0x80081E54, .rodata 0x80732048-0x80732128 (tablas de pools, nombres RTTI, "IAllocator", pool), .data 0x80778550-0x807785D8 (tablas MEMAllocatorFunc + vtables + RTTI de IAllocator), nuevos .bss 0x807C7090-0x807C7268 y .sbss 0x8087EF08-0x8087EF10.
  - symbols.txt: fn_80082490 -> __sys_alloc; ctor/dtor/miembros de plantilla renombrados a mano (pairnames cruzaba <8>/<6>); datos sSmall8Config/sSmall6Config/sMem1Func/sMem2Func (local), @LOCAL@/@GUARD@fn_800827E0__Fv@sInstance, pool @stringBase0. Las funciones de MemoryManager usadas por otras unidades se dejaron como extern "C" con su nombre fn_ (no se renombran).
  - DLAllocator::fn_80084EE8 y IAllocator::vf0C reciben en el juego un argumento mas (nombre/heap) que sus declaraciones en atn/DLAllocator.h / IAllocator.h (unidad DLAllocator matching): no toque las cabeceras; se llama via `extern "C" fn_80084EE8__Q23atn11DLAllocatorFPvUl(...)` y una interfaz local NamedAllocator_unk con vf0C de 4 argumentos.
- Hallazgos MWCC:
  - Wrappers que en el original son otra funcion del fichero con constantes (fn_80081E54 = fn_800838B8(inst, size, 0x20, 0, "")) se autoinlinean y pliegan: escribir la llamada, no el cuerpo.
  - Miembros array de objetos con ctor -> __construct_array; el original llamaba a cada ctor -> eran miembros sueltos.
  - Orden de stores de un ctor trivial (+8,+C,+0,+4): asignaciones en el cuerpo en ese orden.
  - Un `return f();` directo en una rama vs `T x = f(); return x;` cambia el registro salvado del resultado.

## atn/PBoneBasic - 12/18 al 100%, matching: no (commit 1e4a32a)
- Huesos fisicos (PBoneInterface -> PBoneBasic -> PBoneOutForce): leen raiz/punta/limites de un CSV, simulan velocidad con gravedad y limitan el angulo; vf10 rehace la matriz del nodo y recalcula los hijos con ModelPose_unk + fn_8009ADA4.
- 100%: ctor y dtor de Basic y OutForce, vf34 Basic, vf1C (carga), vf28, vf18 (debug draw), vf2C (menu), vf30/vf24/vf20 (weak).
- NONMATCHING: fn_800A73A4 93.4% (eps cargado antes del prologo + copia Vector3), vf0C 99.6% / vf14 99.6% / OutForce vf34 99.9% (la copia asm de Vector3 de math/Vec.h sale con los registros src/dst cruzados cuando ambos son direcciones de pila; el original calcula src primero en el registro alto), vf10 96.7% (el original tiene 3 Vector3 temporales mas en pila con un solo componente guardado; frame 0x20 mayor), OutForce vf28 75% (el original guarda z antes de xy; no hay helper asi en Vec.h).
- Decisiones: symbols.txt renombrados (fn_800A73A4 -> fn_800A73A4__FPQ23atn7Vector3PCQ23atn7Vector3, global en el DOL; ctor/dtor/vfXX). Splits ya ampliados en el commit de SinusCurve. Copias locales de ModelData_unk/ModelJoint_unk/ModelPose_unk (como en atn/LH_Model.cpp); no se toco ninguna cabecera.
- Hallazgos MWCC:
  - Constantes .sdata2 en orden de fuente: hubo que decompilar todas las funciones (aunque no casen) para que vf1C/vf2C/ctors apunten a los offsets correctos.
  - `Vector3 a = expr;` elide la copia; el original hace `Vector3 a; a = expr;` (psq_l/psq_st desde el temporal). Para `dir(tip - root)` + normalize el original usa GetNormalized_unk (copia por referencia).
  - Locales con nombre van arriba en pila y temporales debajo en orden de creacion: un temporal "fuera de sitio" suele ser un local con nombre (force = n * k).
  - El orden de calculo de un float intermedio (gravity) respecto a las sumas de vectores lo decide su posicion en el fuente; mover la linea paso 82% -> 100%.
  - Resultado de cross product asignado a un local reutilizado (temporal en el fondo de pila) = inline que devuelve por valor (Cross_unk).

## std/allocator_char - no empezada (0/21), matching: no
- 0x800E0B90-0x800E24CC: 21 funciones weak de la libreria C++ de MSL (sin fuente en el repo): destructores con shared_ptr de std::tr1 (release__Q43std3tr16detail25shared_ptr_deleter_common), un parser con estado en +0x124 y buffer de 256 bytes (+0x24), etc. Llamadas desde 0x8023B9D0/0x8024372C/0x8068xxxx (otras instancias de iostream/locale). El nombre de clase en game_units.json (basic_stringbuf<char>) es una suposicion.
- No se intento: reconstruir clases internas de MSL (ios/locale/tr1) sin fuente es muy costoso y tocaria cabeceras compartidas; queda para quien tenga los headers de MSL C++.

## Resumen final
- Funciones al 100% en el report: 3997 (ref) -> 4084 (+87). regress sin perdidas. main.dol: OK.

---

# Cola q7_mp

## Notas q7_mp (rama agent-q7_mp, /home/claude/work/q7_mp)

## MP/D2AMpMenuSetRule — 13/15 al 100%, matching: no (commit 03c8de2)
- NONMATCHING: vf0C 98.46% (el original carga la constante 16.0f de fn_8006EF48 antes de la rama de data() del wstring temporal); fn_804CECEC 99.82% (r29/r30 intercambiados en el primer par texto/layout).
- Decisiones:
  - splits.txt: la unidad inferida era más corta que el fichero real. .text ahora empieza en 0x804CD97C (creador del singleton, antes en auto_03_804CD97C), .rodata 0x807595E0-0x807596F0 (tabla {1,3,5,10}, double de conversión, nombre RTTI y pool), .data hasta 0x80790F78 (incluye L"%d"), añadido .sbss 0x8087F5F0-0x8087F5F8 (sInstance).
  - symbols.txt (solo renombrados): funciones de la unidad, sTimes, @stringBase0 (0x80759610, size 0xDA), @wstringBase0 (0x80790F70), sInstance__Q22MP16D2AMpMenuSetRule (0x8087F5F0) y sInstance__Q22MP15D2ACommonDialog (0x8087F588).
  - Cabecera compartida tocada: include/atn/FixedArray.h (+ `void clear() { mSize = 0; }`, regress sin pérdidas).
  - Cabeceras nuevas: include/MP/D2AMpMenu_unk.h (tipos/externos comunes de los menús MP), include/MP/D2ACommonDialog.h (mínima).
  - fn_804CEF84 lleva 7 `((void)0)` (asserts de release) para no autoinlinearse en vf10 (6 no bastan).
- Hallazgos MWCC:
  - Unos locales sin usar (wchar_t[32]={0}, rect con ctor de 5 floats, 4 structs de 6 floats con ctor vacío) dejan exactamente el patrón de stores parciales a la pila del original (fn_804CECEC).
  - Las variables creadas por expansiones inline reciben registro antes que los locales propios: si el original tiene un local de un global en un registro "más alto" que el `this` de un inline, envolver la lectura en un inline (`GetPad_unk()`), o mover el bucle a un helper inline (SetAll_unk) arregla r27-r31.
  - Un bloque repetido con literales en @stringBase0 e inlineado en 4 funciones = función no inline autoinlineada (locales en orden ascendente); en vf10 además va dentro de otro inline (UpdateOpen_unk).
  - `float f = x->mFrame; if (0.0f == f)` da `lfs f0,0.0; lfs f1,frame; fcmpu f0,f1`.
  - Los helpers SetPaneText/SetPaneValue toman el puntero al layout (no la tarea): el original calcula task+0x58 antes del hash.

## MP/D2ACommonDialog — 14/18 al 100%, matching: no (commit 2135e4c)
- NONMATCHING: fn_804AC96C 99.70% (registro del último Play_unk), fn_804ACAF8 98.84% (planificación del primer SetPaneText), fn_804ACE68 98.63% (registros texto/layout antes del bucle), fn_804AD1EC 93.06% (vector<wstring>::push_back inlineado; además el L"" del original es otro literal de .sdata, lbl_8087E0E8+2).
- Decisiones:
  - splits.txt: .text empieza en 0x804AC430 (creador), .rodata 0x80757820-0x80757A30 (pool completo con los mensajes de vector/basic_string), añadidos .sdata 0x8087E0E8-0x8087E0F0 (literales L"" del fichero; solo esta unidad los usa) y .sbss 0x8087F588-0x8087F590.
  - symbols.txt: renombradas las 18 funciones y @stringBase0 (0x80757834, size 0x1F7).
  - include/MP/D2ACommonDialog.h ahora con la clase completa (métodos fn_XXXX miembros); D2AMpMenuSetRule.cpp actualizado a llamadas miembro.
  - Play_unk de include/MP/D2AMpMenu_unk.h pasa a (task, repeat, speed).
  - Incluye std/string_members.h (sus mensajes están en el pool del original) y copia la especialización __vec_split_unk<wstring> de game/MenuCommonTask.cpp.
- Hallazgos MWCC:
  - Un accesor inline `float GetFrame_unk(t) { return t->mFrame; }` da `lfs f0,const; lfs f1,frame; fcmpu f0,f1` sea cual sea el orden de la comparación (sin él sale f1/f0).
  - `return a == 2 || a == 4 || a == 5;` genera el switch por máscara de bits (li r0,1; slw; andi.), un switch con esos case no.
  - switch con `default: return;` escrito primero quita dos `b` sobrantes tras el último case.
  - Para que el código común quede dentro del último case (sin `b` extra) se mete el cuerpo en `case 3:` con `break` en las salidas tempranas (vf10).
  - Un ctor "(m = f())->g(1);" x6 se autoinlinea en el creador; escrito como "m = f(); m->g(1);" (12 sentencias) ya no: el umbral cuenta sentencias del fuente.

## MP/D2AMpMenuCockpit — 12/16 al 100%, matching: no
- NONMATCHING: fn_804B50C0 99.04% (r27/r28 del bucle de marcas), vf14 99.98% (un registro en la 2ª copia inline de UpdatePointPos_unk), fn_804B5B10 93.26% (marcas sobre los personajes: registros del bucle y temporales Vector3), fn_804B67E4 99.62% (texto/layout r30/r31, mismo problema que D2ACommonDialog::fn_804ACAF8).
- Decisiones:
  - splits.txt: .text desde 0x804B4C50 (creador), .rodata 0x80758110-0x80758368 (tabla de mensajes, double, nombre RTTI y pool), .data hasta 0x80790BC8 (incluye @wstringBase0 L"%d"/L"%02d"), añadido .sbss 0x8087F5A8-0x8087F5B0.
  - symbols.txt: funciones renombradas, sInstance, sMessages, @stringBase0 (0x80758150), @wstringBase0 (0x80790BB8).
  - include/MP/D2AMpMenu_unk.h: MpD2A_unk deriva ahora de una declaración mínima de atn::D2A_Task (SetColor_unk/SetNumber_unk, nombres ya existentes en symbols.txt); añadidos SetPaneValue2_unk (fn_801FEDBC), Stop_unk, IsEnd_unk. Ningún fichero de la cola incluye atn/D2A_Task.h (declararía fn_801F3FF8 con otro tipo).
- Hallazgos MWCC:
  - Un literal usado en una función `static` (no inline) autoinlineada va a @stringBase0; dentro de una inline va a @STRING@ (caso GetMarkPos_unk con "neck").
  - `0 <= m && m < 7` conserva las dos comparaciones con signo; `m >= 0 && m < 7` se convierte en `cmplwi m,6`.
  - Las llamadas en cola de un vf10 pequeño (`b fn_...`) salen solas con un switch cuyos case solo llaman.

## MP/D2AMpMenuFriendRoom — 8/12 al 100%, matching: no
- NONMATCHING: vf0C 98.28% (16.0f antes de la rama de data(), como SetRule::vf0C; registros texto/layout de L""), fn_804BA350 99.63% (registros del bucle de OpenList_unk y del case 8), fn_804BA970 99.62% (registros cursor/top/decide/pad y huecos de pila), fn_804BB2CC 94.57% (vector<int>::push_back inlineado).
- Decisiones:
  - splits.txt: .text desde 0x804B9D44 (creador; antes en auto_03_804B9014), .rodata 0x80758600-0x80758848 (tabla de 10 int, nombre RTTI, pool), añadidos .sdata 0x8087E108-0x8087E118 (L"", L"%d", L"/") y .sbss 0x8087F5B8-0x8087F5C0; .sdata2 hasta 0x808873FC.
  - symbols.txt: funciones, sInstance y @stringBase0 (0x80758640) renombrados.
  - include/MP/D2AMpMenu_unk.h: añadido atn::D2A_Task::SetText_unk(const char*, int) (0x801F4C14); el original le pasa la dirección de "" como id.
- Hallazgos MWCC:
  - Un registro de layout que en el original NO reutiliza el del pool ya muerto se consigue con un local `void* layout = task->mLayout;` antes de la llamada (vf10).
  - `n > 0 ? n : 0` da neg/andc/srawi/and; `n < 0 ? 0 : n` da srawi/andc.
  - El orden de las constantes .sdata2 sigue la posición de la definición de la función autoinlineada: mover UpdateScrollBar_unk detrás de vf10 puso 10.0f antes que 197.0f.
  - Un miembro derivado de std::vector (4 comprobaciones de this en el destructor) se reproduce con `struct X : public std::vector<int> {};`.

## MP/D2AMpMenuMessageList — 7/16 al 100%, matching: no
- NONMATCHING: fn_804C35F0 87.6% (SetState; textos controllerType/$Br/$Bl: el original calcula task+0x58 antes del hash y carga el texto después; inlineado en vf0C 91%, vf10 89%, fn_804C4530 87%), fn_804C38C0 99.2% (registros texto/layout), fn_804C4150 88% (condiciones de dirección materializadas como bool, mismo problema abierto que CampSaveLoad::fn_8011B3D8), fn_804C50C0 64% (dibujo con scroll, borrador), fn_804C54FC 48% (búsqueda recursiva: el original inlinea su propia llamada una vez), __sinit 83% (orden del stw).
- Decisiones:
  - splits.txt: .text 0x804C2FF8-0x804C566C (creador, función de mensajes fn_804C54FC, cierre fn_804C5638 y __sinit), .ctors 0x8072D394-0x8072D398, .rodata hasta 0x80758E28, .data hasta 0x80790E20 (@wstringBase0 L"$Bu"... L""), .sbss 0x8087F5D0-0x8087F5E0, .sdata2 0x80887470-0x808874B0.
  - symbols.txt: funciones renombradas (fn_804C54FC como MpMessageTable_unk::fn_804C54FC), __sinit_\D2AMpMenuMessageList_cpp y sCustomData (scope:local), sInstance, sPage, @stringBase0, @wstringBase0.
- Hallazgos MWCC:
  - Una función no inline de ~0x2C8 bytes (SetState) solo se autoinlinea en sus 4 llamadas si su cuerpo de textos va a otra función no inline (SetTexts_unk): el umbral es de sentencias del fuente.
  - Un if/else con la misma llamada inline en ambas ramas (solo cambia el texto) se fusiona en una sola llamada; el original tiene dos llamadas completas.

## MP/D2AMpMenuResult — 12/20 al 100%, matching: no
- NONMATCHING: fn_804C5E9C 99.71% (SetState; registros texto/layout en los estados 7/9/10), vf10 98.26% (registros del bucle índice/puntero/task y huecos de pila del rect en estados 5-7), vf14 99.91% (el original carga x=319 antes que y=42 con los mismos registros), fn_804C76EC 93.99%, fn_804C7D7C 92.85%, fn_804C822C 89.46% (listas de ranking: el original indexa con desplazamiento en bytes y conserva session+slot*0x34 plegando 0x368; además su pool de wide strings tiene relleno), fn_804C7C34 99.63% (texto/layout del último texto), __sinit 83% (orden del stw).
- Decisiones:
  - splits.txt: .text 0x804C566C-0x804C8778 (creador, fn_804C7408..fn_804C822C, cierre fn_804C8740 y __sinit; antes en auto_03_804C7408), añadidos .ctors 0x8072D398-0x8072D39C, .rodata 0x80758E28-0x80759290 (double, nombre RTTI, pool), .data 0x80790E20-0x80790EF0 (3 jumptables), .sdata 0x8087E118-0x8087E140 (wide strings), .sbss 0x8087F5E0-0x8087F5E8; .sdata2 0x808874B0-0x808874E0 (0x808874E0 lo usa SetMessage).
  - symbols.txt: funciones, __sinit_\D2AMpMenuResult_cpp (local), sInstance, @stringBase0 (0x80758E44), @wstringBase0 (0x8087E118, size 0x28).
  - include/MP/D2AMpMenu_unk.h: añadido atn::D2A_Task::SetParam2_unk (0x801F48C8, ya en atn/D2A_Task.h). Regress: 0 perdidas.
  - MP::D2AMpMenuCockpit declarada mínimamente en el .cpp (sInstance, fn_804B50C0) porque la clase completa vive en D2AMpMenuCockpit.cpp.
- Hallazgos MWCC:
  - El problema de registros texto/layout de SetPaneText se resuelve a veces con un inline que recibe el task y toma `void* layout = task->mLayout;` dentro (SetTaskText_unk): vf0C (solo en el primer par) y fn_804C6888 al 100%; en otros pares empeora.
  - En fn_804C7C34 el original carga los dos globales (tabla y sesión) antes del Get_unk: se reproduce con locales `table`/`session` declarados antes.
  - El pool .sdata de wide strings del original tiene relleno a 4 entre literales (L"%d" en +0x10) que nuestro MWCC no genera (0x22 frente a 0x28 bytes).

## MP/D2AMpMenuFriend — 15/21 al 100%, matching: no
- NONMATCHING: fn_804B6F08 99.88% y fn_804B7F88 99.89% (vector<int>::push_back inlineado: el original carga size antes que capacity, problema abierto de std/vector.h), fn_804B79F4 98.71% (registros decide/pad y huecos de pila de los SoundRef y del UpdateScrollBar_unk inlineado, igual que FriendRoom::fn_804BA970), fn_804B83F4 99.94% (registro de `result`), fn_804B9014 97.70% (asignación de registros: el original usa uno menos antes del bucle), fn_804B9694 99.80% (registros índice/layout del texto L"").
- Decisiones:
  - splits.txt: .text 0x804B6988-0x804B9D44 (creador y fn_804B9014..fn_804B9D40, antes en auto_03_804B6988/auto_03_804B9014), .rodata 0x80758368-0x80758600 (double, tablas de 10 y 12 int, nombre RTTI, pool), añadidos .sdata 0x8087E100-0x8087E108 (L"") y .sbss 0x8087F5B0-0x8087F5B8; .sdata2 0x808873A8-0x808873D0.
  - symbols.txt: funciones, sInstance, @stringBase0 (0x807583DC); lbl_808873C8 de size 0x8 a 0x4 (es el float 12.0 seguido de relleno; con 0x8 objdiff lo trata como double).
  - include/MP/D2AMpMenu_unk.h: SetTaskText_unk movido aquí desde D2AMpMenuResult.cpp (compartido). Regress: 0 perdidas.
  - Rebuild_unk usa mIds.__shrink_unk(0) (el original calcula size - size, como erase(begin(), end())); clear() da li 0.
- Hallazgos MWCC:
  - Las variables declaradas antes reciben registros no volátiles más altos: declarar `name` antes que `list` en fn_804B9B68 y `int id;` antes que `list` en Rebuild_unk dio el orden del original.
  - `if (!s[0]) s = Get(); usar(s)` y `usar(s[0] == 0 ? Get() : s)` generan copias distintas (mr en una rama u otra); el original usa la ternaria en fn_804B9014/fn_804B6F08 y la asignación con temporal en fn_804B9B68.
  - fn_804B988C (pequeña) se autoinlineaba en sus llamadas; siete ((void)0) la dejan como llamada, igual que SetRule::fn_804CEF84.
  - En fn_804B9B68 el basic_string temporal va en un bloque propio para que su destructor vaya antes de `m318 = -1`; el .data() se toma antes del if (una sola comprobación SSO).
  - fn_804B8E14 pasó al 100% sin cambios propios tras ajustar otras funciones del fichero (el reparto de registros de una función puede depender del resto de la unidad con -ipa file).

## Event/Dialog — 9/17 al 100%, matching: no
- NONMATCHING: ctor 95.03% (asignación de registros de los bucles de eventos; el ctor de elemento que recibe __construct_new_array es 0x8007A748 en el original), fn_80228CA0 99.78% (solo el orden de .sdata2: el original tiene 0.0f justo detrás de 1.0f, primer uso no encontrado, y las constantes siguientes quedan desplazadas una posición), fn_802297AC 89.06% (planificación de las copias de niebla/bloom y .sdata2), vf14 99.64% (registros de r/g/b), GetPath_unk 99.57% (registros dentro del basic_string(const char*) inlineado), fn_80228FD8 99.78% (orden de incremento índice/desplazamiento), fn_802292AC 89.60% (planificación del ResetPos_unk inlineado), fn_8022960C 99.32% (puntero reducido/carácter en r30/r31).
- Decisiones:
  - splits.txt: .rodata ampliada a 0x80742E48-0x80742F68 ("SE_UICOM_027", tabla sPos de 4x12 bytes, dos dobles, nombre RTTI y pool con los mensajes de basic_string).
  - symbols.txt: funciones renombradas (GetPath_unk__5Event... y sPos__5Event con scope:local), @stringBase0 (0x80742EA8, local).
  - Usa MP/D2AMpMenu_unk.h para MpD2A_unk/atn::D2A_Task (SetParam2_unk) en vez de atn/D2A_Task.h (declara fn_801F3FF8 con otro tipo).
  - Copias de float con el asm paired-single aprobado de math/Vec.h (__Vec4Copy/__Vec2Copy) dentro de constructores de copia/operator= de structs propios (DialogUv_unk, DialogVec4_unk, DialogVec2_unk).
  - Externos de otras unidades declarados extern "C": lbl_8087DBB8..lbl_8087DBD4 (objetivos de niebla/bloom en .sdata), lbl_8087DBD8 (L" "), lbl_8087D760/lbl_8087D764 (cadenas del new[] de depuración).
- Hallazgos MWCC:
  - Un struct de floats devuelto por valor y copiado con psq_l/psq_st (fn_80124C6C) solo sale con constructor de copia propio basado en __Vec4Copy: así además se construye el retorno en sitio (sin temporal + lwz/stw).
  - Una función externa a la que el original no pasa el tercer argumento (fn_800DAB68: r5 queda con basura) se declara con dos parámetros; con el `1` explícito sobraba un li.
  - En fn_80228AF8 llegó al 100% declarando `int i;` antes de la clave en el inline FindHead_unk y leyendo field->m4C en un local `map`.
  - Los arrays de structs con ctor (`Head_unk() : mKey(0) {}`) generan el bucle de construcción desenrollado x8 del original; poner la clave a 0 a mano no lo genera.
  - `switch` con dos case da beq/beq/b como el original; if/else if da bne/cmpwi/bne.

---

# Cola q8_shopcamp

## q8_shopcamp - notas del agente

Repo: /home/claude/work/q8_shopcamp (rama agent-q8_shopcamp). Referencia: notes/q8_shopcamp_ref.json.

## game/BattleResult - 8/9 al 100%, matching: no (commits b58ad49, 915eb20)
- ctor resuelto con un inline envoltorio de fn_801F64D0 (ver hallazgos).
- NONMATCHING fn_800F0D14 99.33% - solo asignacion de registros en los bucles (iteradores/contadores).
- Decisiones:
  - splits.txt BattleResult: .text hasta 0x800F13BC (fn_800F1378/fn_800F13B0 son metodos de BattleResult: escriben 0x15C/0x170 y GameTask los llama con sInstance), .rodata 0x80735440-0x807355C8 (dobles de conversion + nombre RTTI + @stringBase0), .sbss 0x8087F040-0x8087F048 (sInstance), .sdata2 desde 0x808813B8 (0.0f usado por ctor y fn_800F1378). Solo afecta a tramos auto_ vecinos.
  - symbols.txt: renombrados (pairnames) + sInstance__12BattleResult (size 0x4) y @stringBase0 (size 0x162, scope:local).
  - include/game/BattleResult.h nuevo (no compartido).
- Hallazgos MWCC:
  - Comparacion `>=` de floats via bool (mfcr + rlwinm.) = inline que devuelve bool (IsEnd_unk).
  - `lwzu` sobre un FixedArray externo = puntero local `T* a = &g->mArr;` (con referencia o acceso directo se pliega el offset); el iterador arranca en `a` solo si se accede como `a->mData[i].campo` sin puntero local al elemento.
  - Lerp con un unico (float) del origen = inline `Lerp(a, b, t)`; el orden de los args (der. a izq.) fija el orden de conversiones.

## game/NewCampPartsTop - 9/12 al 100%, matching: no (commit f3c4849)
- NONMATCHING ctor 98.88% (registros: std::string inline y puntero de entrada), vf14 99.72% (f30/f31 y registros de SetPaneText_unk), vf2C 99.51% (el original pasa un 3er arg 0 a MenuLoadTask::fn_80117914, declarada con 2 en game/CampSaveLoad.cpp; carga de 16.0f de fn_8006EF48 programada antes).
- Decisiones:
  - splits.txt NewCampPartsTop: .rodata 0x8075D0D0-0x8075D310 (cadenas de la tabla sFile, tablas sInfo/sActions, doble, nombre RTTI, pool), .data 0x80793720-0x807937E8 (jumptable de vf2C + vtable + RTTI + L"%d:%02d:%02d").
  - symbols.txt: renombrados (pairnames) + fn_801D082C -> __ct__21NewCampPartsEntry_unkFv (copia debil en CosEditProtector, no decompilada), @stringBase0 0x8075D1A0, @wstringBase0 0x807937C8.
  - include/game/NewCampPartsBase.h (COMPARTIDA con NewCampPartsGmk): miembros 0x50-0x90 tipados (m50 ArrayPtr_unk<NewCampPartsEntry_unk>, m7C estado, m80 cursor, m84..m90), structs NewCampPartsEntry_unk/NewCampPartsFile_unk, firmas vf3C(int), vf54/vf58(entry, name), vf5C(entry). regress.py: 0 perdidas.
  - include/game/NewCampPartsTop.h nuevo.
- Hallazgos MWCC:
  - Struct devuelto por valor sin ctor -> se copia a la variable; con ctor vacio `T() {}` se construye en sitio (RVO).
  - delete de un global con destructor inline: doble `beq` (null del if + null del delete).
- Notas para otras unidades: fn_805290F8 (auto, .ctors) es el __sinit de NewCampPartsStatus; fn_8052A888 (create de NewCampTask) y auto_03_8052C108 son de NewCampTask (jumptable 0x8079380C antes de su vtable).

## game/NewCampTask - NO EMPEZADA
- Revisado al final: el constructor llama fuera de linea a casi todos los inline (Vector3, DegToRad_unk, operator+) por presupuesto, y depende de Camera_unk/Light_unk/NewCampScene2_unk sin cabecera comun; no se ha escrito codigo.
- El fichero real va de 0x8052A888 (create) a 0x80530FB0 (+ __sinit auto_fn_80530FB0): ~26 KB y ~60 funciones (auto_03_8052C108 son metodos de NewCampTask: los llama NewCampPartsTop con m48). Se deja para el final.

## game/ShopColor - 13/15 al 100%, matching: no (commit 915eb20)
- NONMATCHING vf38 99.38% (push_back inline de std::vector: el original carga size antes que cap, problema conocido de std/vector.h; huecos de pila de __vec_max_unk; punteros de __push_front_unk intercambiados), fn_805880F0 97.90% (solo registros).
- Decisiones:
  - splits.txt ShopColor: .text desde 0x80586DB0 (creador singleton, pool empieza por ""), .rodata hasta 0x80761AA0 (pool), .data hasta 0x80796970 ("Memory allocation failure" de std::vector), .sbss 0x8087F9D0-0x8087F9D8.
  - symbols.txt: fn_8057F280 -> __ct__8ShopBaseFPQ23atn4Taskiii, fn_8057F7A8 -> __dt__8ShopBaseFv (ShopBase no decompilada), pairnames, sInstance__9ShopColor, @stringBase0.
  - include/game/ShopBase.h nuevo (layout de ShopBase desde su ctor; funciones de la base como extern "C" fn_ con self).
  - include/std/vector.h (COMPARTIDA): clear() resta el tamano por referencia (no se pliega a 0 con elementos POD). regress.py: 0 perdidas.
  - Especializacion explicita de std::__vec_imp_unk/__vec_split_unk para ShopColorEntry_unk en include/game/ShopColor.h (ruta escalar como int).
- Hallazgos MWCC:
  - Literal pasado a un inline (envoltorio de fn_801F64D0) dentro de un bucle -> su direccion se iza fuera del bucle y reutiliza el registro del pool; con el literal directo no se iza y con locales con nombre cambia la asignacion.
  - Struct pasado por valor a una funcion extern: declarar el parametro por valor (no puntero a copia) evita que se ice la direccion de la copia.
  - Bucle con `--p; --last; new (p) T(*last)` se convierte en bucle de contador (ctr/divwu); `new (--p) T(*--last)` no.

## game/ShopIdentify - 16/20 al 100%, matching: no (commit dad421b)
- NONMATCHING fn_805897D4 98.66% (registros volatiles), fn_80589DE0 97.60% (programacion de 14.0f/pool, un registro), fn_8058A0F0 96.33% (doble lectura del texto del mensaje, huecos de pila).
- fn_8058A3AC (0x710, copia debil de un helper de insercion de std::vector MSL) NO reconstruido: se llama como extern "C" fn_8058A3AC(vector*, pos, &x, &tag) desde un inline Insert_unk. Decision a revisar.
- Decisiones:
  - splits.txt ShopIdentify: .text 0x80588684-0x8058AABC (creador + insert debil al final), .rodata hasta 0x80761C90, .data 0x80796970-0x80796A20 (jumptable de vf10 + vtable + "Memory allocation failure"), .sbss 0x8087F9D8-0x8087F9E0.
  - Literales anchos L""/L"1"/L"WEAPON" del fichero estan en .sdata (0x8087E690, 0x18 bytes): no se meten en el split; se usan como extern lbl_8087E690 (+0/+1/+3). No se como produce MWCC literales anchos en .sdata.
  - fn_8058AABC (entre el insert debil y el creador de ShopUpGrade) se deja para ShopUpGrade (lo usa UpGrade).
  - ShopBase.h: ShopEntry5C_unk tipado, vf24(int), fn_805846C0 con 6 args.
- Hallazgos MWCC:
  - Llamada a funcion con `this` de vector: pasar el vector por un inline (Insert_unk(v, pos, x)) iza this+0x20000 y separa &v de la carga de begin(); la llamada directa genera lwzu.
  - Cadena if/else con `x == 7 || x == 8` -> `subi; cmplwi 1` (no hace falta switch).

## game/ShopBuySell - 13/16 al 100%, matching: no (commit 2a4d1b0)
- NONMATCHING fn_8058621C 98.82% (registros, orden de carga de args), fn_80586620 99.68% (programacion), fn_805866F4 93.58% (layout de SetPaneText en registro salvado, orden del rectangulo de icono, temporales).
- Decisiones: splits.txt ShopBuySell: .text desde 0x80584FE4 (creador con 4 args), .rodata 0x80761670-0x80761908 (tablas de parametros + doble + nombres RTTI debiles + pool), .data 0x80796840-0x807968E8 (jumptable + vtable + L"WEAPON"/L"--"), .sbss 0x8087F9C8. symbols: pairnames, sInstance, @stringBase0, @wstringBase0.
- Hallazgos MWCC:
  - `m[i] = f(); m[i]->g();` (dos sentencias) calcula la direccion del elemento antes de la llamada; `(m[i] = f())->g()` la calcula despues.
  - max(x,0) como `x < 0 ? 0 : x` -> srawi/andc; `x > 0 ? x : 0` -> neg/andc/srawi/and.
  - Orden de declaracion de locales puntero (equip, base, anime) fija r28/r27/r26.

## game/NewCampPartsHint - 14/19 al 100%, matching: no (commit 5aa96a9)
- NONMATCHING vf14 99.46% (registros), vf20 99.52% (registros del bucle de categorias + push_back inline), vf3C 96.90% (registros: direccion de la entrada y desplazamiento intercambiados), vf30 99.74% (push_back inline), fn_80516004 96.94% (push_back inline + orden de registros salvados).
- El push_back inline de std::vector<T*> no casa por el problema ya documentado en include/std/vector.h (el original carga el tamano antes que la capacidad); ademas el "Memory allocation failure" sale como @STRING@ debil propio en vez de lbl_80775A88.
- Decisiones (a revisar):
  - splits.txt NewCampPartsHint: .text desde 0x80514720 (el creador estaba en un hueco auto, no en el split de Gmk), .rodata 0x8075B580-0x8075B988 (cadenas de la tabla sFiles, sFiles, "hint_board", sCategories, nombre RTTI, pool).
  - El "" de 0x80887838 (.sdata2) solo lo usa la tabla sFiles de Hint pero cae dentro del split de NewCampPartsGmk (.sdata2 hasta 0x8088783C): no toco el split ajeno; la tabla usa extern lbl_80887838. Recomendado: mover 0x80887838-0x8088783C a Hint cuando se toque Gmk.
  - Los floats 0.0f/1.0f de .sdata 0x8087E480/0x8087E484 (objetivos de mOpen) se dejan en el split automatico y se usan como extern: MWCC manda a .sbss una variable inicializada a 0 (probado: literal 0, 1.0f-1.0f, volatile, array, struct), solo -0.0f va a .sdata. El original probablemente usaba explicit_zero_data.
  - symbols.txt: pairnames + renombrados manuales (ctor, vf14, vf20, vf3C, vf48) y @stringBase0 (0x8075B7A4, scope:local).
  - include/game/NewCampPartsBase.h (COMPARTIDA con Gmk/Top): m5C es atn::LH_Xsca + float m64[6], m95/m96/m97, Vector3 mA4/mB0/mBC/mC8, vf40(void* group), vf60(entry, int, int, float). regress.py: 0 perdidas.
- Hallazgos MWCC:
  - Vector suma `a * 0.0f + b * t` evalua primero el argumento; `v = a * 0.0f; v += b * t;` evalua en el orden del original.
  - Bucle con m50[i + 5] -> el original no reduce la fuerza (slwi de (i+5) en cada vuelta, base en registro): `int index = i + 5; Entry* entries = m50.mData;` dentro del bucle lo reproduce.
  - Argumento `&m50[0]` en vez de `m50.mData` invierte el orden de carga de los argumentos (r5 antes que r4).
  - Constante 0.0f compartida por un bucle y una llamada posterior en f31: variable local `float frame = 0.0f`.
  - Campo de bit `x = show == 0` -> cntlzw + rlwimi directo; `!show` anade un srwi.

## game/NewCampPartsStatus - 23/29 al 100%, matching: no (commit 2883ede)
- NONMATCHING vf14 99.70% (registros), fn_80527938 99.40% (registros), fn_80528310 96.47% y fn_80528454/fn_80528574 93.11% (el original calcula base + 0x58 en cada llamada; aqui se calcula una vez), vf3C 98.36% (mismo intercambio de registros que NewCampPartsHint::vf3C).
- Decisiones (a revisar):
  - splits.txt NewCampPartsStatus: .text 0x805268C8-0x80529118 (creador fn_805268C8 y __sinit fn_805290F8 estaban en huecos auto), .ctors 0x8072D3B8-0x8072D3BC, .rodata 0x8075CC30-0x8075D0D0 (cadenas de sFiles, sFiles, "status_board", sMoveFlags, nombres de cursor, nombre RTTI, pool), .bss 0x807C9110-0x807C9120 (Vector3 estatico sOffset), .sdata 0x8087E4A0-0x8087E4A4 (L"-" del fichero: MWCC pone el pool @wstringBase0 en .sdata), .sdata2 desde 0x80887A58 (el "" de la tabla estaba en hueco auto).
  - symbols.txt: pairnames + renombrados manuales (vf24, vf28, fn_80528310/454/574/BF0, copias debiles de FixedArray_unk), @stringBase0 0x8075CED4, __sinit scope:local.
  - include/game/NewCampPartsBase.h (COMPARTIDA): NewCampPartsFile_unk::m08 pasa a unsigned char (la tabla de Status pone 01 en el primer byte). regress.py: 0 perdidas.
  - Copias debiles en este fichero: atn::FixedArray_unk<Set,7>::operator[] (0x80527400, tras vf14), party GetStatus_unk (0x805282A8, la usa tambien NewCampTask), getter de texto (0x805282B8) y FixedArray_unk<Skill,10>::push_back (0x805282CC). Salen solas porque fn_80527938 supera el presupuesto de inline; size()/clear() de la lista se llaman como fn_80372574 / GameUICounter_unk::fn_803CF734 (cast).
- Hallazgos MWCC:
  - `strcmp(x, "\0")` da el literal de 2 bytes 00 00 (dos "" seguidos en el pool).
  - Temporales struct inicializados con {a, b, c, d} con constantes copian de una plantilla en .data; asignar campo a campo en el orden index, text, tension, help reproduce las escrituras.
  - Lerp vectorial (b - a) * t + a en un inline (LerpFrom_unk) cambia la asignacion de f29-f31 frente a escribirlo en linea.
  - Leer mPage y el tamano a locales al principio de fn_80528BF0 lo deja al 100%.
  - Cuerpo de bucle `set.mChara = camp->mMembers[set.mIndex].mChara` (indice releido del struct) iguala el orden de incrementos.

## game/ShopUpGrade - NO EMPEZADA
- El fichero real llega a 0x8059AB98 (~65 KB con muchas instanciaciones de sort); vf3C/vf40 de su vtable estan en la zona auto. fn_8058AABC (entre el insert debil de ShopIdentify y el creador de UpGrade) queda para esta unidad.

## Resumen final
- 7 unidades parciales (matching: no), 0 al 100%, 2 sin empezar (NewCampTask, ShopUpGrade).
- +96 funciones al 100% (regress: 4093 frente a 3997 de la referencia, 0 perdidas). main.dol: OK.

---

# Cola q9_game

## q9_game - notas del agente

Repo: /home/claude/work/q9_game (rama agent-q9_game). Referencia: notes/q9_game_ref.json.

## game/SkillEffectManager - 8/11 al 100%, matching: no (commit f7701cb)
- El fichero real es mucho mayor que el split: empieza en los metodos de Bullet_unk (0x80598874, ctor del elemento del array) y sigue tras 0x8059C084 hasta ~0x8059D7D8 (metodos de SkillEffectManager llamados por CharaTask, usan .sdata2 0x808881F8-0x80888214). No se ha ampliado .text: ~15 KB mas.
- NONMATCHING vf10 98.69% (registros de contadores de bucle), fn_8059B66C 99.01% (registros de parametros), fn_8059B8A8 99.46% (programacion de la carga de skill->m58).
- Decisiones:
  - splits.txt SkillEffectManager: .rodata 0x80762128-0x807622E0 (cadenas sueltas de los .efp apuntadas desde .sdata2, doble, nombre RTTI y @stringBase0; antes 0x80762260-0x80762274), .sdata2 hasta 0x808881FC (incluye el 1.0f de fn_8059B8A8; el resto de .sdata2 es de funciones aun en auto).
  - symbols.txt: renombrados (pairnames + manual: vf0C/vf10/vf14, fn_8059B8A8), fn_80598874/fn_80598900 -> __ct__/__dt__10Bullet_unkFv, lbl_80762274 -> @stringBase0 (scope:local). fn_8059B66C y lbl_8087F9E8 (sInstance) se dejan con nombre fn_/lbl_ porque gmk/HealPoint.cpp y game/CharaTask.cpp los declaran extern "C"; fn_8059B66C se define como extern "C" con `self`.
  - include/game/SkillEffectManager.h nuevo (Bullet_unk 0x140 inferido; no compartido).
- Hallazgos MWCC:
  - Accesor con comprobacion de rango `if (i >= 32) return 0;` que el original conserva dentro de un bucle `i < 32`: debe ser metodo no estatico llamado sobre el singleton (`sInstance->Get(i)`); como estatico o con ternario el compilador elimina la comprobacion.
  - Funcion debil de otro fichero llamada fuera de linea aunque todo lo demas se inlinea: declararla sin cuerpo en este fichero (con -ipa file, definirla despues del uso no basta: se inlinea igual).
  - Bucle sobre FixedArray con `p = &arr + 8*i` reducido a un puntero: contador `int` (con `unsigned` se recalcula base+offset en cada iteracion).
  - lbz + cmpwi (no cmplwi) sobre un unsigned char = leido a traves de un inline que devuelve int.
  - Orden de registros de locales en un bloque: declarar antes (sin inicializar) los que el original tiene en registros mas altos.

## game/MapTask - NO EMPEZADA (aplazada)
- El fichero real empieza en 0x8049D264 (creadores fn_8049D68C/fn_8049D704 y callback fn_8049D264) y sigue tras 0x8049ED90 (fn_8049ED90, fn_804A04AC, fn_804A0BCC, fn_804A0F80 son metodos de MapTask); el pool 0x80756EC8 y .sdata2 0x80887160 quedan fuera del split.
- El ctor real 0x8049D974 recibe (parent, ?, tipo, id1, id2), pero include/game/MapTask.h y game/TownTask.cpp lo usan como MapTask(parent) con el nombre __ct__7MapTaskFPQ23atn4Task: cambiarlo obliga a tocar TownTask. Se deja para el final.

## game/GameOverTask - NO EMPEZADA (aplazada)
- Split .rodata corto: el fichero usa la tabla lbl_8074DDF8 (0x9C) y el pool lbl_8074DEC0 (0xC8): .rodata real 0x8074DDF8-0x8074DF88. 6 funciones grandes (0xB1C, 0x4C0...) con MenuLoadTask/MenuCommonTask/CampSaveData.

## game/Arms - 17/20 al 100%, matching: no (commit 164e9ef)
- NONMATCHING fn_8004439C 99.84% (registros float de la conversion grados->radianes), vf10 99.90% (hueco de pila del vector de escala), fn_80044FBC 99.93% (registro de `last` en el caso 8).
- Decisiones:
  - splits.txt Arms: .rodata 0x80730E28-0x80730F10 (nombre RTTI, nombres RTTI debiles de atn::CsvTab/CsvBase, @stringBase0), .data 0x807773A8-0x807774F0 (ptmf de scoped_ptr::get x2, jumptables de fn_80044FBC/fn_80045510, vtable/RTTI, vtables/RTTI debiles de CsvTab/CsvBase).
  - symbols.txt: renombrados a metodos de Arms (fn_80044394 = get__Q25boost33scoped_ptr<Q23atn13EfpHandle_unk>CFv), lbl_80730E5C -> @stringBase0 (scope:local). fn_80044E0C se deja extern "C" (game/CharaTask.h lo declara asi, con void*).
  - include/game/Arms.h ampliado con el layout completo (0x2A8) y ArmsCond_unk/ArmsData_unk/ArmsEffect_unk (inferidos). Sword.cpp lo incluye: regress 0 perdidas. Arms es la clase que CharaTask ve como chr::CharaPart_unk (m648/m64C).
- Hallazgos MWCC:
  - `i < 0 || i >= 2` se convierte en una comparacion sin signo; `i < 0 || 2 <= i` conserva las dos comparaciones con signo.
  - Locales de un bloque que el original tiene por debajo de los temporales del llamador: moverlos a un inline (los locales de un inline van en orden ascendente despues de los del llamador); un argumento del inline se evalua antes del cuerpo (carga temprana) y un acceso dentro del cuerpo, tarde.
  - Dos structs locales cuyos campos no nulos se escriben al final: ctor por defecto (ceros) + asignacion, no ctor con parametro.
  - Rotacion inline `Mtx t; Mtx r;` (t declarado primero) da el orden de pila del original en Arms::vf10.

## game/MapPartsLocatorSet, game/TeamAIManager - aplazadas (ver mas abajo si se retoman)
- MapPartsLocatorSet: pool lbl_80756B84 (0x144) fuera del split .rodata; usa Texture_unk (0xDC), Buffer de atn::ModelInstance* (0x344) y bitfield 0x340.
- TeamAIManager: .rodata real 0x80763C98-0x80763DC8 (nombres de estado SJIS sueltos + tabla lbl_80763CD8 + pool); los literales "" de Buffer_unk::Resize_unk estan en .sdata (0x8087E6F0/F4); vf14 (0xBEC) es un volcado de depuracion.

## game/EnvSeBase - 35/41 al 100%, matching: no (commit 28ab1a1)
- Clases EnvSeBase/Point/Line/Sphere/Box/Global + EnvSeArea_unk (metodos sueltos al final del fichero).
- NONMATCHING: EnvSeBase::vf20 86% (recarga de lbl_8087EFE8 y `this` en r31), EnvSeLine::vf1C 14.7% (tokenizer/vector: presupuesto de inline), EnvSeLine::vf10 95.2% (pila), EnvSeLine::vf14 99.93% (pila, 4 bytes), EnvSeBox::vf10 77.8% (pila/programacion), EnvSeArea_unk::fn_80363108 99.85% (offsets del pool, depende de vf1C).
- Decisiones:
  - splits.txt EnvSeBase: .rodata 0x8074B630-0x8074B770 (literales de la tabla sTypeNames, la tabla, nombres RTTI y @stringBase0), .sdata2 desde 0x80885690 ("Box" de la tabla va a .sdata2 por tamano 4).
  - symbols.txt: todos los fn_ del split renombrados a metodos (ninguno referenciado desde C++ de otras unidades), lbl_8074B6B8 -> @stringBase0 (scope:local).
  - include/game/EnvSeBase.h nuevo: EnvSeData_unk (0x58), EnvSeManager_unk (lbl_8087F418, solo los miembros usados), clase alineada a 8 (los new usan tamanos redondeados a 8).
  - EnvSeBase::DrawName_unk es una funcion miembro normal que se autoinlinea en todos los vf14 (su copia fuera de linea no esta en el DOL): necesaria para el orden de .sdata2 (12.0 antes que 3.0).
- Hallazgos MWCC:
  - Formatos "%s[%d] %s\0" (NUL extra explicito): en el pool sale un "" detras del formato que no se fusiona con los "" del fichero (creadores).
  - Un literal de cadena corta (<= 4 bytes, "Box") de un inicializador de tabla `const char* const[]` va a .sdata2; los demas son objetos sueltos en .rodata alineados a 4.
  - Constantes .sdata2 en orden distinto al del primer uso en los llamadores = funcion miembro no inline definida antes (autoinlineada en todos los usos y eliminada por el enlazador).
  - Funcion inline libre: sus literales van a @STRING@<func>, no al pool del fichero; una funcion miembro normal (autoinlineada) usa @stringBase0.
  - Comprobacion `a == 0 || f(a) != 0 || a->m != 0` en una sola condicion da `bne fin` + `beq cuerpo; b fin` como el original; con returns separados no.
  - Argumento float que el original carga antes que un ternario de color: en la declaracion extern el float va antes del entero (el orden de evaluacion de args sigue la declaracion).

## game/LibraryTask - 18/23 al 100%, matching: no (commit 5ba7083)
- Clases LibraryTask (lista de libros, singleton) y BookTask (lector) + LibraryPage_unk (pagina, ctor/dtor fuera de linea por new[]).
- NONMATCHING: ctor 99.69% (registros r30/r31 de la ruta y el offset del bucle), vf14 99.51% (registro del temporal del inline frente al contador), fn_80469D60 97.30% (direccion del CsvU en registro guardado durante el ctor, problema conocido de CampSaveLoad; push_back carga size antes que cap, ver std/vector.h), BookTask::fn_8046AE7C 98.89% (registros de temporales de inlines, 20.0f cargado primero, slots de los tags), fn_8046B798 96.14% (CsvU igual, offset de ".texture" en el pool por orden de emision de las cadenas de basic_string, orden de stores de la copia corta).
- Decisiones:
  - splits.txt LibraryTask: .text 0x80468F54-0x8046BBB0 (creador fn_80468F54 y metodos de BookTask del fichero), .rodata 0x807552E0-0x80755608 (cadenas SE_EV1605 + tabla, doble de conversion, tabla de d2a del libro, nombres RTTI, @stringBase0), .data 0x8078FAB0-0x8078FB50 (vtables/RTTI y "Memory allocation failure"), .sdata2 0x80886E88-0x80886EC0.
  - splits.txt LastCocoonQuadruped (unidad SIN fuente): .sdata2 end 0x80886E84 -> 0x80886E88. El float 0.87266463 (50 grados en radianes) es del __sinit fn_80468F48 de ese fichero; dtk no admite un hueco de 4 bytes sin split y dejarlo en LibraryTask desplazaba todas las constantes (no emparejaban). Revisar.
  - symbols.txt: fn_ renombrados a metodos; lbl_8087F508 -> sInstance__11LibraryTask; lbl_80755310 -> sSeTable, lbl_80755370 -> sBookD2A, lbl_80755394 -> @stringBase0 (scope:local). Las .sdata lbl_8087E030-E040 (FontW/FontH/FontWindowW/LineNum/LineSpace) quedan extern (fuera del split).
  - include/std/string.h (compartido): anadido basic_string::append(const basic_string&) y operator+= de string (inline nuevo, regress 0 perdidas).
  - include/game/LibraryTask.h: especializaciones std::__vec_imp_unk/__vec_split_unk<LibraryBook_unk> (el original asigna el elemento tras incrementar el tamano, como con punteros, pero __push_front_unk construye con new de colocacion).
- Hallazgos MWCC:
  - Llamadas a helpers con (layout, hash(name), valor): el original pasa el layout como parametro de un inline (se evalua antes del hash); con el calculo dentro del inline el orden cambia.
  - `int state = mState; switch (mState) {...} if (state == 3)` reutiliza los cr del switch (cmpwi cr6/cr1); con switch(state) o if-chain no.
  - min/max por valor (no por referencia) para float: sin temporales en pila.
  - `(float)__fabs(x) < FLT_EPSILON` en vez de fabs() de biblioteca.
  - Booleano asignado `x = a > 0 && f() == 1;` da el orden de registros del original frente a `x = 0; if (...) x = 1;`.
  - wchar_t comparado con L';' (cmplwi) en vez de ';' (cmpwi).

## game/MiniMapTask - NO EMPEZADA (aplazada)
- El fichero real va de 0x804A7EA4 a ~0x804AC488 (~18 KB; el split .text solo cubre hasta 0x804A9E44: siguen accesores pequenos 0x804A9E44-0x804A9EAC y metodos grandes fn_804A9EB4..fn_804ABF84, p.ej. fn_804AA490 llamado desde vf0C). El pool lbl_8075765C (0x1C4) queda fuera del split .rodata (fin real 0x80757820). Clase con herencia multiple (vtable secundaria en +0x1C guardada en 0x48), Model_unk 0x208, Camera_unk 0x640, 9 Texture_unk, arrays de registros 0x2C/0x160 y 0x8; funcion de dibujo de 0x14F8.

## game/GameChoiceWindow - NO EMPEZADA (aplazada)
- 26 funciones (~9.7 KB) de GameTextMessageWindow/GameMessageWindow/GameChoiceWindow; el pool lbl_8074FD38 (0x210) queda fuera del split .rodata (0x8074FCF8-0x8074FD38).
