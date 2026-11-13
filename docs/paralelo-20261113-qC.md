# paralelo-20261113, cola C (agente C)

Unidades: `game/GameUI`, `game/MiniMapTask`, `game/PopupTask`.

## Resultado por unidad

### game/GameUI (22 al 0 %)

- `GameUI::fn_803DCD34` (ventana de grupo de un personaje: marca de lider, posicion sobre la cabeza o abajo,
  nivel y nombre de la habilidad) 99,4 %: solo los desplazamientos en el pool de cadenas (las funciones
  anteriores de la unidad no estan escritas, asi que `@stringBase0` no cuadra).
- `fn_803DD160` (iconos de oficio y atributo, `atn::D2A_Task`; lo llama NewCampPartsStatus) 87,5 %.
- `fn_803E6D4C` / `fn_803E746C`: las mismas dos funciones para la ventana `D2A_TaskInstanceBase<64, 8>`
  (72 % / 68 %): el original pasa a `SetColor255_unk` el `atn::Color` entero (16 bytes).

## NONMATCHING nuevas

- `fn_803DCD34` 99,4 %: pool de cadenas.
- `fn_803DD160` 87,5 %: registros (task en r31, data en r28) y la copia de cada color (el original toma la
  direccion de la entrada y copia desde ella, como fn_803DD90C).
- `fn_803E6D4C`, `fn_803E746C`: ver pendientes (parametro de `SetColor255_unk`).

## Decisiones tomadas sin el usuario

- `GameUI.h`: declarado `void fn_803DCD34(CharaTask*, int, int, int);` sin nombres de parametro;
  `symbols.txt`: `fn_803DCD34` -> `fn_803DCD34__6GameUIFP9CharaTaskiii` (renombrado; nadie mas lo usa).
- `lbl_807C8628` (tabla de colores del HUD, `.bss` de la unidad) pasa de `GameUIColor_unk[16]` a
  `atn::Color[16]`: la copia del original es la del constructor de copia de `atn::Color` (`lfs`, no `lwz`).
  `SetJobColor_unk` toma `const atn::Color&` (fn_803DD90C sigue en 91,2 %).
- `GameUI.cpp` incluye `game/Arms.h` (para `Arms::fn_80044C30`, como BattleCharaTask).
- `typedef atn::D2A_TaskInstanceBase<64, 8> GameUIWindow_unk` en GameUI.cpp; `mF14` sigue siendo
  `atn::Task*` en `GameUI.h` (cast en fn_803DCD34) para no tocar la cabecera.

## Hallazgos

- `x != 2 && x == 0` sobre un miembro: el original carga una vez y compara dos veces; con una inline `bool`
  para la primera mitad (`IsCursorOn_unk(sel) { return sel.mState != 2; }`) sale igual (GameUI fn_803DCD34).
  En fn_803DCA84 (con `tac != 0 && tac->mA70 != 0` delante) la misma inline pliega las dos comparaciones.
- Escala de un `Vector3` en el orden z, x, y con temporales en registros: `v *= -1.0f` (operator*=), no
  `v = v * -1.0f` (deja un temporal en pila).

## Pendientes

- `atn::D2A_TaskInstanceBase<N, M>::SetColor255_unk(const char*, D2A_RGB_unk)`: el original copia 16 bytes en
  la llamada; el parametro es probablemente `atn::Color` (cambiar `include/atn/D2A_TaskInstance.h` y renombrar
  los dos simbolos `...FPCcQ23atn11D2A_RGB_unk` -> `...FPCcQ23atn5Color`; otra familia).
- MiniMapTask: SetPosition/SetTarget de `atn::Camera` (inline nueva en `Camera_unk.h`, muy incluida) y
  `fn_804ABF74` (ctor de `ComDrawTextureMask`): sin tocar.
