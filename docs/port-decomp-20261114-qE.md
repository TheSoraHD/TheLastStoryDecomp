# port-decomp-20261114, cola E (carga y dibujo de modelos: atn/LH_Model, atn/ModelDraw_unk, atn/GraphicsDevice)

Inicio 2026-11-14 04:41.

## Resultado por funcion

- `atn::LoadModel::fn_80474F0C` (0x80474F0C, 3820 bytes, formatos y arrays de vertices): escrita (antes sin
  escribir), NONMATCHING 96,4 % (huecos de pila de los desplazamientos reducidos por fuerza `i*0x144`/`i*4`,
  orden de los desplazamientos en la rama de version 2 y la carga de `lbl_8087EEE0` antes del temporal
  `VtxFormat_unk`). Firma final: `void atn::LoadModel::fn_80474F0C(ModelData_unk* model)` (la de antes).
  - **Logica frente al asm (para el port):** la version funcional del port es equivalente. Detalles que el port
    tiene igual o de mas: el original solo crea `m84` (las listas de 27 `DrawArray_unk` de las mallas de
    version 1) con la **primera** malla (`i == 0`); el `|| m84.mData == 0` del port es una proteccion de mas, no
    esta en el original. En la rama de version 1 el original escribe tambien `fmt.mList[n]` (lo mismo que se
    vuelve a escribir despues para todas las versiones): redundante. Tablas `.rodata` (0x80755DB4, 0x80755DE0)
    con **11** entradas (tipos 9 rgba6 -> 4/3 bytes y 10 rgbx8 -> 2/4 bytes), no 9 como en el port: los tipos
    9 y 10 en el port leen fuera de la tabla.
  - Hallazgo: los 14 bloques de 16 bytes de `.rodata` 0x80755E10-0x80755EF0 son inicializadores de agregado
    locales (`GXVtxAttrFmtList f = {9, 1, tipo, frac}`, `DrawArray_unk a = {9, datos, paso}`), uno por sitio, con
    los campos no constantes a 0; para que el temporal de `DrawArray_unk` se materialice como en el original en
    colores/texturas hace falta `= {attr, 0, 0}` y luego asignar los campos.
  - Hallazgo: `mTextures[i] = fn_80076C44(lbl_8087EEE0, VtxFormat_unk(desc, fmt))` con `operator[]` (la direccion
    del destino se calcula antes de la llamada): 92 -> 96 %.

## Cambios de firma / layout (para el port)

- `fn_80076C44` (`include/atn/GraphicsDevice.h`, `src/atn/GraphicsDevice.cpp`): el formato pasa de
  `const VtxFormat_unk*` a `const VtxFormat_unk&` (LoadModel le pasa un temporal). Mismo codigo en la Wii.
- `LoadModel` (`src/atn/LH_Model.cpp`): `mTextures` pasa a `SimpleArray<int>` (indices de la cache de formatos)
  y `m84` a `SimpleArray<LoadModel::MeshArrays_unk>` (27 `DrawArray_unk`, 0x144 bytes; su `Resize_unk` tiene
  su propio par de `""` en `.sdata`, 0x8087E058/5C, que confirma el tipo).
- Nueva cabecera `include/atn/ModelVtx_unk.h`: `DrawArray_unk` (sacada de `ModelDraw_unk.cpp`),
  `LoadVtxAttr_unk`, `LoadVtxDesc_unk`, `LoadVtxStream_unk` (mismos nombres que el port).
- `include/atn/ModelPose_unk.h`: `ModelNode_unk` (el chunk 'mesh') con sus campos (`m04` version, `m18`
  descripcion, `m1C`/`m20`/`m24`/`m28`/`m2C[8]` streams, `m54` arrays); `ModelData_unk::m28/m2C` (streams) en
  lugar del relleno. Sin cambio de offsets.

## Decisiones tomadas sin el usuario

- `symbols.txt`: tras `pd.sh` se devolvieron `lbl_8087D6E8/EC`; se aceptan los renombrados de la unidad
  (`@STRING@Resize_unk...MeshArrays_unk...` en 0x8087E058/5C y los `@N` de `.sdata2`).
