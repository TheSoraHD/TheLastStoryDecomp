# mwcc-blockers q4: las 18 constantes ptmf huerfanas de `.data` de `game/CharaTask`

Rama `agent-4`, base `main` 6ace171. Fecha 2026-10-21.

## Resultado

| unidad | antes (ref) | despues | commits |
|---|---|---|---|
| game/CharaTask | 264/436 | 371/436 (+107) | 22ef1fc (+105), 1b5b839 (+2) |
| total DOL | 5375 | 5482 (+107) | |

`main.dol: OK`, `regress.py` lost: 0, ninguna funcion baja de fuzzy (script propio
`/home/claude/scratch/agent-4/fuzzycmp.py`: down 0, up 115).

## Hallazgo MWCC (una linea)

**Los 18 ptmf huerfanos eran 7 lanzadores de control (`fn_80178208(new chr::ControlX(...))`) que el
enlazador elimino por no tener llamador: el .o original conserva sus 3 constantes ptmf de `.data`
(safe-bool de `boost::scoped_ptr` del `fn_80178208` inlineado), pero no su codigo.** Reponerlos como
stubs `Stripped1_unk..Stripped7_unk` (cuerpo `fn_80178208(0);`, cada uno inlinea la cadena una vez =
3 ptmf) coloca el resto de `.data` en el offset original y 105 funciones que solo diferian en el
offset desde `lbl_8077A720` pasan a 100%.

Segundo hallazgo: **cuando una funcion inlinea DOS cadenas `fn_80178208`, el original lanza el
segundo control a traves de un envoltorio inline mas** (metodo `fn_80161570`/`fn_8016D0A0` ya
existente o un `static` helper `StartX_unk`). El nivel inline extra cambia el orden de asignacion de
los slots de pila de los temporales ptmf (el par "tmp/copia" del primer `__ptmf_test`); sin
envoltorio, MWCC da al segundo par los slots del primero. Esto arregla `fn_80138E1C`, `fn_8017547C`
y deja los slots iguales en `fn_80140588`, `fn_801426A4` y `fn_8013CB68` (siguen NONMATCHING por
otros motivos ya documentados en sus comentarios).

Nota: "18 constantes" era en realidad 7x3 = 21 ptmf de mas en el original menos 3 que nosotros
emitiamos de mas en `fn_8017B434` (0%, sin escribir del todo): 0xD8 = 216 = 18x12 bytes netos.

## Inventario (antes de tocar nada)

Script `/home/claude/scratch/agent-4/classify.py game/CharaTask`: de 213 funciones <100% en objdiff,
108 diferian SOLO en offsets de `.data` (`addi/lwz rX, off(r31)` con base `lbl_8077A720`) y 20 mas
tenian ademas 1-3 diferencias (ptmf con `lwzu` directo, @NNN vs lbl_). `ptmfmap.py` (indice de ptmf
por funcion, target vs nuestro) mostro que el desfase crece por saltos de +3 en 7 puntos del fichero:

| hueco (indices ptmf target) | entre | stub |
|---|---|---|
| 188-190 | vf74 / fn_80155790 | Stripped1_unk |
| 240-242 | fn_801591E8 / fn_801595BC | Stripped2_unk |
| 501-503 | fn_8016AFE8 / fn_8016B8D4 | Stripped3_unk |
| 507-509 | fn_8016B8D4 / fn_8016BBCC | Stripped4_unk |
| 513-515 | fn_8016BBCC / fn_8016BEDC | Stripped5_unk |
| 519-521 | fn_8016BEDC / fn_8016C1E0 | Stripped6_unk |
| 605-607 | fn_8017547C / fn_801765D8 | Stripped7_unk |

Ya habia un precedente en el fuente: `GetControlF20_unk` (sin llamador, solo deja la copia debil de
`ControlBase::vf20`). Los otros huecos sin referencias (9, 12-18, 52-73, 78-122, 133-178) son
iguales en target y nuestro: son ptmf de funciones con `-ipa file` que resuelven el `if (mControl)` en
tiempo de compilacion pero cuyos datos se emiten igual.

Funciones con slots de ptmf intercambiados (segundo bloqueo): `fn_80138E1C`, `fn_8017547C`,
`fn_80140588`, `fn_801426A4`, `fn_8013CB68` (primer par). Slots ya iguales: `fn_8016AFE8`,
`fn_8015802C`, `fn_8015EFAC`, `fn_801656A4`, `fn_8016EB48`, `vf80`.

## Intentos

1. Reconstruir el orden de `.data` (dataseq.txt): target y nuestro identicos salvo 18 ptmf extra
   justo antes de `__vt__9CharaTask` ... FALSO: los 18 estan repartidos en 7 huecos por el fichero
   (el diff de secuencia alineaba mal; `ptmfmap.py` por indice lo revelo). Nada que cambiar.
2. Stubs `Stripped1_unk..7_unk` con `fn_80178208(0)` en las 7 posiciones: 238 -> 369 funciones al
   100% en CharaTask (fnstat 223 -> 238 por el bug de `__ptmf_null`-vs-`@NNN` de fnstat; el dato
   bueno es report.json). COMMIT 22ef1fc.
3. Banco `try.sh` con fn_8017547C + cadena: el orden de definicion de las tres funciones no cambia
   los slots (variantes A-E, todas iguales). El tamano de fn_80178208 standalone (185 insns) es el
   del DOL (173 + prologo), asi que la cadena esta bien expandida.
4. Envoltorio `static StartDragged_unk(chara, dragger)` para el SEGUNDO lanzamiento: slots exactos
   del target (74/20, 80/8). Con envoltorio en los dos lanzamientos: se intercambian los dos
   primeros pares (50/44, 5c/2c): el envoltorio debe ir solo en el segundo. Aplicado -> 100%.
5. `fn_80138E1C`: `fn_80161570(motion, 0, 0, -1.0f, -1.0f)` (metodo existente que hace el mismo
   new+fn_80178208) en los dos lanzamientos de ControlOneMotion -> 100%. Aqui los dos usan el
   envoltorio porque estan en ramas distintas de un switch (no en secuencia).
6. `fn_80140588`/`fn_801426A4`: `fn_801750FC(); fn_80178208(new ControlSlip(this,0))` ->
   `fn_8016D0A0(0)` (que ya hace ambas cosas): slots iguales; quedan las otras diferencias
   (registros, scheduling) de sus comentarios. Primer intento con `fn_801750FC()` duplicado bajo a
   95%: cuidado, `fn_8016D0A0` ya llama a `fn_801750FC`.
7. `fn_8013CB68`: `StartPushedAvoid_unk(other, dir, this)` static inline para ControlPushedAvoid:
   el primer par de slots pasa a ser el del target. COMMIT 1b5b839 (intentos 4-7).
8. `CharaStatus`: no inlinea la cadena; sus 17 NONMATCHING son `.sdata`/strings/registros, fuera
   del bloqueo. Sin cambios.
9. Otras unidades con `__ptmf_test` de la cadena (BattleCharaTask 6, Em201/205/209 2 cada una):
   sin fuente todavia; no aplica.

## Lo que NO funciona (no repetir)

- Cambiar el orden de definicion de `fn_8016E970` / `fn_80178208` / el lanzador: no afecta a slots.
- Envoltorio inline en AMBOS lanzamientos de una funcion con dos cadenas consecutivas: intercambia el
  primer par; solo el segundo lanzamiento lleva envoltorio.
- Duplicar `fn_801750FC()` antes de `fn_8016D0A0` (ya la incluye).
- `vf48` (98.6%) no es de este bloqueo: es la copia de `CameraState_unk` (addi r9 al ultimo bloque).

## Decisiones sin el usuario

- Nombres inventados `StrippedN_unk`, `StartDragged_unk`, `StartPushedAvoid_unk` (comentados como
  `inferred`/`name invented`). Los stubs se declaran en `include/game/CharaTask.h` junto a
  `GetControlF20_unk`.
- Se retiran los comentarios "solo difieren los offsets de .data" y los NONMATCHING de
  `fn_80138E1C` y `fn_8017547C`.
- `fn_8017B434` (0%, escrito a medias) emite 7 ptmf frente a 2 del target; no tocado (su propio
  comentario NONMATCHING lo cubre).

## Herramientas (scratch `/home/claude/scratch/agent-4/`)

- `classify.py <unit> [-v]`: clasifica cada funcion <100% en DATA-ONLY (solo offsets de `.data`) u
  OTHER con el detalle de instrucciones.
- `ptmfmap.py`: indice de cada constante ptmf `{0,-1,fn}` cargada por funcion, target vs nuestro
  (relocs de `.text` a `.data` + `addi/lwz` sobre r31). Detecta los huecos.
- `slots.py <sym>...`: pares (tmp, copia) de slots de pila de los temporales ptmf por expansion.
- `fuzzycmp.py <report> <ref>`: subidas/bajadas de fuzzy por `virtual_address`.
- `try.sh`: copia de `tools/decomp/try.sh` con rutas absolutas (la del repo busca
  `build/tools/sjiswrap.exe`, que no existe en esta copia).
