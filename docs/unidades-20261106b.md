Inicio: 2026-11-06 11:36 CET (cierre minimo 2026-11-06 17:36)

# unidades-20261106b: librerias 2010 y unidades que faltan (sesion larga autonoma, tercera)

Rama `unidades-20261106b` desde `main` c6fdafc (13524 / 18117, 652 enlazados). Prompt: `claude/prompt-unidades-restantes.md`.
Referencia guardada antes de tocar nada en `/home/claude/ref/`: `report.json`, `labelcheck` de las 1051 unidades
con `.o` y split (164 lineas) y `rep100` (145 lineas, total 144).

## Resumen

| Que | Antes | Despues |
|---|---|---|
| Funciones al 100% (report) | 13524 | |
| Ficheros enlazados | 652 / 1304 | |

### Unidades y funciones nuevas

| Unidad | Funciones | Notas |
|---|---|---|
| `msc/src/RVL_SDK/kpad/KPAD.c` (matching false) | 22 -> 32/33 | `KPADInsideStatus` 2010 completo (puntero 0x17C, ticks y Motion Plus 0x660-0x688); `reset_kpad(chan)`, `KPADInitEx`, `KPADiConnectCallback`, `KPADiRead`, `KPADiSamplingCallback`, `read_kpad_ext`, `read_kpad_acc`, callbacks WBC y `fn_80658300` (Motion Plus) desde el asm. Falta `fn_80657230` (61%) |

## Decisiones tomadas sin el usuario

- KPAD: campos nuevos de `KPADInsideStatus` con nombres inventados (`mplsBuf` 0x17C, `tickHist[3]`, `prevTick`,
  `curTick`, `cntHist[2]`, `unk_0x676`..`unk_0x67d`, `mplsCallback` 0x680, `unk_0x684`); `KPADStatus.mpls`
  (`KPADMPStatus`/`KPADMPDir` de 0x3C, nombres del SDK posterior) + `unk_0xEC[4]`. Estado WBC 2010 con nombres
  inventados (`kp_wbc_state`, `kp_wbc_err`, `kp_wbc_unk_d2/d5/d6/b0/b8/c0`, `kp_wbc_unk_tbl`); `kp_wbc_enabled` y
  `kp_wbc_state` son `s8`. Prototipos de `KPADMpls_unk.c` (`fn_8065AA50`, `fn_8065AA80`, `fn_8065AEB0`) declarados
  en `KPAD.c` con los tipos que usa `KPADiRead` (el fuente de Mpls aun los declara con `s32`).
- KPAD: `f64 kp_wbc_unk_bss[3]` global (0x18 bytes de `.bss` tras `kp_wbc_average` sin referencias en el `.dol`;
  global para que MWCC lo emita). Estaticos WBC con inicializador `= 0` y en el orden del `.sbss` original (MWCC
  pone primero, en orden de declaracion, los inicializados y despues, al reves, los que no lo estan).
- `WPAD.c` (KEY): los seis stubs `fn_8065CED0`..`fn_8065CF20` eran los `WBC*` debiles del SDK (mismo orden y valores
  de retorno): quitados los `fn_` y los `WBC*` debiles de KEY ocupan su sitio (simbolos renombrados). KPAD los llama.

## Decisiones a revisar

## Cambios de splits

## Renombrados en symbols.txt

## Cambios en libs/

## Hallazgos

## Pendientes

- KPAD (33/33, datos ya en el orden de 2010): probado `matching: true`: el enlazador quita las constantes sin uso
  (200.0f de `KPADSetBtnRepeat`, estaticas de `get_kobj`) y `.data/.bss/.sdata/.sbss` cuadran; **solo** falla el
  orden de tres entradas de `.sdata2` (0x80888920): el original tiene `1/200` y `1/100` antes del `{1,1,1}` de
  `KPADiSamplingCallback`, como si una funcion anterior quitada por el enlazador (entre `KPADInitEx` y
  `KPADiSamplingCallback`) las usara primero. MWCC ordena el pool por primer uso al generar codigo (probado con un
  fichero suelto: una inline no cambia el orden). No he inventado esa funcion: queda `matching: false`.
- `KPADMpls_unk.c` (6/40): libreria Motion Plus de 2010 entera (~10 KB, area de trabajo de 0xDB0 por mando); sin
  fuente de referencia: aplazada. `fn_8065B7F0` es un lerp de `KPADMPDir` + `fn_8065AFB0`; los tres por eje usan
  0x108/0x120/0x138/0x154 + 8 por eje y 0xD80/0xDA4 (+8/+4).
