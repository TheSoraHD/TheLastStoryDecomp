# escribir-20261116 cola N: cerca de enlazar

Inicio: 2026-11-17 02:08 (CET). Tope: 04:38.

## Tabla de la cola

| funcion | antes | despues |
|---|---|---|
| gmk/AutomaticDoor fn_803F53E0 | 93,8 (99,90 con parche qF2) | sin cambio (parche qF2 sigue aplicando, sin commit) |
| gmk/OnOff vf0C | 99,9 | sin cambio |
| gmk/OnOff fn_803F8324 | 99,2 | sin cambio |
| GmkBreakArmor ctor (chr/ControlGmkCoffinOpening) | 99,95 | sin cambio (frsp de RotZ_unk, Vec.h) |
| game/Em303KrakenFootMp vf104 | 99,8 | sin cambio (muy probada) |
| MP/D2AMpMenuCockpit fn_804B50C0 | 99,6 | sin cambio |
| MP/D2AMpMenuCockpit fn_804B5B10 | 96,7 | 99,78 (7afed4c) |
| game/GameOverTask ctor | 99,6 | sin cambio |
| game/GameOverTask vf0C | 86,0 | 96,23 (514e0c6, ee2ead9) |
| gmk/ReinforcePoint vf10 | 99,8 | sin cambio |
| game/FinishCamera_unk fn_803792F0 (faltan 1) | 95,2 | 95,41 (76845c1, c5e843b), dos diferencias reales arregladas |
| gmk/DropItem vf10 (faltan 2, hallada con boolscan) | 91,74 | 94,07 (84205c5) |
| game/Em203GreaterDaemon vf10 (boolscan/latescan) | 96,51 | 97,21 (3a55bb7) |
| game/Em203GreaterDaemonRed vfEC (latescan) | 89,96 | 95,66 (cc10c1b) |
| chr/ControlDropDown vf0C (latescan) | 98,2 | 98,69 (f19bec7) |

## Resumen

Funciones al 100 %: ninguna nueva (15892 igual que $REF). Unidades que enlazan: ninguna nueva. Las de la cola
estan en registros/planificacion muy probados; las mejoras vienen de tres trucos (inline bool en el `if`,
frame leido antes de la llamada con `GetMotionFrame`, orden de campos), aplicados tambien a unidades a 1-2 de
enlazar encontradas con los escaneos (`boolscan.py`, `latescan.py` en `/home/claude/scratch-N/bin/`).
Commits: 514e0c6, 7afed4c, 76845c1, ee2ead9, 84205c5, 3a55bb7, cc10c1b, f19bec7, c5e843b (+ notas).

## Para el port
- game/FinishCamera_unk fn_803792F0: `++m2F8 >= mEntries.mCount` pasa a `++m2F8 >= (int)mEntries.mCount` (el original
  compara con signo, cmpw); los dos `frame >= Model::fn_80097D7C(0)` leen el frame antes de la llamada (`float frame`).
- GameOverTask vf0C (no es del port): sin cambio de comportamiento (inline `PlayEfp_unk`, getters debiles).
- FinishCamera_unk fn_803792F0: inline nueva `FovY_unk(float aspect, float fovx)` (la misma formula del fovy de
  las curvas, tres sitios); sin cambio de comportamiento.
- Sin cambios de firma/layout en cabeceras compartidas (el cambio del ctor de ColorMatrixParam_unk se descarto).

## Decisiones tomadas sin el usuario
- DropItem vf10: la prueba del disparo bloqueado como `if (attack.m0C != 0) { if (IsBlocked_unk(attack)) continue; }`
  (equivalente al `||` anterior; da los saltos directos del original).
- Cockpit fn_804B5B10: el test de pantalla escrito en los dos `if` (sin la inline `IsOnScreen_unk`, borrada).
- FinishCamera: `(int)mEntries.mCount` en la comparacion con m2F8 (el original compara con signo).
- Notas de mixto-qF2 (parche de AutomaticDoor) sin aplicar: sigue en 99,90.

## Hallazgos
- MWCC evalua los argumentos de derecha a izquierda: en `FovY_unk(mCamera.mAspect, curve->fn(...))` la llamada va
  primero y la carga de mAspect queda entre la llamada y `tan` (en un FPR guardado), como el original; con el
  orden de parametros al reves la carga sale antes de la llamada (FinishCamera fn_803792F0). (escribir-20261116 qN)
- `Model::GetMotionFrame(0)` (debil 0x80139550) en `frame >= fn_80097D7C(0)` carga el frame antes de la llamada
  (en f30/f31) como el original; `mTracks[0].m08` directo lo lee despues (Em203GreaterDaemonRed vfEC 90 -> 95,7,
  ControlDropDown vf0C). `latescan.py` busca este patron. (escribir-20261116 qN)
- Una inline bool (aunque sea un solo `==`) dentro de un `||`/`&&` del `if` hace que MWCC materialice el valor de
  toda la condicion (`li rX,1 / li rX,0 / cmpwi rX,0`, `mfcr` en los float); en su propio `if` (con `continue`)
  salta directo. DropItem vf10: `if (attack.m0C != 0) { if (IsBlocked_unk(attack)) continue; }` (91,7 -> 94,1). (escribir-20261116 qN)
- Al reves, `(x & 8) != 8` sale con `cmplwi`; `!((x & 8) == 8)` dentro de una inline bool da el
  `subfic/subi/or/srwi.` del original (DropItem IsFree_unk). (escribir-20261116 qN)
- `/home/claude/scratch-N/bin/boolscan.py` (y boolscan2): busca en las funciones < 100 % de las unidades de
  near.txt un `li rX,0/1` + `cmpwi rX,0` (o `mfcr`) insertado en el nuestro: candidatas al truco anterior. (escribir-20261116 qN)
- Dos originales (MapPartsLocatorSet vf24, DropItem vf10) releen de memoria campos ya leidos tras una copia de
  Vector3 (psq) a la pila; el nuestro no relee nunca (ni con la cadena de punteros escrita dos veces, asignacion
  por campos, inline con referencia de salida, temporales explicitos, ctor por valor): la copia del original
  invalida la memoria y la nuestra no. Sin explicar. (escribir-20261116 qN)
- Orden de los campos de un struct copiado (DamageEffectParam_unk de GameOverTask vf0C): el orden de las
  asignaciones del fuente fija el orden de los stores muertos a la copia de pila; probar las permutaciones
  (zoom, color, enabled: 95,0 -> 96,2). (escribir-20261116 qN)
- Efecto en el origen (pos/rot/color debajo de los temporales de despues): inline `PlayEfp_unk(mgr, efp)` con
  `rot.x = ...; pos.x = ...; color[...] = ...` (como gmk/SwingObject) coloca los tres en el fondo de la pila
  como el original (GameOverTask vf0C 93 -> 93,1, y con `DamageEffectParam_unk damage(*GetDamage_unk())` 95). (escribir-20261116 qN)
- Una inline `bool` con una cadena de `&&` usada como condicion deja el bool en un registro (`li r4,1; cmpwi;
  beq`); escrita en el `if` salta directo (Cockpit fn_804B5B10 96,7 -> 99,8). (escribir-20261116 qN)
- `a->m08 >= a->fn()` lee el miembro despues de la llamada; el original con `float frame = a->m08;` antes lo
  carga en un FPR no volatil antes de la llamada (FinishCamera fn_803792F0). (escribir-20261116 qN)

## Bloqueos
- MapPartsLocatorSet vf24 87,9: relectura de `m17C` tras copiar mMin (ver Hallazgos); variantes 85,2-87,3. (escribir-20261116 qN)
- DropItem vf10 94,07: el original construye `m08 + m20` (fin del segmento) y lo copia antes de releer m08/m20
  para el inicio; ctor por valor o ColliCapsuleShape_unk: igual. (escribir-20261116 qN)
- GameOverTask vf0C 96,23: el original guarda `ColorMatrixParam_unk::mEnabled` del temporal al final (tras las
  filas) y el ultimo cero de `BloomParam_unk::mScale` antes de copiar el bloom. `mEnabled = 0` al final del
  cuerpo del ctor de ColorMatrixParam_unk da 98,44 aqui pero tira LastCocoonEgg (ctor de LastCocoonEggPostFx_unk),
  NewCampTask (NewCampView_unk ctor) y NewTitleTask ctor: no commiteado. `cm` con nombre + `cm.mEnabled = 0`
  (96,73 pero mueve la pila), bucle o reorden de bias/scale: peor. (escribir-20261116 qN)
- Cockpit fn_804B5B10 99,78: el original relee lbl_8087F610 al principio del cuerpo del bucle de jugadores.
  Probado: getter debil fn_800F7F90 en el cuerpo y en la condicion, referencia `MpSession_unk*& s`, while,
  GetPlayer_unk definido despues de la funcion: igual. (escribir-20261116 qN)
- Cockpit fn_804B50C0 99,60 (case 3 en r27, original r28): locales a/b, `task` compartido en toda la funcion
  (99,21), bloque, `this->vf20()`, if/else en vez de switch (96,2), cases en otro orden (64): igual o peor. (escribir-20261116 qN)
- OnOff fn_803F8324 99,16 (destino de la copia de mMtx en r6 y no r3; z de mBoxPos por 8(r8) y no 0x97c(r31)):
  `mtx = mMtx`, ctor de copia de la matriz, `off = mBoxPos`, `Assign_unk`, pos desde mMtx, off antes de
  borrar la traslacion, box antes de mtx: igual o peor. (escribir-20261116 qN)
- OnOff vf0C 99,86: inline `__OffsetMtx_unk(mtx, r, t)` con rot + Concat + SetTrans en los dos case: 96,8. (escribir-20261116 qN)
- AutomaticDoor fn_803F53E0 (parche qF2, 99,90): `GetViewVec_unk()` que devuelve la resta por valor: igual. (escribir-20261116 qN)
- ReinforcePoint vf10 99,81: getter debil fn_80121F00 para la tabla de rutas (y en el m54E4): 99,77. (escribir-20261116 qN)

## Cambios de splits/symbols
- Solo renumerados `@N` de `pd.sh` (GameOverTask, Cockpit, FinishCamera, DropItem, Em203GreaterDaemon,
  Em203GreaterDaemonRed, ControlDropDown) y `lblmap.py`/`lblseq.py --apply` en Em203GreaterDaemon(Red);
  `lbl_8088737C`, `lbl_808862C0`, `lbl_808862EC` pasan a `@N` (locales, sin usos `extern`). Splits sin cambios.

## Scripts (para /home/claude/agentes/bin)
- `/home/claude/scratch-N/bin/tryv.py <unidad> <base.cpp> <simbolo> <viejo> <nuevo>...`: copia la base, aplica
  reemplazos (con \n), compila y da el %.
- `/home/claude/scratch-N/bin/m.sh <unidad> <simbolos>`: compila la unidad y da el % (BUILD FAIL si no compila).
- `/home/claude/scratch-N/bin/side.py <unidad> <simbolo>`: odiff2 con todas las filas (lado a lado).
- `/home/claude/scratch-N/bin/boolscan.py` / `boolscan_all.py` / `latescan.py`: escaneos de los dos patrones.
