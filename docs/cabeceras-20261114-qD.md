# cabeceras-20261114, cola D (inlines de `atn/Model_unk.h` y `atn/Task.h`)

Rama `agent-D`. Referencia: `$REF` = `/home/claude/ref-D` (15424 / 18109).

## Resultado por cabecera

### `atn/Model_unk.h` (puntos 1 y 3, un solo commit)

- Inline en la clase: `Model::CalcMatrixStack(ModelNodeCallback_unk)` (antes en `Event/Puppeteer_CharaTask.cpp`),
  los setters `fn_80139F24`, `SetMotionSpeed`, `SetMotionFrame`, `fn_8013C554` (antes fuertes en CharaTask /
  EnemyTasha e `inline` locales en ControlMountle), los getters `GetMotionFrame`, `fn_80139F2C`, `fn_8013C3DC`,
  `fn_8013C3E4`, `fn_80147A00` (fuertes en CharaTask, `inline` locales en MotionCommand_unk), `Model::GetMtx_unk`,
  y los constructores de `Model::Node_unk` y `NodeWork_unk` (antes en `atn/Model.cpp`; `memset` ya era visible).
- Todas las copias debiles siguen al 100 % (salen solas detras de la primera funcion que las llama con `bl`).
- `GameTask::fn_80371790` 77,4 -> 79,4 % (los setters ya van inline). Resto neutro: 15424 -> 15424, lost 0, sin
  bajadas, labelcheck/rep100 igual.
- Re-emparejado en lote de las 293 unidades que incluyen la cabecera (`pdbatch.sh`, luego `lblmap.py` en 39 que
  seguian peor y dos etiquetas a mano: `@15264` en 0x80887088 de MapPartsBase, `@34096` en 0x80881534 de
  BattleTask, por el desplazamiento uniforme de la unidad).
- Etiquetas vigiladas: `lbl_8087D6E8/EC`, `lbl_8087EE78`, `lbl_80885CB0`, `lbl_80783CA8`, `lbl_8078CB00/08/10`
  devueltas a mano (pdbatch las habia renombrado). `pairdata` renombro tambien `lbl_80885C98` a `sPath` (global
  duplicado: el enlace fallaba con `multiply-defined sPath`): devuelto.
- `@enum$6816ControlAppear_cpp` -> `@enum$6922ControlAppear_cpp` (8 lineas de `symbols.txt`): el numero cambia en
  el `.o` de ControlAppear al incluir la cabecera; sin renombrar se perdian `invoke`/`manage` de su `boost::function`.

## Decisiones tomadas sin el usuario

- Puntos 1 y 3 en un solo commit: los dos tocan `atn/Model_unk.h` y cada re-emparejado en lote cuesta ~25 min.
- Renombrado del numero del enum de ControlAppear (en vez de devolverlo): el `.o` ya no emite el viejo.

## Para el port

- `atn::Model`: `CalcMatrixStack(ModelNodeCallback_unk)`, `fn_80139F24`, `SetMotionSpeed`, `SetMotionFrame`,
  `fn_8013C554`, `GetMotionFrame`, `fn_80139F2C`, `fn_8013C3DC`, `fn_8013C3E4`, `fn_80147A00`, `GetMtx_unk` y los
  ctores de `Model::Node_unk` / `NodeWork_unk` son ahora inline en `include/atn/Model_unk.h`; quitar sus
  definiciones de `Event/Puppeteer_CharaTask.cpp`, `atn/Model.cpp`, `game/CharaTask.cpp`, `game/EnemyTasha.cpp`,
  `chr/ControlMountle.cpp`, `game/MotionCommand_unk.cpp`. Sin cambios de firma ni de layout.

## Hallazgos

- `pdbatch.sh` con la lista de `ninja -n` pierde las unidades ya recompiladas a mano: sacar la lista antes de compilar
  nada (o anadirlas luego).
- `pairdata --relabel` puede dar un nombre global ya usado (`sPath`) a una etiqueta: el enlazador falla con
  `multiply-defined`; revisar los renombrados que no son `@N`.
