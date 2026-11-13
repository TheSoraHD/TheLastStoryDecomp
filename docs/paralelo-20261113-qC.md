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
- `GameUI::fn_803CFC78` (parametros del HUD en el menu de depuracion: colores de oficio/atributo, cursor de la
  vista tactica, contadores, icono de acceso) 99,8 %: el original pasa el primer color con `addi r5, r30, 0`.
  Primera funcion con cadenas tras el constructor: sus desplazamientos en `@stringBase0` ya cuadran.
- `GameUI::fn_803D022C` (crea las partes del HUD; lo llama GameTask) 99,5 %: registros de los bucles.

## NONMATCHING nuevas

- `fn_803DCD34` 99,4 %: pool de cadenas.
- `fn_803DD160` 87,5 %: registros (task en r31, data en r28) y la copia de cada color (el original toma la
  direccion de la entrada y copia desde ella, como fn_803DD90C).
- `fn_803E6D4C`, `fn_803E746C`: ver pendientes (parametro de `SetColor255_unk`).
- `fn_803CFC78` 99,8 %: `addi r5, r30, 0` del primer color (probados la tabla como puntero, el array directo,
  dos arrays `static`).
- `fn_803D022C` 99,5 %: el original da a las rutas sacadas del bucle los registros altos y a los punteros de
  induccion los bajos.

## Decisiones tomadas sin el usuario

- `GameUI.h`: declarado `void fn_803DCD34(CharaTask*, int, int, int);` sin nombres de parametro;
  `symbols.txt`: `fn_803DCD34` -> `fn_803DCD34__6GameUIFP9CharaTaskiii` (renombrado; nadie mas lo usa).
- `lbl_807C8628` (tabla de colores del HUD, `.bss` de la unidad) pasa de `GameUIColor_unk[16]` a
  `atn::Color[16]`: la copia del original es la del constructor de copia de `atn::Color` (`lfs`, no `lwz`).
  `SetJobColor_unk` toma `const atn::Color&` (fn_803DD90C sigue en 91,2 %).
- `GameUI.cpp` incluye `game/Arms.h` (para `Arms::fn_80044C30`, como BattleCharaTask).
- `GameUI.h`: declarados `fn_803CFC78()` y `fn_803D022C()` (sin parametros); `GameUIElem20_unk::m14/m18`
  pasan de `int` a `GameUIInst_unk*` (las crea fn_803D022C). `symbols.txt`: `fn_803CFC78` y `fn_803D022C`
  pasan a `...__6GameUIFv`; `fn_803E78B4` pasa a `__sinit_\GameUI_cpp` (scope:local).
- `lbl_807C8628` se define en GameUI.cpp (regla 10: `.bss` de la unidad); como `atn::Color` tiene constructor,
  sale nuestro `__sinit` (vacio de momento) que se empareja con el original.
- `game/GameTask.cpp` (otra cola): llama `lbl_8087F490->fn_803D022C()` en vez del `extern "C"` (renombrado);
  quitar esa declaracion (con nombre de parametro) libera un `@N`: `pd.sh` re-empareja casi todo y dos floats
  ambiguos de `.sdata2` (0x808857A8 = 3.0, 0x808857AC = 0.4) se renombran a mano (@27400, @27401).
- `GameUISession_unk` (y `GameUISessionPlayer_unk`) se mueven al principio de GameUI.cpp; gana `int m5A8` e
  `IsShowLevel_unk()` (devuelve `unsigned char`: el `clrlwi` del original).
- `typedef atn::D2A_TaskInstanceBase<64, 8> GameUIWindow_unk` en GameUI.cpp; `mF14` sigue siendo
  `atn::Task*` en `GameUI.h` (cast en fn_803DCD34) para no tocar la cabecera.

## Hallazgos

- `x != 2 && x == 0` sobre un miembro: el original carga una vez y compara dos veces; con una inline `bool`
  para la primera mitad (`IsCursorOn_unk(sel) { return sel.mState != 2; }`) sale igual (GameUI fn_803DCD34).
  En fn_803DCA84 (con `tac != 0 && tac->mA70 != 0` delante) la misma inline pliega las dos comparaciones.
- Escala de un `Vector3` en el orden z, x, y con temporales en registros: `v *= -1.0f` (operator*=), no
  `v = v * -1.0f` (deja un temporal en pila).
- Rutas que el original saca de un bucle (en registros y al principio del pool de cadenas): locales
  `const char*` dentro del bucle (GameUI fn_803D022C, 91,6 % -> 99,5 %).

## Pendientes

- `atn::D2A_TaskInstanceBase<N, M>::SetColor255_unk(const char*, D2A_RGB_unk)`: el original copia 16 bytes en
  la llamada; el parametro es probablemente `atn::Color` (cambiar `include/atn/D2A_TaskInstance.h` y renombrar
  los dos simbolos `...FPCcQ23atn11D2A_RGB_unk` -> `...FPCcQ23atn5Color`; otra familia).
- MiniMapTask: SetPosition/SetTarget de `atn::Camera` (inline nueva en `Camera_unk.h`, muy incluida) y
  `fn_804ABF74` (ctor de `ComDrawTextureMask`): sin tocar.
