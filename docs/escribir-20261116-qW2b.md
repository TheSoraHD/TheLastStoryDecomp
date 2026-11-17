# escribir-20261116 cola W2b (game, enemigos y gimmicks, segunda pasada)

Inicio: 2026-11-17 04:23 CET (tope 06:53).

## Tabla de la cola

| funcion | antes | despues |
|---|---|---|
| EnemyTasha `@stringBase0` 0x8074B0A4 (vf6C 0x8035B368) | 0x2E0, vf6C 97,55 % | 0x2E1, vf6C 100 % |
| EnemyTasha (enlazar) | 19/25 (objdiff) | 20/25: no enlaza; quedan vf10 99,97, vf104 99,94, vf108 99,66, fn_803576F8 98,69, fn_8035AB78 90,79 (registros/pila) |
| EnemyTasha fn_803576F8 | 97,18 % | 98,69 % (copias con `VecAssign_unk`, `FrontDir_unk` por referencia) |
| EnemyTasha vf104 | 99,92 % | 99,94 % (cruce r3/r4 de la copia de `diff`) |
| Em301LShade vf10 (0x802D0B84) | 90,2 % (27 saltos con otro destino) | 96,57 % (saltos iguales; ternarios en las pinzas de color, giro in situ, `GetMotionFrame(0)`, `mgr` antes de la posicion del disparo) |
| Em301LShade fn_802D5D18 | 92,9 % | 96,66 % (devuelve `void`; `IsState_unk(0x20)`; `ShadeRand_unk`) |
| Em301LShade fn_802D4628 | 85,3 % | 92,2 % (`20 < x`, `MAX`, `life` local, `pos +=`) |
| Em202ForestSpider vfFC (0x802747D0) | 86,2 % (argumento mal) | 98,87 % |
| Em202ForestSpider vf110 | 93,8 % | 95,17 % |
| Em202ForestSpider vf114 | 94,2 % | 99,52 % |
| Em202ForestSpider fn_80278634 | 95,75 % | 98,25 % (escala mal calculada) |
| Em202ForestSpider vf10 | 91,1 % | 92,24 % |
| Em202ForestSpider fn_80277BC0 / fn_80277DC4 | 99,10 / 99,10 % | 99,35 / 99,57 % |
| Em202ForestSpider vf14, vf0C, ctor | 93 / 97,2 / 98,75 % | sin cambios (6 formas de los operadores de color probadas) |
| Em302KrakenMp vf0C | 99,31 % | 99,31 % (sin el local `zero`; rotacion de registros sin abrir) |
| Em206CocoonMp vf100 (0x802BB410) | 99,76 % | **100 %** (mnemcheck limpio) |
| Em206CocoonMp vf40 (0x802BB104) | 99,51 % | **100 %** (aviso de hielo como inline `NotifyIce_unk`) |
| Em206CocoonMp vf114 / vf20 | 99,70 / 99,45 % | 99,93 / 99,88 % |
| Em206CocoonMp fn_802BF5C0 / fn_802BC4EC | 93,94 / 96,27 % | 95,59 / 96,50 % |
| Em206 ctor, LastCocoonChrysalis ctor, GimmickManager LoadSeTableTask::vf0C (presupuesto de inline) | 0 % | sin cambios (sin idea nueva) |

Funciones al 100 %: 15922 -> 15924 (+2, Em206CocoonMp::vf100 y vf40; vf6C de EnemyTasha ya contaba en report.json). lost 0, sin bajadas.
Barrido de saltos (semdiff) hecho en Em206CocoonMp, Em302KrakenMp, EnemyTasha, Em301LShade y Em202ForestSpider: los saltos con otro destino que quedan son de alineado (no de flujo).

## Para el port
- Em202ForestSpider::vfFC (estado 7): `fn_801426A4(&rot, mag, m568 * speed)` recibe el modulo de la direccion de la ruta (antes 0.0f); `rot` es el ToRotation entero de la direccion normalizada (antes x y z a 0: el original conserva el cabeceo); la direccion al objetivo reutiliza `dir` y se gira in situ (`fn_80139F60`).
- Em202ForestSpider::fn_80278634: la escala z del hilo es `|normalize(copia de m10)| / 50` (= 1/50), no `|waist - head| / 50`; el giro es el de la copia normalizada de m10.
- Em202ForestSpider::vf114: `ok = 1` despues de calcular m18F8 (sin efecto); `back` se asigna tras declararlo; ToRotationZ_unk en el estado 0x74.
- Em202ForestSpider::vf110: el atrapado se comprueba con `IsBindable_unk` (banderas 4/1 en un bool, estado 0x20, agarre); misma logica que IsTargetable_unk.
- Em301LShade::fn_802D5D18 devuelve `void` (antes `int`; el unico llamador ignora el valor).
- Em301LShade::vf10: las pinzas de color son ternarios (`c.r = m15E4 == 0 ? MIN_unk(...) : MIN_unk(...)`), sin cambio de comportamiento; la direccion al objetivo se construye con el ctor y se gira in situ.
- Em301LShade::fn_802D4628: la tasa es `MAX(m156C, m1564 - m1568 * m1560)` (igual que antes); pos de dano con `+=`.
- Em206CocoonMp: `NotifyIce_unk()` nueva inline en la cabecera (vf40 y vf114, mismo codigo); copias de vf20 con `atn::VecAssign_unk`.
- Em206CocoonMp::fn_802BF5C0: pinzas escritas como `if` sobre las variables (`speed`, `wide`, `over`, `accel`); misma semantica que los MIN/MAX anteriores; `turn *= t`.
- EnemyTasha: `FrontDir_unk(const atn::Vector3&)` (antes `float`), copias a m1534 y `diff` con `atn::VecAssign_unk`.
- Literales: `@stringBase0` de EnemyTasha mide 0x2E1 (el "" final).

## Decisiones tomadas sin el usuario
- symbols.txt: tamano del `@stringBase0` de EnemyTasha 0x2E0 -> 0x2E1 (pedido por la cola; strend.py).
- Inlines locales nuevas con `// inferred:` (no funciones inventadas sin llamadores): `ShadeRand_unk` (Em301LShade), `IsInactive_unk`/`IsGrappled_unk`/`IsBindable_unk` e `IsFlag25_unk` (Em202ForestSpider, la ultima copiada de Em302Kraken porque CharaTask.h es de G3).
- Em302KrakenMp::vf0C: quitado el local `int zero = 0` (mismo porcentaje, regla 2).
- Em206CocoonMp: `lbl_808841F0` (global en el original) sigue con su nombre; nuestro .o lo emite local (@N); no lo toco.

## Hallazgos
- `20 < x && x <= 90` deja los dos `cmpwi` del original donde `x > 20 && x <= 90` se pliega en `subi; cmplwi` (Em301LShade::fn_802D4628) (escribir-20261116 qW2b).
- Un salto a una union comun tras dos ramas que asignan el mismo campo (`b` a la union y un solo `stfs` despues) es un ternario `c.r = k ? MIN_unk(a) : MIN_unk(b)`, no un if/else con dos asignaciones (Em301LShade::vf10 90,2 -> 95,1 %) (escribir-20261116 qW2b).
- `fcmpo x, c; ble; fmr x, c` sin `b` es `if (x > c) x = c;` sobre la variable; MIN/MAX (macro o inline) dejan `bge; ...; b` (Em206CocoonMp::fn_802BF5C0) (escribir-20261116 qW2b).
- `fmuls` + `fadds` separados donde el nuestro da `fmadds`: el producto va a su propio local (`float k = 0.75f * Abs(...)`; `float step = d * (...); rot.y += step;`) (escribir-20261116 qW2b).
- Un temporal copiado con psq a un local antes de PSVECNormalize/ToRotation: el original normaliza una copia (`atn::Vector3 n = dir;`) o gira in situ (`v.fn_80139F60()`) en vez de `ToRotation_unk()` por valor; buscar tambien argumentos que faltan (el modulo guardado en un FPR salvado) (Em202ForestSpider::vfFC 86 -> 98,9 %) (escribir-20261116 qW2b).
- Un `int id = x->call()` local antes de una llamada con varios argumentos cambia el reparto de registros salvados: pasar la llamada como argumento (MWCC evalua de derecha a izquierda) (Em206CocoonMp::vf100 al 100 %) (escribir-20261116 qW2b).
- Recargas que el original hace y el nuestro no (`lwz` del mismo campo dos veces): leerlo por una inline const (`IsState_unk`, `IsFlag25_unk`) en vez del campo a mano (Em301LShade::fn_802D5D18, Em202ForestSpider::fn_80277DC4) (escribir-20261116 qW2b).
- Un argumento cargado en un registro salvado antes de `rand()` es el parametro de una inline (`ShadeRand_unk(list.mCount)`), no `rand() % list.mCount` (escribir-20261116 qW2b).
- `x.y = mPos.y` al final de una cadena que usa `const Vector3& origin = mPos` recarga desde `this`: el original mezcla `origin` y `mPos` (Em202ForestSpider::fn_80277BC0) (escribir-20261116 qW2b).
- lblseq.py puede dejar un `@N` de symbols.txt sin pareja cuando el orden de la `.sdata2` del nuestro difiere del original: `lblval.py` da el candidato por valor y `pdfix.py 0xADDR=@N` lo arregla (Em206CocoonMp) (escribir-20261116 qW2b).
- `semdiff.py` de agentes/bin hace `chdir` a work-N: copia con `os.chdir(os.environ.get("WDIR", os.getcwd()))` en `/home/claude/scratch-W2b/bin/semdiff.py`; `rows.py <unidad> <simbolo> <desde> <hasta>` (misma carpeta) imprime las filas alineadas de objdiff con `*` en las distintas (escribir-20261116 qW2b).

- El mismo bloque repetido en dos funciones (aqui `BattleTask* mgr = ...; pos = vfAC(); mgr->fn_80108F38(&pos, 0x4000)` en vf40 y vf114) suele ser una inline del original: escribirla como inline cambia el reparto de registros salvados (Em206CocoonMp::vf40 al 100 %) (escribir-20261116 qW2b).
- Anadir un `#include` (math/VecAssign_unk.h) desplaza todos los `@N` de la unidad; tras pd.sh pasar lblseq.py --apply y lblval.py (los `UNP` se arreglan con pdfix.py por direccion) (escribir-20261116 qW2b).

## Bloqueos
- Operadores de color de Em202SpiderBase (`(target - c) * 0.15f + c`, vf14 de cuatro unidades al 93 %): 6 combinaciones ctor/local de operator-, operator+, operator* y tres formas de la suma sin mejora; el original pone el producto por debajo de la diferencia en la pila (escribir-20261116 qW2b).
- Em301LShade::vf10: el original recarga m15E8 despues de los stores de m1500 y m15EC (como si pudieran solaparse); sin idea para reproducirlo (escribir-20261116 qW2b).
- Orden de los `clear()` de vectores seguidos (Em301LShade ctor 99,27 %): mismo bloqueo que W2 (escribir-20261116 qW2b).
- Em206CocoonMp::fn_802BEE44: el original compara en tiempo de ejecucion `1.0f > 1.0f` (speed = 1.0); con if, inline o MIN el nuestro lo pliega (escribir-20261116 qW2b).
- EnemyTasha::fn_8035AB78: el original lleva el desplazamiento i*0x14 como induccion y suma `this` en cada vuelta; con un puntero o referencia al evento el nuestro induce el puntero (90,8 %) (escribir-20261116 qW2b).
- Em302KrakenMp::vf0C: sin el local `zero` sigue la rotacion key/pool/0 (r28/r29/r30); `key` fuera del bucle tampoco (escribir-20261116 qW2b).

## Cambios de splits/symbols
- symbols.txt: `@stringBase0` de EnemyTasha (0x8074B0A4) 0x2E0 -> 0x2E1 (el "" final que dtk dejaba fuera; vf6C al 100 %).
- Re-emparejados con pd.sh/pdbatch.sh: Em301LShade (+ lblseq.py --apply dos veces), Em202ForestSpider, Em206CocoonMp (lblseq.py --apply y pdfix.py por direccion de 0x80884178 y 0x808841CC), EnemyTasha, Em302KrakenMp (lblseq.py --apply). Solo renombrados de `@N` de `.sdata2`.
