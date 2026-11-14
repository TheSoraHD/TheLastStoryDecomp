# Cola K (cabeceras-20261114): segunda pasada sobre BattleTask (matching neutro)

## Resultado por paso

### 1. `.data` de BattleTask (0x190 en nuestro .o frente a 0x148)
- Lo que sobra y es normal (igual en unidades ya enlazadas como BootTask, 0x58 frente a 0x38): `__RTTI__Q23atn4Task`,
  `__RTTI__Q23atn8TaskBase` y la lista de bases de Task (debiles; el enlazador se queda con la primera copia), y los
  `@STRING@` debiles de `basic_string::reserve/do_replace/compare` (funciones debiles que el original emite en otra
  unidad).
- Diferencia real: el pool `@wstringBase0` del original tiene 0x34 bytes: `L"%2d/%2d"`, `L"%d"`, `L"%d: %s"`, un
  `L""` sin referencias (0x24) y un segundo `L"%d"` (0x26) que usa `fn_80108C10` (no fusionado con el de 0x10). El
  nuestro tiene 0x24 (fn_80108C10 reutiliza el `L"%d"` de 0x10). Sin resolver: `wchar_t text[2] = L""`
  (fn_801061BC, 100 %) no deja la cadena en el pool; `L""[0]` si la deja pero no es codigo natural; ninguna forma
  probada (array local const, `static inline`, `text[32] = L""`, ...) evita la fusion del segundo `L"%d"`.

### 2. `MeleeSort_unk` 73,3 -> 92,5 %
- Ordenacion por seleccion con copias de los iteradores (se leen una vez), mediana de tres con copias de a, b y
  `last - 1` (el pivote queda en registro), llamadas recursivas con copias de los iteradores, `back = last - 1`
  tras el intercambio, `b = first + n * 3 / 4 + sOffset % 5` (a sin cambios).
- Queda: el `i < j` al final de los dos bucles de particion sale como booleano materializado
  (`xor/cntlzw/slw/srwi.`, 0x800FA788 y 0x800FA904) y el `--j` antes de cargar `*j`. Probado sin exito: inline que
  devuelve bool (por valor y por referencia), `bool more = i < j`, `(i < j) == false`, `j <= i`, `do {--j;} while`,
  `while (i < j)` como bucle, inline del paso de particion.
- Hallazgo: con `-ipa file` una inline de plantilla nueva en el .cpp (y tambien variables locales nuevas en una
  funcion) renumera los `@N` de la unidad: hace falta `precommit.sh --pd`. Ademas, el par duplicado `@37207`
  (0x8088157C/0x80881580, dos nombres iguales en symbols.txt, de antes) no lo arregla pd: renombrado a mano.

### 3. `fn_800FDF14` 94,3 -> 95,7 %
- El marco de pila 0x100 mayor venia de que MWCC convertia en escalares el `HitDamage_unk` del bloque de la
  absorcion de odio (nunca se pasa por direccion): pasando `damage.m04` (y no `heal`) a `fn_8012DF7C` el struct
  queda en memoria y el marco es 0x450 como el original (94,27 -> 94,38).
- `HitPos_unk(CharaTask*, const int& index)`: el original relee `p->m40` en la segunda llamada (94,38 -> 94,99).
- Textos de los paneles `hpUpNum`/`hpUpSubNum` por una inline de fichero `SetPaneText_unk` (el layout se evalua
  antes del hash, como `SetPaneValue_unk`) (-> 95,59); `radius` antes de `d` en el tiro a la cabeza (-> 95,65).
- Queda: booleanos materializados del original en `CanAct_unk`/`IsFree_unk` (li/cmpwi; el nuestro los pliega),
  ranuras de pila de varios bloques (en el original los locales del tiro a la cabeza estan en 0x50/0x5C) y la
  temporal de `HitPos_unk` (0xA0 frente a 0x88).

### 4. Funciones al 99,x %
- `BattleTask::fn_801002DC` 99,93 -> **100 %**: el punto medio de la capsula en un `atn::Vector3 center` con nombre
  (ordena las ranuras de pila como el original).
- Probadas sin exito (asignacion de registros o ranuras de pila): `fn_800FBA9C` (`frsp` del `__fabs`),
  `fn_800FDB20` (orden de los campos del dano), `fn_800F8574` (ranuras de `rot`/`v`/matrices), `fn_8010089C`
  (`center.y += m5B0`), `fn_80101524` (la carga de `lbl_8087F3C0` antes del ternario del efecto).
- Etiquetas: tras `pd.sh` quedan nombres `@N` duplicados o viejos en `.sdata2` (pd no renombra los que no puede
  emparejar); se corrigen por desplazamiento y valor con un script (mismo desplazamiento y mismo valor en el .o
  original y el nuestro, hasta 0x80881594; despues el pool diverge por el float de mas).

- `fn_800F80B8` 97,41 -> **100 %**: la rotacion z-y-x por la inline de fichero `MakeRotZYX_unk(atn::Matrix3x4* p,
  const atn::Vector3&)` (en lugar de `Matrix3x4::RotateZYX_unk` de `Vec.h`) y la concatenacion con `p`.
- `fn_800F8290` 82,57 -> **100 %**: la escala pasa por un `atn::Vector3 v(x, y, z)` local (el original guarda los
  tres floats en la pila con `frsp`).
- Rotacion z-y-x de `chara->mRot`/`rot` (identidad + tres `RotateAxis_unk`) en una inline de fichero
  `MakeRotZYX_unk(const atn::Vector3&)` que devuelve la matriz: las temporales de las rotaciones salen en el orden del
  original. `fn_800FC410` 99,46 -> 99,82 %, `fn_800FCD24` 98,10 -> 98,46 %, `fn_800F8574` 99,58 -> 99,61 %. Queda
  en las tres el orden de otras ranuras (en el original los locales declarados al final, `n` y el `Mtx` del else,
  quedan por debajo de todo; en el nuestro, por encima de las temporales) y 0x30 bytes sin usar en 0xD8 de
  fn_800FC410.

## Decisiones tomadas sin el usuario
- `symbols.txt`: solo renombrados de `@N` de BattleTask (pd.sh, y a mano/por script los duplicados que pd deja).

## Para el port
- Sin cambios de firmas, tipos ni layouts (solo cuerpos de funciones de `game/BattleTask.cpp`).
