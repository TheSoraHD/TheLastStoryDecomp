# Lote enemies-finish, cola q8 (EnemyQuarkFirst, EnemyQuarkSecond)

Inicio: 2026-10-22 13:49 CEST. Rama agent-q8 (sale de main 1882325, merge de agent-q3). Referencia:
/home/claude/ref/start-q8.json.

## Unidades

### game/EnemyQuarkFirst (4/15 -> 11/15)
- Unidad empezada en close-units-qE (dtor, vfFC, vf4C, vfA8). Cabecera completada con el layout del ctor
  y de las claves del csv (nombres inventados; m1544-m1564 son cinco `QuarkFlag_unk` {-1, 0}).
- Al 100% nuevas: ctor, vf0C, fn_80334FDC (lector del csv), vf10, vf10C, vf40, fn_803383E4 (camino libre
  hacia el objetivo). Comprobadas tambien con objdiff-cli diff directo (0 instrucciones distintas).
- NONMATCHING: vf30 99.54% (solo offsets de @stringBase0: falta "SE_DG040_105" de vf108).
- Sin escribir: vf100 (2960 bytes), vf104 (1908) y vf108 (5976), la maquina de estados.

### game/EnemyQuarkSecond (0/70 -> 9/70, en curso)
- Unidad SIN FUENTE hasta ahora: fuente nueva `src/game/EnemyQuarkSecond.cpp` y cabecera nueva
  `include/game/EnemyQuarkSecond.h` (subclase directa de EnemyTask: el ctor llama a `__ct__9EnemyTask` y la
  vtable tiene 0xF8 bytes; layout 0x14B0-0x1788 del ctor, del dtor y de las claves del csv; nombres
  inventados). Ya tenia entrada en game_units.json.
- Al 100%: ctor (tokenizer de boost, todo fuera de linea como en el original), dtor, vf3C, fn_80338CDC
  (flag vertical de la capsula), copias debiles de la unidad (QuarkMtxA_unk/QuarkHits_unk ctor,
  fn_803396EC, fn_803396FC, ctor CharaStatusPair_unk(int)).
- NONMATCHING: vf0C 99.99% (solo la llamada a fn_80344780, el menu de depuracion, aun sin escribir);
  fn_80338D14 99.77% (lector de listas: residuo conocido del push_back inlineado, size/cap en otros
  registros); vf10 56% (presupuesto de inline: el original llama fuera de linea a todos los accesores;
  con una candidata artificial de 400 sentencias quedan 87 lineas distintas; no se anade).

## Decisiones tomadas sin el usuario

- **symbols.txt (EnemyQuarkFirst)**: fn_80334FDC -> `fn_80334FDC__15EnemyQuarkFirstFPCci`, fn_803383E4 ->
  `fn_803383E4__15EnemyQuarkFirstFv`, lbl_8074A2D8 -> `@stringBase0` (scope:local, data:string_table).
- EnemyQuarkFirst::vf40 crea `chr::ControlAttackLastOne` (nombre de la vtable que guarda fn_8018E438, unidad
  chr/ControlAttackBase sin fuente) a traves del `extern "C"` fn_8018E438 con `::operator new(0x38, ...)`;
  no se renombra (otra unidad, la llama tambien BattleCharaTask en asm).
- vf0C/vf10 escriben `mStatus.m2D0[3]` (0xABC) y `mStatus.m2FC` (0xAD0) y leen `mStatus.mParamB.m84`
  (0x940) con los miembros ya existentes de CharaStatus; el significado (skill de salto, multiplicador) es
  deducido.

- **symbols.txt (EnemyQuarkSecond)**: ctor, dtor y las 15 virtuales de la vtable renombradas
  (`__ct__16EnemyQuarkSecondFPQ23atn4TaskP13EnemyInfo_unk`, `vf0C__16EnemyQuarkSecondFv`, ...);
  lbl_8074A4C0 -> `@stringBase0` (local); fn_80338D14 -> `fn_80338D14__FP13IntVector_unkPQ23atn3Csv`
  (estatica local: no recibe `this`); fn_80339718 -> `__ct__19CharaStatusPair_unkFi` (copia debil del ctor
  de CharaStatus.h); fn_80338D04/fn_80338D08 -> ctores de QuarkMtxA_unk/QuarkHits_unk.
- **symbols.txt (copias debiles de otras unidades usadas por EnemyQuarkSecond)**: fn_80288390 ->
  `__ct__13IntVector_unkFv` (en Em203GreaterDaemon), fn_8000D760 -> `__dt__13IntVector_unkFv` (en
  Event/Puppeteer_CharaTask), fn_803165E0 -> `__ct__12MtxBlock_unkFv` (en Em316LaserSphere), fn_802BABC0 ->
  `__ct__9ResHandleFv` (en Em206CocoonMp). Son ctores/dtores triviales de clases inventadas (IntVector_unk
  derivada de std::vector<int>, MtxBlock_unk de 12 floats) que el enlazador dejo en esas unidades; nadie
  las llamaba por `extern "C"`. Las unidades que los contienen siguen en asm.
- game/SpecialEnemyBase.cpp: la factoria (case 71) crea `new (3, "", "", 0) EnemyQuarkSecond(parent, info)`
  (se quita el `extern "C"` fn_803388A4).
- EnemyQuarkSecond.cpp define como `extern "C" inline` los accesores de otras unidades que usa (fn_8000DCF4,
  fn_802A36B0, fn_802A4094, fn_80267B28, fn_80122550, fn_8028CFB8, fn_802A49BC, fn_802A4968, fn_8029F3AC,
  fn_80265804, fn_80373148, fn_800F7FA0, fn_800F7FA8, fn_8026607C, fn_800E0AA8, fn_8028B74C, fn_801C3910),
  con el cuerpo de su copia debil, como EnemyJill.

## Hallazgos MWCC

- En vf40 `float k = 5.0f; Vector3 v(k * a.x, 0.0f, k * a.z);` da el orden de operandos de fmuls del
  original (`fmuls f3, f3, f4` con la constante en f4); con el literal en linea salen invertidos.
- Diferencia de dos vectores con `y = 0`: `Vector3 dir = t->mPos - mPos; dir.y = 0.0f;` reproduce el orden de
  stores del original (fn_803383E4); el ctor con `0.0f` en medio no.
- `to = from; to += dir * k;` en vez de `to = from + dir * k` evita un temporal y casa la pila.

- Un lector de csv `static` (fn_80338D14, sin `this`) se reconoce por los argumentos (tabla, csv) en r3/r4.
- `Find_unk(id)` con `int id = atoi(...)` en un local da los registros r29/r30 del original en vf0C de
  EnemyQuarkSecond; con la llamada anidada salen intercambiados con la clave del csv.
- Una inline pequena (fn_8034472C) se expande en vf3C y sale como copia debil para vf10, que esta por
  encima del presupuesto.

## Pendientes para otras colas

- Ninguno por ahora.
