# Siguiente sesion (tras fueraruta-20261112)

> Sesion `fueraruta-20261112` (12 h, unidades fuera de la ruta de arranque) cerrada el 2026-11-12 hacia las 13:00
> (inicio 02:29). Rama `fueraruta-20261112` revisada y mergeada en `main` (2026-11-12, fast-forward).

Estado: **15030 -> 15057 / 18109**, lost 0, sin bajadas de fuzzy, `main.dol: OK` (build limpio y clon limpio).
Funciones al 0 %: **972 -> 870**. 42 DRAFT (0 en la ruta). Ninguna unidad nueva enlaza. Splits sin cambios;
`game_units.json`: entrada nueva `game/CosEditItemList_unk.cpp` (`matching: false`); `symbols.txt`: renombrados,
etiquetas `@N` y dos `@STRING@` de Em203GreaterDaemonRedMp a `scope:local`.
Notas: `docs/fueraruta-20261112.md`.

## Revision

- Revisado al mergear: sin asm/pragma nuevos, sin `extern "C"` mangled, `symbols.txt` solo renombrados (mismas
  direcciones y tamanos), 0 fuera de orden nuevos, ningun DRAFT nuevo.
- Decisiones sin el usuario en `docs/fueraruta-20261112.md` (seccion "Decisiones tomadas sin el usuario").
- Cabeceras tocadas que incluyen muchas unidades: `game/GameCamera.h` (campos y `CameraShake_unk mShake`),
  `game/CosEditBase.h` (`CosEditItem_unk`), `game/PopupTask.h` (firma de `fn_8054E528`), `gmk/SwingPart_unk.h`
  (nueva), `Event/ActionProcessors_unk.h` (nueva), `atn/SingletonNoStatic.h` (destructor vacio).
- `MP/D2AMpMenuLobby`: `lbl_8087F628` es `atn::net::Manager` (antes una vista local); `MP/D2AMpMenuMessageList.cpp`
  aun usa su propia vista (deuda de la regla 10).

## Siguiente

- `game/GameCamera_unk` (33 al 0 %): fn_8038C070 (necesita declarar fn_8038E9B0), fn_8038B650 y fn_80389A00
  (grandes). Los llamadores de la vibracion siguen en 79-99 % por `fn_8008B964` (`extern "C"` en `atn/Model.cpp`;
  el original la inlinea): resolverlo como inline de `RenderManager` sube de golpe ~9 funciones.
- `MP/D2AMpMenuLobby` (8 al 0 %): fn_804C0094 (924 bytes, sencilla; declararla en `MP/D2AMpMenuLobby.h`, que
  incluye PacketPack: pasar `lblmap` a PacketPack y revisar sus etiquetas a mano), fn_804BFB54, fn_804C23E4,
  fn_804BE610, fn_804BEDB8, fn_804C18A8, vf10 y fn_804BC628 (5 KB).
- `game/CosEditItemList_unk` (65 al 0 %): copias de `std::sort` y las inlines debiles; ver la nota de los dos tipos
  de entrada en las notas (no repetir el ctor de protectores).
- Siguen bloqueadas (sin cambio): `atn/LH_Motion_cont`, `LastCocoonChrysalis` ctor, `GimmickManager` vf0C,
  `atn/BoneCalc_unk` (paired-single no aprobado).
