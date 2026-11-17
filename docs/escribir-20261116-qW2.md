# escribir-20261116 cola W2 (game, enemigos y gimmicks)

Inicio: 2026-11-16 23:58 CET (tope 2,5 h: 02:28).

## Tabla de la cola

| Funcion | Antes | Despues |
|---|---|---|
| Em206CocoonMp ctor (0x802BA92C) | 0 % + DRAFT | equivalente NONMATCHING 0 % (presupuesto de inline, bloqueo conocido) |
| Em206CocoonMp `ResHandle::ResHandle` (0x802BABC0) | 0 % (no emitida) | sin cambios: solo se emite si el ctor queda fuera de presupuesto |
| GimmickManager `LoadSeTableTask::vf0C` (0x803EEEDC) | 0 % | equivalente NONMATCHING 0 % (comprobada contra el asm; caso abierto de docs/inlining.md) |
| GimmickManager `fn_803EF3B0` (boost::function::operator()) | 0 % (no emitida) | sin cambios (se inlinea en vf0C) |
| LastCocoonChrysalis ctor (0x80452710) | 0 % | sin cambios (idea nueva probada: locales key/cmp/value) |
| Em302KrakenMp vf0C (0x802DE27C) | DRAFT 99,30 % | equivalente NONMATCHING (orden de IsLoading corregido; quedan registros) |
| Em302KrakenMp vf104 (0x802DF570) | DRAFT 100 % | 100 % (mnemcheck limpio, literales iguales) |
| EnemyTasha ctor (0x803559F8) | DRAFT 100 % | 100 % (mnemcheck limpio, literales iguales) |
| Em202ForestSpider ctor (0x8027278C) | DRAFT 98,40 % | equivalente NONMATCHING 98,75 % (faltaba `m14BC(0)`) |
| Em301LShade vf10 (0x802D0B84) | DRAFT 90,20 % | equivalente NONMATCHING 90,2 % (`m540.y` -> `m540.x`; llamadas, literales, tabla de saltos y offsets comprobados) |

| Em202ForestSpider vf128, vf104, vf108, vf10C, vfEC, fn_8027851C (fuera de la tabla: "approximate") | 97,6 / 96 / 86,5 / 88 / 80,9 / 95,7 % | 100 % (cuatro tenian el flujo mal) |
| Em202ForestSpider fn_802790A0 | 85,8 % | 100 % (`fn_8023A680` con sus 12 argumentos) |
| Em202ForestSpider vf14 / vf114 | 62,1 / 82,7 % | 93 / 94,2 % (vf114 con el flujo del original) |
| Em202ForestSpider vf10 | 88,9 % | 91,1 % (capsula y posicion de fn_805A5078 corregidas) |
| Em202ForestSpider fn_80278634 | 84 % | 95,75 % (`Matrix3x4::RotateZYX_unk` y copia de la escala por temporal) |

## Para el port
- Em202ForestSpider (cambios de comportamiento de funciones "approximate"): vf128 llama a vfFC solo si no ha llegado al punto (else); vf104 llama a vfFC salvo cuando vf154(6) acepta (antes solo en el estado 6); vf108 solo comprueba el golpe (frames 17-22) si la motion no ha terminado (else); vfEC: si m1434 > 0 solo lo decrementa, si no suelta a los atrapados (copia mPos entero, no solo z) y llama a fn_801765D8/fn_800EE360/fn_800EB7A0; `m1434 = 5` solo mientras la motion no termina.
- Em302KrakenMp::vf0C: el original comprueba `m15CC.IsLoading()` justo despues de `m152C` (antes de m1564/m1570/m157C); corregido.
- Em301LShade::vf10: la bajada de la posicion es `mPos.y -= 13.0f * m540.x` (no `m540.y`), como en los demas usos de m540.
- Em202ForestSpider ctor: el original pone `m14BC = 0` ("ReinforceCenter"); faltaba en el init list (miembro sin inicializar).

- Em202ForestSpider: `fn_8023A680` se declaraba con 9 argumentos; el original pasa ademas 0, -1, 1 por la pila (como BattleTask): corregido en las tres llamadas (vf110, fn_80278634, fn_802790A0).
- Em202ForestSpider::vf114 (estado 0x75): con m14DC <= 10 acerca al atrapado (paso limitado a +-1.5) y gira su rot.y; si no, con dist <= 45 pasa a vf14C y vuelve, y si no lo acerca con paso <= m18FC. Antes se hacian las dos cosas y la copia de rot era la de mRot donde el original copia mPos.

- Em202ForestSpider::vf10: la capsula se actualiza con `mColli.m3C = mPos`, `mColli.m48 = (x, y + m5B0, z)`, `m54 = m5B0` (antes m48 = mPos y m3C sin tocar), y fn_805A5078 recibe mPos (no la posicion alta).

## Decisiones tomadas sin el usuario

## Hallazgos
- Rotacion z/y/x de una matriz miembro: `m.RotateZYX_unk(rot)` (Vec.h, con los `if (0.0f != rot.z)`) y la escala con `Vector3(1, 1, k)` + `PSMTXConcat` a un temporal copiado despues dan el codigo del original donde `PSMTXConcat(m, r, m)` en el sitio no (Em202ForestSpider::fn_80278634 84 -> 95,75 %) (escribir-20261116 qW2).
- Las funciones "NONMATCHING: approximate" sin DRAFT pueden tener el flujo mal (else que falta, return temprano): semdiff.py/odiff2 con un `b` insertado o borrado lo delata; Em202ForestSpider tenia cuatro (escribir-20261116 qW2).
- Busqueda de RoutePlace con la tabla del campo leida dentro de la inline (`FindRoutePlace3_unk` de Em202Spider) da el orden de registros del original cuando la version con la tabla por parametro no (Em202ForestSpider::vf128) (escribir-20261116 qW2).

## Bloqueos
- Em206CocoonMp ctor y LastCocoonChrysalis ctor: las locales anidadas key/cmp/value del bucle de tokens (lo que saco LastCocoonImago de presupuesto) no bastan; tampoco una local `rest` mas ni `file = path.c_str()` (escribir-20261116 qW2).

## Cambios de splits/symbols
- Em202ForestSpider: re-emparejado con pd.sh; @15151/@15152 (.sdata2 0x8088384C/50) renombradas a @15192/@15193 (valores iguales en nuestro .o, lblval.py).

## Scripts
- `/home/claude/scratch-W2/bin/semdiff.py <unidad> <simbolo>`: filas de objdiff que siguen distintas tras normalizar registros, huecos de pila y destinos; comprueba destinos de salto por alineado y compara la bolsa de llamadas y de offsets de miembros (encontro el `m540.y` de Em301LShade::vf10 en 5896 B). Para comprobar DRAFT grandes.
- `/home/claude/scratch-W2/bin/jtab.py <obj.o> <funcion>`: tablas de saltos de una funcion como indices de instruccion (comparar el .o original de build/SLSP01/obj con el nuestro).
- `/home/claude/scratch-W2/bin/oside.py <unidad> <simbolo> <desde> <hasta>`: odiff2 con todas las filas de un rango, lado a lado.
- `/home/claude/scratch-W2/bin/fltcmp.py <unidad> <simbolo>`: valores de los literales (.sdata2/.rodata/.sdata) que carga una funcion, en orden, en el .o original y en el nuestro; para comprobar funciones al 100 % (objdiff no compara valores).
