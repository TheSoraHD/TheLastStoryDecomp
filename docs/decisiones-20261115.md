# decisiones-20261115

Aplicacion de las tres decisiones pendientes de `cabeceras-20261114` (aprobadas por el usuario el 2026-11-15 00:22).
Inicio 00:22. Base `main` aa39d1e: 15450 / 18109, rep100 138, labelcheck 19.

1. **Parche de `push_back`** (`docs/cabeceras-20261114-qA5.patch`, colas A2-A5): aplicado. 15450 -> 15455 (5 `push_back`
   debiles al 100 %: CosEditItem_unk, CosEditWeaponItem_unk, KrakenCommand_unk, KrakenMpCommand_unk, gs::HideLink2_unk).
   Re-emparejado: `pdbatch.sh` de 291 unidades (la primera vez se corto con `nohup ... &` al parar la conversacion:
   relanzado como tarea en segundo plano), `lblmap.py` en 46, a mano GameCamera_unk 0x80885988 (@22630) y PacketPack
   0x80759740/48; `@enum$7016ControlAppear_cpp`. Bajadas aceptadas (8, registros): DebugInfoColumn_unk copia 99,64 ->
   99,00, NewCampPartsItem fn_80517B3C 99,40 -> 99,07, Event/Manager fn_80484088 95,18 -> 94,97, GameEventScript
   fn_8039F08C 99,97 -> 99,83, WordWrap fn_800E0000, CosEditProtector fn_801D94D0, Em305Vampire/Mp vf0C (<= 0,02).
2. **Cola M** (rama `cabeceras-20261114-m`): solo el fuente (sin su symbols.txt, de una base antigua). 15455 -> 15460
   (GameCamera::fn_80392D2C, copia debil de SetCamera_unk, Camera(const Camera&), NewCampTask fn_8052C6CC,
   Sequence::fn_8053E8D0; fn_8038C5B0 48,6 -> 99,6 %). pdbatch de 245 unidades, lblmap, a mano PacketPack y
   Em206CocoonMp 0x808841DC (@18417). Bajadas aceptadas: NewCampTask fn_8052E1A0 71,31 -> 70,81, fn_8052FA44 73,22 -> 73,12.
3. **Split de la cola E** ("Event::ActionProcessor" a Event/Behavior_unk): ya estaba en `main`; se queda.

Resultado: **15460 / 18109**, lost 0, 10 bajadas aceptadas, rep100 138, labelcheck 19 -> 20 (BattleTask 1 y Em206CocoonMp 3
etiquetas sin nombre en unidades no enlazadas, lblmap no las resuelve), `poolcheck` igual que `main`, build limpio OK.
Funciones al 0 %: 199. Scripts de integracion nuevos en `tools/decomp/`: `symreapply.py`, `symkeepat.py`, `lcworse.py`, `symconf.py`.
