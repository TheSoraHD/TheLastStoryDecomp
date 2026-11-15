# Siguiente sesion (tras ceros-20261115)

> Sesion `ceros-20261115` (10 h, hasta 4 agentes, 15 colas) del 2026-11-15, 01:36 -> ~11:40. Notas:
> `docs/ceros-20261115.md` y `docs/ceros-20261115-q*.md`. Para el port: `claude/port-ceros-20261115.md` (proyecto).

Estado: **15547 / 18109**, lost 0, sin bajadas, rep100 35, labelcheck 16, `main.dol: OK`. Funciones al 0 %: **150**
(26 fuera de PacketPack, iostream, auto_03_* e ipc). 32 DRAFT (0 en la ruta).

## Decisiones pendientes (parches en `docs/`)

- `ceros-20261115-qK.patch`: quitar `ComDrawTextureMask::m1C` (`atn/DrawCommand_unk.h`) pone MiniMapTask `vf10` al 100 %;
  `DrawHelper::fn_8005E838`/`fn_8005F36C` (ya NONMATCHING ~99 %) bajan un poco de fuzzy.
- `ceros-20261115-qL-redmp.patch`: RedMp `fn_802A3D80` a 99,97 (rep100 +1). Mejor: el `~vector<T*>` del MSL original
  (0x7C, 0x80244674) en `std/vector.h`; arreglaria tambien `Em206CocoonMp::vf104` y `GameTask::fn_803729B4`.
- `ceros-20261115-qJ.patch`: `resize` con un nivel de inline mas en `vector.h` (+1 al 100, 4 bajadas de fuzzy).
- `ceros-20261115-f0c.patch`: `static Matrix3x4 fn_8013A13C(float)`, descartado (baja 32).

## Siguiente

- Ceros que quedan (26): Event/Sequence 5, EventParser_unk 1, Manager 2, Puppeteer 1 (dependen de `vector.h`),
  WordWrap 3, SmallAllocator_8 2, LH_Binary 2 (GQR6), LayerOcclusionCPU 1 (GQR6), Em206CocoonMp 2, GimmickManager 2,
  LastCocoonChrysalis 1, RedMp 1, NewCampTask 1 (`RenderConfig::GodRay::operator=`, tipos `Override_unk<>`),
  AI_archer_attack01 1. Ver `docs/mwcc-blockers.md` (seccion ceros-20261115).
- Gameplay por debajo del 100 %: GameCamera_unk 58/90 (ctor vacio de `Matrix3x4`), BattleTask 175/212, CharaTask
  386/436, GameUI 154/181, TacView 39/46, PopupTask 10/14, CharaStatus 43/55, GameTask 113/133.
- `HitMsg_unk::m64..m80` de `CharaTask.h` como `DamageResult_unk`; `extern "C"` sin gestor que quedan en BattleTask.cpp.
- Quitar la declaracion sin cuerpo de `CharaStatus::ClearFlags_unk` y el ctor de 4 argumentos de `GameUIMark24_unk` en
  un lote de cabeceras (renumeran).
- (Hecho en la cola M: constantes de Em401_01_WhiteLeoPard y Em403WarLion corregidas.)
- Sincronizar el port con `claude/port-ceros-20261115.md`.
