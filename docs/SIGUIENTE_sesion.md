# Siguiente sesión: propuestas de trabajo

## Estado actual (2026-11-06 05:00, rama `unidades-20261106` EN CURSO, sesión autónoma de 12 h)

- `unidades-20261106` (desde `main` 9206c64, 12766): report **13279 / 18117** a las 05:00 (lost 0, `main.dol: OK`;
  fuzzdrop solo lista dos símbolos `pad_03_*` de relleno que desaparecen al enlazar). Notas en
  `docs/unidades-20261106.md`. Punto de control 3 subido (3d231fd).
  - Enlazadas nuevas: `btm_inq.c`, `EXIBios.c` (`-O3`), 9 de `homebuttonLib/sound`, `THPDec.c`, `THPAudio.c`,
    `WUDHidHost.c`, `HBMController.cpp`, `HBMRemoteSpk.cpp`, `HBMCommon.cpp`, **AXFX completo** (8 ficheros,
    3 `Dpl2` escritos adaptando los de 3 canales), **NHTTP completo** (11 ficheros de mscharged-decomp, rama
    `VERSION_R4QK01`), `nw4r db_console`.
  - Sin enlazar: `bd.c`, `WUD.c` 2010 (50/85), `WPAD.c` 2010 (84/89), `HBMBase.cpp` 2010 (22/45, el resto casi
    todo al 99,8-99,9% por etiquetas), `db_assert.cpp` (4/5, `VPanic` necesita asm).
- Fuentes externas útiles: mscharged-decomp (CC0: NHTTP, KPAD/WPAD/WUD de 2007), wii-ipl (CC0: SDK de 2010 con
  `wpad`, `wud`, `axfx`), pokemon-rumble-decomp (MIT: axfx). Buscar con `gh search code "<función>"`.

## Propuestas

1. WPAD 2010: comparar con `wii-ipl/libs/RVL_SDK/src/wpad` (Wii Menu 4.3, 2010) las 5 que faltan
   (`IsControllerDataChanged`, `WPADiManageHandler`, Motion Plus `fn_806649C0`/`fn_80665B30`, `CheckButtonCombination`).
   Idem `wud` para WUD.c 2010.
2. HBMBase 2010: `calc` (86%); el resto está bloqueado por la etiqueta `lbl_80764198` de `.rodata`.
3. Unificar `revolution/nhttp/NHTTPi.h` con `private/nhttp.h` (d_nhttp.c y NHTTP_os_RVL.c).
4. KPAD 2010 y WPADHIDParser 2010 (0x80653EE0, 31 KB; 0x80666860, 32 KB).
5. Tramo 0x807230C0-0x80724BE0 (texto de depuración char/wchar_t con `MEMExpHeap`, `ut::List`, `CharWriter`):
   sin fuente conocida.
6. TeamAI_unk (`std::multimap` y `Move_unk<P>`); unidades sin funciones al 100%.

## Bloqueos conocidos (no repetir sin idea nueva)

- `push_back<boost::shared_ptr>` y el crecimiento de `vector` no trivial.
- MapManager fn_80491D50/DF8; matriz de reflejo de `atn::Scene`; MapTask::fn_804A0F80 (presupuesto de inline).
- `IpcReplyHandler`: `switch` en árbol en el original.
- `pdbatch.sh` renombra `lbl_8087D6E8/EC`, `lbl_80756700`, `lbl_8087EE78`, a veces `0x8087DB00/04`, `__vt__` sobre
  cadenas: devolverlos a mano. `.sdata2` ambiguas: `tools/decomp/sdata2pair.py <unidad>`.
- Tras borrar `build/SLSP01/obj` hay que borrar también `build/SLSP01/config.json`.
- `configure.py` sin flags intenta descargar los compiladores: usar la línea de `docs/entorno-nube.md`.
- `odiff2.py` no recompila: compilar la unidad (o `fnstat.py`) antes.
- HBMBase: objdiff da 99,8% en los `lfs` contra `lbl_80764198` aunque el pool sea idéntico (cambiar nombre o
  tamaño no ayuda; tamaño 0x380 rompe dtk).
