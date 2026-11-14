# port-decomp-20261114, cola I (segunda pasada de interfaz y camaras)

Unidades: game/GameUI, game/PopupTask, game/TacView, game/GameCamera_unk, game/FinishCamera_unk.

## Resultado por funcion

- `GameUI::fn_803DBD44` (0x803DBD44, 3392 B): 0 % -> **97,75 %** (escrita desde el asm, sin DRAFT).
  Ventanas de la vista cenital: anillos bajo los enemigos (fn_803DCA84 con `mE94[n]`), lista de enemigos
  `mF94` ordenada por la altura en pantalla respecto al lider (`m1014`, insercion con memcpy), ventana de
  grupo de cada uno (fn_803DCD34) y ventana de descripcion del enemigo bajo el cursor (m1140/m1144/m1148,
  boton 0x30, texto de `vfB8` cortado en 5 lineas con fn_800E0000 y "text%d").
  NONMATCHING: el original llama a `CharaTask::vfBC` con un argumento (`BattleTask::m3E2E8` o 0); con la
  firma actual sin argumento MWCC quita esa carga. Resto: la rama else de las lineas de texto
  rematerializa el literal vacio.
  **Firma final:** `void GameUI::fn_803DBD44()` (antes `extern "C" void fn_803DBD44(GameUI*)`, deuda de la
  regla 10); symbols.txt renombrado a `fn_803DBD44__6GameUIFv`.
  GameUI: 136 -> 138 funciones al 100 % (los offsets del pool de las siguientes se arreglan al escribirla).

- `TacView::fn_805AA94C` (0x805AA94C, 2148 B): 0 % -> **97,4 %** (escrita desde el asm, sin DRAFT). Modos 0/4
  con la ayuda de la vista activada (`SetConfig::mTacviewHint`) y `free`: efecto (radio >= 15) o enemigo (2,2 x
  radio) mas cercano en XZ a `pos`, y ventana de informacion `GameUI::fn_803E4A20(hit, link)` (mapa de los ids
  compartidos como GameUI fn_803DE89C); modo 1: mueve el cursor `m48` al slot mas cercano en el sector del stick
  (`fn_805A6D20`, girado por la guinada de la camara) o por los enlaces `mNear` del actual (sector y vecinos).
  NONMATCHING: registros y el orden de la primera distancia del bucle de efectos.
  **Firma final:** `void TacView::fn_805AA94C(atn::Vector3* pos, atn::Vector3* move, int free, int reset)`
  (antes `extern "C"` con `TacView*`, deuda de la regla 10); llamador `GameCamera::fn_80390A88` cambiado a
  `lbl_8087F9F8->fn_805AA94C(...)`; symbols.txt renombrado. GameCamera_unk 46 -> 48 al 100 % (etiquetas).

## Cambios de firma o de layout (para el port)

- `GameUI.h`: `float mE94[32]` (antes `unsigned char mE94[0x80]`): fundido de los anillos de los enemigos.
- `GameUI.h`: `CharaTask* mF94[32]` (antes `int mF94[32]`): enemigos de las ventanas de la vista cenital.
- `TacView.h`: nuevo metodo `void fn_805AA94C(atn::Vector3*, atn::Vector3*, int, int);` (sin nombres de
  parametro; labelcheck igual en las 11 unidades que incluyen TacView.h). Quitada la declaracion
  `extern "C"` de `GameCamera_unk.cpp` (el port debe llamar al metodo).
- `GameUI.h`: nuevo metodo `void fn_803DBD44();` (el port necesita su definicion de `GameUI.cpp`).
- `GameUI.cpp`: incluye `game/BattleTask.h` y `game/HomeButtonMenu.h`; `lbl_8087F048` pasa de `void*` a
  `BattleTask*` (y `fn_8013A194()` devuelve `BattleTask*`).
- Vista local `GameUIField_unk` de GameUI.cpp: `int m56D4` (como `lbl_8087F430->m56D4` de CharaTask.cpp).

## Decisiones tomadas sin el usuario

- No se cambia `CharaTask::vfBC` (217 unidades la incluyen; pendiente de la sesion, ver SIGUIENTE_sesion).
- Etiquetas `@N` de `.sdata2` de GameUI renombradas a mano por direccion (pairdata no las tocaba): solo
  renombrados en symbols.txt.

## Hallazgos

- `if (a != 1 && a != 3 && ... && a != 14 && x == 0) { resto }` da la cadena de `cmpwi/beq` del original; con
  `a == 13 || a == 14` (o `switch`) MWCC lo junta en un rango `subi/cmplwi`.
- `dist[i] = d.y * d.y;` y comparar luego con `dist[i]` da el `frsp` del original (con `key = dist[i] = ...` no).
- Quitar una declaracion `extern "C"` con 5 parametros con nombre renumera (-5) las `@N` de la unidad; pairdata
  no las recoloca todas: compilar el .o de antes y el de despues y renombrar por (seccion, offset) entre ambos.
- Tras `pd.sh`, si `labelcheck` sigue con faltas, emparejar por offset del `.sdata2` del .o (valor igual) y
  renombrar a mano.

## Pendientes

- `CharaTask::vfBC(void*)`: tambien la necesita GameUI fn_803DBD44 (ademas de LastCocoonImago fn_80459FB8).
