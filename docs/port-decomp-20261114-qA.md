# port-decomp-20261114, cola A (Interfaz: HUD de la partida)

Inicio: 2026-11-14 02:59 CET. Rama `agent-A`. Unidades: `game/GameUI`, `game/PopupTask`, `game/TacView`.

## Resultado por funcion

| Funcion | Antes | Despues | Firma final |
|---|---|---|---|
| `GameUI::fn_803D83A8` (10148 B) | 0 % (sin escribir) | escrita, NONMATCHING 88,0 % | `void GameUI::fn_803D83A8()` |
| `GameUI::fn_803DAB4C` (4600 B) | 0 % (sin escribir) | escrita, NONMATCHING 96,3 % | `void GameUI::fn_803DAB4C(int show, int keep)` |
| `GameUI::fn_803E0E10` (4864 B) | 0 % (sin escribir) | escrita, NONMATCHING 83,4 % | `void GameUI::fn_803E0E10(CharaTask*, GameUIElem60_unk*, int index)` |
| `PopupTask::vf14` (9328 B) | 0 % (sin escribir) | escrita, NONMATCHING 97,3 % | `void PopupTask::vf14()` |
| `TacView::vf10` (7844 B) | 0 % (sin escribir) | escrita, NONMATCHING 94,5 % | `void TacView::vf10()` |
| `GameUI::fn_803E2110` (768 B) | 0 % (sin escribir) | escrita a mano, NONMATCHING 35,8 % | `void GameUI::fn_803E2110(int index, CharaTask*)` |
| `TacView::fn_805A96D4` (2368 B) | 0 % (sin escribir, `extern "C"`) | escrita, NONMATCHING 80,6 % | `void TacView::fn_805A96D4(CharaTask*, int dir)` (miembro nuevo) |

- `fn_803D83A8`: actualizacion por cuadro del HUD (llamada por GameTask): congela todas las partes; iconos de estado
  sobre los enemigos cercanos (`m2788`), medidores de magia (`fn_803E2410` de jugadores y enemigos), lineas de odio de
  la vista tactica (hacia el personaje bajo el cursor, `m15B4`), lineas de punteria de los jugadores hacia
  `AIManager::m458` (`m1170`, `m1174[]`, efectos `m1158`/`m1164`), icono de habilidad usada (`m2564`, cola `m2568`),
  avisos (`m2550`, sonidos `lbl_80750618[7..9]`), contadores sobre personajes (`m25CC`/`m2610`), marcador del objetivo
  (`m2628`, nodo "spine") y marcador de subida de nivel (`m110C`: estadisticas antes/despues desde
  `Inventory::mExpEntries` o `mDF30`, y habilidades aprendidas `m1134`).
  Diferencias: marco 0x50 mayor, copia de colores (el original copia `GameUIColor_unk` campo a campo con lfs/stfs,
  como un constructor de copia), registros.
- `fn_803DAB4C`: iconos de la vista tactica: anillo del cursor que gira (`mDD0`, envuelto a -pi..pi con `fmod`),
  dibujado con `DrawHelper::fn_80064AC4` y los colores del efecto bajo el cursor (`SkillEffectManager`), ventana de
  orden "TARGET?/RETURN?/ORDER?" (`m2544`/`m2548`, estado `m2540`); `keep == 0` llama a `fn_803DBD44`.
  Diferencias: copia de colores (lfs/stfs intercalados), registros.

- `fn_803E0E10`: ventana de grupo de un personaje (`index` -1 = lider): nombre, nivel, PV (el valor mostrado
  sigue al real en 20 cuadros, `m38`), experiencia, PM, barra de tiempo (`m2F4/m2F8`), cara del trabajo
  (`m60->m2C`, UV 1/8 x 1/4) y marcas de subida de nivel y vidas (`m300`, `m224`). Diferencias: rangos `job > 0 &&
  job <= 26` (MWCC los junta en una resta), copia del `D2A_DrawRect`, registros.
- `PopupTask::vf14`: dibuja los popups: texto de cada tipo (0/1 dano y cura, 3, 4 con contorno, 5, 8 numero
  grande) sobre su posicion en pantalla (con `m14 & 0x40000` se mantiene dentro de la pantalla), luego sus animes
  apiladas (subida de nivel `mA4`, dano `m9C`, `mA0`, combo `m94`, elemento `mA8`, critico `m90`, `m98` con
  "string0007", `mAC`) y los avisos de sus banderas (mensajes 0x69, 0x6B, 0x6F, 0x73, 0x75, 0x77, 0x83, 0x85).
  Diferencias: constantes flotantes que el original guarda como double en la pila; registros.
- `TacView::vf10`: actualizacion de la vista tactica (modos `mA78` 0/1/4/5, cuenta atras de cierre `mA7C`, paso a
  la siguiente casilla `mA84`, boton 0x2E cambia `m1590`). `fn_805AA734` (cerrar) pasa a `static inline
  TacViewClose_unk` (lo inlinea vf10; fn_805AA734 sigue al 100 %). Diferencias: copias de `GameCamera::m684` en la
  pila, registros.
- `fn_803E2110`: anade `{index, chara}` a `m30C` si `chara->mType == 3`. El original inlinea
  `std::vector<T>::push_back` de MSL: `m30C` deberia ser `std::vector<{int, CharaTask*}>` (pendiente, ver abajo).
- `fn_805A96D4`: orden de una direccion para un personaje (rellena `m54`: objetivo, golpe, `AiTarget`, orden de
  la IA, estado `PlayerState_unk` 7/11) y arranca el apuntado (`m15D8`-`m15F8`).

## Cambios de firma / layout (para el port)

- `GameUI.h`: `m1134` pasa de `unsigned char[0xC]` a `GameUISkill_unk* m1134[3]` (habilidades aprendidas, contador
  `m1130`); `m1170` de `int` a `GameUIAimTarget_unk*`; `m15B4` de `int` a `CharaTask*`. Tipos nuevos declarados en
  `GameUI.h` y definidos en `GameUI.cpp`: `GameUISkill_unk` (nombre `m0C`, descripcion `m14`), `GameUIAimTarget_unk`
  (escala `m28`, radio `m5C`).
- `GameUIField_unk` (vista local de `lbl_8087F430` en GameUI.cpp) gana `int m54E4` (= `GameTask::m54E4`).
- `fn_803DAB4C(int, int)`: parametros nombrados `show`, `keep` (sin cambio de tipos).
- `PopupTask.h`: `PopupD2A_unk` gana `float mFrame_unk` (0x100).
- `TacView.h`: nuevo miembro `void fn_805A96D4(CharaTask*, int)` (antes simbolo sin nombre C++); `symbols.txt`:
  `fn_805A96D4` -> `fn_805A96D4__7TacViewFP9CharaTaski`. `fn_805AC758` sigue `extern "C" (TacView*)` (sin
  escribir).
- `TacViewChara_unk::mType` (0x00) es en realidad un `CharaTask*` (vf10 y fn_805A96D4 lo usan como el personaje
  de la entrada; `mChara` 0x04 es su objetivo): de momento con casts; pendiente renombrar/cambiar el tipo.

## Decisiones

- `GameUIAimTarget_unk` es una vista del objetivo de `AiTarget` (`m0C`, tipo `CharaTask*` en `AiTarget.h`, otra
  familia): deuda de la regla 10, pendiente de llevar a `game/AiTarget.h`.
- `fn_803DBD44` declarado `extern "C" void fn_803DBD44(GameUI*)` (el simbolo no tiene nombre C++; sin escribir).
- PopupTask.cpp: tabla de mensajes `lbl_8087F1E4` como vista local `PopupMsgTable_unk` (la cabecera
  `MP/MpCampMsgTable_unk.h` choca con las declaraciones de `fn_801F3FF8` del fichero): deuda de la regla 10.
- Tras cada `pd.sh`/`pdbatch.sh` quedan etiquetas `@N` de `.sdata2` obsoletas en GameUI; se arreglan por valor con
  un script local (`scratch-A/bin/fixlbl.py`: renombra cada `@N` que falta en nuestro .o al `@N` de nuestro .o con
  el mismo valor, si es unico).
- Tras `pd.sh`, 13 etiquetas `@N` de `.sdata2` quedaban obsoletas (labelcheck peor); se renombraron por valor
  (cada valor es unico en nuestro .o) con un script de una sola vez. `GetNodePos_unk` y la declaracion de
  `lbl_8087F0A8` suben mas arriba en GameUI.cpp (inline/declaracion, sin cambio de codigo).

## Pendientes

- `GameUI::m30C`: pasar `GameUIList_unk` a `std::vector<Entry>` (MSL) para que `fn_803E2110` empareje
  (re-emparejar constructor y destructor).
- `GameUIAimTarget_unk` -> `game/AiTarget.h` (otra familia); `TacViewChara_unk::mType` -> `CharaTask*`.
- Sin escribir en la cola: GameUI `fn_803DBD44` (3392 B); TacView `fn_805AA94C`, `fn_805AC758`, `fn_805ADB28`.

## Hallazgos

- Una cadena literal dentro de una funcion inline de fichero sale como `@STRING@<inline>` y no en `@stringBase0`:
  pasar los nombres desde la funcion que llama.
- Un inline de fichero definido despues de la funcion que lo usa no se inlinea con `-ipa file` en este caso
  (`fn_805AA734` en `TacView::vf10`): pasar el cuerpo a un `static inline` anterior.
- Copia de colores con lfs/stfs alternados = constructor de copia miembro a miembro (`atn::Color`);
  `GameUIColor_unk` no lo tiene y copia con lwz/stw.
- `20.0f * 3.5f` se pliega a 70.0f; el original multiplica (`float radius = 20.0f; radius * 3.5f` no se pliega).
