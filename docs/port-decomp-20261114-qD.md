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
- `fn_800991D0` (0x800991D0, pistas aditivas 4 y 5): escrita, NONMATCHING 99,6 % (registros del indice y del
  puntero de la inline `fn_80099010`; direcciones de pila invariantes en registros salvados). Firma final:
  `void atn::Model::fn_800991D0(int advance)` (miembro; antes `extern "C"` con el modelo delante).
- `atn/BoneCalc_unk` `fn_8009ADA4` (0x8009ADA4, matrices desde un motion): escrita, NONMATCHING 98,2 % (las pruebas
  `empty()` de los dos `boost::function` salen materializadas en el original, `cntlzw/srwi.`, aqui `cmpwi`; no
  sale con ninguna forma probada; pila). Firma final:
  `extern "C" void fn_8009ADA4(ModelPose_unk*, int joint, const atn::MotionBuffer*, const atn::Matrix3x4* parent,
  const atn::Matrix3x4* scale)`.
- `atn/BoneCalc_unk` `fn_8009A5CC` (0x8009A5CC, matrices desde la pose de reposo, la sobra de la cola): escrita,
  NONMATCHING 99,1 % (mismo `cntlzw` y huecos de pila). Firma final: `extern "C" void fn_8009A5CC(ModelPose_unk*,
  int joint, const atn::Matrix3x4* parent, const atn::Matrix3x4* scale)`.
- 0x8009B938 (antes `fn_8009B938` escrita a mano) es `boost::function3<void, Matrix3x4*, const NodeChunk&, const
  ModelNodeCallbackContext&>::dummy::nonnull`, que emite el `if (callback)`: renombrada en `symbols.txt`, al 100 %;
  las tres constantes de `.data` 0x80778930 son su puntero a miembro.
- `Model::fn_8008CD60` (ya escrita): 95,8 -> 97,3 % con las inline `IsReady_unk`/`IsReflected_unk` compartidas
  con fn_8008D78C (bools `li 1 ... li 0` del original).

- Parciales de Model: `fn_8008EEDC` 80,4 -> 87,9 % (`bool mesh = m2C != 0` calculado antes y esquinas de la caja
  por valor, `JointBoxMin_unk/JointBoxMax_unk`); `fn_800909B0` 45,3 -> 79,2 % (matrices y pesos sin `const`);
  **`Model::fn_80097A9C` al 100 %** (94,6: sobraba la copia de `m08`, ya la hace el `operator=` implicito de
  `LH_Motion`; mnemcheck limpio); `vf10` 92,0 -> 92,3 % (cuerpo dentro de `if (... == 8 && mFlag6_unk)`);
  `fn_8008E818` 93,9 -> 94,5 % (`m20C_7` con signo).

## Cambios de firma / layout (para el port)

- `fn_8008EEDC` (extern "C" local de Model.cpp): el tercer argumento pasa de `int` a `const atn::Matrix3x4*`
  (vf14 le pasa su `view`, que no usa).
- `include/atn/ModelPose_unk.h`: el ultimo argumento de `fn_8009A5CC`/`fn_8009ADA4` pasa de `int` a
  `const atn::Matrix3x4* scale` (escala heredada del padre; los llamadores pasan 0). Se mantienen los nombres de
  parametro: quitarlos renumera los `@N`/`@enum$N` de todas las unidades (ControlAppear perdia 2 funciones).
  `ModelJoint_unk::m30` pasa de relleno a `int` (el joint no hereda la escala del padre).
- `fn_800909B0` (extern "C" local): `mats` y `w` pasan a no `const` (`atn::Matrix3x4*`, `SkinWeight2_unk*`), y
  `ModelMtxBlock_unk::mSkin2` (`atn/ModelPose_unk.h`) a `SkinWeight2_unk*`; los de 3 y 4 juntas siguen `const`
  (sin `const` bajan).
- `ModelBase::m20C_7` (`atn/Model_unk.h`) pasa a `int m20C_7 : 1` (con signo: fn_8008E818 lo lee con `srawi`).
- **Para el port**: `ModelPose_unk::m18` es en realidad un `ModelNodeCallback_unk` (mismo `nonnull` 0x8009B938 que
  `m2C`), y por lo tanto `ModelBase::mB8` tambien; la cabecera los declara `boost::function<void()>` y BoneCalc
  los lee con `reinterpret_cast` (`NodeCallback_unk`). Cambiar el tipo toca `atn/Model_unk.h` (muy incluida): no hecho.
- `atn::ModelNodeCallbackContext` definido en `src/atn/BoneCalc_unk.cpp`: `{const Matrix3x4* mParent;
  const Quaternion_unk* mRot;}` (matriz del padre con la escala heredada y rotacion local).
- `include/atn/EulerToQuat_unk.h` nuevo: `atn::EulerToQuat_unk` sale de Model.cpp (Model.o identico) y lo usa
  BoneCalc. Los getters `ModelJoint_unk::GetScale_unk/GetRot_unk/GetTrans_unk` siguen definidos en el .cpp (Model y
  BoneCalc): con cuerpo en la cabecera renumeran los `@enum$N`.

- Deuda de la regla 10 saldada en Model: `fn_8008E818` pasa a `void ModelBase::fn_8008E818(RenderManager*)` y
  `fn_800991D0` a `void Model::fn_800991D0(int)` (declaraciones sin nombres de parametro en `atn/Model_unk.h`,
  neutras: `@enum$N` y labelcheck iguales; mismo porcentaje; renombrados en `symbols.txt`).

## Decisiones tomadas sin el usuario

- Inline nuevas en Model.cpp (nombres inventados): `ShadowSphere_unk` (esfera del modelo de sombra m1C4),
  `SetBox_unk` (esquinas de una caja; sus argumentos se evaluan de derecha a izquierda como en el original),
  `IsVisible_unk` (entrada del AABBTree), `IsReady_unk`, `IsReflected_unk`, `HasFlag_unk`. `MtxAdd_unk` y
  `MtxScale_unk` suben delante (las usa ShadowSphere_unk).
- 255.0f de `.sdata2` (0x80880C14) renombrada a mano otra vez (`@22090`); `lbl_8087D6E8/EC` devueltos tras pd.

## Hallazgos

- Con un puntero `const T*` MWCC adelanta las cargas a traves de los stores a otro puntero (supone que no se
  solapan); si el original carga despues de los stores, el parametro no era `const` (fn_800909B0 45 -> 79 %).

- Quitar nombres de parametro de una declaracion de cabecera tambien renumera los `@N`/`@enum$N` de las unidades que
  la incluyen (lo inverso del hallazgo de paralelo-20261112); igual con cuerpos de inline nuevos.

- `bool r = true; bool a = true; if (!A && !f()) a = false; if (!a && !g()) r = false;` reproduce los dos
  `li rX,1` iniciales del original (fn_8008D78C, fn_8008CD60).
- Una prueba de `mFlags` en una inline `bool` (`HasFlag_unk`) fuerza la recarga de la palabra como en el original.
