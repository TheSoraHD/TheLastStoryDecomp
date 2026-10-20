# batch-auto-10kb qA3: atn (D2A, DLMalloc, EffectDesc)

Cola: `atn/D2A_Manager`, `atn/D2Anime_unk`, `atn/DLMalloc_unk`, `atn/EffectDesc_unk`.

## atn/D2A_Manager (commit 4ba1650)

- Resultado: 11/21 al 100% (las 7 copias debiles de 0x801F1818-0x801F19D0 no salen en nuestro `.o`
  porque nuestro fn_801F1658 las inlinea), `matching: false`, `main.dol: OK`.
- NONMATCHING:
  - `fn_801F10E0` 96.41% (dibujar icono): registros; el original guarda la direccion del layout
    (task + 0x58) de la primera llamada en r31 en vez del task, y en la segunda llamada de uv calcula la
    direccion del task antes que la de la definicion.
  - `fn_801F1658` 0% (partir una linea en trozos de texto e iconos): el original supera el presupuesto de
    inlining y llama fuera de linea a todas sus inline (operator=, clear, ctor/dtor del trozo, size,
    operator[], data, push_back); el nuestro las inlinea todas. Con una candidata extra de 3-4 sentencias
    (prueba) se da la vuelta y solo quedan registros de los indices: al original le sobran unas pocas
    sentencias en push_back o en los helpers que no he encontrado. El `push_back` debil (0x801F19D0) ademas
    carga el tamano antes que la capacidad (bloqueo conocido de `include/std/vector.h`) y ordena distinto
    sus temporales de pila.
  - `__sinit_\D2A_Manager_cpp` 98.55%: r3/r4 intercambiados (tabla / primer temporal).
- **Decisiones sin el usuario**
  - Splits: `atn/D2A_Manager.cpp` gana `.sbss 0x8087F138-0x8087F140` (la instancia `lbl_8087F138`, que
    estaba en un hueco auto).
  - `symbols.txt`: `fn_801F0DF0`..`fn_801F1094` -> ctor/dtor/vf0C/vf10/vf14 de `atn::D2A_Manager`;
    `fn_801F1818` -> `std::wstring::operator=`; `fn_801F18F4`/`fn_801F1908` -> ctor/dtor de
    `D2A_MsgPiece_unk`; `fn_801F19D0` -> `push_back__Q23std26vector<16D2A_MsgPiece_unk>FRC16D2A_MsgPiece_unk`;
    `fn_801F1EA0` -> `__sinit_\D2A_Manager_cpp`; datos `sIconFiles`, `sIconDefs`, `sIconUVs`,
    `@stringBase0` (0x8073E228) y `@STRING@__allocate__Q23std26vector<16D2A_MsgPiece_unk>FUi` (0x80782B48).
    Las funciones libres (`fn_801F0D80`, `fn_801F1098`, `fn_801F10E0`, `fn_801F1418`, `fn_801F14FC`,
    `fn_801F1658`, `fn_801F1E7C`) y `lbl_8087F138` conservan su nombre `extern "C"` porque las llaman
    D2A_Message, D2A_Window, D2A_Task, MenuCommonTask, MenuCommon_unk y main.
  - Cabecera compartida `include/std/string.h`: `basic_string::operator=(const basic_string&)` devuelve el
    resultado de `__replace_unk` en la rama larga (la copia debil 0x801F1818 no guarda `this`). regress sin
    perdidas.
  - Tipos inventados (locales): `D2A_IconDef_unk` (tabla de 96 etiquetas `$xx` de 0x34 bytes),
    `D2A_IconUV_unk`/`D2A_IconUVPair_unk` (uv y mascara, copia psq), `D2A_IconRect_unk`,
    `D2A_MsgPiece_unk` (igual que en D2A_Message.cpp), una especializacion de `__vec_split_unk` y de
    `vector::__swap_out_unk` para los trozos (copia uno a uno; los viejos se destruyen con el bufer), y
    una declaracion minima de `D2A_MessageTable_unk` (definida en su .cpp).
  - Nombre real propuesto: `atn_d2a_manager.cpp` (RTTI "atn::D2A_Manager").
- Hallazgos MWCC
  - Una tabla estatica de structs con dos miembros de clase con ctor de copia (`{UV(...), UV(...)}`) se
    construye en temporales y se copia (como el original); una matriz 2D `UV t[17][2]` se construye en
    sitio.
- Pendientes para otras unidades: `src/auto_fn_801F1EA0_text.cpp` es un resto huerfano (sigue en
  configure.py como objeto, sin split); se podria borrar.

## atn/D2Anime_unk (commit 270c9d8)

- Resultado: 4/7 al 100% (ctors de `D2A_Entry8a_unk`, `D2A_Entry8b_unk`, `D2A_Name_unk` y `__sinit`),
  `matching: false`, `main.dol: OK`.
- NONMATCHING:
  - `fn_801FFA04` 91.99% (cargador del .d2b): planificacion de los stores del lector/contexto en el
    prologo; el `vector<float*>::resize` inline pliega la comprobacion de longitud con tamano/capacidad
    conocidos (el original los recarga y conserva un camino rapido muerto); registros de los bucles.
  - `fn_80200ED0` 98.73% (campos comunes de los elementos) y `fn_80201794` 95.98% (textura): el original
    vuelve a materializar el 0 de cada `ReadTracks` inline (el nuestro lo deja en un registro salvado);
    en la textura ademas el `assign` de `std::string` copia el puntero antes de sumar la longitud.
- **Decisiones sin el usuario**
  - Splits: sin cambios (el `.sdata` 0x8087DB08-0x8087DB20 con los `""` de los `Alloc` sigue en `auto_`).
  - `symbols.txt`: `fn_80201DC4`/`DD4`/`DE8` -> ctors de `D2A_Entry8a_unk`/`D2A_Entry8b_unk`/`D2A_Name_unk`;
    **`fn_80201E10` borrado** y el ctor de `D2A_Name_unk` pasa de 0x28 a 0x34: esos 0xC bytes son la rama
    muerta de `clear()` que MWCC deja tras el `blr` del ctor (dtk la tomo por una funcion); `fn_80201E1C`
    -> `__sinit_\D2Anime_unk_cpp`; `lbl_8073E628` -> `@stringBase0` (0x60 -> 0x59);
    `lbl_8087F150/54` -> `__msl_ios_init_unk`/`__msl_wios_init_unk` (`std/ios_init.h`).
  - Cabeceras compartidas:
    - `include/atn/D2A_Data.h`: `D2A_Buffer_unk::Alloc` (inline nuevo, nombre inventado; sus `""` son
      `@STRING@` debiles por instanciacion); `m80` pasa a `D2A_Buffer_unk<unsigned char>` (codigo de
      expresiones, ver `fn_800A6E8C` de ParseTree); `D2A_NameEntry_unk::mName` pasa a
      `basic_string<wchar_t>`; `D2A_Entry8b_unk::m04` pasa a `float`. regress sin perdidas.
    - `include/std/vector.h`: `vector::resize` y `__append_unk` (inline) con `__check_append_unk` fuera de la
      clase (su "vector length error" va al pool del fichero), y `__vec_split_unk<T*>::__append_zero_unk`.
  - Tipos inventados (locales): `D2A_Reader_unk` (lector {datos, tamano, pos, contexto} con `Read<T>`,
    `ReadBytes`, `ReadArray`, `ReadString`, `ReadTracks<N>`), `D2A_LoadContext_unk`, `D2A_TrackGroup_unk<N>`,
    vistas `D2A_ElementData_unk`/`D2A_TextureData_unk`/`D2A_MessageData_unk`/`D2A_ParamsData_unk`/
    `D2A_GroupData_unk` de los campos de los elementos que lee el cargador (D2A_Common.h solo nombra
    unos pocos) y `D2A_LayoutHeader_unk`.
  - Nombre real propuesto: desconocido (cargador de d2anime; por orden alfabetico entre D2A_Window y
    D3Anime).
- Hallazgos MWCC
  - Un ctor inline que llama a `basic_string::clear()` sobre una cadena recien puesta a cero deja la rama
    larga muerta (`sth r4,0(r4); stw r4,8(r3); blr`) detras del `blr`: dtk la parte como funcion propia.
  - Una funcion miembro inline definida en la clase pone sus literales en `@STRING@` de `.data`; para que
    "vector length error" vaya al `@stringBase0` hay que sacarla fuera de la clase (y que sea pequena para
    que `-inline auto` la siga inlineando).
- Pendientes: el `.sdata` 0x8087DB08-0x8087DB20 (6 `""`) deberia entrar en este split si se enlaza.
