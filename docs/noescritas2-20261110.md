# noescritas2-20261110

Inicio: 2026-11-10 23:27 (CET). Funciones al 0 % al empezar: 1023 (report.json, main 7c12ef8: 15008 / 18109).

## Decisiones tomadas sin el usuario

- `fn_8006EF48` (ancho de un texto de la fuente, `atn/Font_unk.cpp`): el orden real de los argumentos es
  `(font, size, text, spacing, a, b)` (mismo ABI: GPR y FPR se asignan por separado). Cambiado en todas las
  declaraciones y llamadas salvo `game/MenuCommonTask.cpp` (fn_804A5824 bajaba 98.8 -> 98.2 por el orden de
  `msg->mFont`), `atn/NandManager.cpp`, `atn/Loader.cpp` (usan el truco del parametro delante, que con el orden
  nuevo baja a 96 %), `game/TutorialTask.cpp` y `atn/D2A_Message.cpp` (declaraciones con otro orden propio).
  Ganancia: D2AMpMenu vf0C, D2AMpMenuSetRule vf0C y NewCampPartsTop vf2C al 100 %, varias subidas.
- `atn::net::NetRanking_unk` (definida en `atn/NetManager.cpp` y vista parcial en `atn/PacketPack.cpp`) pasa a
  `include/atn/net/NetRanking.h`; PacketPack usa `mTimes`/`mIndex` en lugar de `m004`/`m750`.
- `MP/D2AMpMenu.cpp`: `atn::net::Manager` no se puede incluir con `MP/D2AMpMenuFriend.h` (los dos definen
  `MpFriendList_unk` y declaran `lbl_8087F628` con tipos distintos): una vista local de la clase con solo
  `fn_80509AF8` (deuda de la regla 10, anotada en el fuente).
- `D2AMpMenu::mEC[25]` se parte en `mEC[2]` (parpadeo de los lados) y `mF4[23]` (cursor guardado por estado);
  el original indexa `0xF4 + estado * 4`.
- `__declspec(noinline)` (excepcion aprobada) en `D2AMpMenu::fn_804B3C78` y en `atn::fn_800B65B4`: el original
  los llama y MWCC los inlineaba.
- `PopupTask`: la clase pasa a `include/game/PopupTask.h`; `fn_8054D794` es miembro y sus cuatro llamadores
  (FlyObjManager, BattleCharaTask, LastCocoonEgg, ControlStickByQuark) la llaman como `lbl_8087F8A8->...`;
  la vista de GameTask.cpp se sustituye por la cabecera. `PopupAnime_unk` tiene la tarea en +4 y los miembros
  de 0x16D0 a 0x17D8 son siete PopupAnime_unk (antes campos sueltos).
- `atn/LinearAllocator.h` y `atn/PostFxManager.h` (vista de PostFxManager con el asignador en 0x48) para
  TextureComposite; `atn/PostFx.h` incluye la vista. Las unidades PostFx* renumeran etiquetas (lblmap).
- `atn/LayerOcclusionCPU.cpp`: los limites de `.sdata` (0x8087D888-0x8087D8F4) definidos con enlace C; los
  ceros van a `.sbss` sin `explicit_zero_data` (solo aprobado en PostFxColorMatrix), asi que fn_800B6218 queda
  al 99.66 por los datos (REP+ en precommit: report.json lo da al 100).
- `RenderSystem_unk::fn_800C169C` llama a `atn::fn_800B6218` (antes `extern "C"`).
- `gmk::ItemSlotAnime::mBits29` (2 bits) se parte en `mFlag29`/`mFlag28` (el original prueba cada bit con
  `extrwi` de un bit); el ctor y gmk/DropItem no cambian. Las tablas de paneles del split (0x807547DC...) y los
  punteros de `.sdata2` (`sArrowPane`, `sColorPane`, `sPosPane`) definidos en el fuente. Cabecera nueva
  `MP/MpSession_unk.h` (lbl_8087F610); CharaListeners_unk y D2AMpMenuCockpit siguen con su vista local (deuda).
- GameEventScript: los miembros 0x64-0x390 retipados con lo que piden ctor y dtor: `m64[3]`, el mapa de eventos
  (0x7C) y el conjunto de terminados (0x138) como `ScriptTree_unk<Node>` (arbol rojo-negro de MSL: tamano, raiz,
  primero; cuatro capas de destructores para las cinco pruebas de `this` y el truco del assert de release en
  `Destroy_unk` para la profundidad del original), `m104` = `SimpleArray<atn::ColliBox>`, cinco logs
  `SimpleArray<ScriptLog_unk>` en 0x354-0x378, `m384` = `std::vector<std::string>`; `ScriptNpcFlag_unk` gana su
  ctor (0x80395794). `fn_80394C68` y `fn_80395848` pasan a miembros (renombrados en symbols.txt; GameScene llama
  `m134->fn_80394C68()`). Las tres copias de 0x803965FC/0x803972A8/0x80397BEC son una plantilla miembro
  `UpdatePoints_unk<Manager>` con instanciacion explicita en su sitio (`template void ...;`).
- `CanStartArg_unk` (GameEventScript): copia en linea del cuerpo de `fn_80398590`, que el original inlinea en la
  plantilla; con `fn_80398590` como envoltorio de la inline, esta bajaba 99.4 -> 98.5 (fuzzdrop), asi que el
  cuerpo queda duplicado.
- Conocido: `ordercheck` da 1 fuera de orden en GameEventScript (`__ct__13ScriptLog_unkFv`, copia debil que el
  original emite cerca de 0x803AB948 y la nuestra tras el dtor). Las copias debiles de los destructores de capas
  del arbol que emitimos de mas no se referencian (el enlazador las quita).

- GameEventScript (cont.): el conjunto de terminados (0x138) es en realidad `std::map<int, bool>` (nodo de 0x14,
  `ScriptDonePair_unk`; el `bool()` del `operator[]` es un temporal constante que MWCC deja en `.sdata`:
  0x8087DD38 pasa a `@N` scope:local); `Done_unk` es su `operator[]` en linea, definida justo antes de
  fn_8039B7B0 para que la copia debil de `FindInsertPos_unk` caiga en 0x8039BC0C. `m98` pasa a `int`.
- `fn_803AB96C` (log de un cambio de flag desde fuera, lo llama GameTask) pasa a miembro
  `GameEventScript::fn_803AB96C(scope, index, old, value)`; GameTask llama `m10D8->m134->fn_803AB96C(...)` en lugar
  del `extern "C"` con el objeto como primer argumento (regla 10).
- `PrintLog_unk` no es copia debil sino funcion normal (con `inline` sus cadenas iban a `@STRING@...` en lugar de
  `@stringBase0`).
- Tablas de nombres del log (`sFlagScopes_unk` 0x8074ECD8, `sValueTypes_unk` 0x8074ECE8, `sFlagOps_unk`
  0x8074ECF8) definidas en el fuente (regla 10). El original las direcciona como desplazamientos desde el inicio
  del `.rodata` de la unidad (lbl_8074E440); como el resto del `.rodata` (nombres de comandos, tamanos, sonidos)
  sigue con `extern`, los inmediatos no coinciden (fn_8039BF04 al 94.5 %). Ademas "NONE" (0x8074E440) va antes
  que las cadenas de la primera tabla: no se ha podido deducir el orden de definicion, se deja.
- `ScriptValue_unk`: `return rand() % value + 1;` en lugar de reasignar `value` (fn_8039BF04 +2 %, nada baja).
- Grupo lexical_cast (fn_80393BEC, fn_80394318, str(), destructores de basic_[io]stream/stringstream y sus
  thunks, fn_803B2780 el `__sinit` de las guardas de locale::id): es `boost::lexical_cast<std::string>(int)` de
  Boost 1.34.1 (lexical_stream con std::stringstream, `unsetf(skipws)`, `precision(10)`). Necesita
  reconstruir istream/iostream/stringstream de MSL (`include/std/` solo tiene ostream y stringbuf): se deja para
  un lote propio.

## Cambios de splits

Ninguno.

## Cambios de symbols.txt (salvo renombrados)

- `@32305` estaba duplicado en 0x80885B30/0x80885B34 (de una pasada anterior de lblmap): 0x80885B30 (1.0f) pasa
  a `@28388`.

- 0x8087DD38 (`lbl_8087DD38`, byte de `.sdata`) -> `@N` scope:local (el `bool()` de `Done_unk`).
- `lbl_8074ECE0` (size 0x44) se parte en la cadena "Rand" (0x5), `sValueTypes_unk` (0x10) y `sFlagOps_unk` (0x2C).
- `lbl_80790B70` (.data de D2AMpMenu, L"%d" + L"") -> `@wstringBase0` (size 0x8, scope:local): el pool ancho
  del fuente.
- `lbl_80734740` (.rodata de TextureComposite, "UnswizzledTemp" + "DxtTemp") -> `@stringBase0`, scope:local.
- `jumptable_807909CC` -> `@NNNN` (local).
- Varias `lbl_` de `.sdata2`/`.rodata` de PopupTask y LensFlare -> `@N` (scope:local) con un `lblmap.py`
  modificado que tambien empareja `lbl_`; ninguna se usa por nombre en el fuente.

## Resultados por unidad

- MP/D2AMpMenu: las 5 al 0 % escritas (vf10 100 %, fn_804AF520 98.6, fn_804B1528 99.5, fn_804B1E48 98.6,
  vf14 99.7); 21 -> 31/41 al 100 %. Quedan 10 por registros (ver NONMATCHING).
- atn/TextureComposite: fn_800D6E48 escrita (63 %, bucle de pixeles).
- game/PopupTask: fn_8054E0FC (99.87), fn_8054D794 (99.15); queda vf14 (9 KB).
- atn/LensFlare: fn_80078938 escrita (77.7 %); la unidad queda toda escrita.
- atn/LayerOcclusionCPU: fn_800B5528 (89 %), fn_800B6218 (99.66, datos); quedan fn_800B4390/fn_800B47AC
  (asm escrito a mano) y fn_800B5994.
- RecycleBuffer_gmk/ItemSlotAnime: fn_8044F96C escrita (99.27 %, registros); quedan fn_80450B84 (2.6 KB, pila de
  14 KB y la ItemSlotTextCache del borrador fn_8044D9BC) y fn_804515A8 (3.5 KB).
- game/GameEventScript: ctor 99.95, dtor 99.45, fn_80394C68 99.77, fn_80395848 93.1, UpdatePoints_unk x3 98.5;
  al 100 % las copias debiles Destroy_unk x2, ScriptLog_unk ctor, ScriptNpcFlag_unk ctor y ColliBox ctor.
  Ceros: 52 -> 41.
  Despues: fn_803AAAE4 100, fn_803A4A7C 99.3, fn_803A7124 88.7, vf10 89.3 (tabla de comandos por frame),
  fn_8039B7B0 92.9, fn_8039B2BC 34.5, fn_8039BDB4 96.7, TalkCharas_unk x3 98.7, CharaMarkers_unk x3 96.0,
  fn_80399148 99.2, PrintLog_unk 98.3, fn_803AB96C 97.2, fn_8039BF04 94.5; al 100 % ademas FindInsertPos_unk x2.

## Hallazgos

- Argumentos en el orden real del prototipo: si el original carga un argumento float antes que otro entero
  (o al reves) en contra del orden de evaluacion, probar a permutar los parametros de distinto banco en la
  declaracion: el ABI no cambia y el orden de evaluacion si (fn_8006EF48).
- Locales muertos que el original pone a cero (`wchar_t text[32] = {0}; MpRect_unk rect;` sin usar) dan su
  marco de pila y pueden ser los de una inline (D2AMpMenu::RefreshRanking_unk).
- Varias constantes float en registros salvados: el orden de los FPR sigue el de declaracion de locales
  `float near = 1.0f; float far = 8.0f; float step = ...` (PopupTask::fn_8054E0FC 97.8 -> 99.9).
- `x - y >= 0 ? x : y` no da `fsel`; `__fsel(x - y, x, y)` si (pero con `frsp` al pasar a float).
- `(int)floor(x)` frente a `(int)(float)floor(x)`: el original redondea a float antes del `fctiwz`.

## Cierre

- La sesion se corto por el entorno el 2026-11-11 hacia las 04:50 (despues de fn_803A11D8) y se reanudo el
  2026-11-12 a las 02:00, ya pasada la hora minima de cierre (11:27): se termino ApproachCharas_unk y se cerro.
- Fin: 2026-11-12 02:30 (CET). Build limpio `main.dol: OK`, regress lost 0, fuzzy sin bajadas.
- **15008 -> 15030 / 18109**; funciones al 0 %: **1023 -> 972**; DRAFT 42 (0 en la ruta). Ninguna unidad nueva
  enlaza.
- Ultimos escritos: fn_80399148 99.2, PrintLog_unk 98.3, fn_803AB96C 97.2, fn_8039BF04 94.5, fn_803A9DBC 99.2,
  fn_803A8470 98.0, fn_803A11D8 98.2, ApproachCharas_unk x3 95.3, CharaMarkers_unk x3 96.0.
- GameEventScript al 0 %: 16 (fn_80399734, 5 KB, y el grupo de `boost::lexical_cast` con stringstream de MSL).
- precommit: fallos conocidos (ordercheck GameEventScript por la copia debil del ctor de ScriptLog_unk;
  REP+ LayerOcclusionCPU fn_800B6218 por `.sbss`). poolcheck: avisos en Event/Manager (255.0), ItemSlotAnime
  (15.0) y GameEventScript (57.2958, 0.815686), de unidades sin enlazar con el `.sdata2` aun incompleto.
