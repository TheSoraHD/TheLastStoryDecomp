# Lote enemies-finish, cola q2

Inicio: 2026-10-22 11:27 CEST. Rama agent-q2 (referencia start-q2: 6832 funciones al 100%).
Cola: game/Em202Spider, game/Em202SpiderBase, game/Em205Ogre, game/Em208Queen, game/Em209SmallGolem.

Estado inicial (report.json, funciones al 100% / total): Em202Spider 4/25, Em202SpiderBase 3/23,
Em205Ogre 14/21, Em208Queen 0/29 (sin fuente), Em209SmallGolem 7/22.

Ninguna unidad queda enlazada (`matching: true`): en todas faltan funciones grandes de estado (vf0C/vf10 o
las maquinas de estados), y muchas de las escritas solo fallan por desplazamientos de @stringBase0/.sdata2
que dependen de esas funciones sin escribir.

## Unidades

- `game/Em202Spider` (4/25 -> 11/25, fn_8027F69C escrita al 98.77%: &mPos en registro y escala por
  componente, 8 variantes): cabecera completa (campos del ctor; color `SpiderColor_unk` en m1598).
  Al 100% en report: dtor, vf3C, vf40, vf7C, vfEC, fn_80281ED8, fn_802829DC, fn_8027F8A8, fn_80281054,
  fn_80281344, fn_80281634. Las que estan al 99.x% en objdiff solo difieren en los offsets de .sdata2/.rodata
  (constantes de vf0C/vf10/fn_8027EB14... sin escribir); el report las cuenta como 100% salvo:
  - ctor 96.39%: r28-r31 de los ctor de std::string.
  - vf14 92.84%: offsets de @stringBase0 y huecos de pila de los temporales del color.
  - vf54 98.97%: r3/r4 de la copia del centro (familia __VecCopy).
  - fn_80281924 99.44%: r4/r5 de la copia del vector cero y r29/r30 del contador (8 variantes).
  - fn_802826A0 99.32%: f0/f3 del 0.0f de ToRotationZ_unk (mismo caso que Em003LesserDaemon::vf24).
  Sin escribir: vf0C (2.3KB, lector CSV con reserve/push_back), vf10 (2.4KB), fn_8027EB14, fn_8027F504
  (llama a los operadores de Vector3 fuera de linea: presupuesto de inline), fn_8027F69C, fn_8027FA7C,
  fn_80280184, fn_802805C8, fn_80281F88.
- `game/Em202SpiderBase` (3/23 -> 14/23): cabecera completa (campos del ctor; puntos base
  `SpiderBasePoint_unk`). Al 100% en report: dtor, vfEC, vf7C, fn_80287788, vf3C, vf40, fn_80287400,
  fn_8028702C, fn_80286CB0, fn_802847CC, fn_802868A0, fn_80284CC8, fn_802857B4, fn_80285B7C, __sinit
  (algunas al 99.x% en objdiff solo por pool).
  - fn_802878EC 99.78%: huecos de pila de las dos restas del bucle (8 variantes).
  - ctor 99.81%: r26/r27 del segundo ctor de std::string (como Em201ZanGurg).
  - vf14 92.87%: mismo codigo que Em202Spider::vf14.
  - vf80 83.15%: orden de las conversiones int->float de `frames` y de los temporales (10 variantes).
  Sin escribir: vf0C (fn_80283004), vf10 (fn_802837F4), fn_80285064 (1.9KB, aguja con matrices),
  fn_80285FA0 (2.3KB).
- `game/Em205Ogre` (14/21 -> 17/21): vf50, vfB0, vfB4 al 100%; vf104 99.99% (solo pool).
  NONMATCHING: fn_802B98B0 98.59%, vf30 99.97%. Sin escribir: vf108 (fn_802B8AA4, 3.5KB).
- `game/Em208Queen` (0/29 -> 21/29, NUEVA fuente `src/game/Em208Queen.cpp` + cabecera
  `include/game/Em208Queen.h`, subclase de EnemyTask; la reina de la batalla de los canones): al 100% ctor,
  dtor, vf0C, vf3C, vf40, vf50, vf54, vf24, fn_802C9880, fn_802C9BF8, fn_802CA490, fn_802CB1CC, fn_802CB74C,
  fn_802CB8A0, vfEC, fn_802CC39C, fn_802CC708, fn_802CC834, fn_802CCA0C (`extern "C"`, la llama
  chr/ControlCannonLift), vf64 y el `__sinit`.
  NONMATCHING: vf10 99.86% (f7/f8 de pi), vf14 99.58% (r28/r29), vf20 99.81% (__VecCopy), fn_802CA8C0
  97.24%, fn_802CAFB8 95.51%, fn_802CB43C 97.60%, fn_802CC19C 97.29% (r3/r4 de copias y orden de los
  productos por 0.05f), fn_802CC4C8 99.65%.
- `game/Em209SmallGolem` (7/22 -> 14/22): vf0C, vf9C, vf50, vfEC, fn_802CF054, fn_802CF3AC, fn_802CF728,
  fn_802CFB24 al 100% (vfEC: el flag de PlayDeathEffect_unk es el argumento arg9 de fn_8023A8B4, no arg8); ctor escrito por el orden de cadenas (85.96%, presupuesto de inline).
  NONMATCHING: vf98 92.9% (huecos de pila, caso abierto de CharaTask::vf98), vf54 98.87% (__VecCopy),
  vf20 98.49%, vf1C 99.99% (pool), vf80 99.71%, __sinit (pool). Sin escribir: vf10
  (fn_802CD73C, 3.3KB).

## Decisiones tomadas sin el usuario

- **symbols.txt (Em208Queen)**: fn_802C7BE4/7CBC/7D3C/7F10/85B0/881C/8930/8A68/92D4/9388/9750/CC0F4/C39C/C708/
  CAAC -> ctor, dtor, vf0C, vf10, vf14, vf3C, vf40, vf50, vf54, vf20, vf24, vfEC, fn_802CC39C, fn_802CC708, vf64
  (por la vtable 0x80786860), y el resto de fn_802C* escritos como miembros; fn_802CCAFC ->
  `__sinit_\Em208Queen_cpp` (local); lbl_80746DEC -> @stringBase0 (local); lbl_807C83B8 -> sShotOffset_unk
  (local, el vector 3.882/98.679/10.846 que tambien usa chr/ControlCannonLift.cpp con su propia copia).
- **symbols.txt (resto)**: renombrados a sus nombres C++ los metodos escritos de Em205Ogre (vf50, vfB0, vfB4,
  vf104, vf108), Em209SmallGolem (vf0C, vf98, vf9C, vf20, vf50, vfEC, vf1C, fn_*), Em202Spider (ctor
  fn_8027C990, vf14, vf3C, vf40, vf54, vfEC y los fn_* escritos) y Em202SpiderBase (ctor fn_80282AE8, vf14,
  vf3C, vf40, vf80, vfEC, fn_* escritos; fn_80287E54 -> `__sinit_\Em202SpiderBase_cpp` local).
  @stringBase0 (local, string_table): lbl_80744EFC (Em202Spider), lbl_80745100 (Em202SpiderBase) y los
  pools de Queen/Ogre/Golem.
- **symbols.txt (vector)**: `@STRING@reserve__Q23atn36Vector_unk<Q29CharaTask10Item14_unk>FUii(@0)` en
  0x8087D9E0/E4 (los "" del reserve inline de m624, para que el pool de Em205Ogre::vfB4 cuadre).
- **game/SpecialEnemyBase.cpp (factoria)**: los case 12 (Em202Spider), 14 (Em202SpiderBase) y 26
  (Em208Queen) usan `new (3, "", "", 0) EmXXX(parent, info)` en vez del `extern "C"` (fn_805A5374 no baja:
  73.25 -> 73.26 en fnstat).
- **include/game/CharaTask.h** (cabecera compartida, firmas): `virtual int vfB0(atn::Vector3* out)` (era
  `void vfB0()`) y `virtual int vf1C(atn::Matrix3x4* mtx, ModelNodeDesc_unk* node)`; CharaTask::fn_80148CC4
  ahora llama a vf1C(mtx, node). Renombrados vfB0__9CharaTaskFPQ23atn7Vector3 y
  vf1C__9CharaTaskFPQ23atn9Matrix3x4P17ModelNodeDesc_unk. Sin bajadas en regress/fuzzdrop.
- **fn_8020A81C** declarado en Em209SmallGolem.cpp con `const char*` (CharaTask.cpp lo declara con un int;
  mismo simbolo C).
- **Em209SmallGolem.h**: m14FC como float[4] (StoneColor), m14B8 como CharaTask*, m1540/m1544 como
  CharaData_unk*.
- **Em208Queen**: m1540 es la d2anime "ui_point_slash.d2a" vista con un struct local `QueenD2A_unk` (como
  MenuD2A_unk); la tabla de mensajes lbl_8087F1E4 como `QueenMsgTable_unk` local (como en Em309Ghoul).
- **splits.txt (Em209SmallGolem)**: fin de .rodata 0x80747098 -> 0x80747080 (aviso del coordinador). Los tres
  doubles lbl_80747080/88/90 solo los usa Em301LShade (fn_802D0B84, fn_802D3C74; xref.py), ninguna referencia
  desde el codigo de Em209SmallGolem. El split de Em301LShade NO se toca (lo ajusta el coordinador al integrar).
- **splits.txt (Em202SpiderBase)**: anadida `.bss start:0x807C8398 end:0x807C83A8`. lbl_807C8398 (color
  0.7/0.2/0.2/1.0 de las aranas tumbadas) lo construye el __sinit 0x80287E54, que es la entrada .ctors
  0x8072D330 de esta unidad; se define en Em202SpiderBase.cpp como `extern "C"` y lo leen Em202Spider,
  Em202ForestSpider, Em202_01_BSpider y Em308ParmCrab. El hueco 0x807C8398-0x807C83A8 estaba sin unidad.
- Nombres inventados: SpiderColor_unk, SpiderBaseColor_unk, ResetAttack_unk, FrontDot_unk,
  PlayerManager_unk/CharaListNode_unk (copias locales de las de CharaTask.cpp/Em002_01BlackSkelton.cpp).

## Hallazgos MWCC

- vf0C con `int result` y la carga en un `if (result != 0) { ...; return 1; } return 0;` da el bloque `li r3,0`
  al final como el original; con `if (result == 0) return 0;` va arriba.
- Interpolacion de color de 4 floats `c = (t - c) * k + c`: operator- con ctor de 4 args, operator+ y
  operator* con local y asignaciones por campo da el orden de fsubs/fmuls/fadds mas cercano (92.8%); el resto
  es el f31 guardado y los huecos de los temporales.
- `cos(m * 0.5f)` carga la constante antes que el miembro; `cos(m / 2.0f)` da el orden del original
  (miembro primero, mismo fmuls).
- Producto escalar de "hacia delante" en un inline `FrontDot_unk(self, pos)` con `PSVECDotProduct(&d, &fwd)`
  da los huecos de pila del original (fwd en 0x8, d en 0x14).
- `IsDown_unk()` de CharaTask asigna r5/r6 al reves en vf3C; leyendo `mStatus.m0C` en un local y probando las
  dos mascaras a mano sale el original.
- `(pos - mPos).GetNormalized_unk().ToRotation_unk()` (encadenado) da los huecos del original para "mirar a";
  la variante con ToRotationZ_unk deja el f0/f3 cambiado.
- El campo de flags que se compara con `cmpwi` tras `rlwinm` es `int`, no `unsigned int`.
- `float x = k; if (p) x = p->m;` (no el ternario) da el `lfs` del valor por defecto antes del `cmpwi`.
- Un `atn::Vector3 rot;` declarado antes que la diferencia y asignado despues (`rot = d.ToRotation_unk()`) da
  el marco 0x10 mayor del original cuando la rotacion no se usa.

## Pendientes (esta cola)

- Em202Spider: vf0C, vf10, fn_8027EB14, fn_8027F504, fn_8027F69C, fn_8027FA7C, fn_80280184, fn_802805C8,
  fn_80281F88 (al escribirlas se arreglan los pools de casi todas las demas).
- Em202SpiderBase: vf0C, vf10, fn_80285064, fn_80285FA0.
- Em205Ogre vf108; Em209SmallGolem vf10; segunda pasada a los NONMATCHING de Em208Queen.

## Pendientes para otras colas

- Em301LShade: su .rodata deberia empezar en 0x80747080 (los tres doubles recortados de Em209SmallGolem).
- Em202ForestSpider, Em202_01_BSpider, Em308ParmCrab: leen lbl_807C8398, ahora definido en
  Em202SpiderBase.cpp (`extern "C" SpiderBaseColor_unk lbl_807C8398`).
