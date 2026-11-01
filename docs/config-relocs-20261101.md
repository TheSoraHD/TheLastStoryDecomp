# config-relocs-20261101

Rama corta (2026-11-01) con los tres cambios de configuracion aprobados por el usuario. 11467 -> 11489 funciones al 100% (+22), `main.dol: OK`, `regress.py` lost 0, `fuzzdrop.py` 0.

## Cambios

- `config.yml` `block_relocations` con `target:` (no `source:`): bloquea toda relocacion que apunte a una direccion, venga de `.text` o de datos. Mas corto que listar los 17+ `lis/addi`:
  - `.bss:0x80808080-0x80808082`: constantes 0x80808080 (mascara de `strlen`/NETAES) y 0x80808081 (division por 255: nw4hbm, WPAD, ut_CharWriter).
  - `.bss:0x807F8084`, `.bss:0x80818283`: tablas de bytes de `.rodata` y `GXTexture` `.sdata`.
  - `.bss:0x80868085`, `.bss:0x80877B48`: dos valores de la tabla `.rodata` `lbl_80767388` dentro del buffer de SoundSystem.
- `symbols.txt`: quitados los `lbl_` falsos (`lbl_807F8084`, `lbl_80808080`, `lbl_80808081`, `lbl_80809B1B`, `lbl_80818283`, `lbl_80868085`, `lbl_80868309`, `lbl_80877B48`); `gki_cb` size 0x28AE0, `defaultSoundSystemWork$8443` size 0x155E0.
- Splits y `libs_ref` nuevos: `smg2/src/RVL_SDK/bte/gki_ppc.c` (Wii/1.0, flags de `gki_buffer.c`) y `nw4r/src/snd/snd_SoundSystem.cpp` (Wii/1.0, flags de `snd_SoundHeap.cpp`).
- MetroTRK `targsupp.c`: cuatro `asm` `nofralloc` `twi 31, r0, 0; blr` (MWCC no conoce `twui`; mismos bytes). `TRKOpenFile` y `TRKPositionFile` no tienen llamadas: `force_active` en `config.yml`. Excepcion de asm anadida a la regla 2 de `CLAUDE.md`.
