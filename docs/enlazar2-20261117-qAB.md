# enlazar2-20261117, cola AB

Inicio: 2026-11-18 01:00:29 CET. Cierre anticipado a peticion del usuario (01:16).

| unidad | funciones que faltan antes -> despues | enlaza |
|---|---|---|
| game/EnemyQuarkStand | 22 -> 22 (fn_80351AB8 al 100 % en el parche, sin commit) | no |
| AI/AI_archer_attack01, Event/EventParser_unk, MP/D2AMpMenu, game/Em206EggMonster, game/Em309Ghoul, game/CosEditItemList_unk | sin tocar | no |

## Sin commit (parche)
- `docs/enlazar2-20261117-qAB.patch`: EnemyQuarkStand `fn_80351AB8` 99,97 -> 100 % (objdiff). El `+0x8310` de
  `Inventory::instance_` es `mEquip[2]` (equipo de Quark): `Inventory::instance_->GetEquip_unk(2)->mProtectors`
  con `#include "game/InventoryEquip_unk.h"`; fuera `QuarkItemSlot_unk`/`QuarkSlots_unk`. PRECOMMIT FAIL solo por
  labelcheck: el cuerpo inline de la cabecera renumera +1 los `@N` de la unidad (pd.sh deja 4 etiquetas sin
  emparejar, 88 missing 4). Falta re-emparejar (`lblmap`/`lblseq`/`lblfix`) o definir la inline sin consumir `@N`.

## NONMATCHING / probado sin exito
- EnemyQuarkStand `fn_80350BF0` 99,86: inline `QuarkSide_unk(atn::Vector3 dir)` (por valor) con el producto
  vectorial devuelto da 99,92 (arregla el hueco de `dir`/`side` de la primera rama); `up` sigue al fondo de la pila
  (original justo debajo de `dir`). Probados: `up` temporal, orden de declaraciones, `up` como parametro (89),
  inline de producto vectorial anidada, rama entera en inline (94-99,6). No commiteado (solo fuzzy).
- EnemyQuarkStand `fn_80352938` 99,45: `float y = (target->mPos - mPos).ToRotationZ_unk().y;` (diferencia como
  temporal) arregla los huecos 0x50/0x5C (99,51); queda f4/f5 de `diff`/`step`. No commiteado.

## Hallazgos
- Offset fijo en un singleton (`(char*)inst + 0x8310`) con el registro intermedio distinto: buscar el miembro real y
  su accesor (`GetEquip_unk(2)`): la llamada al accesor inline da el registro del original (EnemyQuarkStand).

## Para el port / Decisiones / Bloqueos / Cambios de splits
- Ninguno.
