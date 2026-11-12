# Lote paralelo 2026-11-12, cola C2 (Batalla, segunda pasada)

Inicio: 2026-11-12 21:32 CET. Rama `agent-C2`. Unidades: `game/BattleTask`, `game/TacView`, `game/BattleCharaTask`.

## Resultado por unidad

- `game/BattleTask`: escritas `fn_800F8548` (id de bala, 100 %), `fn_80101434` (efecto de golpe por nivel, 100 %),
  `fn_8010B250` (escala de bala por nivel con la tabla `lbl_80735ED0` definida en el fuente, 100 %),
  `fn_8010A308` (salpicadura al cruzar agua), `fn_800F3D28` (reinicio de la batalla), `fn_800F39A4` (peticiones de
  carga de recursos) y `fn_800F3B4C` (espera de esas cargas, 100 %), `fn_8010ACCC` (totales de jugadores y
  enemigos) y `fn_8010AEF4` (comparacion de los totales, 100 %).
- Los marcadores (`BattleMarker_unk`, `BattleMarkers_unk`) pasan del .cpp a `BattleTask.h` como miembro
  `BattleTask::mMarkers` (0x367B8): con el cast `(BattleMarkers_unk*)&m367B4[4]` el compilador no plegaba los
  desplazamientos (fn_800F39A4/fn_800F3B4C).

## NONMATCHING

- `fn_8010A308` 97,1 %: el original elige el tipo (0/1) con un salto por valor; todas las formas probadas dan
  `li 1` previo o `mfcr` (if/else, ternario, inline, switch); `else if (size >= 6.0f)` deja un compare de mas.
- `fn_800F3D28` 92,9 %: registros; en el original el marcador i se alcanza con un desplazamiento propio
  (i * 0x78 sobre this + 0x30000) y sus ceros usan otro registro (probado: acceso directo, inline de reset,
  accesor `fn_800F8538`).
- `fn_800F39A4` 97,9 %: el original carga el primer puntero de cadena antes de guardar registros; offsets del
  pool de cadenas distintos (fichero incompleto).

- `fn_8010ACCC` 99,5 %: registros r5/r6 cambiados (vida del jugador y base de `this`).

## Decisiones tomadas sin el usuario

- `fn_800F8548`, `fn_80101434`, `fn_8010B250`, `fn_8010A308` y `fn_800F3D28` se definen como `extern "C"` con
  `BattleTask*` (como `fn_800FBA30`): las llaman como `extern "C"` 80, 3, 3, 2 y 1 ficheros de otras colas
  (gmk, Em*, GameTask). **Pendiente**: pasarlas a miembros renombrando en `symbols.txt` y actualizando esos
  llamadores (renumera `@N`, `pdbatch.sh`).
- `BattleTask.h`: `mBulletId` (0x14C), `m3E314`, `mEfp3663C[31]` (0x3663C) y la declaracion
  `fn_800F39A4()` (sin parametros: no renumera `@N` en las 56 unidades que incluyen la cabecera).
- `BattleMarkers_unk` en `BattleTask.h` con `mEfp` (6 `atn::EffFile`) y `mTex` (`atn::Texture`, 0x30 bytes) como
  almacenamiento crudo `unsigned char` y accesores `GetEfp_unk(int)`/`GetTex_unk()` declarados sin nombre de
  parametro y definidos inline en el .cpp: incluir `atn/ResLoader_unk.h`/`atn/Texture_unk.h` en la cabecera, o un
  cuerpo inline con parametro con nombre, cambia los `.o` de las unidades que la incluyen (probado con BulletPoint,
  Arms, ControlDive, ReplayBuffer); un struct solo con datos no. `fn_800F72F4` (por encima del presupuesto de
  inline) usa los casts directos, no los accesores. Se quita `MarkerEfp_unk`.
- `BattleTask.h`: `m38CA4` es un `BattleSel_unk` (copia anterior de `m38C90`), `m38CCC`/`m38CD0` (resultado
  de la comparacion y el anterior) y `BattleSel_unk::IsValid_unk() const` (declarada sin parametros, definida
  inline en el .cpp).
- `mEfp363C0[46]` se parte en `mEfp363C0[26]`, `mSplashEfp[3]` (0x364F8) y `mEfp3651C[17]` (los bucles sobre
  `mEfp363C0[26 + i]` no se reducian a puntero).
- `CollResult_unk` local en BattleTask.cpp con el constructor inline (como `include/gmk/Lemon.h`); el tipo esta
  repetido en muchas unidades (pendiente de unificar en una cabecera).
- Punteros de `.sdata` a cadenas (`lbl_808813F4`... `lbl_808813D8`) y la tabla `lbl_80735988` siguen `extern`
  como el resto de datos del fichero (`lbl_80735DD0`).

## Hallazgos

- Una struct solo con datos anadida a una cabecera no cambia los `.o` de quien la incluye; un metodo inline con
  parametro con nombre o un `#include` de cabeceras con declaraciones con nombre si.

- `(unsigned)kind <= 2` da `subfic/orc/srwi/subf/srwi 31` (comparacion sin signo con constante sin salto).
- `MIN(MAX(x, 0), 15)` como ternario anidado que repite `x > 0 ? x : 0` reproduce el `srawi/and` recalculado en
  la rama (fn_8010B250).
- `size / 8.0f` sale como `fmuls size, 0.125` con los operandos en ese orden; `size * 0.125f` los invierte.
