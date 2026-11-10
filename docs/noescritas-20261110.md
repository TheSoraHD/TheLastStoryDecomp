Inicio: 2026-11-10 12:28 (Europe/Madrid). Cierre minimo: 2026-11-11 00:28.

# noescritas-20261110

Rama desde `main` 4ef537b: 14958 / 18109, lost 0, `main.dol: OK`. Funciones al 0 % (report.json): 1148.

## Decisiones tomadas sin el usuario

- **Bloque 1: la inline de copia no va en `include/math/Vec.h` sino en `include/math/VecAssign_unk.h`** (incluida solo por
  las 33 unidades que la usan). Probado en `Vec.h`: una inline mas en esa cabecera renumera los `@N` de datos de ~300
  unidades (labelcheck peor en casi todas) y los `@enum$N` de los nombres mangled (ControlAppear perdia 2 funciones
  boost con `@enum$6816` en el nombre). Con la cabecera aparte: lost 0, sin bajadas, solo `lblmap` en HomingBullet y
  Cannonball.

## Cambios de splits

## Resultados por unidad

## Hallazgos
