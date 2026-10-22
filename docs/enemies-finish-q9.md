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

## Hallazgos MWCC

- Relectura de un miembro tras una prueba de rango inline: un accesor inline `GetStateOf_unk(this)` (funcion libre)
  da la recarga del original donde `m58C` directo reutiliza el registro (EnemyTasha::vf10C).
- `float y = mRot.y; y += k; mRot.y = WrapAngle(y);` da el `fadds f1, f1, f0` del original; `mRot.y + k` o
  `k + mRot.y` invierten los operandos.

## Pendientes para otras colas
