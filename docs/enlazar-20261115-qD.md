# enlazar-20261115, cola D (datos y librerias)

Inicio 13:22 (reinicios del servidor a las 13:43 y ~14:05). Referencia: `/home/claude/ref-D` (15550 funciones).

## Resultado por unidad

| Unidad | Antes | Despues | Nota |
|---|---|---|---|
| `RVL_SDK/.../ax/AXComp.c` (nueva, solo datos) | - | enlaza | tramo `auto_07_807AC6A0_data` (1/2) |
| `RVL_SDK/.../ax/DSPCode.c` (nueva, solo datos) | - | enlaza | tramo `auto_07_807AC6A0_data` (2/2) + `auto_09_8087E850_sdata` |
| MSL_C `ctype.c`, `errno.c`, `locale.c`, `float.c`, `wctype.c` (nuevas) | - | enlazan | 5 tramos `auto_` (.rodata/.data/.sdata/.sdata2/.sbss) |
| BTE `bta_dm_cfg.c`, `bta_hh_cfg.c`, `bta_sys_cfg.c` (nuevas) | - | enlazan | 4 tramos (`auto_06_80764C58_rodata`, `auto_09_8087EA88_sdata`, `auto_11_80888850_sdata2`, `auto_12_808893A0_sbss2`) |
| `RVL_SDK/.../ipc/ipcProfile.c` | 5/6 | 6/6, enlaza | `AddReqInfo` 99,65 -> 100 % |
| `key/.../WPAD/WPADHIDParser.c` | 32/33 | 33/33, **no enlaza** | `__parse_dpd_data` 98,1 -> 100 %; ver NONMATCHING/Pendientes |
| `ipcclt.c` | 20/23 | igual | `IpcReplyHandler` bloqueada (mwcc-blockers), `IOS_IoctlvReboot` 95 % sin tocar |
| `NETAES.c` | 7/9 | igual | 2 funciones AES al 62 %, ya muy probadas; no repetido |
| `bte/bd.c` | 2/2 | **no enlaza** | ver Hallazgos |

Tramos `auto_*` de datos: 77 -> 49 (17 KB -> 924 bytes).

## Unidades que enlazan (bytes)

`complete_data` del `report.json`: 965894 -> 996966 (**+31072**).

- AXComp 4032, DSPCode 8200, ipcProfile 14984
- MSL_C: ctype 1024, locale 792, wctype 1536, float 16, errno 8
- BTE: bta_dm_cfg 176, bta_hh_cfg 32, bta_sys_cfg 16
- Unidades ya enlazadas que absorben tramos: FriendArganan +64, Fireworks +40, GameLoadTask +16, MotionCommandCommon +8, NpcManager +8, GameSystem_unk +8, ptr_string_base_wchar_t_0 +8, FriendManager +8, OSError +8, OSPlayTime +16, dvdfs +16, dvdqueue +16, snd_SoundPlayer +16, snd_SoundThread +24.

Funciones al 100 %: 15550 -> 15553 (`AddReqInfo`, `__parse_dpd_data`, mas una de recuento de objdiff).

## NONMATCHING con motivo

- `WPADHIDParser` enlazaria (33/33) pero el `.dol` difiere: el `.bss` del original mide 0x60 (5 arrays de 0x10 + 0x10 sin simbolo, 0x8082DDF0-0x8082DE00) y el nuestro 0x50; `_wcb` de WUD sale en 0x8082DDF0. El `.bss` de WUD.o tiene alineacion 32 pero el enlazador no la respeta. Se queda `matching: false`.
- `bd.c`: igual que antes (`.sbss2` 0xE frente a 0x10). Ahora el primer objeto de `.sbss2` es `bta_dm_pm_cfg` compilado (3 bytes, de `bta_dm_cfg.c`), y sigue saliendo 0xE. Probado: `.sbss2` de `bd_addr_null` con GC/3.0a3, 3.0a5.2, 2.6, Wii/1.0 y 1.1: siempre 6.
- `IOS_IoctlvReboot`, `IpcReplyHandler`, `NETAES fn_80699610/fn_80699888`: sin cambios (ver mwcc-blockers y comentarios del fuente).

## Cambios de splits

- `ax/AXComp.c` (.data 0x807AC6A0-0x807AD660) y `ax/DSPCode.c` (.data 0x807AD660-0x807AF660, .sdata 0x8087E850-0x8087E858) tras AXVPB.c. Prueba: `__AXCompressorDefaultTable` y `axDspSlave`/`axDspInitVector`/`axDspResumeVector`/`axDspSlaveLength` en el orden de los fuentes del SDK; tamanos 0xFC0/0x2000 iguales.
- MSL_C: `ctype.c` (.rodata 0x80765338-0x80765738, tras char_io), `errno.c` (.sbss 0x80880348-0x80880350, tras direct_io), `locale.c` (.rodata 0x80765738-0x80765840, .data 0x807BB1D0-0x807BB3C8, .sdata2 0x80888A98-0x80888AB0, tras FILE_POS), `float.c` (.sdata 0x8087EC00-0x8087EC10, tras string), `wctype.c` (.rodata 0x807658A0-0x80765EA0, tras strtoul). Prueba: tablas y simbolos (`__lconv`, `_current_locale`, `__float_nan`...) y orden de secciones entre las unidades vecinas.
- BTE tras `uusb_ppc.c`: `bta_dm_cfg.c` (.rodata 0x80764C58-0x80764CE8, .sdata 0x8087EA88-0x8087EAA0, .sbss2 0x808893A0-0x808893A8), `bta_hh_cfg.c` (.rodata 0x80764CE8-0x80764CF8, .sdata 0x8087EAA0-0x8087EAB0), `bta_sys_cfg.c` (.sdata 0x8087EAB0-0x8087EAB8, .sdata2 0x80888850-0x80888858). Prueba: los punteros `p_bta_*` de .sdata apuntan a esas tablas (p. ej. `p_bta_dm_pm_cfg` = 0x808893A0); tamanos de los `.o` compilados.
- Huecos de alineado sin simbolos al final de la unidad anterior: OSError .bss ->0x807CB300, OSPlayTime .bss ->0x807CC380, dvdfs .sbss ->0x8087FD00, dvdqueue .bss ->0x807D1060, snd_SoundPlayer .bss ->0x808638E0, snd_SoundThread .bss ->0x808795C0.
- `game/GameLoadTask` .sdata 0x8087DD60- y .sbss -0x8087F468 (define `lbl_8087DD60/64 = -1` y `lbl_8087F460`); `db/MotionCommandCommon` .sbss 0x8087F2A8-0x8087F2B0, `game/NpcManager` .sbss 0x8087F890-0x8087F898, `game/FriendManager` .sbss 0x8087F428-0x8087F430 (sus `sInstance`); `game/GameSystem_unk` .sbss 0x8087F9A0-0x8087F9A8 (`lbl_8087F9A0`, instancia de ResetShutdown); `atn/ptr_string_base_wchar_t_0` .sbss 0x8087F1E0-0x8087F1E8 (buffer y tabla que crea). Prueba: cada tramo esta justo antes/despues de la unidad en el orden de enlace y la unidad lo crea/borra.
- `game/FriendArganan` .rodata ->0x8074D988 y .data ->0x8078A038 (RTTI debil de PlayerTask y su nombre); `gmk/Fireworks` .rodata desde 0x80753168 y .data desde 0x8078DE50 (RTTI debil `__RTTI__PFP7MapAreaPv_v`). Prueba: nuestros `.o` emiten esos debiles; `main.dol: OK`.

## Para el port

- Sin cambios de tipos ni firmas. Definiciones nuevas: `MotionCommandCommon::sInstance`, `NpcManager::sInstance`, `FriendManager::sInstance`, `lbl_8087F9A0` (GameSystem_unk.cpp), `lbl_8087F460`/`lbl_8087DD60`/`lbl_8087DD64` (GameLoadTask.cpp), `lbl_8087F1E0`/`lbl_8087F1E4` (ptr_string_base_wchar_t_0.cpp): el port ya no necesita definirlos aparte.

## Decisiones tomadas sin el usuario

- `locale.c` de MSL usa `#pragma options align=native` / `warn_padding` (fuente de MSL tal cual, no para forzar codigo).
- Renombrados en `symbols.txt` de las tablas de MSL (`__ctype_mapC`, `__lower_mapC`, `__upper_mapC`, `__wctype_mapC`...) y de BTE; `msl_iostream_unk.h`/`locale.cpp` (MSL C++) usan los nombres reales en vez de `lbl_80765338/538/638`.
- `ordercheck` de `MSL_C++/locale` ya daba 8 fuera de orden antes (no tocado salvo el renombrado).

## Hallazgos

- Enlazador GC/1.3.2: los datos no referenciados se quitan (p. ej. `p_bta_dm_cfg`, `__double_max`), asi que fuentes de solo datos del SDK enlazan tal cual.
- Un const a cero de 3 bytes (`bta_dm_pm_cfg`) va a `.sbss2`: el tramo `auto_12_808893A0_sbss2` era de `bta_dm_cfg.c`.
- El `.bss` de WUD.o con alineacion 32 se coloca a 16 (0x8082DDF0): el enlazador no alinea la seccion de un objeto por encima de 16 en `.bss`.
- `AddReqInfo`: sacar `OSPhysicalToCached(...)` a una variable antes de `path` arregla el cruce r25/r27.
- `__parse_dpd_data`: puntero local `obj = &status[0]->obj[i]` tras calcular `fx`/`fy` y usado solo para guardar `x` da el registro extra (savegpr_22).

## Pendientes

- WPADHIDParser: averiguar el objeto de 0x10 que falta al final del `.bss` (0x8082DDF0).
- Tramos que dependen de cabeceras muy incluidas: `lbl_8087D720` (" \t" de `Csv()`, `include/atn/Csv.h`), cadenas debiles `@STRING@` de `SimpleVector`/`D2A_Buffer_unk` (primeros usuarios sin enlazar).
- `sInstance__14GameSaveBanner` (0x8087F470) cae antes de GameSaveTask aunque GameSaveBanner.cpp lo define: revisar a que unidad pertenece.
- Debiles absorbibles por unidades sin enlazar (no suman): Font_unk, CompositeCache_unk, CharaTask, ControlAppear, CrowdSimulation, Em305Vampire, FragileObject, LibraryTask (lista de `weakabs.py` en scratch-D).
