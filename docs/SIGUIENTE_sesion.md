# Siguiente sesion (tras noescritas-20261110)

Rama `noescritas-20261110` (mergeada en `main`; inicio 2026-11-10 12:28, cierre 22:40, adelantado):
**14958 -> 15008 / 18109**, lost 0, sin bajadas de fuzzy, build limpio y clon limpio con `main.dol: OK`.
Funciones al 0 %: **1148 -> 1023 (-125)**. DRAFT: 42 (0 en la ruta). Solo renombrados en `symbols.txt`; splits sin
cambios. Notas y decisiones: `docs/noescritas-20261110.md`.

## Primera tarea: terminar `MP/D2AMpMenu` (quedan 5 funciones al 0 %)

Casi todo lo escrito en la unidad esta al 99,x % solo por desplazamientos en `@stringBase0`, en `.sdata2` y en el
pool ancho de `.data` (`L"%d"` + `L""`, lbl_80790B70): faltan las funciones de delante. Al escribirlas, muchas
pasan al 100 % sin tocarlas.

1. `vf10` (0x804B0F58, 928 bytes): `switch (mBC)` de 20 casos (jumptable_807909CC) que llama a las funciones de
   cada estado (`fn_804B12F8`, `fn_804B2934`, `fn_804B1528`, `fn_804B1C88`, `fn_804B1E48`, `fn_804B274C`) y al final
   cuenta `m29A0` hasta `m29A4` (`Work_unk m299C`). Ya esta decodificado en buena parte (ver la sesion).
2. `fn_804B1528` (1888), `fn_804B1E48` (2308), `vf14` (2164) y `fn_804AF520` (5456, cambio de estado: el que mete
   las cadenas que faltan al principio del pool).
3. Helpers ya hechos: `SetTopCursor_unk` (static, cursor + ayuda), `OpenKeyboard_unk`, `Play_unk`/`Stop_unk`,
   `MP::SoftKeyboard` (`include/MP/SoftKeyboard.h`), `MpCampMsgTable_unk` (`include/MP/MpCampMsgTable_unk.h`).
4. Cada cambio de `include/MP/D2AMpMenu.h` pide `lblmap.py` en D2AMpMenuCockpit, FriendRoom, Lobby, SetRule,
   Result, CosEditPalette_unk y atn/SoftKeyboardU (salen como LC+ en precommit).

## Despues

- `game/GameEventScript`: 52 funciones al 0 % (de 224); 101 al 100 %. El resto al 89-99,9 % por el cruce r0/r4
  de `SetNext_unk` y offsets del pool.
- `RecycleBuffer_gmk/ItemSlotAnime` (3 al 0 %), `atn/LayerOcclusionCPU` (5), `game/PopupTask` (3),
  `atn/TextureComposite` (1: `fn_800D6E48` necesita `LinearAllocator`/`PostFxManager` en una cabecera).
- `game/GameCamera_unk`: escribir los callees de `fn_80392964` (`fn_803918EC`, `fn_803920C8`...) antes de
  volver a los modos (presupuesto de inlining R3).
- Unidades a las que solo les faltan funciones sin escribir (de arranque2): `atn/LH_Motion_cont`,
  `game/LastCocoonChrysalis`, `atn/LH_Model`, `atn/LensFlare`, `game/TacView`, `game/RouteWalker_unk`,
  `game/AiTarget`, `game/MapPartsGimmickLoc`, `game/LastCocoonImago` (20), `MP/D2AMpMenuLobby`.

## Pendiente (menor)

- `lbl_8087F1E4` y `lbl_8087EEE0` tienen tipos distintos segun la unidad: unificar antes de poner una cabecera comun.
- BattleTask vf0C (0x800F26D4) necesita retipar `ModelBase::mB8` (callback de nodos).
- ~45 funciones al 97-99 % con `psq_` sin pasar por `asgwrap`; `Loader::Get_unk<T>` (99,7 %) cerraria ocho `atn/LH_*`.

## Decisiones a revisar

- `docs/noescritas-20261110.md` (seccion "Decisiones tomadas sin el usuario").
