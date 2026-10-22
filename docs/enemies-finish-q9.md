# Lote enemies-finish, cola q9 (EnemyQuarkStand, EnemyTasha)

Inicio: 2026-10-22 14:38 CEST. Fin: 2026-10-22 20:55 CEST. Rama agent-q9 (sale de main 8274a20; referencia start-q9: 7061 funciones al 100%).
Cola: game/EnemyQuarkStand, game/EnemyTasha (las dos SIN FUENTE al empezar).

## Unidades

### game/EnemyTasha (0/25 -> 20/25)
- Unidad SIN FUENTE hasta ahora: fuente nueva `src/game/EnemyTasha.cpp` y cabecera nueva
  `include/game/EnemyTasha.h` (subclase de SpecialEnemyBase: el ctor llama a `__ct__16SpecialEnemyBase` y la
  vtable tiene 0x11C bytes; layout 0x14D4-0x16C8 deducido del ctor, del dtor y de las claves del CSV; nombres
  inventados). Entrada nueva en `config/SLSP01/game_units.json` (matching: false). Las 25 funciones escritas;
  unidad al 99.65% (fuzzy), sin enlazar.
- Al 100% (report): ctor, dtor, vf0C, lector del CSV fn_80355DD8, vf10C (filtro de golpes segun la postura),
  vf40, vf100 (switch de estados 6-18 y 2), vf30 (menu "EnemyTasha"), vf68, vf6C, vfFC, fn_80357330,
  fn_8035738C (giro hacia el objetivo), fn_8035A9F4, fn_8035ACB8, fn_8035ACF0 (efectos por estado) y las copias
  debiles fn_80356FFC (accesor de lbl_8087F9E8), fn_80357F38, `TashaStep_unk::Reset_unk` (0x80357ED8) y
  `TashaDamage_unk` ctor (0x80357F04).
- NONMATCHING:
  - vf10 99.97%: r29/r30 del manager de efectos en el dano de evento (el original mantiene vivo el puntero al
    evento en r30); ~8 variantes (inline, locales, orden).
  - vf104 99.92% (maquina de estados): r29/r30 intercambiados en el numero de ataques (2/4) y en el temporal
    del giro del estado 14 (fn_8035738C expandido), r3/r4 de la copia de la diferencia al objetivo; ~12
    variantes.
  - vf108 99.66% (estados): slots de pila de los temporales del efecto del estado 14 y numeracion de FPR del
    lerp del warp (estado 18) y de las pruebas de frame; ~15 variantes. El original expande fn_8035738C,
    fn_8035ACB8 y fn_8035ACF0 (vf108 esta justo en el limite del presupuesto de inline: con ayudantes
    inline propios se pasa y los saca fuera de linea).
  - fn_803576F8 97.20% (destino del warp): numeracion de FPR de las constantes del bucle y slots de pila de los
    temporales de giro; ~8 variantes.
  - fn_8035AB78 90.79%: el original lleva el desplazamiento en bytes del evento como variable de induccion propia
    y vuelve a sumar `this` en cada vuelta; ~10 formas del bucle.

### game/EnemyQuarkStand (0/80 -> 55/80)
- Unidad SIN FUENTE hasta ahora: fuente nueva `src/game/EnemyQuarkStand.cpp` y cabecera nueva
  `include/game/EnemyQuarkStand.h` (subclase de EnemyTask, 0x1CF0 bytes; layout deducido del ctor, del dtor,
  de las claves del CSV y de los estados; nombres inventados). La entrada de `game_units.json` ya existia.
  Las 80 funciones escritas; unidad al 97.75% (fuzzy), sin enlazar.
- Al 100% (report): ctor (tokenizer del CSV), dtor, vf0C, vf3C, vf40, vf64, vf78, vf9C, vfE0/vfDC/vfD4,
  vfB0, vfB4, vf98, fn_80346D8C (eleccion del ataque), fn_8034701C, fn_803470F0, fn_80348568 (caminar),
  fn_803489F4, fn_80348FF4, fn_8034A194, fn_8034A644, fn_8034C060, fn_8034CF48, fn_8034DA0C, fn_8034E898,
  fn_8034EA50, fn_8034F384, fn_8034F41C (estado 0), fn_8034F53C, fn_803519C4, fn_803520D8, fn_8035223C,
  fn_803524AC, fn_803524E4, fn_803526A8, fn_80352C3C, fn_80352EA4, fn_80353370, fn_80353EE4, fn_80353F0C,
  fn_80354110, ctores debiles de los miembros, invoke/manage de boost de los binds, y las copias debiles
  QuarkBulletShot_unk ctor (0x80349424) y FixedArray_unk<QuarkShot_unk,32>::full/PushBack_unk.
- NONMATCHING (% del report):
  - vf10 98.62% (antes 41.9%): con todos los estados escritos el presupuesto de inline ya coincide; queda el
    flag `camera` en r29 (nuestro r30) y el grupo del mapa.
  - fn_803458EC 99.79%: push_back de vector<int> inline (familia conocida: carga capacidad/tamano invertida).
  - vf50 99.54%, vf54 96.73%: slots de la direccion frontal / registros de la copia de nodos (familia
    __VecCopy).
  - fn_803494AC 98.11% (onda sonica): el original reserva 32 bytes mas de pila (stores muertos) y numera
    distinto los FPR del lerp; ~6 variantes.
  - fn_80349BA0 95.41%: numeracion de registros del bucle (r20-r31) y pila de los temporales de rotacion.
  - fn_8034AE48 98.24%: el marco del original es 24 bytes mayor sobre los locales de AimAt_unk.
  - fn_8034B8CC 86.66%, fn_8034ECE8 89.78%: copia de los parametros de camara por defecto (Field_unk::m588)
    en f29-f31 (caso abierto, como EnemyTrista::fn_8035D294).
  - fn_8034C714 89.10%: el original guarda el origen de la lluvia y la escala en f20-f31 (doce FPR).
  - fn_8034D450 96.04%: el original usa un temporal de pila por bucle para la onda.
  - fn_8034DE90 95.29%: numeracion de registros de las copias implicitas de Camera_unk.
  - fn_8034F788 98.46%, fn_80350070 98.85%, fn_80350464 98.81%, fn_8035083C 98.77%, fn_8034FB50 96.26%,
    fn_80350F64 96.25%: familia de slots de pila del apuntado (AimAt_unk: copia del objetivo y temporales de
    ToRotationZ_unk); ~3-8 formas cada una.
  - fn_80350BF0 99.86%: slots del producto vectorial de la primera rama.
  - fn_80351480 88.98%: orden de los productos dir * dist y slots del apuntado.
  - fn_80351AB8 99.41%: el original forma la direccion del slot (lbl_8087F4F0 + 0x8310) en r3 antes que r31.
  - fn_80352938 99.50%: slots de la diferencia y de la copia de ToRotationZ_unk intercambiados.
  - fn_8035359C 98.28%: slots de los dos vectores de distancia (0x28/0x34) y del bind.
  - vfD0 99.64% (0.6f/m5B4, como CharaTask::vfD0), vfD8 99.85% (slots).

## Decisiones tomadas sin el usuario

- **symbols.txt (EnemyTasha)**: las 25 funciones de 0x803559F8-0x8035B42C renombradas a metodos de EnemyTasha
  (ctor, dtor y virtuales por la vtable; el resto `fn_<addr>__10EnemyTashaF...`), 0x80357ED8 ->
  `Reset_unk__13TashaStep_unkFv`, 0x80357F04 -> `__ct__15TashaDamage_unkFv` (copias debiles de dos inlines);
  fn_80356FFC y fn_80357F38 siguen con su nombre C (las llaman tambien game/GameTask y game/TutorialTask, en asm).
  `lbl_8074B0A4` -> `@stringBase0` (scope:local, 0x2E0).
- game/SpecialEnemyBase.cpp: la factoria (case 64) crea `new (3, "", "", 0) EnemyTasha(parent, info)` en lugar del
  `extern "C"` fn_803559F8 (sin cambio de codigo: sigue en 73.27%).
- EnemyTasha: clases inventadas `TashaFlag_unk` (misma forma que SpecialFlag_unk, con ctor -1/0), `TashaEvent_unk`
  (linea "Event"), `TashaDamage_unk` (resultado de fn_8003EA3C, como DamageResult_unk de Em002Skelton) y
  `TashaStep_unk` (vista de SpecialEnemyBase::m14BC-m14D0 que vf100 reinicia en bloque: su Reset_unk es la copia
  debil 0x80357ED8). `fn_8003EA3C` y `fn_802657FC` declarados `extern "C"` en la fuente.
- vf10C: un accesor inline `GetStateOf_unk(this)` (lee m58C) reproduce la recarga del estado que hace el original
  tras la prueba del rango 11-13/15.

- **symbols.txt (EnemyQuarkStand)**: funciones de 0x80345384-0x803545E0 renombradas a metodos de
  EnemyQuarkStand (y a sus helpers `_unk`); `lbl_8074AA58` -> `@stringBase0` (scope:local, 0x4DE); objetos
  RTTI/@STRING@/@LOCAL@/@GUARD@ del bind de ControlThrustForQuarkStand renombrados. fn_803458EC sigue con su
  nombre C (lo llama vf0C con `extern "C"`).
- include/chr/ControlStickByQuark.h (compartido): declarada la clase `chr::ControlThrustForQuarkStand`
  (ctor 0x80194E2C) para el `new` de vfB4. Solo declaracion.
- include/game/CharaTask.h (compartido): `vfE0` devuelve `float` (lo exige EnemyQuarkStand::vfE0; los demas
  usos no cambian de codigo).
- include/game/HitInfo_unk.h (compartido): el hueco m1C se parte para exponer `int m3C` (tipo de bala que
  fn_80348FF4 pasa a fn_8010B250). Sin cambio de layout.
- EnemyQuarkStand: `IntVector_unk` (std::vector<int>, ctor 0x80288390 y dtor 0x8000D760 son copias debiles en
  otros ficheros), `Mtx34_unk` (ctor 0x803165E0 vacio, copia debil de Em316LaserSphere), `QuarkShot_unk`,
  `QuarkWave_unk`, `QuarkSlot_unk`, `QuarkDir_unk`, `QuarkStandState_unk` y `QuarkBulletShot_unk` (hereda de
  BulletShot_unk con el ctor de 0x80349424): nombres inventados.
- EnemyQuarkStand: tablas de ataque como array plano `m1560[9]` con accesor inline (una struct propia creaba
  su ctor/dtor y rompia los del enemigo).
- EnemyQuarkStand::vf0C recorre RainSkill con `EnemyQuarkStand* p = this; ...; p = (char*)p + 4` para
  reproducir el puntero de induccion del original.
- EnemyQuarkStand: statics de .sdata `sLaserNear_unk`... (lbl_8087DC80-9C) con nombres inventados; las dos
  copias 100/500 de fn_800F8524 son variables distintas en el original.
- EnemyQuarkStand: `QuarkGimmick_unk` (vista de GimmickCommon con el float de 0x920, como gmk::PassBoard) y
  una clase local `atn::EffectManager` con solo `fn_8023A254` (patron de Np131LivingArmor).
- EnemyQuarkStand: `fn_80352938` es la copia fuera de linea del giro; el inline `TurnTo_unk` se mantiene para
  fn_80348568/fn_8034C060 (el original lo expande alli).

- include/game/CharaTask.h y src/game/CharaTask.cpp (compartidos): `fn_8016C7D8` recibe `CharaTask* holder`
  (EnemyQuarkStand le pasa el objetivo); symbols.txt renombrado a `fn_8016C7D8__9CharaTaskFP9CharaTask`. La
  unidad CharaTask no cambia (99.68%).
- symbols.txt (EnemyQuarkStand, segunda pasada): fn_803489F4, fn_80352938, fn_8034C060, fn_80350BF0 y
  fn_8034D450 con su firma real; invoke/manage (0x80353C9C/0x80353CCC), `__RTTI` (lbl_80789540), `@LOCAL@`
  (lbl_807C8450) y `@GUARD@` (lbl_8087F411) del segundo bind renombrados a los nombres mangled de
  `bi148bind_t<...EnemyQuarkStand>`.
- EnemyQuarkStand: `Field_unk` local reestructurado (camara m06C Camera_unk, m4D8 QuarkFieldCamera_unk, m588
  QuarkCamParam_unk con ctor de copia, temblor m96C-m978, m97C QuarkFieldLook_unk, m10D8 RouteTable_unk*);
  clases locales QuarkTargets_unk, QuarkGimmick_unk, QuarkStickControl_unk, QuarkWeights_unk,
  QuarkEngineCamera_unk, QuarkGame_unk (lbl_8087EFA8 m3A4), QuarkSettings_unk y Xsca_unk/XsfCurveSet_unk
  locales: nombres inventados, pendientes de unificar con las cabeceras reales.
- EnemyTasha: `TashaGame_unk` (lbl_8087EFA8, m3A4 = escala de tiempo, como GameState_unk de EnemyTrista)
  declarado en la fuente; `fn_800F8C6C` declarado `extern "C"` devolviendo `int` (numero de impactos; en
  Em201ZanGurg esta como `void`).
- Cadenas corregidas en EnemyQuarkStand tras comparar los pools: vf40 usa "SE_DG040_109", vfB4 "SE_DG040_107"
  y vf50/vf54 el nodo "head".

## Hallazgos MWCC

- Relectura de un miembro tras una prueba de rango inline: un accesor inline `GetStateOf_unk(this)` (funcion libre)
  da la recarga del original donde `m58C` directo reutiliza el registro (EnemyTasha::vf10C).
- `float y = mRot.y; y += k; mRot.y = WrapAngle(y);` da el `fadds f1, f1, f0` del original; `mRot.y + k` o
  `k + mRot.y` invierten los operandos.

- Un `bool ok = false; if (cond) ok = true; return ok;` en un inline da el `li r0,0 ... li r0,1; cmpwi r0,0`
  del original (EnemyQuarkStand::fn_8034C060); `return cond;` da `mfcr/rlwinm`.
- Para un `a < b ? &x : &y` con dos sqrt, escribir los dos temporales como locales nombrados antes del test
  sube sus slots como en el original (fn_8034FB50).
- `float step = k * (2.0f * i); rot += step;` evita la fusion en fmadds que da `rot + k * (2.0f * i)`
  (fn_8035223C).
- Pasar `&(a + b)` y `&atn::Vector3(...)` directamente como argumentos coloca los temporales en el orden de
  pila del original cuando los locales nombrados salen invertidos (fn_8035223C).
- `mId` leido como `int` y comparado con constantes > 0x7FFF genera el `addis r0,r3,0; cmplwi` del original
  (fn_80352C3C); como `unsigned` se pierde el addis.

- `(b - a) * t + a` da el orden del lerp del original donde atn::Lerp_unk (`a + (b - a) * t`) lo invierte
  (EnemyQuarkStand); en EnemyTasha::vf108 es al reves: Lerp_unk con `t` en un local nombrado.
- `m16F8.RotZ_unk()` en el sitio (sin temporal) reproduce el giro de EnemyQuarkStand.
- Construir el boost::function en un local antes de asignarlo al control evita mantener vivo el control
  durante el bind (EnemyQuarkStand::fn_8035359C).
- Leer el frame en una variable antes de llamar a `fn_80097D7C` (`frame = ...m08; if (frame >= len)`) da el
  `lfs f22` previo a la llamada del original; leerlo dentro de la comparacion lo carga despues
  (EnemyTasha::vf108).
- En una funcion al borde del presupuesto de inline (EnemyTasha::vf108) cada ayudante inline propio cuenta:
  escribir las pruebas y las copias a mano devuelve los inline del original (fn_8035738C, fn_8035ACB8...).
- Declarar `n` y leer `m16C4` antes del `if (m14D4 >= 1) n = 4;` coloca la carga como el original
  (EnemyTasha::vf104).

## Pendientes para otras colas

- Unificar con la cola q8 (EnemyQuarkFirst/EnemyQuarkSecond): EnemyQuarkStand declara sus propias clases
  (`QuarkSlot_unk`, `QuarkShot_unk`, accesores fn_80338CDC/fn_80339xxx como `extern "C"`); cuando q8 tenga
  cabeceras, sustituirlas.
- `IntVector_unk` y `Mtx34_unk` son copias debiles de otros ficheros (Em203GreaterDaemon,
  Event/Puppeteer_CharaTask, Em316LaserSphere): unificar el nombre cuando esas unidades se decompilen.
- Ninguna unidad enlazada (matching: false): EnemyQuarkStand (25 NONMATCHING) y EnemyTasha (5 NONMATCHING)
  quedan por familias de slots de pila / numeracion de registros.
- Caso abierto de la copia de parametros de camara en FPR (EnemyQuarkStand::fn_8034B8CC/fn_8034ECE8, igual que
  EnemyTrista::fn_8035D294): resolverlo una vez para las tres.
- `fn_800F8C6C`: unificar la firma (int, numero de impactos) con game/Em201ZanGurg.cpp cuando haya cabecera
  del manager de balas (lbl_8087F048).
- `GameState_unk`/`TashaGame_unk`/`QuarkGame_unk` (lbl_8087EFA8): misma struct en tres fuentes; mover a una
  cabecera comun.

