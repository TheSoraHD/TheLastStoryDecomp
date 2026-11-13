# Cola H2 (paralelo-20261113): segunda pasada de Last Cocoon y Quark

Rama `agent-H2`. Inicio 09:28 (Madrid).

## Resumen

| Unidad | Sin escribir antes -> despues | 100 % (fnstat) |
|---|---|---|
| game/EnemyQuarkSecond | 5 -> 0 | 44 -> 44 / 70 (pendiente de confirmar tras pd) |

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

## Decisiones tomadas sin el usuario

- `fn_8023A680` declarada en EnemyQuarkSecond.cpp con `const void* target` (tipo 5: una Matrix3x4), como
  gmk/PlayEffect.cpp con un modelo; la declaracion de `fn_800F8574` sube en el fuente (antes de su primer uso).
- La camara del campo en fn_803412AC usa una vista `QuarkField_unk` local (camara, parametros, mirada y sacudida,
  copiadas de game/Em101ElderZesya.cpp) con un cast desde `lbl_8087F430` (Field_unk del fichero solo tiene lo que
  borra fn_80344BB4). Deuda de la regla 10 (un tipo por objeto): unificar con GameTask/GameCamera.
- `.sdata` 0x8087DC68-0x8087DC7C definidas en el fuente como `static float` (pd.sh las renombra).

## Hallazgos

- El giro hacia el objetivo de los estados de Quark usa la forma `ToRotationZ_unk` (z leido una vez: `fmr f2, f4`
  antes de atan2) y guarda el angulo en f31 (`float target = (...).ToRotationZ_unk().y`).
- `(frame - a) / (len - b)` con el frame leido antes de la llamada: inline con `float frame = ...; return ...`
  (fn_803412AC 96,9 -> 98,9 %).
- Puntero al gestor de balas en un local justo antes de la llamada (`Effect_unk* mgr = lbl_8087F048;`) cuando el
  original lo carga despues de los `rand()` (fn_8033F7FC 95,4 -> 97,7 %).

## Pendientes

- LastCocoonEgg (fn_80455638, fn_804562E4, copias debiles) y LastCocoonImago (fn_8045D3A4, fn_8045EDEC, ctor):
  sin tocar en esta pasada (ver abajo si se avanzan).
