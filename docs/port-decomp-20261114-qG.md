# port-decomp-20261114, cola G: una sola `CollResult_unk`

Objetivo: las vistas locales `struct CollResult_unk` (0x50 bytes) pasan al tipo del motor
`atn::ColliResult_unk` (`include/atn/ColliBase.h`), para que el port tenga una sola definicion
(punteros en m34/m38/m40).

## Decisiones (sin el usuario)

- **Tipo comun:** las unidades usan `atn::ColliResult_unk` directamente (constructor inline del motor).
  Campos que las vistas tipaban distinto se leen con cast en el sitio (`*(unsigned int*)result.m34`,
  `(CharaTask*)result.m38->m0C`, `m38->mFlags` en vez de `CollObj_unk::m08`). No se anaden accesores a
  `ColliBase.h`: un metodo inline nuevo en esa cabecera cambia los `@N` de todas las unidades que la
  incluyen (probado: `IsKind1A_unk` en `ColliResult_unk` movia el `@enum$6811` de ControlAppear a 6818 y
  perdia 2 funciones) y un accesor `AttrId_unk()` metia un nivel de inline que cambiaba el codigo de
  `fn_8013CB68`/`fn_80140588`/`fn_801426A4` de CharaTask.
- **Constructor fuera de linea 0x80140500** (`__ct__14CollResult_unkFv`, llamado con `bl` por CharaTask,
  Em315Coffin, FlamingArrow y ControlReflectLaser): nuevo `include/game/CollResult_unk.h` con
  `struct CollResult_unk : public atn::ColliResult_unk { CollResult_unk(); };`, definido en
  `game/CharaTask.cpp` (`CollResult_unk::CollResult_unk() {}`, sigue al 100 % y en su sitio). Con el
  constructor inline del motor, MWCC emite la copia debil justo antes de su primer llamador
  (`fn_8013CB68`), no entre `fn_801404F8` y `fn_80140518`: el original tuvo que definirlo ahi.
- **`gmk/Lemon.h`**: su vista pasa a `struct GmkCollResult_unk : public atn::ColliResult_unk
  { GmkCollResult_unk() {} };` (sin campos propios). Quitar la definicion entera renumera 4 `@N` en las
  ~30 unidades que incluyen la cabecera; la derivada con constructor inline consume los mismos. La usan
  las unidades gmk y las que ya la usaban; GameCamera_unk y Em206CocoonMp pasan a `atn::ColliResult_unk`
  (con la derivada, el inline de dos niveles consume mas `@N` y bajaba el fuzzy de
  `GameCamera::fn_803903E0` 98,54 -> 98,45; con el tipo del motor, etiquetas y fuzzy como antes: indicio
  de que el original usaba el tipo del motor).

## Cambios de firma (para el port; nombres mangled renombrados en `symbols.txt`)

- `Bullet::fn_80110858(atn::ColliResult_unk*)` (antes `CollResult_unk*`).
- `ColliAttrManager::fn_80184864(atn::ColliResult_unk*, CharaTask*, int, int)`.
- `gmk::Lemon::fn_80416268(atn::ColliResult_unk*, const atn::Vector3*, const atn::Vector3*, float)`.
- `CharaTask::IsLanded_unk(const atn::ColliResult_unk&)` (inline, sin simbolo).
- Cabeceras: `Bullet.h`/`ColliAttrManager.h` declaran `namespace atn { struct ColliResult_unk; }` en vez de
  `struct CollResult_unk;`; `gmk/Lemon.h` incluye `atn/ColliBase.h`.

## Unidades

| Unidad | Antes | Despues |
|---|---|---|
| game/CharaTask | vista local (ctor fuera de linea) | `CollResult_unk` de `game/CollResult_unk.h`; `CollObj_unk` = `atn::ColliBase` |
| game/ColliAttrManager, EnemyLizerdBlade, Em306GurgBeast, Em201ZanGurgFinal | vista local | `atn::ColliResult_unk` |
| game/GameCamera_unk, Em206CocoonMp | `CollResult_unk` de Lemon.h | `atn::ColliResult_unk` |
| gmk/Lemon, Cannonball, Brazier, game/Bullet, EnemyJill, Em403WarLion, SkillEffect, FinishCamera_unk, LastCocoonImago, chr/ControlCharge, ControlDamage, ControlChant | `CollResult_unk` de Lemon.h | `GmkCollResult_unk` de Lemon.h |

Re-emparejado con un solo `pdbatch.sh` de las 27 unidades que incluyen las cabeceras (solo
renombrados de `@N`); sin funciones perdidas ni bajadas, labelcheck/rep100 igual.

## Hallazgos

- Cada inline de dos niveles (constructor de una derivada vacia que llama al inline de la base) consume
  mas `@N` que el inline directo: sirve para decidir que tipo usaba el original.
- Un `pdbatch.sh` por tandas tras cambiar una cabecera deja etiquetas mal en unidades de fuera de la
  tanda: hacer uno solo con todas las afectadas (lista: diff de labelcheck y rep100 frente a `$REF`).
