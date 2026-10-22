# close-units, cola D: notas

Una linea por unidad procesada (orden de cola_D.md).

- PowerPC_EABI_Support/src/MSL_C/MSL_Common/strtold (lib): ENLAZADA. Fallaba el link por `undefined: nan`: el stub `blr` en 0x8068B0FC (auto_03, entre los stubs math acos/asin/.../sqrt) se llamaba `strdup` en symbols.txt; su unico llamador es `__strtold` (`nan(nan_arg)`). Renombrado `strdup` -> `nan` (0x8068B0FC) en symbols.txt. Decision sin el usuario: el renombrado.
- RVL_SDK/src/revolution/ax/AXSPB + RVL_SDK/src/revolution/ax/AXVPB (lib): ENLAZADAS (juntas). Por separado fallan: con solo AXSPB el `.bss` de AXVPB (auto) se desplaza; con las dos, el `.data` de AXVPB sale de 0x90 (la fuente tiene `__AXRmtMixCycles` de 4 entradas) y la tabla siguiente `__AXCompressorDefaultTable` (0x807AC6A0, auto_07) se adelantaba 8 bytes -> todo .data/.bss/.sdata... -0x40. Arreglo en symbols.txt: `__AXRmtMixCycles` (0x807AC688) size 0x18 -> 0x10 (los 8 bytes de ceros son relleno) y `__AXCompressorDefaultTable` (0x807AC6A0) `align:32`. Decision sin el usuario: esos dos cambios de symbols.txt.
- Hallazgo: si el hueco de relleno hasta un objeto alineado a 32 de una unidad auto se cuenta en el size del objeto anterior, el `.data` de la unidad con fuente sale corto y desplaza todo; se arregla poniendo `align:32` al objeto auto siguiente.
- RVL_SDK/src/revolution/esp/esp (lib): ENLAZADA. Faltaba `IOS_IoctlvReboot`: renombrado `fn_8061D4C0` -> `IOS_IoctlvReboot` (0x8061D4C0, auto tras IOS_Ioctlv; unico llamador ESP_LaunchTitle). Decision sin el usuario: el renombrado.
