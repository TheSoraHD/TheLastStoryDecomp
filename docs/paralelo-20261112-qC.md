# Lote paralelo 2026-11-12, cola C (Batalla)

Inicio: 2026-11-12 14:55 CET. Rama `agent-C`. Unidades: `game/BattleTask`, `game/BattleCharaTask`,
`game/TacView`, `game/TeamAI_unk`.

## Resultado por unidad

- `game/TeamAI_unk`: funciones al 0 % 7 -> 0. Nuevas al 100 %: 4 (`fn_805B1918` sube de 99,9 % al pasar
  el cuarto argumento, y las tres copias debiles del arbol: `InsertNode_unk` 0x805B3120, `Destroy_unk`
  0x805B3258, `FindInsertPos_unk` 0x805B33B8). `Move_unk<P>` (tres instancias) al 87,6 %, `fn_805B27A0` al
  98,4 %. Orden del `.dol` ya correcto en la unidad (los templates se definen antes de `fn_805AED74`).

## NONMATCHING

- `TeamAI_unk::Move_unk<P>` 87,6 %: el original tiene un marco mayor (mas temporales de vector en la pila)
  y guarda el resultado sin usar del `map::insert` inline (par iterador/bool con los bool de `.sdata`
  `lbl_8087E6E8`/`E9`); el nuestro lo elimina. Registros.
- `TeamAI_unk::fn_805B27A0` 98,4 %: registros; el original recorre los miembros de un equipo con puntero.

## Decisiones tomadas sin el usuario

- `Move_unk` tiene un cuarto argumento (`int mode` = `TeamMove_unk::m0A`, el original lo lee de r7):
  cambia la declaracion en `include/game/TeamAIManager.h` (incluida por AiTarget, Em206EggMonster,
  GameEventScript y TeamAIManager). Renumera los `@N`: relabel con `pdbatch.sh` de esas cinco unidades y
  cuatro etiquetas de `.sdata2` de GameEventScript (0x80885B10, B2C, B30, B34) corregidas a mano (+1).
- `std::map<float, int>` de MSL escrito a mano en `TeamAI_unk.cpp` (`TeamDistTree_unk`, copia de los
  arboles de `GameEventScript.h` con clave `float`): `<map>` no esta disponible. El miembro se guarda
  como `int` (los dos mapas comparten las copias debiles, asi que tienen el mismo tipo).
- `lbl_8087F048` en TeamAI_unk.cpp pasa de `BulletMgr_unk*` a `BattleTask*` (un objeto, un tipo).

## Hallazgos

- Comparaciones de la clave de un arbol MSL con `mfcr`/`srwi.`: el original pasa por un comparador que
  devuelve `bool` (`std::less`); un `<` directo da `bge`.
- Cambiar los parametros de una funcion miembro declarada en una cabecera renumera los `@N` de todas las
  unidades que la incluyen; `pdbatch.sh` deja sin renombrar las que chocan con el nombre siguiente.
