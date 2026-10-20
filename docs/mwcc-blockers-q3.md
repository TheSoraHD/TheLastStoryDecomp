# mwcc-blockers q3: copias de `atn::Vector3` (`__VecCopy`): r3/r4 cruzados y "z primero"

Agente 3, rama `agent-3`, copia `/home/claude/work3`. Scratch: `/home/claude/scratch/agent-3/`
(`try.sh` con las rutas de esta maquina, `bench.sh`/`bench2.sh` banco de funciones reales,
`fpat.sh`/`fpat2.sh`/`ordscan.sh` bancos sinteticos, `pscopies*.py` inventario del DOL,
`fuzzdrop.py` bajadas de fuzzy, `matrix.py` matriz de ordenes de asm).

## Inventario

### Ordenes de las copias paired-single en TODO el DOL (`pscopies.py`, 3400 sitios)

Codigo: L = `psq_l f1`, Z = `lfs f2` (z), S = `psq_st f1`, T = `stfs f2`.

| orden | sitios | mismo registro base en los dos stores | distinto |
|---|---|---|---|
| ZLST | 1423 | 1090 | 333 |
| LZTS | 1144 | 512 | 635 |
| LZST (el de `__VecCopy`) | 357 | 126 | 234 |
| LSTZ | 265 | 238 | 3 |
| LSZT | 140 | 67 | 73 |
| LTZS | 90 | 47 | 43 |
| ZLTS | 4 | | |

Es decir: el orden "canonico" de nuestro helper (LZST) es el TERCERO mas frecuente. Los dos
mayoritarios son ZLST (z cargada primero) y LZTS (z guardada primero).

### Funciones NONMATCHING del repo atribuidas al bloqueo (grep NONMATCHING + VecCopy/z/swap)

Sub-bloqueo A (r3/r4 cruzados, misma secuencia de instrucciones):

| unidad | funcion | addr | % |
|---|---|---|---|
| chr/ControlUseSkill | vf0C | 0x801C9668 | 99.95 |
| chr/ControlDetect | ControlDetectHateAtt::vf0C | 0x801B0920 | 99.9 |
| chr/ControlCliffhang | fn_801AB6B8 | 0x801AB6B8 | 99.9 |
| chr/ControlOneMotion | ControlOneMotionTransition ctor | 0x801C44A0 | 99.26 |
| chr/ControlStagger | ControlSlip ctor | | 99.7 |
| atn/PBoneBasic | vf0C / vf14 / OutForce vf34 | 0x800A7944 / 0x800A8468 / 0x800A8CB8 | 99.6 / 99.6 / 99.9 |
| game/ColliAttrManager | fn_80184864 | 0x80184864 | 99.7 |
| game/CharaTask | fn_8014DEE4 (copias de matriz) | | ~90 |
| game/CharaTask | fn_80174104 | 0x80174104 | 96 |
| game/ChaserGuard | fn_8017FBEC | 0x8017FBEC | 99.9 |
| game/Em209BigGolem | vf20 | | 99.7 |
| game/Em002_02SkeltonKing | vf20 | | 99.9 |
| gmk/Cannonball | fn_803FBC34 | | 99.72 |

Sub-bloqueo B (orden de z distinto):

| unidad | funcion | orden original | % |
|---|---|---|---|
| game/CostumeEditChara | fn_801E9168 / fn_801E917C / fn_801E91A0 | LZTS | ~60 |
| game/TownTask | fn_805B7B04 | LZTS | 34 |
| chr/ControlWallUp | ControlWallSlash::vf18 (vf14) | LZTS | 60 |
| atn/PBoneBasic | PBoneOutForce::vf28 | LZTS | 75 |
| game/PathData_unk | fn_8054856C | mezcla | 89.9 |
| gmk/RandomMerchant | vf10 | ZLST + LZTS | |
| game/CharaTask | varias (ver `Assign_unk` a mano) | ZLST | |

## Lo ya probado antes de esta sesion (comentarios NONMATCHING / docs)
- `Vector3& p = (v = a + b);` arregla algunos casos A, no los ctors de control.
- ControlOneMotion: global plano, locales ref/puntero, parametro, `__VecCopy` directo (r4/r5 siguen cruzados).
- `Assign_unk()` (`__VecCopyZFirst`) a mano en ~10 sitios: funciona cuando la copia es Z-primero
  (ZLST) pero no cuando el original guarda z primero (LZTS: CostumeEditChara, TownTask, WallSlash,
  PBoneOutForce::vf28).
- Copia con `__VecCopy` directo en vez del ctor copia invierte los registros (batch-10kb-units).

## Intentos

(la lista se rellena a continuacion)

### Banco de pruebas
- Reales (`bench2.sh`, 37 funciones de 20 unidades): ControlUseSkill::vf0C, ControlDetectHateAtt::vf0C,
  PBoneBasic vf0C/vf14/OutForce vf34/vf28, ColliAttrManager fn_80184548/fn_80184864, GameUI
  fn_803CF74C/ctor, LH_ColliObject fn_80472424, ControlOneMotion ctors, CostumeEditChara x2, TownTask
  fn_805B7B04/vf10, WallSlash vf18/vf1C, PathData fn_8054856C, WaitObject, MapLift, RandomMerchant,
  WaterLevel, OnOff, ControlWarp, Rain, CharaTask vf50/fn_80174104, Cannonball, Em209BigGolem,
  ChaserGuard, CharaSub, ControlCliffhang, Lemon, EnvSeLine.
- Sintetico (`forms3.h` + `fpat2.sh`): 5 patrones con el orden de registros del original:
  P1 `local = temp_de_inline` (0x801C9830: dst en el registro menor), P2 `miembro_lejano = global`
  (0x803CF74C: src menor), P3 `miembro de local = local con nombre` (0x804727F8), P4 `miembro via lwz
  = temp de operator-` (0x801B0A04: src menor), P5 `ctor local = local con nombre` (0x80184604: src
  menor). P3/P4 del banco sintetico no reproducen el original ni en la base (no fiables); P1/P2/P5 si.

### Intentos (sub-bloqueo A, registros)
1. `__VecCopy(register const float* src, register float* dst)` (src primero en la firma, ctor y
   operator=): banco real +5 (UseSkill, PBone x3, ColliAttr fn_80184864) pero fn_80184548 8->16
   (P5: ctor desde local con nombre se invierte). NO vale.
2. Solo operator= con un wrapper sin `register` `__VecCopyS(src,dst){__VecCopy(dst,src);}`: igual
   que 1 en los que suben, pero ControlOneMotion pierde 4 funciones (el `c2` truco deja de hacer
   falta y rompe). NO vale.
3. Solo operator= con helper asm src-first con `register` (mismas 4 instrucciones): ninja completo:
   100% 5375 -> 5382 (+9: UseSkill vf0C, PBone vf0C/vf14/vf34, ChaserGuard fn_8017FBEC, ColliAttr
   fn_80184864, SkeltonKing vf20, BigGolem vf20, Cannonball fn_803FBC34) pero 2 perdidas (GameUI
   fn_803CF74C `mD40.mPos = lbl_807C7030` P2; LH_ColliObject fn_80472424 P3) y 4 bajadas de fuzzy.
   NO vale tal cual (condicion 8).
4. Dos sobrecargas `operator=(Vector3&)` (dst,src) + `operator=(const Vector3&)` (src,dst): en el
   banco sintetico P1+P2+P5 OK a la vez (primera vez). Pero MWCC entonces genera el operator= implicito
   de los structs que contienen Vector3 como `operator=(X&)` y `m04 = seg` con `const X&` no compila
   (CharaSub_unk, TownTask, Lemon... 73 unidades). Ver 4b.
4b. Igual que 4 pero la sobrecarga no-const como plantilla `template<class T> operator=(T&)` (no es
   operator= de copia, el implicito de los structs sigue siendo const&). Compila todo; ninja completo:
   100% 5375 -> 5384, lost 0 (con `SetCenter_unk(Vector3&)` en LH_ColliObject), 3 bajadas minimas
   (EnvSeLine vf10 95.23->95.19, TownTask vf10 98.26->98.25, Lemon fn_80416268 85.74->85.66) PERO
   `main.dol: FAILED`: en game/CharaLook_unk (unidad enlazada) fn_80129A6C llama fuera de linea a
   `__as__Q23atn7Vector3FRCQ23atn7Vector3` para TODAS sus asignaciones, tanto desde un miembro no-const
   (`rot = mOffset`, 0x80129B5C) como desde temporales (0x80129CAC), y con la plantilla sale
   `__as<Q23atn7Vector3>__...FRQ23atn7Vector3_RQ23atn7Vector3`. => EL ORIGINAL TIENE UN SOLO
   operator=(const Vector3&). Las dos sobrecargas quedan DESCARTADAS definitivamente.
5. Helpers con `Vector3*` en vez de `float*` (`__VecCopy(this, &v)`): P1 OK pero P2/P5 se invierten
   (GameUI, ColliAttr fn_80184548). Equivalente a 1. NO vale.
6. Variantes de firma que NO cambian nada (banco sintetico, todas identicas a la base): sin `register`
   (no compila: "src was not assigned to a GPR register"), helper miembro con locales `register`,
   locales `register` declarados src-primero o dst-primero, asm directamente en los miembros con
   `this`/`&v` (= igual que 5), casts `(float*)this`, helper estatico con referencias, helper libre
   con referencias, operator= que devuelve void, operator= que toma `const V*` via `set(&v)` (= 5),
   direcciones en locales no-register `s`/`d` antes de llamar (`d` primero = base; `s` primero = 5),
   un tercer parametro dummy, helper que devuelve dst.
7. Orden de las 4 instrucciones del asm (`matrix.py`, 12 combinaciones ctor x operator= de
   LZST/LZTS/ZLST/ZLTS/LSZT/ZTLS): la base LZST/LZST es la mejor (banco: 1294 lineas distintas, 10 OK);
   la siguiente LZTS/LZST 1338/9; cualquier operator= distinto de LZST empeora mucho (1397..1853). Los
   registros de direccion NO dependen del orden de instrucciones del asm (banco sintetico identico
   con las 5 partidas).

### Hallazgos (sub-bloqueo B, orden de z)
- Inventario del DOL (`pscopies.py`): ZLST 1423, LZTS 1144, LZST 357, LSTZ 265, LSZT 140, LTZS 90.
- El scheduler de MWCC reordena libremente las 4 instrucciones del asm inline segun el contexto: con
  la partida LZST nuestra, `obj->m6C = desc->mPos` sale ZLST (WaitObject, MapLift, ... al 100%), y
  `pos = node->GetTrans_unk()` sale LZTS. Es decir, ZLST y LZTS NO son helpers distintos en general.
- Al quitar TODOS los `Assign_unk`/`__VecCopyZFirst` del repo (10 unidades): solo pierden
  CharaSub_unk ctor (copias de un global a 5 miembros seguidos), Event/MotionData fn_80204230 (8->104),
  LogMessageTask fn_8047961C y ControlShoot ctor (38->48); CharaTask SUBE 4 (vf64, fn_80139F60,
  fn_80153B44, fn_8016F67C) y mejora en otras 12; el resto igual. => `Assign_unk` sobra en la
  mayoria de sitios; donde hace falta de verdad el original carga z primero por una razon que el
  scheduler no reproduce desde LZST (CharaSub: 5 copias seguidas del mismo global).
- Funciones minusculas con LZTS en el original (CostumeEditChara fn_801E9168/fn_801E917C, TownTask
  fn_805B7B04, WallSlash vf14/vf18 (retorno por valor), PBoneOutForce::vf28): con partida LZTS salen al
  100% (matrix ctor=LZST assign=LZTS) pero esa partida rompe 15 funciones del banco. Las copias debiles
  fuera de linea del ctor (0x8001047C) y del operator= (0x8000D124) son LZST, luego `mTarget = pos`
  en CostumeEditChara NO es el mismo `operator=(const Vector3&)` del que sale la copia debil
  (hipotesis: otra sobrecarga / otro tipo de miembro). fn_801E917C ademas recarga psq_l para la segunda
  copia (sin CSE), cosa que nuestro inline no hace (CSE de las cargas).
- `__vec2x32float__` + `#pragma vec2x32float_align_4`: MWCC genera `psq_lx/psq_stx` (indexado) para
  copias de ese tipo, nunca `psq_l` con offset; no sirve (y seria asm "nuevo" en espiritu).

### Mas intentos (A)
8. Formas del temporal: ctor de 3 floats con asignaciones en el cuerpo / lista z-primero,
   `GetTrans_unk` construyendo un local con nombre, `GetTrans_unk` como funcion libre, operator=
   definido fuera de la clase con `inline`, operator= con `const Vector3& r = v` intermedio: ningun
   cambio en P1 (sintetico).
9. Contextos de `x = temp` (sintetico, header base): dst recibe el registro menor SOLO cuando dst es
   `Vector3 pos; pos = temp;` (declarado sin inicializar) o un miembro de `this`/puntero; cuando pos ya
   fue construido por otra expresion (`Vector3 pos = a + b; ... pos = temp;`, el caso real de
   ControlUseSkill) el src recibe el menor. PBoneBasic::vf0C real (`Vector3 root; root = ...`) sigue
   fallando por otra razon (dos copias seguidas + `tip - root`).
10. Quitar `Assign_unk` en los 6 sitios de CharaTask uno a uno: ninguno sube ni baja por si solo salvo
   las 3 copias directas `__VecCopyZFirst(&dir.x,&front.x)` que bajan 16->24. (El +4 observado con el
   reemplazo global era un artefacto: el sed rompio el fichero y cmp.py comparo un .o viejo.)
   ControlShoot `m6B8.Assign_unk(ofs)` -> `=`: pierde la Z-primero y ademas cruza r3/r4.
   => `Assign_unk` sigue haciendo falta donde esta.
11. Regla observada en el DOL para copias pila->pila con registros volatiles (stackcopies.py, 469
   sitios): 'dstLower' 342 vs 'srcLower' 127, sin correlacion limpia con el orden de los `addi`, el
   offset del slot o si el dst ya estaba escrito. En nuestro compilado el src recibe el registro
   menor casi siempre salvo cuando el dst es un `Vector3` declarado sin inicializar y asignado una
   vez. Es decir: el original "crea" primero la direccion del dst cuando el src es un temporal; la
   nuestra crea primero la del src. No se ha encontrado la forma de cabecera que lo reproduzca sin
   romper P2/P5.
12. `asm volatile` en el helper: desactiva el CSE de las cargas (fn_801E917C recarga psq_l como el
   original) pero el scheduler deja de reordenar: `obj->m6C = desc->mPos` sale LZST en vez de ZLST y
   la copia desde temporal (P1) pierde el forwarding de z (LST). Rompe los 100% existentes. NO vale.
   `register volatile float*` en los parametros: igual que volatile.
13. Builds completos con otras partidas (fullvar.sh): ctor LZST + operator= ZLST: lost 66, 139
   bajadas; ZLST/ZLST: lost 120; ZLST/LZST: lost 61. La partida LZST/LZST actual es con diferencia la
   mejor. Cualquier cambio del orden de instrucciones de `__VecCopy` queda DESCARTADO.
14. P2 con la fuente global `const`/no-const/`static const` de clase: identico (sintetico).
15. Formas que arreglan P1 pero rompen P2 (= equivalentes a la variante 1): src pasado por un inline
   identidad `idp(&v.x)`, `const float* s = &v.x` antes de la llamada, helper `(Vr&, const float*)`,
   helper `(Vector3*, const Vector3*)`, asm en el cuerpo con `register` locales. Formas que no cambian
   nada: `float* d = &x` antes, `idq(&x)`, helper `(float*, const V&)`.

## Resumen de lo que NO funciona (no repetir)
- Cambiar el orden de parametros del helper (src primero) en ctor+operator= o solo en operator=
  (+9 pero -2 perdidas GameUI/LH_ColliObject y 4 bajadas).
- Dos sobrecargas de operator= (no-const / const): el original solo tiene `operator=(const Vector3&)`
  (CharaLook_unk::fn_80129A6C llama `__as__Q23atn7Vector3FRCQ23atn7Vector3` para lvalues no-const).
- Helpers con `Vector3*`/`void*`/referencias en vez de `float*`; casts; locales `register`; asm
  directamente en los miembros; `volatile`; `asm volatile`; helper que devuelve dst; parametro dummy.
- Cualquier orden de las 4 instrucciones distinto de LZST en ctor u operator= (matrix.py + fullvar).
- `__vec2x32float__`/`#pragma vec2x32float_align_4`: genera psq_lx/psq_stx, no psq_l.
- Quitar `Assign_unk` donde esta hoy (CharaSub, CharaTask, ControlShoot, MotionData, LogMessageTask):
  pierde o no cambia.

## Patron documentado para la proxima sesion (sub-bloqueo A, "P1")
Original 0x801C982C (ControlUseSkill::vf0C, `pos = ((Matrix3x4*)mNode)->GetTrans_unk()` con `pos`
ya construido en 0x94 y el temporal en 0x58):
```
lfs f0, 0x1c(r5) ; addi r4, r1, 0x58 (src) ; lfs f3, 0xc(r5) ; addi r3, r1, 0x94 (dst) ; lfs f2, 0x2c(r5)
stfs f3, 0x58(r1) ; stfs f0, 0x5c(r1) ; psq_l f1, 0(r4) ; stfs f2, 0x60(r1) ; psq_st f1, 0(r3) ; stfs f2, 0x9c(r1)
```
Nuestro: identico salvo `addi r3 <- 0x58` / `addi r4 <- 0x94` (src en el registro menor). Mismo
patron en PBoneBasic vf0C/vf14 (`root = GetTrans()`, r5/r4), ColliAttrManager fn_80184864 (`from =
NodePos + ofs`), ChaserGuard fn_8017FBEC, SkeltonKing vf20, BigGolem vf20, Cannonball fn_803FBC34,
CharaTask fn_80174104. En cambio con src con nombre (local/global/miembro) el original pone el src en
el registro menor (GameUI fn_803CF74C, LH_ColliObject fn_80472424, ColliAttr fn_80184548 ctor) y ahi
nuestro header ya coincide. Hallazgo MWCC (una linea): con `operator=(const Vector3&)` + helper
`(register float* dst, register const float* src)`, MWCC materializa la direccion del src antes que la
del dst siempre; el original la materializa despues cuando el src es un temporal (rvalue) y antes
cuando es un lvalue con nombre, con UNA sola sobrecarga. Queda por encontrar la forma de cabecera que
lo reproduzca (no es ninguna de las ~25 de este fichero).

## Tabla final
| | antes | despues |
|---|---|---|
| funciones al 100% (report.json) | 5375 | 5375 (sin cambios commiteados) |

Commits: solo las notas (`docs/mwcc-blockers-q3.md`). Ningun cambio de `include/math/Vec.h` ni de
fuentes cumple la condicion 8 (sube sin bajar nada): la mejor variante (3: operator= src-first) da
+9 al 100% con 2 perdidas y 4 bajadas; la 4b (+9, 0 perdidas, 3 bajadas) hace fallar el DOL por el
nombre de la copia debil del operator= y esta descartada por CharaLook_unk.

## Decisiones tomadas sin el usuario
- No commitear ninguna variante de cabecera (condicion 8).
- Dejar `Assign_unk`/`__VecCopyZFirst` donde estan.
