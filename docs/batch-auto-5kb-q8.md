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

## game/HeapInfo_unk (commit 83bf398)

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

## MP/CharaListeners_unk (commit 66edd29)

- Resultado: 3/3 al 100%, `matching: true`.
- **Decisiones sin el usuario**
  - Clase local inventada `MpCharaRef_unk` (solo `CharaTask* mChara`): las tres funciones las llama
    `atn/PacketPack` (MP::ManagerMP) con el puntero al personaje de un jugador. `fn_805010C0` lanza
    `fn_800FDE60(lbl_8087F048, 0, chara, (0,1,0))` con los listeners desactivados; `fn_805011CC` reinicia el
    personaje; `fn_80501288` es el setter (salvo sesion en estado 30 y `m50C` 3/4).
  - `MpSession_unk` local (copia de la de `MP/D2AMpMenuCockpit.cpp` con `m4FC`) e inline `fn_8014F8F4` con el
    estatico `sListeners` como en gmk/Item.cpp (el registro de destructor 0x807C8F98 ya estaba en el split).
  - `symbols.txt`: `fn_805010C0`/`fn_805011CC` -> `...__14MpCharaRef_unkFv`, `fn_80501288` ->
    `fn_80501288__14MpCharaRef_unkFP9CharaTask`.
  - Nombre real propuesto: ninguno (parte de MP; sin pruebas).
- Hallazgos MWCC: las copias debiles (destructor de `CharaListeners_unk` y de su `std::vector`) quedan en el
  .o entre funciones propias y el enlazador las descarta sin romper el `.dol` (existen antes en 0x8003EFB0...).

## game/MapPartsFactory (commit 2556bc6)

- Resultado: 2/2 al 100%, `matching: true`.
- **Decisiones sin el usuario**
  - Funciones `extern "C"` con sus nombres de symbols.txt (`fn_80496A28` tipo por nombre: MODEL 0, OCEAN 1,
    COLLI_TREE 2, EFP 3, LOCATORS 4, GIMMICK_LOC 5, AREAFILE 6, LENSFLARE 7; `fn_80496B3C` crea la pieza
    pasando el tipo como `void* arg`, salvo OCEAN que pasa 0). Los creadores de `MapPartsLocatorSet`
    (`fn_8049A018`), `MapPartsGimmickLoc` (`fn_80496EE8`) y `MapPartsArea` (`fn_80491EA0`) se declaran
    `extern "C"` (sin cabecera; no se renombran).
  - `symbols.txt`: `lbl_80756A10` -> `@stringBase0` (0x48 -> 0x43).
- Hallazgos MWCC: el ultimo `if` de una cadena de `return` que sale `li r3,1; beq; li r3,-1` se escribe
  `if (x != 0) return -1; return 1;` (con `== 0` salen invertidos).

## game/PlayerTask (commit a4d7b24)

- Resultado: 6/6 al 100%, `matching: true`.
- **Decisiones sin el usuario**
  - Mismo esquema que `game/EnemyTask` (q7): creador estatico `PlayerTask::fn_8054A3DC(parent, type, data)`
    (si `fn_8020924C((int)data)->m24 == 0x42` crea un `FriendArganan`; el ctor de PlayerTask se autoinlinea
    en el creador), ctor/dtor con `CharaManager<PlayerTask>` `lbl_8087F8A0` (`NextId_unk`, `Add_unk`,
    `Remove_unk`), lista enlazada `mNext` y `fn_8054A68C` (jugador anterior, el ultimo para el primero).
  - `include/game/PlayerTask.h`: declarados `fn_8054A3DC` y `fn_8054A68C`.
  - `symbols.txt`: `fn_8054A3DC` -> `fn_8054A3DC__10PlayerTaskFPQ23atn4TaskiPv`, `fn_8054A68C` ->
    `fn_8054A68C__10PlayerTaskFv`, `lbl_8075E478` -> `@stringBase0` (0x8 -> 0x1).

## Event/EventLoader_unk (commit e2e7629)

- Resultado: 3/5 al 100%, `matching: false`.
- NONMATCHING:
  - `fn_80576030` 97.4% - registros salvados: el original deja seq/pool/p/arg/path en r31/r30/r29/r28/r27
    (path el ultimo); aqui path toma r31 (probado: metodo, helper estatico autoinlineado, locales arriba,
    `&p[n]`, `memset`).
  - `fn_80576258` 95.2% - el original guarda los cuatro contadores en r31..r28 y los punteros en volatiles;
    aqui el puntero al cut toma r31 (probado contador int/unsigned, accesor, `switch`). El accesor
    `Size_unk()` hace que la comprobacion de rango reutilice el tamano cargado por la condicion del bucle.
- **Decisiones sin el usuario**
  - Funciones `extern "C"` (nombres de symbols.txt) sobre un `EventSequence_unk` local con los campos de
    `Event::Sequence` usados (bit alto de +0x98, vector de cuts +0xB8, id +0x190, estado +0x194,
    `atn::LH_Binary` +0x1C4, tamano/datos +0x1CC/+0x1D0, `FixedString_unk<0x100>` +0x2A0). Vectores de
    `shared_ptr` como `EventVec_unk<T>` local con accesor comprobado (`i >= 0 && i < size`).
  - Funcion reconstruida `fn_Stripped_unk` (`strstr(path, ".xml")`): el pool tiene `".xml"` antes de los
    mensajes de `basic_string` (lector eliminado por el enlazador); se incluye `std/string_members.h` para
    que `reserve`/`__replace_unk` instanciados pongan sus mensajes. Pool identico (0x71).
  - `symbols.txt`: `lbl_80760D70` -> `@stringBase0` (0x78 -> 0x71).

## game/MapInstance_unk (commit 0130406)

- Resultado: 3/5 al 100%, `matching: false`.
- NONMATCHING: los dos constructores, 94.1% y 93.1% - planificacion: los stores del contador/id frente a la
  conversion de grados y la carga del nombre en el bloque de LOD (probado id antes/despues de la rotacion,
  inicializadores de miembro, `Vector3 * float`, helper `DegToRad_unk`, rotacion de Euler inline).
- **Decisiones sin el usuario**
  - Clase inventada `MapInstance_unk` (0x90, `new(0xC)` desde `MapPartsLocatorSet::fn_8049B1F0`) con un
    `atn::ModelInstance` como primer miembro (el constructor no guarda vtable propia), bitfield en +0x88
    (oculto:1, contador de frames:15, id:16) y hash del nombre en +0x8C. Dos constructores sobrecargados
    (`MapLocator_unk*` y `MapLocatorShear_unk*`, este con cizalla en +0x30/0x34/0x38): escala, cizalla,
    rotacion z/y/x (inline `RotateEuler_unk` -> `Rotate_unk`, como `Matrix3x4::fn_801496A8`) y traslacion.
  - `include/gmk/Lemon.h` (vista de `atn::ModelInstance`): `m68[0x20]` se parte en `m68[8]`, `m70`, `m74`,
    `m78` (LOD) y `m7C[0xC]`; mismo layout.
  - `symbols.txt`: `fn_80490884` -> `__ct__15MapInstance_unkFPCcP14MapLocator_unk`, `fn_80490B0C` ->
    `__ct__15MapInstance_unkFPCcP19MapLocatorShear_unk`, `fn_80490DE8`/`fn_80490E38`/`fn_80490EA0` ->
    metodos de `MapInstance_unk`, `lbl_8087F550` -> `sCount_unk` (`scope:local`).
- Hallazgos MWCC: varios bloques `if (...) Rotate_unk(...)` (inline con dos `Mtx`) dentro de otra inline
  colocan sus locales en orden ascendente (el primero mas abajo); llamados directamente desde la funcion,
  al reves.
- Hallazgos MWCC: `0.017453292f * x` en un helper inline (`DegToRad_unk`) deja la constante en f0 y los
  campos en f9/f8/f7, como el original; escrito en linea la constante toma f9.

## game/PlayerManager (commit 7a2a1e5)

- Resultado: 6/8 al 100%, `matching: false`.
- NONMATCHING:
  - `fn_80549F14__13PlayerManagerFi` 99.5% - registros salvados (el original reutiliza r30 para el id y la
    cabeza y deja el jugador en r29 en las dos ramas).
  - `fn_8054A09C__13PlayerManagerFP10PlayerTaski` 97.2% - numeracion de registros volatiles en los bucles
    que intercambian lider/jugador en la tabla de huecos del campo.
- **Decisiones sin el usuario**
  - Cabecera nueva `include/game/PlayerManager.h`: `PlayerManager : CharaManager<PlayerTask>` (0x60; `m54`
    lider inicial, `ResHandle mMenu` en +0x58 con el grupo de menu de depuracion, `m5C`). El constructor no
    es `inline` (su literal va en `@stringBase0`); su copia fuera de linea sobra y la quita el enlazador.
  - **Cambio fuera de mis unidades:** `CharaTask::fn_801546F4` pasa de `()` a
    `(int arg, atn::Vector3 pos, float rot)` en `include/game/CharaTask.h` y `src/game/CharaTask.cpp`
    (todos los llamadores pasan r4, un Vector3 por valor y f1; el cuerpo no los lee y sigue al 100%);
    `symbols.txt`: `fn_801546F4__9CharaTaskFv` -> `fn_801546F4__9CharaTaskFiQ23atn7Vector3f`. CharaTask no
    se enlaza (`matching: false`); `regress.py` lost 0.
  - Tipos locales: `PartyTable_unk`/`PartySlot_unk` (tabla `lbl_8087F430->m10D8`, huecos de 0x90 con 32
    punteros a jugador + uno), `LeaderListener_unk` (`lbl_8087FA00`, elementos de 0x37C), `MpPlayer_unk`.
  - `symbols.txt`: los 8 `fn_8054xxxx` -> metodos de `PlayerManager` (creador, `__dt__`, `vf0C`, `vf1C`,
    `fn_80549F14`, `fn_8054A09C`, `fn_8054A33C`, `fn_8054A398`); `lbl_8075E468` -> `@stringBase0` (0x10 -> 0xF).

## Event/Manager_cont (sin commit propio: no decompilada)

- Resultado: 0/3, se deja como unidad en asm (sin fuente ni entrada en `game_units.json`).
- **Decision sin el usuario: no se fusiona ni se decompila.** El `_cont` no es la continuacion de
  `Event/PostSequence` (la unica unidad enlazada vecina) sino del fichero de `Event/Manager` (sin fuente,
  `matching: false`): su `.rodata` 0x80756380 es el pool entero de cadenas de Event::Manager (rutas d2a,
  nombres del menu de depuracion "DebugCamera", "MAStartOffset"..., "event2/%s.event", "SE_UICOM_011",
  "basePos"/"colorR"...), el `__sinit` 0x80490850 lee constantes de la `.sdata2` de Event/Manager
  (0x80886F98, 0x80887000) y las dos funciones grandes son plantillas de `std::vector` de un elemento de
  0x194 bytes de Event::Manager. El fichero real seria Event/Manager + Event/PostSequence (+SequenceRoot)
  + Event/Manager_cont (0x8047B768-0x80490884). Con Event/Manager sin decompilar no se puede reproducir
  ni el pool ni las constantes compartidas, asi que ni la fusion en PostSequence (romperia su
  `matching: true`) ni una unidad propia llegan al 100%.
- Propuesta: fusionar `Event/Manager`, `Event/PostSequence` y `Event/Manager_cont` en `Event/Manager.cpp`
  cuando se decompile Event::Manager.

## Event/Section (commit d3b009a)

- Resultado: 5/6 al 100%, `matching: false`.
- NONMATCHING: `fn_8053C788__Q25Event7SectionFv` 38% - crea un `boost::shared_ptr<Channel>` y lo mete en
  el vector: el original cuenta referencias con bucles `lwarx/stwcx.` (funciones asm de boost para
  CodeWarrior PPC), imposibles sin asm; aqui son incrementos normales. Mientras no se acepte asm, ninguna
  funcion que copie o libere un `shared_ptr` puede llegar al 100% (afecta a Cut, Channel, Section).
- **Decisiones sin el usuario**
  - Cabecera nueva `include/Event/SharedPtr_unk.h`: reconstruccion minima de `boost::shared_ptr`,
    `detail::shared_count`, `sp_counted_base` (vptr en +8, tras los dos contadores) y
    `sp_counted_impl_p<T>`, con `sp_enable_shared_from_this(const shared_count&, ...)` vacia y variadica.
    Con ella salen al 100% `dispose`, `get_deleter` y el destructor de `sp_counted_impl_p<Channel>`.
  - Cabeceras nuevas `include/Event/Section.h` (`Section : Node`, 0x24: `Cut*`, `m0C`, `m10`, `m14`=1,
    `std::vector<shared_ptr<Channel> >` en +0x18; el ctor recibe un indice que no guarda) y
    `include/Event/Channel.h` (`Channel : Node`, 0x20, `std::vector<shared_ptr<Action> >` en +0x14).
    `include/Event/Node.h`: `ParamEnum_unk` (pares nombre/etiqueta de los enums) como tipo de `mEnumNames`.
  - Cuatro tablas de atributos (una por tipo de seccion: director, camera, character, dialog) como
    arrays estaticos separados: el `__sinit` recalcula `base + 0x60*k` para cada una.
  - Funcion reconstruida `Section::fn_Stripped_unk` (`path += "."`) por el pool, como en Label.
  - `symbols.txt`: `fn_8053C750` -> `__ct__Q25Event7SectionFPQ25Event3Cuti`, `fn_8053C788` ->
    `fn_8053C788__Q25Event7SectionFv`, `fn_8053CED8`/`fn_8053CEFC`/`fn_8053CF04` -> `dispose`/`get_deleter`/
    `__dt__` de `boost::detail::sp_counted_impl_p<Event::Channel>`, `fn_8053CF44` -> `__sinit_\Section_cpp`,
    `lbl_8075DE90` -> `@stringBase0` (0x68 -> 0x66); en Event/Cut `fn_8053BF80` -> `__dt__Q25Event7SectionFv`;
    en Event/Channel `fn_8053AC68` -> `destroy__Q35boost6detail15sp_counted_baseFv` y `fn_8053AC88` ->
    `sp_enable_shared_from_this__Q25boost6detailFRCQ35boost6detail12shared_counte`.
  - `symbols.txt` (**simbolos anadidos**): `lbl_80793E58` (0x180) se parte en cuatro de 0x60
    (`lbl_80793E58`, `lbl_80793EB8`, `lbl_80793F18`, `lbl_80793F78`), uno por tabla; con un solo simbolo
    objdiff no da el 100% al `__sinit` aunque las instrucciones sean identicas.
- Hallazgos MWCC: MWCC pone el puntero a la vtable detras de los miembros de datos declarados antes de la
  primera funcion virtual (boost `sp_counted_base`: contadores en 0/4, vptr en 8).

## game/HomeButton_unk (commit be4b98b)

- Resultado: 6/8 al 100%, `matching: false`.
- NONMATCHING:
  - `fn_804411DC` 99.8% - el original carga el stick x/y en f0/f1 (aqui f1/f0) e intercambia los registros
    del contador y el desplazamiento del bucle (r28/r29).
  - `fn_804413D4` 84.5% - el original guarda las palabras altas (0x4330) de las dos conversiones int->float
    al principio de la funcion; aqui junto a la conversion.
- **Decisiones sin el usuario**
  - Codigo calcado del ejemplo HBM del SDK: tipos locales `HBMDataInfo_unk`, `HBMKPadData_unk`,
    `HBMControllerData_unk`, `KPADStatus_unk` (solo los campos usados; el `HBMTypes.h` de libs no esta en el
    camino de includes del juego). Las funciones de la libreria HBM se llaman por sus nombres de
    symbols.txt (`fn_805C3C70` HBMCreate, `fn_805C3F40` HBMCreateSound, `fn_805C3D90` HBMDelete,
    `fn_805C3FA0` HBMDeleteSound, `fn_805C3E00` HBMInit, `fn_805C3ED0` HBMSetAdjustFlag, `fn_805C3FB0`
    HBMUpdateSound, `fn_805C3E30` HBMCalc, `fn_805C3E70` HBMDraw, `fn_80660CF0` WPADProbe; no se renombran).
  - Funciones `extern "C"` con sus nombres (las declara asi `game/HomeButtonMenu.cpp`).
  - `fn_804413D4`: el factor 1.0f es un local por rama (`228.0f * -scale` da `fneg`; con literales o una
    inline se pliega todo).
  - `symbols.txt`: `lbl_80754520` -> `@stringBase0` (0xD0 -> 0xCD).
  - Nombre real propuesto: parte de `game/HomeButtonMenu.cpp` (sin `__sinit` ni pruebas en contra), o un
    fichero propio del ejemplo HBM; se deja separado.
- Hallazgos MWCC: `float s = 1.0f;` local no se propaga: `228.0f * -s` sale `lfs 1.0; fneg; fmuls`, y si
  el local esta en cada rama la constante se carga dentro de cada una.

## Event/Channel (commit HASH14)

- Resultado: 8/11 al 100%, `matching: false`.
- NONMATCHING:
  - `fn_8053A510__Q25Event7ChannelFi` 38% y `__dt__Q25Event7ChannelFv` 49% - copia/liberacion de
    `boost::shared_ptr<Action>` con contadores atomicos (`lwarx/stwcx.`, asm de boost) en el original.
  - `__sinit_\Channel_cpp` 93.6% - mismo problema que `Event/Label` (el valor por defecto del ultimo atributo
    se guarda antes que su hash); aqui los registros coinciden con el original, asi que es el orden del IR.
    Probado ademas: hash como clase con ctor + defecto escalar (`(int)""` sale estatico, no sirve).
- **Decisiones sin el usuario**
  - `include/Event/Action.h` nueva (`Action : Node`, 0x40, ctor `(Channel*, int)` en Event/Node);
    `include/Event/Channel.h` declara `fn_8053A510` (crea y anade una accion) y el lector eliminado.
  - Las funciones debiles que el original emite aqui por primera vez salen al 100%:
    `sp_counted_base::destroy`, `sp_enable_shared_from_this`, `std::bad_alloc::~bad_alloc`/`what`,
    `sp_counted_impl_p<Action>::dispose`/`get_deleter`/destructor.
  - Funcion reconstruida `Channel::fn_Stripped_unk` (`path += "."`) por el pool, como en Label/Section.
  - `symbols.txt`: `fn_8053A4DC` -> `__ct__Q25Event7ChannelFPQ25Event7Section`, `fn_8053A510` ->
    `fn_8053A510__Q25Event7ChannelFi`, `fn_8053AD50` -> `__dt__Q25Event7ChannelFv`, `fn_8053AF34` ->
    `__sinit_\Channel_cpp`, `fn_8053ACD8`/`fn_8053AD18` -> `__dt__`/`what__` de `std::bad_alloc`,
    `fn_8053AD24`/`fn_8053AD48`/`fn_8053AEF4` -> `dispose`/`get_deleter`/`__dt__` de
    `sp_counted_impl_p<Event::Action>`, `lbl_8075DA28` -> `@stringBase0` (0x68 -> 0x62),
    `lbl_80793C0C` -> `@STRING@what__Q23std9bad_allocCFv`, `lbl_80793BF0` ->
    `@STRING@__allocate__Q23std45vector<Q25boost27shared_ptr<Q25Event6Action>>FUi` (`scope:local`); en
    Event/Node `fn_80537708` -> `__ct__Q25Event6ActionFPQ25Event7Channeli`, `fn_8053A40C` ->
    `__dt__Q25Event6ActionFv`, `fn_8053A3CC` -> `vf0C__Q25Event6ActionFv` (por la vtable de Action).
