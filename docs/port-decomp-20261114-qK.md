# port-decomp-20261114 - cola K (restos al 0 % de la partida)

Inicio: 2026-11-14 06:25 CET. Rama `agent-K`. Unidades: `game/AiTarget`, `game/BattleTask`, `game/GameTask`,
`game/MiniMapTask`.

## Resultado por funcion

- `AIManager::vf14` (0x80014A5C, 7724 bytes, antes `fn_80014A5C`): **escrita, NONMATCHING 93 %** (objdiff; fnstat
  98,9 %). Firma `virtual void AIManager::vf14()` (nueva en `AiTarget.h`; es la entrada 0x14 de `__vt__9AIManager`).
  Vista de depuracion de la IA: con "DebugMode" (`m50`) las teclas del teclado USB (puerto 0: 0xF1B8/0xF1B2 cambian
  `m78`, el tipo; puerto 4: 0xF1B4/0xF1B6 el personaje anterior/siguiente de su lista) eligen un personaje
  (`m88` TPC, `m8C` enemigo, `m80` amigo, `m7C` jugador, `m84` NPC) y se dibuja un panel con su estado de IA
  (`CharaC58_unk`: estado, modo, contadores, puntos de ruta, areas, `m140`, banderas), los circulos de sus puntos
  de ruta, una linea sobre la cabeza y el cono de vision (`CharaRes_unk::m4C/m50/m58`); con "DrawHate" (`m54`) la
  lista de odio de `BattleTask::mCharas`; con `m58` los 16 valores `m9C` de los scripts. Las tres tablas de nombres
  (estados, modos, tipos) son `static` de fichero al principio de `.rodata` (el original las direcciona desde el
  ancla `lbl_8072FA00`). Resto: asignacion de registros de los punteros (this/chara/scene/res), orden de evaluacion
  de los argumentos de las palabras de banderas ("Task", "Action") y la carga del angulo del cono.
- `AIManager::fn_8001794C` (0x8001794C, 1348 bytes): **escrita, NONMATCHING 97,8 %**. Firma `int
  fn_8001794C(CharaTask*)` (sin cambio). Paso de IA de los NPC (tipo 1 con `mC50 == 1`): solo cada cuarto frame
  (`m90 == mId % 4`; si no, `mC58.m04++` y 0), bloque de entrada reducido y vuelta del estado y de `m140`/`m9C`.
  Resto: el mismo bloqueo que `fn_80017128` (ver hallazgos).

- `GameTask::fn_803731E8` (0x803731E8, 1496 bytes): **escrita, NONMATCHING 93,4 %**. Firma `void fn_803731E8()`
  (sin cambio). Empieza la repeticion del golpe final (estado 7 del campo): `m5598 = 88`, `m559C = 90`,
  `m55A4 = 15`, graba el frame en `m5590`, camara final (`fn_80378610` si no hay repeticion, `m394 = 1`,
  `fn_803786FC(lider == mChara ? 0 : 30)`), congela los gestores y el HUD (descongela `mB08[4]`/`mB20[0]`),
  efectos en pausa, rebobina el `ReplayBuffer`, baja el sonido y la BGM (x0,2), suena `sFieldSounds_unk[1]` en
  `m55A8`, voz de la arena, matriz de color monocroma (guarda la actual en `m55C4` y la velocidad en `m55C0`) y
  mezcla de la camara hacia `m684` escalado (0,5/0,8/0,6) con `m4B4`. Es la inversa de `fn_803737C0`. Resto:
  orden de la segunda copia de la matriz de color (el original carga antes las constantes de la mezcla) y los
  registros del ternario de `fn_803786FC`.

- `GameTask::fn_80371790` (0x80371790, 3556 bytes): **escrita, NONMATCHING 97,3 %** (objdiff; fnstat 77 %). Firma
  `void fn_80371790()` (sin cambio). Pose de victoria tras un combate (estado 9): copia local de
  `Inventory::mExpEntries` (y de `mDF30` si hay remate), para cada miembro con nivel ganado (o el del remate) y
  activo: con repeticion (`lbl_8087F0A8->mD0 == 1`) solo lo anade a la camara final; si no, lo pone en su punto de
  movimiento mas cercano (`fn_803C1560`), busca una direccion libre (4 rayos `fn_8004ED34` girando 90 grados),
  cuenta las habilidades nuevas por nivel (`fn_80219160`/`CharaStatus::fn_801346C8`), pone la animacion (0x177 si
  aprendio algo, 4/5/3 segun el tipo), su matriz (traslacion, YXZ, escala) y lo anade a la camara final con ese
  numero. Despues camara final 5, ventanas de mensajes, popup, efecto de dano apagado y congela el campo; sin
  miembros, `Inventory::fn_8044D028`. Resto: el original construye la copia local con el constructor de
  `ExpEntry_unk` (memset de `LevelUpResult_unk` y copia miembro a miembro), que `game/Inventory.h` no declara
  (cabecera de otra familia, 58 unidades: pendiente), y los setters de `atn::Model` (`fn_80139F24`,
  `fn_8013C554`, `SetMotionSpeed`, `SetMotionFrame`) van inline en el original y fuera de linea en
  `atn/Model_unk.h`.

## Cambios de firma / layout (para el port)

- `AIManager::vf14()` declarada (virtual) en `include/game/AiTarget.h`; `AIManager::m88` pasa de `int` a `NpcTask*`.
  Simbolo renombrado `fn_80014A5C` -> `vf14__9AIManagerFv`.
- `CharaC58_unk::m140` pasa de `unsigned char[0x60]` a `float m140[24]` (valores de los scripts; los `memset` de
  `CharaC58_unk.cpp` siguen igual).
- `CharaLinkTarget_unk` (`CharaTask.h`): `int m04` (id de grupo) dentro del relleno.
- `ResManager` (`game/ResHandle.h`, `lbl_8087EF10`): `int m04` (menu abierto; es el `Mindows_unk::m04` de
  `atn/MindowsCommon.h`, que declara `lbl_8087EF10` con otro tipo: pendiente unificar).
- `AiTarget.cpp`: `lbl_8087F048` declarado como `BattleTask*`.
- `GameTask.cpp`: vista local `LevelSkills_unk` de la tabla de `fn_80219160` (la misma que `EquipSet_unk` de
  `game/CharaStatus.cpp`): pendiente una cabecera comun. `Inventory::mDF30` se lee como `ExpEntry_unk` (sus 0x90
  primeros bytes tienen esa forma; `InventoryFinish_unk` deberia derivar de `ExpEntry_unk`).

## Decisiones y hallazgos

- `symbols.txt` de GameTask: tras `pd.sh` quedaron 5 etiquetas de `.sdata2` sin emparejar (0x8088571C, 0x80885730-38,
  0x808857A8) y dos `lbl_` nuevas que ahora usa el fuente (0x808857E0/E4); renombradas a mano a las `@N` de
  nuestro `.o` (valores comprobados con `poolcheck`). En AiTarget lo mismo con 0x808806F0-F8.
- Ternario sin salto `x == y ? 0 : n` con `li n` + `nor/srawi/andc`: el original tiene la constante en un local
  (`int frames = 30;`); con el literal sale `rlwinm` (como el truco ya apuntado de `andc`).

- Cadenas de formato del original seguidas de un `""` en `@stringBase0`: se escriben con `"\0"` final
  (`"%d\0"`), como en el original.
- Conversiones int -> float en bucles (`m140[i] = out->m18[i]`): MWCC aqui guarda la palabra 0x4330 una vez en el
  prologo (huecos propios) y desenrolla x8; el original la guarda antes de cada elemento en el hueco compartido con
  los float -> int (0x18) y desenrolla x6/x4. No sale con cast, inline, puntero, `unsigned` ni `do/while` (pruebas
  sueltas): bloquea `fn_80017128` y `fn_8001794C`.
