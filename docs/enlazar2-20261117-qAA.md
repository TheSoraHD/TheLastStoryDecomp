# Cola AA (enlazar2-20261117): game/GameEventScript al 99,5 %

Inicio: 2026-11-18 00:36 CET. Cierre anticipado a peticion del usuario (~01:20).

| unidad | funciones al 100 % antes -> despues | enlaza |
|---|---|---|
| game/GameEventScript | 123 -> 132 / 224 | no |

## Hecho
- Pool `@stringBase0`: los 7 `""` del original tras formatos de `sprintf` son el `\0` explicito del formato
  (`"%d,%s,%d,...%d -> %d,\0"`, ScriptError de ExecComAimGimmick, `"Event not found! ev%04d\0"`, `"START Page%d\0"`,
  `"END\0"`, los dos `%d,...,%s,\0`/`%d,None,...\0`; `"Page%d -> %d"` y `"COM:%s"` sin `\0`). Pool identico al
  original; +8 funciones (fn_803A1B20, 064C, 24A4, AA5E4, 47A4, 4568, InsertNode_unk x2...).
- fn_8039F9A4: `float rate;` declarado tras `int value` y `rate = chara->m568; value = 100.0f * rate;` en el case 1
  (orden de los operandos del fmuls). Renumera +1 los `@N` de la unidad: re-emparejado exacto por (seccion,
  desplazamiento) entre el .o anterior y el nuevo (script en scratch, `remap.py`; 64 nombres en symbols.txt).

## NONMATCHING / bloqueos
- Familia SetNext_unk (21 comandos con `lwz r0` tipo / `li r4` espera en el original): ~25 formas mas sin exito
  (inlines anidadas Set/NextOf/CmdSize, tabla `const`, `int*`, `unsigned`, `args` en vez de `cmd`, `int wait = 0`
  arriba, ctor + asignacion). Solo dos estados: registros bien + `li` antes del `lis` (96,6, forma escrita a mano),
  o `lis` bien + registros cruzados (forma actual). Sin idea nueva.
- fn_803A2378/2210/7D94/98EC: el `stw` de mWait (espera variable) sale 2 instrucciones antes; sin probar.

## Hallazgos
- Re-emparejado exacto tras una renumeracion de `@N` dentro de una unidad: objdump -t del .o antes y despues,
  mismo (seccion, desplazamiento) -> renombrar en symbols.txt solo dentro de los rangos de la unidad en splits.txt;
  pd.sh/lblfix/lblseq dejaban 112-126 de 131.
- Un local float con nombre (aunque se declare arriba) en vez de la expresion consume un `@N` en MWCC.

## Decisiones tomadas sin el usuario
- Commits con PRECOMMIT FAIL solo por el ordercheck previo de GameEventScript (igual en la referencia).

## Para el port / Cambios de splits/symbols
- symbols.txt: solo renombrados `@N` de GameEventScript (renumeracion +1).
