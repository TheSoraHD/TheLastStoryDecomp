# Lote enemies-finish, cola q9 (EnemyQuarkStand, EnemyTasha)

Inicio: 2026-10-22 14:38 CEST. Rama agent-q9 (sale de main 8274a20; referencia start-q9: 7061 funciones al 100%).
Cola: game/EnemyQuarkStand, game/EnemyTasha (las dos SIN FUENTE al empezar).

## Unidades

### game/EnemyTasha (0/25 -> 20/25)
- Unidad SIN FUENTE hasta ahora: fuente nueva `src/game/EnemyTasha.cpp` y cabecera nueva
  `include/game/EnemyTasha.h` (subclase de SpecialEnemyBase: el ctor llama a `__ct__16SpecialEnemyBase` y la
  vtable tiene 0x11C bytes; layout 0x14D4-0x16C8 deducido del ctor, del dtor y de las claves del CSV; nombres
  inventados). Entrada nueva en `config/SLSP01/game_units.json` (matching: false).
- Al 100% (report): ctor, dtor, vf0C, lector del CSV fn_80355DD8, vf10C (filtro de golpes segun la postura),
  vf40, vf100 (switch de estados 6-18 y 2), vf30 (menu "EnemyTasha"), vf68, vf6C, vfFC, fn_80357330,
  fn_8035738C (giro hacia el objetivo), fn_8035A9F4, fn_8035ACB8, fn_8035ACF0 (efectos por estado) y las copias
  debiles fn_80356FFC (accesor de lbl_8087F9E8), fn_80357F38, `TashaStep_unk::Reset_unk` (0x80357ED8) y
  `TashaDamage_unk` ctor (0x80357F04).
- NONMATCHING:
  - vf10 99.97%: r29/r30 del manager de efectos en el dano de evento (el original mantiene vivo el puntero al
    evento en r30); ~8 variantes (inline, locales, orden).
  - fn_803576F8 97.18% (destino del warp): numeracion de FPR de las constantes del bucle y slots de pila de los
    temporales de giro; ~8 variantes.
  - fn_8035AB78 90.79%: el original lleva el desplazamiento en bytes del evento como variable de induccion propia
    y vuelve a sumar `this` en cada vuelta; ~10 formas del bucle.
- Sin escribir: vf104 (0x80357F48, 2.8 KB) y vf108 (0x80358A5C, 8 KB).

### game/EnemyQuarkStand (0/80 -> 38/80)
- Unidad SIN FUENTE hasta ahora: fuente nueva `src/game/EnemyQuarkStand.cpp` y cabecera nueva
  `include/game/EnemyQuarkStand.h` (subclase de EnemyTask, 0x1CF0 bytes; layout deducido del ctor, del dtor,
  de las claves del CSV y de los estados; nombres inventados). La entrada de `game_units.json` ya existia.
- Al 100% (report): ctor (tokenizer del CSV), dtor, vf0C, vf3C, vf40, vf64, vf78, vf9C, vfE0/vfDC/vfD4,
  fn_80346D8C (eleccion del ataque), fn_8034701C, fn_80348568 (caminar), fn_8034F384, fn_8034F41C (estado 0),
  fn_803519C4, fn_803524AC, fn_80352EA4, fn_80353EE4, fn_80353F0C, fn_80354110, ctores debiles de los
  miembros, invoke/manage de boost del bind de vfB4, y (instrucciones identicas, solo offsets del pool hasta
  ahora) fn_803470F0, fn_803489F4, fn_80348FF4, fn_8034C060, fn_8034E898, fn_8034EA50, fn_8034F53C,
  fn_803520D8, fn_8035223C, fn_803524E4, fn_803526A8, fn_80352C3C, fn_80353370, vfB0, vfB4, vf98 y las
  copias debiles QuarkBulletShot_unk ctor (0x80349424) y FixedArray_unk<QuarkShot_unk,32>::full/PushBack_unk.
- NONMATCHING:
  - vf10 41.9%: el original esta por encima del presupuesto de inline y llama fuera de linea a los ayudantes
    de estado; el nuestro aun no (faltan estados en C). Se revisara cuando esten todos.
  - fn_803458EC 99.73%: push_back de vector<int> inline (familia conocida: carga capacidad/tamano invertida).
  - vf50 99.50%, vf54 95.35%: slots de la direccion frontal / registros de la copia de nodos (familia __VecCopy).
  - fn_803494AC 97.78% (onda sonica): el original reserva 32 bytes mas de pila (stores muertos) y numera
    distinto los FPR del lerp; ~6 variantes.
  - fn_80350070 98.62%, fn_80350464, fn_8035083C ~98.6%, fn_8034F788 98.28%, fn_8034FB50 96.11%,
    fn_80350F64 96.11%, fn_80351480 88.77%: misma familia de slots de pila del apuntado (AimAt_unk:
    copia del objetivo y temporales de ToRotationZ_unk); ~3-8 formas cada una.
  - fn_80350BF0 99.66%: slots del producto vectorial de la primera rama.
  - fn_80352938 99.11%: slots de la diferencia y de la copia de ToRotationZ_unk intercambiados.
  - vfD0 99.45% (0.6f/m5B4, como CharaTask::vfD0), vfD8 99.53% (slots).
- Sin escribir: fn_80349BA0, fn_8034A194, fn_8034A644, fn_8034AE48, fn_8034B8CC, fn_8034C714, fn_8034CF48,
  fn_8034D450, fn_8034DA0C, fn_8034DE90, fn_8034ECE8, fn_80351AB8, fn_8035359C (y su bind
  fn_80353C9C/fn_80353CCC).

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

## Pendientes para otras colas

- Unificar con la cola q8 (EnemyQuarkFirst/EnemyQuarkSecond): EnemyQuarkStand declara sus propias clases
  (`QuarkSlot_unk`, `QuarkShot_unk`, accesores fn_80338CDC/fn_80339xxx como `extern "C"`); cuando q8 tenga
  cabeceras, sustituirlas.
- `IntVector_unk` y `Mtx34_unk` son copias debiles de otros ficheros (Em203GreaterDaemon,
  Event/Puppeteer_CharaTask, Em316LaserSphere): unificar el nombre cuando esas unidades se decompilen.
- vf10 de EnemyQuarkStand (41.9%) depende del presupuesto de inline: revisar cuando todos los estados esten
  escritos.

