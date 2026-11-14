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

## Decisiones tomadas sin el usuario
- `symbols.txt`: solo renombrados de `@N` de BattleTask (pd.sh) y el par duplicado `@37207` -> `@37148`/`@37149`.

## Para el port
- Sin cambios de firmas, tipos ni layouts (solo cuerpos de funciones de `game/BattleTask.cpp`).
