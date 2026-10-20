# Lote batch-auto-5kb - cola q9

Cola (de menor a mayor): `game/SetConfig_cont`, `game/GameTimer_cont`, `MP/SaveLoad_cont`, `atn/LH_Motion_cont`,
`game/NpcManager_cont`, `atn/Bgm_cont`, `game/MarketTask_cont`. Todas `_cont`: se fusionan en su unidad enlazada.

## game/SetConfig_cont (commit 382ae8c)

- Resultado: 4/4 al 100%, fusionada en `game/SetConfig` (`matching: true`, `main.dol: OK`).
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**
  - Splits: `game/SetConfig` .text 0x8057E83C-0x8057F0F8 -> 0x8057E83C-0x8057F280; borrada la entrada
    `game/SetConfig_cont` (contigua, sin datos). Confirmado que es el mismo fichero: usa los mismos campos
    (0x94 mSoundType, 0x98-0xA0 volumenes, 0xA4) y la misma logica que `Init_unk`.
  - `symbols.txt` (pairnames): `fn_8057F0F8/F154/F198/F1B4` -> metodos de `SetConfig`.
  - `include/game/SetConfig.h`: cuatro metodos nuevos y tipo inferido `SoundSettings_unk` (tipo de sonido + 3
    volumenes; `GameSaveData_unk` 0x803BD79C lo rellena y llama a 0x8057F1B4).
- Hallazgos MWCC: ninguno nuevo.

## game/GameTimer_cont (commit 87a8ab4)

- Resultado: 7/7 al 100%, fusionada en `game/GameTimer` (`matching: true`, `main.dol: OK`).
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**
  - Splits: `game/GameTimer` .text 0x803CE4B0-0x803CE688 -> 0x803CE4B0-0x803CE854; borrada `game/GameTimer_cont`
    (contigua, sin datos).
  - `symbols.txt` (pairnames): `fn_803CE688..fn_803CE7E0` -> metodos de `GameTimer` (arrancar, pausar/reanudar,
    parar, sumar, restar, cargar/guardar en las variables de juego 0x385/0x386 via `lbl_8087F430`).
  - `include/game/GameTimer.h`: siete metodos nuevos. Externos locales `fn_803761EC` (getter del valor),
    `fn_80370174`/`fn_80370320` (leer/escribir variable de juego; firmas como en `gmk/AttractHatePoint`).
- Hallazgos MWCC: ninguno nuevo.

## MP/SaveLoad_cont (commit 74ace86)

- Resultado: 4/4 al 100%, fusionada en `MP/SaveLoad` (`matching: true`, `main.dol: OK`).
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**
  - Splits: `MP/SaveLoad` .text 0x805020FC-0x80502708 -> 0x805020FC-0x805028D8; borrada `MP/SaveLoad_cont`
    (contigua, sin datos). Mismo fichero: usa la cola de peticiones de 0x64 y los campos 0x48/0x4C/0x50/0x54.
  - `symbols.txt` (pairnames): `fn_80502708` (toma los datos pendientes `lbl_8087F460` y el slot
    `lbl_8087DD60/64`), `fn_8050272C` (prepara el bufer de trabajo y encola una peticion tipo 2),
    `fn_80502848` (ocupado), `fn_80502870` (encola una peticion) -> metodos de `MP::SaveLoad`.
  - `include/MP/SaveLoad.h`:
    - `Request_unk::mType`/`m04` pasan a `int mType[2]` (el tipo se guarda dos veces) y los ctores asignan en el
      cuerpo: necesario para la copia muerta en pila de la peticion (ver hallazgo) y el orden de stores del ctor.
    - `Queue_unk::Push_unk(const T&)`; ctor `Request_unk(type, data, size)`.
    - `SaveWork_unk` partido en cabecera de 0xB8 (`m04` en 0x4) + `mData[0x134A8]` (mismo tamano total).
  - Externos locales en `SaveLoad.cpp`: `fn_8004B20C` (hora de calendario, `CalendarTime_unk` de 0x28 inferido
    como `OSCalendarTime`), `fn_803BE550/8C0/A08/A7C` (operaciones de `GameSaveData_unk`), `lbl_807C8508`
    (cabecera de 0xB8).
- Hallazgos MWCC
  - Un struct con un miembro array no se escalariza: construirlo como temporal y copiarlo deja sus stores muertos en
    pila (`stw` a `0x8(r1)`...) ademas de la copia propagada desde registros. Sin array, MWCC lo elimina entero.
  - `if (a == 0 || b == 0 || c < 0) return;`: la ultima condicion de la cadena `||` sale invertida con salto
    incondicional (`bge L; b fin`), las anteriores con `beq fin`.

## atn/LH_Motion_cont (commit 12fd05a)

- Resultado: 3/4 al 100%, **sin fusionar**: unidad propia `atn/LH_Motion_cont` con fuente, `matching: false`
  (`atn/LH_Motion` sigue `matching: true`; `main.dol: OK`).
- NONMATCHING: `fn_804766B0` (copia debil de `LoadManager_unk::Get_unk<LoadMotion>`): es la plantilla comun de
  los doce `atn::LH_*`, NONMATCHING en todos (~96%, solo reparto de registros; ver `include/atn/LoadManager.h`).
  Se instancia explicitamente en el `_cont` para que objdiff la tenga (no se empareja por nombre: el simbolo se
  queda `fn_804766B0`, como en las demas `LH_*`).
- Motivo de no fusionar: la primera funcion del tramo es esa plantilla (la emite `LH_Motion.cpp` tras
  `LH_Motion::vf0C`), asi que `LH_Motion` no puede quedar al 100% con el tramo fusionado. El fichero real es
  0x80476220-0x804768EC con el pool 0x80755F68 (mensajes de `std::string` + el `""` del `new`).
- **Decisiones sin el usuario**
  - Splits: sin cambios (el `_cont` ya tiene `.text` y su pool `.rodata` 0x80755F68-0x80755FC8).
  - `game_units.json`: nueva entrada `atn/LH_Motion_cont.cpp` (`matching: false`) tras `atn/LH_Motion.cpp`.
  - `symbols.txt` (pairnames): `fn_804768B4`/`fn_804768D0` -> `LH_Motion::fn_804768B4/D0` (devuelven `m5C`/`m60`
    de `LoadMotion`). `fn_80476884` ya tenia nombre.
  - Cabecera nueva compartida `include/atn/LoadMotion.h` (clase `LoadMotion` sacada de `LH_Motion.cpp`; se
    incluye la ultima en `LH_Motion.cpp`, mismo orden de clases/vtables, sigue al 100%).
  - `include/atn/LH_Motion.h`: `fn_804768B4`, `fn_804768D0`.
  - Pendiente para el usuario: fusionar cuando `Get_unk` llegue al 100% (o aceptar `LH_Motion` en `matching: false`).
- Hallazgos MWCC: ninguno nuevo.

## game/NpcManager_cont (commit db860c6)

- Resultado: 4/4 al 100%, fusionada en `game/NpcManager` (`matching: true`, `main.dol: OK`).
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**
  - Splits: `game/NpcManager` .text 0x805477C8-0x80547980 -> 0x805477C8-0x80547C28; borrada `game/NpcManager_cont`
    (contigua, sin datos). Mismo fichero: trabaja sobre los seis campos 0x5C-0x70 que inicializa el ctor de
    `NpcManager` y sobre su lista `mHead`.
  - `symbols.txt`: pairnames `fn_80547980/A20/A84/B54` -> metodos de `NpcManager` (crear los NPC fijos 1-6 salvo
    el 4, borrarlos, borrar los demas NPC, descargarlos devolviendo si alguno sigue cargando). Renombrado a mano
    `fn_80097A20` -> `fn_80097A20__9Model_unkFUi` (necesario para enlazar: lo llama `fn_80547B54` como metodo).
  - `include/game/NpcManager.h`: `m5C..m70` pasan a `NpcTask* mNpcs[6]` (el ctor los pone a 0 en el cuerpo; mismo
    codigo). `NpcManager.cpp` incluye ahora `game/NpcTask.h`.
  - `include/atn/Model_unk.h`: `m224[8]` se parte en `unsigned int mMotionCount` (0x224) + `m228[4]`; nuevo
    `fn_80097A20(unsigned int)` (libera el slot de motion).
  - El argumento `data` de `NpcTask::fn_80547C28` recibe aqui un entero (100001 + i * 100) pasado como `void*`.
- Hallazgos MWCC
  - Bucle sobre un array miembro con contador aparte (`T** p = a; for (i...; i++, p++)`) da `addi rX, this, off`
    + `lwz 0(rX)`; con indice (`a[i]`) sale `mr rX, this` + `lwz off(rX)`. La busqueda de 6 elementos se desenrolla.

## atn/Bgm_cont (commit 9333b72)

- Resultado: 6/6 al 100%, fusionada en `atn/Bgm` (`matching: true`, `main.dol: OK`).
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**
  - Splits: `atn/Bgm` .text 0x8004AD9C-0x8004AE84 -> 0x8004AD9C-0x8004B290 y gana el `.sdata2` 0x80880890-0x80880898
    (0.0f/1.0f) que tenia el `_cont`; borrada `atn/Bgm_cont` (contigua; los datos siguen el orden de `.text`).
  - `symbols.txt` (pairnames): `fn_8004AE84/AFAC/B0E4/B158/B1EC` -> metodos de `atn::Bgm` (arrancar BGM por nombre,
    variante con opcion de stream si `lbl_8087EFE8->m34A0`, preparando, reproducir con fade, parar con fade).
    `fn_8004B20C` (hora de calendario, `OSTicksToCalendarTime(OSGetTime())`) queda como `extern "C"` con su nombre
    (lo usa `MP/SaveLoad`, enlazada); no parece de `Bgm`, pero esta antes del corte de `.sdata2` del fichero.
  - `include/atn/Bgm.h`: cinco metodos nuevos. `include/atn/SoundRef.h`: `SoundObject` gana `m90` (0x90) y `mA8`
    (0xA8; `sizeof` pasa de 0xA8 a 0xAC, nadie lo usa).
  - **Fuera de mis unidades**: `src/game/GameBgm.cpp` (`matching: false`) llamaba a esas funciones como
    `extern "C"`; cambiado a llamadas de metodo para seguir emparejando las relocalizaciones. Comprobado: ninguna
    funcion de `GameBgm` cambia de porcentaje.
  - Nombre real propuesto del fichero: `atn_bgm.cpp` (pista de `splits-auto-A`), sin cambios.
- Hallazgos MWCC: ninguno nuevo.

## game/MarketTask_cont (commit f8a6cdf)

- Resultado: 6/6 al 100%, fusionada en `game/MarketTask` (`matching: true`, `main.dol: OK`).
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**
  - Splits: `game/MarketTask` .text 0x804A1DDC-0x804A24C4 -> 0x804A1DDC-0x804A29C4, `.sdata2` 0x808871B0-0x808871C0 ->
    0x808871B0-0x808871D0 (100.0f/3.0f/4.0f del `_cont`) y `.rodata` 0x807570C8 -> **0x807570C0** (absorbe el double
    int->float `lbl_807570C0` que estaba en un hueco `auto_`; dentro de la misma unidad no hay ciclo de orden de
    enlace). Borrada `game/MarketTask_cont`.
  - `symbols.txt`: pairnames `fn_804A24C4/251C/26DC/2724/2800/28F8` -> metodos de `MarketTask` (vende el item,
    precio actual, flag 251 % 11, precio medio comprado, registrar compra, borrar registro). **Tamano cambiado**:
    `lbl_808871C8` 0x8 -> 0x4 (float + relleno; con 8 objdiff lo trataba como double; regla de CLAUDE_lite).
  - `include/game/MarketTask.h`: seis metodos e inline `Find_unk(id)` (0x26E-0x270 comparten la entrada de 0x26D).
    `Field_unk` local gana `m10D0` (progreso de la historia: x3 entre 300000 y 400000, x4 desde 400000).
  - **Fuera de mis unidades**: `src/gmk/RandomMerchant.cpp` (`matching: false`) llamaba a `fn_804A24C4/251C` como
    `extern "C"`; cambiado a llamadas de metodo. Ningun porcentaje baja.
- Hallazgos MWCC
  - `total += f() * n;` da `mullw rD, rN, rRes` (operandos al reves); con el resultado en un local con nombre
    (`int p = f(); total += p * n;`) sale `mullw rD, rRes, rN`.
  - `x = f() + n` como argumento deja `mr r0, r3` y suma tras cargar el resto de argumentos; con `int c = f(); c += n;`
    la suma va directa al registro del argumento.
  - `(int)(a * b / c) / 10 * 10` reparte r0/r3/r4 distinto que guardando antes el `(int)` en un local.
