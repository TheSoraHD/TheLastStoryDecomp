# Cola S (port-decomp-20261114): tercera pasada del HUD

Inicio: 2026-11-14 11:03 CET. Rama `agent-S`. Unidades: `game/GameUI`, `game/PopupTask`.

## Resultado por funcion

| Funcion | Antes | Despues | Firma final |
|---|---|---|---|
| `GameUI::fn_803E0E10` | 85,05 % | 85,08 % (NONMATCHING) | `static void GameUI::fn_803E0E10(CharaTask*, GameUIElem60_unk*, int index)` |
| `GameUI::fn_803D83A8` | 88,79 % | sin cambios (88,8 %) | `void GameUI::fn_803D83A8()` |
| `GameUI::fn_803E2110` | 35,8 % | sin cambios | `void GameUI::fn_803E2110(int index, CharaTask*)` |
| `GameUI::fn_803DAB4C` | 96,3 % | sin cambios | `void GameUI::fn_803DAB4C(int show, int keep)` |
| `PopupTask::vf14` | 97,3 % | sin cambios | `void PopupTask::vf14()` |

- `fn_803E0E10`: **es estatica**: el original recibe el personaje en r3, la ventana en r4 y el indice en r5
  (los dos llamadores, 0x803D3E8C y 0x803D4330, cargan r3/r4/r5 sin `this`). Equivalencia del canto
  (pregunta de qI): el original calcula `mFB8 / mFBC` y el `fcmpo` del minimo con 1.0 y luego no usa f3 (se
  sobrescribe en 0x143C): **no falta ningun panel**, el valor esta muerto tambien en el original.
- `fn_803E0E10`, lo que queda (~15 %): el original pasa `base` a `fn_801F6E78` **por direccion** (`addi r5, r1,
  0x5c` en las ocho llamadas, sin copia); nuestro `atn/D2A_TaskInstanceC.h` lo declara por valor, y MWCC copia
  el rectangulo en cada llamada (marco 0x180 frente a 0xE0). Con la declaracion `const atn::D2A_DrawRect&` en la
  cabecera la funcion sube a **93,5 %**, pero la definicion de `atn/D2A_TaskInstanceBase_64_8.cpp` (por valor,
  al 100 %) deja de poder ser la misma funcion: por referencia baja a 99,28 % (un `lfs` adelantado, el
  original no puede suponer que el rectangulo no tiene alias), y una segunda declaracion `extern "C"` (con
  referencia, puntero o con `atn::Task*` como primer argumento) da "illegal function overloading". Se deja por
  valor. Ademas: el rectangulo UV de la cara se copia con paired singles (un tipo UV con copia psq, como
  `ShopUV_unk`; el `fn_801F7590` de la cabecera recibe `atn::Color`, cuya copia es miembro a miembro),
  `--lives` da `ble`/`b` en el original (`lives = --lives > 0 ? lives : 0` baja a 93,0 con la cabecera
  cambiada) y `m560 == 29 || m560 == 30` sigue en rango.
- `fn_803D83A8`: revisada; diferencias de registros y una recarga de `lbl_8087F9F8` tras el bucle en el original
  (equivalente). El desfase de 0x14 del pool sigue siendo el `push_back` de `std::vector<wstring>`.
- `fn_803DAB4C`, `PopupTask::vf14`: revisadas, equivalentes (orden de stores de temporales, constantes
  izadas a registros: `lbl_80887D9C`/`lbl_80887DAC` son floats del propio pool sin emparejar, poolcheck limpio).
- `fn_803E2110`: sin tocar. Para emparejarla `m30C` tiene que ser un `std::vector<Entry>` de `std/vector.h`
  (el original inlinea su `push_back` con los dos `fprintf("vector length error")`/`abort`); eso cambia
  `GameUIList_unk` (constructor, destructor, `Clear_unk` 0x803CF78C) y el ctor/dtor de GameUI: no cabia en la cola.

## Cambios de firma o de layout (para el port)

- `GameUI.h`: `fn_803E0E10` pasa a `static` (sin `this`; el nombre mangled no cambia). El port debe llamarla
  como funcion estatica (no usa ningun miembro de GameUI).
- Pendiente (no hecho): `fn_801F6E78` deberia recibir el rectangulo por `const atn::D2A_DrawRect&` en el lado
  de los llamadores (GameUI); en el port el cambio es neutro.

## Decisiones tomadas sin el usuario

- Quitar el `this` implicito renumera (-1) todas las `@N` de GameUI; `pd.sh` dejaba 5 de `.sdata2` mal: se
  renombraron en `symbols.txt` aplicando el mapa nombre viejo -> nuevo por desplazamiento de seccion entre el
  `.o` anterior y el nuevo (solo renombrados, 115 lineas; labelcheck igual que la referencia).

## Hallazgos

- El `this` implicito de un metodo consume un `@N`: hacer `static` un metodo renumera las etiquetas de toda la
  unidad (como un parametro con nombre en una declaracion).
- MWCC no admite dos declaraciones `extern "C"` del mismo nombre en una unidad aunque difieran en el tipo de un
  parametro ("illegal function overloading"; T frente a `const T&` tampoco).
