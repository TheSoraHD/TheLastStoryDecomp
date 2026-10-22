# Lote close-units, cola B (gmk y db)

Una linea por unidad procesada (orden de cola_B.md).

- db/ArrowSet_unk: saltada - fn_80207310 es el bloqueo documentado del push_back en bucle (mwcc-blockers-q1).
- db/ChaseParam_unk: fn_8020A90C 97.94% -> 99.05% (copia `int len = size` para el constructor de atn::Csv). Queda solo r30/r31 (cero/pool de cadenas vs data) intercambiados. No enlaza (1/1 sin 100%).
- db/MapEnvSeTable_unk: fn_80214F80 97.71% -> 99.21% (contador declarado antes que el puntero y `entry++, i++`). Queda r8/r12 intercambiados.
- db/MemberList_unk: fn_80217D9C sin cambios (98.39%); ~10 variantes (puntero a la ultima fila, n = count - 1, comparaciones invertidas, aritmetica de punteros), ninguna mejora.
- db/RandomItemTable_unk: dtor debil de FixedArray_unk<RandomItem_unk,8> sin cambios (96.30%). Una struct derivada (o miembro/base con destructor inline propio) da los dos beq, pero MWCC emite ademas una copia debil sin referencias del destructor interior (0x68 bytes) que el original no tiene. Comentario actualizado.
- db/SkillParam_unk: saltada - presupuesto de inline ya investigado (fn_80219F1C se inlinea, como db/CharaIdTable_unk).
- db/TreasureBoxTable_unk: fn_8021E5A4 96.86% -> 98.91% (`GetRate_unk(item, (int)flags)` en el segundo bucle hace que MWCC recalcule las mascaras, como el original). Queda asignacion de registros guardados.
- db/WeaponParam_unk: saltada - la copia debil pendiente es boost::token_iterator::operator++ (libs/boost, no se toca).
- gmk/Augur: vf10 sin cambios (99.2%); 9 variantes mas en el estado 5 (anotadas en el comentario).
- gmk/Banana: vf40 escrita desde 0 -> 100% (12/12). No enlaza: el .o define el static local sListeners de fn_8014F8F4 y su guarda (como gmk/Item), se deja matching false.

- gmk/BulletPoint: vf10 escrita desde 0 -> 98.52% (7/8; patron de gmk/FlamingArrow). symbols.txt: `lbl_8075312C` -> `@stringBase0` (scope:local data:string_table, pool "" de la fabrica; necesario para que la fabrica siga al 100% al anadirse el double de conversion en .rodata). Pendiente: recarga del contador del vector de objetivos, orden 0.1f/1.0f en .sdata2 y registros del Collect inlineado.
- gmk/CharaObj: saltada - mismo bloqueo documentado que TodomeCountUp::vf10 (marco de pila + bctrl en vez de llamada de cola).
- gmk/Fireworks: vf10 sin cambios (97.9%); ~9 variantes del bucle de destellos.
- gmk/HealPoint: fn_8040ED00 sin cambios (93.75%); ~9 variantes del constructor inline (rotacion local antes/despues de m54, temporal Vector3, Set_unk, referencia a desc).
- gmk/LandMine: vf28 sin cambios (98.33%); ~10 variantes (orden de stores de m78, casts a double, copia del centro, stores directos de la esfera).
- gmk/LottaSummon: vf10 sin cambios (99.94%); ~9 variantes de Back_unk y la rama.

- gmk/OpenBoard: vf10 escrita desde 0 -> 94.8%; vf28 sube a 100% (la mezcla de fn_80097C08 era 0.1f, no 1.0f: bug de constante, ahora el .sdata2 coincide). 12/13. Pendiente en vf10: copia de la caja (3 instr. de mas por la pila), registros float de la interpolacion m56C y orden de stores de los vectores cero del efecto. Helpers inline nuevos en include/gmk/OpenBoard.h (Start_unk/Update_unk/Interp_unk/Finish_unk; nombres inventados).
- gmk/PassBoard: vf10 escrita desde 0 -> 99.5% (11/13; vf0C sigue 95.4%). symbols.txt: `lbl_80753E70` -> `sSeName_unk` (.rodata, local) y `lbl_808867C0` -> `sSe_unk` (.sdata2, local; puntero a la cadena SE_DG040_008). Pendiente: orden en pila de las matrices de los dos __MakeRotation_unk inlineados.
- gmk/ReinforcePoint: sin cambios (99.81%), no da tiempo (patron parecido al FindPlace de BulletPoint).
- gmk/ShakeCamera, gmk/TodomeCountUp: saltadas - bloqueo documentado (marco de pila + bctrl / caso 1 con codigo eliminado).
- db/CharaIdTable_unk: saltada - presupuesto de inline ya investigado (fn_802088A4... fuera de linea).
- db/CharaParam_unk: sin cambios; fn_80209960 (99.41%): variantes del par/contador bajan el fuzzy aunque bajen las lineas distintas (revertido); la copia debil de tokenizer::assign depende de std/string.h (bloqueo documentado).
- db/EventIdTable_unk: 7/7 al 100% (fn_8020F75C y fn_8020F248: subindice unsigned; en fn_8020F248 ademas `int i` declarado tras la entrada). NO enlaza: probado `matching: true` -> main.dol FAILED. Nuestro .o emite copias debiles que el original no tiene en esta unidad (`__dt__Q23atn30Buffer_unk<16EventIdEntry_unk>Fv`, `__dt__/vf08__Q23atn19CsvBase<c,44,13,34>`), los vtables/RTTI de atn::Csv y CsvBase en .data (0x38 bytes; el original no tiene .data), y la guarda/sInstance como comunes en vez de .bss; .text 0x768 vs 0x6A0. Requiere tocar include/atn/Csv.h / Buffer (otras unidades): se deja matching false y anotado como pendiente.
- db/ItemRare_unk: fn_80213B78 -> 100% (indice int comparado como unsigned en HasItem_unk); fn_80213E60 98.90% -> 99.87% (copia HasItem2_unk con valor antes que subtipo; queda el orden de las dos cargas).
- db/MapIdTable_unk: fn_80216CD0 99.80% -> 99.85% (orden entry++, i++ en Find_unk). fn_802174F4 sin tocar.
- db/PaletteTable_unk: no tocada (locales sin tipo conocido / pool).
- gmk/AutomaticDoor: vf0C sin cambios (99.81%, 3 variantes mas).
- gmk/FallingTower: vf20 sin cambios (99.9%, 7 variantes del indice de la ola).
- gmk/FlyObjManager: vf44 99.45% -> 99.88% (subindice unsigned en FindNearest_unk). Queda 0.1f/1000.0f en .sdata2 y la velocidad por f1.
- gmk/MeetingSpot: fn_803FD0A0 99.75% -> 99.97% (local `id` en MatchId_unk; arregla el orden de operandos de cmpw). Queda solo el orden de 6400.0f en el pool.
- gmk/OnOff: fn_803F8324 sin cambios (99.16%, 8 variantes de copias de mMtx/mBoxPos).
- db/ParseUtil_unk: sin cambios (5 variantes mas; coincide con lo documentado).
- db/SpText_unk: no tocada (fn_8021C6A4 depende del orden del pool de cadenas anchas de codigo no reconstruido).

- gmk/Brazier: fn_80407E8C -> 100% (subindice unsigned en GetArenaEntry_unk + cuenta leida por GetCount_unk(), que recarga lbl_8087F610 en cada vuelta). 12/16. vf40 sin cambios (r27/r28).
- gmk/Bat: vf0C, vf14 y vf20 -> 100% (subindice unsigned en los bucles sobre mBats); vf10 99.69% -> 99.77%. 10/15.
- gmk/SparkCube: 19/19 al 100% (vf0C, vf10, vf40 con el indice int comparado como unsigned). NO enlazable: define el static local sListeners de fn_8014F8F4 (como gmk/Item); no se prueba matching true.
- gmk/Leaf: vf0C y vf14 -> 100% (subindice unsigned). 9/15. fn_80400D08 empeora con el truco (revertido).
- gmk/MotionObject: fn_804139D8 -> 100%. 26/29.
- gmk/LemonBucket: vf0C (bucle escrito en la funcion) y vf40 (HideLemons_unk en include/gmk/LemonBucket.h con indice int comparado como unsigned) -> 100%. 16/18. Cabecera compartida tocada (la incluye game/GimmickCommon.cpp): build/regress/fuzzdrop sin bajadas.
- gmk/Fireworks: vf10 97.9% -> 99.8% (subindice unsigned repetido en cada acceso).
- gmk/CharaLight: vf20 -> 100% (`5 > index`, constante primero: MWCC mantiene las dos comparaciones con signo). 13/15.
- gmk/Ocean y gmk/FallingTower: vf20 -> 100% (miembros de la ola escritos con mWaves[state][index] en cada linea, sin referencia).
- gmk/InteractionSpot, gmk/RandomMerchant, gmk/OnOff, db/OptionParts_unk, db/MapLink_unk, db/PlayerParam_unk: probadas variantes sin mejora (o con bajada de fuzzy, revertidas).

- gmk/WaitObject: escritas todas las funciones que faltaban: vf6C (96.5%), fn_8041AAC0 (100%), InArea_unk<T> (plantilla nueva: 3 instancias Npc/Enemy/Player, 98.5%), vf44 (91.8%), vf28 (99.8%), vf40 (99.2%), vf10 (99.97%), vf20 (99.95%). 15/27 al 100%, fuzzy de la unidad 99.0%. symbols.txt: fn_8041AAC0 -> fn_8041AAC0__Q23gmk10WaitObjectFv, fn_8041B28C/fn_8041B4D8/fn_8041B724 -> InArea_unk<7NpcTask>/<9EnemyTask>/<10PlayerTask> (nombres decorados). include/gmk/WaitObject.h: WaitState_unk::m18-m20 pasa a atn::Vector3 mMove (lo incluye game/GimmickCommon.cpp; sin bajadas). Lo que queda son sobre todo los nombres de los miembros safe-bool de boost::scoped_ptr (lbl_8078DC40...: objetos locales @NNNN en nuestro .o).
- db/SpText_unk: fn_8021C3F4 98.43% -> 99.62% y fn_8021B060 95.07% -> 96.78% (subindice unsigned).
- gmk/Leaf (2a pasada): fn_8040161C, fn_80401E50 y fn_804023E0 -> 100%; vf20 99.89%. 12/15.
- gmk/PigeonManager y gmk/ChickenManager: vf14 -> 100% en ambas. PigeonManager::vf10 99.74% -> 99.87% (el segundo bucle reutiliza `active` como contador; queda solo el orden de .sdata2, pendiente de las funciones de Pigeon_unk). ChickenManager::vf10: mismo truco + comparaciones sin signo probados, bajan el fuzzy (revertido; sigue el plegado `3||4` en cmplwi).
- gmk/LemonBucket vf10/vf20, gmk/FallingTower fn_8041044C, gmk/Brazier fn_80407D24: probadas variantes; solo quedan registros/orden de .sdata2, sin mejora.
- gmk/MapLift ctor/dtor: bloqueo documentado (destructor fn_800970CC en vez de ~LH_Motion); gmk/RotaryRoom vf40/vf6C/__sinit: solo orden de .sdata2 (vf0C sin escribir). No tocadas.
- gmk/RotaryRoom: escrita vf0C (0% -> 96.1%); con ella pasan al 100% las copias debiles MapPartsBase::vf24 (0x8042488C) y RotaryEntry_unk::RotaryEntry_unk (0x804248E0), y vf40/vf6C/__sinit suben a 99.93-99.95% (orden de .sdata2 pendiente de fn_80424AD4/fn_80424FB4/fn_80425498). 9/18. Tipos locales nuevos (RotaryLocator_unk, RotaryBox_unk, AABB_unk local como en otras TUs).
- gmk/DropItem::vf60 (copia debil en Cannonball): 10 variantes del inline (bool/int, return directo, IsOut_unk reescrito) sin acertar el orden r4/r5; se deja.
- gmk/Chain: fn_8043B9E0 usa push_back en bucle (bloqueo documentado). Escritas fn_8043C508 (90.6%) y fn_8043CA58 (98.8%; con ella la copia debil ChainJoint_unk::ChainJoint_unk 0x8043C334 pasa al 100%). 8/13. include/gmk/Chain.h: m314/m320 Vector3 (extremos de la cadena). db/PaletteTable_unk fn_8020BB0C: 5 variantes de las locales sin usar, sin mejora.
- gmk/RandomItem: escrita fn_80435CBC (funcion libre llamada desde GameScene: `m11C == 2 && m168 != 0`) -> 100%. 13/19. gmk/CharaPathMove fn_8043D6E4/fn_8043D930: copias debiles de std::vector dentro de vf20 (presupuesto de inline ya investigado); saltadas.
- gmk/PigeonManager (2a pasada): escritas las 6 funciones de Pigeon_unk que faltaban: fn_804048CC (99.93%), fn_80404E70 (99.73%), fn_804050C0 (99.78%), fn_80405A14 (98.7%), fn_804052EC (97.3%), fn_804058A4 (91.5%); el codigo de fn_804048CC, fn_80404E70 y fn_804050C0 ya coincide (cmp OK), solo falta el orden de .sdata2. FindNear_unk reescrito (copia local `t = mTmp = ...` y bandera `same`): vf0C 88.71% -> 93.89%. vf10 99.93%. symbols.txt: fn_80405A14__Q23gmk10Pigeon_unkFv -> fn_80405A14__Q23gmk10Pigeon_unkFP9CharaTask (recibe el jugador en r4). Cabecera include/gmk/PigeonManager.h (solo la usa esta unidad).
- gmk/ChickenManager (2a pasada): escritas las 6 funciones de Chicken_unk (gemelas de Pigeon_unk): fn_8040B668 99.91%, fn_8040BC38 99.73%, fn_8040BE88 99.83%, fn_8040CD2C 99.61% (codigo igual, solo orden de .sdata2), fn_8040C058 97.3%, fn_8040C610 97.7% (huecos de pila). El ctor pasa al 100% (orden de .sdata2 ya correcto). FindNear_unk como en Pigeon: vf0C 88.71% -> 93.89%. 7/15. symbols.txt: fn_8040CD2C__Q23gmk11Chicken_unkFv -> fn_8040CD2C__Q23gmk11Chicken_unkFP9CharaTask. Pigeon fn_80405A14: con Atan2_unk su codigo ya coincide (98.73%, solo .sdata2/.rodata).
- gmk/RandomItem (2a pasada): escritas fn_8043465C (100%; `(m11C == 2 && m168 != 0) || m11C == 3` en una sola condicion da la forma de saltos), fn_80435684 (100%) y fn_80435908 (93.7%, marco y registros). 15/19; queda fn_804348C4 (3520 B) sin escribir.

## Hallazgos MWCC
- Pasar `(int)x` (x unsigned) a un inline en una segunda llamada impide que MWCC comparta las subexpresiones del parametro entre las dos copias inlineadas (TreasureBoxTable).
- Una funcion inline que solo devuelve una constante (p.ej. `return 0.1f;`) definida antes del codigo que la usa mete esa constante antes en .sdata2 (el pool sigue el orden de aparicion en el texto, incluidas las inline): sirve para reordenar constantes (BulletPoint, -98.1f/-0.5f).
- Una copia `int len = size` (unsigned long) antes de pasarla a un constructor inline cambia la asignacion de registros guardados del resto de la funcion (ChaseParam).

- Un indice `int` usado como `mData[(unsigned int)i]` (o comparado como `(unsigned int)i < size`) hace que MWCC incremente el contador antes que el desplazamiento del elemento y cambia la numeracion de los registros del bucle: arreglo EventIdTable::fn_8020F75C, ItemRare fn_80213B78 y FlyObjManager::vf44.
- Una local para un valor leido de memoria (`int id = desc[i]; if (id == chara->m50)`) invierte el orden de operandos de cmpw respecto a la expresion directa (MeetingSpot).
- Dos copias inline de la misma busqueda con los parametros en distinto orden reproducen la numeracion de registros intercambiada entre dos llamadores (ItemRare).

- `if (index >= 0 && 5 > index)` (constante a la izquierda) evita que MWCC funda el rango en una sola comparacion sin signo (cmplwi); con `index < 5` si la funde (CharaLight).
- Escribir `a[i][j].campo = ...` en cada linea en vez de tomar `T& r = a[i][j]` cambia el orden de suma de las dos partes de la direccion (Ocean/FallingTower).
- Una lectura del contador mediante una funcion miembro inline (`GetCount_unk()`) en la condicion del bucle hace que MWCC recargue el puntero global en cada vuelta (Brazier).

- `MIN(MAX(x, 0), 1)` con macros (valor recalculado) reproduce la doble division/comparacion del original; el orden de los argumentos del MAX importa (WaitObject::vf10).
- Reutilizar una variable ya viva (`active`) como contador de un bucle posterior en vez de declarar `int i` cambia la numeracion del registro del contador (PigeonManager::vf10).
- `x += k * (float)(rand() % n)` sale como fmadds; con una local `float d = k * ...; x = x + d;` sale fmuls + fadds como en el original (Pigeon_unk).
- Un `cos(double)` en tiempo de ejecucion con una constante double en .rodata (no plegado) sale de una `static const double` pasada a cos (Pigeon_unk::fn_804052EC).
- `atan2(v.x, v.z)` directo carga z (f2) antes que x (f1); a traves de un inline `float Atan2_unk(float y, float x) { return atan2(y, x); }` carga x primero, como el original (Pigeon_unk/Chicken_unk).
- En un helper inline, declarar como locales los vectores/color pasados por referencia (en lugar de temporales en la llamada) da el orden de pila del original (OpenBoard/WaitObject).

## Decisiones tomadas sin el usuario
- Funciones a 0% (Banana::vf40, BulletPoint::vf10, OpenBoard::vf10, PassBoard::vf10) escritas desde cero aunque no lleguen al 100%: suben el fuzzy y dejan el codigo en el arbol.
- Corregida la constante de mezcla de OpenBoard::vf28 (0.1f; antes 1.0f) al ver que el pool .sdata2 del original la tiene.
- Las comprobaciones de build completo/regress/fuzzdrop se hacen una vez por tanda de unidades independientes y luego se hace un commit por unidad (para no repetir el build completo por cada unidad en una maquina compartida).
