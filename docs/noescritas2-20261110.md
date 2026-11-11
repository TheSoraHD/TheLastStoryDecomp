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

## Cambios de splits

Ninguno.

## Cambios de symbols.txt (salvo renombrados)

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
