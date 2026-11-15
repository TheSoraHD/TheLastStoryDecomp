# enlazar-20261115, cola O

## Resultado por unidad

- `msc/src/RVL_SDK/kpad/KPAD` (33/33): **enlaza**. El orden de `.sdata2` en 0x80888920 (`1/200`, `1/100`,
  `{1,1,1}`) lo daba una funcion real del SDK que el enlazador quita: `KPADGetMplsZeroDriftMode`, entre
  `fn_80658300` (KPADiControlMplsCallback) y `KPADiSamplingCallback`. Cuerpo de la decomp publica del SDK
  2009-12-11 (robojumper/sdk_2009-12-11, `source/kpad/KPAD.c`) sin los asserts. Pruebas: compila a 0xA0 bytes,
  el tamano del simbolo en otros juegos (riptl/wii-symbols SIIP8P: `len=160`; Etrainquandt17/Whippy la pone por
  el mismo motivo). Usa `0.005f`, `0.01f`, `0.02f`, `0.04f`; las dos ultimas no estan en el original porque el
  enlazador quita las constantes que nada usa (igual que el `200.0f` de `KPADSetBtnRepeat`). Build limpio:
  `main.dol: OK`; poolcheck: solo constantes que el enlazador quita (200, 0.02, 0.04 y las tres de `get_kobj`);
  poolorder sin pool de cadenas.
- `msc/src/RVL_SDK/dwc/dwc_match` (99/101, sin cambio): `DWCi_CloseUnacknowledgedClients` 99,13 % y
  `DWCi_ProcessMatchCommand` 99,995 % (ver NONMATCHING).
- `game/GameSaveBanner` (8/9, sin cambio): `fn_803B9638` 99,18 % (ver NONMATCHING).

## Unidades que enlazan (bytes)

- `msc/src/RVL_SDK/kpad/KPAD`: 7412 (`total_data` del report).

## NONMATCHING con motivo

- dwc_match `DWCi_CloseUnacknowledgedClients` 99,13 %: en el original el `i` del segundo bucle va primero en el
  coloreado (r31) y `validPidLen` despues (r29); en el nuestro al reves. Probado ademas (sin cambio): `i` declarado
  primero con otra variable `j` para el primer bucle (los bucles 2 y 3 con la `i`); `validPid[i]` en vez del local
  `profileID` (peor: `cmpw` invertido); compiladores Wii/1.1, Wii/1.3 (igual) y GC/3.0a5.2 (peor).
- dwc_match `DWCi_ProcessMatchCommand` 99,995 %: en RESERVATION el original pone `matchType` en r26 (= `port`) y
  `topology` en r28 (= `ip` de TELL_ADDR), igual que el nuestro; en TELL_ADDR nuestro `ip` reutiliza r30 (`data`,
  muerto) y el original no, como si `data` siguiera vivo. Probado: inicializador `{ data[0] }` (no vale en C).
- GameSaveBanner `fn_803B9638` 99,18 %: el incremento del desplazamiento de icono (0x1200) va el ultimo en el
  original (tras `i`); probado sin cambio: puntero fuente local, `shift` local, contador `k` aparte para el indice
  del icono (`for (...; i++, k++)`). Los argumentos de `NANDInitBanner` y de `memcpy` se calculan del ultimo al
  primero en el original: mismo fenomeno.

## Cambios de splits

Ninguno. `symbols.txt`: solo renombrados de etiquetas `@N` de KPAD (re-emparejado con `precommit.sh --pd`).

## Para el port

Nada (KPAD no es del port). Funcion nueva `KPADGetMplsZeroDriftMode(KPADChannel, s32*)` que llama a
`KMPLSGetZeroDriftParam` (solo declarada; el enlazador quita la funcion).

## Decisiones tomadas sin el usuario

- `KPADGetMplsZeroDriftMode` con el cuerpo de 2009 (no hay fuente publica de 2010); el tamano 0xA0 coincide.

## Hallazgos (una linea cada uno)

- El enlazador GC/1.3.2 quita de `.sdata2` las constantes que solo usa una funcion quitada; las que usa otra
  funcion se quedan en la posicion en que las puso la funcion quitada (orden de `.sdata2`).
- Para buscar funciones quitadas del SDK: decomps publicas de otras versiones (robojumper/sdk_2009-12-11) y los
  tamanos de riptl/wii-symbols.

## Pendientes

- dwc_match: `DWCi_ProcessMatchCommand` 99,995 % y `DWCi_CloseUnacknowledgedClients` 99,13 % (ver qF/qL).
- GameSaveBanner `fn_803B9638` 99,18 %.
