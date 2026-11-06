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


## `IpcReplyHandler` (ipcclt.c, 0x8061BDC0)

El original compila `switch (req->base.fd)` (casos 3, 6, 7) como arbol (`cmpwi 6; beq; bge; cmpwi 3`); `Wii/1.0-1.3` dan cadena lineal. Probado (2026-11-05): casos vacios extra, `default`, casts a enum/u32/u8; los `GC/3.0a*` dan el arbol pero cambian la programacion de las inline de `__ios_Ipc1` en las funciones sincronas.

## `TeamAI_unk::fn_805AE864` y sus copias inline (0x805AE864)

Indice/personaje en r28/r27 en vez de r27/r28 y los dos `Vector3` (dir, back) intercambiados en la pila. Probado: declarar `back` primero, temporal `&Vector3(...)`, variables fuera del bucle.

## `CXUncompressAny` (CXUncompression.c, 0x806254D0)

El original coloca el `CXUncompressRL` inline detras de las tres llamadas de cola, con un `b` de trampolin desde el `beq` del caso 0x30 y un `nop` de alineacion del bucle. Probado (2026-11-06): `switch` y cadena de `if`, `break`/`return` por caso, RL definida antes/despues de `Any`, `inline`/`static inline`, `default: break`, `case RL: break;` con la llamada tras el `switch` (queda a 2 instrucciones: falta el trampolin), compiladores Wii/1.0-1.3.

## `dwc_match` (mscharged R4QK01 + 2010): `DWCi_MatchProcess`, `DWCi_ProcessMatchCommand`, `DWCi_CancelMatchingProcess`, `DWCi_CloseUnacknowledgedClients`

Solo registros (r28/r30 en ProcessMatchCommand, base de `.data` en r31/r30 en CancelMatchingProcess, contadores en CloseUnacknowledgedClients). Probado: cinco ordenes de declaracion en CloseUnacknowledgedClients.

## WUD `fn_80670C70` (0x80670C70)

El `switch (_nandStep)` de 6 casos es un arbol en el original (cadena en Wii/1.0-1.3 y 0x4201_127; GC/3.0a5.2 baja otras funciones) y el bucle de suma de RPHealth.dat se desenrolla x4 (nuestro x8). Probado: casos 6/0xFF explicitos, indexado `ptr[i]`.

## Argumentos `float` cargados antes de la rama de un argumento condicional (MP)

`fn_8006EF48(lbl_8087EEC8, fn_800A4450(pane, 0, 0).data(), 16.0f, 0.0f, 1, 1)`: el original carga el `16.0f` (y la
fuente) antes de la rama de `data()`; nosotros despues. Igual en `D2AMpMenuMessageList::fn_804C50C0` con los floats de
`fn_80061AE4` (`pos.x - off`, `17.0f`, `-100.0f`) frente a la rama de `msg->mText ? ... : lbl_808813D0`. Afecta a
`D2AMpMenuSetRule::vf0C`, `D2AMpMenuFriendRoom`, `D2AMpMenu::vf0C` y `fn_804AF520`. Probado (`unidades-20261106b`):
`std::wstring` con nombre, `c_str()`, variable `float size`, inline con la cadena/el puntero/el pane, accesor inline
`GetText_unk`, wrapper con el texto como ultimo parametro, y los compiladores Wii 1.0/1.1/1.3/1.5/1.6/1.7/0x4201_127 y
GC 2.6/3.0a5.2 en un fichero suelto (todos ponen la rama primero).

## Pares texto/layout en r28-r31 cruzados (MP)

`SetPaneText_unk(task->mLayout, name, text)` con el texto en un registro y `task + 0x58` en otro: en muchas funciones
de los menus MP los dos registros salen cruzados (SetRule `fn_804CECEC`, Result, Cockpit, CommonDialog, SetMessage
`fn_804CB2F0`/`fn_804CC618`/`fn_804CC7A4`/`fn_804CCB54`/`vf10`). `SetTaskText_unk` (layout antes del hash) arregla
unos casos (SetMessage `fn_804C9360`, `fn_804C8D80`) y no otros; una inline miembro para el bucle o el texto arregla
otros (`CountMessages_unk`, `HasMessage_unk`, `PlayVoice_unk`). Sin regla general.

## KPAD 2010 `.sdata2` (`unidades-20261106b`)

`KPAD.c` 33/33 enlaza salvo el orden de tres entradas de `.sdata2` (0x80888920): `1/200` y `1/100` van antes del
`{1,1,1}` de `KPADiSamplingCallback`, como si una funcion quitada por el enlazador entre `KPADInitEx` y
`KPADiSamplingCallback` las usara primero. No se ha inventado esa funcion.

## Varias vistas de `lbl_8087F628` (`unidades-20261106b`)

`include/MP/D2AMpMenuFriend.h` (`MpData_unk` + `MpFriendList_unk` parcial), `D2AMpMenuSetMessage.cpp`
(`MpMsgData_unk`), `D2AMpMenuMessageList.cpp` (`MpData_unk` local) e `include/atn/net/Manager.h` (`atn::net::Manager`
con el `MpFriendList_unk` completo de `atn/NetFriend_unk.cpp`) no se pueden incluir juntos: `D2AMpMenu::fn_804AF520`
llama a `fn_80509AF8` de `atn::net::Manager` y usa la lista de amigos de la vista de Friend. Hay que unificarlas.

## Busqueda de jugador por aid en los menus MP (`unidades-20261106b`)

`D2AMpMenuLobby::vf0C` (99,35%) y `fn_804C2C14` (99,15%), y la misma busqueda inlineada en `fn_804BE610`,
`fn_804C23E4` y `D2AMpMenuFriendRoom vf0C`: bucle sobre `lbl_8087F610->m5E4[j]` (0xD5C bytes) comparando el aid con
`mCC`. El original deja el aid en r4, el desplazamiento en r3 y el jugador en r5 (sesion en r6); MWCC nos da aid r3,
jugador r4, desplazamiento r5. Probado sin cambio: aid `unsigned char`/`int` en la busqueda y en `GetAid_unk`,
`GetAid_unk` con variable de resultado, como miembro de `MpLobbyData_unk`, busqueda como funcion libre con la
sesion de parametro, sesion en variable local, `ret`/`break` en vez de `return`, indexado directo `m5E4[j].mCC`, aid
en variable antes de la llamada. Pista sin probar del todo: en `fn_804C23E4` el original convierte el aid con
`clrlwi r4, r0, 24` antes del bucle (parametro de 8 bits en la busqueda real).

## hermanas-20261106

- Orden de los registros de un bloque grande de un bucle (NewCampPartsMapSelect `fn_80520B04`, 97,8%: el original da r31-r25 a las variables de la ruta y r24-r20 a capitulo, flag, i, id y this). Probado: otros ordenes de declaracion, inline para el cuerpo, `GetMapType()`.
- `b fn; blr` en una funcion con `if (x < 0 || x >= 3)` (NewCampTask `fn_80530D88`): MWCC junta las dos comparaciones en `cmplwi`; el original las deja separadas con `blt; blr`. Probado: `switch` 0-2, `if` anidados, `return`s separados.
- La copia de las dos medidas de un `EffectLists_unk::Clear_unk` (carga ambos tamanos antes de poner a cero), igual que en `Event::EffectLists_unk` (NewCampTask `fn_8053032C`).
- Pool de cadenas: funciones al 99% (MapSelect `20634`, vf48; Item `B164`, vf48) solo esperan a que se escriban las funciones anteriores que crean sus literales.
