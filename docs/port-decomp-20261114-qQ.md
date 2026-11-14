# port-decomp-20261114, cola Q: unidades que pasan a enlazar

Agente Q, rama `agent-Q`. Lista: `near.txt` (unidades fuera de la ruta con una sola funcion por debajo del 100 %).

## Resultado por unidad

| Unidad | Funcion | Antes | Despues | Firma final |
|---|---|---|---|---|
| `db/RandomItemTable_unk` | dtor de la ranura 0x8021D58C | 96,30 % (16/17) | **100 %, enlaza** (17/17) | `RandomSlot_unk::~RandomSlot_unk()` (implicito) |

## Detalle

- `db/RandomItemTable_unk`: la ranura de cinco por tabla pasa de `typedef atn::FixedArray_unk<RandomItem_unk, 8>`
  a `struct RandomSlot_unk : public atn::FixedArray_unk<RandomItem_unk, 8> {}`. El destructor de la derivada
  inlinea el de `FixedArray_unk` (base en el offset 0) y da el segundo `beq` sobre el mismo `cr0`. MWCC emite
  ademas la copia debil `__dt__Q23atn34FixedArray_unk<14RandomItem_unk,8>Fv` sin referencias: **el enlazador
  la quita (deadstrip)**, `main.dol: OK`. Los destructores de `RandomTable_unk`/`WindTable_unk` pasan a ser
  implicitos (sin `~X() {}`): asi las copias debiles salen en el orden del original (ctor de WindTable antes del
  dtor de RandomTable). Renombrados en `symbols.txt`: `__ct__/__dt__Q23atn34FixedArray_unk<14RandomItem_unk,8>Fv`
  -> `__ct__/__dt__14RandomSlot_unkFv`, y tres `@N` de datos (pd.sh).
  - Probado y descartado: poner el almacenamiento de `atn::FixedArray_unk` en una base con destructor propio
    (`include/atn/FixedArray.h`): arregla esta pero baja 12 funciones de otras unidades (MarketTask,
    NewCampPartsStatus, TacView, PopupTask, LogMessageTask, ...). Revertido.

## Sin cambio (intentos, revertidos)

- `atn/LightUtil_unk` fn_8007A154 (98,73 %): ~14 variantes (size() en la comprobacion da la recarga de mSize pero
  cambia los registros: 95,5 %; inline de erase, end(), if anidado, orden de dst/end, contador local).
- `game/PlayerInput_unk` fn_801231D0 (99,46 %): en los casos 39/40 `if ((unsigned int)(skip - 1) > 1) return 0;`
  da el **codigo identico** al original, pero las etiquetas `@N` de los floats de la funcion quedan +2/+3
  (objdiff 99,96 %: compara las `@N` por nombre). Con `break` el numero coincide y el bloque no. ~25 variantes
  (switch, &&, ternario, locales, mover el caso). Ademas la unidad no enlazaria: `poolcheck` da pi/2pi/-pi.
- `Event/EventCommandDef_unk` fn_800081C0 (94 %): solo el orden `li r5,0`/`addi r4`; 9 variantes mas.
- `game/GameSaveBanner` fn_803B9638 (99,18 %): inline `InitBanner_unk` (memset + NANDInitBanner) y locales
  title/comment: sin cambio.

## Cambios de firma o de layout (para el port)

- `db/RandomItemTable_unk.cpp`: `RandomSlot_unk` es ahora una struct derivada de `atn::FixedArray_unk<RandomItem_unk, 8>`
  (mismo layout, 0xC4 bytes); `RandomTable_unk`/`WindTable_unk` sin destructor declarado.

## Hallazgos

- Una copia debil sin referencias que nuestro .o emite de mas (p. ej. el destructor interior inlineado) no impide
  enlazar: el enlazador la elimina (deadstrip). Lo que importa es el orden del resto de copias debiles.
- objdiff compara las etiquetas `@N` de `.sdata2` por nombre: dos fuentes con el mismo codigo pueden quedarse en
  99,9x % si gastan distinto numero de etiquetas internas (cada `return`/condicion cuenta).
