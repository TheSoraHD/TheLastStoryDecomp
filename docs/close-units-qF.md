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

- game/Em101ElderZesya (segunda pasada, 8/20): `fn_802598DC__15Em101ElderZesyaFv` (punto de canto mas cercano
  al centro del grupo) 99.22%: copias de vector r3/r4 (familia `__VecCopy`) + orden de incrementos del bucle.
  Con un inline `PartyCount_unk()` (carga de lbl_8087F8A0 repetida) el resto coincide.
- game/Em205Ogre (0/21 -> 14/21, NUEVA fuente `src/game/Em205Ogre.cpp` + `include/game/Em205Ogre.h`):
  al 100% ctor (0x802B77AC), dtor (0x802BA898), vf0C, vf10, vf14, vf40, vf54, vf7C, vfFC, vf100 (0x802B7F94),
  vf10C (0x802B7DEC), lector del CSV (0x802B7AF8), busqueda del circulo de curacion (0x802B9C5C) y el `get`
  debil del scoped_ptr (0x802B98A8). vf30 99.97% (desplazamiento de "Em205 Ogre[%d]" en el pool, cadenas de
  funciones aun sin escribir). fn_802B98B0 (capsula) 98.59%: familia `__VecCopy` + planificacion de FPR.
  - symbols.txt: fn_802B77AC/BA898/7970/7CE0/7DE8/9BC8/7E60/BA5E0/7F04/7F4C/7DEC/7F94/7AF8/9C5C/98B0 ->
    nombres de Em205Ogre; fn_802B98A8 -> `get__Q25boost27scoped_ptr<13ResLoader_unk>CFv`.
  - SpecialEnemyBase.cpp: factoria case 23 con `new (3, "", "", 0) Em205Ogre(...)`.
  - Pendientes: vf50 (0x802B9DB4, recrea m624 con push_back en linea: bloqueo documentado), vf104/vf108
    (0x802B8398/0x802B8AA4, grandes), slot 0xB0 (0x802BA64C: CharaTask::vfB0 esta declarada `void vfB0()`; su
    firma real devuelve int y recibe un Vector3*; no se cambia la cabecera compartida), vfB4 (0x802BA67C, bind
    de ControlWallSlash).

## Hallazgos MWCC
- vf0C de los enemigos especiales: la cadena de cargas en un unico `if (a == 0 && b == 0 && ...)` reproduce el
  `beq` comun al `return 0` final; con retornos tempranos sale un `li r3,0; b` por condicion.
- Un switch con casos {22, 23, 20} donde 23 cae en el bloque comun: el orden de comparaciones del original
  (22, 23, 20) sale con `case 22: goto reset; case 23: ...; case 20: reset:`.
- Un accesor inline que devuelve `const Vector3&` (`GetPos_unk`) en vez de `p->mDesc->mPos` cambia el orden de
  carga (lwz del puntero antes del lis de la constante).
- `if (p != 0) {...} else { vf100(6); break; }` en vez de `if (p == 0) { vf100(6); break; }` reproduce el orden
  de bloques del original (rama de exito primero).
- `m620 = r; m614 = base + off; m614.y += m620;` (miembro asignado antes de la suma de vectores) fija el orden
  de cargas de FPR del original en el calculo de la capsula.
