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

## Decisiones tomadas sin el usuario

- **symbols.txt (EnemyQuarkFirst)**: fn_80334FDC -> `fn_80334FDC__15EnemyQuarkFirstFPCci`, fn_803383E4 ->
  `fn_803383E4__15EnemyQuarkFirstFv`, lbl_8074A2D8 -> `@stringBase0` (scope:local, data:string_table).
- EnemyQuarkFirst::vf40 crea `chr::ControlAttackLastOne` (nombre de la vtable que guarda fn_8018E438, unidad
  chr/ControlAttackBase sin fuente) a traves del `extern "C"` fn_8018E438 con `::operator new(0x38, ...)`;
  no se renombra (otra unidad, la llama tambien BattleCharaTask en asm).
- vf0C/vf10 escriben `mStatus.m2D0[3]` (0xABC) y `mStatus.m2FC` (0xAD0) y leen `mStatus.mParamB.m84`
  (0x940) con los miembros ya existentes de CharaStatus; el significado (skill de salto, multiplicador) es
  deducido.

## Hallazgos MWCC

- En vf40 `float k = 5.0f; Vector3 v(k * a.x, 0.0f, k * a.z);` da el orden de operandos de fmuls del
  original (`fmuls f3, f3, f4` con la constante en f4); con el literal en linea salen invertidos.
- Diferencia de dos vectores con `y = 0`: `Vector3 dir = t->mPos - mPos; dir.y = 0.0f;` reproduce el orden de
  stores del original (fn_803383E4); el ctor con `0.0f` en medio no.
- `to = from; to += dir * k;` en vez de `to = from + dir * k` evita un temporal y casa la pila.

## Pendientes para otras colas

- Ninguno por ahora.
