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

### 2b. `fn_800F8C6C` (90,6 %, sin cambios)
- La mayor parte de lo que falta viene del `push_back` de `std::vector<MeleeTarget_unk>` (`std/vector.h`, cola A4):
  el original deja `__files` (stderr de los `fprintf` de `__msl_error`) en r31 fuera del bucle y el nuestro lo
  recarga en cada llamada; no se toca aqui.

### 3. `fn_800FDF14` 94,3 -> 96,0 %
- El marco de pila 0x100 mayor venia de que MWCC convertia en escalares el `HitDamage_unk` del bloque de la
  absorcion de odio (nunca se pasa por direccion): pasando `damage.m04` (y no `heal`) a `fn_8012DF7C` el struct
  queda en memoria y el marco es 0x450 como el original (94,27 -> 94,38).
- `HitPos_unk(CharaTask*, const int& index)`: el original relee `p->m40` en la segunda llamada (94,38 -> 94,99).
- Textos de los paneles `hpUpNum`/`hpUpSubNum` por una inline de fichero `SetPaneText_unk` (el layout se evalua
  antes del hash, como `SetPaneValue_unk`) (-> 95,59); `radius` antes de `d` en el tiro a la cabeza (-> 95,65);
  `attacker->GetInfo_unk()->m04` en el mensaje del robo (el original carga `m60` antes del texto, -> 95,75);
  literales sin signo en `(p->m94 & 0x8000u) == 0x8000u`, `(m38A5C.m94 & 0x10u) == 0x10u` y `m9C & 0x8000u`
  (el original compara con `cmplwi` y sin el `addis r0, r3, 0` del nuestro, -> 95,95).
- `target->vf3C(p)`: el original pone `r5 = 1` antes de la llamada (vf3C con un segundo argumento?); no se toca
  (firma compartida de `CharaTask`, port).
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

### Otras probadas sin exito
- `fn_80105DCC` y `fn_800FB1BC`: el original carga `lbl_8087F3C0` antes de elegir el EFP / de los argumentos (la
  llamada escrita a mano o el color temporal empeoran); `fn_80106878`: marco 0x10 mayor (orden de la temporal
  `std::string`, del color blanco y del nombre; con una `std::string path` con nombre o el color temporal baja);
  `fn_80108C10`: el `bne/b` del retorno temprano (ifs anidados, `!m0C`, inline booleana: nada); `Lerp_unk`: el
  orden de los operandos pediria cambiar `math/Vec.h` (lo usan mas de 20 unidades), no se toca.

## Resumen de cifras
- BattleTask: 165 -> 168 / 212 funciones al 100 % (`fn_801002DC`, `fn_800F80B8`, `fn_800F8290`; `report.json`
  15447 -> 15450 en total), 0 al 0 % antes y despues. Subidas: MeleeSort_unk 73,3 -> 92,5, fn_800FDF14 94,3 -> 96,0,
  fn_800FC410 99,46 -> 99,82, fn_800FCD24 98,10 -> 98,46, fn_800F8574 99,58 -> 99,61, fn_800FD658 93,16 -> 93,44.
- `.data` sin cambios (ver paso 1).

## Decisiones tomadas sin el usuario
- `symbols.txt`: solo renombrados de `@N` de BattleTask (pd.sh, y a mano/por script los duplicados que pd deja).

## Para el port
- Sin cambios de firmas, tipos ni layouts de clases compartidas: solo cuerpos de funciones de `game/BattleTask.cpp`.
- Inlines de fichero en `game/BattleTask.cpp` (no en cabeceras): nuevas `MakeRotZYX_unk(atn::Matrix3x4*, const
  atn::Vector3&)` y `SetPaneText_unk(void*, const char*, const wchar_t*)`; `HitPos_unk(CharaTask*, int)` pasa a
  `HitPos_unk(CharaTask*, const int&)`. `fn_800F80B8` ya no llama a `Matrix3x4::RotateZYX_unk`.

## Hallazgos (una linea cada uno)
- MWCC convierte en escalares un struct local que nunca se pasa por direccion (cada campo en su ranura, marco
  mayor); leer el campo del struct en vez de la variable de la que se copio lo deja en memoria (fn_800FDF14).
- Un parametro `const int&` en una inline hace que el original relea el campo en cada uso (`p->m40`).
- La rotacion z-y-x dentro de una inline propia deja las temporales de las tres rotaciones en orden ascendente.
- Un float que el original guarda en la pila con `stfs` + `frsp` al entrar: los argumentos se copian a un
  `atn::Vector3` local (fn_800F8290).
