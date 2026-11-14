# Sesion cabeceras-20261114, cola A (push_back de `include/std/vector.h`)

Inicio: 2026-11-14 14:00 CET. Tope ~16:30.

## Paso 1: despacho de `push_back` por miembros triviales

`std/vector.h` (`__vec_imp_unk::__push_fast_unk`, `__vec_split_unk::push_back`, `__push_front_unk`) decide el
camino POD con `(__builtin_trivial_members(T) & 7) == 7` (destructor, asignacion y copia triviales) en lugar de
`__builtin_is_pod(T)`. Un tipo con constructores propios pero copia trivial toma el camino POD, como en el original.

- Probado primero `& 4` (solo copia trivial, la variante de qT): 3 suben, 4 bajan: `static_string<32>`
  (destructor propio, AreaEffConfig 98,74 -> 83,54), `MotionSetLine_unk` (asignacion propia por
  `FixedString_unk::operator=`, LH_MotionSet 95,58 -> 89,03) y `ColorF_unk` (Puppeteer_CharaTask). Con `& 7 == 7`
  vuelven los dos primeros.
- `ColorF_unk` (`include/atn/ColorF_unk.h`): el original copia en el camino lento con `lfs/stfs` campo a campo
  (0x8000DE44): constructor copia escrito a mano `r(o.r), g(o.g), b(o.b), a(o.a)`. Sin bajadas.
- `MeleeTarget_unk() {}` en game/BattleTask.cpp (local del .cpp): `MeleeMedian3_unk` **100 %**, MeleeSort 64,56 ->
  73,35, fn_800F8C6C 89,94 -> 90,60.
- `CosEditItem_unk()` (`include/game/CosEditBase.h`): `mItem 0, mIndex/mOwner -1, mEnabled 0` (0x801D8C90):
  ctor **100 %**, `MslSwap_unk<CosEditItem_unk>` **100 %**, `MslMedian3_unk<..., CosEditEquipLess_unk>` **100 %**,
  push_back 96,95 -> 99,76, sorts 89,95 -> 94,99, medianas 86,73 -> 97,93, fn_801DD26C 94,87 -> 96,57.
- Suben sin tocar su fuente: Em305Vampire vf0C 93,92 -> 98,95, Em305VampireMp vf0C 93,97 -> 98,89, ShopUpGrade
  fn_805969D4 90,37 -> 99,74.
- Total: 15424 -> 15428, lost 0, sin bajadas de fuzzy.

Por unidad (100 % / 0 %): game/BattleTask 163/0 -> 164/0; game/CosEditItemList_unk 52/2 -> 55/1
(`__ct__15CosEditItem_unkFv`, `MslSwap_unk<CosEditItem_unk>`, `MslMedian3_unk<..., CosEditEquipLess_unk>`).
Commits: 81d50d7 (cabecera + re-emparejado), y el de comentarios/notas.

## Paso 2: forma del original (inline pequena + construir + reallocar): no intentado

Sin tiempo para una via nueva frente a las 22 variantes de qL y el relleno de 300-600 asserts (bloqueo de
presupuesto de inline, `docs/mwcc-blockers.md`). Las 26 copias debiles de Event/Sequence (0x8054372C-0x80545C3C,
0x80542684, 0x805428A8), EventParser_unk (fn_8057DB14/DC38/E274) y Event/Manager (fn_80482400/8049003C/80490098)
siguen sin escribir. Con el paso 1 no cambia ninguna de esas unidades (EffectEntry24_unk, Actor y Path no son
triviales). El ctor de `CosEditItem_unk` si sale (paso 1).

## Para el port

- `include/std/vector.h`: solo la condicion del camino rapido (sin firmas ni layouts).
- `ColorF_unk`: constructor copia nuevo (mismo layout, 16 bytes).
- `CosEditItem_unk`: constructor por defecto nuevo (mismo layout, 0x18 bytes); `m04` y `m14` sin iniciar.
- `MeleeTarget_unk` (local de game/BattleTask.cpp): constructor por defecto vacio.

## Decisiones tomadas sin el usuario

- Criterio `& 7 == 7` (el que mejor sale de los dos probados; ningun tipo baja).

## Hallazgos (una linea)

- MSL decide el camino POD de `push_back` por destructor, asignacion y copia triviales
  (`(__builtin_trivial_members(T) & 7) == 7`), no por `__builtin_is_pod`: los tipos con constructor tambien.
- Copia `lfs/stfs` campo a campo de un struct de floats en un `new (p) T(x)`: constructor copia escrito a mano.

## Re-emparejado

- Renumeran: `atn/ColorF_unk.h` (via `Event/Actor.h`: unidades Event/*, GameEventScript, GameTask, MiniMapTask...),
  `game/CosEditBase.h` (CosEdit*, NewCamp*), BattleTask (ctor local). `pdbatch.sh` sobre 38 unidades
  (`scratch-A/units.txt`) y `lblmap.py` en 11; a mano en BattleTask: 0x808814A8 `@31061`, 0x808814B4 `@31064`
  (lblmap los dejaba repetidos) y 0x80881534 `@34008` (0,33 de fn_800F8574). Toca lineas de `symbols.txt` de
  unidades de otras colas (Event/*, GameEventScript, GameTask, MiniMapTask, NewCamp*, Em305Vampire*).
- Sin cambios en `lbl_8087D6E8/EC`, 0x8087EE78, `lbl_80885CB0`, 0x80783CA8, 0x8078CB00/08/10 ni ControlAppear.
- `ordercheck` de game/GameEventScript ya fallaba antes (`__dt__15GameEventScriptFv`), no es de este cambio.
