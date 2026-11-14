# Sesion port-decomp-20261114, cola L (push_back de std/vector.h; TacView, Event/Sequence, EventParser_unk, Manager)

Inicio: 2026-11-14 07:27 CET. Tope ~09:57. Commits: 217bf0c, 081c82b, 7fd6bc1, c8bef05, d6418b5 y el de notas.

## push_back de `std/vector.h` (lote de cabecera): sin cambio commiteable

Forma probada (la del original segun 0x8054204C/0x80542684/0x805428A8): `push_back` pequena
`if (size < cap) { alloc.construct(data + size, x); ++size; } else __push_back_realloc_unk(x);`, con el
asignador en la direccion de `mCap` (0x8054204C pasa `this + 0x18C` = `&m184.mCap` a la construccion) y la
reallocacion = el camino lento actual. Con esa forma las copias debiles salen del tamano del original
(construccion de `EffectEntry24_unk` 0x228 = 552 B como 0x80542684; reallocacion 0x7C4 frente a 0x7B8), pero
**MWCC no las saca de linea**: `push_back<EffectEntry24_unk>` acepta sus dos candidatas (R3) y queda grande,
y entonces 0x8054204C rechaza todo (llama a los dos push_back, a `Set_unk`...). El original las rechaza.
Medido en Sequence.o (scripts en `/home/claude/scratch-L/tmp/pad*.sh`, variantes `vector.h.v3..v7`):
- `construct` relleno con asserts vacios: `push_back<Entry>` sigue aceptando con 300 sentencias de mas y
  rechaza con 600; `push_back<Actor>` rechaza ya con 150. El original rechaza sin relleno: nuestras
  candidatas cuentan mucho menos (o nuestro presupuesto es mayor) que en el original.
- Relleno en `__push_back_realloc_unk`: con 7-8 sentencias de mas pasa de 13 y deja de ser candidata de
  autoinline (siempre llamada), tambien para `Vector3` (el original la inlinea en 0x8054204C).
- Sin efecto: accesores `size()/capacity()/end()` delante, rama POD eliminada (solo la forma nueva),
  `push_back` o la reallocacion declaradas `inline` (la reallocacion `inline` ademas lleva su
  "vector length error" a un `@STRING@` propio), definir la reallocacion antes de `push_back`, instanciacion
  explicita de la reallocacion antes de 0x8054204C.
- Conclusion: es el mismo caso abierto que `LoadSeTableTask::vf0C` de `docs/inlining.md` (el original
  rechaza candidatas que nosotros aceptamos). Sin entender que hace pesar mas las candidatas del original no
  hay forma que no baje nada. Las funciones sin escribir de Sequence (0x8054372C-0x80545C3C), EventParser_unk
  (fn_8057DB14/DC38/E274) y Manager (fn_80482400/8049003C/80490098) son todas copias debiles de esa cadena
  (construccion/reallocacion de `Actor` y `Path`, con sus `__grow_cap`, `__vec_set_buf`, `__append_n`,
  `__push_front` y destructor del buffer fuera de linea): siguen sin escribir.

## Resultado por funcion

- `TacView::fn_805AC758` (0x805AC758): 90,62 -> **99,20 %**. Firma sin cambios `void TacView::fn_805AC758()`.
  - Cabecera nueva `include/std/vector_wstring.h` (solo la incluye TacView.cpp): `vector<wstring>` mueve los
    elementos con memcpy + memset al crecer, como `vector<string>` (`std/vector_string.h`), con la diferencia
    en bytes `((char*)last - (char*)first) / 12` para que `(data + size) - data` no se pliegue. Ponerla en
    `vector_string.h` renumeraba las etiquetas de ~40 unidades que la incluyen (labelcheck peor).
  - Prueba muerta de `lbl_8087F8A0` al principio: `if (lbl_8087F8A0 != 0) { CharaTask* head = ...->mHead; }`.
  - La posicion como temporal directo de `GetScreenPosition(...fn_805AE138())` (el original guarda su
    direccion en r16).
  - El mensaje de ayuda en un `std::basic_string<wchar_t> message = lines[0] + lines[1] + lines[2];` con nombre
    (el temporal intermedio se destruye antes de la llamada a `GameUI::fn_803E4978`).
  - Queda: registros (info del personaje, fundido) y, en el crecimiento de `push_back`, los registros de la
    capacidad nueva y del bloque y los huecos de pila de los dos operandos de max (`__grow_cap_unk` generico).
- `Event::Manager` luces `fn_80485028` 86,85 -> **95,10 %**, `fn_80485964` 87,53 -> **96,13 %**, `fn_804862C4`
  87,21 -> **96,00 %**: la luz va por valor a un inline `PushLight_unk(LightList_unk*, atn::Light)` (el
  original la copia a un temporal antes del `push_back`). Queda el camino lento de `push_back` (capacidad en
  registro salvado y el 1 de `__recommend_unk` guardado antes de la comprobacion: std/vector.h).

- `TacView::fn_805A96D4` (0x805A96D4): 80,57 -> **98,33 %**. Firma sin cambios. El bloque 0x15D8-0x15FB pasa a
  `TacViewAim_unk mAim` (modo, posicion, radio, mA, mB, cursor; mismo layout): el original construye uno en la pila
  (ctor = el del bloque en el ctor de TacView: 0, (0,0,0), -1, 0, 0, cursor 2/0/0) y lo copia. Ayudantes
  `TacViewSetAim_unk(v, mode, pos, radius, a)` y `TacViewSetAimB_unk(..., b)` (mB solo en 0x179A-0x179C: en el
  resto el original deja el 0 del constructor); casos 0x1788/0x1791 antes que 0x179A-0x179C en el switch;
  `(int)...->m02 == 7` (cmpwi). Queda: registros (cmd/extra) y las comparaciones de cmd 8-11 de la rama de IA, que
  el original hace sobre el `cmd - 6` de la primera (con `switch` baja a 95,8 %).
- `Event::Sequence::fn_8053ED4C` (0x8053ED4C): 98,76 -> **100 %** (`fn_80483714(GetFlag7_unk(), m98_15)`: el
  original lee m98 dos veces, una por el getter y otra por el campo de bits). mnemcheck limpio.
- `Event::Sequence::fn_8053E8D0`: 98,89 -> **99,95 %** (`int id = m190;` antes de `fn_8020F130()`; la matriz de
  color por `GetColorMatrix_unk()`/`SetColorMatrix()` de RenderSetting_unk, definidas inline en Sequence.cpp como en
  GameTask.cpp). Queda el registro del lider del grupo.
- `Event::Sequence::fn_8054136C`: 98,85 -> **99,17 %** (`FindCutByIndex_unk` con `Cut* cut = mB8[i].get()`).
  Quedan registros.
- `Event::Sequence::fn_80541CFC`: 87,77 -> **88,25 %** (`m08[i]` en vez de `c_str()[i]`); el original vuelve a
  probar el bit de cadena larga en cada acceso (los accesores de `std/string.h` usan `!is_long() ? a : b` y MWCC
  guarda el booleano): cabecera de todo el proyecto, sin tocar. Mismo motivo en `Manager::fn_80488E00` (93,4 %).
- `Event ParseSEQ2_unk` (0x80576838): 90,47 -> **98,41 %**: `SequenceStateRec_unk` con constructor copia miembro a
  miembro (el original carga los tres campos, los guarda en la secuencia y despues en el temporal muerto). Quedan
  registros.
- Sin escribir (siguen): Sequence 0x8054372C-0x80545C3C (20), EventParser fn_8057DB14/DC38/E274, Manager
  fn_80482400/8049003C/80490098: copias debiles de la cadena de `push_back` (ver arriba).
- Probado sin mejora: `Manager::fn_8047F580` (`FindSequence_unk(const int&)`: baja a 95,6 %), `fn_8047FCC0`
  (registros), `fn_8048871C` (orden de carga de m1F64), `Sequence::fn_8053F1B4` (relectura de m98_11).

## Cambios de firma o de layout (para el port)

- `TacView`: miembros `m15D8/m15DC/m15E8/m15EC/m15ED/m15F0` -> `mAim.mMode/mPos/mRadius/mA/mB/mCursor`
  (`TacViewAim_unk`, mismo layout; ctor `inline` definido en game/TacView.cpp para no renumerar las `@N` de las 10
  unidades que incluyen TacView.h). Renombrados los usos de game/GameUI.cpp (11) y game/GameCamera_unk.cpp (6).
- `Event/EventParser_unk.cpp`: `SequenceStateRec_unk` con ctor por defecto vacio y ctor copia miembro a miembro.

- Ninguno en clases compartidas. Nuevo `include/std/vector_wstring.h` (especializaciones de
  `__vec_split_base_unk`/`__vec_split_unk`/`vector::__swap_out_unk` para `basic_string<wchar_t>`).
- `PushLight_unk` (static inline en Event/Manager.cpp).

## Decisiones tomadas sin el usuario

- No tocar `std/vector.h` (ninguna forma sin bajadas); la especializacion de wstring en cabecera propia.

## Hallazgos (una linea)

- Un inline de cabecera con un literal float (ctor `mRadius(-1.0f)`) renumera las `@N` de cada unidad que la
  incluye: definirlo `inline` en el .cpp que lo usa (declarado en la clase).
- Un bloque de miembros escrito con stores a la pila justo antes de los mismos stores al objeto: el original
  construye un struct local y lo copia; los stores del ctor que quedan son los que pisa una copia paired-single.
- El mismo campo de bits leido dos veces (dos `lwz`) en una llamada: un argumento por getter `(m98 >> 16) & 1` y
  el otro por el campo de bits.
- Constructor copia miembro a miembro de un struct de 4 bytes: carga los campos por separado (lhz/lbz/lbz) en vez
  de una palabra.

- Una especializacion explicita de plantilla de clase (aunque no se use) renumera las `@N` de cada unidad que la ve.
- Copia de un objeto a un temporal justo antes de un `push_back` inline: el objeto pasa por valor a un inline.
- Temporal intermedio de `a + b + c` destruido antes de la llamada: el resultado va a un local con nombre.
