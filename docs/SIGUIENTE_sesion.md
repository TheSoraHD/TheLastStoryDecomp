# Siguiente sesión: propuestas de trabajo

## Estado actual (2026-11-06 07:00, rama `unidades-20261106` EN CURSO, sesión autónoma de 12 h)

- `unidades-20261106` (desde `main` 9206c64, 12766): report **13392 / 18117** a las 07:00 (lost 0, `main.dol: OK`;
  fuzzdrop solo lista dos símbolos `pad_03_*` de relleno que desaparecen al enlazar). Notas en
  `docs/unidades-20261106.md`. Punto de control 4 subido.
  - Enlazadas nuevas: `btm_inq.c`, `EXIBios.c` (`-O3`), 9 de `homebuttonLib/sound`, `THPDec.c`, `THPAudio.c`,
    `WUDHidHost.c`, `HBMController.cpp`, `HBMRemoteSpk.cpp`, `HBMCommon.cpp`, `HBMAxSound.cpp`, **AXFX completo**,
    **NHTTP completo**, SO (`SOBasic`, `SOInformation`, `SOOption`), MATH CRC, `ncdsystem.c`, `NETMD5.c`,
    `nw4r db_console`, y de Swiftshine/key (CC0): **`WPAD.c` 2010 (98/98)**, `WPADEncrypt.c`, `WPADMem.c`, `lint.c`.
  - Sin enlazar: `bd.c`, `WPADHIDParser.c` (32/33: `__parse_dpd_data` 98%), `WUD.c` 2010 (57/85),
    `HBMBase.cpp` 2010 (32/34), `db_assert.cpp` (4/5, `VPanic` necesita asm), `dwc_match` (59/101).
- Fuentes externas útiles: Swiftshine/key (CC0, Kirby Epic Yarn, SDK de dic. 2009: WPAD/WUD/HBM casi iguales a
  TLS), mscharged-decomp (CC0: NHTTP, SO, DWC R4QK01 de mayo de 2009, KPAD 2009), wii-ipl (CC0),
  pokemon-rumble-decomp (MIT). Buscar con `gh search code "<función>"`.

## Propuestas

1. WUD.c 2010: seguir portando desde `ext/key/src/revolution/WPAD/WUD.c` (no compila entero; usarlo por función):
   `__wudGetDevInfoFromWiiFit` (`fn_80670C70`, con `fn_80670BD0`/`fn_80670C50` y la cadena "Error: the workarea..."),
   `_initWBC` en `WUDInit`, `InitCore`, el orden del `.sbss` (KEY: `_wudNandBufPtr` 0x808802B0, `_initWBC` 0x808802B4).
   Con eso el `.data` y el `.sbss` quedan como en TLS.
2. `WPADHIDParser`: solo falta `__parse_dpd_data` (98%, registro r22 en el bucle de `_wpadRolag`) para enlazarla.
3. KPAD 2010 (0x80653EE0, 30 KB, 55 funciones): `KPADInsideStatus` de 0x688; base posible el `KPAD.c` R4QK01 de
   mscharged (CC0), ~15 funciones del mismo tamaño.
4. HBMBase 2010: `calc` (86%) y `draw` (98,8%).
5. Tramo 0x807230C0-0x80724BE0 (texto de depuración char/wchar_t con `MEMExpHeap`, `ut::List`, `CharWriter`):
   sin fuente conocida.
6. TeamAI_unk (`std::multimap` y `Move_unk<P>`); unidades sin funciones al 100%.

## Bloqueos conocidos (no repetir sin idea nueva)

- `push_back<boost::shared_ptr>` y el crecimiento de `vector` no trivial.
- MapManager fn_80491D50/DF8; matriz de reflejo de `atn::Scene`; MapTask::fn_804A0F80 (presupuesto de inline).
- `IpcReplyHandler`: `switch` en árbol en el original.
- `dwc_match`: el reparto de registros de las ~50 copias inline de `DWCi_SetMatchStatus` (probadas también la
  forma de MSC y el orden de locales).
- `pdbatch.sh` renombra `lbl_8087D6E8/EC`, `lbl_80756700`, `lbl_8087EE78`, a veces `0x8087DB00/04`, `__vt__` sobre
  cadenas: devolverlos a mano. `.sdata2` ambiguas: `tools/decomp/sdata2pair.py <unidad>`.
- `oursyms.py` (scratch) aborta si fuera a poner nombres `__vt__`/`__RTTI__`: las vtables débiles chocan con las
  unidades enlazadas.
- Tras borrar `build/SLSP01/obj` hay que borrar también `build/SLSP01/config.json`.
- `configure.py` sin flags intenta descargar los compiladores: usar la línea de `docs/entorno-nube.md`.
- `odiff2.py` no recompila: compilar la unidad (o `fnstat.py`) antes.
- HBMBase: objdiff da 99,8% en los `lfs` contra `lbl_80764198` aunque el pool sea idéntico.
- Unidad enlazada seguida de asm: poner `align:16` en el `.text` de la siguiente (se pierde el relleno).
