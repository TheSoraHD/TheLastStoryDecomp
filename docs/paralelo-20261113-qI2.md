# Lote paralelo-20261113, cola I2 (segunda pasada: AiTarget, GameTask, ItemManager_unk, TextAnimation, LH_Model)

## Resultado por unidad

- `game/GameTask`: funciones al 0 % 6 -> 2; al 100 % 107 -> 108 (`SetColorMatrix`, copia debil 0x80366D40). Escritas (NONMATCHING, equivalentes):
  - `~GameTask` (98,6 %): suelta los gestores y restaura los ajustes de render (`ResetPostFx_unk`,
    `ResetShadow_unk`, el bloque de dano como en `fn_80373FC4`). El original guarda dos veces `m054.mEnabled`
    (una antes del reset): sale con `render->m054.mEnabled = 1` y un puntero local. Quedan registros de float y
    el orden de los stores del bloque de profundidad de campo (las mismas diferencias que `MapTask::fn_8049ED90`).
  - `fn_80367A40` (98,3 %): nivel del grupo y de los enemigos al empezar el campo (curvas de `db::GrowthEntry_unk`
    con `fn_800A6BDC`, copia del estado al inventario). `x / 2.0f` da el `fmuls f1, f1, f30` del original
    (`x * 0.5f` invierte los operandos).
  - `fn_8036F268` (98,7 %): cambio de mapa (`extern "C"` con el `GameTask*` delante, como la llaman
    `GameEventScript` y `NewCampPartsMapSelect`); el parametro `save` pasa a `GameSaveData_unk*`.

## NONMATCHING con motivo

- Copias de `CharaStatusData_unk` (`m54..m7C`) y de `CharaStatus` (`m2CC..m2EC`): el original las copia por
  pares de 8 bytes, como un agregado. Con un `struct` anonimo en `CharaStatus.h` sale igual, pero el tipo
  anonimo consume un `@N` en las 270 unidades que incluyen la cabecera (CharaStatus baja 40 -> 30 al 100 %).
  `m2CC` + `m2D0[4]` parece un `FixedArray_unk<CharaStatusPair_unk, 4>` (cuenta + 4 pares), pero cambia el
  constructor de `CharaStatus`. No tocado.

## Decisiones tomadas sin el usuario

- `symbols.txt`: `SetColorMatrix__Q23atn12RenderConfigFRCQ33atn12RenderConfig11ColorMatrix` (nombre del RSO) pasa a
  `SetColorMatrix__17RenderSetting_unkFRC20ColorMatrixParam_unk`: nuestro `.o` ya emitia la copia debil (la llama
  fuera de linea `fn_803729B4`) y, con el destructor escrito, sale detras de el como en el original: 100 %. El
  nombre real (`atn::RenderConfig`, `RenderConfig::ColorMatrix`) queda para cuando se renombre la clase.

- `MpSettings_unk::m284` (local de `GameTask.cpp`) pasa de bytes a `int`.
- `GameTask.cpp` incluye `db/GrowthParseGlobal.h`, `atn/Layer3D.h` y `game/SongManager.h`.
- La declaracion local de `fn_8036F268` (con `void* save`) se sustituye por la definicion; los dos llamadores de
  `GameTask.cpp` hacen el cast a `GameSaveData_unk*`.

## Hallazgos

- `pd.sh` no renombra una `@N` de `.sdata2` usada por dos funciones cuando una de ellas (NONMATCHING) usa en
  nuestro `.o` otra constante de igual valor: renombrar a mano la del original (`@28791`, 0x808857A8, de
  `fn_8036D2D0`) o labelcheck da 1 que falta.
- `delete p` de una clase con destructor no virtual da una sola comprobacion; el original de `~GameTask` lleva
  `if (p != 0) delete p;` (dos `beq` para `m5628`).

- MWCC copia los miembros agregados (arrays, structs) de una clase por pares de palabras desde su propio
  desplazamiento, y los escalares de uno en uno: el agrupamiento de la copia delata la frontera de los agregados.

## Pendientes

- `fn_8036F268` deberia ser miembro de `GameTask` (regla 10): exige renombrar en `symbols.txt` y cambiar los
  llamadores de `game/GameEventScript.cpp` y `game/NewCampPartsMapSelect.cpp` (otras colas).
- `fn_80371790` (3,5 KB): copia local de `FixedArray_unk<ExpEntry_unk, 8>` con el constructor de `ExpEntry_unk`
  (memset del `LevelUpResult_unk`): exige constructores en `Inventory.h` (58 unidades), como el ctor de `Inventory`.
- `fn_803731E8`: sigue bloqueada por la firma de `FinishCamera_unk::fn_803786FC` (cola B2).
