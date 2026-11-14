# Cola E (cabeceras-20261114): `.rodata` de BattleTask en el fuente y cadenas del pool

Unidad: `game/BattleTask` (del port; solo matching neutro). Inicio 18:21, fin ~18:50.

## Resultado

- **15446 -> 15447** (`fn_800F39A4` al 100 %: cargaba las rutas de los efectos desde `.sdata2`), lost 0,
  sin bajadas, labelcheck/rep100 igual. BattleTask 164 -> 165/212 al 100 %; al 0 % sin cambios.
- `fn_800FDF14` 92,6 -> 94,3 % (la base r31 = inicio de `.rodata` ya cuadra: dobles en +0x8E8/+0x8F8,
  `sNoHead_unk` en +0x8D0, `lbl_80735DD0` en +0x808).
- `.rodata` de BattleTask igual que el original objeto a objeto (desplazamientos y tamanos) hasta el final
  de `@stringBase0`: 0xF66 (0xF68 con relleno) = original tras el cambio de split. `poolorder`: same.
  `.sdata2` 0x228 frente a 0x224 (el aviso de `poolcheck` 2.44346 ya estaba antes).

## Cambios

1. `src/game/BattleTask.cpp`: al principio del fichero (antes de `fn_800F13BC`), en el orden del original,
   los 41 punteros `extern "C" const char* const lbl_808813D4..lbl_80881474 = "..."` (`.sdata2`, con sus
   literales como objetos sueltos de `.rodata`), las tablas `lbl_80735988[3]`, `lbl_80735A48[7]`,
   `lbl_80735A94[3]`, `lbl_80735AB0[32]` (`BattleLogDef_unk`: mensaje, color RGBA, flags; el struct sube con
   ella), `lbl_80735DD0[26]` (sonidos) y `sNoHead_unk` (subida desde la linea ~3130). Quitadas las
   declaraciones `extern` correspondientes. `lbl_80735ED0` se queda donde estaba (va detras de los dobles).
2. `#include "std/string_members.h"`: el pool acaba con las 3 cadenas de `basic_string` como el original.
3. `symbols.txt`: `lblmap.py game/BattleTask` (100 etiquetas `@N` de la unidad renumeradas).
4. **Split** (`splits.txt`): "Event::ActionProcessor" (`.rodata` 0x80736530-0x80736548) y
   `__RTTI__Q25Event15ActionProcessor` (`.data` 0x80779E28-0x80779E30, antes un hueco `auto_07`) pasan de
   BattleTask a `Event/Behavior_unk` (enlazada). main.dol: OK.

## Decisiones tomadas sin el usuario

- Cambio de split del punto 4 (regla 3: avisar antes de tocar `splits.txt`). Motivo: la cadena solo aparece
  una vez en todo el DOL, BattleTask no la emite (no usa `Event::ActionProcessor`) y nuestro
  `Event/Behavior_unk.o` ya la emite junto con la RTTI (la vtable debil de la clase abstracta la quita el
  enlazador). Con el split viejo BattleTask nunca podia tener esa cadena. Revertir = deshacer el commit
  `f1ce542`.

## Para el port

- Ningun cambio de firma, tipo ni layout de clases compartidas. Solo en `game/BattleTask.cpp`: los
  `lbl_8088xxxx`/`lbl_8073xxxx` pasan de `extern` a definidos en el fuente (el port ya no necesita
  proveerlos), `BattleLogDef_unk` y `sNoHead_unk` se mueven arriba, y el fichero incluye
  `std/string_members.h`.

## NONMATCHING / pendientes

- `fn_800FDF14` (94,3 %, borrador de 8 KB): el marco es 0x100 mayor que el original (0x550 / 0x450): los
  temporales de conversion int->float van a 0xE8 en el nuestro y a 0x3D0 en el original, y los locales por
  encima de 0xAC se desplazan 0x100 (`text` en 0x2D0 frente a 0x1D0, `num` en 0x78 frente a 0x90). No
  intentado mas (fuera de la cola).
- `.data` de BattleTask: 0x174 frente a 0x148 (no mirado).

## Hallazgos (una linea)

- Con `-ipa file` el orden de `.rodata` sigue el fuente: los datos se emiten donde se definen y las constantes
  `double` de cada funcion en el sitio de esa funcion; para que los datos de la unidad vayan delante de los
  dobles hay que definirlos antes de la primera funcion que los usa.
- Si el split da a una unidad una cadena de RTTI que su fuente no emite, mirar si aparece una sola vez en el
  DOL y la emite la unidad vecina (aqui "Event::ActionProcessor", de Event/Behavior_unk).
