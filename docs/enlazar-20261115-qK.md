# enlazar-20261115, cola K (segunda pasada de personajes)

Agente K, rama `agent-K`. Inicio 16:21.

## Resultado por unidad

| Unidad | Antes | Despues | Estado |
|---|---|---|---|
| chr/ControlCdnStone | 56/57 | 57/57 | no enlaza (datos, ver Pendientes): CdnSleep::vf10 al 100 % (accesor `GetModel_unk()` en el ctor inline de CdnRestoreSleep) |
| chr/ControlDraggingStart | 10/11 | 11/11 | enlaza: DraggedStart::vf10 al 100 % (mismo truco en el ctor inline de ControlDragged) |
| chr/ControlChaseCaught | 29/39 | 33/39 | ChaseHeld/ChaseAssist/ChaseCatch/ChaseHold::vf10 al 100 % (mismo truco en los ctors) |
| chr/ControlJumpAt | 23/28 | 24/28 | JumpAt::vf10 al 100 % (mismo truco en ControlGrapple/GrappleThrown + renombrado de la constante ptmf) |

## Unidades que enlazan (bytes)

- chr/ControlDraggingStart (688 B de datos). CdnStone (3304 B) tiene todas las funciones al 100 % pero el
  `.dol` no sale identico (ver Pendientes): sigue con `matching: false`.

## NONMATCHING con motivo

- ControlSlip::vf0C (99,9): el original carga mChara en r5 (r3/r4 ocupados por los argumentos de
  fn_80097D7C ya preparados). Probado ademas de lo de qG: `GetModel_unk()`, orden de declaracion de
  motion/frame, declarar y asignar despues, local `chara`, frame por `motion->`.
- ControlWallUp::vf10 (99,83): r5/r6 y huecos de pila del `rot` del ctor inline de ControlWallDown; probado
  `GetModel_unk()`, `Vector3 rot(...)`, declarar `rot` antes, `SetRot_unk`, `*GetRot_unk()`.

## Cambios de splits

- Ninguno. En `symbols.txt` solo renombrados (re-emparejado de `pdbatch.sh`) y `lbl_80780BA0` ->
  `@13391` (constante ptmf de `ControlJumpAt::vf10`, 12 bytes en `.data`, la que emite nuestro .o).

## Para el port

- Nada (ninguna unidad del port tocada). Cabeceras tocadas: `chr/ControlDraggingStart.h`,
  `chr/ControlJumpAt.h` (solo `&mChara->mMotion` -> `mChara->GetModel_unk()`, mismo valor).

## Decisiones tomadas sin el usuario

- `Motion_unk* motion = mChara->GetModel_unk();` (accesor ya existente en `CharaTask.h`, devuelve
  `&mMotion`) en lugar de `&mChara->mMotion` en los ctors que se inlinean en un `vf10` con `boost::bind`.

## Hallazgos (una linea cada uno)

- Copia en el heap de `boost::function` en r29 en vez de r30 (bloqueo comun de los `vf10` que hacen
  `new X(mChara)` con un ctor inline que guarda `&mChara->mMotion` en un registro salvado): escribir el
  puntero con el accesor inline `mChara->GetModel_unk()` en el ctor; el codigo del ctor no cambia y el
  temporal del functor reutiliza el registro de `next` como el original (CdnSleep, DraggedStart,
  ChaseHeld/Assist/Catch/Hold, JumpAt).
- El residuo no se mueve con nada de `vf10` (new dentro del bind, locales, `boost::function` explicito),
  ni con el `allocate` de `msl_shim/memory`, ni con otras versiones del compilador (Wii 1.0-1.7 dan lo mismo).

## Pendientes

- chr/ControlCdnStone (57/57): con `matching: true` el `.dol` difiere solo en sus datos: `.sdata2` (el
  original empieza por 150.0f/450.0f, los nuestros salen tras -pi/2: los usa una funcion posterior o un inline),
  una cadena de `.rodata` (0x807392E5) y punteros de RTTI de `.data` desplazados 4 bytes; nuestro .o tiene
  `.rodata` 0x6B8 / `.data` 0x6BA / `.bss` 0x40 frente a 0x5D0 / 0x690 / 0x38 del original.
