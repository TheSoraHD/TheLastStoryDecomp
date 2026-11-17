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
