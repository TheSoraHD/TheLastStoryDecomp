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
