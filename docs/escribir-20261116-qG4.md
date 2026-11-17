# escribir-20261116, cola G4 (atn/PacketPack, cuarta pasada: al 100 %)

Inicio: 2026-11-17 06:06 CET (tope 08:36).

## Tabla de la cola

| funcion | antes | despues |
|---|---|---|
| fn_804EE040 | 99,21 % | (en curso) |
| fn_804D8610 | 99,76 % | 100 % |
| fn_804DDE34 | 99,78 % | 100 % |
| fn_804E6518 | 99,84 % | 100 % |
| fn_804EA8B8 | 98,75 % | 100 % |
| fn_804EAA84 | 99,84 % | 99,91 % (queda el registro de `num`) |
| fn_804DD3F0 | 98,81 % | 99,07 % |

## Para el port

- Sin cambios de comportamiento: fn_804D8610, fn_804DD3F0, fn_804DDE34, fn_804E6518 y fn_804EA8B8 pasan
  `GetMyAid_unk()` directamente a fn_804DD158 (antes un local `unsigned int aid`); fn_804EAA84 copia `rot` con
  `atn::VecAssign_unk`.

## Decisiones tomadas sin el usuario

## Hallazgos

- PacketPack: la busqueda inlineada fn_804DD158 con aid/contador cruzados (r5/r4 en vez de r4/r5) era un local con
  nombre para el aid (`unsigned int aid = GetMyAid_unk(); fn_804DD158(mgr, aid)`): pasando la llamada como
  argumento el original sale al 100 % (fn_804D8610, fn_804DDE34, fn_804E6518, fn_804EA8B8); fn_804E8A80 si
  lleva el local (escribir-20261116 qG4)

## Bloqueos

## Cambios de splits/symbols

- Re-etiquetado `@N` de PacketPack (pd.sh + `/home/claude/scratch-G3/bin/fixlbl.py`). Splits sin cambios.

## Scripts

- `/home/claude/scratch-G4/bin/nl.py save|cmp NOMBRE` (tras `m.sh`): cuenta por funcion las diferencias que no son
  solo el nombre de una etiqueta (`@N`, `jumptable_`, `lbl_`): compara variantes sin pasar pd.sh.
- `/home/claude/scratch-G4/bin/m.sh <unidad> <patrones>`: compila y da el % de objdiff-cli (no el de report.json).
