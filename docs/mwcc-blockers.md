# Rama `mwcc-blockers` (2026-10-21): bloqueos recurrentes de MWCC

Propuesta C de `SIGUIENTE_sesion.md`: 4 agentes, uno por bloqueo, cada uno en su copia del repo y
su rama `agent-N`, con tope de ~30 intentos documentados. Condicion de commit: `main.dol: OK`,
`regress.py` lost: 0, mas funciones al 100% y ninguna funcion baja de fuzzy. Base: `main` 6ace171.
Notas por agente: `docs/mwcc-blockers-q{1..4}.md`.

## Resultado

Build limpio (`rm -rf build/SLSP01`) de la rama: `main.dol: OK`, `regress.py` **lost: 0**, 0 bajadas
de fuzzy (127 subidas), **5375 -> 5486 funciones al 100%** (codigo 16,7% -> 18,0%), 335 -> 338
ficheros enlazados. Sin asm ni pragmas nuevos.

| bloqueo | agente | resultado | funciones al 100% |
|---|---|---|---|
| 1. `std::vector::push_back` inline | q1 | sin cambio commiteable (22 variantes de cabecera) | +0 |
| 2. `atn::CsvU` ctor inline, `this` en registro salvado | q2 | **resuelto**: `~CsvBase() {}` en `Csv.h` | +4 (y 7 unidades suben de fuzzy) |
| 3. `__VecCopy` r3/r4 cruzados y copias z-primero | q3 | sin cambio commiteable (16 intentos, patron documentado) | +0 |
| 4. 18 ptmf huerfanos de `.data` de `CharaTask` | q4 | **resuelto**: 7 lanzadores eliminados por el enlazador + envoltorios inline | +107 |

Por unidad (solo las que cambian de funciones al 100%):

| unidad | antes | despues |
|---|---|---|
| `game/CharaTask` | 264/436 | 371/436 |
| `atn/ptr_string_base_wchar_t_0` | 3/4 | 4/4, **enlazada** |
| `db/EquipParam_unk` | 13/14 | 14/14, **enlazada** |
| `db/TutorialTable_unk` | 5/6 | 6/6, **enlazada** |
| `game/GameSaveBanner` | 7/9 | 8/9 |

Suben de fuzzy sin llegar al 100%: `db/SkillParam_unk` (87,8 -> 91,9 en `fn_80219C30`),
`db/MapIdTable_unk` (98,2 -> 99,8), `game/NewStaffRoll` (97,8 -> 99,3), `atn/D2A_MessageTable_unk`,
`game/LibraryTask` (x2), `db/PaletteTable_unk`, `game/CampSaveLoad` (`fn_8011B8D4` 92,1 -> 94,6; lo
que queda es el pool `@wstringBase0`).

## Hallazgos (una linea cada uno; tambien en CLAUDE.md / CLAUDE_lite.md)

- **Constantes de `.data` sin referencias (ptmf `00000000 ffffffff 00000000`)**: son restos de
  funciones que el enlazador elimino; su `.o` original conservaba los datos pero no el codigo.
  Reponerlas como stubs (`Stripped1_unk..7_unk`, cuerpo `fn_80178208(0);`) en la posicion que
  indica el indice de ptmf por funcion (`ptmfmap.py`) recoloca todo `.data` (+105 en CharaTask).
- **Dos cadenas inline iguales en una funcion** (dos `fn_80178208`): el original lanza la segunda
  a traves de un envoltorio inline mas (metodo existente o `static StartX_unk`); el nivel extra da a
  los temporales ptmf los slots de pila del original. Con envoltorio en las dos, se intercambian.
- **`this` de un objeto local en registro salvado durante su ctor inline** (CsvU/CsvTabU): un
  destructor vacio `~CsvBase() {}` en la base lo reproduce (la clase no tiene dtor en el DOL; la
  copia debil `__dt__` que se emite la descarta el enlazador). Solo cambia las derivadas cuyo ctor
  base recibe dos ternarios; los lectores `char` no cambian. La receta de `basic_string` (cast de
  `this` a palabras) no sirve aqui.
- **`push_back`**: `n = mSize; if (n < mCap) { mSize = n + 1; mData[mSize - 1] = x; }` reproduce el
  camino rapido de la copia debil (size antes que cap, cap en el registro bajo); un store a traves de
  `void**` hace que MWCC recargue el elemento tras el store del tamano (un store a `unsigned` no);
  una variable local mas saca `push_back` del autoinline (compensar sacando sentencias a un helper).
  El bloque muerto de `__recommend(1)` solo sobrevive en funciones grandes. Residuo abierto: en
  bucles el original da size r4 / cap r3 y nosotros al reves (11 funciones a 0,02 puntos).
- **`__VecCopy`**: el original tiene UN solo `operator=(const Vector3&)` (CharaLook_unk llama la
  copia debil para lvalues no-const). El orden z-primero/x-primero NO es un helper distinto: el
  scheduler reordena las 4 instrucciones del asm segun el contexto (DOL: ZLST 1423, LZTS 1144, LZST
  357); LZST en ctor y operator= es con diferencia la mejor partida. El cruce r3/r4 aparece cuando el
  src es un temporal (rvalue de un inline): el original materializa el dst antes; con src con nombre
  materializa el src antes (ahi ya coincidimos). Ninguna de las ~25 firmas de helper lo reproduce.

## Lo probado que no funciona (resumen; detalle en cada `-qN.md`)

- q1: comparaciones negadas/invertidas, accesores o referencias para size/cap, `cap`/`n` pasados
  al camino lento, `max` por valor/no inline/if-else/puntero, indexar por `n`, formas con `end()`.
  La mejor variante (`vector_a7.h`, diff completo en q1) da +1 al 100% y 13 subidas con 11 bajadas
  de 0,02-0,04: **decision pendiente del usuario** (aceptarla relajando la condicion o no).
- q2: cast de `this` a palabras, `memset` sobre `this`, alias `CsvBase& self = *this`, helpers
  inline/static para los ternarios, ctores fuera de clase, plantillas miembro, clase intermedia,
  pasar `this` como parametro, ctor base con separadores como parametro.
- q3: orden de parametros del helper (src primero: +9 pero 2 perdidas y 4 bajadas), dos sobrecargas
  de operator=, helpers con `Vector3*`/`void*`/referencias, casts, locales `register`, `volatile`,
  cualquier orden de instrucciones distinto de LZST, `__vec2x32float__` (genera `psq_lx`), quitar
  `Assign_unk` donde esta.
- q4: orden de definicion de la cadena; envoltorio en los dos lanzamientos consecutivos; duplicar
  `fn_801750FC`. `CharaStatus` no inlinea la cadena (sus NONMATCHING son otra cosa).

## Decisiones tomadas sin el usuario

- Stubs `CharaTask::Stripped1_unk..7_unk` (nombres y cuerpo inventados; no estan en el DOL) y
  helpers `static StartPushedAvoid_unk`/`StartDragged_unk` en `CharaTask.cpp`.
- `~CsvBase() {}` en `include/atn/Csv.h` (cabecera compartida; `regress.py` lost: 0).
- Tres unidades pasadas a `matching: true` al quedar al 100% (enlazan con `main.dol: OK`).
- Ningun cambio en `std/vector.h` ni en `math/Vec.h`.

## Adenda (2026-10-21, tras el cierre): `vector_a7.h` aplicada por decision del usuario

El usuario acepto la variante de `std/vector.h` de q1 (camino rapido `n = mSize; if (n < mCap) {
mSize = n + 1; ...}`, store de punteros a traves de `void**`, helper `__vec_set_buf_unk`), relajando
la condicion 8 para sus bajadas de centesimas. Las especializaciones locales de `__vec_imp_unk`
(`U16Table_unk.cpp`, `ShopColor.h`, `LibraryTask.h`) se adaptaron a la firma nueva. Build completo:
`main.dol: OK`, lost: 0, **5486 -> 5487** (`Em002_01BlackSkelton::fn_80248A4C` al 100%), 139 subidas
y 10 bajadas de 0,01-0,04 puntos (NewCampPartsHint x3, ChaserNpc ctor, D2AMpMenuFriend x2,
ArrowSet, Np131LivingArmor, D2AMpMenuFriendRoom, ShopColor), todas por el reparto size/cap en bucles.

## AI_archer_attack01 (2026-10-29, `fix-svenchu-linkage`)

- **Pool de cadenas de `em_scout`**: el original tiene `"隠れるポイント未発見"` (+0x3E0, usada en `vf24`) antes que `"まだ隠れられていない"` (+0x3F5, usada en `vf2C`, que va antes en el `.dol`). Con literales en el fuente MWCC las pone al revés. Probado sin éxito: helper `static inline`, `static` no inline, helper con las dos cadenas y argumento constante, definir el helper antes de `vf2C`. Ningún llamador anterior de `vf24` en el asm. Por eso las cadenas de +0x3E0 en adelante siguen como `lbl_807307A0 + off` (las anteriores ya son literales y el pool coincide hasta +0x3E0).
- **`fn_8001C088`**: MWCC desenrolla los dos bucles de 5 bloques de 0x14 bytes; el original los deja con `mtctr`/`bdnz` sobre las bases de entrada/salida. Probado: índice, punteros con post-incremento, cuenta atrás, `do/while`, helper inline, `operator=` propio en el bloque. Sigue NONMATCHING.
- **`Command_unk(20, "lit", 0, 0, 0)`** (`np_type002::vf20`, `np_wait_reaction_minus` vf0C/vf34, `em_scout`/`em_type106` vf28): los ceros salen en r8 y la cadena en r0 (el original al revés). Probado: literal real, locales explícitos en ambos órdenes, envoltorios de uno y dos niveles. 99,06–99,72%.
- **`np_type002::vf14` / `np_type003::vf14`** (99,32/99,25%): coloreo de la base de salida y del temporal de `m08` en `m18++`; probado estilo `result`, helper `MovePick_unk`, `Vector3` local, orden de stores.
- **Árbol del mapa de scripts** (`fn_8001E730`, `fn_8001EA18`, `fn_8001ECD0`, `fn_8003DD50`, `fn_8003DEE0`, `fn_8003E6B8`): código de `std::map` de MSL con varios niveles de inline (destrucción recursiva inlineada dos niveles, comprobaciones `this` encadenadas); sin la cabecera `tree` de MSL no se reproduce. `fn_8003E120` (rebalanceo) sí sale al 100% a mano.

## `basic_ostream<char>::sentry` / `flush` (atn/WordWrap_unk, 0x800E14A4, 0x800E1630, 0x800E1A48)

Tres funciones de MSL mutuamente recursivas e inline (ctor → `tie()->flush()` → `~sentry` → `flush`). El original las expande un número fijo de niveles e inlinea `setstate` solo en el primero (en los niveles internos lo llama, 0x800E0A64). Escritas en la forma natural, MWCC (Wii/1.3, `-ipa file`) expande más (ctor 0x440 frente a 0x18C) e inlinea `setstate` en todos los niveles; con `setstate` fuera de línea el ctor baja al 12%. Probado (2026-10-29): forma natural con `setstate` inline o no, y cuatro cuerpos de `setstate` con más sentencias para cruzar el umbral de R5 (`docs/inlining.md`): sin cambios. Se queda la forma escrita a mano (35/38/36%). Falta saber cómo limita MWCC la recursión inline.

## `Em303KrakenFoot::fn_802E3EE8` y `CharaTask::IsPlayer() const` (0x802E3EE8)

Con `IsPlayer` const (nombre del RSO) MWCC vuelve a cargar `mType` para el `?:` en vez de reutilizar el valor del `!= 2` anterior (el original hace `cntlzw` sobre él, como da una inline no const: 99,15% frente a 98,96%). Probado (2026-10-29): variable local para el puntero, `int`/`bool` intermedios, cast a `const CharaTask*`, `mType == 0` directo y cinco cuerpos de `IsPlayer` (con `GetType_unk() const`, referencia, `!mType`...): ninguno lo arregla.

