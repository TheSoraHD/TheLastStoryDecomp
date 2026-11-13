# Cola H (paralelo-20261113): Last Cocoon (Egg, Imago)

## game/LastCocoonEgg

- Escritas (0 % -> escritas, NONMATCHING): vf0C (carga, Csv, nodo "catch_point", lista de personajes
  m19A0 con el motion por personaje, post efectos m2560/m2630/m2700[4]), vf14 (mezcla del post efecto
  m2630 hacia el de la fase m2558; 62 %), fn_804570F4 (atrapa al personaje; 91 %), fn_804576D4 (accion 1,
  lo sujeta en m18BC; 93 %), fn_80457E98 (accion 3, lo lanza en el frame 24; 93 %).
- fn_80457C48 68 -> 98 %: el original pone primero la rama de soltar
  (`if (caught == 0 || !CanAct || m55C != 6 || m560 != 17) { soltar } else { ... }`); lo mismo en 76D4/7E98.
- vf0C: el original esta por encima del presupuesto de inlining (todo llamado: fn_80453C88 = operator= de
  BloomParam_unk, fn_80453DB4 = bloom de RenderSetting_unk, fn_80453DBC = operator= de
  ColorMatrixParam_unk, fn_80453E20 = push_back, Vector4_unk, fn_8045817C). El nuestro lo inlinea todo:
  con un inline por post efecto (4 llamadas) basta una llamada mas para pasarse, pero entonces el inline
  tambien se llama. Con el codigo plano ni seis candidatos mas lo pasan. Queda abierto.
- `LastCocoonEggPart_unk`: campos m04 (float), m08, m0C, m10, m14 (motion 0xB4-0xB8 por personaje).
  `m1904` = `MtxPtr` (nodo "catch_point").
- Etiquetas @14084/@14086/@14087/@14309 (.sdata2 0x80886B78/80/84/90) renombradas a mano: pairdata
  las da por ambiguas (mismo caso que la sesion anterior).
- fnstat: 21 / 42 al 100 % (sin cambio). Sin escribir: 14 -> 9 (quedan las copias debiles de vf0C,
  fn_80454554/64 de vf10, fn_80455638, fn_804562E4).

## game/LastCocoonImago

- fn_8045CB24 (colores de las partes del cuerpo: brillo pulsante con `MIN`/`MAX` de macros.h, que
  reevaluan el seno como el original; fundidos por el frame del motion en los modos 9, 10 y 12): 0 -> 95 %
  (los tres canales empiezan iguales y nuestro MWCC multiplica uno y copia; con un `Vector3` baja a 82 %).
- fn_80459FB8 = `vfBC(void*)`: `return sphere != &m18FC` sale al 100 %, pero anadir el argumento a
  `CharaTask::vfBC` renumera las `@N` (y un `@enum$`) de muchas unidades que incluyen CharaTask.h
  (precommit: lost en chr/ControlAppear, labelcheck peor en ~50 unidades). Revertido; pendiente para una
  sesion con tiempo para `pdbatch.sh` de las 217 unidades.
- fnstat 18 / 36 (sin cambio).

## Decisiones tomadas sin el usuario

- `EggField_unk` (lbl_8087F430) pasa a llamarse `Field_unk` en LastCocoonEgg.cpp, con
  `fn_8013C504()` como miembro (el original llama `fn_8013C504__9Field_unkFv`); como en LastCocoonImago.cpp.
- Copias debiles de otras unidades definidas como `extern "C" inline` en el fuente (fn_80339F74,
  fn_8000D9E8, fn_802A36B0, fn_802A4094, como en TacView/EnemyQuarkSecond) y
  `RenderSetting_unk::GetColorMatrix_unk` definida inline en el fuente (como en GameTask.cpp).
- Operadores `-`, `*`, `+` de `atn::Color` y helpers `EggSub4_unk`/`EggMul4_unk`/`EggAdd4_unk` de
  `Vector4_unk` locales al fuente (el `operator*(Vector4_unk, float)` ya existia y devuelve `EggColor_unk`).

## Hallazgos

- Un `if (!cond) { A } else { B }` con A cayendo detras del salto: si el original pone la rama del
  `else` al final, invertir la condicion (LastCocoonEgg fn_80457C48 68 -> 98 %).
