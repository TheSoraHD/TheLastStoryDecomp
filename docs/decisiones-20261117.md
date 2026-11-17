# decisiones-20261117

Inicio 11:47 (2026-11-17). Base `main` ab092a5: 15974 / 18109, lost 0, rep100 24, labelcheck 11, main.dol: OK.
Decisiones D1-D5 aprobadas por el usuario el 2026-11-17 (`claude/prompt-3h-decisiones.md`). Orden: D1, D3, D2, D5.

## D1. `Traits::length` en `std::string::replace` (parche `docs/escribir-20261116-qN6.patch`): aplicada

- 15974 -> **15976** (+2: atn/Font_unk fn_8006D0C8 y fn_8006F5E8), lost 0, fuzzdrop 0, labelcheck 11, rep100 24.
- Renumera 75 unidades: `pdbatch.sh` en tres tandas de 25; luego `lblmap.py` en 19, `lblfix.py game/CharaTask` solo
  para `.data`/`.rodata` (370 etiquetas: las constantes ptmf de 0xC y dos double desplazadas +8; se descartan las 87
  de `.sdata2` que propone, ya bien emparejadas), `lblseq.py --apply` en Em203GreaterDaemon, Em206CocoonMp,
  EnemyQuarkSecond, EnemyQuarkStand y LastCocoonImago, y a mano Event/Dialog `@11041 -> @11045` (0x80742E98) y
  atn/Model `@26164 -> @26170` (0x80778870). Sin `datafix.py`.
- `precommit.sh` (sin `--pd`, ya re-emparejadas) de las 76 unidades: todo bien salvo `ordercheck` de
  game/GameEventScript (`__dt__15GameEventScriptFv` y `FindWarpIn_unk` extra), que ya falla igual en `main`.

## D3. Nivel base de `std::vector<T*>` (parche `docs/mixto-20261116-qB.patch`): aplicada

- Codigo del parche con `git apply -C1` (contexto de Actor.h cambiado) sin su `symbols.txt` (de una base antigua);
  solo sus tres renombrados de funciones: `__dt__Q23std25vector<P14RoutePlace_unk>Fv` (0x80244674),
  `Back_unk__FPQ23std25vector<P14LaserPoint_unk>` (0x8031E060, `scope:local`) y la copia
  `vector<PQ23chr14CharaEquip_unk>` (0x80544294). No quedaban mas `X_unk : public std::vector<T*> {}`.
- 15976 -> **15978** (+2: dtor de `vector<P17MapModelCache_unk>` en atn/Motion_unk y de `vector<P14MapLocator_unk>`
  en MapPartsGimmickLoc, 96,8 -> 100 %); los dos renombrados siguen al 100 %; la copia de EquipList 88,66 -> 88,89.
  lost 0, fuzzdrop 0, labelcheck 11, rep100 24.
- Re-emparejado: `pdbatch.sh` de 34 unidades (2 tandas), `lblmap.py` en 12, `lblfix.py game/CharaTask` (370 de
  `.data`/`.rodata`, sin las 87 de `.sdata2`), `lblseq.py --apply` en Em206CocoonMp, EnemyQuarkSecond, GameTask y
  MapPartsBase.
- `precommit.sh` de 43 unidades: solo el `ordercheck` de GameEventScript que ya falla en `main`; aviso de
  `poolcheck` en game/ChaserGuard (2.44346, no enlazada; el mismo float que el aviso conocido de CharaTask).

## D2. `atn::Abs_unk` en dos niveles (`include/math/Vec.h`): aplicada

- `inline double AbsD_unk(double x) { return __fabs(x); }` y `Abs_unk(float)` como `(float)AbsD_unk(x)`, como
  `fabsf`/`fabs` de MSL (forma de N4/N7, no el local `double r` de mixto qH2).
- Sin re-emparejar: +5 y -2 (los dos functores boost de chr/ControlAppear, por el nombre `@enum$7516` -> `@enum$7520`
  de `ControlAppear_cpp`, renombrado en symbols.txt). Labelcheck 11 -> 369 lineas, rep100 24 -> 300.
- Re-emparejado: `pdbatch.sh` de 360 unidades (12 tandas de 30, ~1,5 min cada una), `lblmap.py` en 58,
  `lblfix.py game/CharaTask` (370 de `.data`/`.rodata`), `lblseq.py --apply` en 22, `valfix.py`; luego, con
  scripts de la sesion (`/home/claude/scratch`, no subidos): renombrado de las `UNP` de `lblval` con candidato unico o
  con el desplazamiento mas comun de la unidad (30), `lblfix` por desplazamiento solo de las etiquetas que faltan
  (ptmf de 0xC de `.data` y `.bss`: Model, ControlCharge/Damage/StickByQuark, DebugInfo_unk, D2AMpMenuLobby) y por
  valor en PacketPack (45 cadenas `VO_*`, +4) y D2AMpMenuLobby; a mano ControlJumpAt (`@13986-8` -> `@13990-2`),
  PacketPack `.bss` (`@11709/10` -> `@11713/4`), DebugInfo_unk `__arraydtor$8383/89/91` -> `$8387/93/95` y
  GameCamera_unk 0x80885934 `@22701` (pdbatch lo habia dejado duplicado como `@22705`).
- Resultado: 15978 -> **15983** (+5: atn::Abs_unk debil en Event/Puppeteer_CharaTask, Event/Path
  EventCamera_unk::fn_80115704, ctor de ControlGmkBreakArmor en ControlGmkCoffinOpening, Em401_01_WhiteLeoPard
  fn_803227A0, EnemySeaLizerd fn_80355390), lost 0, labelcheck 11, rep100 24.
- Bajadas aceptadas (4, funciones que no estaban al 100 %; registros/orden): ControlSwapAttack::vf0C 95,63 -> 95,51,
  Em202MothersSpider::vf14C 94,65 -> 94,25, Em203GreaterDaemon fn_802915F4 89,38 -> 89,23, Em206CocoonMp fn_802BF5C0
  95,62 -> 95,31.
- `precommit.sh` de 360 unidades: fuzzdrop (las 4 aceptadas) y el `ordercheck` de GameEventScript de `main`;
  avisos de `poolcheck` de Event/DialogHead, EventParser_unk, Manager y Path iguales sin el cambio.
