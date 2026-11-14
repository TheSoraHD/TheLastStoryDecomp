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

- `fn_8009BB9C` (0x8009BB9C, matrices de textura): **equivalente** al asm (revisado entero); sigue
  NONMATCHING 95,6 % (solo registros: el `cos` en f24 y las constantes 0/1 en f30/f31 del original).
  Para el port: modo 0 sin skinning carga la matriz UV como **2x4** (`GXLoadTexMtxImm(uvm, gen->m18, 1)`); con
  skinning (`mesh->m14 == -1`) la carga como 3x4 en la matriz posterior `0x40 + 3k` (GX_PTTEXMTX: el texgen
  tiene que ir en modo dual / post-transform). Modos 1-4 (proyectados): sin skinning, la matriz de la malla sin
  traslacion (modo 4: con traslacion) en `gen->m18` y siempre `uvm` (con las columnas 2 y 3 intercambiadas en
  las filas 0 y 1) x `fn_800C0900(src->m10, m14, m18, 0)` en la posterior `0x40 + 3k`.
- `fn_8009B9E8` (0x8009B9E8, matrices de posicion/normal): **equivalente** (revisado); NONMATCHING 98,3 %
  (registros de los dos bucles). Para el port: sin skinning carga la matriz en PNMTX0, su inversa traspuesta
  sin traslacion como normal 0 **y tambien como matriz de posicion 3**, y `GXSetCurrentMtx(0)`; con skinning
  carga `mtx[sub->m10[i]]` en `i*3` (hasta 10 o -1) solo si cambio respecto a `cache[i]`, y con luz (ni sombra
  ni `lbl_8087EFA8->m234`) la normal en `i*3` y la misma normal como textura en `30 + i*3` (GX_TEXMTX0+i, 3x4).
  El indice de matriz lo manda cada vertice (atributo GX_VA_PNMTXIDX directo, tipo 4 de la descripcion de la
  malla en `LoadModel::fn_80474F0C`): si el port no lo respeta, los vertices salen con PNMTX0 (prismas).
- `fn_8009D58C` (0x8009D58C, encola la malla): **equivalente** (revisado); 83,6 -> 94,6 % con
  `DrawColor_unk` = `atn::Color` (la copia del color va por lfs/stfs), `int ef` y `m48 & 2u` (signo de las
  comparaciones) y `mat->m49` leido antes del `if`. Falta el idiomatico `orc/srwi` de la prueba
  `mCmds.mSize < 0x1000` (un `>=` sin signo materializado) y registros de las copias de `Push_unk`.
- `fn_8009BF3C` (0x8009BF3C): sigue 99,99 %. Unico fallo: el original tiene **dos** constantes `0xFFFFFFFF`
  en `.sdata2` (0x80880C50 y 0x80880C54), una por cada expansion de `SetMatColor_unk` (el blanco de
  `GXSetTevKColor(1, ...)`); nuestra inline comparte una. Probado: `static` sin `inline` (no se inlinea).

- `atn/GraphicsDevice` `fn_80074EC8` (texgen y TEV del material): **equivalente** (diferencias solo de
  registros y orden); 98,55 -> 99,28 % con `int mode = 1; if (lbl_8087EFA8->m18C != 0) mode = 2;`. Quedan el
  `mulli` recalculado de la fila de `sTevCosts` en la prueba sin dual y el orden de un `stw`.
  Para el port: con `flags & 1` (dual = malla con skinning, `DrawCmd` bit 0) los texgen de tipo 0 usan
  `GXSetTexCoordGen2(coord, func, src, GX_IDENTITY (0x3C), 0, 0x40 + 3k)` y los de tipo 1-4 la matriz del
  texgen con normalizacion (salvo tipo 4) y la posterior `0x40 + 3k`; el contador `k` avanza en el mismo
  orden que `fn_8009BB9C` carga las matrices posteriores. Sin dual, tipo 0 usa `texGen.mMtx` y `GX_PTIDENTITY`
  (0x7D). En dual las coordenadas se renumeran: tipo 0 desde 0, tipos 1-3 desde 4, tipo 4 desde 6 (las
  ordenes TEV y las indirectas usan `coords[...]`, no el indice del texgen).
- Las que llama `fn_80074EC8` (`fn_80073194` 96,99 %, `fn_80073A6C` 99,88 %, `fn_800742C8` 99,39 %,
  `fn_800748D8` 99,73 %, `fn_80074B24` 99,64 %, `fn_80072F64` 100 %): revisadas con un diff que ignora los
  nombres de registro: **equivalentes** (solo registros, orden de cargas y operandos conmutados).

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

- `include/atn/DrawCmd_unk.h`: `DrawColor_unk` pasa de `struct { float m[4]; }` a `typedef atn::Color`
  (mismo layout; `RenderSystem_unk.cpp` usa `m24.r/g/b/a`). El ctor de `DrawEntry_unk` cambia de nombre
  mangled (`...PQ23atn5Color...`), renombrado en `symbols.txt`.

## Decisiones tomadas sin el usuario

- `symbols.txt`: tras `pd.sh` se devolvieron `lbl_8087D6E8/EC`; se aceptan los renombrados de la unidad
  (`@STRING@Resize_unk...MeshArrays_unk...` en 0x8087E058/5C y los `@N` de `.sdata2`).
- RenderSystem_unk: tras el typedef sus `@N` de `.sdata2` 0x808810B0-C8 subieron 2; `pd.sh` no los renombra
  (nombres repetidos en `symbols.txt`), desplazados a mano +2 conservando las repeticiones que ya habia.
