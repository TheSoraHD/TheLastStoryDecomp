# Plan de splits: tramos `auto_` y reparto de datos

Antes documento `SPLITS_plan.md` del proyecto de claude.ai. La rama `splits-auto` está mergeada en `main` (8c2a8fd); resumen, decisiones y pruebas de cada corte en `docs/splits-auto.md` y `docs/splits-auto-{A,B,C,D}.md`.

## Resultado de `splits-auto` (2026-10-20)

- 4 agentes por rangos de `.text`: A 0x80004000–0x800F7250, B 0x800F7250–0x8033C3D8, C 0x8033C3D8–0x804405F8, D 0x804405F8–0x805BFAC0. Integrados con `git merge --no-ff`; dos conflictos en `splits.txt`, en las unidades frontera (`EnemyQuarkSecond`, `Gun`/`HomeButtonMenu`). En la revisión se restauraron los datos de 12 unidades con fuente que un agente había reducido por error (commit bddf7ac).
- `.text` `auto_` del juego/motor **0x263718 → 0xF14 bytes**; `splits.txt` 629 → 806 unidades (179 nuevas: 118 `_unk`, 13 `_cont`). Datos en unidades `auto_` de todo el `.dol`: `.rodata` 0x357CC → 0xD15C, `.data` 0x21FB8 → 0x1037C, `.sdata2` 0x3078 → 0x928.

## Decisiones a revisar (resumen)

1. 118 nombres inventados `_unk` y algunos deducidos sin `_unk` (`atn/GraphicsDevice`, `atn/LH_Core`, `atn/LH_Handle`, `atn/NetManager`, `game/MapPartsFactory`, `game/main`).
2. 13 unidades `_cont` (a fusionar al decompilar su unidad enlazada).
3. Unidades grandes sin cortes internos (sin pruebas): `AI/AI_archer_attack01`, `game/BattleTask` (~120 KB), `atn/PacketPack` (tramo `MP::Manager*`), `game/CosEditItemList_unk`, `game/Gun`, `game/GameSoundControl`, `game/GameChoiceWindow`.
4. `Event/Node` partida en Node/Channel/Cut/Label/Section.
5. `atn/CsvBase_char_124_13_34`: son 6 cargadores; la unidad queda solo como el de `battle_eff_se_table`.
6. Nombres heredados que no cuadran: `std/bad_cast` = `GameEventScript`; `auto_03_803B27F8_text` = `GameLoadTask` (sus thunks 0x803B27AC–0x803B27F8 son de `GameEventScript`); `std/char_traits_char`/`allocator_char` son cola de otro fichero.
7. Cortes débiles y datos sin asignar por límites de dtk: lista en `docs/splits-auto.md`.

## Qué queda

- 3 tramos `.text` sueltos sin pruebas: 0x8007708C–0x80077160, 0x800F13BC–0x800F1430, 0x801E9218–0x801E9398.
- `.bss` grandes sin dueño (`auto_08_807E7800` 0x34A40, `auto_08_80837140` 0x28DD8, `auto_08_80862F30` 0x1A788): van con la unidad que las usa por primera vez.
- Datos `auto_` compartidos o con dueño ambiguo (`autoscan.py --data`).
- SDK (desde 0x805BFAC0) fuera de este plan.

## Herramienta: `tools/decomp/autoscan.py`

```bash
python3 tools/decomp/autoscan.py --min 0x100              # resumen + los 40 tramos más grandes
python3 tools/decomp/autoscan.py --min 0x100 --md x.md --tsv x.tsv   # inventario completo
python3 tools/decomp/autoscan.py --unit auto_03_8033C3D8_text       # vista por función de un tramo
python3 tools/decomp/autoscan.py --data [--tsv d.tsv]                # datos auto_ usados por una sola unidad
```
Lee `build/SLSP01/asm/**.s` (tras `ninja`); **borrar `build/SLSP01/asm` antes**. Por tramo da clases cuyos métodos de vtable caen dentro, clases que solo se construyen ahí, vecinas (`*` = con fuente), `__sinit` y candidatos de corte por constantes duplicadas. No mira cadenas ni el orden de `.data`/`.rodata`; `--data` no comprueba contigüidad.

## Método por tramo

1. `autoscan.py --unit <tramo>`: clases, `__sinit`, cortes por constantes.
2. Límites de fichero: `__sinit_*` es la última función de su fichero; vtables en orden inverso al código dentro de un fichero y en orden de enlace entre ficheros; misma constante en dos direcciones ⇒ ficheros distintos (lo más fiable); `.data`/`.rodata` alineados a 8 por fichero; `@stringBase0` al final de su `.rodata`; clases `atn::` en orden alfabético de fichero; destructores y plantillas débiles al final del primer fichero que los usa.
3. Bloque en `splits.txt` (orden por dirección); nombre `<familia>/<ClasePrincipal>.cpp` o `_unk`. Sin pruebas, no cortar.
4. Unidades enlazadas: no tocar sus límites; su resto va a `<ruta>_cont.cpp`.
5. `configure` + `ninja` → `main.dol: OK`; `regress.py` → `lost: 0`.
6. Datos: tras ampliar una unidad con fuente, renombrar su pool a `@stringBase0` (`scope:local data:string_table`); en unidades `auto_`, sin `scope:local`.
7. Al integrar: comprobar que ninguna unidad con fuente pierde rango de datos (salvo cambios anotados) y que no quedan `scope:local` nuevos fuera de unidades.
