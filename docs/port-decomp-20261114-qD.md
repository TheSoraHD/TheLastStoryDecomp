# port-decomp-20261114, cola D (Modelos: atn/Model, atn/BoneCalc_unk)

Inicio 2026-11-14 02:59. Unidades: atn/Model, atn/BoneCalc_unk.

## Resultado por funcion

- `ModelBase::vf14` (0x8008FA24, prueba de sombra): escrita, NONMATCHING 98,2 % (registros: el original
  tiene el nivel de detalle en r31 y `this` en r26; registros de direccion de la matriz inline de
  `fn_80094F98`; hueco de pila de la copia del centro de la esfera de sombra).
  Firma final: `int vf14(const Matrix3x4*, const Vector3*, const Frustum_unk*, AABB_unk*)` (la de la fase 0).
  `fn_8008FA24` renombrada en `symbols.txt` al nombre mangled.
- `ModelBase::fn_8008D78C(RenderManager*)` (0x8008D78C, visibilidad): escrita, NONMATCHING 96,4 % (pila y
  registros; la prueba del flag 1 sale materializada como valor en el original, `subfic/or/srwi` tras recargar
  `mFlags`, sin encontrar la forma).
- `Model::fn_8008CD60` (ya escrita): 95,8 -> 97,3 % con las inline `IsReady_unk`/`IsReflected_unk` compartidas
  con fn_8008D78C (bools `li 1 ... li 0` del original).

## Cambios de firma / layout (para el port)

- `fn_8008EEDC` (extern "C" local de Model.cpp): el tercer argumento pasa de `int` a `const atn::Matrix3x4*`
  (vf14 le pasa su `view`, que no usa).
- Sin cambios en cabeceras.

## Decisiones tomadas sin el usuario

- Inline nuevas en Model.cpp (nombres inventados): `ShadowSphere_unk` (esfera del modelo de sombra m1C4),
  `SetBox_unk` (esquinas de una caja; sus argumentos se evaluan de derecha a izquierda como en el original),
  `IsVisible_unk` (entrada del AABBTree), `IsReady_unk`, `IsReflected_unk`, `HasFlag_unk`. `MtxAdd_unk` y
  `MtxScale_unk` suben delante (las usa ShadowSphere_unk).
- 255.0f de `.sdata2` (0x80880C14) renombrada a mano otra vez (`@22090`); `lbl_8087D6E8/EC` devueltos tras pd.

## Hallazgos

- `bool r = true; bool a = true; if (!A && !f()) a = false; if (!a && !g()) r = false;` reproduce los dos
  `li rX,1` iniciales del original (fn_8008D78C, fn_8008CD60).
- Una prueba de `mFlags` en una inline `bool` (`HasFlag_unk`) fuerza la recarga de la palabra como en el original.
