# cabeceras-20261114, cola N: unidades cerca de enlazar

Agente N, rama `agent-N`. Lista generada de `build/SLSP01/report.json` (unidades fuera de `ROUTE` con 1-2 funciones
por debajo del 100 %, el resto al 100 %): 147 unidades. Referencia `$REF` = `/home/claude/ref-N` (15433).

## Resultado por unidad

| Unidad | Funcion | Antes | Despues | Cambio |
|---|---|---|---|---|
| `RVL_SDK/.../hbm/HBMBase` (libreria) | `homebutton::HomeButton::draw` | 98,80 % (33/34) | **100 %, enlaza** (34/34) | `static const GXColor fogClr` |

## Detalle

- `HBMBase` draw: el color de la niebla es `static const GXColor fogClr = {0, 0, 0, 255};` (va a `.rodata` en el
  mismo sitio del pool, 0x330); como local no static la carga se programaba tras los `fmr`. `"matching": true` en
  `libs_ref.json` y `main.dol: OK`. Renumera 7 `@N` de datos de la propia unidad (pd.sh, solo renombrados).

## Sin cambio (intentos, revertidos)

- `chr/ControlDetect` HateAtt vf0C (99,84 %): asgwrap (VecAssign_unk y --template), `-=`, temporal con nombre,
  inverso en local, `chara` local, velocidad aparte, `Assign_unk`, referencia a mPos: r3/r4 del __VecCopy siguen
  cruzados.
- `atn/D2A_Frame` vf18 (99,85 %): copia de oc antes del test, ToU32, ctor de 4 floats, operator=, alpha en local.

## Decisiones tomadas sin el usuario

- `HBMBase.cpp` pasa a `"matching": true` en `libs_ref.json` (enlaza, `main.dol: OK`).

## Para el port

- Nada.

## Hallazgos

- Un agregado local inicializado desde el pool que se programa tarde: probar `static const` (HomeButton::draw).
