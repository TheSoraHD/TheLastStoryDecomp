# Cola N2 (lote paralelo-20261113): segunda pasada de cerca de enlazar

Lista: `/home/claude/scratch/n-candidates.txt` (98 % y menos, y las de 99 % que la cola N no probo).

## Resultado por unidad

- `gmk/MeetingSpot`: fn_803FD328 95,29 -> 100 %, fn_803FD0A0 99,97 -> 100 %; **enlaza** (`matching: true`).
  Dos causas: (1) el motion del estado 7 es un ternario de constantes `enum` en el argumento
  (`PlayMotion_unk(i, i == 0 ? kMeetMotion76_unk : kMeetMotion77_unk, 1)`); (2) `PlayMotion_unk` no es
  `inline`: definida entre vf10 y fn_803FD0A0, `-inline auto` la expande en fn_803FD328 y el enlazador quita
  su copia; sus 1.0f/0.1f quedan en `.sdata2` antes del 6400.0f de fn_803FD0A0, como en el original.
- `chr/ControlShoot`: ctor 98,76 -> 99,90 %. El motion de los casos 0x25/0x26 con el ternario de `enum` y
  `mFB4 = mData->m48 + mFB4`. Queda el registro del cero de mFB0/mFB4 (r6 por r4).
- Sin cambio (intentos revertidos, 2-8 cada una): `db/MapEnvSeTable_unk` fn_80214F80 (operator[] e inline
  de busqueda), `gmk/FallingTower` fn_8041044C (inline de mezcla Blend_unk(a, b, t): igual), `game/MapManager`
  FindArea (orden begin/end), `Event/EventCommandDef_unk` fn_800081C0 (inline de busqueda, referencia),
  `atn/LightUtil_unk` fn_8007A154 (`(int)size()` en la comprobacion da la recarga de mSize, pero cruza
  src/dst; helpers de copia), `db/CharaIdTable_unk` fn_80208BE8 (temporal con nombre; inline de nombres no
  se expande por el presupuesto), `gmk/InteractionSpot` Start (found/break, inline estatica por referencia).
- No probadas por bloqueo conocido o notas previas: RandomItemTable dtor (doble beq, `docs/trucos-mwcc.md`),
  Event/Label `__sinit`, ControlDraggingStart vf10 (mismo residuo que ControlCdnSleep), LandMine Start,
  HealPoint, Texture_unk fn_800D5A3C, CharaLight vf6C, Section/EfpHandle.

## Hallazgos

- **Ternario de constantes `int` que difieren en 1 -> MWCC lo hace sin salto** (`cntlzw`/`subfic`+`nor`+`srawi`
  + `addi`), tambien dentro de una inline o con `!=`/`<`. Con constantes de un **`enum`** queda el salto del
  original: `li r4, 0; li r5, C2; bne; li r5, C1` evaluado en el orden de los argumentos (MeetingSpot,
  ControlShoot).
- Constantes de `.sdata2` antes de las de una funcion anterior: un metodo **no inline** que `-inline auto`
  expande en una funcion posterior y que el enlazador quita deja sus constantes en la posicion de su
  definicion (MeetingSpot::PlayMotion_unk).

## Decisiones tomadas sin el usuario

- `include/gmk/MeetingSpot.h`: `PlayMotion_unk` pasa de `inline` a declaracion normal (la incluyen
  MeetingSpot y GimmickCommon; sin cambio de `@N`, labelcheck/rep100 igual).
- `enum` anonimos con nombres inventados (`kMeetMotion*_unk`, `kShootMotion*_unk`) en los `.cpp`.
- `matching: true` en `gmk/MeetingSpot` (con `main.dol: OK`).

## Pendientes

- Buscar otras funciones con `li rX, C; bne; li rX, C+-1` en argumentos (ternario de `enum`).
- ControlShoot ctor: el cero de mFB0/mFB4.
