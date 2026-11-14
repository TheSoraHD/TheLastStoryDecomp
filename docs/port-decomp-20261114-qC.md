# port-decomp-20261114, cola C (camaras)

Agente C, rama `agent-C`. Unidades: `game/GameCamera_unk`, `game/FinishCamera_unk`.

## game/GameCamera_unk

- `GameCamera::fn_8037F9AC` (0x8037F9AC, 12308 bytes; camara normal detras del lider): escrita (sin DRAFT),
  NONMATCHING 93,3 %. Firma final `void GameCamera::fn_8037F9AC(const CharaTask*)` (sin cambios). Diferencias:
  huecos de pila de los temporales inline (el marco es 0x10 mas pequeno), dos gpr guardados mas en el original
  (guarda en r21-r25 las direcciones de los temporales del barrido) y la disposicion del bucle del stick
  (ble/b frente a bgt; probadas cuatro formas: flag, `break`, `return` en inline y `continue`).
  Hallazgos: el original llama a `GameCameraParam_unk::fn_8037F830` con `bl` (no es inline) y niega con
  `Negate_unk` inline (el `operator-()` de atn es una funcion real); la segunda direccion (tras el giro del
  stick) reutiliza `offset`, no `dir`; `eye` se asigna desde un temporal (`eye = offset + target`).
- `GameCameraParam_unk::fn_8037F830` (0x8037F830): 63,8 % -> **100 %**. Deja de ser inline en la clase: declarada
  en `GameCamera.h` (sin nombres de parametro) y definida en `GameCamera_unk.cpp` entre fn_8037F744 y
  fn_8037F9AC; el cuerpo calcula los 14 valores en locales y guarda m38 antes que m34.

- `GameCamera::fn_80384188` (0x80384188, 6588 bytes; modo cadera/apuntar m5D0 con los dos sticks, zoom mC1C,
  fijado de enemigo m860 con BattleTask::fn_801002DC): escrita desde el asm (no estaba en el port), sin DRAFT,
  NONMATCHING ~33 %: aqui pasa el presupuesto de inlining y llama a todos los operadores de Vector3, el original
  lo inlinea todo. Bisecado: el corte esta en el bloque del limite de pitch + offset + primera colision (tres
  fn_800109E0 y dos fn_80139F60); helpers inline para el codigo repetido no bastan, y copias de Vector3 sin
  `__VecCopy` (probado con una copia de `include/` aparte) tampoco: sigue sin saberse que lo deja por debajo en
  el original. Dudas: `fn_80050900(p, line, out)` (punto mas cercano de la recta) y `target = dir * mDist` en el
  limite de pitch (sin sumar pos; asi en el asm).
- `GameCamera::fn_80385B48` (gemela de fn_80384188): sin escribir (mismo problema de presupuesto).

### Cambios de layout/firma (para el port)

- `GameCamera.h`: `CharaTask* m860` (antes int) y `float m864` (antes bytes): enemigo fijado y su mezcla.
- `GameCamera.h`: `GameCameraParam_unk m3E8` (antes `m3E8[]`, `float m41C`, `m420[]`: es la mezcla de
  parametros de fn_8037F9AC; el constructor de GameCamera ya no inicializa `m41C`, lo hace el constructor por
  defecto de `GameCameraParam_unk`), `atn::Vector3 m960` y `atn::Vector3 m978` (antes arrays de bytes).
- `GameCameraParam_unk::fn_8037F830` ya no es inline: el port necesita su definicion de `GameCamera_unk.cpp`.

### Decisiones tomadas sin el usuario

- Quitar la definicion inline de `fn_8037F830` de `GameCamera.h` renumero (-10) los `@N` de las 148 unidades que
  incluyen `GameCamera.h`: re-emparejadas con `pdbatch.sh` (solo `@N` en `symbols.txt`; ninguna baja). Se
  devolvieron `lbl_8087D6E8`/`lbl_8087D6EC` y se deshizo un `sInstance__9SetConfig` puesto por pairdata en
  `lbl_80885CB0` (.sdata2). Las etiquetas que pairdata dejo ambiguas se emparejaron a mano (desplazamiento -10
  o por los pares de instrucciones de objdiff) en Em206CocoonMp, GameEventScript, LastCocoonEgg/Imago,
  NewCampTask, PacketPack, Em2xx...: labelcheck y rep100 quedan iguales que la referencia.

## game/FinishCamera_unk

- `FinishCamera_unk::fn_803792F0` (0x803792F0, 13216 bytes; actualizacion de la camara de remate): escrita desde
  el asm (el port no la tenia), sin DRAFT, NONMATCHING 94,4 %. Firma `void fn_803792F0()` (sin cambios).
  Diferencias: huecos de pila y registros de los temporales (el original guarda las matrices de las curvas en
  r29; aqui se guardan cinco gpr mas). Para quedar por debajo del presupuesto de inlining (como el original, que
  lo inlinea todo salvo `fn_80379104`/`fn_80378D34`) las curvas se suman a mano en lugar de con `AddCurve_unk`
  (con el helper, la funcion pasa el presupuesto y llama a todos los operadores de Vector3: 38 %), y
  `fn_80379104` lleva `__declspec(noinline)` (forma aceptada de la regla 2: el original la llama con `bl`).
  Modos: 0 de la camara del campo al objetivo, 1 libre con el stick 160 frames, 2/3 curvas xsca (m35C, o m360
  visto desde el objetivo), 5 curvas por personaje de mEntries con desenfoque en los extremos, 6 todos a la vez;
  con m350 > 0 (cuenta de GameTask) giro a camara lenta entre m32C/m320 y m338.
- `fn_803786FC`: firma real `void fn_803786FC(int frames)` (antes `CharaTask*`; todos los llamadores pasan 0 y el
  valor va a m350/m354, que son cuentas de frames). Renombrado en `symbols.txt` a `fn_803786FC__16FinishCamera_unkFi`;
  sigue al 100 %.
- Datos del split definidos en el fuente: `lbl_8087DCE0..DCEC` (.sdata, 1.0/0.3/1.0/0.3).

### Cambios de layout/firma (para el port)

- `FinishCamera_unk.h`: `int m350`, `int m354` (antes `CharaTask*`; GameTask ya los trataba como int),
  `atn::Vector3 m320`, `atn::Vector3 m338`, `float m39C` (antes arrays de bytes), `fn_803786FC(int)`.
- `XsfCurveSet_unk` (local de FinishCamera_unk.cpp): `fn_805BF1E8(int)` y `float m14` (ultimo frame).

### Pendientes

- `m360` es un segundo `Xsca_unk*` (fn_803792F0 llama a su `fn_805BDCBC`), pero `fn_8037C690(Xsca_unk*, int)` lo
  recibe como int desde GameTask (otra cola): queda `int` con un cast en fn_803792F0. Pasar a
  `fn_8037C690(Xsca_unk*, Xsca_unk*)` cambia el nombre mangled y los llamadores de GameTask.

## Hallazgos de una linea

- FinishCamera fn_803792F0: con `AddCurve_unk` (inline con copia de Vector3) la funcion pasa el presupuesto de
  inlining (38 %); con las curvas sumadas a mano lo inlinea todo (94 %).
- `remap.py` (scratch, por pares de instrucciones de objdiff) empareja `@N` que pairdata deja ambiguos, pero hay
  que aplicarlo una sola vez por split: los nombres del asm objetivo son los del ultimo `configure`.

- Una funcion inline definida en una cabecera consume `@N` en cada unidad que la incluye (aqui 10 por
  `fn_8037F830` con tres parametros con nombre; la declaracion con nombres consume 3, sin nombres 0).

## Resultado

- Funciones: fn_8037F9AC 0 -> 93,4 % (escrita), fn_803792F0 0 -> 94,4 % (escrita), fn_80384188 0 -> ~33 %
  (escrita, sobre presupuesto), fn_8037F830 63,8 -> 100 %,
  fn_803786FC 100 % con su firma real. report.json 15389 -> 15390, lost 0, sin bajadas, PRECOMMIT OK.
- poolcheck de GameCamera_unk: aviso de `0.2f` y una cadena (unidad no enlazada; sin comprobar si ya estaba).
