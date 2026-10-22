# Lote enemies-third, cola s1 (Em301LShade, Em302Kraken, Em302KrakenMp; segunda pasada)

Inicio: 2026-10-22 22:08 CEST. Rama agent-s1. Referencia: /home/claude/ref/s1_start.json (7299 al 100%).

## Unidades

### game/Em301LShade (17/31 -> 19/31)
- Al 100%: fn_802D3C74 (inline `DistSq_unk(c->mPos - center)` en el recuento del grupo; el double de
  lbl_80747090 no lo impedía) y vf3C (inline `PlayEffectD_unk`: manager leído al principio, locales color/rot/pos).
- Escrita: fn_802D67F8 (refuerzos), NONMATCHING 97.1%: push_back inlineados (familia conocida) y la comprobación
  `0 <= index < 3`, que el original emite como `blt cuerpo; b fin` (más de 10 variantes de la condición: if/switch/
  early return/constante a la izquierda).
- vfEC 99.1 -> 99.98% (`const atn::Vector3* cam` para la copia de "dead_camera"); solo queda el offset de "head" en el
  pool, que depende de vf0C (0%, presupuesto de inline: probado que con 40 sentencias extra en un inline sigue
  inlineando todo).
- Sin cambios: ctor (std::string), vf10 90%, vf50/vf54 (pool), fn_802D4628 85%, fn_802D57B8 99.2% (pila del efecto;
  4 variantes del inline), fn_802D5D18 92.9% (registros dentro de ShadeFree_unk), fn_802D6004 85% (push_back).

### game/Em302Kraken (38/49 -> 40/49 + 2 escritas)
- Al 100%: fn_802DB534 (`int state = g->m54;` antes de la comparación con `skip`), fn_802DDA90 (accesor
  `IsFlag25_unk`, como gmk/ReinforcePoint), vfCC (un local `ry1..ry3 = mRot.y` antes de cada bloque).
- Escritas: fn_802DCA34 (99.2%, solo registros del push_back inlineado) y fn_802DD1B4 (96.5%, push_back y la misma
  comprobación de rango que LShade::fn_802D67F8). Listas locales derivadas de std::vector (`KrakenCharaList_unk`,
  como las demás: el destructor prueba la dirección tres veces).
- Quitado un `float color[4] = {1,1,1,1}` sin uso del borrador fn_802DAA38: metía 16 bytes en .rodata que el original no
  tiene (ahora .rodata cuadra con el original salvo @stringBase0).
- Sin cambios: ctor 98.3% (segundo std::string), vf0C 99.4%, vf10 97.7% (el `Set_unk(pos, false)` inline de la cámara
  sale con `b; b; li r0,0` en el original: mismo caso abierto que EnemyTrista/EnemyJill; 6 variantes),
  fn_802DA718 90.3% (frsp: 12+ variantes entre las dos pasadas), fn_802DAA38 86.5%, push_back 85.5%/99.95%.

### game/Em302KrakenMp (33/39 -> 34/39)
- Al 100%: vfAC (el desplazamiento del segundo return en un local con nombre).
- vf10 98.8 -> 99.8%: inlines `RaisePillar_unk`/`BreakPillarsMp_unk` (forma del original con los dos `b`), mensaje
  de la primera pasada escrito en línea, y el ctor de `KrakenMpDamage_unk` con `mFlags = 0; m18 = -1;` en el cuerpo
  (orden de Em101ElderZesya). Queda: registros de CanAct_unk (r4/r0/r3; 5 variantes; Em305VampireMp::vf10 sale en
  nuestro orden, Em303KrakenFootMp::vf10 tiene el mismo problema).
- ctor: orden de `clear()` corregido (m160C antes de m1618, como los stores del original); sigue 98.1% (std::string).
- Sin cambios: vf0C 99.3%, fn_802E094C 81.9% (frsp), push_back.

## Resultado
- 7299 -> 7305 funciones al 100% (+6: LShade fn_802D3C74, vf3C; Kraken fn_802DB534, fn_802DDA90, vfCC; KrakenMp vfAC).
- Escritas (NONMATCHING): LShade fn_802D67F8, Kraken fn_802DCA34/fn_802DD1B4. Ya no queda ninguna función sin
  fuente en las tres unidades.
- Ninguna unidad enlazable: les quedan ctor (segundo std::string), vf0C/vf10 y los push_back inlineados.

## Decisiones tomadas sin el usuario
- **symbols.txt**: lbl_80747558 -> `@LOCAL@fn_802DCA34__11Em302KrakenFii@sOrder_unk` y lbl_80747570 ->
  `@LOCAL@fn_802DD1B4__11Em302KrakenFi@sRange_unk` (scope:local): tablas estáticas locales de los refuerzos
  ({1,3,0,2,4,0} y los rangos {0.25,0.5},{0.3,0.75},{0.7,1.0}). Solo las usa Em302Kraken.
- Inlines nuevos con nombre inventado (todos `// inferred:`): DistSq_unk, PlayEffectD_unk, IsFlag25_unk (en LShade y
  Kraken), RaisePillar_unk, BreakPillarsMp_unk, KrakenCharaList_unk.
- `fn_801077A8` declarada `extern "C"` con tres argumentos (Effect_unk*, Model_unk*, Model_unk*): el original pasa el
  modelo dos veces (r4 y r5).
- Sin cambios en cabeceras compartidas, splits ni SpecialEnemyBase.

## Hallazgos
- Un `if (size == 0) return 0; ... if (g != 0) { ...; return g; } return 0;` en un inline cuyo valor no se usa
  reproduce los `b fin; b fin` seguidos del original (Em302KrakenMp::vf10).
- Releer `mRot.y` en un local justo antes de cada `PSMTXRotRad` adelanta la carga por delante del store del vector
  anterior (Em302Kraken::vfCC).
- Un temporal de retorno con nombre (`atn::Vector3 offset(...); return a + offset;`) en una rama y anónimo en la otra
  invierte los huecos de pila de los dos temporales (Em302KrakenMp::vfAC).
- Un array local constante sin uso (`float c[4] = {...}`) deja su inicializador en .rodata: buscarlo si .rodata
  sale más grande que el original.
- configure.py en la nube necesita los flags de docs/entorno-nube.md (sin ellos ninja intenta descargar compiladores).

## Pendientes para otras colas
- Em303KrakenFoot/Em303KrakenFootMp: mismo bloqueo de registros de CanAct_unk que Em302KrakenMp::vf10.
- (heredado) Em209SmallGolem: los doubles lbl_80747080/88/90 son de Em301LShade (no impiden ya fn_802D3C74).
