# escribir-20261116, cola W4 (atn, chr, gmk, AI y SDK)

Agente W4, inicio 2026-11-17 02:08 (tope 04:38). Rama `agent-W4`.

## Tabla de la cola

| Funcion | Antes | Despues |
|---|---|---|
| atn/SmallAllocator_8 `~SmallAllocator<6>` (0x80082B54) | 0 % (escrita, inlinea el ctor del gestor) | igual: equivalente NONMATCHING 0 % (ciclo de inlining, ver Bloqueos) |
| atn/SmallAllocator_8 `operator delete` (0x80084684) | 0 % (escrita) | igual: equivalente NONMATCHING 0 % (mismo ciclo) |
| atn/LayerOcclusionCPU `fn_800B4390` | 0 % (C sin comprobar) | equivalente NONMATCHING 0 % (asm a mano; comprobada contra el asm) |
| atn/WordWrap_unk `ios_base::setstate`, `basic_string(const basic_string&)`, thunk `@8@60@~basic_ostream` | no emitidas | igual (ver Bloqueos) |
| atn/LH_Binary `fn_80471FD0` | no escrita | equivalente NONMATCHING 0 % (paired-single sin GQR6) |
| atn/LH_Binary `SceneTrack_unk::fn_8047202C` | no escrita | equivalente NONMATCHING 0 % |
| chr/ControlStickByQuark `ControlMatHoldDamage::vf0C` | DRAFT 94,56 % | equivalente NONMATCHING 94,6 % (comprobada contra el asm) |
| RecycleBuffer_gmk/ItemSlotAnime `fn_8044D9BC` | DRAFT 50,59 % | **100 %** |
| (extra) ItemSlotAnime `fn_8044EA34` | 69,95 % | 99,04 % (inlinea fn_8044F14C como el original) |
| (extra) LayerOcclusionCPU ctor (0x800B3A94) | 98,87 % (100 en report.json: rep100) | **100 %** en objdiff-cli |
| (extra) LayerOcclusionCPU `fn_800B3978`, `vf20` | 96,85 / 98,56 % | 99,66 / 99,36 % |
| (extra) ItemSlotAnime `fn_8044EEC0` | 96,49 % | 97,55 % (inline no const `IsShown_unk`: reutiliza mState) |
| (extra) LH_Model `Loader::Get_unk<LoadModel>` | 98,83 % | 99,73 % (size de `@stringBase0`; queda el bloqueo de Get_unk) |

## Para el port

- `SceneTrack_unk` pasa de `unsigned char m00[0xC]` a campos (`m00`, `mType`, `mCount`, `mScale`, `mKeys`) en
  la cabecera nueva `include/atn/SceneTrack_unk.h` (la incluye `atn/Light_unk.h` donde estaba el struct);
  `BinTrack_unk` de LH_Binary es ahora un typedef de `SceneTrack_unk`. Parametros de `fn_8047202C`
  renombrados `(const short& key, float frame)` (el `short` es la cache del indice de clave; se escribe).
- `SceneTrack_unk::fn_8047202C` y `fn_80471FD0` pasan de no existir a codigo real (evaluacion de pistas:
  constante, lineal, una muestra por frame y hermite). Los valores s16 se escalan por `2^-mScale` (el
  original lo hace con GQR6); `fn_80471FD0` recibe la escala como sexto argumento. Base de hermite
  `lbl_8078FBF0` = {2, 1, -3, -2, -2, 1, 3, -1} definida en LH_Binary.
- LayerOcclusionCPU `fn_800B4390`: misma semantica; la distancia al plano se suma en el orden del
  original y ya no multiplica `mDist` por `lbl_807790A8[1]` (el 1 no se usa en el original).

- ItemSlotAnime: `ItemSlotTextCache` (y `ItemSlotDraw_unk`, `ItemSlotRecord_unk`) se declaran antes de
  `fn_8044D9BC`; nuevos ctor `ItemSlotTextCache(atn::Task*)` (mNext 0, mMapType -1, mMapId/mMapSubId 0) y
  `ItemSlotRecord_unk()` (mTable/mKind 0, lista vacia). `fn_8044D9BC` pasa de borrador a codigo real: crea
  tambien la cache `lbl_8087F4FC` (antes no se creaba nunca: cambio de comportamiento) y los `push_back` de
  las tareas de cada entrada solo anaden si cabe (`PushTask_unk`, `size() < N`).

## Decisiones tomadas sin el usuario

- LayerOcclusionCPU: funcion inventada sin llamadores `LayerOcclusionPool_unk` (la quita el enlazador) al
  principio del fichero para que `.sdata2` salga 0.5f, 255.0f, 0.0f, 1.0f, 16.0f, 32.0f, 128.0f como en el
  original (antes 1.0f, 0.0f, 255.0f, ...).
- ItemSlotAnime: tres inlines con nombre inventado deducidas de los pools `@STRING@` del original
  (`""` de los `new` en `.sdata` y las rutas d2b en `.data`): `ItemSlotAnimeBuffer::Create_unk`,
  `ItemSlotTextCache::Create_unk` y `ItemSlotAnimeBuffer::Setup_unk`; ayudante `PushTask_unk` en el .cpp.

## Hallazgos

- Literales de rutas en `.data` con etiquetas sueltas en orden inverso (`lbl_8078F4E0` el primero usado, el
  resto hacia atras) = pool `@STRING@` de una inline (base, @0, @1...): mover el codigo a una inline con
  nombre `_unk` y `pd.sh` los renombra (ItemSlotAnime fn_8044D9BC 50 -> 100 %) (escribir-20261116 qW4)
- Una funcion normal (no `inline`) definida despues que el original inlinea en su llamador (y emite en su
  sitio): MWCC la inlinea sola si su cuerpo es pequeno; escribir sus bucles como inlines (`SetTaskFrame_unk`)
  la baja del umbral de autoinline (ItemSlotAnime fn_8044EA34 70 -> 99 %, fn_8044F14C sigue emitida)
  (escribir-20261116 qW4)
- `@stringBase0` cuyo ultimo literal es `""`: dtk le da el tamano sin ese `""` (LayerOcclusionCPU 0x80732E48
  size 0xD, el `""` en +0xD) y objdiff marca DIFF_REPLACE en el `addi rX, base, 0xD` aunque el texto sea
  igual: corregir el `size` en symbols.txt (0xE) lo deja al 100 % (ctor de LayerOcclusionCPU). Buscar en otras
  unidades `addi ..., <size>` sobre su `@stringBase0` (escribir-20261116 qW4)
- Un `new (1, "", "", 0)` con la doble comprobacion `if (inst == 0) { gm = ...; if (inst == 0) ... }`
  (dos `bne` con el mismo cr0) = `if (inst == 0) X::Create_unk(parent);` con la segunda comprobacion dentro
  de la inline estatica (escribir-20261116 qW4)
- `grep -m1 match_percent` de `objdiff-cli diff ... --format json-pretty` da el % de la seccion .text, no
  el de la funcion: usar `/home/claude/agentes/bin/pct.py` (escribir-20261116 qW4)
- Un local nuevo `Matrix3x4* p = &rot;` (sin inline nueva) renumero en 1 los `@N` de ControlStickByQuark
  desde esa funcion: comprobar `labelcheck.py` tras cualquier cambio en unidades sin enlazar (escribir-20261116 qW4)

## Bloqueos

- SmallAllocator_8 (`~SmallAllocator<6>`, `operator delete`, y por ellos `~MemoryMode_unk`): son codigo
  equivalente; el original llama al ctor del gestor dentro del `Instance` inlineado en `delete` (y en lo que
  inlinea `delete`: los dos `~SmallAllocator<N>`, `~MemoryManager`, `~MemoryMode_unk`) y lo inlinea en el
  resto (`__dla__`, `__sys_free`, `operator new`). Medido (tamanos en `/home/claude/scratch-W4/bin/saall.sh`):
  con el ctor al principio (hoy) cortan `~IAllocator`, `~SA<8>` y `~MemoryManager` pero no `~SA<6>` ni
  `delete`; **intercambiando el orden de mSmall8/mSmall6 corta `~SA<6>` y no `~SA<8>`** (corta el primer
  miembro, no un tipo concreto); con el ctor tras `Instance` (orden del .dol) ningun dtor corta y `delete`
  llama a los ctor de `SmallAllocator<N>` fuera de linea (se emiten); dtor de la plantilla definido fuera
  de la clase: igual. Con el ctor del gestor `inline` (sin moverlo) el orden de emision es el del .dol
  (Instance, ctor, ~SA<8>, ~SA<6>, ~MemoryManager, fn_80082CAC: el ctor sale como copia diferida de
  Instance), pero `delete` y los dtors quedan con el ctor del gestor inlineado y los ctor de
  `SmallAllocator<N>` llamados (y emitidos): el corte cae en el ctor de SmallAllocator (su vtable dispara la
  compilacion anidada de los dtors) y no en el del gestor. Ni el ctor de SmallAllocator fuera de la clase ni
  `~IAllocator` no inline lo cambian. Hipotesis: el primer inline de `delete` fija su cuerpo para todos los
  demas; en el original ocurre con el ctor del gestor en curso y sin ningun ctor de SmallAllocator en curso.
- WordWrap_unk (`setstate`, copia de `basic_string<char>`, thunk `@8@60@~basic_ostream`): en el original
  son copias weak que conserva el enlazador porque otras unidades las llaman (`setstate` EffectManager, la
  copia Loader/EffectSequencer; el thunk va en la vtable de `basic_ostream`, que el original emite en
  WordWrap y GameEventScript usa). Sus llamadores dentro de WordWrap no estan en el .dol (los quito el
  enlazador) y nuestro .o no emite ninguna vtable de streams: hace falta el codigo de iostream (ctor de
  `basic_ostringstream`) que las instancia, MSL iostream fuera de alcance. Sin cambios.
- AI_archer_attack01 `fn_8001C088` (ideas nuevas, todas peores o iguales): copia de structs de 0x64 bytes
  (`struct { Block m[5]; }`): bucle de 8 bytes x 12 (68,9 %); `operator=` propio en el bloque + copia del
  struct del array: bucle de punteros con division (42 %); contador `unsigned char`/`unsigned short`: no
  desenrolla pero indexa con `mulli` (69 %); `signed char`/`long`/`char`/`unsigned int` y punteros con
  contador `unsigned char`: desenrolla (0 %).
- ipcclt `pad_03_8061BD88_text`: es el relleno de alineacion de 16 de `ipcclt.o` (`-func_align 16`) tras
  `IPCSetBufferLo`; darselo a ipcMain.c (enlazada: end 0x8061BD90, ipcclt desde 0x8061BD90) da
  `main.dol: FAILED` (nuestro ipcMain.o no lo emite: el enlazador lo pone al alinear la unidad siguiente).
  Se queda dentro de ipcclt hasta que ipcclt enlace; entonces `.text start:0x8061BD90` (el enlazador alinea).

## Scripts (en /home/claude/scratch-W4/bin/)

- `strend.py`: `@stringBase0` cuyo `""` final queda fuera del `size` de symbols.txt (`addi rX, base, <size>`).
- `samediff.py <unidades>`: funciones con lineas DIFF de texto identico (la relocalizacion apunta a otro
  simbolo: tamanos, etiquetas, ceros de `.sdata`).
- `saall.sh`: tamanos de las funciones de SmallAllocator_8 frente al .dol (para el ciclo de inlining).

## Otras pruebas sin resultado (no repetir)

- ItemSlotAnime `fn_8044F434`: test de estados 3-5 con `!(a != 3 && ...)`, `switch`, `?:` encadenado e inline
  `IsSpinning_unk`: siempre `subi; cmplwi` (bloqueo conocido de tres valores).
- ItemSlotAnime: `SetTaskFrame_unk`/`SetTaskSpeed_unk` en los demas bucles de tareas: mismo codigo (sin cambios).
- LH_Binary `fn_804707D4`: bucle de pistas en una inline (por pistas o por el chunk): igual o peor.
- ControlStickByQuark `ControlThrustForCrab::vf0C` y `ControlCaughtSkelton::vf0C`: locales nuevos o quitados
  renumeran los `@N` de la unidad (labelcheck 30-50 que faltan); medir solo tras `pd.sh`.
- WordWrap_unk `fn_800E0F84` sin el local `buf`: 93,7 % y renumera.

- `gmk/ItemSlotAnime.h` `IsState56_unk`: con el bool puesto en un if, peor en ItemSlotAnime, DropItem y
  RandomItem; sin `const`, fn_8044EEC0 sube a 97,55 pero baja RandomItem fn_804348C4 (86,56 -> 86,36): por eso
  la inline local `IsShown_unk` (no const) solo en ItemSlotAnime.

## Pendientes

- `docs/escribir-20261116-qW4.patch`: SmallAllocator_8 `vf0C` <8> y <6> 87,0 -> 96,1 % (calcular la direccion
  devuelta antes de `mUsed++`, como el original). Sin bajadas (precommit: lost 0, fuzzdrop 0, labelcheck igual),
  pero no se commitea porque `ordercheck` de la unidad ya da 1 fuera de orden en HEAD (el ctor del gestor va
  antes de `Instance`; moverlo cambia el corte del ciclo de inlining y baja `~SmallAllocator<8>`): decidir si
  se acepta con esa deuda previa.

## Cambios de splits/symbols

- LH_Model: `@stringBase0` 0x80755F30 size 0x11 -> 0x12. Pendiente para su cola: EnemyTasha `@stringBase0`
  0x8074B0A4 size 0x2E0 -> 0x2E1 (`vf6C__10EnemyTashaFiP9CharaTask` tiene el mismo DIFF_REPLACE); script
  `/home/claude/scratch-W4/bin/strend.py` (lista los `@stringBase0` con `addi rX, base, <size>`).
- LayerOcclusionCPU (pd.sh + pdfix): `.sdata2` 0x80880F40-58 -> `@5970`..`@5973`, `@6189`..`@6191`; `@6438`
  -> `@6444` (0x80732E28); `@stringBase0` 0x80732E48 size 0xD -> 0xE (incluye el `""` final).
- ItemSlotAnime (pd.sh + pdfix.py): `lbl_8078F348` -> `__vt__Q23gmk17ItemSlotTextCache`, las 8 rutas
  `lbl_8078F400..4E0` -> `@STRING@Setup_unk__Q23gmk19ItemSlotAnimeBufferFv[@0-@6]`, `lbl_8087E018..24` ->
  `@STRING@Create_unk__Q23gmk17ItemSlotTextCacheFPQ23atn4Task[@0]` y `...ItemSlotAnimeBufferFPQ23atn4Task[@0]`;
  `@N` renumerados de .rodata/.sdata2 de la unidad (6 a mano con pdfix: 0x807547D0/F8, 0x80754818,
  0x8075488C/B0/BC).
- LH_Binary (pd.sh): `lbl_80755C20` -> `@5283`, `@4836` -> `@5020`, `lbl_80886EE0/EE4` -> `@5277`/`@5390`.
