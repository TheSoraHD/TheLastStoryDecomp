# Siguiente sesion (lo reescribe cada sesion autonoma)

Ultima: `auto-20261102` (notas en `docs/auto-20261102.md`).

## Recetas

- Entorno: `docs/entorno-nube.md`; `configure.py` con las rutas de `tls-build` (script en las notas de la sesion).
- Emparejar nombres de una unidad nueva: compilar, y renombrar los `fn_` del objetivo con las funciones nuestras de bytes identicos (relocs enmascaradas) y por las llamadas de funciones ya identicas (script `callpair.py` descrito en `docs/auto-20261102.md`); luego `pd.sh`.
- No declarar miembros nuevos en `include/atn/Array_unk.h`: aunque no se usen, renumeran las `@N` de ~100 unidades. Para ayudas de `SimpleArray` usar una subclase local (`gs::Array_unk` en `include/game/GameScene.h`).

## Bloqueos

- `SimpleArray<T>::operator[]` real es `(unsigned) const` pero cambiarlo rompe `game/AreaEffConfig` o renumera; se usa la version `int` (nombres `__vc__...Fi`).
