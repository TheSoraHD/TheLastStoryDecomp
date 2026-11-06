Inicio: 2026-11-06 11:36 CET (cierre minimo 2026-11-06 17:36)

# unidades-20261106b: librerias 2010 y unidades que faltan (sesion larga autonoma, tercera)

Rama `unidades-20261106b` desde `main` c6fdafc (13524 / 18117, 652 enlazados). Prompt: `claude/prompt-unidades-restantes.md`.
Referencia guardada antes de tocar nada en `/home/claude/ref/`: `report.json`, `labelcheck` de las 1051 unidades
con `.o` y split (164 lineas) y `rep100` (145 lineas, total 144).

## Resumen

| Que | Antes | Despues |
|---|---|---|
| Funciones al 100% (report) | 13524 | 13553 (punto de control 13:35) |
| Ficheros enlazados | 652 / 1304 | |

### Unidades y funciones nuevas

| Unidad | Funciones | Notas |
|---|---|---|
| `msc/src/RVL_SDK/kpad/KPADMplsMath_unk.c` (nueva, matching false) | 4/6 | rutinas de vectores/rotacion de Motion Plus (`fn_8065AED0`, `fn_8065AF20` movidas de KPADMpls; `fn_8065B630` (producto de rotaciones) y `fn_8065B7F0` (interpolacion) nuevas; `fn_8065AFB0` 90%, `fn_8065B320` 53%) |
| `msc/src/RVL_SDK/kpad/KPADMpls_unk.c` (matching false) | 4 -> 10/16 | area de trabajo de Motion Plus por canal (0xDB0) reconstruida; `fn_80658DF0`, `fn_80659070`, `fn_80659220`, `fn_80659B20/C10/D00` (por eje), mas `fn_8065AA80` 99,8%, `fn_8065A8B0` 98%, `fn_806594D0` 96%; falta `fn_80659DF0` (2752 B) |
| `MP/D2AMpMenuSetMessage.cpp` (nueva, matching false) | 0 -> 10/26 | fabrica, ctor, dtor, `vf0C`, `vf1C`, `vf20`, `fn_804C9360` (oculta las ventanas del estado anterior), `fn_804C9C88` (cursor en rejilla), `fn_804C9EF8` (pagina de mensajes y voz); `fn_804C8D80` 98,4% y `vf10` 99,75% NONMATCHING |
| `MP/D2AMpMenuMessageList.cpp` (matching false) | 7 -> 8/16 | `MpMessageTable_unk::fn_804C54FC` al 100% (inline en cabecera, ver decisiones) |
| `msc/src/RVL_SDK/kpad/KPAD.c` (matching false) | 22 -> 33/33 | `KPADInsideStatus` 2010 completo (puntero 0x17C, ticks y Motion Plus 0x660-0x688); `reset_kpad(chan)`, `KPADInitEx`, `KPADiConnectCallback`, `KPADiRead`, `KPADiSamplingCallback`, `read_kpad_ext`, `read_kpad_acc`, callbacks WBC y `fn_80658300` (Motion Plus) desde el asm. Falta `fn_80657230` (61%) |

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
- KPADMpls: estructuras inventadas `KPADMplsChan_unk` (0xDB0 por canal tras una cabecera de 8 bytes: `m000` y la
  callback), `KPADMplsCalib_unk` (= `WPADMplsCalibration` de KEY), `KPADMplsRaw_unk` (bytes 0x36-0x3D del estado WPAD
  de 2010), tablas `kp_mpls_unk_tbl_a/b`; `__declspec(noinline)` en `fn_80658F50` (el original la llama con `bl`
  desde `fn_806594D0`).
- `WPAD.c` (KEY): los seis stubs `fn_8065CED0`..`fn_8065CF20` eran los `WBC*` debiles del SDK (mismo orden y valores
  de retorno): quitados los `fn_` y los `WBC*` debiles de KEY ocupan su sitio (simbolos renombrados). KPAD los llama.

- `MpMessageTable_unk` (tabla de mensajes de `lbl_8087F610 + 0x2A50`) pasa a `include/MP/MpMessage_unk.h` con
  `fn_804C54FC` **inline**: SetMessage la expande en su sitio (con su llamada recursiva una vez) y MessageList emite
  la copia fuera de linea tras su primer uso. Con el rango `category < 0 || 21 < category` (constante a la izquierda)
  sale al 100% en MessageList. La clase `D2AMpMenuMessageList` pasa a `include/MP/D2AMpMenuMessageList.h` (SetMessage
  usa `sPage`).
- SetMessage: tablas de `.rodata` de la unidad: `sDefaultIds0/1` (`MpMsgIds_unk`, struct de 10 `int` copiado por
  valor a la pila: la copia en dos tandas de registros del original) y `sCharaIds[22]` (ids de personaje de los
  nombres; `GetCharaName_unk` estatica). `MpMsgData_unk`/`MpMsgSession_unk`/`MpMsgSettings_unk` son vistas parciales
  de `lbl_8087F628`/`lbl_8087F610`/`lbl_8087F0A8` como las de MessageList (no hay cabecera comun aun).

## Decisiones a revisar

## Cambios de splits

- `msc/src/RVL_SDK/kpad/KPADMpls_unk.c` se parte en dos: `KPADMpls_unk.c` `.text` 0x80658DB0-0x8065AED0, `.data`
  0x807B8A28-0x807B8A70, `.sbss` 0x808801E8-F0, `.sdata2` 0x80888938-0x808889C0; y `KPADMplsMath_unk.c` (nueva)
  `.text` 0x8065AED0-0x8065B8E0, `.data` 0x807B8A70-0x807B8A98 (rotacion identidad), `.sdata2` 0x808889C0-0x808889D0.
  Prueba: las funciones desde 0x8065AED0 usan otro pool (`@1502` 0.5f, `@1503` 0.0f, `@1508` 1.0f repetidos tras el
  0.0f/1.0f de KPADMpls) y numeros `@N` bajos (fichero pequeno). Build limpio `main.dol: OK`.

## Renombrados en symbols.txt

## Cambios en libs/

## Hallazgos

## Pendientes

- `MP/D2AMpMenuSetMessage.cpp`: faltan `fn_804CA250` (2200 B) y las de 0x804CAAE8 en adelante (15 funciones);
  `vf10` 99,75% (caso 5: texto en r31 y layout en r29; el original r29/r30) y `fn_804C8D80` 98,4%.

- KPAD (33/33, datos ya en el orden de 2010): probado `matching: true`: el enlazador quita las constantes sin uso
  (200.0f de `KPADSetBtnRepeat`, estaticas de `get_kobj`) y `.data/.bss/.sdata/.sbss` cuadran; **solo** falla el
  orden de tres entradas de `.sdata2` (0x80888920): el original tiene `1/200` y `1/100` antes del `{1,1,1}` de
  `KPADiSamplingCallback`, como si una funcion anterior quitada por el enlazador (entre `KPADInitEx` y
  `KPADiSamplingCallback`) las usara primero. MWCC ordena el pool por primer uso al generar codigo (probado con un
  fichero suelto: una inline no cambia el orden). No he inventado esa funcion: queda `matching: false`.
- `KPADMpls_unk.c` (6/40): libreria Motion Plus de 2010 entera (~10 KB, area de trabajo de 0xDB0 por mando); sin
  fuente de referencia: aplazada. `fn_8065B7F0` es un lerp de `KPADMPDir` + `fn_8065AFB0`; los tres por eje usan
  0x108/0x120/0x138/0x154 + 8 por eje y 0xD80/0xDA4 (+8/+4).
