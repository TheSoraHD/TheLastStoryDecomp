# Lote en paralelo 2026-11-12, cola D (camara e interfaz)

Rama `agent-D`. Inicio: 2026-11-12 14:55 CET. Referencia: `/home/claude/ref-D` (15057 / 18109).

## Resultado por unidad

### game/GameCamera_unk

- Funciones al 0 %: 33 -> 29 (`fn_8037DD00` al **100 %**, `fn_8038C070` 98,1 %, `fn_80389A00` 90,8 %, `fn_8038B650` 92,2 %; NONMATCHING con
  motivo).
- Nuevas al 100 %: `fn_80389678`, `fn_8038DDE0` (vibracion, por el getter inline) y `fn_80389060` (giro con
  `Matrix3x4::fn_8013A13C`): 20 -> 28 / 90 contando las que vuelven a 100 tras renombrar etiquetas.
- `fn_8008B964` (getter de `RenderManager::instance_`): definida `extern "C" inline` **en `GameCamera_unk.cpp`**
  (como ya hacia `PlayerInput_unk.cpp`), no en `RenderManager.h`. Ver decisiones.
- `Matrix3x4::fn_8013A13C(rot)` en lugar de `PSMTXRotRad(m, 'y', rot)` (el original pasa el angulo por un
  inline: `fmr f1, fX`): sube fn_80389060 (100 %), fn_8038A200, fn_8038C5B0, fn_8038D1C0.
- `fn_8038C928`: el fovy de la mezcla calculado antes de `SetPosition` (87,4 -> 89,5 %).
- `m958` de `GameCamera.h` partido: `atn::Vector3 m98C[8]` (limpiado por el modo por defecto). Un miembro de
  datos no renumera los `@N` (comprobado con `Event/Dialog.o`); una declaracion de metodo si.

- `GameCamera.h`: los bloques 0x51C-0x7B0 son `GameCameraParam_unk` (0x3C bytes: m558, m594, m5D0, m60C, m648,
  m6C0, m6FC, m738, m774); los campos sueltos anteriores (m5D4, m618, m654, m73C, m744, m780...) pasan a
  `bloque.mXX`. `fn_8038E540` recibe `GameCameraParam_unk*` (era `int* state`, sin usar; renombrado en
  `symbols.txt`; cambiar la firma de un metodo no renumera). `fn_80388A88` 87 -> 92 % (la posicion se pasa como
  `Vector3` temporal). `fn_8037DD00` (valores por defecto) al 100 %: m738/m774 son copias de m51C y m6FC de m6C0.
- 21 `lbl_808859xx` de `.sdata2` (solo usados por GameCamera) pasan a `@N` locales.

## NONMATCHING nuevas

- `fn_8038B650` 92,2 % (modo de apuntar): la diferencia de la vista de `m1F4` queda en el hueco mas bajo de la
  pila en el original (como si el rayo estuviera en un inline) y redondea (frsp) la z del origen. Los ids
  700013-700020/700022/700023 son un `switch` (cadena de comparaciones igual que el original).
  Campos nuevos de `GameCamera.h`: m60C, m610, m618-m624 (miembros de datos, no renumeran).

- `fn_8038C070` 98 %: el original pone la rotacion z de `fn_800109E0` mas arriba en la pila y la x mas abajo
  (aqui al reves); cambiar el orden de los temporales de `fn_800109E0` solo arregla la y y baja fn_803918EC.
- `fn_80389A00` 90,8 %: registros de la copia de la camara (como fn_80392D2C) y huecos de pila de las dos
  mezclas (como fn_8038C928).

## Decisiones tomadas sin el usuario

- **Getter inline local y no en la cabecera**: definir `fn_8008B964` inline en `RenderManager.h` consume
  numeros `@N` (una definicion de funcion gasta 3) en las 117 unidades que la incluyen y bajaba a 99,x % muchas
  funciones por nombres de etiqueta; hacerlo bien exige `lblmap` en todas (y tocar `symbols.txt` de unidades de
  otras colas). Se deja la declaracion de la cabecera y la definicion inline en `GameCamera_unk.cpp`.
  Pendiente: moverla a la cabecera en un lote de integracion con renombrado global de etiquetas.
- `fn_8038D4D4` (80,8 %): el desplazamiento de la cabeza se suma despues de la primera mezcla; queda en
  80,76 % (antes 80,82 %, -0,06: ruido de planificacion por el getter inline).
- `lbl_80885A74` (float de `.sdata2` solo usado por GameCamera) pasa a `@13113` local.
- `fn_8038E23C` (colision del ojo, 772 bytes) no se escribe: necesita declarar un metodo nuevo en
  `GameCamera.h` (incluida por 144 unidades) y eso renumera los `@N`.

## Hallazgos

- Una definicion de funcion inline en una cabecera gasta numeros `@N` (3 por funcion) en cada unidad que la
  incluye; una declaracion de metodo en una clase tambien renumera; un miembro de datos no.
