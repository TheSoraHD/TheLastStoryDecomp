# escribir-20261116 cola W3b (Event, segunda pasada)

Inicio: 2026-11-17 04:16 CET. Tope: 06:46.

| funcion | antes | despues |
|---|---|---|
| Sequence 0x80544650 `__vec_event_unk<Actor>::__grow_by_unk` | 99,90 % | **100 %** |
| Sequence 0x80544748 `__vec_event_unk<Path>::__grow_by_unk` | 99,90 % | **100 %** |
| Sequence 0x80545B90 `__vec_event_unk<Path>::__reserve_unk` | 99,86 % | **100 %** |
| Sequence 0x80545C3C `__vec_event_unk<Actor>::__reserve_unk` | 99,86 % | **100 %** |
| EventParser_unk ParseXXID (MOID/CAID/MVID/STID 99,92; ANID/EFID/POID 99,77; MDID 99,6; DUID 99,38) | idem | sin cambio (ver Bloqueos) |
| EventParser_unk 0x8057DC38 `construct<Path>` | 97,61 % | sin cambio (registros de la copia inline de Path) |
| Manager `vector<Curve_unk>::size` / `__vec_storage_unk<Curve_unk>::clear` | 0 % | sin cambio (sin idea nueva que funcione) |
| Manager 0x80490098 `__insert_realloc_unk<SceneSettingEntry_unk>` | 99,22 % | sin cambio |
| Sequence 0x805455CC `__vec_event_unk<Path>::__push_front_unk` | 95,06 % | 95,16 % (sin cambio de fuente: registros de la copia de Path) |
| Sequence 0x80542684 `construct<EffectEntry24_unk>` | 93,64 % | 93,72 % (sin cambio de fuente; ver Bloqueos: copia de vector de palabras) |
| ActionEffectPlay `ActionCheck::vf08` | 94,05 % | sin cambio |
| Manager 0x8048871C (fuera de la cola, misma familia) | 98,61 % | 99,70 % (orden de la suma de m1F58; quedan r23/r24) |
| Dialog 0x802292AC (fuera de la cola, misma familia) | 90,99 % | 91,79 % (orden del `.rodata`; planificacion de floats) |
| Node 0x8053A168 `MslMedian3_unk<MslIter_unk<ActionKey_unk>>` (familia) | 56,17 % | **100 %** |
| Node 0x805393F8 `Action::fn_805393F8` | 96,93 % | 99,85 % (registros de mSize/mCap del push_back y huecos de la realloc) |
| Node 0x80539870 / 0x80539CEC `MslSort_unk<...ActionKey_unk...>` | 78,97 % | 94,86 / 94,65 % (first/last en registros: cabecera game/MslSort_unk.h, no tocada) |
| Node 0x805377F0 `Action::fn_805377F0` | 94,52 % | 97,09 % |

Funciones al 100 %: 5 (4 de Sequence + MslMedian3 de Node). Unidades que enlazan: ninguna nueva.

## Para el port
- Sequence.cpp: instanciacion explicita `template std::string::basic_string(const std::string&, size_type, size_type);`
  entre fn_8053F818 y fn_8054011C (fuera de `namespace Event`). Sin cambio de comportamiento.
- Dialog.cpp: `sSound` declarado antes que `sPos` (orden del `.rodata`) y el bucle de `ResetPos_unk` escribe `y`
  antes que `x`. Sin cambio de comportamiento.
- Manager.cpp `fn_8048871C`: `m1F58 = action->m10 + frames + m1F64` (antes `action->m10 + (m1F64 + frames)`), mismo
  valor.
- Pendiente (no commiteado, baja 0,07 % ActionCameraMode::vf0C por el alineado de objdiff): la firma real de
  `EventCamera_unk::fn_80112918` es `(int, int, int)` (el llamador pasa los dos primeros sin `extsh`; la funcion
  hace `sth`): con `int` desaparecen los dos `extsh` del original.

- include/Event/Action.h: `Event::ActionKey_unk` tiene ctor por defecto vacio (`inline ActionKey_unk();`, definido en
  Event/Node.cpp y Event/EventParser_unk.cpp): deja de ser POD (las copias del sort van miembro a miembro). Sin
  cambio de comportamiento; para el port basta con un ctor vacio en la cabecera.
- Para quien enlace Event/Sequence: la instanciacion explicita marca la copia debil del ctor de subcadena como
  "force active" (trucos-mwcc, enlazar-20261115): comprobar que el enlazador no la deja en el .dol.

## Decisiones tomadas sin el usuario
- ctor de `ActionKey_unk` declarado `inline` en la cabecera y definido en los dos .cpp que construyen claves (como
  `Sequence::FindCutByIndex_unk`): con el cuerpo en Event/Action.h se renumeran los `@N` de 15 unidades de Event
  (incluidas las Behavior* enlazadas).
- Dialog `ResetPos_unk`: `y` antes que `x` (las dos formas son equivalentes; con `x` primero el cambio de orden del
  `.rodata` bajaba fn_802292AC de 90,99 a 89,60 por la planificacion; con `y` primero sube a 91,79).

## Hallazgos
- Un mensaje de `basic_string` que sale en el pool del original antes de lo que da la instanciacion implicita (al
  final de la unidad) se coloca con una instanciacion explicita del miembro que lo usa en ese punto del fuente
  (`template std::string::basic_string(const std::string&, size_type, size_type);`, como en atn/FileList y
  D2A_Manager): Event/Sequence "out_of_range" antes de "vector length error" (escribir-20261116 qW3b).
- Las cadenas de una funcion no estatica y no inline definida antes de la que la autoinlinea van al pool antes que las
  de esta (se compila primero); con `static` o `inline`, o definida despues, van en el orden del codigo del
  llamador (prueba suelta y ActionActorPosition fn_80557F84, confirma qR de mixto) (escribir-20261116 qW3b).
- Copia de un vector de palabras (memmove) en el original: la cuenta del memmove se recalcula entera (subf, srawi,
  addze, slwi) y `mSize += last - first` reutiliza la resta del `n` de `__init_unk` pero rehace la division; con
  `memmove` por referencias y la suma por una inline con los punteros por valor (`__dist_unk(first, last)`) el
  memmove ya sale como en el original (IntVector_unk/EquipList_unk 88,7 -> 95,4) pero la suma reutiliza `n` entero
  (ver Bloqueos) (escribir-20261116 qW3b).
- En `__push_front_resize_unk` generico, un nivel inline mas en la construccion (`__construct_unk(p, *last)`) pone el
  par de temporales `char` de la copia de string del movimiento en el orden del original (fuente arriba, destino
  abajo) en ParseMOID y familia; solo quedan los dos pares de huecos intercambiados (escribir-20261116 qW3b).
- Etiquetas `lbl_` del propio split que `pairdata` no empareja por ambiguedad (`lbl_X: ambiguous [...]`): renombrar a
  mano por direccion (`pdfix.py`) tras comparar valores y desplazamientos (`/home/claude/scratch-W3b/bin/sd2.py`)
  (escribir-20261116 qW3b).

- Un ctor por defecto vacio en un struct de floats (sin tocar la copia) hace que MWCC copie los temporales del swap
  del sort miembro a miembro con `lfs`/`stfs` y que push_back siga por el camino POD (copia trivial): Event/Node
  `MslMedian3_unk<ActionKey_unk>` 56 -> 100 %; con el ctor de copia explicito ademas, push_back toma el camino no POD
  y baja (escribir-20261116 qW3b).
- Un ctor (aunque vacio y sin parametros) definido en una cabecera renumera los `@N` de todas las unidades que la
  incluyen: declararlo `inline` alli y definirlo en los .cpp que lo usan (escribir-20261116 qW3b).

## Bloqueos
- Event/Sequence `construct<EffectEntry24_unk>` 0x80542684, `__ct__IntVector_unk`/`__ct__EquipList_unk` (88,7), y
  las 13 copias de vectores de palabras del DOL (todas con la misma forma): `mSize +=` tiene que compartir la resta
  con `n` pero no la division. Probado (prueba suelta, `/home/claude/scratch-W3b/t/`): `n` int/unsigned/char/`/4`,
  `/(int)sizeof`, `/(long)`, conversiones, `distance` con etiqueta, `n` por referencia a inlines, suma dentro y fuera de
  `__construct_at_end_unk`, firmas por valor/referencia: o se reutiliza `n` entero o se recalcula todo.
- EventParser_unk ParseXXID: con el nivel inline mas en `__push_front_resize_unk` (Hallazgos) quedan los huecos
  {0x10,0x14} y {0x18,0x1C} de las copias de append y del movimiento intercambiados; la profundidad de la rama de
  append (-1, +2 niveles) y el orden de definicion de los miembros de `__vec_split_unk` no los mueven.
- Manager fn_8048871C / fn_80488B5C: r23/r24 de frames y del corte en `FramesTo_unk`: probados orden de
  declaraciones, `for`, `frames = frames + ...`, inline con el corte como argumento, cuerpo de `FindCutByIndex_unk`
  de Sequence.cpp: nada.
- ActionMusicPlay::vf0C 89,75: el original carga el volumen (lfs) antes que m18/m1C porque el puntero del parametro 1
  usa r5; probados orden de locales, `GameBgm*` local, puntero al parametro: 88,75-93 sin llegar.
- Puppeteer fn_80011E98: con `if (m20 == 7 || m20 == 1)` fuera y el personaje por ternario se arregla el salto (el
  original va al final si el tipo no es 7/1) pero el `m1C` se CSE con el del switch de fn_8001296C (el original lo
  recarga): 92,5; probados `fn_800128FC()` dentro del if y tres cuerpos de fn_800128FC (baja la propia).
- Puppeteer fn_8001122C 58,8: copia de la matriz: el `lwz m1C` se adelanta a las cargas; probados `SetMtx_unk` por
  valor, `m58 = mtx` con copia local en el llamador: igual.
- ActionActorPosition fn_80557F84: el pool sale identico con IsEarring/IsWaist/IsBust no estaticas + string_members.h
  + instanciacion explicita del ctor de subcadena tras la funcion (99,36 -> 99,39), pero sigue el recorrido de los
  huesos por `&model->m384` (FixedArray, ver mixto qR): no commiteado (solo fuzzy).

- Movimiento de los elementos viejos en la realloc de push_back de elementos POD/sin destructor (Puppeteer
  `push_back<ColorF_unk>` 95,6, Node fn_805393F8, Sequence fn_8054204C): el original convierte `while (last > first)`
  en bucle contado (`mtctr`/`bdnz`, cuenta `(last - first + size - 1) / size`) y decrementa `last` antes de las
  cargas; el nuestro deja `cmplw/bgt` y pliega el decremento en los desplazamientos. Mandar los POD por
  `new (--p) T(*--last)` no lo cambia (ColorF ya va por ahi) y baja fn_805393F8: revertido.
- Sequence fn_8054136C 99,17: con `Cut* target = cut;` en fn_80541198 (que sigue al 100 %) la rama de etiqueta sale
  exacta (99,50) pero la de corte sigue con r5/r6 cruzados; no commiteado (solo fuzzy, local de copia).
- DebugInfo fn_802219CC 88,9: el original evalua `c_str()` antes de los floats; con un local para el texto o la
  llamada directa a `fn_800616C0` (sin `fn_80221CAC`) sale igual.
- Node fn_8053A2E8 89,3: el original recarga `mKeys.mData` en la comparacion del bucle y usa la carga inicial para la
  interpolacion; con la comparacion por una inline `KeyTime_unk(keys, i)` 90,26 (no commiteado).

## Cambios de splits/symbols
- symbols.txt: `@N` de Sequence re-emparejados (pd + lblmap); Dialog `lbl_80742E48`/`lbl_80742E88`/`lbl_80742E98` ->
  `@10570`/`@12615`/`@11041` (scope:local); Manager `lbl_80886FBC..FD4` y el segundo `@42245` (0x80886FE0) ->
  `@42237..@42243`, `@42246`. Sin cambios de splits.

## Scripts (en /home/claude/scratch-W3b/bin/)
- `poolrefs.py <obj.o>`: desplazamientos de `@stringBase0` que usa cada funcion (aproximado).
- `oside.py <unidad> <simbolo> [desde hasta]`: las dos columnas de objdiff enteras.
- `sd2.py <unidad> [desde hasta]`: simbolos de `.sdata2` (desplazamiento, nombre, valor) del original y nuestros.
- `lblsuggest.py <prefijo>`: `lbl_` de datos del split con el mismo objeto (tamano y bytes) en nuestro .o.
- `scan.py <prefijo>`: semdiff (saltos con destino distinto, bloques) de las funciones < 100 % de las unidades.
- `pool.sh`, `slots.sh`, `t.sh`: compilacion suelta con try.sh (pool, huecos de pila, asm de una funcion).
