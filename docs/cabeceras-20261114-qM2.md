# Cola M2 (cabeceras-20261114): las 2 bajadas de NewCampTask que deja la cola M

Inicio 21:32. Referencia `$REF` = `/home/claude/ref-M2` (antes de la cola M). Resultado: **no sale**; sin
cambios de codigo (solo estas notas). La rama sigue con las dos bajadas de M:
`NewCampView_unk::fn_8052E1A0` 71,31 -> 70,81 y `fn_8052FA44` 73,22 -> 73,12 (precommit FAIL por fuzzdrop
contra `$REF`, igual que tras M).

## Causa

- Las dos funciones son NONMATCHING estructurales: el `atn::Scene::operator=` implicito se inlinea entero
  (el original llama a las copias de Texture, GodRay, HazeParam_unk y ColorMatrixParam_unk) y eso anade
  registros guardados (r29/r24) que desplazan todo el resto de la funcion.
- Con `Frustum_unk::mBounds` como struct, la copia inline de `atn::Camera` materializa el puntero de las
  esquinas del lado de `this` (`addi r6, r29, 0x17c` en fn_8052E1A0, `addi r5, r29, 0x17c` en fn_8052FA44);
  el original las direcciona sobre `this` (`0x17c(r30)`). Con dos miembros sueltos (antes de M) se plegaba
  en nuestro build, pero entonces bajan las 4 funciones que M subio al 100 %.
- Prueba aislada (`try.sh`, estructuras minimas): en una copia por asignacion `View::f() { mCamera =
  RM::instance_->mCamera; }` el puntero sale **igual con struct que con miembros**; solo el constructor de
  copia distingue los dos. El plegado del original en NewCampTask depende del contexto de la funcion (no del
  tipo), y nuestro contexto ya es otro por la copia de la escena inlineada.

## Intentado (sin llegar a la referencia)

Valores de `objdiff-cli` (la referencia en ese mismo calculo: 71,30 y 73,21).

| Variante | fn_8052E1A0 | fn_8052FA44 |
|---|---|---|
| actual (M) | 70,80 | 73,11 |
| `atn::Camera& cam = mCamera; cam = ...` (o `Camera* cam = &mCamera`, o con `const Camera& src`) | **71,11** | = |
| local `const Camera&` del origen; local antes de la copia de la escena | = | = |
| fn_8052FA44: `instance_->mCamera = mCamera` | = | 72,09 |
| fn_8052FA44: `fn_8052C6CC(instance_, &mCamera)` | = | 73,03 |
| fn_8052FA44: referencia local a `mCamera` (const o no) | = | = |
| cabecera: `atn::Vector3 mBounds[2]` | 71,30 | 73,21, pero GameCamera fn_80392D2C 100 -> 98,8 y fn_80392A04 99,42 -> 98,01 |
| cabecera: struct con `Vector3 m[2]` | = | = |
| cabecera: struct con esquinas y `mPlanes` | 70,34 | 73,15, y bajan C6CC, SetCamera_unk, copia de MiniMapTask |
| `CharaLightData_unk::operator=` implicito (prueba, fuera de la cola) | 29,9 | 31,6 (la escena sigue inlineada) |

`cam = ...` sube fn_8052E1A0 a 71,11 (quita la copia de `this` en r24) pero no llega a 71,30, y no es avance
real (regla 10: solo fuzzy): no aplicado.

## Pendiente (fuera de esta cola)

- Arreglo real: que el `atn::Scene::operator=` pase el presupuesto R3 como en el original. Pide que los
  `operator=` de `std::vector<atn::Light>`, `std::vector<atn::Camera>` (weak 0x8052E980, 0x8052EEBC, en
  NewCampTask) esten definidos en `std/vector.h` (cabecera compartida). `CharaLightData_unk::operator=`
  implicito solo no basta (sigue dentro del presupuesto: todo inline, 29 %).
- Mientras tanto: aceptar las dos bajadas de M (son ruido de registros de dos funciones estructuralmente
  NONMATCHING) a cambio de las 5 funciones al 100 % de M.

## Decisiones tomadas sin el usuario

- Ninguna de codigo. No se commitea codigo (lo pide la cola si no sale).

## Para el port

- Sin cambios.

## Hallazgos

- La copia implicita de `atn::Camera` por asignacion materializa el puntero de `Frustum_unk::mBounds` igual con
  struct que con dos miembros en una prueba aislada; solo el constructor de copia distingue los dos.
