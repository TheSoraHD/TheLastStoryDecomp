# Cola K (enemigos) - lote paralelo 2026-11-13

Inicio: 2026-11-13 05:03 CET. Rama `agent-K`. Tope 07:33.

## Resultado por unidad (funciones al 0 % / sin escribir, antes -> despues)

- `game/EnemyQuarkSecond`: 10 -> 6 (en este commit). Escritas, todas `NONMATCHING`:
  - `fn_8033DB8C` (estado 14, rafaga de balas teledirigidas a la party) 96,8 %: huecos de pila (parametros
    de la bala por encima de las matrices en el original) y un registro guardado mas.
  - `fn_8033E8D4` (paso del salto: subida 0x13F, vuelo 0x140, aterrizaje 0x141) 98 %: huecos de pila de los
    vectores de distancia.
  - `fn_803402C0` (atrae una bala a la cabeza; guarda su habilidad) 95 %: relectura del contador y huecos de
    los temporales de las dos ramas.
  - `fn_803405C0` (estado 16, ataque del baston) 98,8 %: huecos de los locales del efecto del paso 0 y
    registros.

- `game/Em316LaserSphere`: 6 -> 0 (todas escritas). `fn_80318AA4` (el personaje apunta a la esfera: magia lanzada, su habilidad o
  la linea de su disparo cruza la capsula) escrita, NONMATCHING 98,4 %: marco 0x10 mayor y registros.
  Se incluye `game/BattleCharaTask.h` (`fn_800E932C`): renumera los `@N` de la unidad (+227); pd.sh y dos a mano
  (`@13913`, `@13962`, floats 0.1 y 0.2 que solo usan funciones sin escribir; tras fn_8031A670, `@14344`, `@14393`).
  `fn_8031A670` (esquiva el ataque de un personaje: estado 10 y luego 7) escrita, NONMATCHING 30 %: el original
  inlinea los miembros pequenos y dos de las cuatro posiciones "spine1" y llama a ToRotation_unk y fn_80318648
  (patron R5 de docs/inlining.md: su cuerpo es una inline fuera de presupuesto); aqui pasa el presupuesto y lo llama
  todo. Con una inline envoltorio sube al 67 %, pero entonces fn_80318A30 inlinea fn_8031A670 y baja de 100 a 0: se
  deja la forma directa. `Em316LaserSphere::m1524` pasa de `int` a `CharaTask*` (el atacante).
  `fn_80317EF8` (estado 1: dispara al acabar la espera o se mueve a un sitio aleatorio alrededor del objetivo)
  escrita, NONMATCHING 90,8 %: registros guardados (f30/f31 y un GPR mas). El sitio de destino es una inline con
  dos `return` (`LaserMovePlace_unk`: el original escribe la suma directamente en el local) y el aleatorio
  `lo + (hi - lo) * rand() / 32767` (`LaserRand_unk`: 0.20000005 = 1.2f - 1.0f plegado).
  `fn_80319C90` (dispara el laser: matriz trasladada, girada a lo largo del rayo y escalada a su longitud)
  escrita, NONMATCHING 98,3 %: huecos de pila de los temporales.
  `fn_803176D0` (reparte los objetivos: ordena las esferas activas por el id de su objetivo con
  `vector::insert`, y da a cada una su indice m14C8 y el tamano de su grupo m14CC) escrita, NONMATCHING 97,7 %
  (el original prueba la bandera del lider una vez para los dos objetivos y los destructores de los vectores
  prueban el vector una vez mas). La copia weak 0x8031ACC0 es `vector<Em316LaserSphere*>::__insert_one_unk`
  (renombrada en `symbols.txt`; 17 %, como la NONMATCHING conocida de `std/vector.h`). `lbl_8087DC50` (1 en
  `.sdata`) es `static int sFirstCount_unk = 1` (pd.sh la renombra).

## Decisiones tomadas sin el usuario

- `fn_8033E8D4` y `fn_803402C0` son funciones `static` del fichero con el enemigo como primer argumento (el
  simbolo del original no tiene clase); renombradas en `symbols.txt` a su nombre mangled con `scope:local`
  (como Em316LastLaser en el lote anterior). Si en realidad fueran miembros, basta con moverlas a la clase.
- `lbl_80885304` (35.0f, de esta unidad) deja de ser `extern`: literal en `fn_80341BEC` y `fn_803405C0`
  (regla 10); pd.sh lo renombra a `@N`.
- `QuarkHoming_unk` (parametros de la bala teledirigida) sin el giro por defecto 0.0942478: el pool de la
  unidad no lo tiene (el llamador siempre lo pone) y dejarlo anadia un float al `.sdata2`.
- `QuarkHits_unk::mData` se queda como `int[32]`: se guarda el `HitInfo_unk*` de la bala con un cast.

## Hallazgos

- `bge X; b Y; X: fmr` en un minimo de floats: inline `if (a < b) { return a; } return b;` (QuarkMin_unk).
- `li r0, 0; ... li r0, 1; cmpwi r0` tras la llamada a la longitud de la animacion: inline con
  `float end = len - n; bool r = false; if (frame >= end) r = true; return r;` (con la resta dentro del `if`
  el `bool` sube a un registro guardado antes de la llamada).

## Pendientes

- EnemyQuarkSecond: fn_8033C490, fn_8033D040, fn_8033E058, fn_8033EF90, fn_8033F7FC, fn_803412AC.
