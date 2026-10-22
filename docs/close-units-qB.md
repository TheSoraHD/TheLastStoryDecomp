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

## Hallazgos MWCC
- Pasar `(int)x` (x unsigned) a un inline en una segunda llamada impide que MWCC comparta las subexpresiones del parametro entre las dos copias inlineadas (TreasureBoxTable).
- Una funcion inline que solo devuelve una constante (p.ej. `return 0.1f;`) definida antes del codigo que la usa mete esa constante antes en .sdata2 (el pool sigue el orden de aparicion en el texto, incluidas las inline): sirve para reordenar constantes (BulletPoint, -98.1f/-0.5f).
- Una copia `int len = size` (unsigned long) antes de pasarla a un constructor inline cambia la asignacion de registros guardados del resto de la funcion (ChaseParam).

## Decisiones tomadas sin el usuario
- Las comprobaciones de build completo/regress/fuzzdrop se hacen una vez por tanda de unidades independientes y luego se hace un commit por unidad (para no repetir el build completo por cada unidad en una maquina compartida).
