# Lote enemies-third, cola s4

Rama agent-s4 (sale de main 297fdca). Cola: game/Em316LaserSphere, game/Em316LastLaser, game/Em401_01_WhiteLeoPard.
Cerrada antes de tiempo (cierre del lote pedido por el coordinador). Al final: `main.dol: OK`, `regress.py` lost 0
y `fuzzdrop.py` 0 frente a `/home/claude/ref/s4_start.json`, y `poolcheck.py` sin avisos en las tres unidades.

## Resultado por unidad (diff directo de objdiff-cli)

| Unidad | Antes | Después / total | Nota |
|---|---|---|---|
| Em401_01_WhiteLeoPard | 12/25 | 16/25 | todas las funciones escritas; no se puede enlazar (quedan 9 NONMATCHING) |
| Em316LastLaser | 8/19 | 8/19 | vf10 escrita (de 0% a 95,3%); faltan 5 funciones grandes |
| Em316LaserSphere | 14/40 | 14/40 | no me dio tiempo |

### Em401_01_WhiteLeoPard
- Nuevas al 100%: vf14 (llega al 100% al ordenar el pool: los arrays `lbl_80749898`/`lbl_807498A4` al principio
  del fichero colocan los double de `.rodata`), vf34, vf50 y fn_80322AD4 (llega al 100% al quedar completo el pool de `.sdata2`).
- Escritas y NONMATCHING:
  - fn_8031FE4C 98,63%: el original coloca `li r4,0` y `lwz r10,m594` al revés en la llamada a fn_800F8C6C.
  - fn_803202E0 99,79%: registros r3/r4/r5 de los bucles de FindPlace inline.
  - fn_803209E0 98,32%: mismo bloqueo que `Em403WarLion::fn_80324A38`.
  - vf24 99,94%: dt va en f29 y no en f28.
- Siguen NONMATCHING sin cambios: fn_80321724, fn_80321900, vf20, vf54 y fn_803227A0 (98,02%).
- vf24 y vf50 tienen el mismo código máquina que Em403WarLion::vf24 y vf50. Leer m500/m508 antes que la tasa
  sube vf24 de 99,75% a 99,94%. En vf50, poner `mRadius` antes del centro da el 100%. Las dos cosas sirven
  también para WarLion (cola de WarLion).
- `fn_DebugMenu_unk` es inventada: el enlazador la eliminó y solo quedan sus cadenas ("em401_01_WhiteLeoPard",
  "DebugDisp", ...) tras "tale". Sigue el mismo patrón que Em403WarLion. Las llamadas y los rangos son una suposición.

### Em316LastLaser
- vf10 escrita: 95,3%, NONMATCHING. Fallan el hueco de pila de la copia de la posición del objetivo, la
  asignación de r28-r31 y la cadena "spine1", que va a `@STRING@` del helper inline en vez de a `@stringBase0`.
  Lo que funcionó:
  - un helper `bool CanTarget_unk` para el test del jugador;
  - `LengthSq_unk(temporal)` en vez de un local con nombre;
  - helpers inline para la distancia a "spine1" y para la cápsula, que dan el orden de pila del original.
- Sin escribir: fn_8031CA30, fn_8031D350, fn_8031D6DC, fn_8031E078 y fn_8031E8BC. fn_8031D350 y fn_8031E8BC
  llaman fuera de línea a los operadores de Vector3 (presupuesto de inline). La copia del fuente quedó en
  `/home/claude/scratch/s4/`.

## Decisiones tomadas sin el usuario
- **symbols.txt**:
  - renombradas `fn_803202E0__21Em401_01_WhiteLeoPardFv` y
    `fn_803209E0__21Em401_01_WhiteLeoPardFRCQ23atn7Vector3iff`;
  - renombradas `fn_8031CA30__14Em316LastLaserFv`, `fn_8031D350__14Em316LastLaserFv` y
    `fn_8031E8BC__10LaserTraceFRCQ23atn7Vector3RCQ23atn7Vector3`;
  - ningún otro .cpp las llamaba.
- **Cabeceras (solo de la cola)**:
  - `Em316LastLaser::m14B8` pasa de `int` a `CharaTask*` (objetivo);
  - `LaserPoint_unk` se define con 0x50 bytes (Matrix3x4, inicio, dirección, longitud y vida; deducido de
    fn_8031E8BC y de fn_8031E078);
  - WhiteLeoPard declara vf24, vf34 y vf50, y amplía `Field_unk` con `m10D8`.
- Los arrays de .rodata de WhiteLeoPard van como `extern "C" const int lbl_...[3]` (los `static` no daban el nombre
  del pool).
- No se tocaron splits ni cabeceras compartidas.

## Hallazgos generales
- Un objetivo de una búsqueda que el original calcula como un bool con `li 0`/`li 1` encadenados sale con un
  helper inline `return a() && !b();` y no con la condición `&&` escrita en el `if`.
- Pasar a una función de puntero un temporal mediante un inline `const T&` (`LengthSq_unk(a - b)`) deja el
  temporal en la zona de temporales, no en la de locales con nombre.

## Pendientes / conflictos al integrar
- Em403WarLion (otra cola): vf24 y vf50 pueden ganar con los mismos cambios (ver arriba).
- Ficheros tocados: `config/SLSP01/symbols.txt` (solo renombrados de estas unidades), `src/game/Em401_01_WhiteLeoPard.cpp`,
  `include/game/Em401_01_WhiteLeoPard.h`, `src/game/Em316LastLaser.cpp`, `include/game/Em316LastLaser.h`.
