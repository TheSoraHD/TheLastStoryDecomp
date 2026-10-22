# batch-auto-20kb-atn qA7: atn (D2A_Common, D2A_TaskInstanceBase)

Cola: `atn/D2A_Common`, `atn/D2A_TaskInstanceBase_64_8`.

## atn/D2A_Common (commit 2ee9136)
- Resultado: 13/13 al 100%, `matching: true` (enlazada), `main.dol: OK`.
- **Decisiones sin el usuario**
  - Splits: `atn/D2A_Common.cpp` gana `.rodata 0x8073CD68-0x8073CDA8` (su `@stringBase0` con los formatos
    "pos%04d"... que estaba en un hueco `auto_`) y `.sdata 0x8087DAB0-0x8087DAD0` (pools debiles `@STRING@` de
    `D2A_PtrArray_unk::Reserve` de sus cuatro instanciaciones; otro hueco `auto_`).
  - `symbols.txt`: `lbl_8073CD68` -> `@stringBase0` (0x3F); `lbl_8087DAB0..DACC` -> `@STRING@Reserve__...`
    de `D2A_PtrArray_unk<D2A_Anim_unk<1>*>`, `<D2A_Anim_unk<5>*>`, `<D2A_Anim_unk<4>*>`, `<D2A_IntAnim_unk*>`.
  - `fn_801ED910` (pone el padre) y `fn_801EDCB8` (vacia) siguen `extern "C"` (las llaman D2Anime_unk y
    D2A_Texture con ese nombre).
  - Cabeceras compartidas (regress sin perdidas):
    - `include/atn/D2A_Common.h`: layout completo de `D2A_Common` (0x12C): `m10` es una union palabra/bitfield
      (el ctor la pone a 0 con un `stw`), `D2A_Anim_unk<N>` (hash del nombre, N tablas de claves y N valores:
      "pos" 5, "col" 4, "rot" 1, "sdw_col" 4, "sdw_ofs" 1), `D2A_IntAnim_unk` ("blend"), `D2A_Expr_unk`
      (codigo de expresiones 0x104-0x12C), `D2A_IntParamTable`; `D2A_ParamTable` gana ctor inline (pone a 0).
      `m30/m38/m58` desaparecen: `src/atn/D2A_Reference.cpp` usa ahora `mRect.mTables[2/3]` y
      `mRect.mValues[4]` (mismo codigo, sigue al 100%). `mFC` sigue siendo `D2A_Owner_unk*` (D2A_Window lo usa
      asi); D2A_Common lo convierte a `D2A_Data*`.
    - `include/atn/D2A_Data.h`: `D2A_Layout_unk` con sus seis arrays tipados (0x00 `<1>`, 0x0C `<5>`, 0x18 ?,
      0x24 `<4>`, 0x30 textos, 0x3C int).
  - Tipos/funciones inventados locales: `Add_unk` (cuatro sobrecargas inline que registran un valor animado en
    el layout; dan los registros del original y, por su orden de definicion Int, 4, 5, 1, el orden de los
    pools `@STRING@` en `.sdata`), `UpdateAnim_unk`, `D2A_Shadow_unk`. `fn_801EC784` se autoinlinea en `vf0C`.
  - Las cadenas de formato tienen dos terminadores en el pool: escritas como `"pos%04d\0"` (igual que los
    formatos anchos de D2A_Task).
- Hallazgos MWCC
  - Un array de objetos con ctor inline trivial se construye con el primer elemento "pelado" (3 stores) y un
    bucle `bdnz` de 8 bytes para el resto, aunque el resto este vacio (N = 1).
  - El orden de los pools debiles `@STRING@` de una plantilla inline sigue el orden inverso de definicion de las
    funciones inline que la instancian por primera vez (las que no pasan por ellas van antes, en orden de uso).
- Pendientes: ninguno.

## atn/D2A_TaskInstanceBase_64_8 (commit fbf0cc6)
- Resultado: 47/51 al 100%, `matching: false`, `main.dol: OK`.
- NONMATCHING: `vf10` de las tres instanciaciones (97.18%) y `fn_801F69A8` (Update de <32, 8>, 96.94%): el
  original vuelve a cargar `mTask` tras sumar la velocidad al frame en vez de reutilizar el valor del test
  (unas 10 variantes probadas: getters, GetData, locales, casts, inline anidado).
- **Decisiones sin el usuario**
  - Splits: sin cambios.
  - Clase plantilla `atn::D2A_TaskInstanceBase<N, M>` (RTTI) en `include/atn/D2A_TaskInstance.h`: hasta N
    valores de panel y M textos que se aplican al `D2A_Task` compartido (cache estatica de tareas por hash de
    ruta, `ID2A_TaskInstance::sCache_unk`, 0x8087F148) antes de dibujarlo. Nombres de miembros inventados.
    `ID2A_TaskInstance` es una base sin virtuales propias.
  - `symbols.txt`: las funciones de <64, 8> y <8, 2> y las de <32, 8> sin llamadores en `src/` pasan a nombres
    mangled (Create, dtor, vf0C/10/14, Apply, SetValue*, SetColor*, SetText, SetNumber, SetInt, GetRect,
    ctor/dtor de `Text_unk`); `lbl_8073E508` -> `@stringBase0`, `lbl_80782D50` -> `@wstringBase0`,
    `lbl_8087F148` -> `sCache_unk__Q23atn17ID2A_TaskInstance` (size 4).
  - Las 14 funciones de <32, 8> que llaman otros ficheros (`fn_801F64D0` Create, `fn_801F69A8` Update,
    `fn_801F6A38` Draw, `fn_801F6C10`, `fn_801F6C2C`, `fn_801F6C38`, `fn_801F6C80`, `fn_801F6D7C`,
    `fn_801F6E78`, `fn_801F7590`, `fn_801F791C`, `fn_801F7DF0`, `fn_801F837C`, `fn_801F8598`) conservan su
    nombre automatico: son envoltorios `extern "C"` con el cuerpo del miembro escrito dentro (un nivel de inline
    mas invierte el orden de pila de los locales del inline). Pendiente: pasar los ~200 llamadores de 14
    ficheros a los miembros y renombrar.
  - `include/atn/Task.h`: `Task::m24` pasa de bytes a `Task* m24` (padre de las tareas d2anime compartidas).
  - Los setters de varios valores reciben structs por valor (`D2A_DrawRect`, `D2A_DrawColor`, `D2A_RGB_unk`):
    con `const float*` la carga de v[0] se adelantaba al prologo (mismo problema que los NONMATCHING de
    D2A_Task::SetParam*). Los llamadores (ShopUV_unk por valor...) lo confirman.
  - `SetFloat_unk` (L"%f") instanciado para <8, 2> solo por su literal: el enlazador lo elimina.
  - Tipos inventados: `D2A_FixedArray_unk<T, N>` (cuenta + array fijo, push_back con placement new, erase),
    `D2A_FixedArray2_unk` (un nivel de destructor mas: el original prueba dos veces la direccion del miembro
    antes de `__destroy_arr`), `Param_unk`, `Text_unk`, `CacheEntry_unk`, `D2A_RGB_unk`; helpers inline
    `AddParam_unk`, `SetParam_unk`, `AddText_unk`, `SetLayoutValue*_unk`.
- Hallazgos MWCC
  - Un parametro struct pasado por valor no se carga antes de los stores del prologo; uno `const float*` si
    (explica los "v[0] cargado al principio" de D2A_Task::SetParam*).
  - Literales `""` de un `new` en una funcion `inline` (libre o miembro) van a `@STRING@`; en una funcion
    miembro de plantilla sin `inline` que se autoinlinea, a `@stringBase0`.
  - Una busqueda con `return` dentro del bucle en una funcion con un local con destructor duplica el
    destructor; movida a un inline aparte, el destructor sale una sola vez como en el original.
  - Inline con parametro que luego lee `inst->mTask` tras otra llamada: el valor queda en registro volatil
    (r3) en vez de salvado, como en el original.
- Pendientes: renombrar los 14 `fn_` de <32, 8> cuando se actualicen sus llamadores; orden de emision de los
  ctor/dtor de `Text_unk` (aqui salen tras Create, en el original al final de cada grupo; solo importa para
  enlazar).
