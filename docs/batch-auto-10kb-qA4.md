# batch-auto-10kb qA4: atn+game

Cola: `atn/LH_Binary`, `atn/THPPlayer_unk`, `atn/Xsca_unk`, `game/SongManager`, `game/TeamAIManager`.

## atn/LH_Binary (commit 70aaa17)
- Resultado: 13/19 al 100%, `matching: false` (no enlazable: dos funciones sin decompilar), `main.dol: OK`.
- NONMATCHING:
  - `Get_unk<LoadCore>` (0x80470604) 98.5% - la plantilla compartida de `include/atn/LoadManager.h`, mismo
    bloqueo de registros que en las otras doce copias.
  - `fn_804707D4` ('anmn') 99.07%, `fn_80470A54` ('locs'/'loct') 98.78%, `fn_804716B8` (switch de chunks)
    99.88%: solo asignacion de registros en un bucle.
  - `fn_80471FD0` (paired-single a mano: fres/ps_madds/ps_sum0) y `SceneTrack_unk::fn_8047202C` (mtspr GQR6 +
    psq_l cuantizados): sin decompilar, no se pueden escribir sin asm (MWCC 1.3 no tiene `__psq_l`).
- **Decisiones sin el usuario**
  - Splits: sin cambios.
  - `symbols.txt`: pairnames (Get_unk<LoadCore> y las 10 funciones de chunks `fn_XXXXXXXX__FP10Loader_unk...`),
    `lbl_8078FBE0` -> `@4611` (ptmf `safe_bool` de `boost::function`, 0xC, local), `lbl_80755C18` ->
    `@stringBase0` (pool `""` de `new (2, "", "", 0)`).
  - `fn_804714A4`/`fn_804714B0` se mantienen `extern "C"` (los llaman LH_Texture/LH_Scene/LH_Model/LH_Motion).
  - Tipos inventados locales: `BinChunk_unk` (cabecera de chunk de 16 bytes: tag, version, ?, tamano) y una
    estructura por chunk (`BinModel_unk`, `BinMesh_unk`, `BinLocs_unk`...). `LH_Binary::vf0C` usa
    `Get_unk<LoadCore>` (el original construye un `LoadCore` de 0x5C sin subclase).
  - `((void)0)` (asserts de release) en `fn_80470C20`/`fn_80470FA4`/`fn_80471078`/`fn_804711B0`: sin ellos se
    autoinlinean en `fn_804716B8` y el original los llama.
  - Nombre real propuesto: sin pruebas (el fichero contiene LH_Binary + el "fixup" de chunks del cargador:
    fn_804714B0 recorre los chunks, los reubica y llama al callback; al acabar escribe 'fixd' en la cabecera).
- Hallazgos MWCC
  - Un bucle de 15 iteraciones con cuerpo pequeno se desenrolla x3 con un contador muerto que avanza de 2 en 2;
    17 iteraciones no se desenrollan.
  - Funciones de 3-4 sentencias con un inline de 5 sentencias dentro necesitan ~12 `((void)0)` para dejar de
    autoinlinearse; con un bucle, 6-8.
- Pendientes: ninguno.

## atn/THPPlayer_unk (commit 48e71f6)
- Resultado: 18/22 al 100%, `matching: false`, `main.dol: OK`.
- NONMATCHING: `fn_805B5AD4` (THPSimpleSetBuffer) 95.74% (un registro cero de mas), `fn_805B602C`
  (THPSimpleLoadStop) 89.26% (planificacion de cargas y r6/r7), `fn_805B6120` (THPSimpleDecode) 99.81% y
  `fn_805B6504` (MixAudio) 97.62%: solo registros.
- **Decisiones sin el usuario**
  - Splits: sin cambios.
  - Es `THPSimple.c` del ejemplo "thpsimple" del SDK THP (sin hilos), con un campo extra `soundMode` (0x188) y
    una funcion nueva `fn_805B6C44` (estereo/mono). Funciones publicas con su nombre automatico `extern "C"`
    (las llama `atn/VideoTHP.cpp`; los nombres del SDK van en comentarios); las tres `static` del SDK
    (ReadFrameAsyncCallback, MixAudio, THPAudioMixCallback) tambien conservan su `fn_`.
  - `symbols.txt`: datos con nombres del SDK (`SimpleControl`, `WorkBuffer`, `SoundBuffer`, `Initialized`,
    `SoundBufferIndex`, `OldAIDCallback`, `LastAudioBuffer`, `CurAudioBuffer`, `AudioSystem`, `VolumeTable`,
    todos `scope:local`), `lbl_80763DE0` -> `@stringBase0` ("THP"), constantes `@938` (doble int->float),
    `@638` (127.0f), `@918` (0.0f).
  - Tipos del SDK declarados localmente (THPHeader, THPFrameCompInfo, THPVideoInfo, THPAudioInfo, THPSimple...).
  - Nombre real propuesto: `THPSimple.c` (o una copia `.cpp` del juego).
- Hallazgos MWCC
  - `__attribute__((aligned(32)))` en una variable global grande desactiva optimizaciones en las funciones que
    la usan: `x + 1 >= 10 ? 0 : x + 1` deja de salir sin saltos (addc/subfe/andc) y los bucles de 10
    iteraciones dejan de desenrollarse del todo. Sin el atributo, todo casa (la alineacion la da la variable
    siguiente).
  - `x + 1 >= N ? 0 : x + 1` da la secuencia `xoris/addc/subfe/andc`; `x + 1 < N ? x + 1 : 0` da
    `subfc/srwi/subfe/and`.
  - `n = 48; if (f() == 0) n = 32;` deja el `li` antes de la llamada; con `u32 r = f(); n = 48; if (r == 0)`
    sale despues, como en el original.
- Pendientes: ninguno.

## atn/Xsca_unk (commit 35b43a2)
- Resultado: 4/6 al 100%, `matching: false`, `main.dol: OK`.
- NONMATCHING: `fn_805BD4B8` 99.09% (push_back inline: carga capacidad antes que tamano, como en
  `std/vector.h`; dos temporales del crecimiento intercambiados en pila; registros de los pasos del bucle),
  `fn_805BE37C` 97.02% (planificacion de los tres `stwbrx` de floats).
- **Decisiones sin el usuario**
  - Splits: sin cambios. **Pendiente (corte con `atn/XsfParser_unk`)**: `fn_805BE8FC`/`fn_805BEA34` reciben el
    `Xsca_unk` como primer argumento y solo los llaman `fn_805BDE48`/`fn_805BE37C`: probablemente son metodos de
    Xsca_unk y el corte real estaria en 0x805BEACC (ctor de XsfFile_unk). No se ha movido (unidad ajena); aqui se
    llaman como `extern "C"`.
  - `symbols.txt`: metodos de Xsca_unk (`fn_805BD4B8`, `fn_805BDCBC`, `fn_805BDE48`, `fn_805BE37C`); en el tramo de
    XsfParser_unk (sin llamadores en C++): `fn_805BEEC4` -> `__ct__15XsfCurveSet_unkFPCc`, `fn_805BF164` ->
    `__dt__15XsfCurveSet_unkFv`, `fn_805BF660` -> `fn_805BF660__15XsfCurveSet_unkFv`; datos `lbl_80764040` ->
    `@stringBase0` (0x6E), `lbl_807979E8` -> `@STRING@__allocate__Q23std23vector<13XscaEntry_unk>FUi`.
  - Cabecera compartida `include/std/vector.h` (solo anadidos): `vector::__reserve_checked_unk(n)` (reserve con
    comprobacion de longitud y bufer crudo) y `vector::__check_length_unk(n)` fuera de clase (mensaje al pool del
    fichero). regress: lost 0.
  - Tipos inventados locales: `XscaEntry_unk` {chunk, curvas}, especializaciones locales de
    `std::__vec_imp_unk`/`__vec_split_unk` para `XscaEntry_unk` (se guardan como punteros: tamano primero, sin
    comprobar null), `ScopedArray_unk<std::string>` (como `boost::scoped_array`: `reset` + `delete[]`),
    `XsfVec_unk` (3 floats). Redeclaradas localmente `XsfFile_unk`, `XsfIter_unk`, `XsfCurveSet_unk`.
  - Instanciacion explicita `template void std::string::reserve(unsigned long);` para que su mensaje entre en
    el pool en su sitio (copia debil que el enlazador descarta).
  - Nombre real propuesto: sin pruebas.
- Hallazgos MWCC
  - `for (int j = 0; j < n; j++)` con `n` unsigned da `cmplwi n,0; ble` + `srwi.` (bucle de punteros);
    con `unsigned j`, `cmpwi n,0; beq` + `cmplwi n,8` (bucle indexado).
  - Los locales de un helper inline quedan por debajo de los temporales de otro inline (orden de pila): el codigo
    de nombres de fn_805BD4B8 estaba en dos helpers inline.
  - Tres floats copiados a la pila salen en el orden del original declarando los locales al reves
    (`float z; float y; float x;`). Un `p + 4` intermedio en otra sentencia (`str = data + 4; str += align`)
    evita que el +4 se pliegue en el desplazamiento de las cargas.
- Pendientes: el corte Xsca_unk/XsfParser_unk (ver arriba).

## game/SongManager (commit d5a7727)
- Resultado: 7/13 al 100%, `matching: false`, `main.dol: OK`.
- NONMATCHING: creador `fn_805A2240` 92.19% (orden de creacion de 1.0f/0.0f en el ctor inline y un `frsp` de
  0.1f), `vf10` 99.02% (pila/registros de los temporales de la interpolacion de Vector3), `fn_805A2FD4` 97.88% y
  `fn_805A32C8` 68.12% (el original carga el 1.0f del volumen de `GameBgm::fn_8037D4C0` antes de las ramas del
  `GetFade` inline; objdiff alinea mal), `fn_805A3448` 95.96% (1.0f al inicio), `fn_805A38F0` 99.77% (r3/r4 en
  `type == 8`).
- **Decisiones sin el usuario**
  - Splits: sin cambios. `fn_805A38F0` (la llama GameBgm como `extern "C"`) sigue en esta unidad; no hay pruebas de
    que sea de SongManager.cpp (no usa nada de la clase).
  - `symbols.txt`: pairnames de los metodos (`fn_805A2240` creador estatico, `fn_805A2404` estatico de 3 ints,
    dtor, `vf10`, `vf14`, setters), `lbl_8087F9F0` -> `sInstance__11SongManager` (0x4), datos `sSongs` (tabla de
    canciones, local), `sSongDistance_unk`/`sSongMinVolume_unk`/`sSongMaxVolume_unk` (.sdata 300/0.1/1.0, globales:
    el ctor los relee), `lbl_8076328C` -> `@stringBase0`, constantes `.sdata2` -> `@1894..@2137`, doble `@2338`.
  - Clase `SongManager` local en el .cpp (nadie la usa desde C++): `atn::Task` + estado, cancion, 4 interpoladores
    (`SongInterp_unk<T>`: tiempo/duracion/velocidad/activo + from/to/cur/delta; nombre inventado) y `atn::Bgm*`.
    Vistas locales de los globales: `SongEnv_unk` (lbl_8087F418, con `Lower_unk`/`Restore_unk` inline),
    `SongSoundCfg_unk`, `SongParty_unk`/`SongChara_unk`, `SongBattle_unk`, `SongLimit_unk`.
  - `operator*(float, const atn::Vector3&)` local (no esta en math/Vec.h): `return atn::Vector3(v * f);` da los
    temporales del original.
  - Nombre real propuesto: SongManager.cpp (el nombre de la clase esta en el DOL).
- Hallazgos MWCC
  - Un metodo inline `const` (`GetFade_unk() const`) evita que el compilador reutilice la comparacion de un
    miembro ya probado (`bne cr1`) y lo relee, como el original.
  - Un parametro que el inline guarda en un miembro y luego lee del miembro (`mF0 = x; ...if (mF0)`) no se
    pliega como constante en las llamadoras: se comprueba en tiempo de ejecucion (`cmpwi r3,0` con r3 = 0).
  - Probar un puntero como palabra (`*(const int*)&ref != 0`) y usarlo despues como puntero obliga a recargarlo
    (el original hace `lwz r0,8(r3)` para el test y `lwz r3,8(r3)` para la llamada).
  - `a = (b - c) * t; r = c + a;` en dos sentencias evita el `fmadds` (con `fp_contract on`).
- Pendientes: ninguno.

## game/TeamAIManager (commit ab61461)
- Resultado: 7/17 al 100%, `matching: false`, `main.dol: OK`.
- NONMATCHING: `Rebuild_unk` 98.87% (registros del bucle que enlaza los equipos y orden de las variables de
  induccion), los manejadores de mensajes `OnTarget_unk`/`OnTarget2_unk` 91.24%, `OnMsg20_unk` 98.85%,
  `OnParent_unk` 96.89%, `OnMembers_unk` 99.04%, `OnMsg21_unk`/`OnMsg24_unk` 98.75%, `OnChara_unk` 99.25% (todos por
  la asignacion de registros del `Find_unk` inline: en el original el indice va en el registro mas bajo y tipo/id
  despues; no lo consegui con ~15 variantes), `vf14` 98.39% (x/z intercambiados en f29/f30 y orden de operandos
  del `cmpw` del buffer circular).
- **Decisiones sin el usuario**
  - Splits: `.sbss` de la unidad empieza en 0x8087FA00 (antes 0x8087FA04): la instancia `lbl_8087FA00` estaba en un
    hueco sin unidad y la define este .cpp (va antes de `lbl_8087FA04`, como en el DOL).
  - `lbl_8087FA00`/`lbl_8087FA04` conservan el nombre (`extern "C"`): `AI/AiCommon_unk.cpp` y
    `game/PlayerManager.cpp` usan `lbl_8087FA00` con sus propios tipos.
  - `symbols.txt`: metodos con nombres inventados (`Create_unk`, `Rebuild_unk`, `IsTop_unk`, `On*_unk`,
    `Find_unk`), `vf0C/vf10/vf14`, dtor, ctor inline `__ct__10TeamAI_unkFv` (0x805B37F8). Datos: cadenas SJIS de
    estados `@891..@895`, tabla `sStateNames`, nombre RTTI `@1015`, `@stringBase0`, lista RTTI base `@1016`,
    `@STRING@Resize_unk__Q23atn24Buffer_unk<10TeamAI_unk>Fii(@0)` (.sdata) y `.sdata2` `@2085..@2096`.
  - Clase `TeamAIManager` local en el .cpp (`atn::Task`, `atn::Buffer_unk<TeamAI_unk>` de 0x37C, nodo raiz de un
    arbol de equipos, `ResHandle` "TeamAI", modo debug, 5 `atn::LH_Binary`). Vista local `TeamAI_unk` (clase de
    game/TeamAI_unk.cpp: nodo, 2 buffers circulares de 32, bytes de estado, miembros en 0x250) y
    `TeamTable_unk` (tablas de lbl_8087F430->m10D8: enemigos/amigos de 0x90 en 0x148/0x150, 3 listas de objetivos
    de 0x94 en 0x158).
  - El constructor de TeamAIManager no es inline: se define antes de `Create_unk` (se autoinlinea y el enlazador lo
    elimina) para que "TeamAI"/"DebugMode" vayan al principio del pool. Dos funciones eliminadas reconstruidas
    (`TeamDefault_unk`, `TeamName_unk`) con los literales "team_default\0" y "ai_team%03d\0" (el cero extra es el
    "" que sigue a cada uno en el pool).
  - Una tabla `static` sin uso (`sWaitName_unk`) con el literal del segundo estado antes de `sStateNames`: en el
    DOL ese literal va primero en .rodata.
- Hallazgos MWCC
  - Un literal en el inicializador de una tabla `static const` sin uso no se emite, pero fija el orden del objeto
    del literal cuando otra tabla lo reutiliza despues.
  - Reutilizar la variable del total como limite del primer bucle (`for (i = 0; i < (count = t->n); i++)` y luego
    `count + i`) da la carga del original en el registro de `count` y evita recargar el limite en el 2o bucle.
  - Para el cursor vertical de un HUD, reutilizar la variable `y` del rectangulo (y no otra local) da los registros
    del original para las columnas.
  - Una comparacion `int` de un `unsigned char` en if/else (`int type = t->mType; if (type == 1)`) da `cmpwi`; el
    `switch` sobre el byte se compila igual que la cadena pero `t->mType == 1` da `cmplwi`.
  - `int n = mSize; if (++sel >= n)` da el orden de registros del original (`sel` en r0, `n` en r4).
- Pendientes para otras colas: game/TeamAI_unk.cpp (0x805AE26C-0x805B3420) usa la misma clase `TeamAI_unk`; si
  se descompila, la vista local de este .cpp deberia pasar a un header compartido.
