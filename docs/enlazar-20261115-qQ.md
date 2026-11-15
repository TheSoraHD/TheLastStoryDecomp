# enlazar-20261115, cola Q (segunda pasada con los trucos nuevos, atn/gmk a una funcion)

Agente Q, 2026-11-15 18:24-19:45. Solo los trucos de la sesion (accesores inline/const, getters por valor,
un puntero local por sentencia, parametros const, `x = f() != 0`, case vacio con `return`), 3-8 variantes por
funcion, sin repetir lo de qB/qC/qE.

## Resultado por unidad

| Unidad | Funcion | Antes | Despues | Estado |
|---|---|---|---|---|
| atn/PostFxDof | fn_800ACCB8 | 97,47 % | **100 %** | **enlaza** (3/3) |
| gmk/OpenBoard | vf10 | 94,8 % | 99,76 % | 12/13, commit (ultima funcion de la unidad) |
| atn/LH_Handle | fn_80473FCC | 99,22 % | 99,56 % | 8/9, commit (ultima funcion) |
| gmk/ShakeCamera | vf10 | 96,74 % | 98,18 % | 8/9, commit (estructura de saltos ya como el original) |
| gmk/HealPoint, gmk/CharaLight, gmk/FallingTower, atn/EffectSequencer_unk, gmk/LandMine, atn/PostFxRain, atn/PostFxMotionBlur, gmk/FlyObjManager, gmk/InteractionSpot, atn/CompositeCache_unk, atn/LH_* (Get_unk<T>) | | | | sin cambios (variantes abajo) |
| atn/TextureDraw_unk, atn/VideoTHP, atn/NandManager, gmk/AutomaticDoor, gmk/BulletPoint, atn/ShaderManager_unk, atn/RenderSetting_unk, atn/Texture_unk | | | | solo revisadas: diffs de registros/pila o de `.sdata2` ya muy probados o fuera del alcance de los trucos |

## Unidades que enlazan (bytes)

- `atn/PostFxDof`: 128 bytes de datos (`measures.total_data`). Build limpio `main.dol: OK`, complete_data
  -> 1039302, complete_units 689 -> 690; poolcheck y poolorder limpios.

## NONMATCHING con motivo

- gmk/OpenBoard vf10 (99,76 %): registros float de la interpolacion de m56C. Probado: `old` tras `t` (peor),
  Scale_unk sin local (igual), `Lerp_unk::Interp_unk(float)` miembro (98,76), `Scale + mFrom` (igual), `cur`
  local (igual).
- atn/LH_Handle fn_80473FCC (99,56 %): lista/elemento/contador del bucle de callbacks en r22/r23/r21 (original
  r21/r22/r23). Probado: lista const, `At_unk(int)`, referencia al elemento, contador declarado antes,
  `(mData + i)->operator()`.
- gmk/ShakeCamera vf10 (98,18 %): rate en f30 (f31 en el original) y el puntero del campo en r7 (r5).
  Probado: `min +=` en RandRange_unk (98,0), `const float&` (81,4), rate construido en sitio (86,8), local del
  campo, `m96C %= 2` (igual).
- gmk/HealPoint fn_8040ED00: `RotY_unk(const GimmickDesc_unk*)` por valor (igual, 93,75).
- gmk/CharaLight vf6C: `group` declarado antes de `name` (igual).
- gmk/FallingTower fn_8041044C: `s` local en Height_unk (99,56), `const Wave_unk*` por sentencia (99,29),
  `b + blend * (a - b)` (igual), `Height_unk(const float&)` (99,41), `Blend_unk(const FallingTower*)` (99,49),
  `Mix_unk(a, b, t)` por valor (99,56), `Clamp_unk(x, lo, hi)` por valor (96,09).
- atn/EffectSequencer_unk fn_802440F0: `elem = fn_802438C4(i)` (95,33), elem tras el test, `bool failed` (igual).
- gmk/LandMine Start: `const GimmickDesc_unk* desc = mDesc` (igual).
- atn/PostFxRain vf10: GetWidth_unk/GetHeight_unk const en PostFxRenderMgr_unk (igual).
- atn/PostFxMotionBlur vf10: `lbl_8087EFA8->m240 != mA4` (igual).
- gmk/FlyObjManager FlyObj::fn_80427870: `Sphere_unk(mPos, r)` (96,99), `GetPos_unk() const` por valor (igual),
  cola en una inline (98,63), estado 3 en una inline (98,71).
- gmk/InteractionSpot Start: Find* const con `const Spot_unk*` (97,71), FindSpot_unk con parametro de salida (95,61).
- atn/CompositeCache_unk fn_800D8458: for con erase, `end` con nombre antes/despues de begin, entry const (99,04),
  test invertido (93,16), cuerpo de Purge en la funcion (97,94).
- `Loader::Get_unk<T>` (con el parche de qC, 99,83 %): predicado con `GetHash_unk() const` y `const LoadCore*`,
  `LoadCore* const&`, hash por `const&`, predicado con nombre (igual); find_if con `return` en el for (96,28).

## Cambios de splits

Ninguno.

## Para el port

Ninguno (ninguna unidad del port tocada).

## Decisiones tomadas sin el usuario

- Commits de subidas de fuzzy en la ultima funcion de tres unidades (OpenBoard 99,76, LH_Handle 99,56,
  ShakeCamera 98,18): ShakeCamera no llega a 99,x, pero arregla la estructura de saltos (lo que queda son
  registros).
- `symbols.txt`: renombrados `@N` de los pools de OpenBoard, PostFxDof y ShakeCamera (las inlines nuevas
  consumen numeros); sin el `data:string_table` de MiniMapTask.

## Hallazgos (una linea cada uno)

- Una inline estatica con los operandos por valor (`DofDepth_unk(float n, float f, float z)`) cambia la
  asignacion de FPRs de una expresion repetida: PostFxDof fn_800ACCB8 al 100 % (con `const Camera_unk&` no).
- Una carga y dos stores (el valor copiado a un temporal y al destino) salen de `dst = Getter_unk(src)` con un
  getter inline que devuelve el `Vector3` por valor (temporal + copia): OpenBoard vf10.
- `x / 2.0f` da `fmuls x, 0.5` con los operandos en el orden del original donde `x * 0.5f` los invierte.
- `if (s == 1) return; if (s == 2) {...}` conserva las dos comparaciones que un `switch` con case 1 vacio pierde
  (ShakeCamera vf10; probar en gmk::TodomeCountUp::vf10).
- Accesores const `Size_unk()`/`At_unk(i)` sobre un vector de `boost::function` y contador `int` arreglan `this` y
  el desplazamiento del bucle (LH_Handle).

## Pendientes

- gmk/OpenBoard vf10, atn/LH_Handle: un paso de registros de la ultima funcion para enlazar.
- gmk/TodomeCountUp::vf10: probar `if (m54 == 1) return;` (fuera de la cola).
