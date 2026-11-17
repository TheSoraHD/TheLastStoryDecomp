# escribir-20261116 cola N7: chr y enemigos cerca de enlazar

Inicio: 2026-11-17 08:45 CET (tope 10:55).

Candidatas (tabla.py; 1-3 funciones < 100 %, todas >= 97 %, no trabajadas hoy en N-N6):
chr/ControlRide, chr/ControlDamage, game/Em209SmallGolem, game/Em101LittleZesyaSecond, game/EnemySeaLizerd, game/Em315Coffin.

## Tabla de la cola

| funcion | antes | despues |
|---|---|---|
| chr/ControlRide ControlRideBeast::vf0C (0x801C71A8) | 97,79 % | 100 % |
| chr/ControlRide ControlRideBeast ctor (0x801C706C) | 99,68 % | 100 % |
| chr/ControlRide (unidad) | 8/10 | 10/10, enlaza (matching: true, main.dol OK) |
| game/Em209SmallGolem vf80 (0x802CEDFC) | 99,71 % | 100 % |
| game/Em209SmallGolem vf20 (0x802CEF60) | 98,49 % | 100 % |
| chr/ControlDamage Knockdown ctor (0x801AD34C) | 99,95 % | sin cambio (r25/r26 de la ruta y strlen) |
| chr/ControlDamage Knockdown::vf0C (0x801ADC14) | 98,10 % | sin cambio |
| chr/ControlDamage DamageLastOne::vf0C (0x801AFF1C) | 99,35 % | sin cambio (parche qY 99,64: cruce model/bool) |
| game/Em209SmallGolem vf10 (0x802CD73C) | 99,62 % | sin cambio (pila) |
| game/Em101LittleZesyaSecond vf10 (0x802604E0) | 99,96 % | 99,97 sin commit (`/ 8.0f`); queda la pila de Slash_unk |
| game/Em101LittleZesyaSecond fn_80261078 / fn_80261DC0 | 98,51 / 98,17 % | sin cambio |
| game/EnemySeaLizerd fn_80354EE4 / fn_80355390 | 98,41 / 99,96 % | sin cambio (parche qA 99,85; Abs_unk de Vec.h) |
| game/Em315Coffin | 2 funciones | no tocada (ctor bloqueado por el `.sdata` de 0.0f) |

## Para el port

- chr/ControlRide, game/Em209SmallGolem: solo forma del fuente (locales, inline `Em209SmallGolem::SetAppear_unk` nueva en la cabecera, sin nombres de parametros). Sin cambios de firma ni de comportamiento.

## Decisiones tomadas sin el usuario

- `Em209SmallGolem::SetAppear_unk(const Vector3&, const Vector3&)`: inline nueva declarada en `include/game/Em209SmallGolem.h` (nombre inventado, como `Em316LastLaser::SetAppear_unk`); parametros sin nombre en la declaracion para no renumerar las unidades que incluyen la cabecera (Em209BigGolem, SpecialEnemyBase).

## Hallazgos

- Orden de los operandos de un `fmuls` con una variable salvada (`rate`, `f31`): MWCC pone primero la variable que vive mas aunque el fuente diga `x * rate`; `x *= rate` (asignacion compuesta) o una inline con parametros deja `fmuls x, x, rate` (chr/ControlRide vf0C). `x / 2.0f` se emite como `x * 0.5f` con el otro orden de operandos que `0.5f * x` (Em209SmallGolem vf20). (escribir-20261116 qN7)
- Registros cruzados de dos cargas del mismo puntero miembro (`mBeast`, `mRider`): copiar uno a un local (`CharaTask* beast = mBeast;`) solo en la sentencia que toca, combinado con el accesor real (`GetModel_unk()`) para el otro; cada cambio solo no hace nada (chr/ControlRide ctor y vf0C). (escribir-20261116 qN7)
- Copias `m14E4 = pos; m14F0 = pos + offset; mPos = pos;` con r3/r5 cruzados: en una inline miembro (`SetAppear_unk`, como Em316LastLaser) salen los registros del original (Em209SmallGolem vf80; antes 10 formas sin la inline). (escribir-20261116 qN7)
- Orden de dos stores seguidos (`m620` y el z del temporal): asignar `m620` antes de la suma del vector (`m620 = m5B0 / 2.0f; m614 = mPos + m5A4; m614.y += m620;`) y declarar el radio despues (Em209SmallGolem vf20). (escribir-20261116 qN7)
- Vector con nombre para un temporal de operador cuando el original lo deja encima de los otros temporales: `atn::Vector3 d = a - s; d *= rate; x = s + d;` (ControlRide vf0C, el marco pasa a 0x70 como el original). (escribir-20261116 qN7)

## Bloqueos

- `ControlDamageLastOne::vf0C` / `ControlKnockdown::vf0C`: la inline miembro que recarga `mChara` (fn_801B0260 / fn_801AE41C): el bool de la inline coge r31 y el `model` de fuera r30 (original al reves). Reproducido en un fichero suelto; sin cambio con local `chara`, bool con nombre, bool declarado antes, `!= false`, getter del modelo o doble inline. (escribir-20261116 qN7)
- `ControlKnockdown` ctor: r25/r26 de la ruta del motion y del `strlen` del ctor de `std::string` (inline en `std/string.h`); sin cambio con la ruta en otra inline, conversion implicita, string con nombre o la declaracion de `path` en otro sitio. (escribir-20261116 qN7)

## Cambios de splits/symbols

- symbols.txt: solo los `@N` renumerados de chr/ControlRide y game/Em209SmallGolem (pd.sh).

## Scripts

- `/home/claude/scratch-N7/bin/vv.py <unidad> <simbolo> <variantes.py>`: como var.py, pero cada variante es una lista de pares (viejo, nuevo); `vv2.py` anade el recuento de stk.py. `ours.sh <unidad> <simbolo> [desde] [hasta]`: nuestro desensamblado con indices. `scanops.py`: busca en chr/ y Em*/Enemy*/Np* funciones con `f*` de operandos solo permutados (candidatas al truco de `*=`).
