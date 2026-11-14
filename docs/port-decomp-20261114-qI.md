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

- `TacView::fn_805AC758` (0x805AC758, 5072 B): 0 % -> **90,6 %** (escrita desde el asm, sin DRAFT). Ventanas del
  personaje bajo el cursor: ventana de seleccion (`mSelect`, basePoint 320/220) con los cuatro paneles de orden
  (`mDirs[1..2,0,3]` en select_left/right/up/down, `mHpFlash` en select_center), tensionOn (m228 < 1 -> 0,3),
  fundido `mLayouts.m20 / 12` (invertido si hay panel elegido `m1C`), nombre (`mFace`), PV (nowHP, hpRate y color
  por la tasa < 0,3 / < 0,6), contadores `mStatus.m224`/`m300` (colOn, colR/G/B, frame), y por orden: texto,
  msgLen, skipSelect (`m54.mObj == i`), icono (`fn_803DDD54`), canSelect y frame; con la ayuda activada manda a
  `GameUI::fn_803E4978` las tres lineas "nombre：descripcion". Al final `m20 = clamp(m20 + 1, 0, 15)`.
  NONMATCHING: crecimiento de `std::vector::push_back` (std/vector.h) y detalles de pila.
  **Firma final:** `void TacView::fn_805AC758()` (antes `extern "C"` con `TacView*`); symbols.txt renombrado.
- `std::operator+(const wstring&, const wstring&)` (0x805ADB28, 216 B): 0 % -> **62 %**. Plantilla definida en
  `TacView.cpp` (no en `std/string.h`, para no renumerar); necesita acceso a los miembros privados (un `friend` en
  `std/string.h`) para leer datos y tamano con una sola prueba. symbols.txt renombrado a `__pl<w,...>__3std...`.

- `TacView::vf10` (0x805A781C): 94,50 -> **94,87 %**. `IsPadPressed_unk` devuelve `bool` (el original convierte
  el resultado del ternario con neg/or/srwi); los llamadores sin `!= 0`. Equivalente; resto: registros y la
  conversion a bool en el `||`.
- Revisadas sin cambios (diferencias solo de registros, pila y orden; equivalentes): `GameUI::fn_803DAB4C`
  (96,3 %; `mFrame >= mEndFrame` directo en el caso 3 baja a 96,2), `PopupTask::vf14` (97,3 %).
- `GameCamera::fn_80385B48`: sin escribir. No es una copia de fn_80384188 con otros offsets: usa el bloque de
  parametros m60C (m610 fovy, m624, m60C distancia), lee `chara->mF80`, m12A4, m674 y m538 (rotacion y), y el
  original inlinea `fn_80392964` (fn_8004B378, fn_80392A04, fn_803918EC, fn_803920C8, fn_803928C0, SetCamera_unk),
  `fn_800109E0` (tres RotRad + Concat) y `ToRotation_unk` (atan2), con PSMTX44MultVec: unas 170 diferencias de
  logica frente a fn_80384188; con el mismo problema de presupuesto de inline no pasaria del ~30 %.

- `FinishCamera_unk::fn_8037865C` (0x8037865C): 99,2 -> **100 %** (mnemcheck limpio): la entrada se rellena
  miembro a miembro en vez de con el constructor con argumentos. `fn_80379104` (99,7 %): asgwrap y el orden de las
  declaraciones no lo arreglan.

- `GameUI::fn_803E0E10` (0x803E0E10): 83,36 -> **85,05 %**. `0 < job && job <= 26` (con `job > 0` MWCC junta el
  rango en subi/cmplwi; tambien en las otras cuatro comprobaciones del oficio del fichero, sin cambios en las
  demas funciones) y `chant = 1.0f < chant ? 1.0f : chant;` tras el calculo del canto (el original deja el fcmpo
  del minimo). Pendiente de equivalencia: `chant` no se usa despues en nuestro fuente (el original tampoco lo usa
  en lo que se ve; revisar si falta un panel). El `m560 == 29 || m560 == 30` sigue en rango (switch y orden de
  operandos probados).

- `GameUI::fn_803D83A8` (0x803D83A8): 87,96 -> **88,79 %**: `SetLevelUp_unk` pasa a `static inline`; como
  `inline` con enlace externo sus 13 cadenas iban a `@STRING@SetLevelUp_unk...` en vez de a `@stringBase0`, lo que
  desplazaba 0x84 bytes las cadenas de todas las funciones siguientes. Queda un desfase de 0x14: la cadena
  "vector length error" del `push_back` de `std::vector<wstring>` (std/vector.h, pendiente compartido).

## Cambios de firma o de layout (para el port)

- `GameUI.h`: `float mE94[32]` (antes `unsigned char mE94[0x80]`): fundido de los anillos de los enemigos.
- `GameUI.h`: `CharaTask* mF94[32]` (antes `int mF94[32]`): enemigos de las ventanas de la vista cenital.
- `TacView.h`: nuevo metodo `void fn_805AA94C(atn::Vector3*, atn::Vector3*, int, int);` (sin nombres de
  parametro; labelcheck igual en las 11 unidades que incluyen TacView.h). Quitada la declaracion
  `extern "C"` de `GameCamera_unk.cpp` (el port debe llamar al metodo).
- `TacView.h`: `TacViewLayouts_unk::m1C` es `atn::D2A_TaskInstanceBase<32, 8>*` (panel de orden elegido) y `m20`
  es `int` (frames del fundido), antes `atn::Task*` los dos; nuevo metodo `void fn_805AC758();`.
- `TacView.cpp`: `std::operator+` de wstring definida aqui (el port puede usar la de su STL).
- `HitInfo_unk::m0C` (HitInfo_unk.h, bytes 0x0C-0x10) es el texto de descripcion (`const wchar_t*`); en TacView se
  lee con un cast (pendiente cambiar el tipo en la cabecera compartida).
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
- Construir un struct local con su constructor con argumentos cruza r8/r9 en las direcciones de sus Vector3;
  rellenarlo miembro a miembro tras el constructor por defecto da el orden del original (FinishCamera fn_8037865C).
- Las cadenas de una funcion `inline` no `static` de un .cpp van a `@STRING@<funcion>@N` (fuera de `@stringBase0`)
  y desplazan el pool de las funciones siguientes: `static inline` las deja en el pool (GameUI SetLevelUp_unk).
- `0 < x && x <= N` evita que MWCC junte las dos comparaciones en un rango `subi/cmplwi` (con `x > 0` las junta).
- Tras `pd.sh`, si `labelcheck` sigue con faltas, emparejar por offset del `.sdata2` del .o (valor igual) y
  renombrar a mano.

## Pendientes

- `GameCamera`: el destructor original llama a `__dt__Q23atn6CameraFv(this, -1)` para el objeto en 0: el
  `atn::Camera` de 0 es un miembro, no la base (GameCamera.h dice `class GameCamera : public atn::Camera`); mismo
  layout, pero cambiarlo toca todos los usos (lote propio).

- `std/string.h`: `friend` de `operator+(const basic_string&, const basic_string&)` (y quitar la de TacView.cpp).
- `HitInfo_unk.h`: `m0C` como `const wchar_t*` (descripcion).
- `std/vector.h`: el crecimiento de `push_back` (memcpy de los elementos) baja fn_805AC758 (ya en SIGUIENTE_sesion).

- `CharaTask::vfBC(void*)`: tambien la necesita GameUI fn_803DBD44 (ademas de LastCocoonImago fn_80459FB8).
