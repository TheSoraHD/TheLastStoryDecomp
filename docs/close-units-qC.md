# Lote close-units - cola C (chr y game)

Una linea por unidad procesada.

- chr/ControlAttractHateCharge: 7/8. Subida `ControlOverdrive::ControlOverdrive` (0x801B77DC): la llamada a fn_8021FD7C es el metodo `VoiceEntry_unk::fn_8021FD7C(int) const` (atn/ptr_string_base_char_0.cpp), declarado localmente y llamado sobre `(VoiceEntry_unk*)mChara->m7C`. Pendiente `ControlOverdrive::vf0C` 99.87%: la comparacion muerta (0.2 contra el tiempo de frame) usa f0/f1 al reves; 10 variantes probadas (orden de operandos, >=, !<=, ternario, helpers min, locales para 0.1/0.2, resta separada), ninguna. No enlaza.
- chr/ControlCaught: ENLAZADA (10/10). Ctor (0x801A2290) subido usando el accessor `IsOnLedge_unk()` para el test de mFlag31 (recarga la palabra, como en ControlDetect). symbols.txt: lbl_8077F190 -> `__RTTI__Q35boost3_bi162bind_t<...ControlRelease>>>`, lbl_8077F288 -> `@STRING@allocate__Q23std188allocator<...ControlRelease>>>>FUlPCv`, y lbl_8077DC48 (en la .data de chr/ControlCdnStone) -> `@STRING@allocate__Q23std190allocator<...ControlKnockdown>>>>FUlPCv`: el original deduplica el @STRING debil del bind de ControlKnockdown en la primera TU que lo emite; sin el nombre real nuestro .o lo anadia al final de su .data y desplazaba todo. No se uso `pairnames --apply` (empareja mal invoke/manage de ControlRelease con los de ControlKnockdown).
- chr/ControlDetect: saltada - familia `__VecCopy` r3/r4 (mwcc-blockers-q3, sub-bloqueo A).
- chr/ControlDraggingStart: 10/11, sin cambios. `ControlDraggedStart::vf10` 99.73% (copia del bind en r29/r30); 10 variantes (cast a ControlBase*, new dentro del bind, locales CharaTask*/Chara_unk*, boost::function explicito, ctor de ControlDragged via chara/mChara o fuera de linea). Comentario actualizado.
- chr/ControlOneMotion: saltada - familia `__VecCopy` r3/r4 (mwcc-blockers-q3).
- chr/ControlShoot: 2/3, sin cambios. Ctor 98.76%; 10 variantes (ternario/if-else/decremento del id de motion, local para el motion, orden de los case, local cero y suma invertida en mFB4). Comentario actualizado.
- chr/ControlSitting: 2/3. vf0C 95.55 -> 95.61%: la constante de fmod del original es `(double)6.2831855f` (0x401921FB60000000), no el doble exacto; suma del lerp invertida (`(mStart - mSeat) * t + mSeat`). Pendiente: this en r31/motion en r30 y slots de pila de temporales.
- chr/ControlUseSkill: saltada - familia `__VecCopy` r3/r4.
- game/BattleResult: saltada - `push_back` en bucle (bucle de miembros; registros de contadores/iteradores).
- game/CopyRobotTask: 7/8, sin cambios. vf10 99.84% (ToRotation_unk inline reconvierte z). Probadas 12 formas locales (no se toca math/Vec.h, compartida por 36 unidades). Comentario actualizado.
- game/CrossbowMenuTask: saltada - `push_back` en bucle (r28/r29 del primer bucle).
- game/Em209BigGolem: saltada - familia `__VecCopy` r3/r4 (mwcc-blockers-q3).
- game/EnemyLizerdBlade: 12/13. fn_80334460 91.36 -> 93.36%: locales `dz`/`dx` antes de declarar `dir` y clamp con ternario `t = t > 1.0f ? 1.0f : t`.

Hallazgos MWCC:
- Constantes dobles de fmod: comprobar el valor exacto en el pool; ControlSitting usa `(double)6.2831855f`, no 2*pi en doble.
- Un `@STRING@allocate...` debil de boost::function (o RTTI debil del bind) que el original deduplica en otra TU debe llevar su nombre real en symbols.txt; si no, nuestro .o lo emite al final de su .data y el .dol se desplaza.
