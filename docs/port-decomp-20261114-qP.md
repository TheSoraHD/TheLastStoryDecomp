# port-decomp-20261114, cola P (eventos de la primera escena)

Rama `agent-P`. Referencia: `report.json` de `$REF` (15401).

## Resultado por funcion

### Event/Puppeteer_CharaTask (88 -> 91/111 al 100 %, 0 % 6 -> 2)

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
- `Actor::fn_8000FAE8`: **equivalencia**: los nombres de nodo son `"op_hand_line"` (`fn_8009373C(.., 0)`) y
  `"seatpoint"` (`GetNodeMtx_unk`), no `"set_y"`/`"z"` (offsets 0x48/0x55 del pool del original). **Port**: el
  mismo error en `src/Event/Puppeteer_CharaTask.cpp` lineas 861 y 887.

## Cambios de firma o de layout (para el port)

- `include/Event/Actor.h`: `ActorAnim_unk::m00` es `atn::SimpleVector<PoseKey_unk>` (incluye
  `Event/PoseKey_unk.h`); `ActorKey2C_unk` eliminado (mismo tamano 0x2C). Fuera los casts de Puppeteer_CharaTask y
  ActionActorPosition. Renumera las `@N` de las 33 unidades que incluyen `Event/Actor.h`: re-emparejadas
  (`pdbatch.sh` + reasignacion por posicion para los floats repetidos, ver hallazgos).

## Decisiones sin el usuario

- Inlines de `atn::Model` y `atn::Task` definidas en el `.cpp` del primer usuario en vez de en su cabecera (coste
  de re-emparejar 40 / 367 unidades).

## Hallazgos

- `pairdata --relabel` no resuelve floats repetidos ("ambiguous"): se reasignan comparando el `.o` viejo y el nuevo
  por (seccion, offset) y llevando el nombre a la direccion de `symbols.txt` (script de una vez en el scratch).
- Pool de Puppeteer_CharaTask: `""`, `"pc101_"`, `"up_"`, `"lo_"`, `"wa_"`, `"ga_"`, `".chr"` sin ninguna referencia
  en el `.dol` (entre `"seatpoint"` y `"eye_R"`): una funcion quitada por el enlazador; sin escribir (no hay codigo
  del que sacarla).
