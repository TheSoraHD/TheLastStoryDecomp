# Lote close-units, cola A (atn y std)

Inicio: 2026-10-22 04:21 CEST. Rama agent-A. Nota: la herramienta Projects no estaba disponible en esta sesion;
las reglas se tomaron del prompt del lote y de las notas de docs/.

## Unidades

- `std/char_traits_char` (1/1 al 100%): NO enlaza. Fuente reescrita como especializacion completa
  `basic_ios<char, char_traits<char> >` con destructor fuera de linea (funcion clave): el .o emite exactamente
  el dtor 0x58, la vtable y las dos RTTI (antes salia ademas una funcion auxiliar de 0x38 que no esta en el
  original). Con `matching: true` el .dol falla: MWCC alinea la .rodata del .o (nombres RTTI) a 8 y el split
  empieza en 0x807350A4 (alineado a 4), asi que todo lo posterior se desplaza 4 bytes (0x807350A4 -> 0x807350A8).
  No se puede arreglar sin tocar `atn/WordWrap_unk` (unidad con fuente, su .rodata acaba en 0x807350A4): se queda
  `matching: false`. Solo enlazara fusionada con el fichero anterior (WordWrap_unk + char_traits_char + allocator_char,
  ver batch-auto-10kb-qA2.md).
- `atn/D2A_Frame` (2/3): sin cambios. `vf18` sigue 99.85% tras 10 intentos (orden de declaraciones, copia
  del color, temporales del alfa, forma de fn_ToU8, bloques): solo cambia el reparto de registros flotantes.
- `atn/D2A_MessageTable_unk` (5/6): saltada, bloqueo documentado (`push_back` en bucle: tamano/capacidad).
- `atn/EffectLoader_unk` (4/5): `fn_80239030` 90.51% -> 93.20% (10 intentos). Las tablas de curvas de
  versiones antiguas se reservan como `new EffectCurveData_unk[n][22]` (un solo `mulli 0xB0`, como el
  original). Queda reparto de registros (cabecera r26/recurso r27 en el original).
  - Hallazgo MWCC: `(T(*)[N]) new T[n * N]` sale con `mulli N` + `slwi`; `new T[n][N]` con un solo `mulli N*sizeof(T)`.
