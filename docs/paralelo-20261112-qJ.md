# Lote paralelo 2026-11-12, cola J (motor atn)

Inicio: 18:19 (corte del servidor 18:33-20:16; reanudado 20:16). Rama `agent-J`.

## atn/XsfParser_unk

- Funciones al 0 %: 11 -> 0. Al 100 % nuevas: 10 (`XsfCurveSet_unk` ctor/dtor, `fn_805BF214`, `fn_805BF4B4`,
  `fn_805BF590`, ctor/dtor del mapa, `Destroy_unk`, `InsertNode_unk`, `FindInsertPos_unk`; `fn_805BF204` y
  `fn_805BF410` suben a 100 % al existir `fn_805BF214`). report: 15156 -> 15166.
- NONMATCHING: `fn_805BF660` 79,2 % (seleccion de la referencia de `std::min/max` con puntero precargado, indice
  `i * 16` y doble comprobacion del tamano; ~10 variantes). Siguen las de antes (`fn_805BE8FC` 50 %, `fn_805BEA34`
  99,5 %, `fn_805BEB54` 94,5 %, `fn_805BEE04` 87 %).
- **Decisiones sin el usuario**
  - `std::map<float, XsfKey_unk>` modelado a mano en el `.cpp` (como `ScriptTree_unk` de GameEventScript):
    plantillas `XsfKeyTree_unk<Node>` + capas L4/L3/L2 + `XsfKeyMap_unk<Node>` (el dtor prueba `this` 5 veces),
    16 bytes: tamano, raiz (nodo fin en `&mRoot`), comparador vacio (`Less_unk`, da el `mfcr`), primero.
  - `symbols.txt` (solo renombrados): `fn_805BEFCC`, el dtor `map<Texture*,bool>`, `fn_805BF214`, `fn_805BF794`,
    `destroy__...__tree_deleter...` y `fn_805BFA54` -> nombres de nuestras clases. Los nombres std anteriores
    venian de pegar codigo identico, no del RSO.
  - `XsfCurveSet_unk::fn_805BF4B4/F590` con `__declspec(noinline)` (el original las llama con `bl` desde
    `fn_805BE8FC/BEA34`; regla 2, aceptado).
  - Las vistas parciales de `XsfCurveSet_unk` en `atn/Xsca_unk`, `game/FinishCamera_unk`, `EnemyQuarkStand`,
    `Em309Ghoul`, `GameCamera_unk`, `NewCampTask` siguen sin unificar (pendiente: cabecera `atn/XsfCurveSet_unk.h`;
    no son de esta cola). `CurveSegment_unk` duplicado con `atn/Bezier_unk.cpp`.
  - Orden de emision: las plantillas (`Destroy_unk`, `FindInsertPos_unk`) no salen en el orden del original
    (unidad no enlazada; `ordercheck` las da como "missing", 0 fuera de orden).
- Hallazgos
  - Recursion de destruccion de `std::map`: las aserciones vacias `(void)((Node*)n->mLeft->mParent == n)` fijan
    dos niveles de inline (con `Parent_unk()` inline no hacen efecto).
  - `std::pair<iterator,bool>(..., true).second` dentro del inline: el `true` va a `.sdata` y se lee con `lbz`.
  - Iterador de arbol: `--it` sobre un struct iterador da el registro del original; con puntero suelto no.
