# enlazar-20261115, cola L (segunda pasada)

## Resultado por unidad

- `Event/EventCommandDef_unk`: 2/3 -> 3/3, **enlaza** (31220 bytes de datos; build limpio `main.dol: OK`,
  poolcheck/poolorder limpios). `fn_800081C0` 94 -> 100 %: no era el C++ de la funcion sino que era la
  primera del fichero. MWCC planifica distinto la primera funcion de una unidad: tras cualquier funcion que
  opere con sus argumentos (`return a + 1`, `-a`, `a * b`, `&tabla[i]`; no basta una vacia, una que lee un
  global ni una inline/estatica sin emitir) el `li r5, 0` va detras del `addi` como en el original. En el
  original habia una funcion delante que el enlazador quito; repuesta como `fn_Stripped_unk(int i)`
  (`return &sCommands_unk[i];`, cuerpo inventado). Su parametro con nombre renumera las `@N`: `pd.sh`.
- `game/SystemErrorMessage_unk`: 10/11 sin cambio. `ErrorManager::fn_803646C4` 99,86 %: probado ademas (unas 15
  formas): fila o mensaje en un local, idioma en un local, `(&fila[0])[i]`, indice plano `err * 4 + i`, while,
  incrementos cambiados, `y` inicializada fuera, las dos inline como miembros de ErrorManager, orden de los locales
  de DrawString_unk (peor, 36 lineas), orden de los parametros de DrawString_unk. Sin efecto.
- `msc/src/RVL_SDK/dwc/dwc_match`: 98/101 -> **99/101**. `DWCi_CancelMatchingProcess` 99,65 -> 100 % con
  `hasConnections = DWCi_GT2GetConnectionListIdx() != 0;` (en vez de `? TRUE : FALSE`). Quedan
  `DWCi_ProcessMatchCommand` 99,995 % y `DWCi_CloseUnacknowledgedClients` 99,13 % (ver NONMATCHING).
- `atn/ShaderManager_unk`: 3/4 sin cambio; `fn_800C5644` 95,5 -> 98,4 %: la descripcion se carga antes del `new`
  si el `new` va en una inline `NewShader_unk(model, file->m34[j])` (los argumentos se evaluan antes de la
  llamada); con un local `desc` la funcion pasa del presupuesto de inline (mucho peor).
- `atn/RenderSetting_unk`: sin tocar (ctor 97,1 %, 197 lineas de diferencias: orden de constantes de `.sdata2` y
  temporales de color, ya estudiado; no hay atajo a la vista).

## Unidades que enlazan (bytes)

- `Event/EventCommandDef_unk`: 31220 (total_data del report).

## NONMATCHING con motivo

- dwc_match `DWCi_ProcessMatchCommand` 99,995 %: `ip` de TELL_ADDR comparte el registro de `data` (r30; r28 en
  el original). Probado ademas: `ip = *(const SOInAddr*)data`, `*data`, sin `ip` (direccion directa en
  `remoteaddr`), `u32 ip`, `sin_port` antes de `sin_addr`, orden de declaracion. Con
  `remoteaddr.sin_addr.addr = ip.addr;` `ip` deja de compartir r30 pero ip/port/estado salen r25/r28/r26 (original
  r28/r26/r25); combinado con otras formas, peor.
- dwc_match `DWCi_CloseUnacknowledgedClients` 99,13 %: probado ademas `while` con `validPid[i++]` (arregla
  `validPidLen`, pero cambia la estructura del bucle), `validPidLen > i`, `++i`, declaraciones sin inicializar en
  el cuerpo, la llamada dentro del `if`, `++validPidLen`, `result = TRUE` antes del segundo bucle.
- ShaderManager_unk `fn_800C5644` 98,4 %: el contador de LoadShaders_unk en r21 (r16 en el original, los de en
  medio desplazados) y el swap de la realocacion de `std::vector` recarga el tamano que el original sabe que es 0
  (`include/std/vector.h`). Probado: contador `int`, puntero, ruta sin local, `j` unsigned, fichero antes del
  `push_back`.

## Cambios de splits

Ninguno. `symbols.txt`: solo renombrados (`@N` re-emparejadas, `sCommands_unk__5Event`, `sCommandCount_unk__5Event`).

## Para el port

## Decisiones tomadas sin el usuario

- `fn_Stripped_unk` en EventCommandDef_unk: funcion inventada (no esta en el DOL) para reproducir la
  planificacion de la primera funcion, como los `Stripped*_unk` de otras unidades.

## Hallazgos (una linea cada uno)

- `x = f() != 0;` y `x = f() ? TRUE : FALSE;` dan el mismo codigo pero colorean distinto (DWCi_CancelMatchingProcess).
- El `new T(args)` evalua los argumentos despues de la reserva; para cargarlos antes, el `new` en una inline con
  los argumentos como parametros (ShaderManager_unk fn_800C5644).
- La primera funcion solo se ve afectada por funciones anteriores en la planificacion: un stub delante no cambio
  nada en ChaseParam_unk, Em201ZanGurg, ControlShoot, ArrowSet_unk ni CsvBase_char_124_13_34 (su unica funcion
  fallida es la primera del `.text`).

- La primera funcion de un fichero se planifica distinto (orden de `li`/`addi` independientes): si el original
  difiere solo ahi, habia una funcion quitada por el enlazador delante (EventCommandDef_unk fn_800081C0).

## Pendientes

- `include/std/vector.h`: el swap de la realocacion de `vector<T*>` recarga `mSize` (el original lo propaga como 0);
  afecta a ShaderManager_unk fn_800C5644 y quiza a otras push_back.
