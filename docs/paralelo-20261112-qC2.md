# Lote paralelo 2026-11-12, cola C2 (Batalla, segunda pasada)

Inicio: 2026-11-12 21:32 CET. Rama `agent-C2`. Unidades: `game/BattleTask`, `game/TacView`, `game/BattleCharaTask`.

## Resultado por unidad

- `game/BattleTask`: escritas `fn_800F8548` (id de bala, 100 %), `fn_80101434` (efecto de golpe por nivel, 100 %),
  `fn_8010B250` (escala de bala por nivel con la tabla `lbl_80735ED0` definida en el fuente, 100 %),
  `fn_8010A308` (salpicadura al cruzar agua), `fn_800F3D28` (reinicio de la batalla), `fn_800F39A4` (peticiones de
  carga de recursos).

## NONMATCHING

- `fn_8010A308` 97,1 %: el original elige el tipo (0/1) con un salto por valor; todas las formas probadas dan
  `li 1` previo o `mfcr` (if/else, ternario, inline, switch); `else if (size >= 6.0f)` deja un compare de mas.
- `fn_800F3D28` 86,5 %: registros; en el original el marcador i se alcanza con un desplazamiento propio
  (i * 0x78 sobre this + 0x30000) y sus ceros usan otro registro (probado: acceso directo, inline de reset,
  accesor `fn_800F8538`).
- `fn_800F39A4`: los efectos de linea y la textura se alcanzan en el original como miembros de `this`
  (`this + 0x40000 - x` cada vez); aqui por el puntero `markers` (BattleMarkers_unk esta en el .cpp). Offsets del
  pool de cadenas distintos (fichero incompleto).

## Decisiones tomadas sin el usuario

- `fn_800F8548`, `fn_80101434`, `fn_8010B250`, `fn_8010A308` y `fn_800F3D28` se definen como `extern "C"` con
  `BattleTask*` (como `fn_800FBA30`): las llaman como `extern "C"` 80, 3, 3, 2 y 1 ficheros de otras colas
  (gmk, Em*, GameTask). **Pendiente**: pasarlas a miembros renombrando en `symbols.txt` y actualizando esos
  llamadores (renumera `@N`, `pdbatch.sh`).
- `BattleTask.h`: `mBulletId` (0x14C), `m3E314`, `mEfp3663C[31]` (0x3663C) y la declaracion
  `fn_800F39A4()` (sin parametros: no renumera `@N` en las 56 unidades que incluyen la cabecera).
- `BattleMarkers_unk` (en el .cpp): `mEfp` pasa a `atn::EffFile[6]` y `mTex` a `atn::Texture` (0x30 bytes, llega
  justo a 0x389F4); se quita `MarkerEfp_unk`.
- `CollResult_unk` local en BattleTask.cpp con el constructor inline (como `include/gmk/Lemon.h`); el tipo esta
  repetido en muchas unidades (pendiente de unificar en una cabecera).
- Punteros de `.sdata` a cadenas (`lbl_808813F4`... `lbl_808813D8`) y la tabla `lbl_80735988` siguen `extern`
  como el resto de datos del fichero (`lbl_80735DD0`).

## Hallazgos

- `(unsigned)kind <= 2` da `subfic/orc/srwi/subf/srwi 31` (comparacion sin signo con constante sin salto).
- `MIN(MAX(x, 0), 15)` como ternario anidado que repite `x > 0 ? x : 0` reproduce el `srawi/and` recalculado en
  la rama (fn_8010B250).
- `size / 8.0f` sale como `fmuls size, 0.125` con los operandos en ese orden; `size * 0.125f` los invierte.
