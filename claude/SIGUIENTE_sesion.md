# Siguiente sesion (lo reescribe cada sesion autonoma)

Ultima: `auto-20261102` (mergeada; notas en `docs/auto-20261102.md`). 11764 / 18308 al 100%.

## Por donde seguir

- `game/GameScene` (117/188): escribir los stubs en orden (`fn_803C0634`..`vf14`, `fn_803CA0F4`..`fn_803CB820`, `fn_803CC1A4`...) y el `__sinit`; pulir constructor (96%), vf0C (78%), `fn_803CC718` (97.7%), plantillas de grupos (98%: orden del pool `SceneCharaGroup`/`ScenePointGroup`).
- `atn/PacketPack` (7/380): seguir en orden desde `fn_804D0164` y el constructor de `MP::Manager2`.

## Recetas

- Entorno: `docs/entorno-nube.md`.
- Unidad nueva con muchas copias weak: compilar y emparejar con `python3 tools/decomp/callpair.py <unidad> --unique --apply` (bytes identicos con relocs enmascaradas + llamadas de funciones ya identicas; repetir tras `configure`), `tools/decomp/selfnames.py <unidad> --apply [ADDR=nombre...]` para los metodos `fn_<addr>`, luego `pd.sh`.
- No renombrar en `symbols.txt` funciones que otras unidades enlazadas llaman como `extern "C"` (`grep -rn "extern.*fn_XXXX" src`): rompe el enlace (`undefined: fn_...`).
- No declarar miembros nuevos en `include/atn/Array_unk.h`: renumera las `@N` de ~100 unidades.

- Re-emparejar `@N` de muchas unidades a la vez: `resetlocal.py` de todas, `configure` + `ninja`, `pairdata.py --apply` de todas, `configure` + `ninja`, y luego `regress.py`/`fuzzdrop.py`/`labelcheck.py` (mucho mas rapido que `pd.sh` por unidad). Revisar el diff de `symbols.txt` por nombres sin mangling (`sInstance`) que choquen al enlazar.

## Bloqueos

- `atn::SimpleArray::operator[]` es `(unsigned int) const`: en codigo nuevo, indices `unsigned` salvo que el original muestre otra cosa.
