# port-decomp-20261114, cola J (segunda pasada de modelos y dibujo)

Inicio 2026-11-14 06:19. Unidades: atn/Model, atn/BoneCalc_unk, atn/PBoneBasic, atn/LH_Model, atn/ModelDraw_unk.

## Resultado por funcion

- Pendiente de D hecho: `ModelPose_unk::m18` y `ModelBase::mB8` pasan a `atn::ModelNodeCallback_unk`
  (`include/atn/ModelPose_unk.h`, `include/atn/Model_unk.h`); BoneCalc pierde el `reinterpret_cast`
  (`NodeCallback_unk`). Sin perdidas (lost 0, sin bajadas); solo hubo que renombrar el constructor de
  `ModelPose_unk` en `symbols.txt` y el 255.0f de Model.
- `Model::Model` / pool de Model: el `""` detras de "NODE_EDIT" no sale todavia (ver hallazgos). Se mueve la reserva
  del buffer de poses compartido a una funcion `static` (`NewNodePoses_unk`, nombre inventado) definida tras
  `fn_8008A00C`: el constructor debil de `Model::Node_unk` sale en su sitio (0x8008A4B0, antes salia junto a vf24)
  y `sNodePoses` pasa a ser `Model::Node_unk*` (desaparece `NodePose_unk`, que tenia un constructor propio que el
  original no tiene). Mismo porcentaje.

- **`fn_8009DE60` (0x8009DE60, dibujo de una malla): 100 %** (99,04). Firma sin cambios
  `void fn_8009DE60(DrawCmd_unk*, float)`. `shader` declarado el primero (sin inicializar) y cargado el ultimo
  (r31 como el original) y `SetArray_unk(const DrawArray_unk*)` (inline nueva, nombre inventado) para el
  `GXSetArray` del bucle: el original vuelve a cargar el atributo dentro de la llamada.
- **`fn_8009D1A4` (0x8009D1A4, prueba de mips): 100 %** (99,84). `atn::Texture_unk* t` declarado entre `stage` y
  `width` (fuera del bucle) para el registro de la textura.
- **`fn_8009B93C` (0x8009B93C): 100 %** (98,37). La copia (muerta) de la matriz de la camara a traves de
  `MtxAssign_unk(Matrix3x4&, const Matrix3x4&)` (inline local de ModelDraw_unk.cpp, un nivel mas, como
  `VecAssign_unk`): da el r4/r5 del original.
- Sin cambios (probados y revertidos): `fn_8009DCE0` (95,19 %, orden de declaraciones de `index`/`id`/`shader`),
  `fn_8009B9E8` (98,30 %, `m` dentro y fuera del bucle, sin `idx`), `ModelBase::vf14` (98,2 %, `lod` con
  inicializador), `Model::fn_80094F98` (99,5 %, `MtxAssign_unk` en la suma: el cruce esta dentro de
  `MtxAdd_unk`, compartida con vf14; no tocada). BoneCalc `fn_8009A5CC`/`fn_8009ADA4`: el `cntlzw` es del
  `empty()` de boost (`!vtable`) en el `operator safe_bool` y en `operator()`; con el tipo real de `m18` sigue
  igual. `fn_800991D0`, `fn_8009D58C` (pila 0x20 mayor en el original), `LoadModel::fn_80474F0C`: sin tiempo.

## Cambios de firma / layout (para el port)

- `ModelPose_unk::m18`: `boost::function<void()>` -> `atn::ModelNodeCallback_unk` (callback de los nodos de la
  lista ordenada m40). `ModelBase::mB8`: igual (se copia a `ModelPose_unk::m18` en CalcMatrixStack).
- Constructor `ModelPose_unk(ModelData_unk*, Matrix3x4*, const int*, atn::ModelNodeCallback_unk, const unsigned int*,
  int)` (antes `boost::function<void()>`); LH_Model y PBoneBasic pasan `atn::ModelNodeCallback_unk()`.
- `sNodePoses` (Model.cpp, local): `Model::Node_unk*` en vez de `NodePose_unk*`.

## Decisiones tomadas sin el usuario

- Funcion `static` `NewNodePoses_unk` en Model.cpp (inventada, ver arriba).
- Inlines locales nuevas de ModelDraw_unk.cpp (nombres inventados): `SetArray_unk`, `MtxAssign_unk`.

## Hallazgos

- Los literales de una funcion `inline` (con o sin clase) van a `.sdata` como `@STRING@<funcion>`, no a
  `@stringBase0`; los de una funcion `static` que acaba inlineada entera desaparecen de la pool (se vuelven a
  crear en el primer uso emitido). Una funcion `static` se compila en su sitio del fuente (sus funciones debiles
  salen detras de la anterior), una `static inline` no.

- Copia de `Matrix3x4` con r4/r5 cruzados en el `__MtxCopy`: el mismo arreglo que `VecAssign_unk` (una inline
  `MtxAssign_unk(dst, src)` local) (fn_8009B93C).
- Una lectura de campo usada como argumento y como condicion de un bucle sale reutilizada; dentro de una inline
  (`SetArray_unk(a)`) se vuelve a cargar como en el original (fn_8009DE60).

## Pendientes

- El `""` de la pool de Model (offset 0xA, detras de "NODE_EDIT"): en el original lo crea algo compilado entre
  `fn_8008A00C` y `fn_8008AD4C` que no queda en el `.text` (el constructor debil de Node_unk sale ahi). Una
  funcion global lo reproduce, pero sobraria en el `.text`; `static`, `static inline`, `inline` y
  `std::string("")` en fn_8008AD4C no.
- `Model::Model`: `__construct_new_array` de `m224` recibe en el original el constructor de `LH_Motion` y el
  destructor de `MotionSlot_unk` (aqui el constructor implicito de `MotionSlot_unk`; mismo caso en gmk/MapLift).

Fin: 2026-11-14 08:15 (1 h 55 min). Funciones al 100 % 15394 -> 15397 (report.json), lost 0, sin bajadas.
