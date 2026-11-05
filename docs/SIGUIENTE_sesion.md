# Siguiente sesion (cierre de `unidades-20261105`, 2026-11-05)

Rama `unidades-20261105` (desde `main` f6a9f6e), **mergeada en `main`** (bc4135b): 12710 -> **12766 / 18119** (lost 0, fuzzdrop 0,
`main.dol: OK` en build limpio y en clon limpio, labelcheck 160 -> 149 lineas, rep100 145 -> 144). Notas:
`docs/unidades-20261105.md`.

## Hecho

- `chr/CharaEquip_unk` (nueva): 20/26; clase en `include/chr/CharaEquip_unk.h` (fuera de `CharaStatus.h`).
- `game/TeamAI_unk` (nueva): 11/33; metodos de `TeamAI_unk` y mensajes de la cola en `include/game/TeamAIManager.h`.
- Librerias: `ipcclt.c` 19/21, `ipcProfile.c` 5/6 (version 2010 escrita desde el asm).

## Revisar antes de mergear (decisiones tomadas sin el usuario)

- Unidades nuevas en `game_units.json` (CharaEquip_unk, TeamAI_unk) y en `splits.txt`/`libs_ref.json` (ipcclt,
  ipcProfile, `matching: false`); `libs/RVL_SDK/.../ipcProfile.c` reescrito.
- `AIManager` declarado en `include/game/AiTarget.h`; `TeamEntry_unk` = `TeamData_unk`.

## Propuestas

1. TeamAI_unk: `Move_unk<P>` (3 x 2988 B), `fn_805B27A0` (`std::multimap<float,...>`, 2432 B) y los nodos del arbol.
2. Tramos `auto_` de libreria con fuente parecida: WUD.c (2010, `App_MEMalloc` 0x14), WPAD.c (10 de 23 por nombre),
   KPAD, BTM. Metodo de IPC: unidad `matching: false`, `pairnames.py`, reescribir lo que cambio en 2010.
3. Unidades sin funciones al 100% (todas > 20 KB): Event/DebugInfo_unk (MSL `vector<string>`), NewCampTask, TacView.
