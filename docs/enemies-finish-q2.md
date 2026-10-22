# Lote enemies-finish, cola q2

Inicio: 2026-10-22 11:27 CEST. Rama agent-q2 (referencia start-q2: 6832 funciones al 100%).
Cola: game/Em202Spider, game/Em202SpiderBase, game/Em205Ogre, game/Em208Queen, game/Em209SmallGolem.

Estado inicial (report.json, funciones al 100% / total): Em202Spider 4/25, Em202SpiderBase 3/23,
Em205Ogre 14/21, Em208Queen 0/29 (sin fuente), Em209SmallGolem 7/22.

## Unidades

- `game/Em208Queen` (0/29 -> 12/29, NUEVA fuente `src/game/Em208Queen.cpp` + cabecera
  `include/game/Em208Queen.h`, subclase de EnemyTask; la reina de la batalla de los canones): al 100% ctor, dtor,
  vf0C, vf3C, vf40, vf24, vfEC, fn_802CC39C (suelta al personaje comido), fn_802CC708 (muerte), fn_802CCA0C
  (`extern "C"`, la llama chr/ControlCannonLift), vf64 y el `__sinit` (vector estatico del punto de impacto).
  vf14 99.54% y vf54 99.98% solo por desplazamientos de @stringBase0 (cadenas de vf10/vf20/vf50 sin escribir).

## Decisiones tomadas sin el usuario

- **symbols.txt (Em208Queen)**: fn_802C7BE4/7CBC/7D3C/7F10/85B0/881C/8930/8A68/92D4/9388/9750/CC0F4/C39C/C708/
  CAAC -> ctor, dtor, vf0C, vf10, vf14, vf3C, vf40, vf50, vf54, vf20, vf24, vfEC, fn_802CC39C, fn_802CC708, vf64
  (por la vtable 0x80786860); fn_802CCAFC -> `__sinit_\Em208Queen_cpp` (local); lbl_80746DEC -> @stringBase0
  (local); lbl_807C83B8 -> sShotOffset_unk (local, el vector 3.882/98.679/10.846 que tambien usa
  chr/ControlCannonLift.cpp con su propia copia).
- game/SpecialEnemyBase.cpp: la factoria (case 26) usa `new (3, "", "", 0) Em208Queen(parent, data)`.
- Em208Queen: m1540 es la d2anime "ui_point_slash.d2a" vista con un struct local `QueenD2A_unk` (como MenuD2A_unk);
  la tabla de mensajes lbl_8087F1E4 como `QueenMsgTable_unk` local (como en Em309Ghoul).

## Hallazgos MWCC

- vf0C con `int result` y la carga en un `if (result != 0) { ...; return 1; } return 0;` da el bloque `li r3,0`
  al final como el original; con `if (result == 0) return 0;` va arriba.

## Pendientes (esta cola)

## Pendientes para otras colas
