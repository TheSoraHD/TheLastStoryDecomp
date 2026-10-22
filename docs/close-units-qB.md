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

## Hallazgos MWCC
- Pasar `(int)x` (x unsigned) a un inline en una segunda llamada impide que MWCC comparta las subexpresiones del parametro entre las dos copias inlineadas (TreasureBoxTable).
- Una copia `int len = size` (unsigned long) antes de pasarla a un constructor inline cambia la asignacion de registros guardados del resto de la funcion (ChaseParam).

## Decisiones tomadas sin el usuario
- Las comprobaciones de build completo/regress/fuzzdrop se hacen una vez por tanda de unidades independientes y luego se hace un commit por unidad (para no repetir el build completo por cada unidad en una maquina compartida).
