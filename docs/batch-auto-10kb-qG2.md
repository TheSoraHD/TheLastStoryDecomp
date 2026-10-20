# batch-auto-10kb qG2: enemigos

Cola: `game/Em201ZanGurg`, `game/Em305VampireMp`, `game/EnemyTrista`, `game/SpecialEnemyBase`.

## game/Em201ZanGurg (commit 8186734)
- Resultado: 11/15 al 100%, `matching: false`, `main.dol: OK` (regress lost: 0).
- NONMATCHING:
  - ctor 99.8%: registros del segundo constructor inline de std::string (literal/this r27/r26
    intercambiados) y del strlen de los parametros.
  - vf0C 98.0%: r3/r4 intercambiados entre la palabra de flags mStatus.m18 y la direccion del pool.
  - vf3C 99.7%: en el ToRotation_unk inline el original guarda la z redondeada en f3 para atan2.
  - fn_802640FC 99.9%: f0/f3 de mRot.y y rot.y en AngleDiffDeg_unk (el mismo inline casa en vf10).
- Decisiones sin el usuario:
  - Split sin cambios (ya cubria el fichero real, ver splits-auto-B.md).
  - symbols.txt: nombres de las 15 funciones (pairnames + 4 a mano: vf0C, vf10, fn_80264024,
    fn_80264C88) y lbl_807443C0 -> @stringBase0 (scope:local).
  - Cabecera nueva include/game/Em201ZanGurg.h (miembros 0x14B0-0x170C; nombres inventados con
    inferred). Helpers inline inventados en el .cpp: StartCounter_unk (vf3C), StartKnockback_unk
    (vf40), SelectAttack_unk (vf10), AngleDiffDeg_unk, MaterialEdit_unk, PlayMotion_unk,
    PlayEffect_unk, GetNodePos_unk.
  - Funcion reconstruida fn_DebugMenu_unk (no existe en el .dol): el pool tiene tras "rightwepoint"
    las cadenas "em305", "DebugDisp", "WaitTime", "AttackFrequcy" de una funcion eliminada por el
    enlazador (menu de depuracion con el ResHandle m1704 y m1708).
  - fn_8006AFF8 se llama como `extern "C" std::string fn_8006AFF8(const std::string&, const std::string&)`
    (concatenacion, copia debil de otro fichero).
  - Nombre real del fichero propuesto: Em201ZanGurg.cpp (sin pistas en contra).
- Hallazgos MWCC:
  - Bloques de un metodo con el orden de pila invertido (locales ascendentes) = helper inline del
    original: en vf3C/vf40/vf10 el codigo de cada estado era un inline propio.
  - Un temporal pasado por referencia a un inline (`f(Vector3(-x, -y, -z))`) se coloca tras los
    locales pequenos del inline y antes de los grandes (Mtx).
  - `name = s + 8` (operator=(const char*)) y no `assign(s + 8)` da el orden de las copias del tag de
    iterador del original.
  - Un local copiado de un miembro (`Vector3 pos = m14B8->mPos; d = pos - mPos;`) mas una sentencia
    saca una funcion del autoinline; un `int state = m55C; switch (state)` basta para otra.
  - Comparacion `frame >= f()` con la carga del miembro antes de la llamada (en registro salvado) =
    local `float frame = ...` antes del if.
  - Un local float declarado antes que otros (aunque se cargue tarde) recibe el FPR mas alto.
  - `int id = m590;` antes de una llamada fija el orden de carga miembro/global de los argumentos.
- Pendientes: ninguno para otras unidades.

## game/Em305VampireMp (commit 7a8c060)
- Resultado: 12/14 al 100%, `matching: false`, `main.dol: OK` (regress lost: 0).
- NONMATCHING:
  - ctor 96.7%: orden de carga de las palabras de flags y de los tamanos de los vectores, y registros
    del segundo std::string (mismo caso que Em201ZanGurg).
  - vf0C 94.0%: los push_back inline de std::vector (bloqueo conocido: la via rapida del elemento de
    16 bytes no lleva la comprobacion de null del placement new) y registros.
- Decisiones sin el usuario:
  - Split sin cambios. Pendiente: los 0x20 bytes .data 0x80787548-0x80787568 ("Memory allocation
    failure" del __allocate inline de std::vector<VampireDoor_unk>) estan en un hueco entre
    Em305Vampire y Em305VampireMp; pertenecen a esta unidad (ampliar .data a 0x80787548 cuando se
    enlace).
  - Cabecera nueva include/game/Em305Vampire.h (clase base sin decompilar: miembros 0x14D4-0x1608
    deducidos de su ctor y de los usos en Mp; VampireDoor_unk inventado) e include/game/Em305VampireMp.h.
  - symbols.txt (unidad Em305Vampire, ajena, solo nombres): fn_802E8580 -> ctor, fn_802E89B0 -> dtor,
    fn_802EA27C -> vf20, fn_802EB9F8 -> vfEC, fn_802EA994 -> vf11C y los 15 metodos no virtuales que
    llama Mp -> fn_XXXXXXXX__12Em305VampireFv (todos void(), comprobado que no leen r4).
  - CABECERA COMPARTIDA include/game/SpecialEnemyBase.h: struct SpecialState_unk nuevo; firmas
    vf110(SpecialState_unk*), vf114(const SpecialState_unk*), vf118(HitInfo_unk*) (el asm de la base
    las usa: guarda/restaura m58C/m14BC; vf118 lee r4). symbols.txt: renombrados sus manglings
    (vf110__16SpecialEnemyBaseFP16SpecialState_unk, vf114__...FPC16SpecialState_unk,
    vf118__...FP11HitInfo_unk). Em006DarkMaster sigue al 100% y main.dol OK.
  - fn_803E6850 recibe 5 argumentos (arena, chara, restantes, total, 0).
- Hallazgos MWCC:
  - Dos colores locales en ambitos anidados: el declarado en el ambito interior va mas abajo; para el
    orden del original declarar ambos al principio del bloque exterior (constructor vacio + campos).
  - Un `bool r = true; if (a == 0 || b == 0) r = false;` inline da el patron li 1 / li 0 del original;
    `return a != 0 && b != 0` invierte las ramas.
  - `Vector3 d = pos - mPos; d.y = 0.0f;` elimina la resta de y y guarda x, z, y en ese orden.
  - `mRot.y = dir.ToRotation_unk().y;` (temporal) coloca la copia por debajo de los locales con nombre.
  - `m++; if (m >= n)` en lugar de `if (++m >= n)` cambia el orden de carga de los operandos.
