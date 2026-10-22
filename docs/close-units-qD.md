# close-units, cola D: notas

Una linea por unidad procesada (orden de cola_D.md).

- PowerPC_EABI_Support/src/MSL_C/MSL_Common/strtold (lib): ENLAZADA. Fallaba el link por `undefined: nan`: el stub `blr` en 0x8068B0FC (auto_03, entre los stubs math acos/asin/.../sqrt) se llamaba `strdup` en symbols.txt; su unico llamador es `__strtold` (`nan(nan_arg)`). Renombrado `strdup` -> `nan` (0x8068B0FC) en symbols.txt. Decision sin el usuario: el renombrado.
