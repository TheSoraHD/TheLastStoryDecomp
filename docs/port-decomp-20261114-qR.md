# port-decomp-20261114, cola R: minimapa del HUD (game/MiniMapTask)

## Resultado por funcion

- `MiniMapTask::vf14()` (0x804A894C, 5368 bytes): **escrita y equivalente** (sin DRAFT), 0,07 -> **94,2 %**
  (`// NONMATCHING`). Firma: `void MiniMapTask::vf14()` (sin cambios). Estructura:
  - `m5D8->mFlags |= 4`; sale si `m4C == 0`, sin `lbl_8087F8A0` o sin lider.
  - Caja "LOWER_LAYER" (m15F4..m1604): `m1608` baja 0,05 hasta 0,2 dentro y sube 0,05 hasta 1 fuera; quita
    el flag 0x10 de `m208` y pone ese alfa a los materiales `m_house_alpha`, `m_line_alpha`, `m_road_alpha`
    (rgb conservado; por debajo de 0,99 modo 3 y flag 0x10 en el modelo, si no modo 1).
  - `m160C` (DrawAll) -> `fn_804A9EB4()` y sale.
  - Sale si el lider esta en una caja "OFF_AREA" (`m1538`, minimo + tamano).
  - Angulo del mapa segun `lbl_8087F0A8->mTitle.m24` (GameConfig_unk +0x5A8; modo de giro del minimapa,
    nombre del miembro sin cambiar): 1 = rotacion Y del lider - pi, 2 = -(rotacion de la direccion de la camara
    del RenderManager).y - pi, otro = 0.
  - Flecha del jugador (`fn_8005EE38`, color 0xFF395A83, `m1D8`, 12x12).
  - 64 marcadores: dentro del rectangulo (margen 3) punto `m620`/`m1A8` + marcador (`fn_8005E7F4`); fuera, si
    `m24`, flecha en el borde (`m610`/`m178` y color del marcador con `m88`/`m118`) girada hacia el marcador.
  - Camara ortografica `m640` sobre el lider (altura 100, up (0,0,-1)), `fn_8004B378` y dos `AddRender` (2, 16).
  - Resto: asignacion de registros float en el bucle (el original deja el centro en f30/f31 y las constantes en
    f24-f29), huecos de pila (+4 en los locales con nombre) y planificacion alrededor de las llamadas.
  - Presupuesto de inline: con un helper inline grande para los materiales vf14 dejaba de inlinear todo (R3, todo
    o nada); el helper pequeno `MiniMapSetAlpha_unk` (solo el color) si entra.
- `vf18` y `vf10(RenderManager*)`: sin tocar (no dio tiempo).

## Cambios de firma o de layout (para el port)

- Ninguno en cabeceras. `MiniMapTask.cpp` incluye `game/GameConfig_unk.h` y declara
  `extern "C" GameConfig_unk* lbl_8087F0A8;`. El bloque `lbl_8087EEE0`/`GetGraphics_unk`/`lbl_8087F430`/
  `lbl_8087F8A0` se movio delante de vf14.

## Decisiones tomadas sin el usuario

- **splits.txt**: el double de conversion u32->float de 0x80757640 pasa de `game/MenuCommonTask` a
  `game/MiniMapTask` (`.rodata` de MenuCommonTask acaba en 0x80757640, la de MiniMapTask empieza ahi). Motivo:
  lo usa vf14 (conversion de los bytes del GXColor), MenuCommonTask no lo referencia ni lo emite, y su
  `@stringBase0` acaba justo en 0x80757640. Sin el cambio, labelcheck empeoraba en las dos unidades.
- symbols.txt: solo renombrados (`pd.sh` + tres floats de `.sdata2` a mano comprobando el offset en el `.o`).

## Hallazgos de una linea

- Un `ColorF_unk` con nombre en el cuerpo de vf14 ocupa la zona de locales con nombre (+0x30 en la pila); el
  original lo tiene como temporal: dentro de un inline pequeno sale en la zona de temporales.
- `m5E4 / 128.0f` y `m5E8 / 256.0f` (no `* 0.0078125f`): orden de operandos del `fmuls` como el original.
- `if (a == 0 || b == 0 || b->c == 0) return;` da el `bne`/`b` del original en la tercera condicion; tres `if`
  separados dan `beq`.
