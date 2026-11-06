# Siguiente sesión: propuestas de trabajo

## Estado actual (2026-11-06, rama `unidades-20261106` sin mergear: la revisa el usuario)

- `unidades-20261106` (desde `main` 9206c64, 12766): report **13524 / 18117** (lost 0, `main.dol: OK`, 652 de
  1304 unidades enlazadas; fuzzdrop solo lista dos símbolos `pad_03_*` de relleno que desaparecen al enlazar).
  Notas, decisiones a revisar, splits, renombrados y cambios de `libs/`: `docs/unidades-20261106.md`.
  - Enlazadas nuevas: `btm_inq.c`, `EXIBios.c` (`-O3`), 9 de `homebuttonLib/sound`, `THPDec.c`, `THPAudio.c`,
    `WUDHidHost.c`, `HBMController.cpp`, `HBMRemoteSpk.cpp`, `HBMCommon.cpp`, `HBMAxSound.cpp`, **AXFX completo**,
    **NHTTP completo**, SO (`SOBasic`, `SOInformation`, `SOOption`), MATH CRC, `ncdsystem.c`, `NETMD5.c`,
    `nw4r db_console`, de Swiftshine/key (CC0) **`WPAD.c` 2010 (98/98)**, `WPADEncrypt.c`, `WPADMem.c`, `lint.c`;
    de mscharged (CC0) `dwc_nonport.c`, `dwci_np_math.c`, `dwc_common.c`, **VF (16 ficheros)**; `dwci_encsession.c`;
    `CXStreamingUncompression.c` (desde el asm).
  - Sin enlazar: `bd.c`, `WPADHIDParser.c` (32/33), `WUD.c` 2010 (77/85), `HBMBase.cpp` (33/34: `draw`),
    `db_assert.cpp` (4/5, `VPanic` con asm), `CXUncompression.c` (3/4), **`dwc_match` (96/101, fuente R4QK01 de
    mscharged + cambios de 2010)**, **KPAD 2010** (`KPAD.c` 22/63 y `KPADMpls_unk.c` 6/40, structs rellenados).
- Fuentes externas útiles: Swiftshine/key (CC0, Kirby Epic Yarn, SDK de dic. 2009), mscharged-decomp (CC0: NHTTP,
  SO, DWC/VF/KPAD R4QK01 de mayo-junio de 2009), wii-ipl (CC0), pokemon-rumble-decomp (MIT).
  `robojumper/sdk_2009-12-11` no tiene licencia y `mkwcat/nsmbw-project` es MPL 2.0: sin usar, decide el usuario.

## Propuestas

1. KPAD 2010: arreglar el final de `KPADInsideStatus` (`accScaleReset` en 0x67E, campos nuevos 0x676-0x680,
   callback en 0x680) y portar `KPADInitEx`, `KPADiConnectCallback`, `KPADiRead`, `read_kpad_ext`,
   `read_kpad_acc`, `KPADiSamplingCallback`, callbacks de la balance board (sus `kp_wbc_*` cambian de sitio) y
   el resto de Motion Plus (`KPADMpls_unk.c`: los tres `fn_80659B20/C10/D00` por eje son iguales).
2. `dwc_match` (96/101): quedan 4 de registros (ver `mwcc-blockers.md`); para enlazar, quitar
   `DWC_IsValidCancelMatching`/`DWC_RegisterMatchStatus` (deadstrip) y cuadrar `.data`.
3. Mismo método (compilar el `.o` externo, emparejar por tamaño con `difflib`, renombrar, cambios 2010) para
   lo que quede con fuente en mscharged/KEY.
4. WUD.c 2010 (77/85), `WPADHIDParser::__parse_dpd_data` (98%), `HBMBase::draw` (98,8%), `CXUncompressAny` (84%).
5. Unidades del juego sin fuente con funciones pequeñas (NewCampTask, TacView, ShopBase, LastCocoonEgg...):
   empezar el fuente como `game/GameEventScript.cpp`.
6. TeamAI_unk (`std::multimap` y `Move_unk<P>`, ya con nombre en symbols).

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
