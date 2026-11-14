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

## Cambios de firma / layout (para el port)

- `ModelPose_unk::m18`: `boost::function<void()>` -> `atn::ModelNodeCallback_unk` (callback de los nodos de la
  lista ordenada m40). `ModelBase::mB8`: igual (se copia a `ModelPose_unk::m18` en CalcMatrixStack).
- Constructor `ModelPose_unk(ModelData_unk*, Matrix3x4*, const int*, atn::ModelNodeCallback_unk, const unsigned int*,
  int)` (antes `boost::function<void()>`); LH_Model y PBoneBasic pasan `atn::ModelNodeCallback_unk()`.
- `sNodePoses` (Model.cpp, local): `Model::Node_unk*` en vez de `NodePose_unk*`.

## Decisiones tomadas sin el usuario

- Funcion `static` `NewNodePoses_unk` en Model.cpp (inventada, ver arriba).

## Hallazgos

- Los literales de una funcion `inline` (con o sin clase) van a `.sdata` como `@STRING@<funcion>`, no a
  `@stringBase0`; los de una funcion `static` que acaba inlineada entera desaparecen de la pool (se vuelven a
  crear en el primer uso emitido). Una funcion `static` se compila en su sitio del fuente (sus funciones debiles
  salen detras de la anterior), una `static inline` no.

## Pendientes

- El `""` de la pool de Model (offset 0xA, detras de "NODE_EDIT"): en el original lo crea algo compilado entre
  `fn_8008A00C` y `fn_8008AD4C` que no queda en el `.text` (el constructor debil de Node_unk sale ahi). Una
  funcion global lo reproduce, pero sobraria en el `.text`; `static`, `static inline`, `inline` y
  `std::string("")` en fn_8008AD4C no.
- `Model::Model`: `__construct_new_array` de `m224` recibe en el original el constructor de `LH_Motion` y el
  destructor de `MotionSlot_unk` (aqui el constructor implicito de `MotionSlot_unk`; mismo caso en gmk/MapLift).
