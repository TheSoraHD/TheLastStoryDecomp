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

## Hallazgos MWCC
- Pasar `(int)x` (x unsigned) a un inline en una segunda llamada impide que MWCC comparta las subexpresiones del parametro entre las dos copias inlineadas (TreasureBoxTable).
- Una funcion inline que solo devuelve una constante (p.ej. `return 0.1f;`) definida antes del codigo que la usa mete esa constante antes en .sdata2 (el pool sigue el orden de aparicion en el texto, incluidas las inline): sirve para reordenar constantes (BulletPoint, -98.1f/-0.5f).
- Una copia `int len = size` (unsigned long) antes de pasarla a un constructor inline cambia la asignacion de registros guardados del resto de la funcion (ChaseParam).

- Un indice `int` usado como `mData[(unsigned int)i]` (o comparado como `(unsigned int)i < size`) hace que MWCC incremente el contador antes que el desplazamiento del elemento y cambia la numeracion de los registros del bucle: arreglo EventIdTable::fn_8020F75C, ItemRare fn_80213B78 y FlyObjManager::vf44.
- Una local para un valor leido de memoria (`int id = desc[i]; if (id == chara->m50)`) invierte el orden de operandos de cmpw respecto a la expresion directa (MeetingSpot).
- Dos copias inline de la misma busqueda con los parametros en distinto orden reproducen la numeracion de registros intercambiada entre dos llamadores (ItemRare).

## Decisiones tomadas sin el usuario
- Funciones a 0% (Banana::vf40, BulletPoint::vf10, OpenBoard::vf10, PassBoard::vf10) escritas desde cero aunque no lleguen al 100%: suben el fuzzy y dejan el codigo en el arbol.
- Corregida la constante de mezcla de OpenBoard::vf28 (0.1f; antes 1.0f) al ver que el pool .sdata2 del original la tiene.
- Las comprobaciones de build completo/regress/fuzzdrop se hacen una vez por tanda de unidades independientes y luego se hace un commit por unidad (para no repetir el build completo por cada unidad en una maquina compartida).
