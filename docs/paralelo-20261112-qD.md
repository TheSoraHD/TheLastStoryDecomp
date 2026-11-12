# Lote en paralelo 2026-11-12, cola D (camara e interfaz)

Rama `agent-D`. Inicio: 2026-11-12 14:55 CET. Referencia: `/home/claude/ref-D` (15057 / 18109).

## Resultado por unidad

### game/GameCamera_unk

- Funciones al 0 %: 33 -> 27 (`fn_8037DD00` al **100 %**, `fn_803903E0` 98,4 %, `fn_803830A8` 97,4 %, `fn_8038C070` 98,1 %, `fn_80389A00` 90,8 %, `fn_8038B650` 92,2 %; NONMATCHING con
  motivo).
- Nuevas al 100 %: `fn_80389678`, `fn_8038DDE0` (vibracion, por el getter inline), `fn_80389060` (giro con
  `Matrix3x4::fn_8013A13C`), `fn_80389210` (direccion como temporal y `pos` con nombre) y `fn_8037DD00`:
  20 -> 30 / 90 (`report.json` 15057 -> 15062).
- `fn_803893A0` 81 -> 92 % (direccion con `GetNormalize()` como referencia a temporal).
- Comentarios NONMATCHING antiguos que culpaban al getter `fn_8008B964` actualizados.
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

- `fn_803830A8` 97,4 % (camara libre con el stick): huecos de pila de las dos rotaciones. La mezcla del objetivo
  en una inline `LerpV_unk(a, b, t)` (95 -> 97 %) y el fovy por mitad en su propia variable (sin `fmsubs`).
  `poolcheck` avisa de una "cadena" `?fffA ` en `.sdata2`: son los bytes de 0.9f y 10.0f seguidos (falso positivo).

- `fn_803903E0` 98,4 % (inicio de la vista tactica, llamada por FinishCamera): asignacion de registros. Trucos que
  subieron 92 -> 98: la altura del mapa como inline con `return` temprano (un solo camino para 30), `d` reutilizado
  campo a campo, la mascara del rayo en una variable (`r18`), un puntero `start = &m8A4` para el bucle. Campos
  nuevos m8BC/m8C8/m8CC.

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

## Pendientes

- `fn_8038E23C` (colision, 772 bytes; llamada por fn_8038378C, fn_803884B8, fn_8038A5F0): falta declararla en
  `GameCamera.h`, y una declaracion de metodo renumera los `@N` de las 144 unidades que la incluyen. Sin ella
  tampoco se pueden escribir fn_8038A5F0, fn_803884B8 ni fn_8038378C.
- `__dt__10GameCameraFv` (152 bytes): necesita `ResHandle m7EC` en `GameCamera.h` (incluir `game/ResHandle.h`,
  cuyas inline renumerarian `@N`).
- `fn_80382A1C` (1668 bytes, sobre el presupuesto: todo fuera de linea): llama a `TownTask::fn_805B81F0(int, int)`
  (sin declarar, cabecera de otra cola) y a `atn::Camera::SetUpVector` (metodo nuevo en `Camera_unk.h`).
- `fn_8037EF48` (seguir al lider, 1784 bytes) y `fn_8038F52C`: no empezadas.
- `MiniMapTask::fn_804AAFCC`: la zona 0x834-0x1538 son 64 registros de 0x2C y un `static_vector` de 64 pares
  {CharaTask*, indice} en 0x1334; habria que rehacer el constructor (borrador) para cambiar la cabecera.
- Mover `fn_8008B964` a `RenderManager.h` en un lote de integracion (ver decisiones).
- FinishCamera_unk, GameUI, MiniMapTask y PopupTask: sin cambios (no dio tiempo).

## Hallazgos

- Una definicion de funcion inline en una cabecera gasta numeros `@N` (3 por funcion) en cada unidad que la
  incluye; una declaracion de metodo en una clase tambien renumera; un miembro de datos no.
- Pila: un `Vector3` con nombre se coloca por encima de los temporales; si en el original una suma queda entre
  locales con nombre, era un local (`pos` en fn_80389210); si un local queda por debajo de los temporales, era un
  temporal (expresion directa) o una referencia `const&` a uno.
- `switch` con 8 `case` consecutivos + 2 sueltos da `subi; cmplwi 7; ble` y luego las comparaciones sueltas.
- Una funcion con varias copias de un bloque de parametros (`m738 = m51C; m738.m0C = ...`) da stores de constantes
  ya propagadas y solo lee de memoria el campo no inicializado (m34).

Fin: 2026-11-12 ~16:25 CET.
