# Lote paralelo 2026-11-12, cola E2 (Last Cocoon, segunda pasada)

Inicio: 2026-11-12 22:21 (reloj del contenedor). Rama `agent-E2`.

## game/LastCocoonQuadruped

- fn_80461838 (movimiento de los modos 0/3/7, como `Em403WarLion::fn_803241E8`): 100 %.
- fn_80462038 (como `Em403WarLion::fn_80324A38`, con `vf24()` y sin la bandera de aterrizaje): 98,1 %
  (registros del giro y `frsp` del clamp de m584, igual que en WarLion). Declarada como miembro
  `fn_80462038(const atn::Vector3&, int, float, float)`; renombrada en `symbols.txt`.
- fn_80465B88 (modo 16, sobre el gimmick 0x455): 97,6 % (ranuras de pila de la rama de salto y el orden
  de m15BC en la del modo 17). `QuadHoming_unk` (valores por defecto de `HomingParam_unk`) y `QuadShot_unk`
  (copia de los parametros del disparo construida en sitio: `Bullet::GetShot_unk` hace una copia de mas).
- Sin escribir: 3 -> 0. 100 %: 38 -> 39 / 54.

## game/LastCocoonImago

- fn_8045DF34 (modo 11: lleva el gimmick m18F8, dispara 0x5BD y espera el empuje
  `chr::ControlThrustForCocoon`): 97 % (pila y orden de la primera interpolacion). fn_8045ECC4 pasa a 100 %.
- Sin escribir: 13 -> 12. 100 %: 17 -> 18 / 36.

## game/LastCocoonEgg

- fn_80454D4C (matrices del modelo de depuracion m14EC): 100 %. La escala maxima sale con
  `EggColumn_unk(mtx, i).Length_unk()` (columna devuelta por valor) en un inline: con el constructor de
  `Vector3` directo cambian los registros, y fuera de un inline se invierten las ranuras de pila.
- fn_8045531C (color del material "light"): 99,7 % (offset de "light" en el pool de cadenas y el registro
  del factor del brillo). `EggColor_unk` como `LeoColor_unk`; `sGlowColor_unk` se define antes de la funcion.
- fn_80454F08 (flota sobre el lugar 601 con el temblor m2548 y la escala m540): 98,9 % (registros de la
  busqueda del lugar, pila del desplazamiento del temblor). El desplazamiento sale de un inline que devuelve
  `Vector3` (`EggShakeOffset_unk`): el original guarda la direccion del resultado en r31.
- Sin escribir: 17 -> 14. 100 %: 17 -> 21 / 42 (fnstat).

## Decisiones tomadas sin el usuario

- `__declspec(noinline)` en `LastCocoonImago::fn_8045ECC4` (el original la llama con `bl` desde fn_8045DF34;
  excepcion aceptada de la regla 2).
- Includes nuevos en fuentes de la familia: `game/GimmickManager.h` y `macros.h` (Quadruped),
  `CharaManager.h`, `PlayerTask.h`, `GameUI.h`, `GameConfig_unk.h`, `chr/ControlStickByQuark.h` (Imago),
  `macros.h`, `atn/Material_unk.h` (Egg). Renumeran las `@N`: `pd.sh` lo arregla salvo las tres etiquetas
  ambiguas de Egg (0x80886B78/80/90), renombradas a mano en cada commit (ahora @12603/@12605/@12828).
- Renombrados en `symbols.txt`: fn_80462038, fn_8045DF34, fn_80454D4C, fn_8045531C, fn_80454F08 a sus
  nombres mangled de miembro.
- `EggField_unk` (vista local de lbl_8087F430 en Egg) gana `m10D8` (tabla de rutas).
- `extern "C" lbl_8087F0A8` de Quadruped movida antes de fn_80462038 (solo la declaracion).

## Hallazgos

- `Bullet::GetShot_unk().m0C` deja una copia de mas del struct; el original construye la copia en sitio
  (ctor con lista de inicializacion).
- `(mPos - pos) * k + pos` da el orden de operandos `t + pos` del original (no `pos + (mPos - pos) * k`).
- `(m9C & 1u) != 1` da el `cmplwi` del original en el movimiento de los cuadrupedos (WarLion tiene `cmpwi`).
- poolcheck de Egg avisa de `?333Dz` (0.7f y 1000.0f vistos como cadena) tras fn_80454F08; floats 25/20
  ya salian antes.

## Pendientes

- Egg: vf0C (presupuesto), fn_80453E20, vf14, fn_80455638, fn_804562E4, fn_804570F4, fn_804576D4,
  fn_80457E98 y las copias debiles.
- Imago: ctor, vf10 0x8045883C, vf40 0x804596E8, vf50 0x80459FD0, fn_8045ABB4, fn_8045B82C, fn_8045BFFC,
  fn_8045CB24, fn_8045D3A4, fn_8045EDEC (dibujo de avisos en pantalla con GameUI::fn_803E3050);
  0x80459FB8 (vfBC con argumento, firma compartida).
