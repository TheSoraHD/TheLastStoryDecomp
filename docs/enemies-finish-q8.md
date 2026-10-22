# Lote enemies-finish, cola q8 (EnemyQuarkFirst, EnemyQuarkSecond)

Inicio: 2026-10-22 13:49 CEST. Rama agent-q8 (sale de main 1882325, merge de agent-q3). Referencia:
/home/claude/ref/start-q8.json.

## Unidades

### game/EnemyQuarkFirst (4/15 -> 11/15)
- Unidad empezada en close-units-qE (dtor, vfFC, vf4C, vfA8). Cabecera completada con el layout del ctor
  y de las claves del csv (nombres inventados; m1544-m1564 son cinco `QuarkFlag_unk` {-1, 0}).
- Al 100% nuevas: ctor, vf0C, fn_80334FDC (lector del csv), vf10, vf10C, vf40, fn_803383E4 (camino libre
  hacia el objetivo). Comprobadas tambien con objdiff-cli diff directo (0 instrucciones distintas).
- NONMATCHING: vf30 99.54% (solo offsets de @stringBase0: falta "SE_DG040_105" de vf108); vf104 ~99%
  (r23/r24 y r26/r27 intercambiados en la busqueda de sitio alrededor del objetivo; 12 variantes);
  vf100 95.34% (borrador del arranque de estados: huecos de pila de los temporales de rotacion, giro
  inline TurnTo_unk, y el pool de .sdata2 no sigue el orden original: pi sale despues de 0.1).
- Sin escribir: vf108 (5976 bytes, la maquina de estados); sus cadenas y constantes son las que faltan
  en el pool para vf30/vf100.

### game/EnemyQuarkSecond (0/70 -> 31/70)
- Unidad SIN FUENTE hasta ahora: fuente nueva `src/game/EnemyQuarkSecond.cpp` y cabecera nueva
  `include/game/EnemyQuarkSecond.h` (subclase directa de EnemyTask: el ctor llama a `__ct__9EnemyTask` y la
  vtable tiene 0xF8 bytes; layout 0x14B0-0x1788 del ctor, del dtor y de las claves del csv; nombres
  inventados). Ya tenia entrada en game_units.json.
- Al 100%: ctor (tokenizer de boost, todo fuera de linea como en el original), dtor, vf3C, fn_80338CDC
  (flag vertical de la capsula), copias debiles de la unidad (QuarkMtxA_unk/QuarkHits_unk ctor,
  fn_803396EC, fn_803396FC, ctor CharaStatusPair_unk(int)).
- Despues: vf0C, vf4C, vf64, vf68, vf10 (ver decisiones), fn_80341E14, fn_80342FB4 y otros al 100%
  (31/70, todos comprobados con cmp.py/objdiff directo, 42 OK contando copias debiles).
- NONMATCHING: fn_80338D14 99.77% (residuo conocido del push_back inlineado); fn_8033A7A0 (inline en
  vf10, ver decisiones; en el original esta fuera de linea, en nuestro .o no se emite); fn_8033AB88 ~88%,
  vf54 ~80%, fn_8033C000 ~96%, fn_80342004 ~93%, fn_8034228C ~87%, vfD8 ~80% (orden/huecos de pila de
  temporales); vf50 ~99% (scheduling de la copia de la capsula); fn_8033D750 99.7% (f30/f31);
  fn_80344780 99.5% y otras 99.x% (offsets de @stringBase0 o pool de .sdata2: faltan los estados sin
  escribir).
- Sin escribir (unos 27 KB): fn_8033C490, fn_8033D040, fn_8033DB8C, fn_8033E058, fn_8033E8D4,
  fn_8033EF90, fn_8033F7FC, fn_803402C0, fn_803405C0, fn_803412AC, fn_80342674, fn_80342BC8, fn_8034315C,
  fn_803435B8, fn_80343B0C. Mientras falten, el pool de .sdata2 y @stringBase0 no pueden quedar en el
  orden original y bastantes funciones escritas se quedan en 99.x%.

### Unidades enlazadas
- Ninguna nueva (las dos unidades siguen sin enlazar: faltan funciones).

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

- **EnemyQuarkSecond::fn_8033A7A0 declarada `inline`** (eleccion de la siguiente accion, 1000 bytes): asi
  vf10 supera el presupuesto de inline de -ipa file y llama fuera de linea a todos los accesores como el
  original, y vf10 sale al 100%. Coste: fn_8033A7A0 no se emite en nuestro .o (cuenta como 0%) hasta que
  se encuentre otra forma. No es una funcion inventada ni un calculo sin uso, pero cambia que funcion
  matchea; revisar.
- Renombres de symbols.txt de EnemyQuarkSecond: todos los fn_8033xxxx/fn_8034xxxx de la unidad pasan a
  `fn_XXXXXXXX__16EnemyQuarkSecondFv` (o con sus argumentos, p. ej. fn_8034472C__16EnemyQuarkSecondFii);
  fn_80338D04 -> `__ct__13QuarkJump_unkFv` (antes QuarkMtxA_unk: el miembro de 0x1568 es el salto).
- fn_8026607C y fn_8029F3AC se dejan como `extern "C"` sin cuerpo en EnemyQuarkSecond: sus constantes
  estan en el pool de otra unidad, y con el cuerpo inline el pool propio se desordena.
- Dos variantes de la inline de efectos (PlayEffect_unk con color, rot, pos y PlayEffectA_unk con pos,
  rot, color): el original construye los argumentos en los dos ordenes segun el estado.
- EnemyQuarkFirst::TurnTo_unk (giro hacia m14E4) es una inline de la clase con nombre inventado; se probo
  como funcion normal y con una comprobacion vacia del angulo para mover constantes, y se descarto (no se
  usan calculos sin uso).

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

- Hacer inline una funcion grande que el original tiene fuera de linea puede ser la forma de que el
  llamador supere el presupuesto de -ipa file y deje de inlinear los accesores pequenos (vf10 de
  EnemyQuarkSecond); con funciones reales en su lugar (fn_80344BB4, fn_8034472C, fn_8034441C) vf10
  vuelve a inlinear todo.
- Una inline pequena llamada desde una funcion que excede el presupuesto se emite como copia debil en la
  unidad (fn_8034472C).

## Pendientes para otras colas

- EnemyQuarkFirst: escribir vf108 (y con ello vf30/vf100 al orden de pool correcto).
- EnemyQuarkSecond: escribir los 15 estados de la lista y reordenar pool; buscar alternativa al
  `inline` de fn_8033A7A0.
- Fin de sesion q8: 2026-10-22 17:57 CEST.
