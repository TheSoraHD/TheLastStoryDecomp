Inicio 2026-11-19 02:12 | Cuentas: unidades 742 | 100% 16074 / 18109 | codigo 62.39 % | datos 76.18 % | 0% 45 (20) | DRAFT 0

# secuencial-20261119

Sesion secuencial de 12 h (sin agentes). Parte de `main` 3197b85. Prioridad: port de PC; despues, enlazar.
Referencia: `/home/claude/ref` (1077 unidades con .o, labelcheck 11, rep100 23).

## Puntos de control

| Hora | Fase | Cuentas |
|---|---|---|
| 02:30 | 0 | 742 u, 16074, 62.39 / 76.18, 0% 45, DRAFT 0 |

## Fase 0: listas

Unidades con 1-3 funciones < 100 % y todas >= 97 % (86): `docs/secuencial-20261119-f3list.txt` (con las notas de enlazar2/mwcc-blockers que las citan).

Inventario de vistas del port (`grep -rlw` en src + include):

| Vista | Ficheros antes | Despues |
|---|---|---|
| `Field_unk` | 99 | |
| `BulletField_unk` | 1 | |
| `SoundEngine_unk` | 7 | |
| `MenuD2A_unk` | 6 | |
| `PathData_unk` | 11 | |
| `ActionOwner_unk` | 9 | |
| `Settings_unk` | 47 | |
| `EffectMgr_unk` | 92 | |
| `GameTaskTable_unk` | 2 | |
| `CharaIdEntry_unk` | 2 | |
| `ShopChara_unk` | 1 | |
| `NewCampStatusName_unk` | 1 | |
| `MenuItem_unk` | 3 | |
| `SelectMemberAnim_unk` | 2 | |
| `SceneData_unk` | 0 | |
| `MotionData_unk` | 1 | |
| `ModelOcclusion_unk` | 0 | |
| `fn_8013C504__9Field_unkFv` | 1 | |

## Decisiones

- D3 (aprobada): lo de "Para revisar" de enlazar2 se queda como esta (`int& cur = m84`, `float zero`, `FindPlace_unk(id = ...)`, `GXWGFifo` directo, `GetSaveFileName_unk`, alias `self`, las dos `const` de C2).
- Repo del port (TheLastStory-PortTest): no se toca; solo notas (respuesta del usuario al empezar).

## Fase 1: D1 y D2

## Fase 2: port

## Fase 3: enlazar

## Fase 4: patrones en lote

## Cambios de splits

## Bloqueos
