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
- `dwc_match`, `GameSaveBanner`: no empezadas (tope de tiempo de la sesion tras el reinicio del servidor).

## Unidades que enlazan (bytes)

- `msc/src/RVL_SDK/kpad/KPAD`: 7412 (`total_data` del report).

## NONMATCHING con motivo

Ninguno nuevo.

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
