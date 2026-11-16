# Siguiente sesion (tras mixto-20261116)

> Sesion `mixto-20261116` (12 h, hasta 4 agentes, 29 colas) del 2026-11-16 10:33 al ~22:45. Notas:
> `docs/mixto-20261116.md` y `docs/mixto-20261116-q*.md`. Para el port: `claude/port-mixto-20261116.md` (proyecto).

Estado: **15878 / 18109**, lost 0, rep100 26, labelcheck 11 lineas, `main.dol: OK`. Codigo `matched_code_percent`
60,19 %, datos enlazados **1085062 / 1432078 B (75,77 %)**, 725 unidades completas. PacketPack 0 % 23, 100 % 270/380.

## Decisiones pendientes del usuario

- Revisar "Para revisar" de `docs/mixto-20261116.md`: funciones inventadas que el enlazador quita para el orden de
  pools/`.sdata2` (AiCommon_unk, ControlSitting, Puppeteer_CharaTask, SpText_unk, DebugInfo_unk), 700 asserts vacios en
  `GetM50C_unk` (vf10 de Manager2), rangos de `case` de GCC en PacketPack, `(float)__fabs` en atn/Model, `long` en
  `HitInfo_unk::m48`, literales y comparaciones corregidos (cambian el comportamiento), commits que solo corrigen valores.
- `#pragma pack(1)` para los payloads de red de PacketPack (parche `docs/mixto-20261116-qG3.patch`): no aprobado.
- `atn::Abs_unk` con `double r = __fabs(x)`: daria varias funciones pero renumera casi todo y baja GameCamera.
- Restos de regla 2 anteriores a la sesion: `volatile` en AiCommon_unk fn_8001A464, `float unused` en
  ActionCameraSetting::vf08, `((void)0)` en AABBTree Node::fn_80045C88.
- Parche de `std::vector<T*>` con su nivel base real (`docs/mixto-20261116-qB.patch`): +2 funciones, pero hay que
  re-emparejar ~35 unidades (CharaTask 376 etiquetas).

## Siguiente

- Trucos que mas rindieron (grep `mixto-20261116` en `docs/trucos-mwcc.md`): getters debiles del DOL (tambien dentro de una
  inline auxiliar), bucle entero en una inline que recibe el contenedor, tipos/plantillas reales en vez de vistas,
  `\0` explicito de formatos (`nulscan.py`), etiquetas de `symbols.txt` que cubren varios objetos (`relonly.py`),
  firmas reales de las funciones llamadas (`float` y no `double`, argumentos que faltaban), metodos `inline` que se
  emitian fuera de linea. Scripts de los agentes: copiar de las notas; no estan en el repo salvo `tools/decomp/perm/`.
- Cerca de enlazar: AutomaticDoor fn_803F53E0 99,90 (parche qF2), OnOff vf0C 99,95, GmkBreakArmor ctor 99,95,
  ReinforcePoint vf10 99,81, Em303KrakenFootMp vf104 99,96, Cockpit fn_804B50C0, GameOverTask (ctor 99,59).
- PacketPack: busqueda inline fn_804DD158 (aid/desplazamiento cruzados, ~10 funciones al 99 %), `vector<PPPlayer_unk>`
  (fn_804D6688), `.rodata` antes de fn_804DD574, `m2A50` como `MpMessageTable_unk`, fn_804E4EB0/EB934/F3EEC/DE884.
- decomp-permuter: solo C; no compensa (ver qP).
- Sincronizar el port con `claude/port-mixto-20261116.md`.
