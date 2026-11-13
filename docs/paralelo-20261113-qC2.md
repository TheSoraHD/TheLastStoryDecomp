# Cola C2 (lote paralelo-20261113): segunda pasada de Interfaz

Unidades: `game/GameUI`, `game/MiniMapTask`, `game/PopupTask`.

## Resultado

- (se actualiza al cerrar)

## game/GameUI

- `GameUI::fn_803E2410(CharaTask*)` (medidor de puntos de magia sobre un personaje: el canto de su magia actual,
  la magia elegida en la vista tactica o el tiempo de los estados de los bits 0 y 31 de `mStatus.m0C`; los
  personajes con `mFlagB13` dibujan el suyo con `vfC0(this)`): 0 % -> 99,63 %. Resto: registros de los flags de
  resultado de `CanActB_unk` (r5/r0/r3 en el original; mismo problema que fn_803E4A5C; probados `CanAct_unk()` de
  CharaTask y las tres condiciones en un `||`) y las etiquetas de 320/170 en `.sdata2`.
- `GameUI::fn_803E2A30(int, int, atn::Vector3, int, int)` (siguiente medidor de puntos de magia del HUD), desde el
  borrador de la cola C: 0 % -> 92 %. `pos` va **por valor** (el llamador copia el vector); los segundos y el
  fotograma salen de fn_803CE854/fn_803CE8A8 inline. NONMATCHING por el parametro de `SetColor255_unk`
  (`D2A_RGB_unk`; el original copia el color entero) que agranda la pila y mueve los registros.

## Decisiones tomadas sin el usuario

- `GameUI.h`: declarados `fn_803E2410(CharaTask*)` y `fn_803E2A30(int, int, atn::Vector3, int, int)` sin nombres
  de parametro. `symbols.txt`: `fn_803E2410` -> `fn_803E2410__6GameUIFP9CharaTask`, `fn_803E2A30` ->
  `fn_803E2A30__6GameUIFiiQ23atn7Vector3ii`.
- `game/Np131LivingArmor.cpp` (sin cola) llamaba a fn_803E2A30 como `extern "C"` con un `void* draw`: quitada la
  declaracion y llamada como `static_cast<GameUI*>(draw)->fn_803E2A30(...)` (lo pide el prompt al renombrar);
  re-emparejada con `pd.sh`, sin bajadas.
- `GameUISession_unk` (local de GameUI.cpp) gana `int m540` (estado de la partida, 2 = jugando).

## Hallazgos

## Pendientes

- `atn::D2A_TaskInstanceBase<N, M>::SetColor255_unk`: el parametro deberia ser `atn::Color` (16 bytes); bloquea
  fn_803E2A30, fn_803E4E58, fn_803DDD54 (otra familia).
