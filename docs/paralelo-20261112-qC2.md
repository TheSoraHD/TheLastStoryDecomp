# Lote paralelo 2026-11-12, cola C2 (Batalla, segunda pasada)

Inicio: 2026-11-12 21:32 CET; cierre 22:50 CET. Rama `agent-C2`. Unidades: `game/BattleTask`, `game/TacView`, `game/BattleCharaTask`.

## Resultado por unidad

- `game/BattleTask`: escritas `fn_800F8548` (id de bala, 100 %), `fn_80101434` (efecto de golpe por nivel, 100 %),
  `fn_8010B250` (escala de bala por nivel con la tabla `lbl_80735ED0` definida en el fuente, 100 %),
  `fn_8010A308` (salpicadura al cruzar agua), `fn_800F3D28` (reinicio de la batalla), `fn_800F39A4` (peticiones de
  carga de recursos) y `fn_800F3B4C` (espera de esas cargas, 100 %), `fn_8010ACCC` (totales de jugadores y
  enemigos), `fn_8010AEF4` (comparacion de los totales, 100 %), `fn_801002DC` (objetivo de la camara, miembro
  nuevo), `fn_8010A868` (efectos temporizados `mStuns`, 100 %) `fn_80108C10` (popup de dano) y
  `fn_80102EAC` (odio hacia el atacante y los que siguen al personaje), `fn_80108F38` (popup del nombre de un
  estado, 100 %) y `fn_8010A4A8` (alta en `mStuns`, efecto y popup).
- Resumen BattleTask: funciones al 0 % 68 -> 53 (15 escritas, sin `DRAFT`); 7 nuevas al 100 %
  (`fn_800F8548`, `fn_80101434`, `fn_8010B250`, `fn_800F3B4C`, `fn_8010AEF4`, `fn_8010A868`, `fn_80108F38`);
  `report.json` 15179 -> 15187. TacView (6) y BattleCharaTask (1) sin tocar: sus funciones al 0 % son de
  2-8 KB (`fn_805ADB28`, 216 bytes, es el `operator+(wstring, wstring)` de MSL que solo emite `fn_805AC758`).
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

- `fn_801002DC` 99,9 %: pila; el original pone la diferencia en 0x14 y la suma escalada en 0x20 (probado:
  asignacion aparte, inline del centro, local del objetivo).

- `fn_80108C10` 97,8 %: el `return` por dano vacio sale `bne/b` en el original; `lbl_8087F610` se recarga antes
  de mirar el modo; la cadena de formato esta en la tabla de cadenas anchas del fichero (incompleto).

- `fn_80102EAC` 99,6 %: registros de la entrada y del puntero de la lista cambiados (r29/r30).

- `fn_8010A4A8` 95,8 %: el original vuelve a leer `mStunCount` para el `push` despues de guardar la entrada; la
  altura se carga en el otro orden de registros; offsets del pool de cadenas. La copia debil del destructor de
  `atn::static_string<32, wchar_t>` (0x8010A828) que el original emite detras no sale (el destructor no esta
  definido en `atn/static_string.h`).

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
- `BattleTask.h`: `mTargetCount`/`mTargets[64]` (0x48/0x4C, lista de objetivos de la camara), `mStunCount`,
  `mStuns[4]` (`BattleStun_unk`, 0xC bytes) y `mStunEfp[2][3]` (0x3E348, cargados en el constructor); metodos
  `fn_801002DC` y `fn_8010A868` declarados sin nombres de parametro. Renombrados en `symbols.txt`:
  `fn_800F39A4`, `fn_801002DC`, `fn_8010A868` a sus nombres de miembro (solo los llama asm: el constructor y
  GameCamera_unk).
- `mEfp363C0[46]` se parte en `mEfp363C0[26]`, `mSplashEfp[3]` (0x364F8) y `mEfp3651C[17]` (los bucles sobre
  `mEfp363C0[26 + i]` no se reducian a puntero).
- `fn_80102EAC` tambien `extern "C"`, con un quinto parametro sin nombre que no se lee (Em101ElderZesya pasa -1).
  `fn_8010ACCC` usa `CharaTask::CanAct_unk()` (ya existia) en vez de inlines locales.
- `fn_8010A4A8` tambien `extern "C"` (game/CharaTask.cpp lo llama asi; se probo como miembro y se volvio atras
  para no romper ese llamador). Push de `mStuns` con `new` de colocacion (`std/new.h`) en un inline local.
- `fn_80108F38` tambien `extern "C"` (Em202ForestSpider, Em308ParmCrab...); `BattleMsg_unk` se mueve antes de
  ella en el .cpp.
- `fn_80108C10` tambien `extern "C"` (12 llamadores `extern "C"`); su registro de dano es otra vista local
  (`BattleDamage_unk`, como `StatusDamage_unk`/`DaemonDamage_unk` de otras unidades) y `Arena_unk` local
  (como en CharaTask.cpp) para `lbl_8087F610`, que pasa de `void*` a `Arena_unk*`.
- `CollResult_unk` local en BattleTask.cpp con el constructor inline (como `include/gmk/Lemon.h`); el tipo esta
  repetido en muchas unidades (pendiente de unificar en una cabecera).
- Punteros de `.sdata` a cadenas (`lbl_808813F4`... `lbl_808813D8`) y la tabla `lbl_80735988` siguen `extern`
  como el resto de datos del fichero (`lbl_80735DD0`).

## Pendientes

- Pasar a miembros de `BattleTask` las funciones `extern "C"` con `BattleTask*` (fn_800F8548, fn_80101434,
  fn_8010B250, fn_8010A308, fn_800F3D28, fn_80108C10, fn_80102EAC, fn_80108F38, fn_8010A4A8, y la vieja
  fn_800FBA30) junto con sus llamadores `extern "C"` de otras colas (renumera `@N`).
- Unificar `CollResult_unk`, `Arena_unk` y el registro de dano (`BattleDamage_unk`) en cabeceras.
- Definir los datos de `.sdata`/`.rodata` del fichero que siguen `extern` (`lbl_808813F4`..., `lbl_80735988`,
  `lbl_80735DD0`).

## Hallazgos

- `return k == 1 || k == 4 || k == 5;` en un inline `bool` da la prueba de bits (`slw`/`andi. 0x19`); con `if` y
  variable o `switch` sale una cadena de comparaciones.
- Un borrado de vector inline (`for (i = it - data; i < n - 1; i++) data[i] = data[i + 1]; n--;`) dentro de un
  helper inline intercambia los registros del indice y del puntero respecto al bucle escrito en linea.
- Un helper inline con los argumentos de una llamada (`PlayEfpScaled_unk(efp, model, scale)`) adelanta el calculo
  de esos argumentos antes de los stores del color temporal.

- Una struct solo con datos anadida a una cabecera no cambia los `.o` de quien la incluye; un metodo inline con
  parametro con nombre o un `#include` de cabeceras con declaraciones con nombre si.

- `(unsigned)kind <= 2` da `subfic/orc/srwi/subf/srwi 31` (comparacion sin signo con constante sin salto).
- `MIN(MAX(x, 0), 15)` como ternario anidado que repite `x > 0 ? x : 0` reproduce el `srawi/and` recalculado en
  la rama (fn_8010B250).
- `size / 8.0f` sale como `fmuls size, 0.125` con los operandos en ese orden; `size * 0.125f` los invierte.
