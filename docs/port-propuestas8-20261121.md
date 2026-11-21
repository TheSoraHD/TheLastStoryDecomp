# port-propuestas8-20261121

Rehecha sobre `main` `754ff60f` (con `codigo-20261120` y `grandes-20261120`) desde `port-propuestas8-20261120-v2`
(`268920cf`, que no se llegó a mergear), con una segunda parte (8b, sincronización 6 del port).

Arreglos del port de PC (sincronización 5, `sync-decomp5-20261120`) neutros para MWCC: miembros reales en vez de
offsets de bytes de la Wii. En PC esos offsets leían o escribían otros bytes (punteros de 8 bytes, `atn::Task` de
0x70). Base: `main` `feb340d2` (con `mixto2-20261120`).

| Fichero | Antes | Después |
|---|---|---|
| `game/Em203GreaterDaemonRed` | `*(float*)((char*)player + 0xAD0)` | `player->mStatus.m2FC` |
| `game/Em302Kraken` | `*(atn::Vector3*)((char*)target + 0x6C)` | `((GimmickCommon*)m1654)->m6C` (m1654 es el gimmick de fn_802DC4E0) |
| `game/Em315Coffin` | `*(int*)((char*)mgr + 0xC4)` | `mgr->mSystem.m78` (con `atn/EffectManager.h`) |
| `game/Em209SmallGolem` | `*(unsigned int*)((char*)param + 0x9C)` | `param->m9C` |
| `game/SkillEffectManager` | `*(int*)((char*)hit + 0xC8)` | `hit->mC8` |
| `atn/EffectDesc_unk` | `(FogSettings*)((char*)item->m6C + 0x3C)` | `&((atn::Scene*)item->m6C)->mFog` |
| `atn/PacketPack` | `(char*)route + 0xF4` | `&((GameScene*)route)->mF4` |
| `game/CharaTask` | `(char*)lbl_8087EEB0 + 0x70` | `&lbl_8087EEB0->m70` |
| `game/CostumeEditChara` | `*(unsigned int*)((char*)equip + 0x28)` | `equip->m024.mFlags` |
| `gmk/TodomeCountUp` | `(char*)msg->m10 + 0x528` | `&msg->m10->mPos` (con `game/CharaTask.h`) |
| `atn/Model` | `*(void**)((char*)lbl_8087EFA8 + 0x4C)` | `lbl_8087EFA8->m04C` |
| `atn/Motion_unk` | `*(unsigned int*)((char*)*it + 0x21C) & 1` | `(*it)->mModel.m20C_0` |
| `game/CopyRobotTask` | `(*(CopyModelRes_unk**)((char*)&mMotion + 0x16C))->mNumNodes` | `mMotion.GetRes_unk()->m44` |

## 8b (sincronización 6)

| Fichero | Antes | Después |
|---|---|---|
| 11 enemigos (Em001Necromancer, Em101ElderZesya/Second/Little, Em203GreaterDaemon/Red, Em204Mulue, Em301LShade, Em302Kraken/Mp, Em309Ghoul) | `*(const wchar_t**)((char*)m60 + 4)` (32 sitios) | `m60->m04` (`CharaInfo_unk::m04`, nombre del personaje en los mensajes de combate) |
| `atn/ptr_string_base_wchar_t_0`, `db/SkillParam_unk` | `unsigned int* size` / `unsigned int size` | `unsigned long` (como la definición `atn/FileList` `fn_8006BA8C`) |
| `game/GameLoadTask` (`LoadDebugData_unk`), `Event/Manager` (`.gs` del mapa) | `int* size` / `int size` | `unsigned long` |

En PC `fn_8006BA8C` escribe un `unsigned long` de 8 bytes: con un `int` local pisaba la pila (textos de sistema vacíos
en Linux); el nombre del personaje leía 4 bytes de relleno y media dirección (crash del asalto al Terracor). En la
Wii son los mismos 4 bytes. Queda en el port, con `#if TARGET_PC`: `Event/EventParser_unk` (la decomp pasa
`&m1CC`, un miembro `int`; en PC va por un local).

## Resultado

`main.dol: OK`, **16366 / 18113**, `regress.py` lost 0 y `fuzzdrop.py` 0 contra el `report.json` de `754ff60f`; el
porcentaje de **todas** las funciones del `report.json` es el mismo que en `754ff60f` (0 diferencias), `ordercheck.py`
0 fuera de orden en las 26 unidades tocadas.

No neutro o sin probar (se queda en `#if TARGET_PC` en el port): `NewCampPartsStatus::SetTaskFloat_unk` (el
comentario de la decomp explica que el `task + 0x58` mantiene el registro entre llamadas) y las vistas con relleno
de la Wii (`LabelPlayer_unk`, `BigMulueChara_unk`, `DoppelChara_unk`, `PickCharaNode_unk`, `CrossbowPlayer_unk`...),
que en la decomp copian la estructura de golpe.
