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
- game/GameSaveBanner: 8/9. fn_803B9638 80.77 -> 88.2%: `mIconSpeed = mIconSpeed & ~...` (no `&=`, evita el not/clrlwi) y `mFlags = mFlags | m144 | 1` (orden del or). Pendiente: el bucle de iconos usa desplazamientos sobre this (i*8, i*4, i*0x1200) en el original y orden de args de NANDInitBanner.
- game/Gun: saltada - sub-bloqueo B de `__VecCopy` (orden de z; ya probadas muchas formas segun su comentario).
- game/HomeButtonMenu: ENLAZADA (5/5). fn_80440994 (0x80440994) subida: `w /= sy * (k * sx); h *= sy;` (la division antes de la multiplicacion de h y sin local para k*sx). Faltaba la definicion de `HomeButtonMenu::sInstance` (la usa GameSystem_unk); anadida en el .cpp. Sin cambios en splits/symbols.
- game/IffChunkFile_unk: 4/5. ChunkFile_unk ctor 86.62 -> 98.97%: los intercambios in situ como `*p = Swap32_unk(*p)` (con el intrinseco `__stwbrx` salian lwzu en vez de addi) y en el bucle del rama swapped el desplazamiento y el valor apuntado en locales con nombre (`off`, `w`). Queda: r3/r6 y r4/r5 cruzados en m18 y en algunos swaps.
- game/LastCocoonChrysalis: saltada - presupuesto de inline (std::string/tokenizer fuera de linea; mismo caso que CharaTask::vf2C).
- game/MapPartsEfp: 5/6, sin cambios. vf10 98.65%: un helper inline con los tres temporales en la llamada arregla los FPR pero guarda el color despues de rot/pos (mismo %); revertido, anotado en el comentario.
- game/MapPartsRain: saltada (marcada SALTAR en la cola).
- game/PickTask: saltada - vf10 (3868 B) 96.5% de asignacion de registros/spills de bucles ya investigada; no compensa con 10 intentos.
- game/StrapTask: 5/6. vf14 83.5 -> 85.9%: el color de cada linea en un local con nombre arregla la recarga de la direccion de la linea tras la llamada de ancho. Queda: FPR de alpha (f31 vs f29) y planificacion de los ratios de pantalla (los cambios de orden de HomeButtonMenu no ayudan aqui).
- game/TownTpcLoader: 3/4, sin cambios. vf0C 99.59% (puntero/contador del ultimo bucle r29/r30); 10 variantes (unsigned, local para el elemento, mData directo, puntero que avanza, contador compartido, ready==1...).
- chr/ControlDropDown: saltada - ctor: z guardada del temporal + mChara en r6 (familia `__VecCopy`); vf0C: slots de pila de las sondas.
- chr/ControlTurnQuadruped: ctor 99.92% sin cambios (el original guarda el double directamente con stfs f1 y convierte en otro registro para comparar); 8 variantes (local float antes/despues, comparar el double, mAngle, ternario, casts). vf0C 85% (slots de pila de matrices) no intentada.

Hallazgos MWCC (2):
- `w /= sy * (k * sx); h *= sy;` (division antes de la multiplicacion, sin local para k*sx) reproduce los fmr y el orden del original en HomeButtonMenu::fn_80440994.
- Byte swap in situ: `*p = swap(*p)` con la funcion rlwimi da `lwz/addi/stwbrx`; el intrinseco `__stwbrx(*p, p, 0)` da `lwzu`.
