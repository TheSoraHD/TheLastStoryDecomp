# Siguiente sesión: propuestas de trabajo

## Estado actual (2026-11-06 08:30, rama `unidades-20261106` EN CURSO, sesión autónoma de 12 h)

- `unidades-20261106` (desde `main` 9206c64, 12766): report **13489 / 18117** a las 08:30 (lost 0, `main.dol: OK`;
  fuzzdrop solo lista dos símbolos `pad_03_*` de relleno que desaparecen al enlazar). Notas en
  `docs/unidades-20261106.md`. Punto de control 5 subido.
  - Enlazadas nuevas: `btm_inq.c`, `EXIBios.c` (`-O3`), 9 de `homebuttonLib/sound`, `THPDec.c`, `THPAudio.c`,
    `WUDHidHost.c`, `HBMController.cpp`, `HBMRemoteSpk.cpp`, `HBMCommon.cpp`, `HBMAxSound.cpp`, **AXFX completo**,
    **NHTTP completo**, SO (`SOBasic`, `SOInformation`, `SOOption`), MATH CRC, `ncdsystem.c`, `NETMD5.c`,
    `nw4r db_console`, de Swiftshine/key (CC0) **`WPAD.c` 2010 (98/98)**, `WPADEncrypt.c`, `WPADMem.c`, `lint.c`;
    de mscharged (CC0) `dwc_nonport.c`, `dwci_np_math.c`, `dwc_common.c` y **VF (16 ficheros, 25 funciones)**;
    `CXStreamingUncompression.c` (escrita desde el asm).
  - Sin enlazar: `bd.c`, `WPADHIDParser.c` (32/33), `WUD.c` 2010 (77/85), `HBMBase.cpp` 2010 (33/34: `draw`),
    `db_assert.cpp` (4/5, `VPanic` necesita asm), `CXUncompression.c` (3/4), **`dwc_match` (92/101, ahora con el
    fuente R4QK01 de mscharged + cambios de 2010)**.
- Fuentes externas útiles: Swiftshine/key (CC0, Kirby Epic Yarn, SDK de dic. 2009: WPAD/WUD/HBM casi iguales a
  TLS), mscharged-decomp (CC0: NHTTP, SO, DWC R4QK01 de mayo de 2009, VF, KPAD 2009), wii-ipl (CC0),
  pokemon-rumble-decomp (MIT). Buscar con `gh search code "<función>"`. `robojumper/sdk_2009-12-11` no tiene
  licencia: no usado.

## Propuestas

1. `dwc_match` (92/101): quedan `DWCi_ConnectToAnybodyAsync`, `DWCi_MatchProcess`, `DWCi_CloseMatching`?,
   `DWCi_SBUpdateAsync`, `DWCi_ProcessMatchCommand` (99,99), `DWCi_CancelMatchingProcess`,
   `DWCi_CloseUnacknowledgedClients`, `DWCi_NNCompletedCallback`: comparar con el asm (cambios de 2010 pequeños,
   como los ya hechos). Para enlazar: quitar `DWC_IsValidCancelMatching`/`DWC_RegisterMatchStatus` (deadstrip) y
   cuadrar `.data`.
2. Otros DWC con fuente R4QK01 en mscharged: `dwc_transport.c` (23/24), `dwc_main.c`, `dwc_init.c`,
   `dwc_auth_interface.c`: probar el mismo método (compilar el `.o` de MSC, emparejar por tamaño con
   `difflib`, renombrar en symbols y aplicar los cambios de 2010).
3. WUD.c 2010 (77/85): `VirginStandardSync`, `SyncHandler`, `DeleteDevice`, `fn_80670C70` (switch en árbol).
4. KPAD 2010 (0x80653EE0, 30 KB, 55 funciones): base posible el `KPAD.c` R4QK01 de mscharged.
5. `CXUncompressAny` (84%): colocación del `CXUncompressRL` inline.
6. TeamAI_unk (`std::multimap` y `Move_unk<P>`); unidades sin funciones al 100%.

## Bloqueos conocidos (no repetir sin idea nueva)

- `push_back<boost::shared_ptr>` y el crecimiento de `vector` no trivial.
- MapManager fn_80491D50/DF8; matriz de reflejo de `atn::Scene`; MapTask::fn_804A0F80 (presupuesto de inline).
- `IpcReplyHandler`: `switch` en árbol en el original.
- `pdbatch.sh` renombra `lbl_8087D6E8/EC`, `lbl_80756700`, `lbl_8087EE78`, a veces `0x8087DB00/04`, `__vt__` sobre
  cadenas: devolverlos a mano. `.sdata2` ambiguas: `tools/decomp/sdata2pair.py <unidad>`.
- `oursyms.py` (scratch) aborta si fuera a poner nombres `__vt__`/`__RTTI__`: las vtables débiles chocan con las
  unidades enlazadas.
- Tras borrar `build/SLSP01/obj` hay que borrar también `build/SLSP01/config.json`.
- `configure.py` sin flags intenta descargar los compiladores: usar la línea de `docs/entorno-nube.md`.
- `odiff2.py` no recompila: compilar la unidad (o `fnstat.py`) antes.
- HBMBase: objdiff da 99,8% en los `lfs` contra `lbl_80764198` aunque el pool sea idéntico.
- Unidad enlazada seguida de asm: poner `align:16` en el `.text` de la siguiente (se pierde el relleno). Para
  la `.bss`, `align:` en splits no vale: la alineación sale del `.o`.
- Al renombrar un `fn_` en symbols.txt, renombrarlo también en **todos** los fuentes (incluidos `src/` del juego),
  o el enlace falla (`main.elf`).
