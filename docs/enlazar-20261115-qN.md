# enlazar-20261115, cola N (parche de `std/string.h` de la cola H, rehecho y limpio)

Agente N, rama `agent-N`, 2026-11-15 17:23 -> 18:45. Decision para el usuario: **no mergear sin su permiso**.

## Resultado

Parche de la cola H (`docs/enlazar-20261115-qH.patch`) aplicado sobre la integracion actual (`bd0a966`, 15578):
`char_traits<T>::length(const T*)` nuevo en `include/std/string.h`, `basic_string(const T*)` mide con
`Traits::length(s)`, y `reserve(size_type res_arg)` -> `reserve(size_type)` para no gastar un `@N`.

| Paso | funciones | lost | bajadas fuzzy | labelcheck/rep100 peores |
|---|---|---|---|---|
| referencia (`$REF`, integracion actual) | 15578 | - | - | - |
| solo cabecera (build completo) | 15606 (+28) | 0 | 5 | 127 unidades |
| + pdbatch de las 127 + symkeepat | 15608 | 0 | 3 | 27 |
| + lblmap/lblseq/datafix/valfix en las 27, pdbatch, symkeepat | 15608 | 0 | 3 | 2 |
| + Em305Vampire a mano, MapTask/CharaTask/CampSaveLoad/Model | 15608 (+30) | 0 | 0 | iguales (PRECOMMIT OK) |

- +30 funciones al 100 %: las 28 de la cola H (ctores de Em201ZanGurg, Em201ZanGurgFinal, Em202SpiderBase,
  Em204Mulue(Mp), Em308InvertCrab/ParmCrab, Em101*Zesya*, Em203GreaterDaemon*, Em005DoppelGanger, Dialog,
  DialogHead, D2A_Task, FileList, Loader, MaterialParser, ptr_string_base_char_0, CharaTask, EnemyJill,
  MapPartsLocatorSet, SpecialEnemyBase) + MapTask `fn_804A06B4` y `fn_804A0BCC` (antes 99,91/99,80 %).
- Las 5 bajadas, resueltas sin perder subidas:
  - MapTask `fn_804A06B4`/`fn_804A0BCC` -> **100 %**: el nombre del modelo en un local
    (`const char* n = ...->mModel.fn_8008B130(); std::string(n)`) en la rama de tipo 0.
  - CharaTask `fn_801371AC` 99,67 -> 99,70 (report): `const char* n = csv.fn_8005B9CC(); std::string path(n);`
    en `MOTION_SET`.
  - CampSaveLoad `vf0C` (objdiff 92,75 -> 93,14): `const char* dir = "data/texture/"; std::string path(dir);`.
  - atn/Model `fn_8008BD10` (objdiff 97,06 -> 97,09): `model->mLods[0].mHandle.fn_80473F18()` en vez de
    `model->fn_8008B130()` (el original llama a la de LoadHandle directamente).
  - Resultado: `fuzzdrop` 0.
- Unidad que enlaza: `game/Em201ZanGurg` (`"matching": true`, 736 bytes de datos), `main.dol: OK` en build
  limpio; `poolorder` igual; `poolcheck` avisa de 0.7/2.44346/25 en `.sdata2` (copias debiles que el enlazador
  quita: el `.dol` sale identico).
- Model: tras cambiar el fuente, `@22781` (0x80880C14, 255.0) pasa a `@22779` a mano (labelcheck).

## Re-emparejado

- `symbols.txt`: ~3750 lineas renombradas, solo `@N`/`@stringBase` (symkeepat). Nombres no-`@N` que pdbatch
  pone y que duplican definiciones al enlazar (revertidos; 23 en la primera pasada, 12 en la segunda):
  `@STRING@__allocate__Q23std33vector<Q23atn17TextAnimEntry_unk>FUi` (.data 0x807798A0),
  `@STRING@__allocate__Q23std24vector<Q23std9vector<f>>FUi` (0x807798BC),
  `@STRING@clear__Q23std8ios_baseFQ33std8ios_base7iostate` (0x80783CA8),
  `__vt__Q25boost17bad_function_call` (0x8078CB00), `__vt__Q43std3tr16detail57shared_ptr_deleter<...>` (0x8078CB08),
  `__vt__Q23std13runtime_error` (0x8078CB10), `@STRING@__allocate__...EffectEntry24_unk` (0x807945D0),
  `@STRING@__allocate__...ActorPair_unk` (0x807945EC), `@STRING@Resize_unk__Q23atn14SimpleArray<i>Fii[@0]`
  (.sdata 0x8087D6E8/EC), `@STRING@Resize_unk__20TextAnimArray_unk<f>Fi[@0]` (0x8087D948/4C),
  `@STRING@Resize_unk__42TextAnimArray_unk<...>Fi[@0]` (0x8087D950/54),
  `@STRING@Reserve__Q23atn42D2A_PtrArray_unk<...>FUii[@0]` (0x8087DB00/04),
  `@STRING@__put_float_unk<c>...[@0]` (0x8087DC10/14), `@STRING@Resize_unk__Q23atn38SimpleVector<...ActorKey10_unk>FUi[@0]`
  (0x8087E4C0/C4), `sCrcTable_unk` (.rodata 0x807348A0), `sLwKeys` (.data 0x807931A0), `sSeLogName_unk`
  (.sdata2 0x80886F10).
- `lblseq.py` de TextAnimation renombro `lbl_807347E0` (de CompositeIntensityTintMulti, linea protegida de
  `comun.md`) a `@8737`: devuelto a mano.
- Em305Vampire: el original repite el valor en 0x808847EC y 0x80884800 y antes los dos se llamaban `@12770`;
  pdbatch los separo (`@12441`/`@12778`) y `fn_802EBBA8` quedaba en REP+: 0x808847EC vuelve a llevar el mismo
  nombre que 0x80884800 (`@12778`).

## Unidades que enlazan (bytes)

- `game/Em201ZanGurg`: 736 bytes de datos (con el parche; sin el no se puede).

## NONMATCHING con motivo

- Quitados los dos de MapTask (`fn_804A06B4`, `fn_804A0BCC`).

## Cambios de splits

- Ninguno. `symbols.txt`: solo renombrados `@N`.

## Para el port

- Ninguno de tipos ni firmas. `include/std/string.h`: `char_traits<T>::length` nuevo; `reserve` sin nombre de
  parametro.

## Decisiones tomadas sin el usuario

- Ninguna de integracion: la rama queda para que el usuario decida.

## Hallazgos

- Con `Traits::length` en el ctor de `std::string`, pasar el `const char*` por un local con nombre cambia el
  orden de colores de puntero/longitud (MapTask, CharaTask, CampSaveLoad).
- `lblseq.py` puede renombrar `lbl_` de otra unidad contigua en `.rodata` (0x807347E0): mirar las lineas protegidas.
- pdbatch separa dos `@N` iguales del original que antes compartian nombre (valores repetidos en `.sdata2`):
  si sale un REP+ de una sola etiqueta, volver a darles el mismo nombre.

## Pendientes

- Decidir el merge de `agent-N` (cabecera + ~3750 renombrados de `symbols.txt`): choca con cualquier rama que
  toque `symbols.txt` a la vez; mejor mergearla sola y re-emparejar las demas despues.
