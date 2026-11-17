# escribir-20261116 cola N5: gimmicks cerca de enlazar

Inicio: 2026-11-17 06:52 CET (tope 09:22). Rama agent-N5.

## Tabla de la cola

| funcion | antes | despues |
|---|---|---|
| gmk/Ocean fn_804188A0 | 77,8 % | 81,2 % (altura antes de copiar el resultado) |
| gmk/SwingObject LoadSwing_unk | 90,1 % | **100 %** (846d149) |
| gmk/SwingObject vf10 | 91,8 % | 95,8 % |
| gmk/SwingObject fn_803FEF78 | 95,3 % | **100 %** (4d15803) |
| gmk/SwingObject vf6C | 97,4 % | **100 %** (31e19f4) |
| gmk/SwingObject vf0C / fn_803FE390 / fn_803FE570 | 99,98 / 99,88 / 99,86 % (orden de `.sdata2`) | **100 %** (31e19f4) |
| gmk/SwingObject SwingPart_unk ctor / fn_803FFF40 / fn_804003E0 | 99,5 / 96,3 / 97,2 % | 99,46 / 96,4 / 97,2 % |
| gmk/LandMine Start | 98,3 % | 98,3 % (sin cambio) |
| gmk/BulletPoint vf10 | 98,5 % | 98,5 % (sin cambio) |
| gmk/ShakeCamera vf10 | 98,2 % | 98,2 % (sin cambio) |
| gmk/HealPoint fn_8040ED00 | 93,8 % | 93,8 % (sin cambio) |
| gmk/PassBoard vf0C / vf10 | 98,7 / 99,5 % | 98,7 % / **100 %** (6b04a66) |
| gmk/MotionObject vf10 / vf24 | 97,8 / 99,9 % | sin cambio |
| gmk/RandomMerchant vf0C / vf10 / vf20 | 99,8 / 98,6 / 99,3 % | **100 %** (f542938) / 98,6 / 99,3 % |
| gmk/MapLift vf0C / vf20 / fn_8042AB58 | 96,7 / 99,2 / 88,7 % | sin cambio |
| gmk/FlamingArrow vf10 / Collect_unk x2 | 97,5 / 96,1 / 96,1 % | sin cambio |

Resumen: 8 funciones nuevas al 100 % (LoadSwing_unk, SwingObject vf6C, vf0C, fn_803FE390, fn_803FE570 y
fn_803FEF78, PassBoard vf10, RandomMerchant vf0C); funciones 15937 -> 15942 en report.json (las otras tres
de SwingObject ya contaban por fuzzy), lost 0, sin bajadas, labelcheck/rep100 igual. Ninguna unidad nueva
enlaza: SwingObject queda 16/20, PassBoard 12/13, RandomMerchant 21/23.

## Variantes probadas

- gmk/SwingObject `.sdata2`: el orden del pool es el de creacion de las constantes al compilar las funciones
  en el orden del fuente (incluidas las inlines expandidas), no el del texto. El original empieza por 0.0f,
  1.0f (PlayEfp_unk) y las constantes de UpdateSwing_unk con 100.0f antes de 0.017453292f y 6.0f/0.03f
  antes de 0.5f; 45/10 (ctor de SwingCore_unk) y 90000 despues. Arreglo: PlayEfp_unk no inline, una funcion
  sin llamadores SwingUpdate_unk que expande UpdateSwing_unk, `ToRad_unk(x)` (0.017453292f * x) para el
  primer angulo con el seno en un local y el coseno del viento en un local. Etiquetas re-emparejadas con
  pd.sh y por direccion (`sd2.py --apply`, ver scripts): habia nombres repetidos (`@8917` dos veces).
- SwingObject vf10 (91,6 -> 95,8): `||` de los tres nodos en un solo `if` (los `bne/b` del original) y el LOD
  por una inline `SetLod_unk(model, lod)` (recarga la palabra de bits como el original). Queda la direccion
  del modelo en r4 antes del salto y los FPR de UpdateSwing_unk (mMaxWind/mWindPower cruzados): probados
  range/wind/max/ratio como locales en varios ordenes, sin rango, if/else, ternario, referencia a la
  SwingCore_unk, `param` al final: igual o peor. `SetLod_unk(ModelBase&)` pierde la recarga.
- LoadSwing_unk: la letra del eje en una inline `ParseAxis_unk` con `if` anidado (el ternario sale sin
  saltos) llamada dos veces (el original no tiene bucle) y la copia de los PARAM que faltan dentro de
  `if (count < 2)` (el original compara antes del `mulli`): 100 %.
- fn_803FEF78: `cmplw` del original = hash sin signo (`atn::NodeChunk::mHash` pasa a `unsigned int`); las
  matrices de las tres RotateAxis2 como locales de la propia funcion (t antes que r; en una inline quedan
  debajo de las de RotateAxis_unk, 99,77) y `AxisMtx_unk` que devuelve la matriz en cada caso (el original
  pone el argumento de PSMTXConcat en cada camino): 100 %. RotateAxis2_unk borrada (sin usos).
- SwingPart_unk ctor (99,46): registros del bucle del ctor de `SwingParam_unk[2]` y la vtable de LH_Binary;
  no probado nada.
- gmk/PassBoard vf10 (99,53 -> 100): la segunda rotacion (caja de colision) en una inline
  `SetColliBox_unk(this)` y el sonido en `PlaySe_unk(&pos)` (un nivel mas de inline invierte el orden de
  pila), `Mtx t; Mtx a;` en `__MakeRotation_unk`, la posicion del par por
  `((MapInstance_unk*)mA)->mInst.mMtx.GetTrans_unk()` y la matriz de mB por su tipo, `d`/`y` (los dos
  fabs) como locales, el cociente del rate con `den`/`num` locales (den primero) y `mFlags |= 1` antes de
  `m20 = 0x4000`.
- gmk/PassBoard vf0C (98,67): perm.py de count/object/hash/name/i (120 ordenes): 98,36 como mucho.
- gmk/RandomMerchant vf0C (99,77 -> 100): la creacion del personaje en una inline de dos niveles
  (`NewMerchant_unk` -> `NewChara_unk` con el std::string y la asignacion de mChara; con un solo nivel se
  invierten los dos bytes de iterator_tag del ctor de string; devolviendo el puntero, el store de mChara
  sale tarde) y `MerchantEntry_unk* e = &mEntries[i]` en el ultimo bucle.
- gmk/RandomMerchant vf20 (99,32): perm.py de entry/count/posCount/field (24 ordenes): igual; `p` local para
  la posicion (97,8), inlines SetAppearPos/Rot con self e indice: igual. El cursor de posiciones (reduccion
  de fuerza del compilador) toma r26 y el de entradas r25 (al reves en el original), como en PassBoard vf0C.
- gmk/MotionObject vf24 (99,94): perm.py de size/data/model/fade/sw/field (720 ordenes): 99,94 en todos.
- gmk/MotionObject vf10 (97,79): perm.py de model/play/efp/colli/state (97,88 como mucho, sin commit),
  prevEfp/prevColli arriba o en otro orden, prevEfp sin local (96,6). El original da a prevEfp el ultimo
  registro (r24) y a this r25.
- gmk/LandMine Start: stmtperm de las 5 asignaciones (120 ordenes): 98,33 como mucho; esfera con nombre,
  `zero`, radio, desc local: igual; rotY local primero: 58,9.
- gmk/ShakeCamera vf10: perm.py de rate/x/y: 98,18 como mucho.
- gmk/HealPoint fn_8040ED00: `SetPlace_unk(desc->mPos, Vector3(0, rotY, 0))` miembro inline carga rotY
  primero como el original pero sigue tras los stores de mWait/m54 (3 diferencias semanticas en vez de 4);
  m54 despues, mWait en el cuerpo, rotY por parametro: igual o peor. Sin commit (toca la cabecera).
- gmk/BulletPoint vf10: perm.py de bullet/skill/table/id/place (120): igual; FindPlace en inline estatica
  con (tabla, id), (id, tabla), puntero a lugares, `unsigned i` fuera: igual o peor.
- gmk/FlamingArrow Collect_unk: while, cast a CharaTask*, slot arriba, perm.py de los locales de
  CanTarget_unk (24): 96,09 en todos.
- gmk/RandomMerchant vf0C antes del arreglo: ramas del if invertidas (33 %), Vector3 con nombre, inline
  ResetScale_unk: igual.
- gmk/MapLift fn_8042AB58: columnas de MaxScale_unk como locales en orden directo/inverso, Vector3 de
  columnas, escala con nombre: igual o peor. vf20 es el push_back conocido (no tocado).
- gmk/Ocean fn_804188A0: copia local de pos (asignacion, `=`, xyz), indices como locales, base sin zoff,
  tabla al final, zi antes que xi, inicializador de corners: igual o peor (81,15 se queda).

## Para el port

- `atn::NodeChunk::mHash`: `int` -> `unsigned int` (hash de fn_800DC6B4; solo lo usa gmk/SwingObject).
- gmk/SwingObject: `PlayEfp_unk` deja de ser inline (global, la quita el enlazador), `SwingUpdate_unk` nueva
  sin llamadores, `RotateAxis2_unk` borrada (fn_803FEF78 escribe las tres rotaciones), `AxisMtx_unk` devuelve
  `MtxPtr`. Mismo comportamiento.
- Literales: ninguno corregido. Ninguna unidad del port tocada.

## Decisiones tomadas sin el usuario

- gmk/SwingObject: funcion inventada sin llamadores `SwingUpdate_unk` (la quita el enlazador) y `PlayEfp_unk`
  no inline, para el orden de `.sdata2` del original (permitido por las reglas del lote, con `// inferred:`).
- gmk/SwingObject LoadSwing_unk: `if (count < 2)` alrededor del `for` que copia los PARAM que faltan (guarda
  redundante pero con efecto en el codigo; es lo que compila el original).
- Inlines estaticas nuevas (nombres inventados): ToRad_unk, SetLod_unk, ParseAxis_unk (SwingObject);
  PlaySe_unk, SetColliBox_unk (PassBoard); NewChara_unk, NewMerchant_unk (RandomMerchant).

## Hallazgos

- Orden de `.sdata2`: las constantes se crean al compilar cada funcion en el orden del fuente, con las inlines
  expandidas en su sitio (no por el texto de la inline); una funcion no inline sin llamadores (o que MWCC
  inlinea y el enlazador quita) colocada antes fija el orden (gmk/SwingObject). (escribir-20261116 qN5)
- Pila: los locales de la propia funcion van por encima de los de las inlines, y cada nivel mas de inline
  invierte el orden de sus locales (bloques en espejo = la segunda expansion un nivel mas honda): gmk/PassBoard
  vf10, gmk/RandomMerchant vf0C (dos niveles), gmk/SwingObject fn_803FEF78. (escribir-20261116 qN5)
- `addi rN, r1, X` repetido en cada caso de un switch antes del salto a una llamada comun: la inline
  devuelve el puntero en cada `case` (`return r.m;`) y la llamada lo usa como argumento. (escribir-20261116 qN5)
- `cmplw` en una comparacion de hashes: uno de los dos es `unsigned` (fn_800DC6B4 devuelve unsigned). (escribir-20261116 qN5)
- Colores de FPR de una expresion: subexpresiones como locales con nombre (`den` antes que `num`, los dos
  fabs) y la posicion por `GetTrans_unk()` de la matriz real (gmk/PassBoard vf10). (escribir-20261116 qN5)
- Ternario `x += c ? 1 : 2` sale sin saltos (subi/subfic/nor/srawi); el original con saltos es un if/else
  anidado. Un bucle de 2 iteraciones sin bucle en el original: dos llamadas a una inline. (escribir-20261116 qN5)

## Bloqueos

- Reduccion de fuerza: el cursor que crea el compilador para `a[i]` toma otro registro no volatil que en el
  original (gmk/PassBoard vf0C, gmk/RandomMerchant vf20); el orden de declaracion no lo mueve.
- FPR de UpdateSwing_unk (mMaxWind/mWindPower cruzados): bloquea SwingObject vf10, SwingPart fn_803FFF40 y
  fn_804003E0.

## Cambios de splits/symbols

- splits.txt sin cambios. symbols.txt: renumerados `@N` de gmk/SwingObject (pd.sh y por direccion:
  `lbl_808860D8` -> `@N` local, los dos `@8917` a nombres unicos), gmk/Ocean, gmk/PassBoard y
  gmk/RandomMerchant.
- Scripts (`/home/claude/scratch-N5/bin/`): `sd2.py <unidad> <lo> <hi> [--apply]` (`.sdata2` del DOL con los
  nombres de symbols.txt al lado del nuestro; `--apply` renombra por direccion las etiquetas con el mismo
  valor), `var.py <unidad> <simbolo> <base.cpp> <viejo> <nuevo>` (una variante), `ops.sh` (diferencias
  semanticas por funcion), `sw3.sh` (las tres funciones de UpdateSwing_unk).
