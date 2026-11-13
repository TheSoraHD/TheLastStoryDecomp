# Lote paralelo 2026-11-13, cola D (Efectos)

Inicio: 2026-11-13 01:18 CET. Rama `agent-D`. Unidades: `atn/EffectSystem_unk`, `atn/EffectManager`.

## Resultado por unidad

- `atn/EffectSystem_unk` (commit 1): 0 % 1 -> 0; escrita NONMATCHING fn_8022DD1C (dibujado de un slot,
  7592 bytes, 84,8 %). Toda la unidad queda escrita; al 100 % sin cambios (43/58). No enlaza (15 NONMATCHING).

## Decisiones tomadas sin el usuario

- Empece por fn_8022DD1C y no por EffectManager: la cola de EffectManager son 26 instanciaciones de MSL
  (`num_put::do_put` x8, ~20 KB, `numpunct`, `__put_grouping_float`...) que solo se emiten si se reconstruye
  la plantilla entera de MSL; fn_8022DD1C cierra la unidad.
- `EffectParam_unk::m60` pasa de relleno (`unsigned char[4]`) a `float m60` (distancia hacia la camara) en
  `atn/EffectSystem_unk.h` (67 unidades): solo un miembro de datos, no renumera (precommit OK, fuzzy drops 0).
- `EffectFileEntry_unk::m110[8]` pasa a `float m110, m114` (pivote u/v del quad) en `atn/EffectFileEntry_unk.h`.
- Cabecera nueva `include/atn/EffectVertex_unk.h` con `EffectVertex_unk` (0x14 bytes). `atn/EffectDesc_unk.cpp`
  sigue con su propia definicion en el .cpp (no es de mi cola): pendiente que incluya la cabecera.
- `fn_80236B84`/`fn_80236FFC` declaradas en EffectSystem_unk.cpp con la misma firma que en EffectDesc_unk.cpp
  (ultimo argumento `int`: es la escena; se pasa `(int)scene`).
- `pd.sh` empareja mal 5 floats de `.sdata2` de la unidad (2.0 y los coeficientes -3, 4, 3, 5 de la spline):
  renombrados a mano en `symbols.txt` por valor (0x808830D8, 0x808830F0-0x808830FC).

## NONMATCHING

- `fn_8022DD1C` 84,8 %: marco 0x90 bytes mayor (huecos de pila de las matrices temporales; el original
  parece declarar todas las matrices al principio), registros y orden de la aritmetica de indices del anillo
  de la estela. Probado: angulos por referencia (`const float&`, 81 -> 84,8 %: el original los relee del slot).

## Hallazgos

- Las `@N` de los literales de toda la unidad se renumeran con cualquier cuerpo nuevo (aunque este al final del
  fichero): en EffectSystem_unk, +1201 con fn_8022DD1C; los parametros y locales de las inline cuentan.
- El orden de `.sdata2` de los coeficientes de la spline (-3, 4, 3, 5) sale del orden de las sentencias en el
  fuente (c2 antes que c1).

## Pendientes

- EffectManager: MSL de `ostream << int` (fn_8023BA10) y `num_put`/`numpunct` (ver notas qF2).
- `atn/EffectDesc_unk.cpp`: usar `atn/EffectVertex_unk.h`.
- Orden 0.5/pi del pool (0x808830DC/E0) ya estaba cruzado antes del lote (fn_8022D790).
