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
- `__sinit_\GameUI_cpp` (0x803E78B4): escrito 0,76 % -> 65,7 %. Valores sacados emulando el asm original
  (script `scratch-C2/tools/emu.py`): `lbl_807C8628` = 16 `atn::Color(r / 255.0f, g / 255.0f, b / 255.0f, 0)`, y
  la `.data` 0x8078BE68 (0x65C bytes) son tres tablas `static`: `sButtonGlyphs_unk[20]` ({`wchar_t[4]`,
  `atn::Vector4_unk` UV}; "A", "B", ..., "ZL", "ZR"), `sButtonHelp_unk[18]` y `sButtonHelp2_unk[23]` ({flag,
  tipo de icono, id de mensaje, UV}, 0x1C bytes). El UV es `atn::Vector4_unk` (copia paired-single). Resto:
  orden de los registros y la pila del inicializador.
- `GameUI::fn_803E3468` (aviso de boton de una accion: item, glifo del boton segun el icono, el mando, el tipo de
  ataque y el de recogida de `SetConfig`, flechas de la cruceta y nombre/icono de la accion): 0 % -> 90,3 %.
  Resto: el original guarda `&task->mData.m10` en un registro antes del hash (aqui se rematerializa; probados
  helpers con referencia a la capa), y lee dos veces el texto de `lbl_8087F1E4` (aqui una).

- `GameUI::fn_803E5200` (posicion del icono sobre un personaje: sobre "head"/"neck"/el centro de colision,
  3 unidades hacia la camara, girado hacia ella con `Vector3::ToRotation_unk` y escalado por la distancia entre
  m2670..m2674): 0 % -> 99,2 %. `top.y += 5; top.y += m149C` (BattleCharaTask) y `Vector3 scale(1, 1, 1)` antes
  de asignarle el lerp suben de 89 a 99,2. Resto: el lerp suma la base al final (`atn::Lerp_unk` la suma
  primero; escrito al reves se mueven los temporales).
- `GameUI::fn_803D7850(CharaTask*, CharaTask*, int, int)` (linea de odio `index` entre dos personajes, entre
  los bordes de sus colisiones; el extremo se desliza 5 fotogramas cuando `slide`; dos efectos de m1554/m1584
  con el color segun el tipo de ataque): 0 % -> 98,7 %. `float dist` local, `p1 = (m60 - p1) * t + p1` y
  `float width = 0.5f` antes del `if` (se queda en f31) suben de 96,6. Resto: la pila (0x20 bytes menos aqui;
  matrices del `RotateZYX_unk` inline en otro orden) y registros float.

## Decisiones tomadas sin el usuario

- `GameUI.h`: declarados `fn_803E2410(CharaTask*)` y `fn_803E2A30(int, int, atn::Vector3, int, int)` sin nombres
  de parametro. `symbols.txt`: `fn_803E2410` -> `fn_803E2410__6GameUIFP9CharaTask`, `fn_803E2A30` ->
  `fn_803E2A30__6GameUIFiiQ23atn7Vector3ii`.
- `game/Np131LivingArmor.cpp` (sin cola) llamaba a fn_803E2A30 como `extern "C"` con un `void* draw`: quitada la
  declaracion y llamada como `static_cast<GameUI*>(draw)->fn_803E2A30(...)` (lo pide el prompt al renombrar);
  re-emparejada con `pd.sh`, sin bajadas.
- `GameUISession_unk` (local de GameUI.cpp) gana `int m540` (estado de la partida, 2 = jugando).
- `GameUI.h`: declarado `fn_803D7850(CharaTask*, CharaTask*, int, int)` sin nombres; `symbols.txt`:
  `fn_803D7850` -> `fn_803D7850__6GameUIFP9CharaTaskP9CharaTaskii` (solo lo llama fn_803D83A8, sin escribir).
  `GameUIElem7C_unk` pasa de `unsigned char m00[0x78]` a `atn::Matrix3x4 m00, m30; atn::Vector3 m60;` + relleno
  (solo miembros de datos; el ctor de GameUI sigue en 98,11 %).
- `GameUIField_unk` (local) gana `atn::Vector3 mEye` en 0x74 (posicion de la camara).
- `extern "C" GameConfig_unk* lbl_8087F0A8` se sube en GameUI.cpp (antes de fn_803E3468).
- `symbols.txt`: seis floats de `.sdata2` que solo usa el `__sinit` (0x80885DA0, DB0, DB8, DD8, DE8, DFC) se
  renombran a mano a los `@N` de nuestro .o (pd.sh no los empareja porque el `__sinit` no esta al 100 %;
  labelcheck volvia a "peor").

## Hallazgos

- Tras cada funcion nueva el `pd.sh` renumera los floats que solo usa el `__sinit` y labelcheck da "peor":
  renombrarlos por valor (`scratch-C2/tools/fixlabels.py`; ojo con un `@N` nuestro ya usado en otra direccion).

- Un `__sinit` de datos se puede reconstruir emulando su asm (lfs/stfs/psq) para sacar los valores finales.
- `x = a; if (c) x = b;` da `li; beq; li`; el ternario da codigo sin saltos (neg/or/srwi).

## Pendientes

- `atn::D2A_TaskInstanceBase<N, M>::SetColor255_unk`: el parametro deberia ser `atn::Color` (16 bytes); bloquea
  fn_803E2A30, fn_803E4E58, fn_803DDD54 (otra familia).
