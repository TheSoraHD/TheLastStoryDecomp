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
