# Lote batch-auto-5kb - cola q4

Cola (de menor a mayor): `atn/LH_Core`, `atn/LH_Handle`, `atn/DwcAllocator_unk`, `atn/THPDraw_unk`,
`atn/PPDATAU`, `atn/FileSystem_unk`, `atn/XsfParser_unk`, `atn/Bezier_unk`, `atn/NetFriend_unk`.

## atn/LH_Core (commit HASH)

- Resultado: 7/7 al 100%, `matching: true`, `main.dol: OK`.
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**
  - Splits: sin cambios (la unidad no tiene `.rodata`/`.sdata2`; la RTTI de `atn::LoadCore` la emite `LH_ColliTree`).
  - `symbols.txt` (pairnames): `fn_804730E4` -> `fn_804730E4__Q23atn8LoadCoreFv` (quita una referencia; lo usan
    `LoadHandle` y `FileSystem_unk`), `fn_80473184` -> `vf0C__Q23atn8LoadCoreFv` (slot 0x0C de la vtable).
  - `include/atn/LoadCore.h` (compartida): `mState` y `m05` pasan a `volatile` (cargador con hilos: 0x80473104
    relee `mState` tras el store y el ctor intercala el store de la vtable entre los dos `stb`); nuevo
    `fn_804730E4`. El ultimo 0 de la vtable (0x18 bytes) es relleno: las vtables hijas miden 0x14.
  - `include/atn/LoadManager.h`: nuevo miembro `m3EDC` (bandera que pone 0x804730E4 al quedar sin referencias).
  - Nombre real propuesto del fichero: se mantiene `LH_Core` (sin pruebas nuevas).
- Hallazgos MWCC
  - Store de un miembro seguido de su relectura (`stb` + `lbz` en vez de reusar el registro) = miembro `volatile`;
    dos miembros `volatile` dejan el store de la vtable entre sus stores en el ctor.
