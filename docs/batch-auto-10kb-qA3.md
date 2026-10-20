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
