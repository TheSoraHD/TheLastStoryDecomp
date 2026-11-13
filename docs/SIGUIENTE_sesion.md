# Siguiente sesion (tras paralelo-20261113)

> Sesion `paralelo-20261113` (12 h, lote en paralelo con hasta 4 agentes, 23 colas) cerrada el 2026-11-13 hacia las
> 12:40 (inicio 00:19). Rama `paralelo-20261113` revisada y mergeada en `main` (2026-11-13, a08a870).

Estado: **15232 -> 15389 / 18109**, lost 0, sin bajadas de fuzzy, rep100 135 -> 135, `main.dol: OK` (build limpio y
clon limpio). Funciones al 0 %: **550 -> 253**. 38 DRAFT (0 en la ruta). Enlazan 9 unidades mas (131 -> 140).
Splits sin cambios; `symbols.txt` solo renombrados. Notas: `docs/paralelo-20261113.md` (resumen, integraciones,
decisiones) y `docs/paralelo-20261113-q*.md` (una por cola).

## Revision (decisiones de los agentes)

- Deuda de la regla 10 que ha crecido: funciones `extern "C"` con el objeto delante porque las llaman unidades de otras
  colas (BattleTask ~20, AiTarget/ItemManager, atn/Model `fn_800991D0`/`fn_8008E818`, GameCamera
  `fn_805AA94C(TacView*, ...)`, GameTask `fn_8036F268`), vistas locales de `lbl_8087F430` (Quark, Imago, GameCamera) y
  de TacView. Hace falta un lote de integracion que cambie todos los llamadores a la vez.
- `GameCamera` contendria su `atn::Camera` (el dtor la destruye como miembro), no derivaria de ella: sin cambiar.
- Asserts vacios en cantidad para el presupuesto de inline (CosEditItemList_unk: 22 por predicado y 60 en begin/end;
  DebugInfo_unk: 30).
- MSL reconstruida en `include/std/locale_facets_unk.h` e `include/std/istream.h`; `locale::imp` -> `__locale_imp`.
- `atn/PacketPack` (fuera de alcance): solo renombrados dos destructores (cola N).

## Siguiente

- Lotes de cabeceras compartidas (re-emparejar en lote, como la fase 0):
  - `push_back` de `std/vector.h` con la forma del original (inline pequena + dos ayudantes; camino POD por copia
    trivial) y copia profunda de `atn::SimpleVector` (`atn/Array_unk.h`): desbloquea Event/Sequence (19),
    EventParser_unk (3), Event/Manager (3) y el ctor de `CosEditItem_unk`.
  - `CharaTask::vfBC` con argumento (217 unidades): LastCocoonImago fn_80459FB8 al 100 %.
  - Tipos reales de `Event/Actor.h` (`ActorAnim_unk::m00`, `m10/m1C`) (~30 unidades).
  - `D2A_TaskInstance::SetColor255_unk` con `atn::Color` (sube 4 de GameUI); `GameUI::m30C` como `std::vector`.
  - Firmas de `atn/Model_unk.h` (`ModelBase::vf14` con 4 argumentos, `fn_8008D78C`), `GameBgm::fn_8037D4C0`
    (~45 llamadas), struct anonimo de `CharaStatus.h` (270 unidades), constructores de `Inventory.h`.
  - Deuda de la regla 10 de BattleTask (10 + ~11 funciones, ~100 llamadores).
- Cola D2 sin integrar: `docs/paralelo-20261113-qD2.patch` (`do_put(long double)` al 90 %; bloqueada porque
  GameEventScript instancia la misma plantilla y baja).
- `atn/WPad_unk` tiene todo al 100 % pero `matching: true` da `main.dol: FAILED`: averiguar por que.
- Funciones al 0 % que quedan: BattleTask 13 (5-8 KB), Sequence 19 (bloqueo push_back), LastCocoonEgg 8 (6 por el
  presupuesto de inline), Puppeteer_CharaTask 6 (copias de otras familias), GameUI 5, TacView 5, EffectManager 5.
- Cerca de enlazar: `/home/claude/scratch/n3-candidates.txt` se regenera con el script de las notas de N; la cola N
  fue la mas rentable (9 unidades enlazadas en 3 pasadas).
- Siguen bloqueadas: `atn/LH_Motion_cont`, `LastCocoonChrysalis` ctor, `GimmickManager` vf0C, `atn/BoneCalc_unk`,
  `atn/LayerOcclusionCPU` (asm/GQR6 sin aprobar).
