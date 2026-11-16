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
| 3. `__VecCopy` r3/r4 cruzados y copias z-primero | q3 | **resuelto el cruce** en `arranque2-20261110`: copia a traves de una inline `XAssign_unk(dst, src)` (`tools/decomp/asgwrap.py`) | +30 |
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
  **Arreglo (`arranque2-20261110`)**: la asignacion a traves de un nivel de inline mas,
  `static inline void XAssign_unk(atn::Vector3& dst, const atn::Vector3& src) { dst = src; }`, en la copia que sale
  cruzada. `tools/decomp/asgwrap.py <unidad> <simbolo>` prueba cada asignacion; ~30 funciones al 100 %. Donde no
  sirve, el cruce viene de otra cosa (registros del bucle, carga del origen desde un miembro).

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

**Resuelto (enlazar-20261115 qX, la unidad enlaza):** ProcessMatchCommand con `u32 ip` en el ambito de la funcion tras `u16 port` (`SOInAddr` en el bloque del caso, o `u32` en el bloque, no bastaban); CloseUnacknowledgedClients con `pending = DWCi_GetServerNode_unk()` (accesor inline de `&DWCi_GetMatchCnt()->serverNode`).

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
- Palabra de bitfield de un struct en la pila (`lwz rX, 0x54(r1); clrlwi r7, rX, 4`): el original la carga en r3, nosotros en r7 (LastCocoonQuadruped `fn_80465294` 99,8%, igual que Em204Mulue `fn_802A91D0`). Probado: campo de 4 bits solo, `unsigned char`, orden de las asignaciones del constructor, puntero al estado.
- `cmpw r0(entrada), r28(argumento)` al comparar un campo de una tabla `static const` con un argumento: MWCC pone el argumento a la izquierda con cualquier orden de operandos, `!=`/`continue`, referencia a la entrada o inline (LastCocoonQuadruped `fn_804687B0` 99,8%).
- Registro del tiempo de frame (`dt`) en f28 en vez de f29 en vf24 de LastCocoonQuadruped (99,5%): sin cambio con órdenes de declaración, copia tras las longitudes de motion ni pesos separados.
- LastCocoonQuadruped `fn_80467C18` (86%): `stw` de `m15AC` planificado después del paso (un registro guardado de más). Probado: orden m15B0/m15AC, `pos` en vez de `m15B0`, diferencia con nombre, escalar a la izquierda.
- GameEventScript (`maraton-20261107`): intercambio r0/rX en `ScriptResult_unk::SetNext_unk` de una docena de comandos (0x803A61CC, 61F4, 5F14, 5F88, 66B0, 7C28, 7CC0, 81F8, 83FC, 98C4, AB06C, AB144, AB1F4, 6324, 5A10). El mismo fuente casa en otros comandos (0x8039DA10); probado: `const`/no `const`, `mWait` antes/después, ctor de `ScriptResult_unk`, variable `wait`, inline con otro orden de parámetros, código muerto previo. Parece depender del contexto del fichero.
- Registros intercambiados en bucles con dos punteros/índices (AIManager::fn_80018078, BattleTask::fn_80103E60/80103484/80108A5C, TacView::fn_805ADDD4, GameEventScript::fn_803957F0/803A2DD8/80398530): órdenes de declaración probados sin éxito; quedan al 97-99,7%.
- GameCamera::fn_80392C94 (43%): el original carga todos los miembros antes del primer store en el struct de salida; ni el orden de las asignaciones ni un local con copia final lo reproducen (la función inversa fn_80392CE0 sí casa).
- `Event::DebugInfo_unk` ctor 0x80221E08 (90%, `maraton-20261107`): el original guarda -1 en r31, 0 en r30 y la tabla de columnas en r29 (nosotros al reves); probado array estatico/global, puntero local, array decaido.
- `GameCamera::fn_80389678` (`maraton-20261107`): MWCC inlinea `atn::RenderManager::SetCamera_unk` (copia de 0x1F4 bytes) donde el original llama a la copia debil, y reparte los floats en f9-f13; retirada.
- GameEventScript fn_803A61F4 (familia r0/rX, `maraton-20261107`): escribiendo el `SetNext_unk` a mano (`result.mWait = 0; result.mNext = (ScriptCmd_unk*)((const char*)cmd + lbl_8074ED24[cmd->mCmd]);`, sin la referencia `int& size`) los registros salen como el original y solo queda el `lis` de la tabla despues del `li 0` (4 lineas); probado tabla en local, puntero a la entrada, indice en local, `next` precalculado.
- (`s12h-20261107`) Siguen sin resolver tras ~10-15 variantes cada una: `GameTask::fn_80376254` (r4/r8 de la copia
  de `MotionBlurParam_unk`), `fn_80374710` (registros del bucle de `FindPlace_unk`, tambien cambiando la cabecera),
  `fn_8036CA60` (r3/r4 de `lbl_8087F418` y del mapa), `fn_80376324` (el original relee `mCount` de la cola),
  `CharaTask::fn_8016DAB0` (orden fb8/fbc y hueco del segundo `fctiwz` de MAX), `BattleTask::fn_80109828`
  (`bne` sobre `blr`; ni `if` anidados ni inline), `NewCampPartsOption::vf2C` (huecos de los `SoundRef` del
  modo 8: invertir el `if` acierta los huecos pero cambia el orden de los bloques), `Em308ParmCrab::fn_802FBA30`/
  InvertCrab (huecos de `dir` y del temporal de `ToRotation_unk`, 30 formas), `Event::EventCamera_unk::fn_80115704`
  (registro del segundo `fabs`), `ControlWallMountle::vf0C` (22 lineas con `rate`/`speed`/`st`),
  `fn_8017C9AC` de CharaTask (solo r6/r7 de la copia del segmento tras pasar el segmento y el empuje a inlines).
- (`s12h-20261107`) `TacView` (copias debiles `fn_805A96C0`, `fn_805ABD6C`...) y
  `GameTask::fn_80372574`: funciones de 8-124 bytes que son copias debiles detras de su primer usuario grande
  (`fn_80371790`, `vf10` de TacView...); no se pueden colocar sin escribir antes ese usuario.
- (`s12h-20261108`) Bucles por los slots de jugador de `MP::Manager2` (`atn/PacketPack`): el original da otros
  registros al indice/desplazamiento/contador (y al cero del bucle, `mr rX, rIndice`) en fn_804DF310, fn_804DBD08,
  fn_804FB220, fn_804DD1A8, fn_804F1F34 (incrementos contador/puntero invertidos); probados: indice `unsigned`,
  `while`, `continue`, puntero que avanza, inline `Clear_unk`, el buscador fn_804DD158 con locales de varios tipos.
- (`s12h-20261108`) `Event/EventParser_unk` ANID/EFID/CAID/POID/MVID: con los ctor `(owner, name, id)` de
  MotionData.h llegan al 99,5% con el mismo bloqueo de registros/pila que MOID; revertido.
- (`s12h-20261108`) `MP/func_enter_setting` vf0C/vf10 de `PacketPackMP` (0x8050105C/0x80501054): son copias debiles
  emitidas tras las de los `func_*`; definidas inline en la clase salen tras vf18 y la vtable al principio de `.data`;
  con el dtor declarado no se emite la vtable.
- (`s12h-20261109`) `fn_804DD158` (buscador de jugador de PacketPack) inlineado: mismos registros cambiados
  (aid/desplazamiento/contador) en ~10 funciones (EB4AC, FC0D4, D8610, EB010, DD3F0, EB2D8...); probados `aid`
  `unsigned int`/`unsigned char`/`int`, firma `unsigned char`, como miembro, con `PPPlayerArray_unk&`, resultado en
  variable y envoltorios inline. Solo fn_804E8A80 sale con el `unsigned int` local.
- (`s12h-20261109`) `GameEventScript`: los 62 comandos sin escribir caen en el r0/r4 (tipo/cero) de `SetNext_unk`;
  probados SetNext explicito, `int wait`, `int& size`.
- (`s12h-20261109`) `RecycleBuffer::Alloc_unk` inline (RecycleBuffer_gmk/ItemSlotAnime, 5 funciones): el original da
  el registro mas alto al indice del byte `i >> 3`; ~20 formas (orden de operandos, `/ 8`, `int i`, while, TrySet/
  IsUse/SetUse, puntero al byte) sin cambio.
- (`s12h-20261109`) `ReplayRecorder_unk::fn_8056BD34` y `ReplayBuffer::fn_8056FC24`: con el medio float y los
  indices ya iguales queda el marco de pila 0x20 menor (huecos de los temporales); probado el orden de las
  declaraciones.
- (`s12h-20261109`) Quitar o anadir declaraciones en `game/ReplayBuffer.h` o `game/Bullet.h` renumera los `@N` de
  GameTask: `fn_8036D2D0` (99,98%) necesita renombrar a mano su etiqueta (linea `0x808857A8` de `symbols.txt`) tras
  `pd`, que solo empareja las de funciones al 100%.
- (`arranque-20261109`) Par contador/desplazamiento de un bucle con los registros cambiados (EnvSeManager fn_8035F33C,
  GameEventScript fn_803A2DD8, MemberList fn_80217D9C, TextureCacheManager): el orden de declaracion no lo cambia
  (el desplazamiento lo crea MWCC); probados `unsigned`/`int`, cast, `++i`, puntero al elemento, `break`.
- (`arranque-20261109`) `atn::Lerp_unk(Vector3)` de math/Vec.h: `(b - a) * t + a` da el 100 % pero renumera las
  etiquetas de 13 unidades y baja TutorialTask vf10 y CharaFaceControl; no repetir sin arreglar esas dos.
- (`arranque-20261109`) SoundManager fn_800CB800/BE8C/C518: una inline libre con el indice primero arregla los
  registros de la segunda inline, pero mueve el `@LOCAL@...stored_vtable` de boost::function (.bss) y baja 9 funciones.
- (`arranque-20261109`) Saved FPR (QuadrupedTask vf24, CharaTask fn_8015EB2C) y PostFxRain vf10: `declperm.py`
  no cambia nada; el orden de declaracion solo vale para volatiles y algunos saved GPR.
- (`arranque-20261109`) Em308ParmCrab vf10 / Em401 fn_8031FE4C: `fn_800F8C6C` (124 llamadas, `extern "C"` en
  muchas unidades) tiene el mismo problema de orden de argumentos que fn_80170A20; no tocado.

- `noescritas-20261110`: GameEventScript `fn_8039CCD0` (recursiva) al 89,8 %: el original la inlinea una vez en si misma y en tres llamadoras, que heredan los registros; no se consigue con `inline`/`noinline` (sin pragmas).
- `noescritas-20261110`: GameCamera, modos escritos sin 100 % mientras falten los callees de `fn_80392964` (cabe en el presupuesto R3 e inlinea lo que el original llama); no poner `noinline`, escribir los callees.
- `noescritas-20261110`: D2AMpMenu `fn_804B3494` lee `D2ACommonDialog::sInstance->m4C` al final sin usarlo; un `if` vacio desaparece. Probable inline con una rama vacia (assert/debug); sin resolver.
- `noescritas-20261110`: `lbl_8087F1E4` y `lbl_8087EEE0` tienen tipos distintos en varias unidades: no se pueden poner en una cabecera comun incluida por todas sin unificar antes esos tipos.
- `noescritas2-20261110`: seis manejadores de GameEventScript (fn_803A2600, 2E20, 36D8, 49F4, AB1B0, AB26C) toman `ScriptCmd_unk*` no const (truco de registros); con `const` bajan del 100 %, y en la tabla de punteros a miembro de vf10 el cast hace que la tabla se inicialice en tiempo de ejecucion (vf10 89 %). Sin resolver.
- `noescritas2-20261110`: `push_back` de `std::vector<std::string>` inlinado (PrintLog_unk, fn_803AB96C, fn_8039BF04): el original calcula `(begin + size) - begin` sin simplificar; nuestra cabecera lo pliega. No tocar `vector_string.h` sin comprobar MotionCommand/MapLift.
- `noescritas2-20261110`: GameEventScript direcciona sus tablas de `.rodata` como desplazamientos desde el inicio de la seccion (lbl_8074E440); hasta escribir todo el `.rodata` de la unidad en orden los inmediatos no coinciden. "NONE" (0x8074E440) va antes que la primera tabla: orden de definicion sin deducir.
- `fueraruta-20261112`: los llamadores de la vibracion de `GameCamera` se quedan en 79-99 % porque `fn_8008B964` (getter de `RenderManager::instance_`) es `extern "C"` en `atn/Model.cpp` y el original la inlinea; hace falta declararla inline en la cabecera de `RenderManager` sin romper `atn/Model` (enlazada).
- `fueraruta-20261112`: en `CosEditProtector` las entradas de armas y protectores parecen tipos distintos, pero darle ctor a la de protectores saca `fn_801DD26C` del presupuesto R3 (81 -> 0 %) y pierde `Inventory::Order`: no repetir sin otra pista.
- `paralelo-20261112`: `fn_8008B964` (getter de `RenderManager::instance_`) como inline en `RenderManager.h` desplaza los `@N` de 117 unidades; se dejo `inline` local en `GameCamera_unk.cpp`. Igual con `fn_8038E23C` en `GameCamera.h` (144 unidades) y los metodos de `GameUI.h` (35): hacerlo en un lote de integracion que re-empareje todas las etiquetas a la vez.
- `paralelo-20261112`: `MP/D2AMpMenuLobby`: cruce de registros en la busqueda de miembro (`FindMember_unk`) en vf0C, fn_804BE610, fn_804BEDB8, fn_804C0A40, fn_804C23E4, fn_804C2C14 (4 variantes sin mejora).
- `paralelo-20261112`: `NewCampTask` fn_8052CA6C/fn_8052E1A0/fn_8052FA44: el `atn::Scene::operator=` implicito del original llama fuera de linea a las copias de Texture/GodRay/HazeParam_unk/ColorMatrixParam_unk; aqui se inlinean mientras los `operator=` de `vector<Light>`, `vector<Camera>` y `CharaLightData_unk` solo esten declarados.
- `paralelo-20261112`: `atn/EffectManager` fn_8023BA10 es un `operator<<` de `ostream` instanciado en la unidad: necesita `num_put`, `numpunct` y `basic_ostream` de MSL en `libs/`.
- `paralelo-20261113`: `fn_8008B964` resuelto: inline en `RenderManager.h` con re-emparejado en lote; atn/Model sigue emitiendo la copia debil (tras `fn_8008B140`, que ahora usa el getter). `MP/func_enter_setting` resuelto (enlaza): base `atn::SingletonNoStatic<PacketPackMP>` y orden de vtables/RTTI por cabeceras (`include/MP/PacketPackMP.h`, `include/atn/net/FuncMessage.h`). `atn/EffectManager` fn_8023BA10 resuelto en parte: MSL en `include/std/locale_facets_unk.h` (quedan `do_put(long double)` y los ayudantes de floats).
- `paralelo-20261113`: `push_back` de `std/vector.h`: el original usa una inline pequena con dos ayudantes (construir y reallocar) y toma el camino POD aun con ctor (decide por copia trivial, no por `__builtin_is_pod`). Bloquea las 19 de Event/Sequence, 3 de EventParser_unk, 3 de Event/Manager y el ctor de `CosEditItem_unk` (0x801D8C90); con la copia de `Actor` hace falta ademas la copia profunda de `atn::SimpleVector` (`atn/Array_unk.h`, 352 unidades). Cambio de cabeceras de todo el proyecto: decision del usuario.
- `paralelo-20261113`: `CharaTask::vfBC` con argumento (LastCocoonImago fn_80459FB8 al 100 %) renumera las 217 unidades que incluyen `CharaTask.h` (lost en chr/ControlAppear): solo en un lote propio de re-emparejado.
- `paralelo-20261113`: tipos reales de `Event/Actor.h` (`ActorAnim_unk::m00` = `SimpleVector<PoseKey_unk>`, `m10/m1C` = `Vector3`, sin `ActorKey2C_unk`) renumeran ~30 unidades: lote propio con `pdbatch.sh`; hoy casts en Puppeteer_CharaTask.
- `paralelo-20261113`: atn/LayerOcclusionCPU (3 al 0 %): `fn_800B4390` es asm a mano y las otras dos escriben GQR6 con `mtspr`: sin asm aprobado no se pueden escribir.
- `paralelo-20261113`: LastCocoonEgg (6 copias weak de vf0C/vf10), ctor de LastCocoonImago, ctor de Em206CocoonMp y `ResHandle::ResHandle`, `fn_802A3D80` de Em203GreaterDaemonRedMp, `vector<int>::operator[]` de Em203GreaterDaemon: el original queda por encima del presupuesto de inline y el nuestro no (o al reves); `__declspec(noinline)` empeora.
- `paralelo-20261113`: pools de cadenas con `""` sin referencias en el original: Event/DebugInfo_unk (detras de "vector length error") y GameEventScript (7, detras de formatos de `sprintf` de depuracion); ni parametro por defecto `""` ni `strcat(buf, "")` los producen.
- `paralelo-20261113`: deuda de la regla 10 que crece por llamadores de otras colas: funciones `extern "C"` con el objeto delante en BattleTask (~20), AiTarget/ItemManager (fn_80018C80, fn_80019254, fn_800197A4, fn_80019E88...), atn/Model (fn_800991D0, fn_8008E818), GameCamera (`fn_805AA94C(TacView*, ...)`), AI_archer `fn_8003E6B8`; vistas locales de `lbl_8087F430` (Quark, Imago, GameCamera). Lote propio que cambie todos los llamadores a la vez.
- `port-decomp-20261114`: `push_back` de `std/vector.h` con la forma del original (inline pequena + construir + reallocar): nuestro MWCC mete los dos ayudantes dentro de `push_back<EffectEntry24_unk>` y el original no; harian falta 300-600 asserts vacios. Probados accesores delante, sin rama POD, `inline` explicito, orden de definicion e instanciacion explicita (qL). Bloquea 26 copias debiles de Event/Sequence, EventParser_unk y Event/Manager.
- `port-decomp-20261114`: presupuesto de inlining en `GameCamera::fn_80384188` (~32 %: muy por encima; el original lo inlinea todo), `MiniMapTask::vf10(RenderManager*)` (le faltan ~36-40 sentencias para pasar el presupuesto como el original) y el ctor de LastCocoonImago (le faltan 3-4 unidades de coste). Ver qC, qO, qM.
- `port-decomp-20261114`: `cntlzw` del `empty()` de `boost::function` en BoneCalc_unk `fn_8009ADA4`/`fn_8009A5CC`: sale `cmpwi` con todas las formas probadas (qD, qJ).
- `port-decomp-20261114`: bucles int->float en AIManager (`fn_80017128`, `fn_8001794C`): nuestro MWCC saca fuera del bucle un valor que el original reescribe por elemento (desenrollado 6/4 frente a 8) (qK).
- `port-decomp-20261114`: `atn/LayerOcclusionCPU fn_800B47AC` (77,7 %): el original pone GQR6 con `mtspr` y carga s16 con `psq_l` (asm no aprobado); escrita en C con escala equivalente. `fn_800B5994`: codigo identico, sus limites a cero van a `.sbss` y en el original a `.sdata`.
- `port-decomp-20261114`: el `""` de la pool de atn/Model detras de "NODE_EDIT" lo crea algo compilado entre `fn_8008A00C` y `fn_8008AD4C` que no queda en `.text`; con `static`, `static inline`, `inline` o `std::string("")` no sale (qJ).

## cabeceras-20261114

- **Forma original de `push_back` (`std/vector.h`)**: rapido + `slow` (`expand_by(1)` / `realloc`) sube 45 funciones y
  5 al 100 %, pero deja 8 bajadas de registros (puntero al buffer de realloc en un registro mas alto). Probadas ~60
  variantes en las colas A2-A5 (`docs/cabeceras-20261114-qA2..qA5.md`); mejor parche `docs/cabeceras-20261114-qA5.patch`.
  Pendiente de decision del usuario.
- **Copias de `atn::Scene`/`atn::Camera` en NewCampTask** (`fn_8052E1A0`, `fn_8052FA44`): el original llama a los
  `operator=` de `vector<atn::Light>`/`vector<atn::Camera>` (0x8052E980, 0x8052EEBC) y nosotros lo inlineamos todo;
  con `Frustum_unk::mBounds` (cola M, rama `cabeceras-20261114-m`) bajan 0,5/0,1. Necesita esos `operator=` en vector.h.
- **`PathLengths_unk` en WindyItem/PathData_unk/ActionActorPosition**: 0x8000D844/0x8032B6F8 son el dtor/ctor de
  `vector<float>`, no de una clase propia; con locales del tipo se inlinean y bajan mucho (qG, qJ).
- **`LevelUpResult_unk` != `CharaStatusData_unk`**: misma forma, distinta copia (campo a campo frente a bloque); no
  unificar (qI).
- **`BattleCharaTask::fn_800E9110`** (98,8 %): el original deja `&mStatus.m2CC` (0xAA0) en un registro; 15 variantes
  pliegan el offset (qC, qI).
- **`@wstringBase0` de BattleTask**: un `L""` sin referencias y un segundo `L"%d"` sin fusionar (qK).
- **`.data` de BattleTask** 0x174 frente a 0x148: RTTI de Task/TaskBase y `@STRING@` debiles, como en unidades enlazadas (qK).

## ceros-20261115

- `static Matrix3x4 fn_8013A13C(float)` por valor (en vez del miembro): MWCC la inlinea en 17 unidades (ControlAttackBase, ControlDamage, PBoneBasic, Sound3dListener, varios Em*) donde el original la llama: 32 funciones perdidas y `GameCamera::fn_80382A1C` sigue al 99,92 (parche `docs/ceros-20261115-f0c.patch`). El bloqueo de GameCamera (`fn_80382A1C`, `fn_80387910`) es la llamada al ctor vacio de `Matrix3x4` sobre el temporal.
- `atn/SmallAllocator_8` (2 al 0 %): el original inlinea el ctor del gestor de memoria en todas salvo `operator delete` y los destructores; cinco colocaciones del ctor cortan el ciclo ctor -> dtor -> delete -> Instance en otro sitio (cola B).
- `atn/WordWrap_unk` `setstate`: escrita sube a 98,4 % pero el sentry de ostream baja de 35 a 1,1 % (cola B). El ctor copia y el thunk `@8@60@` vienen de vtables de iostream que nuestra WordWrap no emite.
- `Em206CocoonMp` (ctor y `ResHandle::ResHandle`) y `GimmickManager` (`LoadSeTableTask::vf0C`, `fn_803EF3B0`): presupuesto de inline con todas las inline candidatas en cabeceras compartidas (probablemente asserts en el ctor de `atn::Csv` del original) (cola L).
- `Em203GreaterDaemonRedMp::fn_802A3D80` 99,97 %: llama a 0x80244674 (`__dt__18NecroPlaceList_unkFv`), casi seguro el `~vector<EnemyTask*>` del MSL original (0x7C; el nuestro mide 0x78). Mismo caso en `Em206CocoonMp::vf104` y `GameTask::fn_803729B4` (parche `docs/ceros-20261115-qL-redmp.patch`).
- `AI_archer_attack01::fn_8001C088`: MWCC desenrolla las copias en todas las variantes probadas (colas E y L).
- TacView `fn_805AB828`: llama a copias debiles de `size()` que el original comparte entre tipos distintos (probablemente una base no plantilla de `SimpleVector`/`FixedArray_unk`) (cola I).
- `vector.h`: el `resize` con un nivel de inline mas (cola J, `docs/ceros-20261115-qJ.patch`) sube EffectSystem `fn_8022BEAC` a 100 pero baja ActionActorPosition `vf0C`, PathData `fn_8054875C`, D2Anime `fn_801FFA04` y EffectSystem `fn_8022B850`.

## enlazar-20261115

- `chr/ControlAppear`: el `.rodata` tiene `value<MOTION_ID::@enum$24281control_appear_cpp>`: fichero original `control_appear.cpp` y contador de MWCC en 24281 al leer `MOTION_ID` (el nuestro, ~7508). Solo se reproduce ajustando `#include`; se rompe con cualquier cambio de cabeceras. Decision del usuario: dejarla para el paso final de la decompilacion.
- `msc/src/RVL_SDK/kpad/KPAD`: enlaza anadiendo `KPADGetMplsZeroDriftMode` (funcion real del SDK que el enlazador quita) con el cuerpo de `robojumper/sdk_2009-12-11`, que no tiene licencia: aplicado con permiso del usuario en `kpad-readme-20261115` (agradecimiento en README.md).
- `smg2/src/RVL_SDK/bte/bd`: `.sbss2` total 0xE frente a 0x10; `bd_addr_null` es lo ultimo de `.sbss2` y el original conserva 2 bytes tras el (como ut_TextWriterBase). Cinco compiladores dan 6 bytes.
- `Event/Label` (y Channel, Cut, Sequence) `__sinit`: las unicas funciones del DOL con un store entre las restauraciones del epilogo; ~15 ficheros sueltos sin reproducirlo.
- `chr/ControlDamage` `ControlDamageLastOne::vf0C` 99,64 % (`docs/enlazar-20261115-qY.patch`): `model` en r30 y el bool del helper en r31, al reves que el original; ~20 variantes.
- `GameSaveBanner fn_803B9638` 99,18 %: argumentos de `NANDInitBanner`/`memcpy` del ultimo al primero; ~20 variantes.
- `NETAES` (62 %), `ipcclt IpcReplyHandler`, `dwc_transport`, `dwc_account` (`DWC_ReportFriendData`: compara 2,3,1 con cuerpos 3,2,1), WUD `SyncHandler`/`fn_80670C70`: sin cambios.

## cierre99-20261115

- `dwc_account` `DWC_ReportFriendData`: compara los casos 2,3,1 y coloca los cuerpos 3,2,1; ningun MWCC de Wii (1.0-1.7) lo da con un `switch` normal. Resuelto con `case FRIEND_KEY: goto` al principio (aprobado por el usuario 2026-11-16, excepcion de la regla 2). Mario Strikers Charged (CC0, GC/3.0a5.2) usa una cadena if/else: aqui no reproduce el bloque de comparaciones.
- qA: ControlSitting::vf0C: this/motion r31/r30 y `dir` bajo el resultado del lerp; `GetModel_unk()` 98,10, `IsPlayer()` peor.
- qA: ControlDuckUnderB ctor (91,9): la direccion frontal queda bajo las dos copias por valor de fn_80050A1C en el original; escrita sin la inline GetFrontDir_unk sigue igual.
- qA: ControlGrappleThrown::vf0C (99,82): copia `pos = result.m10` (r3/r4); probados accesor de posicion, puntero local, `pos(...)`, declaracion previa, `chara` en los argumentos.
- qA: ControlChaseCatch::vf0C (98,1): la Mtx de FrontDir_unk y `scale` comparten hueco por encima de `m` en el original; probados `scale` arriba, `m` arriba, bloque para `m`.
- qA2: ControlCharge ctor: flags r4/r5 de IsCursorPlayer_unk/IsCursorMode_unk (ver NONMATCHING).
- qA2: ControlGrappleThrown::vf0C: copia `pos = result.m10` con dst/src en r4/r3; resiste VecAssign_unk y todas las ordenaciones de `c`/`pos`.
- qB: Orden de la pila de un temporal resultado (`mtx` de Leaf vf10, `d`/producto de Cannonball vf40 antes de la inline): con locales con nombre el primero declarado va arriba; en el original a veces es al reves. Una inline intermedia lo invirtio en Cannonball, no en Leaf.
- qB: gmk/MapLift ctor y atn::Model::Model: `__construct_array` recibe el ctor de `LH_Motion` y el dtor de `MotionSlot_unk`; ninguna forma de `MotionSlot_unk` probada con try.sh lo reproduce.
- qB: gmk/RandomItem vf14: el original iza la fuente de la copia de matriz antes que el destino; con `__MtxCopy(dst, src)` no sale.
- qC: Em315Coffin ctor: `sZero_unk` (0.0f) en `.sdata` del original; sin `explicit_zero_data` va a `.sbss`.
- qC: Em303KrakenFoot/KrakenFootMp `vf0C`: orden de carga de `m54C |= 0x200` y `mStatus.mParamB.m9C |= 0x2A` (el original carga primero el segundo en r3 y guarda primero el primero); ninguna forma lo da.
- qC: Em202SpiderBase `fn_802878EC`, Em206EggMonster `fn_802C38E4`: temporales de direccion/rotacion en orden inverso al original aun con inlines (ver NONMATCHING).
- qC2: Em315Coffin ctor: `sZero_unk` en `.sdata`; ninguna expresion de 0.0f (salvo `-0.0f + 0.0f`, que vale -0) evita `.sbss` sin `explicit_zero_data`.
- qC2: Em001Necromancer ctor (cadena de `m18` y dos `clear()`), `vf14` (r28/r29 texto/layout, como Em208Queen).
- qD: Ver NONMATCHING arriba. Ademas, sin cambio (nada commiteado): ChaseManager fn_80183948 (permutaciones de fn_80182874 rompen las otras dos), TeamAIManager OnMembers_unk (Find_unk: helpers, orden, split), PlayerManager fn_80549F14, EnvSeManager fn_8035EA2C (relectura de mBgmState: getters, bool, switch), GameSaveData_unk fn_803BE6B8 (puntero a la entrada en FindGimmick_unk 97,3 % pero base distinta), DamageCalc_unk fn_8003EA3C (orden de argumentos de fn_80040B44: locales, inline), GameTask fn_8036CC90 (puntero/referencia a dof), ItemManager_unk fn_80448B80 (operandos de cmpw), CosEditBase fn_801CF3C0 (`part` fusionado con `add`), NewTitleTask dtor, LastCocoonQuadruped fn_80468534 (99,92 con VecAssign, solo fuzzy), AI_archer_attack01 (cuatro vf con dos `lis` cruzados: cadena como `lbl_807307A0 + off`), GameChoiceWindow/MapPartsBase/ NewCampTask/CampSaveLoad/NewCampPartsMapSelect fn_80520634 (pool de cadenas incompleto).
- qD2: Ver NONMATCHING arriba. Ademas, sin probar a fondo por ser cruces de registros ya documentados: CrossbowMenuTask fn_801E9544, ArenaTask vf10, ChaseManager, CharaC58_unk fn_8011CAE4, AreaEffConfig fn_80043014, GameEventScript (familia SetNext_unk), NewCampPartsStatus fn_80528310/454/574 (base + 0x58 por llamada), AI_archer_attack01 vf24 (pool de cadenas sin reconstruir), CharaTask fn_8014FF9C (doble `beq`), CrowdManager fn_801EB7C4.
- qD2: `SkillEffectManager::fn_8059D3B0` 99,94 %: el bloque repetido en las dos ramas como inline miembro (`Attach_unk`) da 99,92 % (huecos distintos); no es la forma original.
- qD2: `EnvSeLine::vf14` 99,93 %: los temporales quedan 4 bytes mas abajo; color/punto medio/suma en local, orden y forma de `pa`/`pb` (asignacion, copia, `VecAssign_unk`): igual o peor.
- qE: `Loader::Get_unk<T>` (LH_*): sin cambios, mismos diffs que qC/qQ (no repetido).
- qE: NumberFormat_unk fn_800DD3FC, VideoTHP vf10, TextureCacheManager8x32 ctors, LipSync_unk: reparto de registros (webs de variables / IV del bucle); 2-3 variantes cada uno sin efecto.
- qE: D3Anime Draw / D3AnimeInstance Draw (99,29): volatiles; camView movido detras de iw/ih (igual o peor).
- qE: Scene fn_800C258C: operador `*` de Vector2_unk (`m04 - mSize * 0.5f`, constante en f1), Mul2 con resultado con nombre, division entre 2, min antes que p: ninguna mejor que la actual.
- qE: CsvBase_char_124_13_34 fn_802078C4: levels/i declarados arriba en 6 ordenes, count reutilizado como indice: igual o peor.
- qE: FileList fn_8006AC08/B2D8 (rfind de std/string.h): orden de declaracion de p/sz en rfind sin efecto.
- qE: atn_colli ColliObject::fn_800588FC (orden de stores de la escala: sx con nombre, Set_unk, tres llamadas en el ctor) y ColliTree::fn_80059264 (orden de incrementos: tris[i] desde node, `tris++` en el for, `*tris++`, unsigned): sin cambio.
- qE: D2A_Data ~D2A_Data: accesores locales del buffer, `int i`: sin cambio (patron contador/desplazamiento).
- qE: LH_Handle fn_80473FCC: i y callbacks declarados al principio de la funcion: sin cambio (r24-r31 los ocupan las constantes del camino de bad_function_call).
- qE: LipSync_unk fn_8007A7BC: cur/m2C f2/f3; probados delta antes de cur (cambia a f3/f4), cur arriba, sin delta, sin cur, step con nombre, cur al final de la suma.
- qE: Loader fn_8046ECDC: layout del "dotpos" en r29 (r28 en el original, ambos libres); FreezeTask/MeltTask en vf10 sin efecto.
- qE: PostFxRain vf10: `this`/0xFFFFFF frente a fullW/fullH (bloques de registros cruzados); 0xFFFFFF en un local: igual.
- qE: PostFxMotionBlur vf10: puntero local a los parametros: peor.
- qE: NetFriend_unk fn_8050F2F0/F470/F734: desplazamientos en `@stringBase0` (faltan cadenas de funciones sin escribir).
- qE: ColliCalc_unk fn_8005AADC/AD7C: orden de los `lfs` de radio/z y max.z/end.z; probados orden de parametros, esfera/capsula por puntero, locales hi/lo y max local (todas peor o igual).
- qE2: EffectRes_unk fn_80238588 (99,73): `zero` (vector muerto) se escalariza; con un puntero local `z = &zero` queda en bloque (99,77) pero no detras de `mtx` compartiendo hueco con el temporal de la conversion como el original; 6 ordenes de declaracion, bloque, array, ctor: igual o peor.
- qE2: Sort_unk de Loader (99,89): comparador con `const&`, `>` invertido, `*++i`: igual o peor.
- qE2: PostFxMotionBlur vf10: el tipo real de lbl_8087EFA8 es `RenderSetting_unk` (m240 = `MotionBlurParam_unk`); sin probar con el (la cola E probo el puntero local: peor).
- qE2: VideoTHP vf10: `GraphicsDevice_unk` real en vez de la vista `RenderState_unk` (regla 10) da el mismo 99,28 %.
- qE2: FileList fn_8006AC08: rfind de std/string.h con `sz` antes de `p` o `const T&`: igual o peor.
- qE2: THPPlayer_unk fn_805B6120: `s32 i`, orden de declaraciones: igual o peor. LipSync fn_8007A7BC: inlines min/max: peor.
- qE2: D2A_Frame-truco (`const float&`) en Light_unk ToU8_unk: sin efecto.
- qE2: Texture_unk fn_800D5A3C: con el cuerpo de liberacion escrito en fn_800D5B58 (en vez de llamar a fn_800D58A4) sube a 99,83 % (solo queda el orden de carga ancho/alto); 12 variantes de locales w/h (orden de declaracion, declarar y asignar, referencia al gestor, accesores, struct): 99,37-99,58. Sin commit.
- qE2: CompositeCache_unk fn_800D8458: postfijo, sin `entry`, `end` antes, `!(==)`, `!mRefCount`, lista por referencia: invariante (99,04).
- qE2: LH_Effect vf0C (99,89): `dir + "/" + name` con el `operator+` de std/string.h quita las 6 diferencias de nombre (`fn_8006D008` es `std::operator+`) pero no cambia el %; queda r19/r20 de path/strlen (11 variantes). Parche: `docs/cierre99-20261115-qE2-lheffect.patch`.
- qE2: D3Anime vf14 (99,80): frustum declarado antes, llamada directa, IsVisible_unk/CalcMtx_unk de D3AnimeInstance: igual o peor.
- qE2: TextureCacheManager8x32 ctors: helper inline, while, punteros locales, i arriba: igual (contador/desplazamiento).
- qE2: PPDATAU fn_8050DC58: `end()` en la condicion, mensaje en local: peor.
- qE2: DrawHelper fn_800602E8 (99,81): el original copia el 0 de `valid` a `ready` (`mr r21, r22`); 7 variantes de las inlines DrawTexValid/DrawTexReady y del ternario: igual o peor (las inlines las usan 13 funciones).
- qE2: EffectDesc_unk fn_80236B84 (99,79): el original hace `frsp` al guardar el minimo de profundidad; un min con parametros double da el frsp pero con rama else (99,28); UpdateMin con double, cast, local double: fmr.
- qE2: Model fn_80089D70 (99,89): AbsRef const&, IsZero_unk, __fabs directo: peor.
- qE2: LayerOcclusionCPU fn_800B64DC/65B4 (99,6): bias+dato, const, local, fila, puntero, inline: igual.
- qE2: atn_colli ColliTree::fn_80059264: sin local tris, n += 3 arriba, contador local, while: peor.
- qE2: TextAnimation fn_800DAB68: ademas del mullw, el destructor debil 0x8000D844 se llama `Event::PathLengths_unk::~PathLengths_unk` en symbols.txt (es `std::vector<float>::~vector`; lo usan 6 unidades).
- qE2: Light_unk fn_800795DC (96,52): color en local, `mRadius * 2.0f`, `const float&` en ToU8_unk: sin cambio.
- qF: Un 0 copiado de otro registro (`li rX,0; mr rY,rX`) en vez de `li rY,0` en el original: ParseUtil_unk (4 funciones), ActionActorBehavior fn_8054FBE0. Sin forma conocida que lo reproduzca.
- qF: Pares texto/layout de los menus MP: SetTaskText_unk solo arregla FriendRoom vf0C; barrido automatico de cuatro envoltorios sin mas 100.
- qG: Pool `@wstringBase0` de PacketPack en `.sdata` (lbl_8087E1C4, 0x26C, cadenas de la pagina de depuracion).
- qG: Busqueda inline de jugador que devuelve `mData[i].mRef.mChara` (mensajes 5/7/8/58): el original carga con `lwzx base, offset`; probados puntero/indice, `(unsigned int)i`, inline que devuelve el jugador con ternario, retorno por indice.
- qG: Pendientes con trabajo previo: `std::vector<PPPlayer_unk>` (fn_804FC8CC/FF3D0/FC9C0/FCA34/FE2DC, desde fn_804D6688), `std::sort` de jugadores (fn_804E0694/09CC/0CE4/2840/291C) y de ranking (fn_804F62D8, fn_804F60E0), fn_804EDB0C (usa el literal y el estatico de la inline `__SetDebugKey_unk` de NetManager), fn_804F2040 (llama a un `vf114` que no esta declarado en CharaTask/BattleCharaTask/EnemyTask), fn_804FAF48 (arbol de codigos de error DWC), la vtable de `MP::Chara<EnemyTask>` (sale con el ctor, inlineado en fn_804DE884).
- qG2: `""` muerto en `@stringBase0` (PacketPack 0x150 y 0x18D): probados local sin usar, `if (0)`, inline con parametro sin usar, ternario `i ? "" : ""`, objeto local con ctor, `char b[8] = ""`, `strcpy/strcat/sprintf` con `""` (estos lo referencian); solo `strlen("")` plegado lo deja (ver Hallazgos).
- qG3: `""` muertos de `@stringBase0` de PacketPack (0x150 y 0x18D): ver arriba; siguen bloqueando los offsets de fn_804EDB0C, fn_804DC450, fn_804DC684, fn_804DBE24, fn_804F2258, fn_804F2040 y fn_804EE040.
- qG3: Las inlines debiles que solo usa vf10 fuera de linea (fn_804D5F0C, GetProtector, fn_804D648C, SetClearColor) y las de `std::vector<PPPlayer_unk>` de fn_804D6688 (fn_804FC8CC/FC9C0/FF3D0) no se emiten hasta escribir sus llamadores.
- qH: `fn_805AC758` (TacView, 99,39) y `fn_805A96D4` (98,3): ~100 y ~190 instrucciones distintas, no abordadas.
- qI: `Loader::Get_unk<T>` (cierre99 qI): ~110 variantes mas sobre el parche de qC, siempre 4 instrucciones (fin de lista r7/`m48` r0); ver NONMATCHING arriba antes de repetir.
- qI: `ControlKnockdown::vf0C`/`ControlDamageLastOne::vf0C` (cierre99 qI): el bool del helper inline toma un registro guardado mas alto que en el original en las dos; ~25 formas del helper y del llamador sin cambio.
- qI: `RecycleBuffer::Alloc_unk` (cierre99 qI): con `b` local y cast quedan i/indice/byte rotados (r10/r4/r8 frente a r8/r10/r4).
- qJ: `__sinit` de las tablas `ParamDef_unk` (Label, Channel, Cut, Sequence): el `lwz r31` del epilogo sube por encima del ultimo store; no se reproduce con ninguna forma ni version de MWCC probada (~26 formas en total) (`cierre99-20261115` qJ).
- qJ: `push_back` de `vector<boost::shared_ptr<...>>` (Section/Cut/Channel/Sequence): con el parche qJ queda la coloracion de los bucles de destruccion del buffer (la misma que `Channel::~Channel`), el `delete` por `this` y el `return`; y falta un despacho que solo afecte a `shared_ptr` (`cierre99-20261115` qJ).
- qJ: Bucle muerto del destructor de `__vec_split_gbase_unk`: `~__vec_split_unk<Actor>` (0x80544840) quiere `n` antes que `p`; EventParser_unk y `push_back<BigMulueCapsule_unk>` lo quieren al reves (`cierre99-20261115`).
- qK: `lis r0,0x4330` + `stw` de la palabra alta adelantados al prologo (HBMRsoDraw): no se reproduce en ningun caso simple del banco (if/else, ramas, bucle, inlines, asm de copia). Funciones al 100 % con el patron: gmk/WaitObject vf10 (conversion repetida por `Min_unk(Max_unk(...))`), gmk/Banana vf40 (bucle con `rand`).
- qL: `LightList_unk::fn_8007A154` 99,63 % con inline de erase: registros del recuento `end - pos - 1`; parche en docs.
- qL: `PlayerManager::fn_80549F14` 99,66 %: id/head comparten r30 en el original; con la inline de creacion se cruzan player/head.
- qM: Precommit: `ordercheck:game/GameEventScript` (`__dt__15GameEventScriptFv` fuera de orden) ya falla sin estos cambios (comprobado compilando el arbol sin ellos); no es de esta cola.
- qM: atn/Model `ModelBase::fn_80091A90` (99,61): el original adelanta el `psq_st f6, 0x88` de la copia a `m60` antes de las cargas siguientes. Sin cambio (99,61) con `__MtxCopy(m60.m, m)`, puntero local `Mtx* dst`, inline `MtxAssign_unk`, `Matrix3x4(m)` (91,9), y reordenando o declarando antes `data`/`skin`/contador (10 variantes).

## mixto-20261116

- qA: Sin bloqueos nuevos; los de la cola siguen como en mwcc-blockers (SelectMember r30/r31, MapPartsRain mMax, CostumeEditChara `addi+lwzx`, GameSaveBanner, ColliAttrManager fn_80183FFC, push_back de U16Table/ShopColor, inline budget de LastCocoonChrysalis/GimmickManager).
- Em303KrakenFootMp vf104 99,96: speed/rate del bloque de andar del default (dos niveles de inline) con f28/f29 cambiados; ~40 variantes en qA, qE y qA2. (mixto-20261116 qA2)
- qA3: LastCocoonQuadruped: `"ef_eye\0"` en `@stringBase0` sin referencias en el .o original (necesitaria una funcion inventada).
- qA3: CharaC58_unk fn_8011CAE4 / ArenaTask fn_80043A24 / PlayerManager fn_80549F14 / GameSaveBanner fn_803B9638 / SelectMember ctor / PopupTask vf10: bucle o bloque en inline, getters y ordenes de declaracion probados sin 100 % (ver NONMATCHING).
- qA4: (ninguno nuevo)
- Camara del campo (lbl_8087F430 + 0x4D8, `Set_unk(4, param)` de los jefes: Em101LittleZesyaSecond fn_80261DC0, EnemyTrista fn_8035D294, EnemyQuarkStand): el original guarda `&field->m4D8` / `&field->m97C` en un registro y escribe relativo a el; nosotros plegamos el desplazamiento. Probado: puntero local antes o despues de la copia de los parametros, inline libre con el puntero, paso como parametro: igual. (mixto-20261116 qA5)
- Em209SmallGolem vf10 99,62: el bloque de lanzamiento (dos apariciones) en una inline `GolemThrow_unk(self)` da la distribucion relativa del original (pos/normalizado/temporal) pero en otro sitio (99,17-99,38, una o las dos apariciones). Em202SpiderBase fn_802878EC 99,97: el original pone `m1570[i]->mPos - mPos` encima de `d`; local `delta` con nombre, distancia en inline: 99,95. (mixto-20261116 qA5)
- CrowdManager fn_801EB7C4 98,25: el cero de `mChara = 0` compartido con el indice del bucle (i en el for, j, sin cast): peor (97,2). (mixto-20261116 qA5)
- Em309GhoulSimple vf108 esta en el borde del presupuesto de inline (R3): una inline o temporales con ctor mas sacan de linea todas las candidatas. (mixto-20261116 qA5)
- Nivel base de `std::vector<T*>` (parche `docs/mixto-20261116-qB.patch`): correcto en funciones (+3, lost 0, fuzzdrop 0 tras typedef de los envoltorios) pero labelcheck peor: game/CharaTask 376 `@N` sin nombre tras pdbatch (2 pasadas) + lblmap; Event/Sequence, Em206CocoonMp, EnemyQuarkSecond, GameTask, MapPartsBase 2-9. Necesita una sesion de re-emparejado completo (`pdbatch` de todo + lblmap/lblseq/valfix).
- qB: atn/Util_unk fn_800DC12C (97,73): el `b` a la instruccion siguiente al principio; probado ademas recursion de cola para los espacios, `for(;;)`, bloque, `do/while(0)` con `break`, `if (s != 0) {...}`: no sale.
- qB2: (resuelto para ControlSitting, ver Hallazgos) Orden de `.sdata2` distinto del primer uso: probado antes que no cambia con `* 0.125f`, `atn::Abs_unk`, `static const float`, inline antes de la funcion (FallingTower) ni `1.0f / 30.0f`; el codigo muerto dentro de la funcion tampoco sirve (MWCC no quita la llamada a atan2/fmod). Quedan sin probar con la funcion que quita el enlazador: FallingTower, Brazier, LH_ColliObject, BulletPoint, InteractionSpot, PassBoard, DropDown, PostFxDamage, ColliBaseFunc, DropItem, FlyObjManager (todas con otra funcion por debajo del 100 %).
- qB2: chr/ControlShoot ctor 99,90 (la unidad enlazaria): el cero de mFB0/mFB4 en r6; ver NONMATCHING.
- qB3: atn/AABBTree vf18: ni como metodo de Frustum_unk cambia el orden `mr r3`/`addi r4` de la primera prueba inline.
- qB3: Pendiente para revisar (regla 2): `src/atn/AABBTree.cpp` Node::fn_80045C88 tiene `((void)0);` (sentencia sin efecto) para limitar el inlining recursivo; no tocado (no es de esta pasada).
- AiCommon_unk no enlaza por el orden de `.sdata2`: el -1.0f de fn_8001B508/fn_8001B5B4 va en el original antes del 1.5f de fn_8001AEDC. Solo lo reproduce un local sin usar (o una asignacion muerta) con -1.0f en una funcion entre fn_8001AC54 y el case 0 de fn_8001AEDC; `static const float` (se pliega) e inline sin llamar no. (mixto-20261116 qC)
- Cockpit `fn_804B5B10` 96,68: el original relee `lbl_8087F610` al principio del cuerpo del bucle (CSE con la condicion en el nuestro); probado getter inline en el cuerpo y en la condicion, `session2` local, cuerpo en inline miembro (94,9). Y `IsOnScreen_unk` inline deja el bool en registro (if/return true, `int`: igual). (mixto-20261116 qC)
- Cockpit `fn_804B50C0`: tras arreglar el bucle, el case 3 (`Play_unk(m48, 0, -1.0f); Play_unk(m4C, 0, -1.0f);`) da al task r27 y el original r28 (el registro de los task del case 1). (mixto-20261116 qC)
- MapLink `fn_80216544/fn_80216700` 92,23: contador y puntero de datos de la busqueda de columna cruzados (r5/r6) y el `mr r7, r5` antes; ~15 formas del bucle. (mixto-20261116 qC)
- MP: layout del par texto/layout con registro distinto del original cuando el original lo comparte con el task de `Play_unk` (Result `fn_804C5E9C`: r30) o con el puntero de un bucle de otro case (FriendRoom `fn_804BA350`): parece orden de coloreado entre cases del `switch`. (mixto-20261116 qC2)
- AI_archer_attack01 vf24 (em_scout, em_type106, fr_runaway) y fr_warppointescape vf18: `lis` de la cadena antes que el del global en el original; ni literal ni inline con el texto como parametro lo cambian. (mixto-20261116 qC3)
- `push_back` inline de std/vector.h (size/cap en r4/r5 al reves) aparece en ChaserNpc ctor, Em202SpiderBase vf0C, Em308InvertCrab vf0C, D2AMpMenuFriend fn_804B7F88, NewCampPartsHint vf30, LibraryTask fn_80469D60, CharaFaceControl fn_8011EF14: bloqueo conocido, no tocado. (mixto-20261116 qD)
- `GameLoadTask::Option(const char*, int)` inlineado: origen/destino del memcpy con registros cruzados (GameOverTask, CampSaveLoad, NewTitleTask ctors). (mixto-20261116 qD2)
- qD3: LayerShadow fn_800B6C70 (99,88): el original reutiliza los huecos de conversion 0x40/0x38/0x30 en cada snap; las 8 formas probadas (inline por valor/referencia, `int step`, codigo en la funcion, ceil a int en inline) dejan los ultimos `(int)step` en huecos nuevos.
- DialogHead vf10/vf18 (bucle de equipo de CharaTask): el original da al equipo r3 y lo actualiza con `lwzu` al modelo; ninguna forma del bucle ni de la mascara lo reproduce (mixto-20261116 qD4 + qH).
- qE2: Sin bloqueos nuevos. Pendientes con pista: Em301LShade fn_802D57B8 (orden de carga de `13.0f * m540.x` tras arreglar la pila), Em204BigMulue vf10 (orden x/y de `GetNodePos_unk` en el bucle de esferas), BattleTask fn_800F8574/fn_800FC410 (huecos de pila de `rot`/`v` y de `n`).
- Orden de los temporales: el original pone el primer temporal de la funcion (FlyObj estado 3, AutomaticDoor `target - pos`) en el fondo de la pila y el resto en orden inverso de creacion; nosotros todos en orden inverso. Probado: inline antes/despues (definida tras la funcion), local con nombre dentro de la inline, bloque propio, referencia a la camara, `VecAssign_unk`, construccion por componentes. (mixto-20261116 qF)
- `fabs fA; frsp fB, fA` (registros distintos) del original frente a `fabs fB; frsp fB, fB` con `atn::Abs_unk`: solo en algunas apariciones de la misma funcion (Model fn_80089D70: la primera de nueve; Path fn_80115704: la segunda). `(float)__fabs(x)` en esa aparicion da el codigo identico en Model fn_80089D70 (solo renumera `@N`), no en Path (cruza los FPR). No commiteado por artificial. (mixto-20261116 qF)
- `std::vector<float>` de TextAnimation llama al dtor 0x8000D844 (`PathLengths_unk` en symbols) y al ctor de `vector<float>`: falta el nivel base de vector del parche de qB. (mixto-20261116 qF)
- PacketPack swap 0x804E0CE4 / seleccion 0x804E291C: puntero fuente copiado de la segunda parte del equipo (ver NONMATCHING; con el parche, layout de cuatro miembros, queda solo el orden mAid/bucle y registros). (mixto-20261116 qG)
- PacketPack ctor de PPPlayer_unk (con el parche): r5/r6 de los bucles de construccion del array de 32 slots; probados ctor del slot en la clase o fuera, lista de init o cuerpo, struct envoltorio con/sin ctor, array sin envolver (peor), parametros por defecto. (mixto-20261116 qG)
- Probados sin resultado: fn_804D8610 (aid `unsigned int`/`unsigned char`/`int`/directo), fn_804F4E40 (5 variantes del puntero de ids), fn_804DC858 (4 ordenes de declaracion), fn_804F1F34 (4 variantes del bucle), fn_804DBD08 llamando a fn_804D1F6C (inline). `SetClearColor` inline en `atn::RenderConfig` (derivada de RenderSetting_unk): el borrador de vf10 tambien la inlinea y no se emite. (mixto-20261116 qG)
- PacketPack vf10 0x804D25D0 (94,4 %): `mgr` en r24/r25 y los bucles desenrollados de `m9A74/m99F4/m9A14/m9A34` con direccion por indice (`stbx val, mgr, i+off`) sin plegar; ver NONMATCHING. El mismo patron (`stwx r5, lbl, i*4+off` con el global releido) aparece en fn_804D824C (sin escribir) con `fn_800F7F90()->m9A54[i] = -1`. (mixto-20261116 qG2)
- PacketPack fn_804FB674 (98,7 %): r5/r6 del cero y de la fila 0 en el fn_804D0164 inlineado. (mixto-20261116 qG2)
- PacketPack fn_804D824C/fn_804F47FC: payload de red empaquetado; sin `#pragma pack` (no aprobado) el bucle de palabras no sale. (mixto-20261116 qG3)
- PacketPack fn_804DD574: base de `.rodata` de la unidad; necesita toda la `.rodata` anterior (0x47C bytes de cadenas/tablas de funciones sin escribir). (mixto-20261116 qG3)
- PacketPack, busqueda inlineada fn_804DD158 con aid/desplazamiento cruzados (fn_804E448C, fn_804D8610, fn_804E6518, fn_804EB4AC, fn_804DD1A8, fn_804FC010...): probados en fn_804DD158 la comparacion al reves (baja), un resultado con `break` (baja mucho) y la busqueda como inline miembro de `PPPlayerArray_unk` (sin cambio en los llamadores). (mixto-20261116 qG4)
- PacketPack fn_804D6688 (resize de `std::vector<PPPlayer_unk>`): `PPPlayerArray_unk` deberia ser el `std::vector` real (`{data, size, capacity}`, `m5EC` es la capacidad), pero ~40 bucles comparan `int i < mNum` con signo y `mSize` es `unsigned int`: el cambio bajaria esas funciones. (mixto-20261116 qG4)
- PacketPack fn_804F2450 (receptor de posicion de un enemigo, mensaje con medios floats): tiene inlineado el cuerpo de fn_804EE4F0 (95,11 %, medio float con `rlwimi` en otro orden y la `z` con `frsp`); no puede llegar al 100 % antes. (mixto-20261116 qG4)
- PacketPack `.rodata`: nuestro orden (dobles, cadenas GROUP_* del ctor, tablas de etapas, tablas de fn_804ED768...) no se parece al original (tablas de fn_804ED768, dobles, cadenas de rutas y voces VO_, ..., `@11026`, tablas de etapas); completar los 0x47C bytes antes de fn_804DD574 exige reordenar toda la `.rodata`. (mixto-20261116 qG4)
- PacketPack `atn::DegToRad_unk`: vf10 la llama fuera de linea (como el original, `bl` a 0x801125F8), pero nuestra copia debil y su `0.017453292f` salen en esta unidad (el original no los tiene: declarada sin cuerpo en su cabecera). Los pi/2pi/-pi de `.sdata2` son del borrador fn_804E7AC0 (WrapAngle_unk inlineada). (mixto-20261116 qG4)
- PacketPack fn_804F50DC: `m2A50` tipado `PPTable_unk` en vez de `MpMessageTable_unk` (ver Decisiones); la inline sobre un cast no pliega el desplazamiento 0x2A50. (mixto-20261116 qG5)
- PacketPack fn_804FAD48: `beq` muerto tras `b` en la guarda `lbl_8087F628 == 0 || sInstance == 0` (tres formas probadas). (mixto-20261116 qG5)
- `Manager::vf10` 99,5: el original asigna `m2424`/`m242C` con `atn::Vec2_unk::operator=` (fuera de linea, 0x800F833C) y construye `stick` con el ctor de copia de `atn::Vector2_unk`, los dos desde `lbl_807C7028`: el mismo global con dos tipos (y la `Matrix3x4 rot` sin ctor). Cambiar los miembros a `Vec2_unk` en Manager.h (incluyendo game/GameConfig_unk.h) y `Scale_unk` devolviendo `Vec2_unk` arregla dos llamadas; queda el ctor de `stick`. (mixto-20261116 qH)
- `DialogHead vf10()`: `lwz m524; cmpwi` muerto antes de fn_80076FF8 (no sale sin sentencia sin efecto). (mixto-20261116 qH)
- Puppeteer `fn_80012294`: compares 7, 3, 1 con el cuerpo de 7/1 primero (ni orden de case ni cuerpos duplicados). (mixto-20261116 qH)
- `Manager fn_8048E9E0` 99,16 (registros del clamp de gamma): inlines min/max por valor/referencia y una `RaiseGamma_unk(rs, step, max)`: peor (86-95). (mixto-20261116 qH)
- `EventParser_unk ParseXXID` 99,92: los dos temporales `char` del push_back de std/vector.h (construccion frente a la copia de la reasignacion) al reves; `T data(...); push_back(data)` en `DataList_unk::Add_unk`: mismo codigo. (mixto-20261116 qH)
- `atn::Abs_unk` (math/Vec.h): la forma del original (`double` local) renumera todo el proyecto; ver NONMATCHING. (mixto-20261116 qH2)
- Store de un parametro `double` a un miembro float y uso del mismo valor como argumento float: el original no reutiliza el `frsp` (`stfs fD` directo + `frsp` aparte), nosotros si (Path SetFovy_unk, fn_801140A0). (mixto-20261116 qH2)
- Llamadas a copias debiles fusionadas entre tipos (`empty()` 0x803D6EDC desde Event::Manager::vf10 con un `Ring_unk<QueuedSequence_unk,8>`): requiere una base comun en game/CharaStatus.h. (mixto-20261116 qH2)
- KPAD `fn_806594D0`: el codigo sale con `w = v = m1.Z = fn_unk_identity_dir.Z;` y `w = v = m2.Z;`, pero el original consume 4 `@N` mas (0.383864f es `@2235`, nosotros `@2231`) sin el temporal en pila; falta una forma que de ambas cosas. (mixto-20261116 qP)
- Event/ActionActorPosition fn_80557F84 (99,39 con el parche): el original recorre los huesos desde `&model->m384` (FixedArray); el miembro FixedArray en Model_unk.h baja atn/Model 9 funciones. (mixto-20261116 qR)
- Orden de locales con nombre de dos `case` (OnOff vf0C): el original da mtx(0), mtx(4), rot(0), rot(4) (los dos `mtx` antes de los dos `rot`) y despues los locales de `__MakeRotation_unk`; ni bloques anidados, ni `Mtx`, ni inline (por valor o con `rot` dentro) lo reproducen. (mixto-20261116 qS)
- Color de un unico registro sin cambio de codigo (ControlShoot cero r4/r6, ControlSlip mChara r3/r5, AutomaticDoor r5/r6, BreakArmor frsp f3/f4, LandMine FPR): las formas naturales probadas no lo mueven. (mixto-20261116 qS)
