# port-decomp-20261114 - cola K (restos al 0 % de la partida)

Inicio: 2026-11-14 06:25 CET. Rama `agent-K`. Unidades: `game/AiTarget`, `game/BattleTask`, `game/GameTask`,
`game/MiniMapTask`.

## Resumen

Funciones al 0 % (fnstat) de las unidades de la cola: AiTarget 2 -> 0, GameTask 2 -> 0, BattleTask 10 -> 5
(quedan el constructor y el destructor de `BattleTask`, sus copias debiles `__dt__12HomingBullet` y
`__dt__Q210BattleTask9BetoModel`, y `vf2C__Q23chr11ControlBase`), MiniMapTask 3 -> 3. Al 100 % (fnstat):
BattleTask 105 -> 156/212 por el re-emparejado de etiquetas. Partida 15395 / 18109 sin cambios (las cuatro
unidades no enlazan), lost 0, sin bajadas, labelcheck/rep100 iguales.

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

- `fn_800F8C6C` (0x800F8C6C, 6272 bytes, unidad BattleTask): **escrita, NONMATCHING 89 %** (fnstat; antes 0 %).
  Firma `extern "C" int fn_800F8C6C(BattleTask* mgr, HitResult_unk* out, int outCount, CharaTask* chara,
  HitInfo_unk* data, int id, float angle, int time, int max)` (el float va antes de `time`/`max`, como ya
  declaraba EnemyTrista; devuelve el numero de personajes golpeados). Ataque cuerpo a cuerpo: jugadores y enemigos
  alcanzados (`fn_800FBA9C`) en un `std::vector<MeleeTarget_unk>` ({personaje, esfera, distancia x (3 - 2 cos)}),
  `std::sort` por esa puntuacion y golpe en orden (`fn_800FC410`) hasta `max`; por golpe valido
  (`m38A5C.m64` 1/4/5) los estados del atacante disparan balas (0x28 -> 0x2329, 0x2F -> 0x232A, 0x32 -> 0x232B,
  0x3E -> la bala del estado), efectos (0x34 -> 0x232D) o curan (7-9 -> 0x232F y 0x2330 al atacante y, con 8, a los
  jugadores); `out[i] = {chara, m38A5C.m94}` (banderas del golpe, 8 = critico). Despues mensaje de golpe a los gimmicks dentro del arco (tipo 1 con
  nivel >= 2, tipo 0x10 si el atacante es jugador), sube el medidor (`m38A14`) y registra el golpe en `mShots`.
  Sigue `extern "C"` con el `BattleTask*` delante (la llaman ~40 unidades con nombre C): deuda de la regla 10.
  Resto: el `push_back` del vector va inline en el original (crecimiento de MSL con `fprintf`/`abort`), registros.
- `MeleeSort_unk` / `MeleeMedian3_unk` (0x800FA4FC / 0x800FA9F4, plantillas del `std::sort` de MSL con iteradores
  por referencia, como `Sort_unk` de `atn/RenderSystem_unk.cpp`): **escritas, NONMATCHING 64,5 % / 76,8 %**.
  Renombradas en `symbols.txt` (`fn_800FA4FC`/`fn_800FA9F4` -> nombres de las plantillas); el `static` del desfase
  es `lbl_8087D978`.
- Re-emparejado de BattleTask (`pd.sh` + `pairdata99.py --relabel`): fnstat 105 -> 156/212 al 100 % (etiquetas
  `@N` viejas); `@STRING@__allocate__...vector<MeleeTarget_unk>` coincide con `lbl_80779DD8` del original.

- `GameTask::fn_803729B4` (pose de victoria multijugador, 98,9 %): borrador comprobado contra el original (solo
  difiere el constructor de `atn::Matrix3x4` llamado fuera de linea y registros); quitado el `DRAFT`.

## Pendientes (otras familias o cabeceras compartidas)

- Constructor/destructor de `BattleTask` (y las dos copias debiles): tipar los `atn::EffFile`/`atn::Texture` de
  `BattleTask.h` (renumera las `@N` de ~57 unidades).
- `vf2C__Q23chr11ControlBaseFv`: `virtual int vf2C() { return 1; }` inline en `chr/ControlBase.h`.
- MiniMapTask `vf10(RenderManager*)` y sus copias debiles (`SetPosition`/`SetTarget` de `atn::Camera`,
  `fn_804ABF74` = constructor de `ComDrawTextureMask`): sin tocar; dependen de `atn/RenderManager.h`
  (`SetCamera_unk` inline) y del constructor de copia de `atn::Camera` (cabeceras de otra familia).
- Constructores de `Inventory::ExpEntry_unk`/`LevelUpResult_unk` en `game/Inventory.h` (`fn_80371790`).
- Setters inline de `atn::Model` (`fn_80139F24`, `fn_8013C554`, `SetMotionSpeed`, `SetMotionFrame`) en
  `atn/Model_unk.h` (`fn_80371790`, `fn_803729B4`).
- Bloqueo de las conversiones int -> float de `AIManager::fn_80017128`/`fn_8001794C` (ver hallazgos).

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

- `fn_800F8C6C`: estructuras nuevas locales de BattleTask.cpp `MeleeTarget_unk`, `MeleeTargetLess_unk` y
  `HitResult_unk` (la misma vista que `HitResult_unk` de `game/BattleCharaTask.cpp`: pendiente llevarla a
  `BattleTask.h`, renumera las `@N` de las ~57 unidades que la incluyen).
- **Port (x86-64):** de las 43 declaraciones `extern "C"` de `fn_800F8C6C` en otras unidades, 39 ponen el float al
  final (`..., int time, int max, float angle)`) y 3 en septimo lugar (EnemyTrista y otras). En PowerPC da igual
  (el float va en f1), pero en el ABI de Windows x64 los argumentos van por posicion: el port debe declararla con
  el float en septimo lugar, como la definicion (`id, float angle, int time, int max`). `HitInfo_unk::m50` (bytes) se lee como dos floats (angulo del arco y
  altura): pendiente tiparlo en `HitInfo_unk.h` (`float m50; float m54;`, ya apuntado por la cola B).
- poolcheck de BattleTask: el float 2.44346 de `vfA0__9CharaTask` ya salia antes (no es de esta cola).

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
