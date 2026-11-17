# escribir-20261116 cola W1c (game, interfaz y sistema; tercera pasada: al 100 %)

- Inicio: 2026-11-17 06:32 (CET). Tope: 09:02.

## Tabla de la cola

| Funcion | Antes | Despues |
|---|---|---|
| GameScene::GameScene (0x803BEC40) | 96,55 % | **100 %** |
| GameScene::vf10 (0x803C1950, modelos restringidos) | 96,19 % | **100 %** |
| GameScene::fn_803C74E0<gs::NpcSetPoint> (0x803C74E0) | 99,86 % | **100 %** (`int offset;` declarado antes de `base`; la copia debil 0x803C795C renombrada a `GetData_unk__Q23atn31SimpleArray<Q22gs11NpcSetPoint>CFv`) |
| GameScene::fn_803CB044 (esconderse) | 95,09 % | equivalente NONMATCHING 98,36 % (pila de los vectores del bucle) |
| GameTask::vf14 | 98,16 % | equivalente NONMATCHING 99,22 % (y en f31; constante 0x4330) |
| TutorialTask::fn_805BA934 | 97,99 % | equivalente NONMATCHING 99,27 % (`fn_801231B8(...) == 0`: los dos cntlzw; queda la carga de pos.z) |
| TutorialTask::fn_805BB654 | 90,44 % | equivalente NONMATCHING 94,77 % (sube al llamar a atn::SoundManager::fn_800D089C como miembro) |
| GameChoiceWindow::fn_803B4C38 (choices) | 88,63 % | equivalente NONMATCHING 99,93 % (r18/r19 y el pool ancho) |
| GameTextMessageWindow::vf10 | 91,30 % | equivalente NONMATCHING 93,98 % (bools de HasVoice_unk) |
| GameTextMessageWindow::fn_803B6970 | 89,77 % | equivalente NONMATCHING 92,35 % |
| GameTextMessageWindow::fn_803B6E04 | 88,37 % | equivalente NONMATCHING 90,97 % |
| ReplayBuffer::fn_80570A74 | 89,43 % | equivalente NONMATCHING 91,42 % (FixedArray::erase real) |
| GameMessageWindow::fn_803B5974 / GameTextMessageWindow::fn_803B710C | 99,99 % | sin cambios (pool ancho, ver Bloqueos) |
| GameChoiceWindow fn_803B3D2C | 99,46 % | sin cambios |
| TutorialTask::vf0C | 99,83 % | sin cambios (10 variantes, ver Bloqueos) |
| GameTask fn_8036DDD4 | 99,7 % | sin cambios (11 variantes) |
| GameTask::fn_8036CC90 | 99,97 % | sin cambios |
| GameScene::fn_803C1E3C | 98,23 % | sin cambios (7 -> 2 diferencias con `bgm`/`volume` locales, no commiteado) |
| MapPartsArea::vf1C | 99,90 % | sin cambios |
| MapTask::fn_804A0F80 | 5 % | sin cambios (cuerpo en una inline: 30,8 %, no commiteado) |

## Para el port
- GameScene::GameScene: ya no pone a 0 m64 ni m134 en la lista de inicializacion (el original no lo hace; m64 se asigna despues siempre, m134 solo si existe el .gs: queda sin inicializar si no hay escena). El SongManager se crea con padre lbl_8087F430 (GameTask), no con la escena (`fn_805A2240((atn::Task*)lbl_8087F430)`): cambio de comportamiento. El mensaje "/game_message/%s_xx.u16" se compone en el mismo buffer que la escena.
- ArenaTask::fn_800431F4(int, int) -> (int, int, int) (el tercero, el submapa, no se usa; GameScene lo pasa). symbols.txt: `fn_800431F4__9ArenaTaskFii` -> `fn_800431F4__9ArenaTaskFiii`.
- GameScene: `MakeScenePath_unk(out, fmt, ...)` -> dos `static inline` `GetScenePath_unk`/`GetMessagePath_unk` con la cadena dentro; `ErrorManager::SetLoading_unk` en vez de escribir m18/m1C; dinero del pueblo con `0 <= money && money < 400000` (mismo comportamiento).
- GameScene::vf10: inlines `RotateZYX_unk`, `SetModelMtx_unk` (copia de la matriz y mayor escala de los ejes, `GetColumnLength_unk`); mismo comportamiento.
- GameScene::fn_803CB044: sin el operador coma (`if (!(lensq < 4)) { n = off; normalizar; if (!(dot > 0.5)) continue; }`), `MaxD_unk` (max como double, frsp) y el mundo de colision en un local antes de la llamada; mismo comportamiento.
- GameTask::vf14: `IsTableBusy_unk(m550C)`, prueba del sonido de los iconos como `(!swing && !(h0 && h1)) || (swing && !(h2 && h3))` (equivalente), `FindWarpPoint_unk` y `GetHeapUsed_unk(DLAllocator&)` con el MemoryManager en un local; `text` declarado antes que `name`. Mismo comportamiento.
- GameChoiceWindow::fn_803B4C38: especializacion `std::__vec_split_unk<std::wstring>::__push_front_unk` en el .cpp (las cadenas se mueven con memcpy/memset al crecer el vector, como MSL; el original no copia una a una). Para el port de 64 bits: mueve objetos `basic_string` como bytes (valido con la implementacion de MSL del port).
- GameTextMessageWindow: `HasVoice_unk` como tres bools planos (`bool x = a && b`); quitados `HasName_unk`/`CanVoice_unk`. Mismo comportamiento.
- TutorialTask: `atn/SoundManager.h` en vez de la vista `SoundManager_unk` (lbl_8087EFE8 como atn::SoundManager*, fn_800D089C miembro); `m5B4 = fn_801231B8(SetConfig::sInstance) == 0` (mismo valor).
- ReplayBuffer::fn_80570A74: borrado con `FixedArray_unk::erase` y la inline `RemoveEffects_unk(effects, emitter)`; mismo comportamiento.

## Decisiones tomadas sin el usuario
- `MaxD_unk(float, float)` que devuelve `double` (GameScene fn_803CB044): unica forma encontrada del `frsp` tras el maximo; con `// inferred:`.
- `atn::MemoryManager* mm` reasignado en cada estadistica de GameTask::vf14 (el original llama a Instance() cada vez y recalcula la direccion del heap en cada llamada virtual).
- Especializacion de `__vec_split_unk<std::wstring>::__push_front_unk` dentro de GameChoiceWindow.cpp (no en std/vector.h ni en std/vector_wstring.h: la de TacView tiene otra forma de buffer y empeora 4C38).

## Hallazgos
- Literales de una inline con enlace externo van a objetos `@STRING@` propios; para que vayan al `@stringBase0` de la unidad, la inline tiene que ser `static inline` (GameScene ctor 99,2 -> 99,8 %). (escribir-20261116 qW1c)
- Un formato pasado como parametro a una inline se evalua (y guarda en un registro) antes de la llamada interior; si el original carga la cadena justo antes del sprintf, cada formato va en su propia inline con el literal dentro. (escribir-20261116 qW1c)
- `x >= 0 && x < N` se pliega a `cmplw` sin signo; `0 <= x && x < N` deja los dos `cmpw` del original (GameScene ctor). (escribir-20261116 qW1c)
- Recarga de un global tras guardar en uno de sus miembros (`g->a = 1; g->b = 1;` recarga g): el original usa un setter inline del objeto (`ErrorManager::SetLoading_unk`). (escribir-20261116 qW1c)
- Buffers locales: si el original reusa un buffer para dos rutas, quitar el segundo array (pila 0x100 menor). Orden de dos arrays en pila: el declarado primero queda mas arriba (GameTask vf14 `text`/`name`). (escribir-20261116 qW1c)
- Argumento extra en r5 antes de una llamada a una funcion de 2 parametros: la funcion real tiene un tercer parametro sin usar (ArenaTask::fn_800431F4). (escribir-20261116 qW1c)
- Pila de temporales de matrices/vectores al reves que el original: un nivel mas de inline (RotateZYX_unk con los tres RotateAxis_unk; SetModelMtx_unk con la escala; `float sz = ...; sy; sx; Vector3 scale(sx, sy, sz)` para el orden de los temporales) (GameScene vf10 96,2 -> 100 %). (escribir-20261116 qW1c)
- Un objeto pasado por referencia a una inline como `mm->mMem1` (mm local) se re-evalua en cada uso (el original guarda mm y hace `addi r3, r29, 0x138` antes de cada llamada virtual); con `&Instance()->mMem1` se guarda la direccion (GameTask vf14). (escribir-20261116 qW1c)
- `a == 0 ? ... : ...` dentro de `!(A || B)` frente a `(!s && !(h0 && h1)) || (s && !(h2 && h3))`: el original vuelve a probar `s` en la segunda rama (saltos `bne` a la prueba de B). (escribir-20261116 qW1c)
- Recarga de un campo probado justo antes (`if (x.mState != 0) { if (x.mState == 1)`): la primera prueba va en una inline bool (`IsTableBusy_unk`). (escribir-20261116 qW1c)
- Bucle de busqueda con `li rX, 0` tras el bucle (retorno 0 al salir): es una inline que devuelve el puntero (`FindWarpPoint_unk`), con el id como parametro cargado antes del bucle. (escribir-20261116 qW1c)
- Bools que el original deja materializados (li 0 / li 1 / cmpwi): `bool x = a && b;` planos en una sola inline; con `bool r = false; if (...) r = true;` anidados MWCC enhebra los saltos. (escribir-20261116 qW1c)
- Dos `cntlzw; srwi` seguidos (`x != 0` como `!(x == 0)`): el original llama a una inline que devuelve `x == 0` (aqui la copia debil fn_801231B8 de SetConfig) y compara su resultado con 0 (TutorialTask fn_805BA934). (escribir-20261116 qW1c)
- Una vista local de una clase del motor con `extern "C"` (SoundManager_unk + fn_800D089C(self, ...)) puede bajar funciones vecinas: con la cabecera real y el metodo miembro TutorialTask fn_805BB654 sube 90 -> 94,8 %. (escribir-20261116 qW1c)
- `data + size - data` sin plegar antes de `/ 12` en el movimiento de un vector: diferencia en bytes `((char*)last - (char*)first) / (int)sizeof(T)` (como std/vector_wstring.h). (escribir-20261116 qW1c)

## Bloqueos
- GameChoiceWindow pool ancho (fn_803B5974/fn_803B710C 99,99 %, fn_803B4C38): el original tiene L"" de fn_803B3D2C en 0x8087DD80 (objeto propio, hasta 8 bytes), L"" de fn_803B4C38 en +0 de 0x8087DD88 y L"\xFF1F..." en +4 (relleno a 4 o 2 bytes mas antes). MWCC junta los L"" iguales y no alinea. Probado: fn_803B3D2C `inline` (se inlinea en sus llamadores). Sin probar: que 3D2C/4C38 fuesen funciones debiles/plantilla (sus literales irian a `@STRING@`).
- TutorialTask::vf0C (r26/r27 del primer SetPaneText_unk): probados local para el texto, inline con las dos llamadas, hash en local, copia del layout, segundo bucle con otro puntero, orden de page/loading, locales arriba, bool, return temprano (10).
- GameTask fn_8036DDD4 (r28/r29 del flag y la constante 0x4330): probados sin flag, flag declarado arriba/al principio del bucle/asignado tarde, rango antes, inline del rango, inline HpRate_unk, rate en local (11). Declarar `int found = 0;` antes de `d` deja solo el `li` fuera de sitio.
- GameScene fn_803C1E3C: con `GameBgm* bgm` y `float volume` locales quedan 2 diferencias (el `lfs` del volumen sale antes del `mr r3`).
- GameScene fn_803C1CAC: la recarga de mDefaultCamera para la llamada; probados el orden de la comparacion, inline de reset y bool IsDefaultCamera_unk.
- GameScene fn_803CB044: pila de los vectores del bucle (n, up, d, off, closest, sphere, side, dir en otro orden).
- GameTask vf14: y en f31 (nuestro f29); probados los floats del heap declarados antes/despues y en una inline.
- GameTask fn_80370320: recarga de m56F4.mCount en el Push_unk; inline CanPush_unk empeora (98,9 -> 98,5).
- GameTask fn_803729B4: el original no llama al constructor vacio de atn::Matrix3x4 del bucle de rayos (nuestro lo llama por el presupuesto R3).
- ReplayBuffer fn_80570A74 (91,4 %): rotacion de r10/r11/r12; fn_8056A9E4: el original intercala los `lfs` de la columna z entre los `psq_st` de __MtxCopy (con SetModelMtx_unk de GameScene igual).
- GameScene fn_803C606C/fn_803C6FE4 y sus copias (PointGroup/CharaGroup, 98,2-98,6 %): el original inicializa el contador n antes del memset y el desplazamiento n*4 tras begin() del tokenizer; mover `int n = 0` despues del tokenizer empeora (98,19 -> 97,0). Ademas el orden de "SceneCharaGroup"/"ScenePointGroup" en el pool (0x3b3/0x3c3 cambiados) y las envolturas `extern "C"` fn_803C72D4... de las plantillas de SimpleArray (deuda de la regla 10).
- GameScene fn_803C1338 (99,2 %): orden de los FPR de las constantes (el original da f31 al 0.0f); una inline Rand01_unk no cambia nada.
- GameScene fn_803CAED0 (97,6 %): pila de los temporales de `(b - a).GetNormalize()` frente a side/up; probados declarar side/up antes, `Vector3 d = b - a`, inline GetLinkDir_unk.
- GameScene fn_803C0938/0634/0C2C: el original recalcula `m90[index]` desde el indice para `id` (no lo reutiliza); un return temprano empeora (45 -> 58 diferencias).
- MapPartsArea::fn_804962B0 (98,55 %): r30/r31 (iterador y desplazamiento interior); declarar `i`/`it` arriba empeora (13 -> 19 diferencias).
- ReplayBuffer fn_8056D60C (78 %): el original carga 255.0f y 0.0f una vez y precarga f3-f11 con fmr (resultados de los ternarios del clamp); probados locales lo/hi, ternarios en locales, `v * 255.0f`, min/max por plantilla (valor y referencia) en un fichero suelto: ninguno da las precargas.
- ReplayBuffer vf08 (92,5 %): el original copia mColor (0x30-0x3C) palabra a palabra con r0 y los Vector3 con psq_l/psq_st/lfs/stfs; ningun tipo probado (struct POD, array, ints, op= propio) da ese patron.
- MapTask fn_804A0F80 (5 %): con el cuerpo en una inline (R5) sube a 30,8 % (se inlinean Lerp_unk<float> y accesores, no los Lerp de Color/Vector2/3/4 ni GodRayParam::operator=); no commiteado.
- MapPartsArea::vf1C (99,90 %): `lwzx base, offset` del original frente a offset+base en las condiciones de los bucles interiores; probados begin()[i] y *(begin() + i) (peor).

## Cambios de splits/symbols
- Sin cambios de splits. symbols.txt: `fn_800431F4__9ArenaTaskFii` -> `fn_800431F4__9ArenaTaskFiii`; `fn_803C795C` -> `GetData_unk__Q23atn31SimpleArray<Q22gs11NpcSetPoint>CFv`; renumerados `@N` de GameChoiceWindow, ReplayBuffer, GameScene y GameTask (pd.sh; 0x808857E0/E4 de GameTask a mano con lblmap: @31593/@31594). `lbl_8087EE78` devuelto tras pd de ArenaTask.
