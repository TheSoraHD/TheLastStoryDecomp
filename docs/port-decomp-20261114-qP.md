# port-decomp-20261114, cola P (eventos de la primera escena)

Rama `agent-P`. Referencia: `report.json` de `$REF` (15401).

## Resultado por funcion

### Event/Puppeteer_CharaTask (88 -> 91/111 al 100 %, 0 % 6 -> 1)

- `atn::Model::CalcMatrixStack(ModelNodeCallback_unk)` (0x8000D430) 0 -> 100 %: inline de `atn::Model` definida
  en `Event/Puppeteer_CharaTask.cpp` (unica copia debil del `.dol`): `ModelBase::CalcMatrixStack((const
  MotionBuffer*)m21C.mData, callback)`. No se pone en `atn/Model_unk.h` para no tocar las ~40 unidades que la
  llaman con `bl` (pendiente: moverla a la cabecera con re-emparejado en lote).
- `atn::SimpleVector<Event::PoseKey_unk>::~SimpleVector` (0x8000D7DC) 0 -> 100 %: sale solo al dar a
  `ActorAnim_unk::m00` su tipo real.
- `atn::Task::vf18(Task*)` (0x80013CAC) 0 -> 100 %: inline vacia, definida en el `.cpp` (en `atn/Task.h`, con o sin
  nombre de parametro, renumera las `@N` de las 367 unidades que la incluyen; pendiente para un lote propio).
- `basic_string::reserve` (0x80013DC4) 0 -> 99,9 % y `do_replace` (0x80013F78) 0 -> 90,3 %: el fuente incluye
  `std/string_members.h` (las copias debiles del `.dol` estan aqui y sus mensajes en el pool). `reserve` solo
  difiere en el offset de su cadena: al pool le faltan 7 cadenas sin referencias (ver hallazgos).
  `do_replace` llega a 99,3 % con dos cambios en `std/string_members.h` (`size_type tsz = tmp.size();` antes de
  la llamada recursiva y `j1 = tmp.mFlags.mIsLong ? tmp.mLong.mData : tmp.mShort.mData;`), pero renumeran las
  `@N` de las 125 unidades que incluyen la cabecera (~2800 renombrados en `symbols.txt`, unidades de otras
  colas): **no aplicado**, pendiente para un lote propio. Lo que queda: el original vuelve a medir `j2 - j1`
  para el `memmove` de la rama que reserva (con referencias `const T* const&` en un inline sale la resta, pero
  cambia la asignacion de registros).
- `Actor::fn_8000FAE8`: **equivalencia**: los nombres de nodo son `"op_hand_line"` (`fn_8009373C(.., 0)`) y
  `"seatpoint"` (`GetNodeMtx_unk`), no `"set_y"`/`"z"` (offsets 0x48/0x55 del pool del original). **Port**: el
  mismo error en `src/Event/Puppeteer_CharaTask.cpp` lineas 861 y 887.

### Event/ActionActorPosition (34/51 al 100 %, sin cambios; solo NONMATCHING)

- `ActionActorEyes::fn_80553A74` 95,8 -> 98,5 %, `ActionActorEyes::vf08` 92,4 -> 94,8 %, `ActionActorEyes::vf0C`
  96,5 -> 98,3 %, `ActionActorNode::vf08` 96,1 -> 96,9 %: grados por `atn::DegToRad_unk`, desplazamientos
  aleatorios sin fusionar (`float v = 2*r*rand; x = v - r;`), y en `vf0C` el objetivo y el nodo leidos y sin usar
  (como en fn_80553A74: el original conserva la comprobacion muerta del numero de parametros).
- **Equivalencia / port**: en `ActionActorEyes::vf08` los argumentos de `Set_unk(x, y, 0)` los evalua MWCC de
  derecha a izquierda: el **primer** `rand()` va a `y`. Ahora el fuente lo hace explicito con locales; en el port
  (`src/Event/ActionActorPosition.cpp:880`) el orden depende del compilador (clang: izquierda a derecha).
- Pendiente (regla 10, no se puede resolver solo en esta cola): `Actor::SetPath_unk`/`PathSpline_unk::Build_unk`
  destruye sus vectores con `extern "C" fn_8000D844`, nombre que no existe (0x8000D844 es
  `__dt__Q25Event15PathLengths_unkFv`): en el port es un stub (`stubs_auto.S`). Lo correcto son locales
  `PathLengths_unk xs(n)` con un ctor `PathLengths_unk(unsigned long)` = 0x8032B6F8, que hoy es `extern "C"
  fn_8032B6F8` definido en game/Em403WarLion (y usado por gmk/WindyItem y game/PathData_unk): renombrarlo toca
  esas tres unidades.

### Event/ActionActorBehavior (11/14 al 100 %, sin cambios)

- `Sequence::fn_80550254` (frame de la secuencia) 63,5 -> **99,4 %**, reescrita desde el ensamblador con los tipos
  reales (antes offsets crudos y `extern "C"` con nombres inexistentes). **Equivalencia**: la version anterior
  usaba una tolerancia de 1.0 (es 0.00001), mezclaba las marcas de la accion (son inicio `m10`, fin del fundido de
  entrada `m10+m18`, inicio del de salida `m14-m1C` y fin `m14`; vf08 mientras `m19C >= inicio && m198 <= fin`),
  leia el procesador de la clase 0x1F como un puntero (es `fn_8010DB54(param0)`), cortaba con `break` las
  secciones/canales desactivados (es `continue`), guardaba `action->m3C = k` nunca, y el final del corte no
  miraba `m98_8`/`Dialog::fn_802291BC`, no llamaba a `Dialog::fn_802297AC` y pasaba 0.0 como `frames` a
  `GameTask::fn_80370B78` (es `(-1, 0, 0, 0.0f)`). Queda: registros salvados (r21-r24 rotados).
  **Port**: la funcion del port es la version vieja (`src/Event/ActionActorBehavior.cpp:506`): sustituirla.

## Cambios de firma o de layout (para el port)

- `include/Event/Sequence.h`: `Sequence::m1C0` es `Action*` (la accion que retiene la secuencia, antes `int`);
  `void fn_805501DC(Action* arg)` (antes `int`; renombrado en `symbols.txt` a
  `fn_805501DC__Q25Event8SequenceFPQ25Event6Action`). Con el nombre de parametro para no renumerar las `@N`.
  `class Action;` adelantada en la cabecera.
- `src/Event/ActionActorBehavior.cpp`: fuera los `extern "C" fn_80489A50/fn_8048F9B0/fn_80484FBC/fn_804870F4/
  fn_80486F50/fn_8048F184/fn_8048FC7C(void* manager, ...)` (nombres que no existen; ahora metodos de
  `Event::Manager`) y `lbl_80887E20/24` (datos propios: ahora literales); `lbl_8087F0A8` como `GameConfig_unk*`.

- `include/Event/Actor.h`: `ActorAnim_unk::m00` es `atn::SimpleVector<PoseKey_unk>` (incluye
  `Event/PoseKey_unk.h`); `ActorKey2C_unk` eliminado (mismo tamano 0x2C). Fuera los casts de Puppeteer_CharaTask y
  ActionActorPosition. Renumera las `@N` de las 33 unidades que incluyen `Event/Actor.h`: re-emparejadas
  (`pdbatch.sh` + reasignacion por posicion para los floats repetidos, ver hallazgos).

- `dtor_80014250` (0 %): destructor de un `{T* m00; int m04}` con `delete` escalar que solo usa MSL `locale.cpp`
  (tabla de excepciones); sin tipo en el juego del que sacarlo, sin escribir.

## Decisiones sin el usuario

- No aplicar la mejora de `do_replace` (renumera 125 unidades de otras colas).

- Inlines de `atn::Model` y `atn::Task` definidas en el `.cpp` del primer usuario en vez de en su cabecera (coste
  de re-emparejar 40 / 367 unidades).

## Hallazgos

- Quitar el nombre de un parametro en una declaracion de cabecera renumera las `@N` (igual que anadirlo): al
  cambiar el tipo de `fn_805501DC` se mantiene `arg`.
- Las inlines `atn::Task::vf18` en `atn/Task.h` renumeran las `@N` aun sin nombre de parametro.
- MWCC no fusiona `a*b - c` si el producto va en una local (`float v = a*b; x = v - c;`): fmuls + fsubs.

- `pairdata --relabel` no resuelve floats repetidos ("ambiguous"): se reasignan comparando el `.o` viejo y el nuevo
  por (seccion, offset) y llevando el nombre a la direccion de `symbols.txt` (script de una vez en el scratch).
- Pool de Puppeteer_CharaTask: `""`, `"pc101_"`, `"up_"`, `"lo_"`, `"wa_"`, `"ga_"`, `".chr"` sin ninguna referencia
  en el `.dol` (entre `"seatpoint"` y `"eye_R"`): una funcion quitada por el enlazador; sin escribir (no hay codigo
  del que sacarla).
