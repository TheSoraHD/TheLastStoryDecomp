# Cola H2 (paralelo-20261113): segunda pasada de Last Cocoon y Quark

Rama `agent-H2`. Inicio 09:28 (Madrid).

## Resumen

| Unidad | Sin escribir antes -> despues | 100 % (fnstat) |
|---|---|---|
| game/EnemyQuarkSecond | 5 -> 0 | 44 -> 46 / 70 (fn_80341BEC, fn_80344780) |
| game/LastCocoonImago | 4 -> 2 (quedan el ctor, por presupuesto, y fn_80459FB8 = vfBC, bloqueada) | 19 -> 20 / 36 (vfCC, por las etiquetas) |
| game/LastCocoonEgg | 8 -> 6 (quedan las 6 copias debiles de vf0C/vf10) | 21 -> 21 / 42 |

## game/EnemyQuarkSecond

Escritas (0 % -> escritas y equivalentes, todas `NONMATCHING`):

- `fn_8033C490` (estado 6, laser: gira hacia el objetivo con ToRotationZ_unk, matriz muerta con mRot * 0.3, efecto
  m15BC en el frame 70; en el 75 dispara el laser de "head" a "spine" del objetivo con el efecto m15C8 sobre la
  matriz m15E0): 99,35 %. Pila y el orden de las cargas de las comparaciones de los angulos y/x.
- `fn_8033E058` (estado 10, salto de ataque: ralentiza el motion entre los frames 65-67, lerp del inicio al
  destino del salto con `MAX(0, MIN(1, (f - 58) / 3))`, sigue al objetivo hasta el 49 con un salto
  `QuarkSetJumpZ_unk` y retrocede 45; golpe entre 61 y 65): 95,6 %. Pila y registros.
- `fn_8033EF90` (estado 8, espadas: salta al centro, canta la habilidad m14DC con mFB8/mFBC, cinco espadas
  teledirigidas en abanico desde "spine"): 97,2 %. El original tiene un marco 0x50 mayor con cuatro `1.0f`
  muertos separados 0x14 (inline sin identificar).
- `fn_8033F7FC` (estado 15, lluvia: esfera m1668 sobre la matriz m15E0 trasladada y escalada, crece de 0,1 a 4 y
  encoge a 0,01 mientras cae una lluvia de tres balas por miembro del grupo): 97,7 %. Las cuatro escalas son
  `static float` de la `.sdata` de la unidad (`sGrowFrom_unk`...).
- `fn_803412AC` (estado 17, final: gira al lider, pasa a la tercera forma con el motion 0x1E1, crece de 0,4 a 1
  (`sFinaleFrom_unk`/`sFinaleTo_unk` en `.sdata`) con la camara del campo en modo 4 mirando "spine1", sacude la
  camara en el frame 108 y se oculta en el 110): 99 %.

`report.json` 15357 -> 15358 (vfCC de Imago al 100 %, mnemcheck limpio), lost 0, sin bajadas, PRECOMMIT OK en
los dos commits.

## Decisiones tomadas sin el usuario

- `fn_8023A680` declarada en EnemyQuarkSecond.cpp con `const void* target` (tipo 5: una Matrix3x4), como
  gmk/PlayEffect.cpp con un modelo; la declaracion de `fn_800F8574` sube en el fuente (antes de su primer uso).
- La camara del campo en fn_803412AC usa una vista `QuarkField_unk` local (camara, parametros, mirada y sacudida,
  copiadas de game/Em101ElderZesya.cpp) con un cast desde `lbl_8087F430` (Field_unk del fichero solo tiene lo que
  borra fn_80344BB4). Deuda de la regla 10 (un tipo por objeto): unificar con GameTask/GameCamera.
- `.sdata` 0x8087DC68-0x8087DC7C definidas en el fuente como `static float` (pd.sh las renombra).
- Etiquetas `.sdata2` ambiguas para pd.sh renombradas por valor (script de scratch-K, valrelabel) en
  EnemyQuarkSecond (11), LastCocoonImago (5 + `@17937` -> `@19245` a mano, 0.0 ambiguo) y LastCocoonEgg (8; se
  deja `lbl_80886C18`, otro 0.0, sin tocar para no duplicar `@15168`).
- `fn_80455638`/`fn_804562E4` de Egg pasan a miembros (renombrado en symbols.txt), como las de Imago.
- Imago `fn_8045D3A4`: copia de la camara del campo con `atn::Camera` (copia implicita), como EnemyQuarkStand.
- poolcheck avisa de floats 2.44346 / 25 / 20 (y la cadena '?fff?333?' en Egg): vienen de las copias debiles de
  CharaTask::vfA0/vf70/vf58 (cabecera), no de las funciones nuevas; avisos previos.

## Hallazgos

- El giro hacia el objetivo de los estados de Quark usa la forma `ToRotationZ_unk` (z leido una vez: `fmr f2, f4`
  antes de atan2) y guarda el angulo en f31 (`float target = (...).ToRotationZ_unk().y`).
- `(frame - a) / (len - b)` con el frame leido antes de la llamada: inline con `float frame = ...; return ...`
  (fn_803412AC 96,9 -> 98,9 %).
- Puntero al gestor de balas en un local justo antes de la llamada (`Effect_unk* mgr = lbl_8087F048;`) cuando el
  original lo carga despues de los `rand()` (fn_8033F7FC 95,4 -> 97,7 %).

- `fn_80341BEC` llamaba a "SE_EM211_012"; el original usa "SE_SP007_001" (stringBase0+0x36F). Corregido: con
  todas las funciones escritas el pool coincide y `fn_80341BEC` y `fn_80344780` (debug menu) pasan a 100 %
  (mnemcheck limpio); report 15358 -> 15360.

## game/LastCocoonImago

- `fn_8045D3A4` (modo 12: camara de las curvas xsca "cm000_em206_00" alrededor del imago, como
  EnemyQuarkStand::fn_8034DE90; al acabar el motion desactiva las capsulas; efectos m196C y "SE_EM206_011" en
  los frames 139-220; bandera m240 de lbl_8087EFA8 entre 140 y el final - 10; desde el 200 la camara del campo
  vuelve (fn_80392D2C) y pasa al modo 14 con el mensaje 3 a m18F8): 95,2 %.
- `fn_8045EDEC` (flecha del punto debil `index`: si la camara del campo esta en modo de mirar (m7FC 3/1/4) y la
  esfera esta delante y visible, GameUI::fn_803E3050 y una vez el aviso del ataque sImagoAttacks_unk[index].m10;
  si no, cerca (800) y visible, tambien la muestra; como gmk/WindyItem): 73,6 %. El original escribe -1/0 en
  siete palabras muertas de la pila al empezar (inline sin identificar).
- Field_unk de LastCocoonImago.cpp gana la camara `atn::Camera m006C`; incluye atn/Camera_unk.h y
  atn/RenderManager.h (renumera solo la unidad; pd + valrelabel).

## game/LastCocoonEgg

- `fn_80455638` (drenaje de vida: cada 40 frames con balas sobre un atacante, 50 PV por bala en un orbe
  `LastCocoonEggSeg_unk` que vuela de su "spine" al huevo y lo cura al llegar; aviso m14 la primera vez que el
  atacante tiene el estado 0x20000): 98,7 %. Renombrada en symbols.txt a `fn_80455638__13LastCocoonEggFv`.
- `fn_804562E4` (modo 7: carga 60 + 45 frames con los efectos m252C/m2538 y "SE_EM206_023"; una bala 0x5AB a
  "hips" de cada miembro atacable y seis abanicos aleatorios de la habilidad 0x5AC; la bala sale donde el
  segmento de 30 corta la capsula propia, fn_80053BD0): 87,2 %. Renombrada a `fn_804562E4__13LastCocoonEggFv`.
- `LastCocoonEggSeg_unk` con sus campos (m00 en uso, m04 posicion, m10 PV, m14 inicio, m20 fin, m2C matriz del
  efecto m1994); el constructor sigue limpiando solo m00. LastCocoonEgg.cpp incluye game/BattleTask.h
  (fn_80101340).

- Egg: el orden de `@stringBase0` no coincide porque vf0C inlinea fn_8045817C (cadenas del menu de depuracion) y
  vf10 inlinea fn_80456E1C ("SE_EM206_027"); el original las llama (presupuesto, bloqueo conocido). Por eso
  fn_8045531C sigue en 99,7 %.

## Pendientes

- Egg: las seis copias debiles de vf0C/vf10 (presupuesto de inlining). Imago: ctor (presupuesto) y vfBC
  (argumento en CharaTask::vfBC, renumera 217 unidades).
- Deuda de la regla 10: vistas locales de lbl_8087F430 (Field_unk / QuarkField_unk) en lugar de GameTask.
