# Siguiente sesion (lo reescribe cada sesion autonoma)

Ultima: `auto-20261102` (sin mergear; notas en `docs/auto-20261102.md`). 11755 / 18308 al 100%.

## Por donde seguir

- `game/GameScene` (109/188): escribir los stubs en orden (`fn_803C0634`..`vf14`, `fn_803CA0F4`..`fn_803CB820`, `fn_803CC1A4`...) y el `__sinit`; pulir constructor (96%), vf0C (80%), `fn_803CC718` (97%). Antes de enlazar: mover `GetData_unk`/`Grow_unk` a `atn::SimpleArray` (y `pd.sh` en las ~100 unidades que renumera) y pasar los llamadores `extern "C"` de 19 funciones a metodos de `GameScene` (lista en las notas).
- `atn/PacketPack` (7/380): seguir en orden desde `fn_804D0164` y el constructor de `MP::Manager2`.

## Recetas

- Entorno: `docs/entorno-nube.md`.
- Unidad nueva con muchas copias weak: compilar y emparejar con `python3 tools/decomp/callpair.py <unidad> --unique --apply` (bytes identicos con relocs enmascaradas + llamadas de funciones ya identicas; repetir tras `configure`), `tools/decomp/selfnames.py <unidad> --apply [ADDR=nombre...]` para los metodos `fn_<addr>`, luego `pd.sh`.
- No renombrar en `symbols.txt` funciones que otras unidades enlazadas llaman como `extern "C"` (`grep -rn "extern.*fn_XXXX" src`): rompe el enlace (`undefined: fn_...`).
- No declarar miembros nuevos en `include/atn/Array_unk.h`: renumera las `@N` de ~100 unidades.

## Bloqueos

- `SimpleArray<T>::operator[]` real es `(unsigned) const`; cambiarlo rompe `game/AreaEffConfig` o renumera; se usa la version `int` (nombres `__vc__...Fi`).
