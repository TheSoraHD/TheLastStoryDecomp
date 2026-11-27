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

## escribir-20261116

- PacketPack fn_804EE040: pasar el tipo `(unsigned char)` a fn_804F1564/2040/2450 (o parametros `unsigned char`) deja el `clrlwi r4` del original pero rota la asignacion de registros salvados (98,87 %); probados `unsigned char`/`unsigned int`/`char` locales y declarados en el bucle (escribir-20261116 qG)
- PacketPack: prueba `my == server` materializada (cntlzw) en fn_804E9360/fn_804EF0F0; probados `IsServer_unk()`, `== 1`, fn_804EB1AC (escribir-20261116 qG)
- PacketPack fn_804E9C64: inlinea `push_back` de `std::vector<PPCharaEntry_unk>` en `mCharas` (vista `PPCharaArray_unk`): necesita el vector real (escribir-20261116 qG)
- PacketPack fn_804DE884 (parche): el ctor de `MP::Chara<EnemyTask>` inlineado hace emitir la vtable y con ella copias debiles de virtuales inline de CharaTask.h (`vfE0`, `vfA0`, `vf70`) cuyos floats (1.5708, -1.5708, 2.44346, 25) entran en la `.sdata2` de PacketPack (poolcheck nuevo); el original no los tiene: esas virtuales no serian inline en CharaTask.h. El parche aplica sobre 0f9e820 (incluye el ctor inline y `Create_unk`, `PPSlot_unk` movida, `fn_804C566C` declarada); tras aplicarlo, pd.sh y pdfix de 0x807596F0, 0x807598B0.. (35 VO_) y los `.bss` 0x807C8AE8/AF4/B00 (escribir-20261116 qG)
- PacketPack resize de `vector<PPPlayer_unk>` (0x804D6688): el original inlinea el slot nuevo (constructor + copia) y la reasignacion (solo llama a crecimiento, reserva, slots nuevos, traslado y dtor del buffer); el nuestro pasa del presupuesto y lo llama todo. Una inline aparte para la reasignacion la inlinea entera (con crecimiento/reserva/traslado dentro) y deja una funcion debil de mas; sin ella el bucle en sitio tambien sale fuera (escribir-20261116 qG2)
- PacketPack fn_804E9C64: la copia de la entrada en el push_back es en bloque para 0x34-0xAF en el original y miembro a miembro en fn_804E7434 (100 %); un struct anidado da 95,6 % en fn_804E9C64 pero rompe fn_804E7434 (escribir-20261116 qG2)
- PacketPack fn_804EE040: pasar `type` a fn_804EFF74 (como el original) rota registros (99,54 -> 99,21 %); con `(unsigned char)type` (el `clrlwi r4` del original) peor (escribir-20261116 qG3)
- PacketPack fn_804DB1C4: el original lee `mDCScore` dos veces (suma y valor anterior); leer la suma por `mDC.mChara` sube la funcion (98,87 %) pero baja fn_804E4EB0 (98,1 %) (escribir-20261116 qG3)
- PacketPack fn_804EB934 y fn_804F3EEC: `points >>= 1` de la constante 10 (`li 10; srawi`) que el original no pliega (escribir-20261116 qG3)
- PacketPack fn_804EE040 (99,21 %): con `(unsigned char)type` en las cuatro llamadas toda la estructura sale como el original (el `clrlwi r4` del switch, el cero compartido con `data`/`type`) y solo queda una permutacion de registros (aid r31 -> r26; 98,92 %). Probados: `unsigned char`/`unsigned int` para `type`, un local `unsigned char kind` para el switch, parametros `unsigned char` en los manejadores (97,5 %), los 24 ordenes de declaracion de type/header/ cursor/player (98,87-99,26 %, el mejor renumera `@N`), `if (player != 0) switch`, fn_804F4FC4 con `unsigned int` (escribir-20261116 qG4)
- PacketPack fn_804CF468 (99,91 %): el hueco compartido de los dos `fctiwz` no sale con una inline `PackValue_unk`, `(unsigned short)(int)` ni `id` declarado arriba (escribir-20261116 qG4)
- PacketPack fn_804EDCCC (99,86 %): r4/r0 del byte de user data y del modo: probados el local `unsigned int`, la expresion sin locales, un puntero `mgr`, inline bool (con `&&` y con `if`) (escribir-20261116 qG4)
- PacketPack fn_804EE4F0/fn_804F2450: orden de las dos direcciones de PSVECDotProduct (`PPDot_unk` por puntero, llamada directa, `dir(...)`, referencia `front`, comparacion al reves: igual o peor) (escribir-20261116 qG4)
- PacketPack fn_804F39D4/fn_804F3880 y PPFindTeam_unk (fn_804F2040, fn_804F2258): el manager en r5 y el desplazamiento en r3 del original; un local para el aid o el manager, y el getter fn_800F7F90: igual o peor (escribir-20261116 qG4)
- PacketPack fn_804EB26C: el original relee `mFlags` para el segundo `if (m28)` (el nuestro lo reutiliza); probado releer por el indice (escribir-20261116 qG4)
- PacketPack fn_804DC858/fn_804EDB0C: la lista de amigos (`+0x430`) un registro mas abajo que en el original; orden de declaraciones y contador `unsigned`: peor (escribir-20261116 qG4)
- PacketPack fn_804E4B44: `m08[i - 1]` sale reducido al puntero del bucle (el original `mulli`); con `(&m08[i])[-1]` igual (escribir-20261116 qG4)
- PacketPack: el original relee el tamano de `mPlayers` (o `lbl_8087F610`) dentro del cuerpo de los bucles que llaman a fn_804D5F0C/PPSlot_unk (fn_804DCA54, fn_804DE2F8, fn_804DF310, fn_804E5BC4...) y el nuestro reutiliza la carga de la condicion; probado el bucle de la cuenta por equipos en una inline con `int* counts`: igual (escribir-20261116 qG5)
- PacketPack fn_804F3258 (99,57 %): datos/cola en r7/r6; probados sin local de cola, getter fn_800F7F90, cola declarada antes, `id` local, una inline (cola, datos) y (datos, cola): igual o peor (escribir-20261116 qG5)
- PacketPack fn_804E6DD0 (99,85 %): `own`/`head` en r14/r15 cruzados; probados `head` declarado antes, `own` asignado despues y PPFindSlot_unk: igual (escribir-20261116 qG5)
- PacketPack fn_804EAA84 (99,91 %): `num` en el registro de `sel` (r27) y no en el de `index` (r23); probados `int num`, `num` declarado dentro del `if` y al principio de la funcion: igual o peor (escribir-20261116 qG5)
- PacketPack fn_804CF198 (99,37 %): `d` en f3 (registro de `frame`) y no en f1; probados `__fabs` sin `(float)`, local para el limite y para el valor absoluto, `speed * 4.0f`, `d` double, sin `d`, `+=`/`-=`, referencia al slot, `k = 255.0f`: igual o peor (escribir-20261116 qG5)
- PacketPack fn_804EFF74/fn_804F2994: el original relee `lbl_8087F610` antes de la segunda busqueda (atacante y objetivo); un local `mgr` para la segunda y una inline que lee el global no cambian nada (escribir-20261116 qG5)
- PacketPack fn_804CF468 (99,91 %): el hueco compartido de los dos `fctiwz`; probados `if/else` por rama (98,87 %, sale el hueco pero no el `clrlwi` comun), local `unsigned short`/`int` asignado en `if/else` o ternaria, `(int)`, ternaria con `0.0f` e inlines `PPPackF_unk(task, f)` (tres formas): igual o peor (escribir-20261116 qG6)
- PacketPack fn_804DE884 (99,99 %): el original relee `player->mRef.mChara` (r29) para mFlagC25 tras IsTeamPlayer_unk; probados `player->`, `players[i]`, `mgr->mPlayers[i]`, una inline `PPBattleZ(player)` y IsTeamPlayer sobre `player`/`players[i]`, y la prueba de nulo por `players[i]`, `mgr->mPlayers[i]` o un local `chara`: igual o peor (escribir-20261116 qG6)
- PacketPack fn_804F3BAC y fn_804F1564: el original relee `lbl_8087F610` en el cuerpo y en la condicion; probados getter/global/local `mgr` cruzados entre condicion y cuerpo y un local dentro del cuerpo: igual o peor (fn_804F1564 99,30 % con condicion getter y cuerpo `mgr`, pero cruza r6/r7 y r28/r31) (escribir-20261116 qG6)
- La referencia local `players` en el resto de bucles con `mgr` (fn_804DF11C, fn_804DF4B8, fn_804E4EB0, fn_804E5BC4 y dos de fn_804DE884) baja o no cambia (escribir-20261116 qG6)
- Hacer fn_804D5F0C metodo, `const PPManager2_unk*`, un puntero local al vector dentro de la inline o una inline de `size()` en la condicion: igual o peor en fn_804DCA54 (escribir-20261116 qG6)
- Em203GreaterDaemonRed fn_8029795C 91,35: `GetMotionFrame(0)` en `m14C4 == 6 || frame >= fn_80097D7C(0)` pone esa carga igual que el original (f28) pero baja el total a 91,07 (registros de otro tramo): no commiteado. (escribir-20261116 qN)
- MapPartsLocatorSet vf24 87,9: relectura de `m17C` tras copiar mMin (ver Hallazgos); variantes 85,2-87,3. (escribir-20261116 qN)
- DropItem vf10 94,07: el original construye `m08 + m20` (fin del segmento) y lo copia antes de releer m08/m20 para el inicio; ctor por valor o ColliCapsuleShape_unk: igual. (escribir-20261116 qN)
- GameOverTask vf0C 96,23: el original guarda `ColorMatrixParam_unk::mEnabled` del temporal al final (tras las filas) y el ultimo cero de `BloomParam_unk::mScale` antes de copiar el bloom. `mEnabled = 0` al final del cuerpo del ctor de ColorMatrixParam_unk da 98,44 aqui pero tira LastCocoonEgg (ctor de LastCocoonEggPostFx_unk), NewCampTask (NewCampView_unk ctor) y NewTitleTask ctor: no commiteado. `cm` con nombre + `cm.mEnabled = 0` (96,73 pero mueve la pila), bucle o reorden de bias/scale: peor. (escribir-20261116 qN)
- Cockpit fn_804B5B10 99,78: el original relee lbl_8087F610 al principio del cuerpo del bucle de jugadores. Probado: getter debil fn_800F7F90 en el cuerpo y en la condicion, referencia `MpSession_unk*& s`, while, GetPlayer_unk definido despues de la funcion: igual. (escribir-20261116 qN)
- Cockpit fn_804B50C0 99,60 (case 3 en r27, original r28): locales a/b, `task` compartido en toda la funcion (99,21), bloque, `this->vf20()`, if/else en vez de switch (96,2), cases en otro orden (64): igual o peor. (escribir-20261116 qN)
- OnOff fn_803F8324 99,16 (destino de la copia de mMtx en r6 y no r3; z de mBoxPos por 8(r8) y no 0x97c(r31)): `mtx = mMtx`, ctor de copia de la matriz, `off = mBoxPos`, `Assign_unk`, pos desde mMtx, off antes de borrar la traslacion, box antes de mtx: igual o peor. (escribir-20261116 qN)
- OnOff vf0C 99,86: inline `__OffsetMtx_unk(mtx, r, t)` con rot + Concat + SetTrans en los dos case: 96,8. (escribir-20261116 qN)
- AutomaticDoor fn_803F53E0 (parche qF2, 99,90): `GetViewVec_unk()` que devuelve la resta por valor: igual. (escribir-20261116 qN)
- ReinforcePoint vf10 99,81: getter debil fn_80121F00 para la tabla de rutas (y en el m54E4): 99,77. (escribir-20261116 qN)
- Orden de declaracion de los locales (todos arriba, 120 permutaciones): sin efecto en dwc_transport DWCi_TransportProcess (C, las 120 iguales), MovieData::vf08, SelectMember ctor (registros salvados), ControlSlip::vf0C (mChara r3/r5, tambien con `Chara_unk* chara` local), PostFxRain vf10 (fullW/fullH/w/h/image: maximo el actual 99,72); FallingTower fn_8041044C (heights/i/a/b/blend: peor, 99,69). (escribir-20261116 qN2)
- EfpData_unk::fn_802440F0 99,36 (con el bucle en inline): contador/desplazamiento r29/r30 cruzados. (escribir-20261116 qN3)
- InteractionSpot Start, CharaLight-like ya resuelto; CostumeEditChara vf14 (`addi+lwzx`) sigue. (escribir-20261116 qN3)
- ControlGrappled::vf10 (99,89): (1) Mtx del segundo bloque por debajo del bind_t del primero (probado else if, else anidado, bloque interior, `boost::function` con nombre, Mtx declarada antes del dir, RotY_unk: arregla la pila pero la llama fuera de linea); (2) la base r31 de los ptmf es `...data.0` en el nuestro y `@13439` en el original porque el original emite primero en `.data` el `@STRING@allocate` del bind_t de 4 argumentos (198); el nuestro lo emite el tercero de los cuatro al final. (escribir-20261116 qN4)
- Otros probados sin 100: Em101 vf0C (push_back), Em001Necromancer ctor (24 ordenes de las sentencias de banderas y 7 posiciones de los `clear()`), CannonLift vf10 (`else`, inversion del `if`, `boost::function` local, inlines `NewTarget_unk`/`BindNext_unk`), CannonThrow vf0C (`PSVECMag(&(point - m08))` 99,94 sin commit; GetQueenPoint_unk deja &point en r29), Cockpit fn_804B50C0 (`if`/`else if` 96,2), ShopColor fn_805880F0 (`i` arriba, `party` despues: igual o peor), ControlChant::vf0C (24 ordenes de las declaraciones de arriba: igual o peor), LH_Model (sin tocar: Get_unk y pila de fn_80474F0C). (escribir-20261116 qN4)
- NewCampPartsTop ctor (98,88): el bucle de las 8 entradas (entrada/sInfo/desplazamiento) y la copia de `mInfo` (r0, r3, r0 en el original): referencia, sin local, copia miembro a miembro, `info` puntero: igual o peor. vf14: texto/layout de `select00_text` (local `text`, `text`+`layout`, llamada directa) y el f29 de `enableLoad` (local con nombre arriba): igual o peor. (escribir-20261116 qN4)
- Cliffhang vf0C (99,83): FPR de `speed` del estado 1 (f30 en el original) con `scale`/`speed` declarados arriba: igual. (escribir-20261116 qN4)
- Cliffhang vf0C (99,69): `r` como temporal, `dir` en inline por valor (95), `Transform_unk` por referencia (96), `speed` en una expresion (95): peor. El original tiene `dir` (0x38) por debajo de los temporales de `dir * 4 * speed`. (escribir-20261116 qN4)
- OnOff fn_803F8324: `__VecCopy`/`__Vec2Copy`+z para `off`, referencia de la matriz: igual o peor (99,2). (escribir-20261116 qN4)
- ControlMagic::vf0C: copia de `off` en SpreadTarget_unk (local con nombre + copia da la forma, pero mueve las sumas del angulo; 94,9) y la direccion de la Mtx en r15 (Matrix3x4 miembro, puntero, MulVec_unk): sin cambio. (escribir-20261116 qN4)
- Reduccion de fuerza: el cursor que crea el compilador para `a[i]` toma otro registro no volatil que en el original (gmk/PassBoard vf0C, gmk/RandomMerchant vf20); el orden de declaracion no lo mueve. (escribir-20261116 qN5)
- FPR de UpdateSwing_unk (mMaxWind/mWindPower cruzados): bloquea SwingObject vf10, SwingPart fn_803FFF40 y fn_804003E0. (escribir-20261116 qN5)
- SwingPart_unk ctor (99,46): color de vt/begin/end del bucle del array de SwingParam_unk tras el ctor inline de LH_Binary. SwingPart_unk fn_803FFF40 (99,90): m98 de m9C/mA0/mA4 en r26/r27 (r28/r29). SwingObject vf10 (98,36): la direccion del modelo izada antes del LOD. (escribir-20261116 qN5b)
- WaitObject vf60: relectura de m54 en el bucle. MotionObject vf24: registro del tamano del CSV. (escribir-20261116 qN5b)
- ChickenManager/PigeonManager FindNear_unk: pila (pos, t, diff) del original no reproducida. (escribir-20261116 qN5b)
- ColliAttrManager fn_80183FFC: `static inline CopySe_unk(this, index, data)` + 4 asserts vacios (para no inlinear en fn_80183D00) da 92,45 (como qD); path arriba, return temprano, data en dos pasos: igual. (escribir-20261116 qN6)
- EffectLoader_unk fn_80239030 93,31: el segundo byteswap (cuenta de new[]) sale en otro orden de rlwimi; las 24 permutaciones de terminos, inline Swap32_unk y mascaras antes/despues: iguales o peores (93,79 con `((n & 0xFF00) << 8)`, no commiteado). Header r26/res r27 rotados. (escribir-20261116 qN6)
- NetManager ctor 97,9: bucle de constructores de NetPlayerInfo_unk generado por el compilador (puntero final r27 en el original); no tocado. (escribir-20261116 qN6)
- SoundManager fn_800CE6E0 99,72: el primer erase() inlinado tiene indice/texto cruzados (r27/r28) y el segundo no; probados forma del bucle de erase (for/while, ++i, 1+i, referencias src/dst, begin()), operador de copia explicito de SoundLogEntry, variantes de SoundString::operator=, estructura del while (continue, for, inline estatica UpdateLog_unk). (escribir-20261116 qN6)
- SoundManager fn_800CECF0 98,3: x/y por bloque empeora (97,5). (escribir-20261116 qN6)
- NetManager fn_8050CFC8: el `== 1` de IsJoined_unk se pliega a `!= 0` (int/bool/unsigned, inline antes o despues, local `joined`): sin efecto. (escribir-20261116 qN6)
- ShopBuySell fn_805866F4 93,58: la direccion `anime->mLayout` del primer SetPaneText se calcula antes del hash en el original; `SetTaskText_unk` (como MP) y locales sin efecto. (escribir-20261116 qN6)
- FileList fn_8006BD78 97,7: `mr r7, r6` del final de la cadena en assign (replace con Traits::length, `e = s; e += n`, assign(first, last)): igual o peor; queda ademas el push_back. (escribir-20261116 qN6)
- Em312DarkServant fn_8030E5C8: 6 ordenes de master/list/i arriba: 98,23-98,31. (escribir-20261116 qN6)
- `ControlDamageLastOne::vf0C` / `ControlKnockdown::vf0C`: la inline miembro que recarga `mChara` (fn_801B0260 / fn_801AE41C): el bool de la inline coge r31 y el `model` de fuera r30 (original al reves). Reproducido en un fichero suelto; sin cambio con local `chara`, bool con nombre, bool declarado antes, `!= false`, getter del modelo o doble inline. (escribir-20261116 qN7)
- `ControlKnockdown` ctor: r25/r26 de la ruta del motion y del `strlen` del ctor de `std::string` (inline en `std/string.h`); sin cambio con la ruta en otra inline, conversion implicita, string con nombre o la declaracion de `path` en otro sitio. (escribir-20261116 qN7)
- GameTextMessageWindow HasVoice_unk: el original guarda tres bools (li 0 al principio y li 1 de cada uno); `return a && b`, `bool r = false; if (...) r = true;` y `bool r = a && b;` en inlines anidadas dan el mismo codigo (MWCC pliega los dos interiores). (escribir-20261116 qW1)
- GameChoiceWindow fn_803B6970/fn_803B6E04/vf10: el original deja materializados los tres bool anidados de HasVoice_unk (r28/r29/r30); probadas las inlines con `return a && b;` (sin cambio). (escribir-20261116 qW1b)
- TutorialTask fn_805BA934: `m5B4 = mAttackType != 0` sale con neg/or; el original lo hace con dos cntlzw. Probados `!(x == 0)`, `!!x` y una inline bool (MWCC lo pliega). (escribir-20261116 qW1b)
- GameScene fn_803CB618 (91 %), fn_803CAED0 (97,6 %), fn_803C1CAC (98,9 %): orden de cargas/pila; probados reordenar las referencias a/b, operator- en vez del ctor por componentes (91,4 %), bloque anidado y declarar sin inicializar en fn_803CAED0, y `mCamera != mDefaultCamera` en fn_803C1CAC. (escribir-20261116 qW1b)
- ReplayBuffer fn_80570A74: con las inlines reales `mEffects.begin()/end()/erase(p)` sube a 89,7 % pero desplaza dos `@N` (labelcheck peor); no commiteado. Sigue el reparto de registros (contador exterior en r31). (escribir-20261116 qW1b)
- GameChoiceWindow fn_803B5974/fn_803B710C: el pool ancho original (.sdata 0x8087DD80-0x8087DD98) tiene 0x18 bytes: L"" de fn_803B3D2C en +0, el de fn_803B4C38 en +8 y L"\xFF1F..." en +0xC; probado `L"\0"` en fn_803B4C38 (queda en +6). Falta saber que literales ocupan +2..+8. (escribir-20261116 qW1b)
- GameTask vf14: el original guarda MemoryManager* y recalcula +0x138/+0x150/+0x168 en cada llamada virtual; probados la inline con referencia y con puntero a miembro (sin cambio). (escribir-20261116 qW1b)
- ReplayBuffer fn_8056A9E4: las lfs de la columna z se intercalan en la copia paired-single de __MtxCopy en el original; mover las declaraciones no cambia nada. (escribir-20261116 qW1b)
- GameChoiceWindow pool ancho (fn_803B5974/fn_803B710C 99,99 %, fn_803B4C38): el original tiene L"" de fn_803B3D2C en 0x8087DD80 (objeto propio, hasta 8 bytes), L"" de fn_803B4C38 en +0 de 0x8087DD88 y L"\xFF1F..." en +4 (relleno a 4 o 2 bytes mas antes). MWCC junta los L"" iguales y no alinea. Probado: fn_803B3D2C `inline` (se inlinea en sus llamadores). Sin probar: que 3D2C/4C38 fuesen funciones debiles/plantilla (sus literales irian a `@STRING@`). (escribir-20261116 qW1c)
- TutorialTask::vf0C (r26/r27 del primer SetPaneText_unk): probados local para el texto, inline con las dos llamadas, hash en local, copia del layout, segundo bucle con otro puntero, orden de page/loading, locales arriba, bool, return temprano (10). (escribir-20261116 qW1c)
- GameTask fn_8036DDD4 (r28/r29 del flag y la constante 0x4330): probados sin flag, flag declarado arriba/al principio del bucle/asignado tarde, rango antes, inline del rango, inline HpRate_unk, rate en local (11). Declarar `int found = 0;` antes de `d` deja solo el `li` fuera de sitio. (escribir-20261116 qW1c)
- GameScene fn_803C1E3C: con `GameBgm* bgm` y `float volume` locales quedan 2 diferencias (el `lfs` del volumen sale antes del `mr r3`). (escribir-20261116 qW1c)
- GameScene fn_803C1CAC: la recarga de mDefaultCamera para la llamada; probados el orden de la comparacion, inline de reset y bool IsDefaultCamera_unk. (escribir-20261116 qW1c)
- GameScene fn_803CB044: pila de los vectores del bucle (n, up, d, off, closest, sphere, side, dir en otro orden). (escribir-20261116 qW1c)
- GameTask vf14: y en f31 (nuestro f29); probados los floats del heap declarados antes/despues y en una inline. (escribir-20261116 qW1c)
- GameTask fn_80370320: recarga de m56F4.mCount en el Push_unk; inline CanPush_unk empeora (98,9 -> 98,5). (escribir-20261116 qW1c)
- GameTask fn_803729B4: el original no llama al constructor vacio de atn::Matrix3x4 del bucle de rayos (nuestro lo llama por el presupuesto R3). (escribir-20261116 qW1c)
- ReplayBuffer fn_80570A74 (91,4 %): rotacion de r10/r11/r12; fn_8056A9E4: el original intercala los `lfs` de la columna z entre los `psq_st` de __MtxCopy (con SetModelMtx_unk de GameScene igual). (escribir-20261116 qW1c)
- GameScene fn_803C606C/fn_803C6FE4 y sus copias (PointGroup/CharaGroup, 98,2-98,6 %): el original inicializa el contador n antes del memset y el desplazamiento n*4 tras begin() del tokenizer; mover `int n = 0` despues del tokenizer empeora (98,19 -> 97,0). Ademas el orden de "SceneCharaGroup"/"ScenePointGroup" en el pool (0x3b3/0x3c3 cambiados) y las envolturas `extern "C"` fn_803C72D4... de las plantillas de SimpleArray (deuda de la regla 10). (escribir-20261116 qW1c)
- GameScene fn_803C1338 (99,2 %): orden de los FPR de las constantes (el original da f31 al 0.0f); una inline Rand01_unk no cambia nada. (escribir-20261116 qW1c)
- GameScene fn_803CAED0 (97,6 %): pila de los temporales de `(b - a).GetNormalize()` frente a side/up; probados declarar side/up antes, `Vector3 d = b - a`, inline GetLinkDir_unk. (escribir-20261116 qW1c)
- GameScene fn_803C0938/0634/0C2C: el original recalcula `m90[index]` desde el indice para `id` (no lo reutiliza); un return temprano empeora (45 -> 58 diferencias). (escribir-20261116 qW1c)
- MapPartsArea::fn_804962B0 (98,55 %): r30/r31 (iterador y desplazamiento interior); declarar `i`/`it` arriba empeora (13 -> 19 diferencias). (escribir-20261116 qW1c)
- ReplayBuffer fn_8056D60C (78 %): el original carga 255.0f y 0.0f una vez y precarga f3-f11 con fmr (resultados de los ternarios del clamp); probados locales lo/hi, ternarios en locales, `v * 255.0f`, min/max por plantilla (valor y referencia) en un fichero suelto: ninguno da las precargas. (escribir-20261116 qW1c)
- ReplayBuffer vf08 (92,5 %): el original copia mColor (0x30-0x3C) palabra a palabra con r0 y los Vector3 con psq_l/psq_st/lfs/stfs; ningun tipo probado (struct POD, array, ints, op= propio) da ese patron. (escribir-20261116 qW1c)
- MapTask fn_804A0F80 (5 %): con el cuerpo en una inline (R5) sube a 30,8 % (se inlinean Lerp_unk<float> y accesores, no los Lerp de Color/Vector2/3/4 ni GodRayParam::operator=); no commiteado. (escribir-20261116 qW1c)
- MapPartsArea::vf1C (99,90 %): `lwzx base, offset` del original frente a offset+base en las condiciones de los bucles interiores; probados begin()[i] y *(begin() + i) (peor). (escribir-20261116 qW1c)
- Orden de carga de varios `vector::clear()` inline seguidos (Em202ForestSpider ctor, Em302KrakenMp ctor, NewCampTask::fn_8053032C): el original carga los tamanos en un orden que no sigue al de las llamadas; 6 ordenes probados en ForestSpider (escribir-20261116 qW2).
- Em206CocoonMp ctor y LastCocoonChrysalis ctor: las locales anidadas key/cmp/value del bucle de tokens (lo que saco LastCocoonImago de presupuesto) no bastan; tampoco una local `rest` mas ni `file = path.c_str()` (escribir-20261116 qW2).
- Operadores de color de Em202SpiderBase (`(target - c) * 0.15f + c`, vf14 de cuatro unidades al 93 %): 6 combinaciones ctor/local de operator-, operator+, operator* y tres formas de la suma sin mejora; el original pone el producto por debajo de la diferencia en la pila (escribir-20261116 qW2b).
- Em301LShade::vf10: el original recarga m15E8 despues de los stores de m1500 y m15EC (como si pudieran solaparse); sin idea para reproducirlo (escribir-20261116 qW2b).
- Orden de los `clear()` de vectores seguidos (Em301LShade ctor 99,27 %): mismo bloqueo que W2 (escribir-20261116 qW2b).
- Em206CocoonMp::fn_802BEE44: el original compara en tiempo de ejecucion `1.0f > 1.0f` (speed = 1.0); con if, inline o MIN el nuestro lo pliega (escribir-20261116 qW2b).
- EnemyTasha::fn_8035AB78: el original lleva el desplazamiento i*0x14 como induccion y suma `this` en cada vuelta; con un puntero o referencia al evento el nuestro induce el puntero (90,8 %) (escribir-20261116 qW2b).
- Em302KrakenMp::vf0C: sin el local `zero` sigue la rotacion key/pool/0 (r28/r29/r30); `key` fuera del bucle tampoco (escribir-20261116 qW2b).
- Manager `size`/`clear` de `vector<Curve_unk>` (0x8048B3B0/0x8048E994): quitando `__declspec(noinline)` a `fn_8048D3A4`/`fn_8048DE54` y poniendo N asserts vacios: N <= 5 se inlinan enteras en `fn_8048E904` y en `~Manager` (0 %); N >= 6 dejan de ser candidatas y `size`/`clear` se siguen inlinando. Umbral seco: en el original son candidatas que rompen el presupuesto de `fn_8048E904` pero no de `~Manager`. Sin idea nueva. (escribir-20261116 qW3)
- `dtor_80014250` (Puppeteer): sigue siendo el destructor del buffer de vector de punteros compartido (`void*`) del original (lo usa la tabla de excepciones de locale.cpp); haria falta que todos los `vector<T*>` usen una sola implementacion `void*` en `std/vector.h`. No intentado. (escribir-20261116 qW3)
- Pool de Sequence: el original tiene "basic_string: out_of_range" antes de "vector length error" (ninguna funcion del DOL lo referencia: viene de codigo quitado por el enlazador); las 4 funciones `grow`/`reserve` al 99,9 % solo difieren en ese offset. (escribir-20261116 qW3)
- Probados sin exito (revertidos): `Sequence::fn_80541F18` 99,48 % (dos formas del `FindActor_unk`, registros r7/r8 de id y actor); `~vector<shared_ptr<Cut>>` 98,97 % (`__shrink_unk` con la cuenta primero: 98,68); `__insert_realloc_unk<SceneSettingEntry_unk>` (Manager 0x80490098) con un nivel inline menos: 99,22 -> 99,32 % (arregla los huecos de pila del crecimiento; quedan r7/r8 del ctor del buffer), no commiteado por no llegar al 100 %; `__push_front_unk` generico con el bucle de los tipos con destructor sin el nivel de `__push_front_resize_unk`: ParseMOID y familia bajan (99,92 -> 99,54). Los ParseXXID al 99,9 % solo difieren en los huecos de pila de las dos copias de string (append y push_front) del realloc inline. (escribir-20261116 qW3)
- `DataList_unk<Actor>::Add_unk` 98,13 %: el original recalcula `&buf` (`addi r3, r1, 0x8`) en cada llamada; el nuestro lo guarda en r30. (escribir-20261116 qW3)
- Event/Sequence `construct<EffectEntry24_unk>` 0x80542684, `__ct__IntVector_unk`/`__ct__EquipList_unk` (88,7), y las 13 copias de vectores de palabras del DOL (todas con la misma forma): `mSize +=` tiene que compartir la resta con `n` pero no la division. Probado (prueba suelta, `/home/claude/scratch-W3b/t/`): `n` int/unsigned/char/`/4`, `/(int)sizeof`, `/(long)`, conversiones, `distance` con etiqueta, `n` por referencia a inlines, suma dentro y fuera de `__construct_at_end_unk`, firmas por valor/referencia: o se reutiliza `n` entero o se recalcula todo. (escribir-20261116 qW3b)
- EventParser_unk ParseXXID: con el nivel inline mas en `__push_front_resize_unk` (Hallazgos) quedan los huecos {0x10,0x14} y {0x18,0x1C} de las copias de append y del movimiento intercambiados; la profundidad de la rama de append (-1, +2 niveles) y el orden de definicion de los miembros de `__vec_split_unk` no los mueven. (escribir-20261116 qW3b)
- Manager fn_8048871C / fn_80488B5C: r23/r24 de frames y del corte en `FramesTo_unk`: probados orden de declaraciones, `for`, `frames = frames + ...`, inline con el corte como argumento, cuerpo de `FindCutByIndex_unk` de Sequence.cpp: nada. (escribir-20261116 qW3b)
- ActionMusicPlay::vf0C 89,75: el original carga el volumen (lfs) antes que m18/m1C porque el puntero del parametro 1 usa r5; probados orden de locales, `GameBgm*` local, puntero al parametro: 88,75-93 sin llegar. (escribir-20261116 qW3b)
- Puppeteer fn_80011E98: con `if (m20 == 7 || m20 == 1)` fuera y el personaje por ternario se arregla el salto (el original va al final si el tipo no es 7/1) pero el `m1C` se CSE con el del switch de fn_8001296C (el original lo recarga): 92,5; probados `fn_800128FC()` dentro del if y tres cuerpos de fn_800128FC (baja la propia). (escribir-20261116 qW3b)
- Puppeteer fn_8001122C 58,8: copia de la matriz: el `lwz m1C` se adelanta a las cargas; probados `SetMtx_unk` por valor, `m58 = mtx` con copia local en el llamador: igual. (escribir-20261116 qW3b)
- ActionActorPosition fn_80557F84: el pool sale identico con IsEarring/IsWaist/IsBust no estaticas + string_members.h + instanciacion explicita del ctor de subcadena tras la funcion (99,36 -> 99,39), pero sigue el recorrido de los huesos por `&model->m384` (FixedArray, ver mixto qR): no commiteado (solo fuzzy). (escribir-20261116 qW3b)
- Movimiento de los elementos viejos en la realloc de push_back de elementos POD/sin destructor (Puppeteer `push_back<ColorF_unk>` 95,6, Node fn_805393F8, Sequence fn_8054204C): el original convierte `while (last > first)` en bucle contado (`mtctr`/`bdnz`, cuenta `(last - first + size - 1) / size`) y decrementa `last` antes de las cargas; el nuestro deja `cmplw/bgt` y pliega el decremento en los desplazamientos. Mandar los POD por `new (--p) T(*--last)` no lo cambia (ColorF ya va por ahi) y baja fn_805393F8: revertido. (escribir-20261116 qW3b)
- Sequence fn_8054136C 99,17: con `Cut* target = cut;` en fn_80541198 (que sigue al 100 %) la rama de etiqueta sale exacta (99,50) pero la de corte sigue con r5/r6 cruzados; no commiteado (solo fuzzy, local de copia). (escribir-20261116 qW3b)
- DebugInfo fn_802219CC 88,9: el original evalua `c_str()` antes de los floats; con un local para el texto o la llamada directa a `fn_800616C0` (sin `fn_80221CAC`) sale igual. (escribir-20261116 qW3b)
- Node fn_8053A2E8 89,3: el original recarga `mKeys.mData` en la comparacion del bucle y usa la carga inicial para la interpolacion; con la comparacion por una inline `KeyTime_unk(keys, i)` 90,26 (no commiteado). (escribir-20261116 qW3b)
- SmallAllocator_8 (`~SmallAllocator<6>`, `operator delete`, y por ellos `~MemoryMode_unk`): son codigo equivalente; el original llama al ctor del gestor dentro del `Instance` inlineado en `delete` (y en lo que inlinea `delete`: los dos `~SmallAllocator<N>`, `~MemoryManager`, `~MemoryMode_unk`) y lo inlinea en el resto (`__dla__`, `__sys_free`, `operator new`). Medido (tamanos en `/home/claude/scratch-W4/bin/saall.sh`): con el ctor al principio (hoy) cortan `~IAllocator`, `~SA<8>` y `~MemoryManager` pero no `~SA<6>` ni `delete`; **intercambiando el orden de mSmall8/mSmall6 corta `~SA<6>` y no `~SA<8>`** (corta el primer miembro, no un tipo concreto); con el ctor tras `Instance` (orden del .dol) ningun dtor corta y `delete` llama a los ctor de `SmallAllocator<N>` fuera de linea (se emiten); dtor de la plantilla definido fuera de la clase: igual. Con el ctor del gestor `inline` (sin moverlo) el orden de emision es el del .dol (Instance, ctor, ~SA<8>, ~SA<6>, ~MemoryManager, fn_80082CAC: el ctor sale como copia diferida de Instance), pero `delete` y los dtors quedan con el ctor del gestor inlineado y los ctor de `SmallAllocator<N>` llamados (y emitidos): el corte cae en el ctor de SmallAllocator (su vtable dispara la compilacion anidada de los dtors) y no en el del gestor. Ni el ctor de SmallAllocator fuera de la clase ni `~IAllocator` no inline lo cambian. Hipotesis: el primer inline de `delete` fija su cuerpo para todos los demas; en el original ocurre con el ctor del gestor en curso y sin ningun ctor de SmallAllocator en curso. (escribir-20261116 qW4)
- WordWrap_unk (`setstate`, copia de `basic_string<char>`, thunk `@8@60@~basic_ostream`): en el original son copias weak que conserva el enlazador porque otras unidades las llaman (`setstate` EffectManager, la copia Loader/EffectSequencer; el thunk va en la vtable de `basic_ostream`, que el original emite en WordWrap y GameEventScript usa). Sus llamadores dentro de WordWrap no estan en el .dol (los quito el enlazador) y nuestro .o no emite ninguna vtable de streams: hace falta el codigo de iostream (ctor de `basic_ostringstream`) que las instancia, MSL iostream fuera de alcance. Sin cambios. (escribir-20261116 qW4)
- AI_archer_attack01 `fn_8001C088` (ideas nuevas, todas peores o iguales): copia de structs de 0x64 bytes (`struct { Block m[5]; }`): bucle de 8 bytes x 12 (68,9 %); `operator=` propio en el bloque + copia del struct del array: bucle de punteros con division (42 %); contador `unsigned char`/`unsigned short`: no desenrolla pero indexa con `mulli` (69 %); `signed char`/`long`/`char`/`unsigned int` y punteros con contador `unsigned char`: desenrolla (0 %). (escribir-20261116 qW4)
- ipcclt `pad_03_8061BD88_text`: es el relleno de alineacion de 16 de `ipcclt.o` (`-func_align 16`) tras `IPCSetBufferLo`; darselo a ipcMain.c (enlazada: end 0x8061BD90, ipcclt desde 0x8061BD90) da `main.dol: FAILED` (nuestro ipcMain.o no lo emite: el enlazador lo pone al alinear la unidad siguiente). Se queda dentro de ipcclt hasta que ipcclt enlace; entonces `.text start:0x8061BD90` (el enlazador alinea). (escribir-20261116 qW4)

## enlazar2-20261117

- PostFxMotionBlur vf10 99,90: puntero `lbl_8087EFA8` r4/r6 al principio (~14 variantes: int local, if/else en vez de ternario, `step` local, `a`/`b` locales, `double x`). (enlazar2-20261117 qA)
- VideoTHP vf10 99,28: invariante a 9 cambios de orden/forma (registros volatiles r8/r9 y un lis/addi). (enlazar2-20261117 qA)
- CompositeCache_unk fn_800D8458: it/end r31/r30 aun con el `cmplw` arreglado (tambien `continue`, `last` const, `!(it == end())`). (enlazar2-20261117 qA)
- CsvBase_char_124_13_34 fn_802078C4: entry/levels r26/r24 (original r24/r26); tambien `level` local y `unsigned long len`. (enlazar2-20261117 qA)
- Constante 0 copiada de otro registro (`li r6, 0; mr r5, r6`, BulletPoint vf10): ni asignaciones encadenadas ni `disabled = ok` ni el `||` materializado la dan. (enlazar2-20261117 qB)
- Orden de coloreo de los GPR salvados (SwingPart fn_803FFF40 bases de m98 r26/r27 frente a r28/r29; MotionObject vf24 tamano del CSV r22 frente a r25): sin cambio de codigo que los mueva. (enlazar2-20261117 qB)
- game/MapPartsRain: 8/8 al 100 % pero no enlaza por el `.sdata` a cero (lbl_8087E0D0 = 0) del propio split. (enlazar2-20261117 qC)
- game/GameSaveBanner fn_803B9638: argumentos de NANDInitBanner del ultimo al primero (todo lo demas al 100 % con el desplazamiento unsigned). (enlazar2-20261117 qC)
- Event/Node fn_805389B8 99,60: r5/r6 de la secuencia frente a la prueba de rango (ver arriba). (enlazar2-20261117 qC2)
- Reparto de registros salvados sin cambio de codigo (ControlKnockdown vf0C r27/r28, DamageLastOne r30/r31, EnemyTasha vf10 r29/r30, Em001Necromancer vf14 r28/r29): las formas naturales de la llamada y de la inline no lo mueven. (enlazar2-20261117 qD)
- `frsp` de la z en `v = m574; v.y += c; m574 = v;` (ControlDropDown ctor): en fichero suelto sale con dos niveles de copia, pero en la unidad no. (enlazar2-20261117 qD)
- FileList fn_8006BD78 99,38: registros y ranuras del realloc del push_back de `vector<FileList::Entry>`; no depende del llamador (5 formas) ni de los ctors de Entry; cambiar niveles de `std/vector.h` no lo arregla. (enlazar2-20261117 qE)
- RenderSetting_unk ctor 97,0: planificacion de cargas de constantes; valores iguales; un sub-struct con ctor inline no la cambia. (enlazar2-20261117 qE)
- ColliCalc_unk fn_8005AD7C: referencias en la inline de rango, peor. (enlazar2-20261117 qE)
- NetManager fn_8050CFC8: el `== 1` del bool de IsJoined se pliega; sin idea nueva. (enlazar2-20261117 qE)
- Dialog ctor 97,95: registros de los bucles anidados (original: cut r31, section r30, kind r29, arg r28; aqui los mas bajos). Probados: declaraciones arriba en el orden del original (sin efecto), `CutAt_unk` inline, if/else del cut, arg antes que kind: igual. (enlazar2-20261117 qF)
- Dialog fn_802297AC 98,93 (parche): queda el 1 de `mEnable` (r3 en el original) y `lbl_8087EFA8` (r8) cruzados. Probados: `env` local en la inline (89,7), env/dst como parametro, `mEnable` como parametro, `GetEnv_unk()`, `bloom.m48 = bloom.mEnable = 1`, bloque del bloom en otra inline (93,2), copia `fog = src` (91,7), sin `mEnable = 1`. (enlazar2-20261117 qF)
- ActionCameraEffect vf08 99,52 (parche): quedan arg/fadeOut (r28/r27 en el original, al reves aqui) y el tiempo transcurrido del helper de fundido en f4 (aqui reutiliza f1/f2). Probados: helper como miembro de Action (`this`), `t` declarado fuera, `rate` como `t`, `now` por referencia, `start`/`end` por referencia, `const Action*`, `float in = start`, `fadeOut` declarado en la asignacion: igual o peor. (enlazar2-20261117 qF)
- MotionData vf08 99,8: `inline` explicito en MovieKind_unk (94,4), `find` en el `if` (98,5), ternario (96,5), `rtmv` local (38): peor. (enlazar2-20261117 qF)
- ActionCameraSetting vf08 99,09 (mas): `int end` entero aparte, `left` sin `now`, `MinF_unk` por valor y por referencia (97,6), `r` local con `t = 1.0f`: igual o peor. (enlazar2-20261117 qF)
- Cockpit fn_804B50C0/fn_804B5B10: sin idea nueva (no tocadas tras leer las notas). (enlazar2-20261117 qF)
- dwc_ranking DWC_RnkResGetRow 98,6: probados `(u8*)header.body + header.size`, `for`, `row +=`, `!header->size`, comparacion en `u32`, `(&header)->size`, puntero `int* size` al campo en el helper (93,9), helper `static inline`: igual o peor. (enlazar2-20261117 qF)
- Cockpit fn_804B5B10 / fn_804B50C0 y MotionData vf08, ActionActorBehavior fn_8054FBE0 (0 copiado), Label/Channel `__sinit`, Channel/Section (push_back shared_ptr): no tocadas, bloqueos documentados. (enlazar2-20261117 qF)
- ActionCameraSetting vf08 99,09: lerp inline con `const float&` (3 firmas), `const float& rate = t`, `float end` aparte, `step` local, `t = 1.0f` inicial: igual o peor. El original da a `t` el FPR de `now` (f3), aqui el de `left` (f2). (enlazar2-20261117 qF)
- Dialog fn_802297AC 98,24 (parche `docs/enlazar2-20261117-qF-dialog297ac.patch`): quedan dos pares de FPR cruzados (m14/m18 en f8/f7, m1C/m28 en f6/f5) y el 1 de `mEnable` (r3) con `lbl_8087EFA8` (r8). Probados: orden m14/m18 y m1C/m28, todo al reves (89), setter `SetFog_unk` por valor/referencia (62/93), `SetBloom_unk`, `GetBloom_unk`, lerps en locales (87,6), fabrica `MakeFog_unk` con 12 argumentos (79), lerp en dos niveles, `const float t`: igual o peor. (enlazar2-20261117 qF)
- game/ChaseManager vf10: el `switch` 1..2 sale con `cmpwi` con signo en el original porque el estado esta en r0; el problema es de registros, no de la forma del switch (MWCC usa `subi/cmplwi` cuando puede). (enlazar2-20261117 qG)
- game/PopupTask fn_8054D794: [0] encaja con una inline de retorno temprano y [2]/[1] con la de variable resultado; resultado y puntero del bucle siguen cruzados en [2]/[1]. (enlazar2-20261117 qG)
- game/CharaD74_unk fn_8011CD84: rama muerta `b` tras la limitacion del radio del caso 4 (el optimizador de saltos la quita en todas las formas probadas). (enlazar2-20261117 qG)
- atn/Pad_unk fn_800A58D0/58F8: tabla de traduccion en r5 en una funcion hoja de 10 instrucciones; ninguna forma de C++ cambia el orden de coloreado (gestor antes que tabla). (enlazar2-20261117 qG)
- push_back inline de `std/vector.h`: tamano/capacidad con los registros cruzados (ArrowSet, ShopColor, ShaderManager); ~25 formas mas probadas en enlazar2 qH (lista en Registro), ninguna cambia el reparto. (enlazar2-20261117 qH)
- Swap de la realocacion de `vector`: el movimiento del tamano (parche qH) baja Event/Manager fn_80485964/fn_804862C4. (enlazar2-20261117 qH)
- game/CrowdSimulation ctor: parche qA2 (97,88) reaplicado; solo numeracion: el `0x4330` de la conversion int->float va a r31 (r16 en el original, compartido con `data`). Probado: `(float)` explicito, `x/1000*2`, `index` arriba, `int j`, `index++` en el indice, `hour` antes de `count` (97,89), `mDir` sin local, `boost::function` con nombre (86), `int r = rand()`; rejilla de tipos (`int`/`unsigned`) de los dos `i`, `j` e `index` (16): 97,88 o 96,81; perm.py de `count`/`hour`/`index` arriba: 97,14 como mucho. Sin commit. (enlazar2-20261117 qI)
- gmk/AutomaticDoor fn_803F53E0: parche qF2 (99,899 < 99,9, sin commit): r5/r6 (instancia de RenderManager / temporal de la resta). Probado ademas: getter `fn_8008B964()` (una o dos veces, local), `Camera&`/`Camera*`/`const Vector3& target` (99,57), dos niveles de inline, `diff` con nombre, `__VecCopy` directo, RenderManager por parametro: igual o peor. (enlazar2-20261117 qI)
- gmk/HealPoint fn_8040ED00: permutaciones de 5 sentencias del cuerpo del ctor (mWait, m54, `rot` con nombre, m6C, m78; 60 ordenes): 93,75 como mucho; `float rotY` local (90,5), `const GimmickDesc_unk* d` (93,75). (enlazar2-20261117 qI)
- atn/EffectLoader_unk fn_80239030 (93,31): los tres punteros `textures`/`emitters`/`defs` calculados ANTES de los seis `+=` (justo tras `res->mVersion = ...`) dejan el primer bloque como el original (94,95, sin commit: solo fuzzy; parche `docs/enlazar2-20261117-qI-effectloader.patch`). Queda (a) el swap de m40/m44: el original intercala los rlwimi de v y w y suma `swap + header` (nosotros `header + swap`); `header + SWAP`, swaps en locales, `+=` despues, sin locales, cast a int o a puntero: igual o peor; (b) el swap de la cuenta del new[] (original empieza por `rlwinm 24,16,23`): el orden de los terminos del `|` no cambia nada (24 permutaciones identicas, MWCC lo canoniza); 81 formas de los terminos (mascara antes/despues del desplazamiento): 95,46 como mucho (`(n & 0xFF00) << 8`), con un `mr` de mas. (enlazar2-20261117 qI)
- atn/PBoneBasic vf10: los temporales muertos del original son A@0x8 (solo y=0 en 0xc), B@0x14 (z=0 en 0x1c) y C@0x38 (x=1.0); el de 0x2c es el `Vector3(1,0,0)` de m14 (su z muerta en 0x34). No probado mas (sin idea del origen). (enlazar2-20261117 qI)
- game/PickTask vf10, game/FinishCamera_unk fn_803792F0 (13 KB, marco 0x10 distinto), MP/CryptMP vf0C (pila + doble 0.0 en .data), atn/D2A_MessageTable_unk fn_801F36C8 (el original relee tamano/capacidad del vector tras el `fprintf`/`abort` de length_error; nosotros los guardamos en r16/r17): solo revisadas. (enlazar2-20261117 qI)
- GameSoundControl fn_803CDF84: switch sobre un campo leido en local; el original da r0 al valor del switch y reutiliza la base para el otro; ninguna forma de C lo reproduce (enlazar2-20261117 qJ).
- D3AnimeInstance Draw: lbl_8087EEB0 recargado en r5 en el original (r3 nuestro) en todo el cuerpo; insensible a la forma de las sentencias. (enlazar2-20261117 qJ)
- push_back inline (tamano/capacidad cruzados en ~73 sitios): resuelto en el banco con c capturada y sin lectura muerta de `mCap`, pero eso invierte las copias fuera de linea (100 -> 99,84). Hace falta una forma que de ambos o aceptar el cambio con sus bajadas (decision del usuario; parches qK). (enlazar2-20261117 qK)
- ControlChaseFollow vf0C 99,94: en la ultima rama dist/near en f29/f30 cruzados y mChara/&d en r3/r4 (22 variantes). (enlazar2-20261117 qL)
- MP/D2ACommonDialog fn_804AC96C 99,70: task del ultimo Play_unk en r26 (12 variantes en total, ver notas). (enlazar2-20261117 qL)
- atn/D2Anime_unk fn_80201794 99,91 (parche): 0/data del primer ReadTracks cruzados (r30/r31). (enlazar2-20261117 qL)
- DamageCalc fn_8003EA3C/fn_8003F1E4: el original calcula `&target->mStatus` (r6) antes que `&attacker->mStatus` (r5); ninguna de ~12 formas (envoltorios, referencias, locales, ternario) lo cambia. (enlazar2-20261117 qM)
- SpText fn_8021C3F4/fn_8021B060: la longitud del primer bucle comparte registro con `text` en el original (r23/r25) y con `pos * 2` aqui. (enlazar2-20261117 qM)
- OptionParts fn_8020C5D4 y fn_8020CC64: el orden de operandos del std::equal inline de IsKey_unk es distinto en las dos funciones con la misma inline (cada forma arregla una y estropea la otra). (enlazar2-20261117 qM)
- `XLook_unk::Set_unk` inlineada sobre `lbl_8087F430->m97C`: el original materializa `this` (r30+0x97C en r5) y escribe `mPos` en `0xc(r5)` y m04 en `0x4(r5)` (las de m00/m01 van por r30+0x97C); nosotros plegamos a r30+0x988. Igual en Em204Mulue vfEC, Em101ElderZesya vf10, Em203GreaterDaemon fn_8028E800 y Em302Kraken vf10. Probados puntero local, `SetPos_unk` anidada, `this->`, ternario, `__VecCopy`. (enlazar2-20261117 qN)
- Em204Mulue vf10: huecos de pila del case con `fn_801781B0` y del bloque catchpoint (el bloque en una inline: 99,88). (enlazar2-20261117 qN)
- ControlJump ctor 99,86: el original calcula `&out` en r3 antes del store de m570 y carga mChara en r4; setter, `__VecCopy`, referencia, inline libre/miembro con argumentos: nada. (enlazar2-20261117 qN)
- EnvSeManager vf14: variable de induccion del desplazamiento del bucle (`mAreas[i]`) y `bool current` cruzados (r26/r28); ~35 formas entre N y N2 (ver arriba). (enlazar2-20261117 qN2)
- Em204MulueMp fn_802B55B0: reparto completo r23-r31 (el bucle de jugadores de la sesion con `GetPlayer_unk(i)`); ~20 formas. (enlazar2-20261117 qN2)
- Em208Queen fn_802CB43C: r0/r4 del int->float de `mParamB.m84` y del fctiwz en `MAX(0, MIN(tenth, hp - 1))`. (enlazar2-20261117 qN2)
- OptionParts fn_8020D688 (99,89): nombre del hueco y puntero de la busqueda inline cruzados (r25/r22); ~30 formas entre qM y qO. (enlazar2-20261117 qO)
- PacketPack fn_804EE4F0 (99,89 %): el original calcula `&chara->mFA4` (r4) antes que `&dir` (r3) en PSVECDotProduct; probados la llamada directa, `PPDot_unk` con argumentos cruzados (99,95, r3/r4 al reves), inline de punteros, inline en dos niveles, referencia local y local `dot` (enlazar2-20261117 qP)
- PacketPack fn_804DDA04 (98,54 %): `PPCountTeams_unk` da 98,97; quedan los bucles de `order` (el original compara con signo y divide con signo, pero con `int i` cambia la reduccion de fuerza: 96,5) y 0x10 de marco (enlazar2-20261117 qP)
- PacketPack fn_804E5BC4 (99,60 %): el bucle de recuento de vivos en una inline 99,52; el primer bucle (temporizadores mD50) en una inline: igual (enlazar2-20261117 qP)
- PacketPack fn_804F8464 (99,89 %): el original reutiliza el `lbl_8087F628` de la condicion del bucle de hosts para `mC215` y relee el global para `info`/`mFlags`; probados orden de locales, sin `net`, `operator[]`, `vf10`, condicion directa (enlazar2-20261117 qP2)
- PacketPack fn_804D8910/fn_804FAB40 (99,44/99,58 %): cola, cursor y contador de fn_804F5A98 inlineada rotados (r8/r6/r5 en el original); getter, cast, local, copias inline del bucle: igual o peor (enlazar2-20261117 qP2)
- PacketPack fn_804E5BC4 (99,60 %): el 0 de `mr r24, r29` copiado del contador (bloqueo conocido del 0 copiado de otro registro) (enlazar2-20261117 qP2)
- PacketPack fn_804F3BAC (97,86 %): el original no reutiliza el global de la condicion del bucle en el cuerpo ni en la busqueda inlineada; condicion por fn_804D5F40 (noinline), inlines de la cuenta o del slot, getter: igual o peor (enlazar2-20261117 qP3)
- PacketPack `~PacketPack`/`~PacketPackMP` (98,99/99,63 con `end` local + `erase(begin(), end)`): r27/r28 cruzados en el destructor de la lista miembro de tres niveles (dos o cuatro niveles, `erase` en la base: peor) (enlazar2-20261117 qP3)
- ControlChaseFollow vf0C 99,97: mChara/&d r3/r4 cruzados en la ultima rama (ver arriba). (enlazar2-20261117 qR)
- `XLook_unk::Set_unk` (lbl_8087F430 + 0x97C) en ~20 funciones: el original tiene `this` de la inline como valor en el IR (y `flag` sin sustituir: ternario plegado tarde); ninguna forma natural probada (unas 70, lista arriba) hace que el frontend no sustituya `this`. Con `flag ? m00 : 0` y `__VecCopy` directos Mulue vfEC llega a 99,96 (solo `this`). (enlazar2-20261117 qT)
- game/EnemyQuarkFirst con los tipos reales (`GameCameraModeParam_unk m46C`, `GameCameraParam_unk` local con su ctor inline `m34(0.0f)` definido en la unidad, `SetLook_unk(GameCamera&, ...)`): vf108 94,59 -> 94,40, vf100 igual tras pd. Parche en docs/enlazar2-20261117-qV.patch; probar sin el ctor (un struct POD para el local) o `GameTask* field`. (enlazar2-20261117 qV)
- push_back de `std::vector<T*>` con tamano/capacidad en r3/r4 cruzados en Em202Spider vf0C, Em308InvertCrab vf0C y Em202SpiderBase vf0C (mismo diff; seria la cabecera compartida `std/vector.h`, no probado por el coste del build completo). (enlazar2-20261117 qW)
- em005 fn_802531F4: `li r4, 1` muerto delante de `li r4, 0` en `fn_8016E4C4(IsHiddenPhase_unk())` tras `m14BC = 2` (14 formas). (enlazar2-20261117 qW)
- Em003 ctor: orden de los `li` 30/600/10/60 del bucle de valores por fase (con `m16E8.mRes = 0` en el cuerpo el resto sale). (enlazar2-20261117 qW)
- 5c `SoundEngine_unk` -> `atn::AudioDevice`: sin cabecera de AudioDevice (pendiente: sacar la clase de atn/AudioDevice.cpp a include/atn/AudioDevice.h y unificar sus vistas nw4r::snd con atn/SoundCore.h y atn/Sound3dEngine.cpp). (enlazar2-20261117 qX)
- HitInfo_unk::m48 `int`: chr/ControlShoot ctor 100 -> 99,90 (operandos del `add`). (enlazar2-20261117 qX)
- fn_8006EF48 con el orden real `(font, size, text, ...)` en atn/NandManager (fn_800A4244 100 -> 96,95), atn/Loader (fn_8046ECDC 99,92 -> 96,77) y game/MenuCommonTask (fn_804A5824 98,83 -> 98,20): el original carga el tamano antes del salto del `data()` inlineado, lo que sale solo si el tamano se evalua antes que el texto (declaracion con el texto delante). (enlazar2-20261117 qX)
- GXSetFog con `GXColor` por valor en atn/RenderSystem_unk: fn_800C08B0 100 -> 71 (ya conocido). (enlazar2-20261117 qX)
- Copia debil `FindPlace_unk__14RouteTable_unkFi` (0x800EC204, la emite game/BattleCharaTask) llamada fuera de linea por 15 unidades (FlamingArrow, Em302Kraken, Em302KrakenMp, Em403WarLion, Em203GreaterDaemon, Em203GreaterDaemonRed, GameTask, Em301LShade, BattleCharaTask, LastCocoonQuadruped, Em203GreaterDaemonRedMp, CharaTask, Em310Gargoyle, Em001Necromancer, GameEventScript): hay que pasarlas todas a la vez y renombrar el simbolo a `FindPlace_unk__9GameSceneFi`. (enlazar2-20261117 qZ)
- Copias debiles de `std::vector<RoutePlace_unk*>` (`__dt__` 0x80244674, `push_back` 0x80244CCC, `At_unk` 0x802D8550) y `RoutePlace_unk*` en cabeceras de enemigos (Em101ElderZesyaSecond.h, ArenaTask.h, Em101LittleZesya.h, Em202MothersSpider.h, EnemyFieldMagician.h, Em203GreaterDaemon.h, Em204Mulue.h, Em305Vampire.h, Em303KrakenFoot*.h, Em308*.h, Em302Kraken*.h, LastCocoonImago.h, Em005DoppelGanger.h, Em202ForestSpider.h, BattleCharaTask.h, Em309Ghoul.h, Em306GurgBeast.h, Em301LShade.h, Em202Spider.h, Em001Necromancer.h): los `vector<RoutePlace_unk*>` acoplan a todas las unidades que los emiten (renombrar los tres simbolos a la vez). (enlazar2-20261117 qZ)
- GameEventScript fn_803A2378 / fn_803A2210 / fn_803A7D94 (97,3-97,8 %): `SetNext_unk(cmd, wait)` con `wait` variable; el `stw` de `mWait` sale entre `lis`/`addi` de `lbl_8074ED24` y el original lo pone tras el `addi`. 10 variantes en fn_803A2378 (expandir la inline a mano, tabla/indice/referencia en varios ordenes, `bool`, ternario, ifs anidados, dos llamadas): sin cambio. (secuencial-20261119)
- Vistas de globales que no se pueden quitar sin cambiar codigo: `fn_8006EF48` con los argumentos en el orden de su definicion (Loader, NandManager, MenuCommonTask), `TextTable_unk` en CharaTask (fn_80149A30/fn_8014C540) y StrapTask (vf14), `typedef TextTable_unk` en MP/MpCampMsgTable_unk.h (PacketPack fn_804F50DC), `RainParam_unk` en Em302Kraken. (secuencial-20261119)
- Pasar a miembros las `extern "C"` con `GameTask* task` de game/GameTask.cpp (`fn_8036DBC0`, `fn_8036DDD4`, `fn_8036E624`, `fn_8036E6D4`, `fn_8036FE74`): `fn_8036DDD4` cambia registros y, solo por declararlas en `GameTask.h`, GameEventScript cambia la pila de `fn_8039B2BC`/`fn_8039B7B0`/`fn_8039CF8C`. No repetir sin mirar el presupuesto de inlining de GameEventScript. (secuencial-20261119)
- `CharaTask::mF1C` (matriz que sigue el personaje) como puntero: compila, pero arrastra los constructores `ControlCaught(Chara_unk*, int, ...)`, `ControlMatHold(Chara_unk*, int)`, `ControlThrownAway/Avoid(Chara_unk*, int, ...)` y sus envoltorios de CharaTask (simbolos mangled). Hacerlo de una vez con los renombrados. (secuencial-20261119)
- `extern "C"` con `self` llamadas desde la propia unidad (EnemyQuarkStand `fn_80353EE4`/`fn_80353F0C`): como miembros MWCC las inlinea en sus llamadores y cambian 3 funciones; solo convertir las que se llaman desde otras unidades o cuya llamada no se inlinea. (secuencial-20261119)

## datos-20261119

- GameConfig_unk ctor: el 255 de TitleConfig_unk vive en r30 hasta el rojo del primer color (el `li` del rojo se elimina); sin forma conocida. (datos-20261119 qA)
- SoundManager fn_800CECF0 (99,82 con el parche): GPR de `off`/`sound` de los niveles 5 y 6/7 por debajo del original. (datos-20261119 qA)
- NetManager fn_8050CFC8: recarga de `lbl_8087F628->m260` en el comando 5 y `== 1` de IsJoined sin forma. (datos-20261119 qA)
- StrToInt_unk<wchar_t> (atn/Util_unk 0x800DC1E0): la lectura `s[i]` de la condicion del bucle sale por un puntero inductivo aparte (el original usa el mismo desplazamiento que el cuerpo, `lhzx`); la instancia de float de la misma forma si sale bien. Es lo unico que falta para enlazar Util_unk. (datos-20261119 qA2)
- D3Anime vf14: ademas de lo apuntado en qH/qR2, `Frustum&`, frustum/esfera declarados arriba (4 ordenes), `static inline` en las dos inlines, `const Sphere* s`, `Vector3 + mPos`: 99,80 o peor. (datos-20261119 qA2)
- PostFxDamage fn_800AB774 (99,77 con el parche): las ranuras de pila de los GXColor siguen el orden inverso de declaracion (k1/k0/k); declarar k primero sin inicializar o con una inline que construye el color descoloca todo el marco (89-98 %). (datos-20261119 qA2)
- Patron repetido `mChara->m570 = x;` seguido de una copia de un Vector3 de la pila: el original pone la direccion del vector en r3 y mChara en r4; nosotros al reves (ControlChaseFollow vf0C, ControlJump ctor, ControlChant vf0C `m08 = mChara->mPos + FrontDir_unk(...) * 0.1f`). Ninguna forma probada lo da (ver notas anteriores). (datos-20261119 qB)
- ControlDive vf0C 99,89 (parche): pos/type de la llamada a fn_803C1560 en r30/r29 (original r29/r30); 261 formas de la inline y 72 de locales con nombre (todas las permutaciones, con/sin `chara`, `ret` despues): ninguna. La regla "registros por orden de declaracion" no se cumple aqui (las 24 permutaciones dan lo mismo). (datos-20261119 qB)
- ControlCannonLift vf10: el objeto del `new` (ctor de ControlCannonTarget inlinado) toma r31 antes que this/ret; en el original queda el ultimo (r28, compartido con el `new` del functor de boost::function). (datos-20261119 qB2)
- Em308ParmCrab/Em308InvertCrab/Em202SpiderBase vf80 y vf14: orden de los huecos de pila de los temporales invertido frente al original (ver NONMATCHING); ~12 formas en esta pasada. (datos-20261119 qC)
- Color con operadores (`(target - cur) * 0.15f + cur`) de las aranas/cangrejos: el original pone el producto debajo de la resta en la pila y la resta de `a` en f31; ni la forma de los operadores (miembro/friend, ctor/local/copia) ni inlines estaticas lo reproducen (datos-20261119 qC2).
- Em403WarLion fn_80327EF0: orden de operandos de un `fmuls` conmutativo (`(a / abs) * K`); el orden del fuente no lo cambia (MWCC lo canoniza). (datos-20261119 qC3)
- Em314Ivy vf14 / CrowdManager vf14: color elegido con rama despues de todos los argumentos float de `DrawHelper::fn_80062C8C`; ~25 variantes entre lotes. (datos-20261119 qC4)
- Em201ZanGurgFinal vf40: orden `lwz r5` / `mr r3` / `lfs f1` antes de `fn_8015D8C0`. (datos-20261119 qC4)
- Familia vf3C de NewCampParts (Hint/Status/Option/Item, 96,9-98,4): desplazamiento del bucle y direccion de la entrada cruzados r6/r7; ni `m50.mData[i]` en cualquier combinacion ni las formas ya probadas lo cambian. (datos-20261119 qD)
- SelectMember/LibraryTask ctor: el original cruza list/offset al reves en cada unidad; sin forma que mueva los dos. (datos-20261119 qD)
- Numeracion de registros salvados permutada sin cambio de estructura (NewCampPartsOption vf20 98,87 con el parche). (datos-20261119 qD)
- ArenaTask vf10: cruce del cociente de `rand() % n` (y el indice del erase) con el chara cargado para `rot`; ~35 formas (ver arriba), sin idea nueva. (datos-20261119 qD2)
- ActionActorBehavior fn_8054FBE0: el 0 copiado del bucle de actores (`mr r4,r6`) depende de la forma del clear de la rama else (iterador `first` de `const list&`): las formas que arreglan los registros del clear quitan la copia. Ver NONMATCHING. (datos-20261119 qE)
- FriendRoom fn_804BA350 (con el parche): `msg` coalescido con `msg * 8` (r28) donde el original los separa (r30/r28). (datos-20261119 qE)
- ActionActorBehavior fn_8054FBE0: el 0 copiado (`li r6,0; mr r4,r6`) solo sale con un clear por `const_iterator` en la rama else, que es justo lo que cruza los registros de ese clear; 32 combinaciones de la rama else lo confirman (binario const/no const). Ver NONMATCHING. (datos-20261119 qE2)
- Cockpit fn_804B5B10: el original no reutiliza en el cuerpo del bucle el global cargado en la condicion; ninguna forma del getter, de la condicion ni del global lo reproduce. (datos-20261119 qE2)
- gmk/AutomaticDoor fn_803F53E0 (99,90 con el parche): r5/r6 instancia/temporal de `target - pos`; probados ademas accesores por referencia de la camara, `Dir(cam)`, `Dir2(t, p)`, `SetDir(dir, ...)`, `VecAssign_unk`, `-=`, negacion, `atn::Camera&` local: igual o peor. (datos-20261119 qF)
- gmk/ReinforcePoint vf10 (99,89 con FindPlace2): `to`/`from`/global rotados; probados `Scene_unk()`, `DescPos_unk` por referencia, `CopyPos_unk`, ctor de copia, table antes, inline con la busqueda y la tabla dentro: igual o peor. (datos-20261119 qF)
- gmk/OnOff vf0C (99,95 con MaxScale_unk): `rot` de los dos case despues de los dos `mtx` en la pila; `ApplyRot_unk` inline (97,0) o `MakeRot_unk` por valor (98,9): peor. (datos-20261119 qF)
- gmk/OnOff fn_803F8324: `mtx(mMtx)`, `mtx = mMtx`: igual (99,16). (datos-20261119 qF)
- gmk/LandMine Start: `SetSphere_unk(c, p, const float&)`, radio en local, esfera antes/despues de m78, `zero` local: igual o mucho peor. (datos-20261119 qF)
- gmk/FallingTower fn_8041044C: `float s = sin(..)`, `sin * amp`, angulo en local: igual o peor. (datos-20261119 qF)
- gmk/MotionObject vf24: locales antes, `int size`, `void* data`, `field` fuera del do, orden model/fade/sw: igual o peor. vf10: orden prevEfp/prevColli, declarados fuera del if, inline de la llamada de fn_80097C08 (struct o argumentos en orden del struct): igual o peor. (datos-20261119 qF)
- gmk/SwingObject vf10: `Hide_unk(this)` inline, `SetLodRef_unk(Model&)`, `SetLodB_unk(ModelBase&)`, `DecLod_unk`, local `model`: igual o peor (98,53 como mucho con el cast en lod/SetLod). (datos-20261119 qF)
- gmk/BulletPoint vf10: `FindPlace2_unk` (98,44-98,52): peor. (datos-20261119 qF)
- gmk/PassBoard vf0C: setter `SetPair_unk(Pair&/Pair*, obj)`: igual (bloqueo del cursor de la reduccion de fuerza). (datos-20261119 qF)
- gmk/OpenBoard vf10: `player->mNext` real: igual (99,76). (datos-20261119 qF)
- (ver NONMATCHING) (datos-20261119 qG1)
- (ver NONMATCHING) (datos-20261119 qG1b)
- Ninguno nuevo. (datos-20261119 qG2)
- Registros salvados del disparo `fn_800FAB80` con `hit` de `mChara->m638` (QuarkStand y Ghoul): las formas inline/locales/orden de declaracion/parametros solo permutan r27-r29; no se encontro la del original. (datos-20261119 qG2b)
- 13 funciones de CharaTask (fn_80154EC4, fn_801551AC, vf74, fn_80155790, fn_80155A88, fn_80157444, fn_8015783C, fn_80157C34, fn_8015802C, fn_801588A0, fn_801562A0, fn_80151448 y parte de fn_8013CB68/fn_80140588/fn_801426A4) tienen el codigo identico: solo difieren los `R_PPC_NONE` que objdiff deduce desde la base `lbl_8077A720` (el original resuelve base+off a `@72840`+12/+24, nosotros a tres `@N` distintos). Ya documentado (solo-20261103): no se arregla renombrando. (datos-20261119 qG3)
- `fn_8017C9AC`: orden de las ranuras de pila de los temporales Vector3 (target-eye el mas bajo en el original); sin forma con inlines, VecAssign ni nombres. (datos-20261119 qG3b)
- Command 20 (`np_type002::vf20`, em_scout/em_type106 vf28, wait_reaction_minus vf0C/vf34): probadas 24 permutaciones del orden de asignacion y 24 del de declaracion en la inline, `const char*` como parametro, inicializacion en la declaracion, `d_ = c_ = b_ = 0`, `&lbl[off]`, array agregado, locales sueltos: mejor 99,47, sin 100. (datos-20261119 qG4)
- `lis` de la cadena antes que el del global (fr_warppointescape vf18, fr_runaway vf24): `Msg_unk(const char*)` con mChara dentro (tres formas), locales `chara`/`name` en ambos ordenes, `CanUseItem_unk` a un local: sin cambio. (datos-20261119 qG4)
- fn_8001C088: bucle anidado `int[5][5]` (5 x 5) tambien se desenrolla entero. (datos-20261119 qG4)
- fn_8001E730: `NextNode_unk` inline (incremento del iterador) baja a 99,22. (datos-20261119 qG4)
- Familia `SetNext_unk`: ~55 variantes mas (ver arriba), en la funcion y en la cabecera; sin idea nueva. (datos-20261119 qG5)
- Copia implicita de `atn::Camera` inlineada (bucle de `mPlanes`, src/dst cruzados): fn_803A11D8 y Event/Path. (datos-20261119 qG5)
- (ver NONMATCHING) (datos-20261119 qG5b)
- `fn_8039CCD0` con `ScriptEndOf_unk(m94)`: baja a 45 % (la llamada recursiva deja de inlinearse igual). (datos-20261119 qG5b)
- `fn_803A5384` 98,48 (miembro/puntero r5/r7): `SetSide_unk`, array en local, `old` en local, `const`, `const int&`, `reinterpret_cast`, `while`, `i` fuera: sin cambio. (datos-20261119 qG5b)
- `fn_803A372C` 97,23 (orden z/y/x de la resta de vectores): delta/target en locales, `-=`/`+=`, orden de operandos: igual o baja. (datos-20261119 qG5b)
- BattleTask fn_80109F7C: arbol de comparaciones de un switch de 4 valores + rango hasta 0 (ver arriba). (datos-20261119 qG6)
- Slots de pila de AimAt_unk (EnemyQuarkStand, 5 funciones): ver arriba. (datos-20261119 qG7)
- Em203GreaterDaemon fn_8028CFE0: almacenes muertos en la pila (0.0 en 0x14/0xB0, `400.0f / frames` en 0x24) en el bloque `m08 <= 50`; sin forma conocida (regla 2 prohibe locales sin usar). (datos-20261119 qG7b)
- PacketPack `~PacketPack`/`~PacketPackMP` (98,99/99,52 con `erase` y `void* buf`): r26/r27 nodo/valor y r28/r29 fin/cero cruzados en el bucle de liberacion; 7 formas del bucle (datos-20261119 qG8)
- PacketPack fn_804EFE98 (91,45 %): el original pone el buffer en r6 y `&ret` antes de los argumentos; 8 formas de la llamada a `GameVoice::fn_803E9B7C` (datos-20261119 qG8)
- PacketPack ctor de PPPlayer_unk (97,34 %): inicio/fin del bucle del array de 32 `PPEquipSlot_unk` en r6/r5 (datos-20261119 qG8)
- Ranuras de las copias de argumentos GXColor por valor en dos llamadas seguidas (0x8/0xc al reves): PostFxBloom fn_800A981C y PostFxDamage fn_800AB774 (ver arriba lo probado). (datos-20261119 qH)
- D3Anime vf14 99,80: r29/r30 de la esfera y el frustum (el par que comparte registro con la matriz); frustum antes, declaraciones arriba, inlines estaticas de matriz/visibilidad, accesores: igual o peor. (datos-20261119 qH)
- (sin nuevos; ver NONMATCHING) (datos-20261119 qR)
- PassBoard vf0C: el cursor reducido `this + 8*i` de `mPairs[i]` toma el ultimo registro en el original (como un local declarado tras `name`); ninguna forma de la tienda/bucle/inline lo mueve (ver arriba). (datos-20261119 qR2)
- OnOff vf0C: orden de pila case0.mtx, case4.mtx, case0.rot, case4.rot en el original (agrupados por nombre); el nuestro sigue el orden de declaracion. (datos-20261119 qR2)
- Ninguno nuevo; los probados arriba se suman a los ya documentados. (datos-20261119 qR3)

## mixto2-20261120

- ItemSlotAnime `Alloc_unk` (5 funciones): 40 combinaciones mas sin cambio (ver NONMATCHING); no repetir sin una idea nueva. (mixto2-20261120 qA3)
- GameScene Grow/Resize<int/EventBox/...> (99,7-99,9): solo nombres: el original apunta a las cadenas `""` de otra copia debil (lbl_8087D6E8/EC, fuera de la unidad); no se arregla en la unidad. (mixto2-20261120 qA3)
- EffectSystem fn_8022C3F0: el `lwz` de `m1AC` antes de la rama del maximo solo sale con una inline bool de la condicion, que materializa el bool (mfcr/extrwi.). (mixto2-20261120 qA3)
- `__sinit` de las tablas `ParamDef_unk` (Label, Channel, Cut, Sequence): el `lwz r31` del epilogo sube por encima del store del ultimo hash; ningun MWCC Wii/GC ni flag de optimizacion lo reproduce en un fichero suelto, y ninguna funcion del DOL fuera de estas cuatro lo hace (mixto2 qB1, ~20 formas mas sobre las ~26 de cierre99 qJ). (mixto2-20261120 qB1)
- Em202_01_BSpider vf0C: el original empieza un registro nuevo en el tercer `|= ` de mStatus.m18; ni intercalados ni constantes combinadas ni retornos tempranos lo dan. (mixto2-20261120 qC5)
- Producto `abs * 0.75f` con la constante segunda (Em401 fn_803209E0, Em403WarLion fn_80324A38): solo sale si la constante viene de una inline; sin forma natural encontrada. (mixto2-20261120 qC5)
- Em309Ghoul vf10: el original solapa en la pila los buffers de sprintf con los locales de las inlines; bloque, `if` en vez de `return` e inline estatica no lo reproducen. (mixto2 qC6) (mixto2-20261120 qC6)
- EnemyQuarkSecond fn_803412AC: el `m97C.Set_unk` de los enemigos (bloqueo conocido). (mixto2 qC6) (mixto2-20261120 qC6)
- em005 vf3C: `b` inalcanzable tras el `return` de los ids; switch (3 formas), if vacio, `||` invertido, goto: igual o peor. (mixto2-20261120 qC7)
- Em203GreaterDaemonRedMp `atn::Keyboard::Press`: `lwz mD90` antes de `li r31, 0`; int/bool, `||` en return, if anidado, local de mD90, `return 1/0`: igual o peor. (mixto2-20261120 qC7)
- Em203GreaterDaemonRedMp fn_8029E950 y LastCocoonQuadruped fn_804655AC: cruce r3/r4 de copias `__VecCopy` que VecAssign_unk, const&, getter por valor ni puntero a la copia arreglan. (mixto2-20261120 qC7)
- Em202ForestSpider fn_80277DC4: FPR de las constantes izadas (0.0f en f31 frente a f29); `DegToRad_unk(90.0f)`: igual. (mixto2-20261120 qC7)
- TeamAIManager `TeamTargets_unk::Find_unk` (OnTarget/OnTarget2 91,4): el original no tiene `mulli` y suma base + desplazamiento solo en la rama que devuelve; puntero local lo calcula antes, indice saca `mulli`. (mixto2-20261120 qC8)
- GameScene::FindPlace_unk (copia debil 0x800EC204 96,9 y Em206EggMonster fn_802C653C): orden i/desplazamiento/mData de los registros; 7 formas del bucle en la cabecera sin cambio. (mixto2-20261120 qC8)
- GameMessageList fn_803B35FC: el original mantiene `size / 2` en el registro de `size` hasta el `SetData` (CSE con el ctor de CsvU); sin forma natural. (mixto2-20261120 qC8)
- Em403WarLion vf24: el dt f28/f29 (mismo bloqueo que QuadrupedTask/Em401 vf24); no reabierto. (mixto2-20261120 qC9)
- Em309Ghoul ctor: la busqueda propia en cualquiera de las 5 llamadas no mejora (96,6-96,97). (mixto2-20261120 qC9)
- GameVoice Stop_unk: personaje en r4 y m1154.m00 en r3 (el original al reves) en 5 funciones; invariante con 12 formas (SetLip_unk, referencia/puntero a Chara1154_unk, local, const, ifs anidados, `> 0`, `!keepChara`, orden de la comparacion). (mixto2-20261120 qD3)
- GameVoice id de voz (`VoiceId_unk`) en r3 y `mr r4, r3` donde el original lo deja en r4: ternario, if/else, return temprano, local, sin inline: igual. (mixto2-20261120 qD3)
- D2AMpMenuLobby fn_804BEDB8: el original relee lbl_8087F610 tras `m508 == 0` (accesor static inline, local short, `Session_unk()`: igual). (mixto2-20261120 qD3)
- CosEditItemList MslMedian3 (4 instancias): vb en f3 (original f1): Abs_unk, local de la diferencia, orden va/vb, `const float&`, punteros locales, `vb < va`: igual o peor. (mixto2-20261120 qD3)
- TutorialTask vf0C (texto/layout r26/r27): SetTaskText_unk plantilla con layout local / hash local / envoltorio: igual o peor. (mixto2-20261120 qD3)
- D2AMpMenuLobby `FindMember_unk`: offset del bucle y miembro devuelto en registros cruzados en todas sus llamadas; 6 formas de la inline sin cambio. (mixto2-20261120 qD4)
- D2AMpMenuFriend fn_804B9014: texto en r24 en vez de r16 (desplaza el resto un registro), invariante a la posicion de la declaracion. (mixto2-20261120 qD4)
- Bucle de la base del buffer de realloc de `vector<shared_ptr>` (Section/Channel/Cut/Sequence push_back, 99,90): `n`/`&pn` cruzados y el `delete` lee `mData` por el `this` del buffer (r30); ~40 formas. (mixto2-20261120 qE3)
- `delete` del buffer de realloc de `vector<shared_ptr>` por `lwz r3, 0x0(r30)` en Section/Channel/Cut (Sequence, mismo template, por `0x30(r1)`) y ~vector<shared_ptr<Cut>> leyendo por `r25`: ~40 formas de delete/destructor y 255 combinaciones de niveles, nada. (mixto2-20261120 qE4)
- Event::Manager fn_8047EA34: carga del contador del anillo hoisteada fuera del bucle de borrado (el original la recarga); ni `operator!=` por referencia, ni `mSize` directo, ni `it++`. (mixto2-20261120 qEM)
- ControlThrustForGhoul vf0C: tres zonas (f4/f5 de `m34`, r4/r5/r6 de la primera rama, r27-r29 del disparo) salen con la asignacion invertida frente al original; ninguna forma local las cambia (ver arriba). (mixto2-20261120 qG12)
- GameUI ctor: con `m30C.clear()` antes de `m3D8 = 0` (orden de stores del original) MWCC usa un cero nuevo para 0x3d8-0x3f0; con `m3D8` antes, los stores salen cruzados (99,986). (qG1c) (mixto2-20261120 qG1c)
- GameUI fn_803E2A30: el `li r5, 0` del indice de `SetValue3_unk` va tras el ternario float en el original; ninguna forma de ternario/local/inline lo mueve. (qG1c) (mixto2-20261120 qG1c)
- CharaTask fn_8016D3F8 y fn_80176E30: reproducidos en un fichero suelto; ninguna forma cambia los registros (ver arriba). (mixto2-20261120 qG3)
- CharaTask `IsSameSideHit_unk` (fn_80178BFC, fn_80178668) y fn_8015061C: coloracion de a/ta/tb/bool interior; 6 formas. (mixto2-20261120 qG3b)
- CharaTask `AttachWeapons_unk` (fn_8014EF48/fn_8014DEE4): copia de la identidad con destino/fuente r4/r3; tampoco con una inline de un nivel mas `(dst, src)`. (mixto2-20261120 qG3b)
- fn_8001E730 99,76 %: nodo del bucle en r30 frente a r31 (coloreado compartido con la raiz del clear inlineado); ~30 formas. (mixto2-20261120 qG4)
- Constante 0 sacada del bucle como copia del 0 inicial de `i` (`mr rX, r_i`) y una copia por cada uso (ctor + store explicita, o guardada en la pila): CharaMarkers_unk x3, fn_8039B7B0, fn_8039B2BC, UpdatePoints_unk x3. El original carga `li rX, 0` aparte y una sola vez; `unsigned int i` fuera, `int i`, inline para el bloque: sin cambio. (mixto2-20261120 qG5)
- fn_8039AB10: `mfcr r0` + `extrwi r23, r0` del bool `facing` (8 formas). (mixto2-20261120 qG5)
- Cero copiado del contador en bucles con una copia `asm` de Vector3 (CharaMarkers_unk, fn_8039B7B0, fn_8039B2BC, UpdatePoints_unk): el original tiene la copia `asm` en el bucle y aun asi carga `li r30, 0`; probado (banco y unidad): tipo de `i` (int, long, unsigned long), `i` fuera, `while`, bucle en una inline, copia en inline/condicional, `asm` de una sentencia, `__asm`, `asm` de funcion (no se inlinea), cero en variable (sale `li` pero en un registro de variable, r25/r26, no el r30 de temporal), marker antes del cuerpo, `m00` por ternario (max sin salto), `register i`, `M m = M()`. (mixto2-20261120 qG5b)
- vector<string> push_back: registro del puntero del almacen nuevo frente a la capacidad (ver el parche). (mixto2-20261120 qG5b)
- SkipBlock_unk: contador/recuento r4/r5 (8 formas). (mixto2-20261120 qG5b)
- BattleTask fn_80109F7C: con el parche, registro de `res` frente a los punteros de induccion (99,74). (mixto2-20261120 qG6)
- BattleTask fn_8010A4A8: orden de operandos/registros de un `fadds` (99,96 con el parche). (mixto2-20261120 qG6)
- BattleTask fn_800F8574: destino de dos `fmuls`/orden de operandos de un `fadds` (99,955 con el parche shots). (mixto2-20261120 qG6b)
- BattleTask fn_80108C10: `bne; b` del retorno temprano y recarga de lbl_8087F610 (98,9). (mixto2-20261120 qG6b)
- atn/Model fn_80098DD8 y otras: la cadena "spine" sale en otro offset de `@stringBase0` (faltan 17 cadenas del pool); no cierran hasta escribir lo que falta. (mixto2-20261120 qM)
- atn/DrawHelper fn_8005E838/F36C/FD80/607C0: el original reserva 0x20 para ComDrawTextureMask (MiniMapTask::vf10 necesita 0x1C, parche qK). (mixto2-20261120 qM)
- atn/GraphicsDevice fn_80071E04/fn_800713D4: cruce f6/f7 del viewport inline (fn_80075F58) tambien con `fn_80075DEC(device)`; mover `mVtxFormat = 0` detras baja. (mixto2-20261120 qM2)
- Puntero cargado frente a direccion de un temporal de pila en r5/r6 o r3/r4 (AutomaticDoor, ChaseFollow, ControlJump, ControlChant): reproducido en un fichero suelto (`dir = inst->a - inst->b`): con operator-, `Vector3(...)` por componentes, inline `Sub_unk`, `VecAssign_unk`, local con nombre o `RenderManager*` local, el temporal siempre toma el registro mas bajo y se define antes de la carga. Sin forma que lo invierta. (mixto2-20261120 qN1)
- PBoneBasic vf10 99,974: marco 0x10 mayor que el original con todas las ranuras iguales (ver arriba). (mixto2-20261120 qN1)
- `StrToInt_unk<wchar_t>` 92,74: 16 formas mas del bucle sin cambio. (mixto2-20261120 qN1)
- EnemyQuarkFirst vf100/vf108: comparacion flotante muerta que el original conserva (`fabs(57.3*diff) >= 0` de TurnToCheck) y MWCC quita en todas las formas probadas. (mixto2-20261120 qN2)
- NewCampParts* vf3C: offset del bucle y direccion de la entrada en r6/r7 invertidos; ~20 formas sin cambio. (mixto2-20261120 qN2)
- Em204MulueMp fn_802B55B0: el grupo {&to, &tmp, n} sigue coloreado antes que {i, this, desplazamiento, cero, cadenas}; 10 formas mas (inlines del disparo y de la normalizacion, getters del global, ctor del disparo). (mixto2-20261120 qN3)
- SoundManager fn_800CECF0: `off` del nivel 5 (r26, original r30) y `sound` del 6/7 (r25, original r26): 3 formas mas. (mixto2-20261120 qN3)
- ControlQuarkSeirenAttack vf0C (99,40): hit antes que chara, fx antes o despues de `at`, sin local fx, `chara` en el primer argumento, `type` local, orden de done/motion: igual o peor (el original da a chara/hit/fx r29/r30/r28). (mixto2-20261120 qN4)
- Node fn_8053A2E8 (89,3): `ActionKey_unk* keys = &mKeys[0]` para todo salvo la comparacion del bucle (84,0) y ademas la comparacion por un getter `static inline KeyTime_unk(this, i)` (84,0): MWCC sigue juntando las cargas de `m20`. (mixto2-20261120 qN4)
- WaitObject vf60 (93,33): getter `static inline CurState_unk(this)` para m54 en el bucle o en la primera prueba: igual. (mixto2-20261120 qN4)
- OnOff fn_803F8324: `self` local, `MtxPtr src` local: igual (99,16). (mixto2-20261120 qN4)
- ControlJump ctor: con `Chara_unk* chara = mChara;` antes o despues del store de m570, o reasignado: igual (99,86). (mixto2-20261120 qN4)
- PlayerParam fn_8021921C: `level` sin inicializar, `entry` local, `found` antes del rango (96,3), `(unsigned)player > 6`: igual o peor. (mixto2-20261120 qN4)
- SpText fn_8021B2F8: 24 permutaciones de las declaraciones de data/length/section/count: la actual es la mejor (99,49). (mixto2-20261120 qN4)
- OptionParts fn_8020CC64 (parche qO de enlazar2, 99,96): `operator=` explicito, `mStr`, puntero a la capa, `std::string& path` en las tres sentencias (99,81: orden de los addi bueno pero r22/r23 cruzados). (mixto2-20261120 qN4)
- WaitObject vf40: id/blend/a/b en locales en orden de memoria, solo blend o id+blend: igual (99,28). (mixto2-20261120 qN4)
- SwingObject vf10: LOD con los campos de ModelBase (`mModel.m20C_23_16`, sin GetModel_unk) 98,53 (el addi del modelo ya va en su sitio; falta la segunda carga de 0x300 del original), `lod--` despues del store 98,63, SetLodB/SetLodP inline: igual. (mixto2-20261120 qN4)
- ControlJump ctor (ControlMountle): `p = &out`, `const&`, `__VecCopy`, `Assign_unk`, `if (!found)`: igual o peor. (mixto2-20261120 qN4)
- PlayerParam fn_8021921C: indice `i + player * 32` o `index` local en fn_80219160: igual o peor. (mixto2-20261120 qN4)
- Node fn_805389B8: sin local `manager`, `param`/`id` locales, getter `Manager_unk(this)` static inline, `channel` local: igual o peor. (mixto2-20261120 qN4)
- OnOff fn_803F8324: `off` por `__VecCopy`, `=`, puntero/`const&` a mBoxPos: igual (99,16). El `lfs 0x97c(r31)` del original es el `lfs 8(src)` del asm con la direccion plegada: sale solo si &mtx queda en r3 (dst de la copia). (mixto2-20261120 qN4)
- OptionParts fn_8020D688: `name` local (99,84), llamada dentro (99,44), `strlen` en local, resultado+break, `!strncmp`: igual o peor. (mixto2-20261120 qN4)
- SpText fn_8021C3F4: `text` local en el primer bucle, `n` local, `++sTextCount`, orden de length/sTextCount, if sin continue: igual (99,88). (mixto2-20261120 qN4)
- ControlCharge vf10 (con el parche de qB, 99,73): retorno temprano `if (m30 == 0)` (99,02), `else`, `new` dentro del bind en el ultimo caso (96,99): igual o peor. (mixto2-20261120 qN4)
- PacketPack fn_804CF640: la segunda busqueda de PPSetTargets_unk relee `lbl_8087F610` pero reutiliza la cuenta de la primera; con el getter en cada uso se comparte la cuenta pero no se relee el global (98,4 -> 99,0). (mixto2-20261120 qP)
- PacketPack fn_804F2040/fn_804F2258/fn_804F2450 (PPFindTeam_unk): orig contador r3 y manager r5; probados referencia, parametros cruzados, manager como parametro, global dentro de la inline, sin local `player`: igual o peor. (mixto2-20261120 qP)
- PacketPack fn_804EFE98 (91,5 %): el original usa r6 para el buffer (el registro de `data[1]`) y pone `addi r3` (retorno de SoundRef) antes; probados struct `PPMsgVoice_unk`, local `SoundRef`, getter en los argumentos (60 %). (mixto2-20261120 qP)
- PacketPack fn_804E6DD0 (99,85 %): `own`/`head` cruzados tambien con punteros `const`. fn_804EAA84: orden `index`/`player` peor (99,87). fn_804DC858: `PPFriendList_unk()` en el local igual, sin local 92 %. (mixto2-20261120 qP)
- PacketPack fn_804F1564 (99,84 %): punteros de huecos y manager en r31/r28 (original r30/r31); probadas 6 formas de declarar `mgr`/`slots`. (mixto2-20261120 qP2)
- PacketPack fn_804F2994: la recarga del manager entre las dos busquedas sale con `PPManager2Ref_unk()` solo en la segunda, pero el registro va a r5 (r7 en el original). (mixto2-20261120 qP2)
- PacketPack fn_804F3BAC: orden de los registros guardados (manager de la busqueda en r28, r30 en el original) tras arreglar recargas y la comparacion. (mixto2-20261120 qP2)
- ControlThrustForGhoul vf0C: las tres zonas invertidas (f4/f5 de `m34`, r4/r5/r6, r27-r29 del disparo) siguen igual con locales, inlines y casts (ver arriba). (mixto2-20261120 qS1)
- ControlMatHoldDamage vf0C: el orden de r28-r31 (m, this, p, c en el original) no sale con ningun orden de declaracion ni ambito de `c`. (mixto2-20261120 qS1)
- push_back inline a profundidad par (dentro de otra inline: D2AMpMenuFriend `Rebuild_unk`, NewCampPartsHint `ReadList_unk`): con locales de usuario el reparto tamano/capacidad alterna con la paridad; las formas con temporales de CSE no alternan pero dan el orden contrario salvo con el camino lento delante. (mixto2-20261120 qV)
- push_back inline donde el original relee la capacidad en el camino lento (Event/Manager fn_80485028, ~2 % de los sitios): la forma nueva no relee; una relectura (CSE) rompe los demas. (mixto2-20261120 qV)
- push_back<T> no trivial fuera de linea (AddLight_unk, atn::Light): los huecos de pila de los dos calculos de crecimiento (muerto en `__expand_by_cap_unk`, vivo en `__grow_cap_unk`) salen en bloques y orden interno al reves; un nivel mas en el muerto solo invierte su orden interno. (mixto2-20261120 qV2)
- vector<string> push_back inline (GameEventScript fn_803AB96C/PrintLog_unk/fn_8039BF04, MapLift vf20): el registro del almacen nuevo tras `__nw` y del inicio de `clear()` no se mueve con ~30 variantes locales; solo leer `mCap` en la variable de la capacidad lo mueve, a costa de juntar capacidad vieja y nueva (bien en MapLift, mal en fn_803AB96C). Banco minimo descrito arriba. (mixto2-20261120 qV3)

## codigo-20261120

- GameScene `FindPlace_unk` (copia inline): registros de indice/desplazamiento (~17 variantes) (ChaserNpc fn_80181D94 y otros).
- Em203GreaterDaemonRed/RedMp fn_8029C564/fn_802A4194: contador y desplazamiento del bucle del mas cercano (r29/r30); declaraciones, `int`, helper `fn_801C3910` de la gemela Em203GreaterDaemon: igual.
- D2AMpMenuLobby `FindMember_unk`: registros volatiles de la busqueda (vf0C, fn_804BEDB8); formas por indice, `found` o `m5E4 + j` bajan todas las funciones de la unidad.
- PacketPack fn_804F2994: falta la relectura de lbl_8087F610 antes de la segunda busqueda; con `fn_800F7F90()` en la segunda sale pero cambian los registros.
- EnemyQuarkStand fn_80350BF0 (99,86): huecos de pila de las dos ramas del salto lateral; Cross inline, `+=`, `-20.0f`: igual o peor.
- BattleTask fn_800F8574 (99,6): `rot` y el vector del bucle intercambiados en la pila; sin `rot`, `v` fuera del bucle o por campos: igual o peor.
- Em101LittleZesya fn_8025D90C, ChaserNpc fn_80181D94, GameScene fn_803CB820: la expresion `GetNormalize().ToRotation_unk()` baja (95 %); ahi el original si tiene el local.
- em005 vf3C: `b` inalcanzable tras el retorno de la prueba de id (5 formas, ya anotadas).

## grandes-20261120

- CharaStatus fn_8012F440 (15,5 KB, 99,851 con VecAssign_unk/GetFlag22_unk/GetModel_unk: VecAssign_unk de pos, GetFlag22_unk, GetModel_unk; no guardado): queda el bucle de bits con `fn_8013322C` inlineado (constantes 0/1/4 en r9/r10/r0 frente a r10/r0/r6); `1u << i`, `unsigned i`, sin `bit`, `int bit`: igual o peor.
- ChaserGuard fn_8017D13C (99,9926 con `ToArgb2_unk` `(r<<16)|(a<<24)|(g<<8)|b` y `ToU8r_unk(const float&)`): quedan r0/r4 de `a<<24`/`r<<16`.
- Em309Ghoul fn_80301DDC (99,81 con `/ 2.0f`): solo permutacion r28/r29/r30 de los canales de `Color4f_unk::ToU32`; orden de la expresion, `const float&`, una sola expresion: igual o peor.
- RenderSystem_unk Sort_unk<RenderEntry_unk> (dos copias, 1132 B): registros del MinElement inlineado; orden de locales, `T* it` en el for, comparador sin `const` o con if/return: igual o peor.
- EnemyQuarkSecond fn_8033D750, EnemyTasha vf10, DamageCalc fn_8003EA3C (orden de `addi r5/r6`; local del estado, inline con CharaTask*, `GetStatus_unk()`), D2AMpMenu fn_804B1528 (99,968 con `FindPane` inline; queda el puntero del pad r29/r28), SkillEffect fn_8059A298 (`mr r3, r26` de la matriz; puntero local, asignacion, inline con puntero), CompositeIntensityTintMulti fn_800D8894 (this/command r29/r30; copia local, referencia, lista derivada), MenuCommonTask fn_804A5824 (firma con `float size` segunda corrige la carga de `size` pero desordena `lhz`/`li`/`lfs`: baja), WaitObject vf40 (`bool`, referencia, puntero al modelo), GameEventScript fn_803A26F4 (6 ordenes de `mBC`/`flags`), GameChoiceWindow fn_803B3D2C (cuerpo en inline: baja), MapPartsBase push_back<MapArea*> (la cadena "vector length error" la pone el AddLight_unk inlineado de ParseScene; con `mLights.push_back` directo no se mueve).
- LastCocoonQuadruped fn_80462038 (99,63): solo el `li r31, 1` de keepRot (con la declaracion arriba el registro es el del original pero el `li` sube); 62 posiciones, `bool`/`unsigned`, firma de fn_8004D388 de Em403WarLion: igual.
- RouteWalker_unk fn_80127EF4: el `!=` materializado (subfic/or/srwi.) antes de la prueba; inlines `(v & f) != f` / `== f` con mascara parametro (la que cerro QuadrupedTask) dan `cmplwi` directo.
- CharaTask fn_8013655C (99,89 con `equipLoading` tras `equips1`): indice/puntero r28/r27 del bucle de m6A4; `operator[]`, `int`, puntero, local del elemento, `i` fuera: igual o peor.
- CharaTask fn_8013CB68 (14,7 KB): los retoques de la familia (k, angle, y, m584) suben 98,45 -> 98,62 pero quedan ~780 lineas (this en r27 frente a r31: un GPR guardado mas); los retoques no se guardaron (rehacerlos: ver la familia en las notas).
- Em203GreaterDaemon vf10 case 17: dos temporales de `m151C * (1/len) * dt` en otra posicion de pila; locales con nombre, asignacion, `rate`: igual.
- Em306GurgBeast vf10 (98,50 con dos intercambios del barrido): huecos de pila de top/base; orden de declaracion, `Set_unk`, campos, `GetTrans_unk`: igual o peor.
- GameTask fn_803729B4 (99,99 con dos locales de un solo uso como expresion: `dir.fn_80011410(Matrix3x4::fn_8013A13C(rot.y))` y la motion): solo el nombre de la llamada `size()` de `vector<EnemyTask*>`: el original llama a la copia debil 0x800E0AA8, que symbols.txt llama `fn_800E0AA8` y diez unidades declaran `extern "C"`; renombrarla a `size__Q23std19vector<P9EnemyTask>CFv` exige pasar esas diez a `list.size()` (no hecho por tiempo).
- DamageCalc fn_8003F538 (99,88 con el barrido): orden de carga/operandos de `data->m2C * attacker->mParamB.m04` (el original carga `data` primero y multiplica al reves) y r30/r31 de `damage`.

## barrido-20261121

- ShopUpGrade fn_80598530 (95,96): el original lee el texto de la tabla con las dos formas de direccion (`lwzx` sobre base+4 = `table->mStrings[id].mPtr` directo; `add`+`lwz 4` = `this` del elemento en una inline `c_str()`/`get()`); con `mStrings[id].c_str()` (c_str en `ptr_string_base`) 96,30 / 96,55 (`mPtr == 0 ? vacio : mPtr`) y solo quedan esas 8 lineas, pero MWCC junta las dos lecturas en cualquier forma escrita (directa+get(), directa+c_str(), if/else, Get_unk de palabras 97,87 con otro indice): ~20 variantes. (barrido-20261121)
- Em301LShade fn_802D57B8 (99,26): rama else en una inline `PlayEffectAt_unk(efp, model, const Vector3& pos)` (el color va en la ranura de la inline) + `float k = m540.x` -> 99,72 (parche en scratch lshade_9972.diff); quedan los registros float del 13*k (el original tiene m540.x en f1), la copia de MotionBlurParam (r3/r9) y una ranura de head. (barrido-20261121)
- Em204Mulue fn_802A8BBC (99,58): `float ry = rot.y;` (barrido, kind ld) -> 99,974; queda solo `frame` en f20 (el original lo funde con la constante @13566 en f21; nosotros con `angle`): frame sin local, `end` aparte, `!(<)`, orden best/found, angulo en pasos: igual o peor. (barrido-20261121)
- Em203GreaterDaemonRed fn_80296DB8 (99,78): `atn::VecAssign_unk(m1530, point - mPos)` arregla el r3/r4 de la copia (99,92); quedan el `cmpw` de m1528/mIndex y las ranuras de los temporales de la interpolacion de mPos (LerpV_unk, local d, orden de la suma: peor). (barrido-20261121)
- LastCocoonQuadruped fn_80468534 (99,77): plateau + vassign (`VecAssign_unk(v, dir)`) 99,92; quedan r28/r29 de &dir/&v (v fuera del bucle, Mtx+PSMTXRotRad, VecAssign del resultado: igual). (barrido-20261121)
- chr/ControlDropDown ctor (98,04): `VecAssign_unk` en la copia de m574 (barrido) -> 99,37; queda mChara en r4 (original r6): local `c`, `chara` en vez de `mChara`: igual o peor. (barrido-20261121)
- `GameCameraModeParam_unk`/`m4D8` del modo de camara (Em101ElderZesya fn_8025922C, EnemyJill fn_80330D74): el original guarda `&field->m4D8` en un registro (`addi r5, r6, 0x4d8`) mientras carga los parametros por defecto; puntero local, `split` del local de la escena: igual. Mismo caso que `m97C.Set_unk` (inline miembro). (barrido-20261121)
- Pasadas sin resultado: `!= 0.0f` -> valor de verdad en todo el juego (getsweep, 128 sitios); `vassign/byval/cref/split/ld` con plateau sobre las ~120 funciones cuyo NONMATCHING habla de cruces r3/r4 en copias (solo decimas); plateau sobre las 99,8-100 % (63) y 99,5-99,8 %. (barrido-20261121)
- CosEditBase fn_801CDE14 (99,50): plateau (cswap de tres pruebas `mChara[i]->mFace == member->mId`, `next` partido) -> 99,80; quedan los `cmpw` (el original carga mFace primero y compara (mId, mFace)): `!(a != b)`, inline `HasFace_unk` con varios parametros: igual o peor. (barrido-20261121)
- GameTask fn_803729B4 / `size()` 0x800E0AA8: la copia que quedo es la fusion del enlazador (code merging) de los `size()` identicos de vectores distintos (`vector<int>` de IntVector_unk, `vector<MapLocator_unk*>`, `Count4_unk`...); con el nombre `size__Q23std19vector<P9EnemyTask>CFv` GameTask llega al 100 % pero bajan 12 funciones de 10 unidades (en Em206CocoonMp, `list.size()` real, la copia debil nueva cambia el inlining de toda la unidad: ctor 100 -> 0). Revertido. (barrido-20261121)

## bloqueos-20261122

- `m4D8`/`m97C` (modo de camara y mirada de lbl_8087F430): no reproducido en fichero suelto (`try.sh`): puntero/referencia local, inline libre por referencia/puntero, inline miembro dentro/fuera de la clase, doble test de fn_8037E89C, dos niveles, getter del puntero, autoinline de funcion no inline, parametro por valor, `register`, global directo, `Val_unk(&...)`: MWCC siempre pliega el desplazamiento. El original tiene `this` como valor propio (usos de otro bloque basico por el registro, `0xc(r5)` en el asm de la copia) y une los dos stores de m04 de la mirada (`b; b; li r0, 0; stw r0, 4(r5)`); if/else, ternario, `== true`, int intermedio, `const bool&`, `m154C != 0`: no. 12 funciones con `addi rX, rY, 0x4d8`. (bloqueos-20261122)
- `Loader::Get_unk<T>`: predicado por inline (`HashOf_unk`, `Val_unk(core->m48)`, `Val_unk(mHash)`), operandos al reves, `Val_unk` en begin/end/hash de Find_unk: 99,70. (bloqueos-20261122)
- Em204Mulue fn_802A8BBC (99,974): `frame` en f20; plateau 6 (200) y `val`: nada. Em203GreaterDaemonRed fn_80296DB8 / RedMp fn_8029F3FC (99,97): pila de la interpolacion de mPos (resultado debajo de sus temporales); local con nombre, inline LerpTo_unk, `switch`, orden de la suma: no. LastCocoonQuadruped fn_80468534 / Em403WarLion fn_8032A3D4 (99,92/99,93): r28/r29 de &v/&dir; `v = dir`, constructor, puntero, inline con parametros al reves, `operator=`, `val`/`valb`: no. (bloqueos-20261122)
- Em403WarLion fn_80329ED8 (99,25): solo `Val_unk(c)->m560 == 4` (copia del puntero por una inline identidad) da el 100 %; accesor con puntero o referencia, inline `IsChanting_unk`, local `state`: no. No guardado (decision del usuario). (bloqueos-20261122)
- Em101ElderZesya vf10 (98,25): `BackOf_unk(yaw)` y `Matrix3x4 t` tras `offs[3]` dan la pila del original pero 98,03 y queda `m97C`. Em101ElderZesyaSecond vf10 (99,94, solo pila): color en una inline y prueba de mControl en otra dan casi toda la pila, pero esa inline materializa el bool (99,28). EnvSeLine vf14 (99,93): hueco de 4 bytes bajo pa/pb en el original (orden de declaraciones, local del color, en la inline DrawName_unk: no). SkillEffectManager fn_8059D3B0: los dos bloques iguales en una inline estatica: 99,92 (peor). Em206CocoonMp vf114: locales de Die_unk/NotifyIce_unk en orden inverso (orden de definicion, `else if`: no). (bloqueos-20261122)
- ControlChaseFollow ctor de std::string en ControlKnockdown ctor (r26/r29 de path/strlen dentro de `std::string(const char*)`): cabecera compartida, no tocado. CharaStatus fn_80134800 (99,96): r0/r4 de skip/m540; `Val_unk`, inline con el puntero, `int skip = ... && ...`: peor. DamageCalc fn_8003F1E4/fn_8003EA3C: orden de `addi r5/r6` de los `&x->mStatus` (accesor, local, ambos): igual. em005 vf3C: el `b` muerto tras el return (goto, if anidado, switch, llamadas en cada rama): no. QuadrupedTask vf24 (y gemelas): dt en f29; orden de declaraciones, weight partido: no. (bloqueos-20261122)
- NewCampParts vf3C (Hint/Item/Option/Status, 96,9-98,4 %): el cursor de m50 y el elemento cruzan registros; `m50.mData[i]`, `(m50 + i)`, referencia, punteros locales, `continue`, `!show`, `show ? 0 : 1`, bool local, `Val_unk` en cada lectura: el mismo codigo (bloqueos-20261122).
- atn/Task fn_800D32B8 (y fn_800D2D7C, que la inlinea): cualquier cambio de los accesores (FirstOf_unk/NextOf_unk) en ella o en fn_800D3B74 cambia el inlining y baja a 89,5 (bloqueos-20261122).
- CharaTask fn_8014EF48: con el cuerpo en la propia funcion la pila es la del original, pero el cruce r3/r4 de las copias de Matrix3x4 no cambia con `MtxAssign_unk`, `__MtxCopy`, `IdentityMtx_unk()` ni el puntero por inline (bloqueos-20261122).
- Mirada de la camara (GameCamera::m910..m934, `this` materializado y `flag` sin plegar): probados ademas la funcion sin `inline` (static), definida despues de los llamadores, struct en m910 con metodo fuera de la clase y una sonda de la regla R5 (inline pequena dentro de una inline que rechaza sus candidatas): MWCC siempre sustituye `this`. (accesores-20261126)
- TextAnimation fn_800DAB68: el `mullw` invertido resiste diez formas (locales, `*=`, casts) y el destructor 0x8000D844 se llama PathLengths_unk (con `Event::PathLengths_unk times` sale el destructor pero no el constructor por defecto). (accesores-20261126)
- DamageCalc fn_8003EA3C / fn_8003F1E4: el original prepara r6 (estado del objetivo) antes que r5: accesores, locales y orden de declaracion no cambian nada. (accesores-20261126)
- Em202 (Spider, BSpider, ForestSpider, SpiderBase) vf14: el original usa f31 y un marco 16 bytes mayor en la interpolacion del color; operadores de SpiderBaseColor_unk con local o constructor y lerps en inline: igual o peor. (accesores-20261126)
- Em203GreaterDaemonRed fn_80296DB8 (99,97 %): el resultado de la interpolacion de mPos queda por debajo de sus temporales; LerpPos_unk, copia con nombre, `step` con nombre, declaracion separada de `point`: igual o peor. EnvSeLine vf14 (99,93 %): hueco de 4 bytes encima de los extremos; color en un local, punto medio con nombre: igual o peor. (accesores-20261126)
