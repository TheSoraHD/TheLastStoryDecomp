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
- game/CharaC58_unk: sin cambios. fn_8011CAE4 99.46%: 9 variantes del bucle de busqueda (comparacion invertida, puntero+i, int, break con indice, puntero a la tabla, locales id/sid/cid); la mejor deja 10 lineas (base 12) sin llegar. fn_8011CD84 no intentada (97.97%, varias causas).
- game/CrowdSimulation: sin cambios. vf10 98.89% (f5/f6 entre la resta z y el radio); 11 variantes (orden radio/diferencia, local del radio, fn_805F9920 directo, diff con nombre/copia). Ctor no intentado (offset doble del grupo + destruccion de boost::function).
- game/Em002_01BlackSkelton: sin cambios. vf0C 94.38% (r5/r8 de la palabra de flags 0x12A4; 10 variantes de orden de flags, shrink/clear, or directo). vf10 96.1%: llamar a Erase_unk a traves de `CharaList_unk* l = &m14BC` dentro del if da el recalculo del final, pero cambia this/busy (r30/r31); revertido y anotado.
- game/PlayerManager: fn_80549F14 99.49 -> 99.66%: local propio en la rama else (`PlayerTask* p`). Queda el id en r29 (r30 en el original, compartido con head).
- game/SelectMember: 11/12. fn_80572450 (0x80572450) SUBIDA: el conteo de seleccionados escrito a mano en vez de `CountSelected_unk()` (con el inline el contador sale en r25 en vez de r22) y la llamada es `PlayerManager::fn_8054A33C(const char*)` (vista local de la clase; el id del miembro se pasa como nombre). Ctor 99.64% sin cambios (lista r30/r31; 4 variantes nuevas: lista reasignada, slot puntero, do-while, local).

Hallazgos MWCC (3):
- Un inline que cuenta (bucle con contador local) asigna el contador antes que los demas registros salvados; escrito a mano en el cuerpo, el contador toma el registro mas bajo (r22), como en el original de SelectMember::fn_80572450.
- game/GameSystem_unk: sin cambios. fn_80570CF8 99.93% (mFade/sInstance r3/r4 en el bloque del fade; 10 variantes: locales fade/menu, ifs anidados, or partido). fn_80570A74 no intentada (89%, registros de contadores ya investigados).
- game/HomeButton_unk: sin cambios. fn_804411DC 99.76% (f0/f1 del stick; 3 variantes nuevas ademas de las del comentario). fn_804413D4 (84.5%) no intentada.
- game/SystemErrorMessage_unk: fn_80363DBC 99.47 -> 99.77%: `input = lbl_8087F018;` de nuevo antes de leer m40C0 (el original recarga). Queda: GetExt_unk usa la primera carga (r4) en el original; mError recargado; registros de 3/0.
- chr/ControlGmkCoffinOpening: ctor ControlGmkCoffinOpening 99.46 -> 99.64%: el id 0x5B en un local (`int id = 0x5B; mChara->m560 = id;`) arregla r10; quedan r9/r11 cruzados. Los otros dos ctors no intentados (93.6% y borrador 58%).
- chr/ControlRide: saltada - ctor: r3/r6 de la copia `mStartPos = mBeast->mPos` (familia `__VecCopy`); vf0C/vf10 slots de pila / planificacion ya documentados.
- chr/ControlStagger: sin cambios. vf0C 99.26%: `mTime--` antes de copiar la rotacion da mChara en r6 pero mueve el decremento (92.8%); revertido y anotado. ControlSlip ctor = familia `__VecCopy`; ControlSlip::vf0C no intentada.
- game/ArenaTask: 10/12. fn_80043D58 (0x80043D58) SUBIDA: comprobar `voices.mCount == 0` y despues leer `unsigned int n = voices.mCount` (el registro del contador se reutiliza para el elemento, como en el original). fn_80043A24 98.82% (2 variantes nuevas sin mejora) y vf10 99.43% sin cambios.
- game/ChaseManager: sin cambios. fn_80183948 99.22% (0/1 en r0/r4 del inline fn_80182874; 5 variantes nuevas: cuerpo directo, retorno temprano, locales cero/uno). fn_80183618 y vf10 no intentadas (ya muy investigadas).
- game/ColliAttrManager: fn_80183D00 99.72 -> 99.81%: el nombre del efp se guarda en el propio `attr` (el original reutiliza su registro). Queda el indice del primer bucle (r29 vs r30); 10 variantes. fn_80183FFC/fn_80184864 no intentadas (registros / `__VecCopy`).
- game/CrowdManager: no intentada (comentarios con muchas variantes ya probadas: cero compartido, sInstance recargado).
- game/Em002_02SkeltonKing: saltada - vf20 en la familia `__VecCopy` (mwcc-blockers-q3); el resto con notas de prologo/planificacion.
- game/EnemySeaLizerd: sin cambios. vf10 99.87% (r28/r29 entre el gestor de balas y los datos); 10 variantes de orden de locales.
- game/GameSaveLoadBackgroundCommon: sin cambios. fn_803B8B08 99.98%: probado anadir funciones con los literales perdidos del pool ("%s/save%02d.dat", "%s/save_sys.dat"); con static se eliminan, sin static salen pero los "" se fusionan (-str reuse) y el jumptable de fn_803B87A8 cambia de nombre (99.92%). Revertido.
- game/MenuCommon_unk: sin cambios. fn_80116FC0 (y sus dos inlines) 99.7%: 8 formas nuevas de Get_unk / acceso a la tabla, ninguna cambia r0/r3.
- game/EnemyFieldMagician, game/LogMessageTask, game/MapManager, game/Np131LivingArmor, game/PathData_unk, game/U16Table_unk, game/main: saltadas - push_back en bucle / pila / orden de z / presupuesto de inline segun sus comentarios.
- game/NewCampPartsTop: saltada - vf2C necesita un tercer argumento en MenuLoadTask::fn_80117914 (firma compartida con CampSaveLoad/GameOverTask); vf14/ctor: SetPaneText_unk / std::string.
- game/ShopBuySell: 14/16. fn_80586620 (0x80586620) SUBIDA: `int slot = 0; fn_805866F4(id, 0, slot++);` (el contador empieza en 0 y se incrementa en la primera llamada; asi el li 1 se planifica como en el original).
- game/Em201ZanGurg: 13/15. SUBIDAS vf0C (0x80262CF8): los colores del material como temporales pasados por direccion directamente a `mMotion.fn_80094958` (el inline MaterialEdit_unk con referencias, o locales con nombre, cambiaban r3/r4 de la palabra de flags y el literal; inline eliminado), y fn_802640FC (0x802640FC): `float my = mRot.y;` antes de AngleDiffDeg_unk. Quedan ctor 99.77% (std::string) y vf3C 99.71% (ToRotation_unk, como CopyRobotTask).

Hallazgos MWCC (4):
- Pasar un argumento float de un miembro a un inline a traves de un local con nombre cambia el orden de carga f0/f3 (Em201ZanGurg::fn_802640FC).
- `&Color4_unk(...)` (direccion de temporal) frente a un inline con `const Color4_unk&`: cambia la asignacion de r3/r4 de las instrucciones previas.
- game/Arms: sin cambios. fn_80044FBC 99.93% (`last` r24/r22; 4 variantes nuevas: arms local, mFDC releido, break temprano, reutilizar `sub`). Las otras dos (ROTATION f0/f2, slot de pila) no intentadas.
- chr/ControlPushedDown, game/MapPartsLocatorSet, game/MotionCommand_unk, game/ShopIdentify, chr/ControlAppear, chr/ControlCliffhang, game/CharaFaceControl, game/CostumeEditChara: saltadas - slots de pila / push_back / orden de z / `__VecCopy` / SetPaneText segun sus comentarios.
- game/GameOverTask: fn_80377298 99.45 -> 99.55%: `opts = sOptRetryTitle; mChoices.push_back(opts[0]);` arregla r3/r4 de la opcion unica. Lo que queda es de reubicaciones: el original referencia las tablas como lbl_8074DDF8 + n (pool unico); objdiff lo ve como diferencia de argumento.
- game/LibraryTask: sin cambios. Ctor 99.69% (lista/offset r30/r31; 9 variantes, como el ctor de SelectMember pero al reves).
- game/NewCampPartsHint, chr/ControlDive: saltadas (push_back en bucle / registros ya investigados / `__VecCopy`).
- game/ChaserNpc: 9/14. fn_8018128C (0x8018128C) SUBIDA: `m14C4 = (float)MAX(...); m14C4 *= 1.32f;` (asignacion compuesta en vez de `x * 1.32f` en la misma expresion; cambia el orden de operandos de fmuls). fn_80181D94 99.57% (bucle de FindPlace_unk, como CharaC58_unk) no intentada.
- chr/ControlAttractHateCharge: ENLAZADA (8/8). ControlOverdrive::vf0C (0x801B79D8) subida en una segunda pasada: la comparacion muerta es `float lim = 0.2f; float d = m3A4; lim = MIN(lim, d - 0.1f);` (macro MIN con el limite primero). No se uso pairnames --apply (empareja vf60 con vf40 de GimmickCommon, inlines de 8 bytes).

Hallazgos MWCC (5):
- Clamp muerto `x = MIN(lim, d - c)` con el limite en un local declarado antes: carga las constantes y compara como el original (fcmpo lim, d); las formas `if (d > lim) d = lim` invierten f0/f1.
- `a = expr; a *= k;` en vez de `a = expr * k;` invierte el orden de operandos de fmuls (ChaserNpc::fn_8018128C).
- game/GameBgm: sin cambios. fn_8037D4C0 99.6% (engine/name r28/r29; 4 variantes con reutilizacion de variables, ninguna mejora). Resto no intentado.
- chr/ControlTurnQuadruped: segunda pasada sin exito (4 variantes mas: initializer, bool previo, MIN, encadenado).
- chr/ControlWallUp: 17/24. vf40 (0x801CB24C) SUBIDA: `float s = 1.0f - t;` en un local con nombre antes de construir el vector (los FPR de 0.0f y 1-t quedaban cruzados). vf0C/vf10 tienen copias `__VecCopy` r4/r5 (familia sin solucion); vf18/vf14 orden de z.

Hallazgos MWCC (6):
- Una subexpresion float (`1 - t`) usada dentro de un temporal Vector3 en un local con nombre cambia la asignacion de FPR respecto a escribirla en linea (ControlWallUp::vf40).
- game/SpecialEnemyBase: sin cambios. fn_805A3D68 99.88% (".csv" en r27; 3 variantes con local/reutilizar `s`).
- game/SongManager: fn_805A38F0 (0x805A38F0) SUBIDA: en los dos `case 8` el contador en un local (`int n = counts[8]; if (n >= __Limit_unk())`) para que la carga de counts[8] vaya despues del limite como en el original. (fnstat da valores algo distintos de report.json en otras funciones de la unidad; report.json sin bajadas.)

Hallazgos MWCC (7):
- Comparar `array[k]` contra un inline que carga un global: con el elemento en un local con nombre, el global se carga primero (r3) y el elemento despues (r4), como en el original.
- game/RouteWalker_unk: sin cambios. fn_80128930 99.53%: `float r = mChara->m5B0; r = 5 + (target->m5B0 + r);` da el orden de carga pero cruza f1/f2/f3 (99.3%); revertido y anotado.
- game/SkillEffectManager: sin cambios (fn_8059B8A8: 3 variantes de m58 mas, ya habia 8).
- game/TeamAIManager: sin funciones nuevas al 100%, pero `int id = in->mId; int type = in->mType; Find_unk(type, id)` (id leido antes que type) mejora OnMembers_unk 99.04->99.47, OnChara_unk 99.25->99.38, OnMsg21_unk/OnMsg24_unk 98.75->98.98, OnTarget_unk/OnTarget2_unk 91.24->91.42. En OnMsg20_unk y OnParent_unk empeora (no aplicado).
- game/GameSoundControl: 9 funciones nuevas al 100% (de 6/19 a 16/19), escritas desde cero: fn_803CE070 (vacia), fn_803CE074 (posicion guardada de una BGM: pares id/posicion en m68, cuenta m64), fn_803CE148/fn_803CE154 (constante 0x2D5000), fn_803CE160 (el mapa es arena: area 1, o area 2 mapa 16 / mapa 68 sub 1), fn_803CE278 (la BGM del mapa puede cambiar: switch con jumptable), Unk_803CE3B8::Unk_803CE3B8 / fn_803CE408 / fn_803CE448 (cabecera de guardado; clase de include/game/GameSaveBackground.h, definida en este fichero) y fn_803CE44C (validacion de la cabecera). fn_803CE0B4 91.89% (el original recarga m64 tras el test `>= 4`; con lectura volatile iguala, no usada). Struct nuevo MapId_unk en Unk8087F540_unk::m70 (+0x58). fn_803CDF84 sin cambios (11 variantes).

Hallazgos MWCC (8):
- Rango con la constante a la izquierda (`v < 0 || 999 < v`) mantiene las dos comparaciones con signo; `v > 999` se pliega en una comparacion sin signo (fn_803CE44C).
- Campos de un struct empaquetado leidos via inline que toma el struct por valor reproducen la copia a pila (stw x3 + lbz/lhz) del original.
- game/NewStaffRoll: 8/9. fn_80531E80 (0x80531E80) escrita desde cero (`extern "C"`, la llama Event/ActionEffectPlay): el staff roll llego a la linea END (sin lineas -> 1; ultima linea tipo 4 y mPos >= su Y). Queda fn_80531394 (99.3%, registros).
- game/HColliData_unk: fn_804405CC (inicializador de dos guardas de .sbss, lbl_8087F3CC/CD) no intentada: sin llamadores y fuera de un patron claro de la fuente.
- game/GimmickStateCommand: saltada - las funciones pequenas al 0% son copias debiles de inlines que el original llama fuera de linea (presupuesto de inline, documentado en la unidad).
- game/NewStaffRoll fn_80531394: 3 variantes de orden texto/longitud sin mejora.
- game/CampSaveLoad: MenuLoadTask::fn_80117914 llama ahora a `GameSaveLoadCommon::fn_803B8758(int)` (vista local de la clase, como GameOverTask) en vez del `extern "C"` con dos argumentos: el simbolo del original es el metodo. Queda el intercambio r29/r31 del CopyString_unk (literal/destino); un local para el destino da el orden pero baja la puntuacion (98.44%), no aplicado.
