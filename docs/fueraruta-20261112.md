Inicio: 2026-11-12 02:29 CET. Funciones al 0 % al empezar (report.json): 972. Al 100 %: 15030 / 18109.

# fueraruta-20261112

Sesion autonoma de 12 h sobre unidades fuera de la ruta de arranque (prompt `claude/prompt-12h-fuera-ruta.md`).

## Resultado

## Cambios de splits

## Decisiones tomadas sin el usuario

- `gmk/WindyItem`: `WindyPath_unk` es la misma clase que `Event::PathSpline_unk` con tres campos mas (mT, m54, m58);
  se anade `WindyPath_unk::Build_unk` (copia de `PathSpline_unk::Build_unk`) en vez de unificar los tipos (la
  unificacion tocaria `Eval_unk` y las funciones ya emparejadas). Pendiente para un lote de regla 10.
- `Em203GreaterDaemonRedMp`: los `@STRING@__allocate__Q23std24vector<14DaemonBolt_unk>FUi` y
  `...<17DaemonThunder_unk>` de su `.data` pasan a `scope:local` en `symbols.txt` (pairnames los renombro con el
  mismo nombre global que los de `Em203GreaterDaemonRed` y el enlazador fallaba por duplicados).
- `atn/EffectDesc_unk`: `EffectRender_unk` pasa a ser `typedef atn::GraphicsDevice_unk` (regla 10: un objeto, un
  tipo); `fn_800763C0` se llama con el prototipo de `atn/GraphicsDevice.h` (casts de las texturas `void*`).
- `include/Event/ActionProcessors_unk.h` (nueva): las 49 clases de procesadores de acciones salen de
  `ActionEffectPlay.cpp`, `ActionCameraMode.cpp` y `ActionActorPosition.cpp` (y la nueva `ActionActorBehavior`)
  para que `fn_8054E574`/`fn_8054F380` las creen y borren. Las tres copias locales de `atn::SingletonNoStatic`
  se quitan (se usa `atn/SingletonNoStatic.h`), que gana un destructor vacio (el `delete` conserva su prueba de
  nulo). `fn_805510F8`/`fn_80551100` = `ActionActorBehavior::vf0C/vf08` (renombrados).

- `game/Em314Ivy`: con vf0C escrita, `fn_803132D0` paso del 100 % de objdiff al 99,95 % por el pool de `.sdata2`
  (REP+); al escribir vf100 el pool vuelve a cuadrar y vuelve al 100 % (PRECOMMIT OK).
- `PopupTask::fn_8054E528` recibe un segundo argumento que no lee (`gmk::RandomItem` lo pasa en r5): firma
  `(unsigned int, int)`, simbolo renombrado; las unidades que incluyen `game/PopupTask.h` renumeran etiquetas
  (`lblmap` en ControlStickByQuark, BattleCharaTask, GameTask, LastCocoonEgg, FlyObjManager).
- `Inventory`: los bytes 0xDB18-0xDB2B pasan a campos (`mDB18`..`mDB28`: mapa del objeto que se anade, escrito por
  `gmk::RandomItem`).
- `SwingPart_unk` (nueva cabecera `include/gmk/SwingPart_unk.h`): la parte de mapa que se balancea; la
  `ModelInstance` es un miembro (el ctor no guarda vtable). `fn_803FFD10` = su ctor y `fn_803FFF40`,
  `fn_804003E0`, `fn_804006E0` = miembros (simbolos renombrados). `MapPartsGimmickLoc::mScenes` pasa a
  `SimpleArray<SwingPart_unk*>` (antes `MapPartsGimmickLocScene_unk`), y su vf14 sube al 100 %.
- `ModelInstance` (vista de `gmk/Lemon.h`): `m80` es el puntero a las matrices de nodo (antes relleno).
- `MP/Text.cpp`: `MpTextEntry_unk` describe el mensaje de la tabla del lobby con sus campos de bits; la tabla
  se lee con una vista `MpTextTable_unk` (mismo objeto que `MpMessageTable_unk` de `MP/MpMessage_unk.h`): deuda
  de la regla 10 apuntada en el fuente.

## Hallazgos

- Los `""` de `new (tag, "", "", 0)` en una inline van a `@STRING@<inline>` de `.sdata`; con una plantilla las
  parejas salen en orden de instanciacion, con sobrecargas no plantilla (una por clase, macro) en orden inverso de
  definicion, que es lo que da el original de `fn_8054E574` (100 %).
- `delete p` con la prueba `if (p)` delante: el original repite el `beq` cuando la clase tiene destructor vacio
  inline (base `atn::SingletonNoStatic`); sin destructor MWCC fusiona las dos pruebas.
- Componentes de un `Vector3` calculados uno a uno (`r.x = ...; r.y = ...; r.z = ...`) en vez del constructor de
  tres argumentos: arregla el orden de evaluacion de la curva cubica de `WindyItem` (93,7 -> 98,6 %).
- Un local declarado sin inicializar y asignado despues (`atn::Vector3 d; d = a + b;`) conserva el temporal y su
  copia (el constructor desde la expresion lo elide): `fn_8059A298`.
- Una constante usada en un bucle se carga en un registro guardado solo si es una variable (`unsigned int mask =
  0x80080000;`) declarada justo antes del primer uso (antes: la primera llamada usa `lis` inmediato).
- `!x` de un bit da `cntlzw/srwi`; el original de `EffectDesc fn_80235BF8` usa `x ^ 1` (`xori`).
- Un inicializador constante de un array local (`float m[2][3] = {...}`) se copia de una plantilla de `.rodata`
  palabra a palabra (lwz/stw); asignar elemento a elemento da `stfs` sueltos.
- Presupuesto de inline (R3) al limite: escribir las asignaciones de un helper pequeno a mano (12 sentencias propias)
  hace que la funcion vuelva a inlinear todo (`WindyItem::vf10`, 36 -> 89 %).

## Pendientes

- Bloque 1 (las cuatro unidades a las que solo les faltan funciones al 0 %), sin avance, todas bloqueadas:
  - `atn/LH_Motion_cont` `fn_804766B0` = copia debil de `Loader::Get_unk<LoadMotion>`, ya instanciada en el fuente
    (99,7 %, mismo problema de registros que las doce copias, `include/atn/LoadManager.h`). Renombrar el simbolo al
    mangled no sirve: `atn/LH_Motion.cpp` (enlazada) la llama como `extern "C" fn_804766B0` y dejaria de enlazar;
    llamarla con `lbl_8087F518->Get_unk<LoadMotion>` haria que LH_Motion.o emitiese la copia (99,7 %) y rompiera el
    `.dol`. Fusionar `_cont` con `LH_Motion` exige antes el 100 % de `Get_unk`.
  - `game/LastCocoonChrysalis` ctor: el original esta por encima del presupuesto de inlining (R3) y llama todo fuera
    de linea; el nuestro lo inlinea todo. Mismo caso que el ctor de `LastCocoonImago`; probado ya en sesiones
    anteriores (ver el `NONMATCHING`). Sin candidato legitimo que falte.
  - `game/GimmickManager` `LoadSeTableTask::vf0C` + `fn_803EF3B0` (`boost::function::operator()`, que solo se emite
    si vf0C no lo inlinea): es el caso abierto de `docs/inlining.md`.
  - `atn/BoneCalc_unk` `fn_8009A5CC`/`fn_8009ADA4`: paired-single (88 y 116 instrucciones `ps_`/`psq_`), asm no
    aprobado en esta unidad. Apuntadas.
