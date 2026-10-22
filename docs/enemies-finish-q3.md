# Lote enemies-finish, cola q3 (Em301LShade, Em302Kraken, Em302KrakenMp)

Inicio: 2026-10-22 09:05 CEST. Rama agent-q3 (sale de main 64df7a6). Referencia: /home/claude/ref/start-q3.json.

## Unidades

### game/Em301LShade (0/31 -> en curso)
- Unidad SIN FUENTE hasta ahora: fuente nueva `src/game/Em301LShade.cpp` y cabecera nueva
  `include/game/Em301LShade.h` (subclase de SpecialEnemyBase; layout 0x14D4-0x16B0 deducido del ctor, del dtor
  implicito y de las claves del csv; nombres inventados).
- Al 100%: vf14, vf20, vf40, fn_802D4238, fn_802D7058, vf100/vf104/vf108/vfFC (vacias) y el dtor (implicito,
  ultima funcion del fichero).
- NONMATCHING >= 99% (casi todo es la posicion de las constantes de .sdata2 y de las cadenas del pool, que
  fija vf10 (5.9 KB, sin escribir) y vf0C): ctor, vf54, vf64, fn_802D404C, fn_802D4378, fn_802D4E9C,
  fn_802D5020, fn_802D5674.

## Decisiones tomadas sin el usuario
- **splits.txt**: `.rodata` de game/Em301LShade ampliada de 0x807470A4 a 0x807473F8: absorbe el tramo
  auto_06_807470A4 (pool de cadenas de la unidad, `lbl_807470A4`, que solo usa Em301LShade).
- **symbols.txt**: los 31 simbolos de 0x802D0038-0x802D73E4 renombrados a metodos de Em301LShade (ctor, dtor,
  vf0C, vf10, vf14, vf20, vf3C, vf40, vf50, vf54, vf64, vfEC, vfFC, vf100-vf108 por la vtable; el resto
  `fn_<addr>__11Em301LShadeF...`); fn_802D0B6C -> `IsSet_unk__13ShadeFlag_unkFv` (copia debil de un inline:
  `mGlobal != -1`); lbl_807470A4 -> `@stringBase0` (scope:local).
- game/SpecialEnemyBase.cpp: la factoria (case 29) crea `new (3, "", "", 0) Em301LShade(parent, info)` en lugar
  del `extern "C"` fn_802D0038 (mismo tamano 0x16B0; la factoria sigue en 73.27%).
- HitMsg_unk+0x40 (indice de la esfera de dano) se lee con un cast (`*(int*)&msg->m3C[4]`), como en
  Em101ElderZesya, para no tocar CharaTask.h.

## Hallazgos MWCC

## Pendientes para otras colas
