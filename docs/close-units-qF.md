# Lote close-units, cola F (chr y game, 20 o mas funciones pendientes)

Inicio: 2026-10-22 05:39 CEST. Rama agent-F. La herramienta Projects no estaba disponible: reglas tomadas del
prompt del lote y de docs/.

Observacion general: de las 78 unidades de la cola solo 4 tienen fuente (game/AreaEffConfig,
game/GimmickStateCommand, game/SkillEffectManager, game/GameUI); el resto esta al 0% sin .cpp. Ninguna puede
cerrarse en el tiempo del lote; se prioriza (1) las funciones NONMATCHING >= 95% de las unidades con fuente y
(2) empezar las unidades sin fuente por la cabeza de la cola, escribiendo las funciones pequenas/medianas.

## Unidades

- game/AreaEffConfig: sin cambios. fn_80043014 99.57% (contador exterior / desplazamiento interior r27/r28):
  ~35 variantes (las 24 permutaciones de declaraciones de count/n/i/j, while, continue, unsigned, referencia
  local, mSize en la condicion). fn_80041D44 0%: presupuesto de inline (bloqueo documentado), no tocada.
- game/SkillEffectManager: sin cambios. fn_8059B66C 99.01% (parametros r29-r31/r25-r27 desplazados): 11
  variantes (bucle de FindFree_unk en la cabecera, retorno temprano, locales de user, m02 directo). vf10 y
  fn_8059B8A8 no intentadas (registros ya investigados).
- game/GimmickStateCommand: vf10 94.49% sin cambios (4 variantes del bucle de tablas: unsigned, continue,
  referencia, indice fuera). El resto: presupuesto de inline (documentado en la unidad).
- game/GameUI: no intentada (ver close-units-qC; ctor 98.1% de planificacion documentado).
- game/Em101ElderZesya (0/20 -> 7/20, NUEVA fuente `src/game/Em101ElderZesya.cpp` + cabecera
  `include/game/Em101ElderZesya.h`): al 100% Barrier_unk ctor/dtor (0x80255F2C/0x80255F68), dtor (0x80256014),
  vf0C (0x802561C4), vf3C (0x80259604), vf4C (0x80257A94), vf64 (0x80257A14) y `__sinit` (0x8025A03C).
  ctor 99.6% (r25/r26 del strlen de los parametros y del grupo del menu, igual que Em201ZanGurg), vf54 99.9%
  (el original recarga m624.mData antes de copiar el centro, mismo caso abierto que CharaTask::vf54).
  - symbols.txt: fn_802558B0 -> `__ct__15Em101ElderZesyaFPQ23atn4TaskP13EnemyInfo_unk`, fn_80255F2C/F68 ->
    `__ct__/__dt__Q215Em101ElderZesya11Barrier_unkFv`, fn_80256014 -> `__dt__15Em101ElderZesyaFv`,
    fn_802561C4 -> vf0C, fn_80257A14 -> vf64, fn_80257A94 -> vf4C, fn_80258368 -> vf54, fn_80259604 -> vf3C,
    fn_8025A03C -> `__sinit_\Em101ElderZesya_cpp` (scope:local).
  - src/game/SpecialEnemyBase.cpp: la factoria (case 7) usa `new (3, "", "", 0) Em101ElderZesya(...)` como con
    Em201ZanGurg (sin cambio de codigo generado).
  - Decision sin el usuario: el barrier guarda un `Gimmick_unk*` opaco (posicion en +4, rotacion en +0x14):
    incluir game/GimmickCommon.h choca con declaraciones locales de SpecialEnemyBase.cpp.
  - HitMsg_unk+0x40 se lee con un cast (`*(int*)&msg->m3C[4]`) para no tocar la cabecera compartida CharaTask.h.

## Hallazgos MWCC
- vf0C de los enemigos especiales: la cadena de cargas en un unico `if (a == 0 && b == 0 && ...)` reproduce el
  `beq` comun al `return 0` final; con retornos tempranos sale un `li r3,0; b` por condicion.
- Un switch con casos {22, 23, 20} donde 23 cae en el bloque comun: el orden de comparaciones del original
  (22, 23, 20) sale con `case 22: goto reset; case 23: ...; case 20: reset:`.
- Un accesor inline que devuelve `const Vector3&` (`GetPos_unk`) en vez de `p->mDesc->mPos` cambia el orden de
  carga (lwz del puntero antes del lis de la constante).
