# Cola T (port-decomp-20261114): cuarta pasada de combate

Rama `agent-T`. Unidades: `game/BattleTask`, `game/AiTarget`, `game/BattleCharaTask`. Lote corto (~35 min de trabajo
efectivo antes del tope de las 12:15).

## Resultado por funcion

- `BattleTask::vf14` (0x800F5314, 7996 bytes): **NONMATCHING 85,4 -> 90,3 %** (fnstat; el comentario dice 90,1 %).
  Firma sin cambios `void vf14()`. Equivalente: solo cambia la forma del C++.
  - Seis matrices distintas en el bucle de disparos (0x358-0x448 en el original): cada giro es una inline
    `RotateY_unk(atn::Vector3&, float)` (fichero, nombre inventado) con su propia `atn::Matrix3x4`. El marco pasa
    de 0xDB0 a 0xEB0 (el del original).
  - Los datos del ataque (`m50`/`m54` de `HitInfo_unk`, angulo del arco y media altura) se vuelven a leer en cada
    uso (`shot->mData` cada vez), sin locales `data`/`angle`/`width`: el original los carga de memoria en cada
    uso y asi sobra el registro f21. Accesores locales `HitArc_unk`/`HitHeight_unk` (BattleTask.cpp).
  - Giro de la flecha: `rot = 57.295776f * mCB64.ToRotation_unk().y` (inline de Vec.h) en lugar de copia +
    `fn_80139F60()` (+0,1 %).
  - Base local `BattleChara_unk* charas = mCharas;` en los bucles de depuracion (modo 1/2, modo 3 y HateAtt): el
    original guarda la base en un registro aparte del puntero de la entrada (+1,7 %). Una sola base para los
    dos modos baja.
  - Resto: el orden de las locales de la pila (las del `fn_80139F60` inline y `aim` quedan por debajo de las
    matrices y del primer texto en el original; 0x10 bytes menos en el nuestro) y registros.
- `BattleTask::__ct__` (0x800F1430): **99,99 -> 100 %** (fnstat) solo por las etiquetas (abajo).
- `BattleTask::vf10` (98,5 %), `fn_80103FD4` (98,3 %), `fn_800F8C6C` (89,9 %), `fn_800FDF14` (92,3 %),
  `AIManager::fn_80017128` (90 %): sin cambios (ver hallazgos). `fn_801002DC` (99,93 %): 8 variantes de la
  expresion del centro, ninguna cambia el orden de los dos temporales.

## Etiquetas

- `symbols.txt` de BattleTask: 13 etiquetas de `.sdata2` viejas (nombres repetidos `@31579`, `@32638`, `@32649`,
  `@32775` y `lbl_808814EC/F4/FC/80881534`) renombradas a mano a los `@N` de nuestro `.o` (mismo valor y mismo
  desplazamiento): `pairdata` no puede con nombres repetidos. Quedan `@35784` (x2) y las de 0x80881598 en adelante
  (funciones posteriores con otro orden).
- Tras cambiar vf14 el `@N` de la unidad se renumera (~100 etiquetas): `pd.sh` las deja mal (funciones < 99 %,
  labelcheck +15, rep100 peor). Se resuelve con `maprelabel.py` (scratch-N) entre el `.o` del commit anterior y el
  nuevo (`/home/claude/scratch-T/relabel.sh`): PRECOMMIT OK sin `--pd`.

## Hallazgos (una linea)

- `fn_800FDF14`: el original usa un registro base (r31 = inicio de `.rodata`, 0x807355C8) para los dobles
  (+0x8E8, +0x8F8), `sNoHead_unk` (+0x8D0) y `lbl_80735DD0` (+0x808): hace falta el `.rodata` de la unidad
  definido en el fuente (el nuestro tiene 0x69D de 0xF80 bytes; `lbl_80735DD0` sigue `extern`). Bloquea ~5 %.
- `fn_800F8C6C`/`MeleeSort_unk`/`MeleeMedian3_unk`: con `MeleeTarget_unk() {}` las copias salen miembro a miembro
  (`lfs/stfs` del float) como en el original y `MeleeMedian3_unk` llega al **100 %** (MeleeSort 64,5 -> 73,3 %),
  pero `push_back` deja el camino POD (comprobacion de nulo): `fn_800F8C6C` baja a 87,2 %. Probado sin commitear:
  con `std/vector.h` despachando por copia trivial (`__builtin_trivial_members(T) & 4` en lugar de
  `__builtin_is_pod(T)`, lineas 118, 232 y 250) `fn_800F8C6C` sube a 90,5 %. Confirma el bloqueo ya apuntado en
  `docs/mwcc-blockers.md` (push_back por copia trivial): decision del usuario.
- `vf10`: el orden de carga de las constantes 50/80/10/100 del `switch` del medidor no cambia con el orden de los
  `case` ni con el ternario al reves (12 combinaciones, ninguna mejora); `const` en `leader` no cambia nada.
- `fn_80103FD4`: la recarga de `mCharaCount` del original en `FindChara_unk` es por presion de registros (r0
  ocupado por los indicadores de `CanAct_unk`), no un cambio de semantica; base local de `mCharas` baja a 97 %.
- `valdiff` de BattleTask: todos los avisos restantes son cargas de constantes en otro orden (registros).

## Cambios de firma o de layout (para el port)

- Ninguno. Solo inlines nuevas locales de `game/BattleTask.cpp`: `RotateY_unk(atn::Vector3&, float)`,
  `HitArc_unk(const HitInfo_unk*)`, `HitHeight_unk(const HitInfo_unk*)`.

## Pendientes

- Tipar `HitInfo_unk::m50/m54` como `float` en `game/HitInfo_unk.h` (cabecera de 200 ficheros; `LH_Binary.cpp`
  lo recorre como bytes): entonces sobran `HitArc_unk`/`HitHeight_unk`.
- `.rodata` de BattleTask en el fuente (para `fn_800FDF14` y otras con `lbl_80735DD0`).
- `std/vector.h` por copia trivial (ver hallazgos), decision del usuario.
- `tools/decomp/pairdata99.py` copiado sin commitear.
