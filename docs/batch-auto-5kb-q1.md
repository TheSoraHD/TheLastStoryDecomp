# Lote batch-auto-5kb - cola q1

Cola (de menor a mayor): `atn/Math_unk`, `atn/LipSync_unk`, `atn/Pad_unk`, `atn/LightUtil_unk`, `atn/Light_unk`,
`atn/PostFxBloom`, `atn/PostFxColorMatrix`, `atn/FileUtil_unk`, `atn/Geometry_unk`, `atn/BoneCalc_unk`.

## atn/Math_unk (commit COMMIT_MATH)

- Resultado: 1/1 al 100% (`__sinit`), `matching: true`.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**:
  - Split sin cambios (ya cubria `.text`, `.ctors`, `.bss` 0x807C7028-0x807C7090 y `.sdata2` 0x80880BC8-0x80880BD0).
  - `symbols.txt`: `fn_80081DC8` -> `__sinit_\Math_unk_cpp` (scope:local).
  - Los globales se mantienen como `extern "C" lbl_807C70xx` (otras unidades ya los declaran asi): vector 2D cero,
    `atn::Vector3` cero, dos `atn::Quaternion_unk` (cero e identidad), `atn::Matrix3x4` identidad (tipos inferidos por los stores).
  - `include/math/Vec.h`: anadidos ctores inline `Quaternion_unk(x, y, z, w)` y `Matrix3x4(12 floats)`.
  - Nombre real propuesto: `atn_math.cpp` (sin prueba directa).
- Hallazgos MWCC: objetos de `.bss` de mas de 8 bytes alineados a 8 dentro del fichero (Vector3 de 0xC ocupa 0x10).
