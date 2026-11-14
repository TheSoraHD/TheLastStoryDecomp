# Cola J (cabeceras-20261114): restos de la regla 10 de G y H

## 1. `Event::PathLengths_unk` en WindyItem, PathData_unk y ActionActorPosition: NO HECHO

Siguen con `extern "C" fn_8032B6F8`/`fn_8000D844` (nombres que ya no existen en `symbols.txt`). Probado en
ActionActorPosition (`vf0C__Q25Event19ActionActorPosition`, 98,67 % en HEAD) y PathData_unk:

- Locales `PathLengths_unk xs(n)` y el resto igual (version de G): ctor y dtor se inlinean (unicos candidatos de
  `Build_unk`, caben en el presupuesto R3). PathData_unk 97,70 -> 29,63 %.
- Destructor con 40 / 150 / 400 asertos vacios: solo con 400 se llaman ctor y dtor, y aun asi baja (96,16 %).
  Descartado (absurdo y toca una cabecera de 38 unidades).
- `mX.resize(n - 1)` y `mLengths.resize(n - 1)` reales (los `resize` son candidatos grandes: `Build_unk` se pasa
  del presupuesto y llama a ctor, dtor y resize como el original). ctor/dtor salen llamados con su nombre, pero
  97,41 %: (a) las llamadas a `resize` piden renombrar `fn_8032B314` -> `resize__Q23std31vector<Q25Event13PathCubic_unk>FUi`
  y `fn_8032BE88` -> `resize__Q23std9vector<f>FUi` (y que Em403WarLion llame a `resize` en vez de sus envoltorios
  `extern "C" inline`); (b) las ranuras de pila de xs/ys/zs salen al reves (xs en 0x74, el original 0x5c) y el
  puntero del trazado no se guarda en r29 (el original hace `mr r3, r29` en cada `fn_8032B7F0`).
- Orden de las ranuras: con `Build_unk` dentro de `SetPath_unk` dentro de vf0C (dos niveles de inline) las
  locales declaradas xs, ys, zs quedan en orden inverso; con un solo nivel (cuerpo de SetPath_unk escrito en
  vf0C o en Em403WarLion `fn_8032AF90`) quedan en orden directo. Array `PathLengths_unk v[3] = {n, n, n}`,
  declaracion unica `xs(n), ys(n), zs(n)` y bloque anidado: no lo arreglan.
- Hallazgo: 0x8000D844 lo llama tambien `atn/TextAnimation` (`fn_800DAB68`) sobre un `std::vector<float>` local,
  y 0x8032B6F8 es el `vector(size_type n)` de `std/vector.h` (su comentario ya lo dice). Es decir, ambos son
  `std::vector<float>` (ctor y dtor), no una clase derivada: `PathLengths_unk` es un apano. El arreglo real
  esta en `std/vector.h` (dtor de `vector<float>` no vacio, con el nombre `__dt__Q23std9vector<f>Fv`), cola A3.

## 2. `GameTask::fn_803729B4`: 98,87 -> 98,89 % (nombres arreglados)

- `symbols.txt`: `fn_802A4170` -> `__ct__Q23std19vector<P9EnemyTask>Fv`, `fn_802A4184` ->
  `__vc__Q23std19vector<P9EnemyTask>FUi` (copias debiles de `std::vector<EnemyTask*>`). Sin tocar `std/vector.h`.
- `game/Em203GreaterDaemonRedMp.cpp`: fuera las definiciones `extern "C"` con self; instanciacion explicita
  `template std::vector<EnemyTask*>::vector();` y `template EnemyTask*& std::vector<EnemyTask*>::operator[](size_type);`
  delante de `fn_802A3D80` (detras de su primer uso MWCC da "redefined"); `fn_802A3D80` (0 %) usa `targets[i]`.
  Las dos copias siguen al 100 % con su nombre real.
- `game/Em206CocoonMp.cpp`: `list[rand() % n]` en vez de `*fn_802A4184(&list, ...)`; vf104 97,79 -> 97,81 %.
- Lo que queda en fn_803729B4: el ctor vacio de `atn::Matrix3x4` del bucle de rayos (`atn::Matrix3x4 m;`), que
  el original no llama (bloqueo conocido, `docs/trucos-mwcc.md`), y registros.
- Re-emparejado: pdbatch de las tres unidades + lblmap en Em206CocoonMp y Em203GreaterDaemonRedMp;
  `lbl_808841C0` (solo lo usa Em206CocoonMp) -> `@18117` local.

## 3a. `PostFx*::vf10` sobrescribe `IRender::vf10(RenderManager*)`

- Las ocho `PostFx*` (Bloom, ColorMatrix, Damage, Dof, Gamma, GodRay, MotionBlur, Rain) declaraban
  `vf10(void*)`: una virtual nueva en la ranura 0x20 y la 0x10 pura (vtable de 0x40 frente a 0x20 del original).
  Ahora `vf10(RenderManager* view)`: vtable igual que el original (0x20, vf10 en 0x10). `symbols.txt`: los ocho
  `vf10__Q23atn<N>PostFx<X>FPv` -> `...FPQ23atn13RenderManager`. Porcentajes iguales; precommit OK.

## 3b. Una sola `atn::VideoTHP` (`include/atn/VideoTHP.h`)

- La cabecera tenia una vista sin bases (virtuales `vf0C..vf3C` sin argumentos, `int vf30()`); ahora declara
  `DebugGroup_unk`, `atn::Vector2f_unk`, `atn::Video : IRender` y `atn::VideoTHP : Video` reales (las de
  `atn/VideoTHP.cpp`, que ahora la incluye). Sin cuerpos inline en la cabecera ni nombres de parametro (para
  no renumerar `@N`): `Video::Video`, `~Video`, `VideoTHP::SetName_unk` y los dos miembros de `DebugGroup_unk`
  se definen `inline` en VideoTHP.cpp. Usan la cabecera TutorialTask (via `game/TutorialTask.h`: tambien
  GameTask, CampSaveLoad, NewCampPartsHint, NewCampPartsStatus) y Event/MotionData.
- Las inline de `IRender.h` renumeraban `@N`: pdbatch de las siete unidades y dos renombrados a mano en GameTask
  (0x808857A8/AC: `@29544`/`@29545`; lblmap completo bajaba vf10 y fn_80371790 0,01-0,02). Neutro, precommit OK.
- Pendiente: `Event/MovieState_unk.h` (vista de `VideoTHP` para ActionEffectPlay y `Event::Manager::fn_8048E9A4`,
  con `vf3C(int, float)` frente a `vf3C(float, int)`): cambiarla toca la firma de `Manager::fn_8048E9A4` en
  `Event/Manager.h` (cabecera de muchas unidades).

## Decisiones tomadas sin el usuario

- Punto 1 dejado como estaba (la forma con locales reales baja en las tres unidades).
- Notas comunes `docs/cabeceras-20261114.md`: ya venia modificado en la copia (dos lineas quitadas) sin ser mio; no lo incluyo en mis commits.

## Para el port

- `include/atn/VideoTHP.h`: `atn::VideoTHP` es ahora la clase real (bases `Video`/`IRender`); antes la vista
  declaraba 13 virtuales sin argumentos (en TutorialTask `vf30()` devolvia `int`, ahora `unsigned int`). Layout
  igual (0x98). Ninguna unidad del port.

- `atn::PostFx*::vf10(void*)` -> `vf10(RenderManager*)` (ahora sobrescribe `IRender::vf10`; antes la vtable
  tenia una ranura de mas). Ninguna unidad del port.

- `fn_802A4170`/`fn_802A4184` ya no existen: son `std::vector<EnemyTask*>::vector()` y `operator[]`
  (instanciacion explicita en Em203GreaterDaemonRedMp). Ninguna unidad del port cambia.

## Hallazgos

- Una instanciacion explicita de un miembro definido dentro de la clase tiene que ir antes de su primer uso
  implicito (despues MWCC da "object redefined"); la copia debil sale igualmente detras del primer llamador.
- Locales con constructor de una inline dentro de otra inline (dos niveles) reciben ranuras de pila en orden
  inverso al de declaracion; con un nivel, en orden directo (ActionActorPosition frente a Em403WarLion).
