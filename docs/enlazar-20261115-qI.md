# enlazar-20261115, cola I (motor y juego varios a 2-4 funciones)

Inicio 15:18 CET.

## Resultado por unidad

- `game/MenuCommon_unk`: **enlaza**. Funciones 7/10 -> 10/10 (fn_80116EB8, fn_80116FC0, fn_801170C8): el
  accesor inline `MenuMsgTable_unk::Get_unk` como `const` deja la tabla en r0 y el indice en r3
  (`add r3, r0, r3`) como el original.
- `atn/D2A_TaskInstanceBase_64_8`: funciones 47/51 -> 51/51 (vf10 de <64,8>, <32,8> y <8,2> y Update de
  <32,8>, fn_801F69A8) con un accesor `HasTask_unk() const` en `include/atn/D2A_TaskInstance.h` para la
  prueba de mTask de Update: la carga por un camino `const` no se une (CSE) con la de GetEndFrame y se
  relee mTask como el original. **No enlaza**: `main.dol` falla por el orden de las copias debiles (ver
  Pendientes); `matching` sigue en false.
- Probadas sin 100 % (sin cambios en el fuente): `atn/Pad_unk` fn_800A58D0/58F8 (accesores sin const),
  `game/CostumeEditChara` vf14 (accesores/vistas de m680, tipos del indice), `gmk/PassBoard` vf0C (pares
  indexados: estructura igual pero registros permutados; ordenes de declaraciones), `gmk/Brazier`
  fn_80407D24 (radio con local/orden), `atn/LipSync_unk` fn_8007A530 (accesor const de las claves: peor),
  `game/GameSoundControl` fn_803CDF84 (const, locales cur/prev, comparacion invertida: 8 lineas en vez de
  12, sin 100 %), `MP/D2AMpMenuCockpit`/`FriendRoom` (Get_unk const en `MpMsgTable_unk`: sin efecto o
  peor), `atn/NetManager` fn_8050CFC8 (relectura de lbl_8087F628 en Match_unk), `atn/ColliCalc_unk`
  fn_8005AADC (forma y orden de parametros de `__SphereInRangeZ_unk`), `game/CharaC58_unk` fn_8011CAE4
  (punteros const a los puntos: 12 -> 10 lineas, sin 100 %), `atn/D2A_Manager` fn_801F10E0 (layout por
  referencia, puntero local de la prueba), parche `Loader::Get_unk<T>` de qC (predicado const, `last`
  const, predicado por valor: sin cambio).

## Unidades que enlazan (bytes)

- `game/MenuCommon_unk`: 1524 bytes de datos (`measures.total_data`); complete_data 1005158 -> 1006682,
  complete_units 685 -> 686. Build incremental con `main.dol: OK`; poolcheck y poolorder limpios.

## NONMATCHING con motivo

Sin funciones nuevas en NONMATCHING; las probadas conservan su comentario.

## Cambios de splits

Ninguno.

## Para el port

- `include/atn/D2A_TaskInstance.h`: nuevo inline `bool HasTask_unk() const { return mTask != 0; }` en
  `D2A_TaskInstanceBase<N, M>` (usado por Update). Sin cambio de layout ni firmas.
- `src/game/MenuCommon_unk.cpp`: `MenuMsgTable_unk::Get_unk` pasa a `const`.

## Decisiones tomadas sin el usuario

- `HasTask_unk` como accesor `const` (nombre inventado) en vez de un cast a `const` de `this` (mismo codigo).

## Hallazgos

- Un accesor inline `const` cambia el codigo: la carga por un camino `const` no se une por CSE con las
  cargas del mismo miembro por el camino normal (relectura como en el original) y en `a[i]` cambia el
  operando que hereda el registro del resultado (`add r3, r0, r3`).
- Con `template class X<N, M>;` (instanciacion explicita de la clase) MWCC emite los miembros en el orden
  de la declaracion y deja las funciones implicitas que usan (ctor/dtor de una clase anidada) al final del
  grupo; con instanciaciones miembro a miembro las emite tras el primer miembro que las usa.

## Pendientes

- `atn/D2A_TaskInstanceBase_64_8` (51/51 al 100 %, 528 bytes): no enlaza por el orden de las copias de
  `Text_unk` (ctor/dtor) de cada instanciacion: en el original van al final de cada grupo (0x801F6450,
  0x801F8914, 0x801F9474), aqui tras Create/el destructor. Con `template class D2A_TaskInstanceBase<64, 8>;`
  y `<8, 2>` en vez de la lista de miembros el orden de esos dos grupos sale como el original (el
  enlazador quita los miembros no usados). El grupo <32, 8> es tambien una instanciacion de la clase (el
  orden de 0x801F64D0-0x801F8994 es el de la declaracion), pero sus miembros estan escritos como
  `extern "C" fn_801F64D0...fn_801F8598` (14 nombres, `include/atn/D2A_TaskInstanceC.h`) que llaman unas 30
  unidades (GameUI, MenuCommonTask, Shop*, NewCampParts*...). Para enlazar: renombrar esos 14 simbolos a
  los miembros (Create, Update, Draw, GetData, GetEndFrame, GetTrack_unk, SetValue0_unk, SetValue1_unk,
  SetValues1_unk, SetValues2_unk, SetValue3_unk, SetColor_unk(fff), SetText_unk, SetNumber_unk), cambiar
  los llamadores a llamadas de miembro (regla 10) y usar `template class` para los tres grupos. Necesita
  permiso (toca muchas unidades, algunas del port).
- `MP/MpCampMsgTable_unk.h` y `MenuMsgTable_unk` (MenuCommon_unk) y otras vistas locales son el mismo objeto
  `lbl_8087F1E4` con tipos distintos (regla 10).
