# Lote batch-auto-5kb - cola q8

Cola (de menor a mayor): `game/MapPartsOcean_cont`, `MP/NetGlobals_unk`, `Event/Label`, `game/HeapInfo_unk`,
`MP/CharaListeners_unk`, `game/MapPartsFactory`, `game/PlayerTask`, `Event/EventLoader_unk`,
`game/MapInstance_unk`, `game/PlayerManager`, `Event/Manager_cont`, `Event/Section`, `game/HomeButton_unk`,
`Event/Channel`, `game/GameSystem_unk`, `game/PathData_unk`.

## Arreglo previo de enlace (commit b0c20bf)

La copia recibida (merge de q1/q4/q5/q7) no enlazaba: `undefined fn_800698DC` (desde `atn/LH_Core`) y
`undefined fn_80473F18` (desde `game/BattleEfp_unk`), porque otra cola habia renombrado esos simbolos a
`fn_800698DC__17DebugMessages_unkFPCc` y `fn_80473F18__Q23atn10LoadHandleFv`.
- **Cambio fuera de mis unidades:** `src/atn/LH_Core.cpp` declara un `DebugMessages_unk` minimo y llama a
  `lbl_8087EEB8->fn_800698DC(mPath)`; `src/game/BattleEfp_unk.cpp` llama a `mLoader.fn_80473F18()`
  (miembro de `atn::LoadHandle`). Mismo codigo; ambas siguen al 100%, `main.dol: OK`, lost 0.

## game/MapPartsOcean_cont (commit 06f950f)

Fusionada en `game/MapPartsOcean`.

- Resultado: 1/1 al 100% (`__sinit_\MapPartsOcean_cpp`); `game/MapPartsOcean` sigue `matching: true`, `main.dol: OK`.
- **Decisiones sin el usuario**
  - `splits.txt`: `game/MapPartsOcean.cpp` .text 0x8049C8DC-0x8049CDA0 -> 0x8049C8DC-0x8049CDBC y gana
    `.ctors` 0x8072D384-0x8072D388 y `.sbss` 0x8087F560-0x8087F578 (los del `_cont`); borrada la entrada
    `game/MapPartsOcean_cont.cpp`.
  - El `__sinit` solo guarda la direccion de tres estaticos de 4 bytes en otros tres: se reproduce con
    `static int sStatic0..2_unk` y `static StaticRef_unk sStaticRef0..2_unk(&sStaticN_unk)` (tipo inventado
    con constructor; un puntero inicializado con `&x` sale estatico en `.sdata`, sin `__sinit`). El mismo
    `__sinit` exacto esta en `gmk/CharaLight` (0x80423340) y `atn/SequenceHolder` (0x800C41B4): viene de
    alguna cabecera comun (sin identificar).
  - `symbols.txt`: `fn_8049CDA0` -> `__sinit_\MapPartsOcean_cpp`; `lbl_8087F560..574` -> `sStatic0_unk`,
    `sStatic1_unk`, `sStatic2_unk`, `sStaticRef0_unk`, `sStaticRef1_unk`, `sStaticRef2_unk` (`scope:local`).
- Hallazgos MWCC: un objeto estatico sin usar cuyo constructor inline solo guarda un puntero no se elimina y
  genera `__sinit` (`li` x3 + `stw` x3); estaticos POD sin usar si se eliminan.

## MP/NetGlobals_unk (commit 6a60737)

- Resultado: 1/1 al 100% (`__sinit_\NetGlobals_unk_cpp`), `matching: true`.
- **Decisiones sin el usuario**
  - El `__sinit` construye y registra los dos estaticos de `<iostream>` de MSL: cabecera nueva
    `include/std/ios_init.h` con `std::ios_base::Init` (0x8068B770 crea cin/cout/cerr/clog sobre `__files`)
    y `std::wios_base_Init_unk` (0x8068BD60, lo mismo con otro fichero de la libreria; nombre real
    desconocido) y los estaticos `__msl_ios_init_unk`/`__msl_wios_init_unk` (nombres inventados). La usaran
    `MP/CryptMP` (0x804AE7C4), `MP/func_enter_setting` (0x80501064) y `Event/ActionActorPosition` (0x8055A440).
  - `symbols.txt`: `fn_8068B770` -> `__ct__Q33std8ios_base4InitFv`, `fn_8068BC30` -> `__dt__Q33std8ios_base4InitFv`,
    `fn_8068BD60` -> `__ct__Q23std18wios_base_Init_unkFv`, `fn_8068C240` -> `__dt__Q23std18wios_base_Init_unkFv`
    (funciones de libreria en asm; ningun fuente las declaraba); `fn_804FF960` -> `__sinit_\NetGlobals_unk_cpp`,
    `lbl_8087F620`/`lbl_8087F624` -> `__msl_ios_init_unk`/`__msl_wios_init_unk` (`scope:local`).
  - Los `.sbss` 0x8087F628/2C/30 (en el split de la unidad) se definen aqui como globales `extern "C" void*`
    con su nombre `lbl_`: los escribe `atn/PacketPack` (MP::ManagerMP) y los leen los menus MP
    (`MpData_unk*`). `lbl_8087F630` sigue con tamano 0x8 en symbols.txt (aqui 4 bytes + relleno; enlaza OK).
  - Nombre real propuesto: un fichero de globales MP que incluye `<iostream>` (sin pruebas del nombre).

## Event/Label (commit bb4920e)

- Resultado: 1/2 al 100%, `matching: false`.
- NONMATCHING: `__sinit_\Label_cpp` 93.8% - tras la ultima llamada al hash el original guarda el valor por
  defecto de "frame" (+0x3C) antes que su hash (+0x34) e intercala el epilogo; probados valor por defecto
  int/puntero/clase/union/omitido, clase para el hash, tabla static/global/const (unas 10 variantes). El
  mismo patron final aparece en el `__sinit` de `Event/Cut`.
- **Decisiones sin el usuario**
  - Cabeceras nuevas `include/Event/Node.h` (`Event::Node`: vtable + `NodeName_unk` en +0x04 con `delete[]`
    en el destructor; `ParamDef_unk`, entrada de 0x18 de las tablas de atributos: nombre, hash
    `fn_800DC880(nombre)`, tipo (0 int, 0x1C cadena, 0x1F enum...), valor por defecto `ParamValue_unk`,
    numero y tabla de nombres del enum; `ParamValue_unk` tiene constructor porque el `__sinit` guarda
    tambien los 0) y `include/Event/Label.h` (`Label : Node`, 0x14: `Cut*`, indice, frame).
  - `~Node` se declara en la clase y se define `inline` despues: el destructor de Label (0x8053B7B4, copia
    debil en `Event/Cut`) lleva el cuerpo de `~Node` inlineado sin reponer la vtable.
  - Funcion reconstruida `Label::fn_Stripped_unk(std::string&)` (`path += "."`): el pool `@stringBase0`
    empieza por `"."`, `"basic_string: out_of_range"`, `"basic_string: length_error"` (literales de
    funciones eliminadas por el enlazador: el lector de atributos y el `__replace_unk` instanciado). Con ella
    el pool sale identico (0x47 bytes).
  - `symbols.txt`: `fn_8053C6A4` -> `__ct__Q25Event5LabelFPQ25Event3Cuti`, `fn_8053C6C8` -> `__sinit_\Label_cpp`,
    `lbl_80793DF0` -> `sParamDefs_unk__5Event` (`scope:local`), `lbl_8075DD20` -> `@stringBase0` (0x48 -> 0x47),
    `fn_8053B7B4` (en Event/Cut) -> `__dt__Q25Event5LabelFv`, `fn_80537780` (en Event/Node) -> `__dt__Q25Event4NodeFv`.
  - `game_units.json`: nueva entrada `Event/Label.cpp` tras `Event/Node.cpp`.
- Hallazgos MWCC: las cadenas de las funciones de plantilla instanciadas (p. ej. `basic_string::__replace_unk`)
  van al `@stringBase0` del fichero que las instancia, aunque el enlazador descarte la copia debil.

## game/HeapInfo_unk (commit HASH4)

- Resultado: 4/4 al 100%, `matching: true`.
- **Decisiones sin el usuario**
  - Clase local inventada `HeapInfo_unk` (0x98, la crea `fn_8047A4C8` de main en `lbl_8087F534`): cinco
    `FrameTimer_unk` (inicio/fin/transcurrido `OSTime`, main los rellena con `OSGetTime`), cuatro floats con
    los microsegundos (`OSTicksToMicroseconds` sobre la parte baja), tres resultados de
    `DLAllocator::fn_80084F84` de los heaps de `fn_800827E0()` (+0x138/+0x150/+0x168, `MemoryManager_unk`
    local) y `fn_805A6D10` = 1000000.0f / microsegundos (fps). Macros del SDK copiadas en local con sufijo `_unk`.
  - Los cinco temporizadores son miembros sueltos (con un array el constructor sale como bucle).
  - `symbols.txt`: `fn_805A6B54` -> `__ct__12HeapInfo_unkFv`, `fn_805A6BD4`/`fn_805A6BD8`/`fn_805A6D10` ->
    `...__12HeapInfo_unkFv`.
- Hallazgos MWCC: `float = (unsigned)x` da `fsubs`; el original (`fsub` + `frsp`) pasa por `double`:
  `f = (double)(unsigned)x`.
