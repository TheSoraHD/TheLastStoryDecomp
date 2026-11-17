# Cola N5b (lote escribir-20261116): gimmicks, segunda pasada

Inicio: 2026-11-17 08:55 (tope 10:55). Rama agent-N5b.

| Funcion | Antes | Despues |
|---|---|---|
| gmk/SwingObject vf10 | 95,8 % | 98,36 % (e418d23) |
| gmk/SwingObject SwingPart_unk ctor | 99,5 % | |
| gmk/SwingObject fn_803FFF40 | 96,4 % | 99,90 % (e418d23) |
| gmk/SwingObject fn_804003E0 | 97,2 % | **100 %** (e418d23) |
| gmk/PassBoard vf0C | 98,7 % | |
| gmk/RandomMerchant vf10 / vf20 | 98,6 / 99,3 % | |
| gmk/MotionObject vf10 / vf24 | 97,8 / 99,9 % | |
| gmk/LandMine Start | 98,3 % | |
| gmk/ChickenManager vf0C (extra) | 93,9 % | 96,15 % |
| gmk/BulletPoint vf10 | 98,5 % | |
| gmk/ShakeCamera vf10 | 98,2 % | 99,34 % |

## Variantes probadas

- gmk/SwingObject UpdateSwing_unk (los FPR que bloqueaban vf10, fn_803FFF40 y fn_804003E0): tres arreglos.
  (1) `core->mMaxWind * ToRad_unk(2.0f)` en vez de `* 0.034906585f`: el literal suelto se carga antes que
  el miembro (MWCC lo pone como primer operando); plegado desde la inline, el miembro va primero como en
  el original (mismo valor en `.sdata2`). (2) La suma del viento escrita en cada uso
  (`range / mMaxWind * (mWindPower + mWind)` y `if (mWindPower + mWind > mMaxWind)`) en vez del local
  `wind`: con el local MWCC reasocia `range / max * wind` como `(range * wind) / max` en fn_804003E0 y
  conmuta los operandos en vf10. (3) Los tres senos por `SinF_unk(float)` (inline que devuelve
  `(float)sin(x)`): el argumento del seno siguiente se calcula antes que el producto, como en el original;
  el seno del primer angulo ya no necesita local. Probados antes sin efecto: `s * ToRad`, `0.0349 * max`,
  locales `max`/`ratio`, `range * wind / max` (peor), sin2 en local, `ToRad_unk(1.0f) *`.
- SwingPart_unk fn_804003E0 (-> 100): el bucle de las matrices de nodos en una inline `CalcNodes_unk(part)`
  compartida con fn_803FFF40 (con `local` antes que `count` y `mtx` antes que `t`: un nivel de inline
  invierte la pila). perm.py de nodes/data/count/local/i fuera de la inline: 99,82 en los 120 ordenes.
- SwingPart_unk fn_803FFF40 (96,4 -> 99,90): CalcNodes_unk y el efecto en `PlayPartEfp_unk(mgr, efp, owner)`
  (el gestor es argumento de la inline y se carga antes, como en el original). Queda m98 de m9C/mA0/mA4 en
  r26/r27 (r28/r29 en el original): sin base local, tres locales, base arriba, base como
  `atn::Matrix3x4*`, inline `PartNodeMtx_unk(this, i)` / `(this, node)`, AllocNodes_unk entera (96,7,
  saca cadenas `@STRING@`), m80 antes de mA4, `m98.reset(new ...)` directo: igual o peor.
- SwingObject vf10 (98,36): queda la direccion del modelo (r31+0xF4) calculada antes del salto del LOD.
  Probados: `(atn::Model*)&mModel` en la llamada, `atn::Model* model` local (95,5), LOD en
  `DecLod_unk(model)` (95,5), el lod repetido sin local (98,48 pero peor forma), `Hide_unk(this)`.
- SwingPart_unk ctor (99,46): m94..m134 en el cuerpo (88,6), `m98()`, `mLoader()` explicito, m134 en el
  cuerpo: igual. Es el color de vt/begin/end del bucle del array de SwingParam_unk.
- gmk/ShakeCamera vf10 (98,18 -> 99,34): `SqrtF_unk(float)` para las dos raices del volumen (la rate en
  f31) y el tono con `Rand01_unk` escrito a mano (como ColliAttrManager: con la inline el 0.5f se carga
  antes que el campo). Queda r5/r7 del puntero a lbl_8087F430 (y r5/r6 del resto de rand() % 1001).
  Probados: `SetShake_unk(m96C % 2, ...)` como MotionCommand_unk, x/y en la llamada, frame local, perm de
  rate/x/y/field (24), RandRange_unk con el factor o el rango local o escrito: igual o peor.
- gmk/ChickenManager vf0C (93,92 -> 96,15): `Chicken_unk* c = &mChickens[i]` para mCenter/mRadius (el
  original no recarga mChickens entre las dos copias). Mismo arreglo valdria para PigeonManager vf0C.
- gmk/ChickenManager FindNear_unk (pila de fn_8040C058/fn_8040C610/vf0C): `atn::Vector3 t = ...; mTmp = t;`
  quita el `frsp` de `t = mTmp = ...` (97,56/97,97 pero vf0C 95,89; sin commit). Modelo de pila que
  predice las tres variantes medidas (de arriba abajo): locales con nombre de la funcion en orden de
  declaracion, temporales, y despues cada inline con su lista (con nombre + temporales + inlines
  anidadas) invertida. El original (next, n, d, op, temp de next, diff, t, pos) no sale con FindNear por
  valor, con `AvoidOthers_unk` (bloque comun de fn_8040C058/fn_8040C610) ni con las dos cosas.
- gmk/MotionObject vf24 (99,94): `const void*`/`void*`/`int` para data/size, CsvTab(const void*) con los
  separadores dentro, size local y data en la llamada: igual o peor (size toma el registro de `field`, r22;
  en el original el de `fade`, r25).
- gmk/Chain fn_8043B9E0: lo que falta es el push_back de std/vector.h (cabecera compartida), no tocado.
- gmk/FlyObjManager fn_80427870: el fabs/frsp esta en fn_80139F60 de Vec.h (fuera de la cola).
