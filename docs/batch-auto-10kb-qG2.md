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

## game/EnemyTrista (commit b086b63)
- Resultado: 12/18 al 100%, `matching: false`, `main.dol: OK` (regress lost: 0).
- NONMATCHING:
  - ctor 98.6%: el original carga info->m20 antes de guardar los registros.
  - vf10 98.8%: registros puntero/contador del bucle de eventos (StartEvent_unk).
  - fn_8035C8CC 82.1%: huecos de pila de los temporales del salto lateral (Side_unk); codigo igual.
  - fn_8035CF78 98.0%: hueco del test del scoped_ptr (copia del ptmf) y orden de argumentos.
  - fn_8035D294 60.9%: copia miembro a miembro de los parametros de camara por defecto (el original
    los carga en FPR salvados antes del test de modo).
  - vf3C 95.0%: el original conserva r5 (segundo argumento de vf3C, que CharaTask::vf3C no usa) y lo
    pasa a CharaTask::vf3C. Cambiar la firma de vf3C afecta a toda la jerarquia: no hecho.
- Decisiones sin el usuario:
  - Split sin cambios (ya ampliado en splits-auto-C).
  - CABECERA COMPARTIDA include/game/CharaTask.h + src/game/CharaTask.cpp + symbols.txt:
    `fn_8015783C(HitInfo_unk*, const Vector3&)` -> `fn_8015783C(HitInfo_unk*, const Vector3&, float dist)`
    (los 8 llamadores del DOL pasan f1; la funcion no lo lee; sigue al 99.74%). Mangled:
    fn_8015783C__9CharaTaskFP11HitInfo_unkRCQ23atn7Vector3f.
  - CABECERA COMPARTIDA include/game/CharaTask.h + symbols.txt: `virtual void vfA8()` ->
    `virtual int vfA8(int kind)` (CharaTask::vfA8 en 0x80041A20 devuelve -1; EnemyTrista la
    sobrescribe con un int). vfA8__9CharaTaskFv -> vfA8__9CharaTaskFi (76 vtables en asm, se
    regeneran solas). Nadie en src la llamaba.
  - CABECERA COMPARTIDA include/game/HitInfo_unk.h: `float m40` (alcance) partiendo m1C[].
  - Cabecera nueva include/game/EnemyTrista.h (TristaEvent_unk inventado). Clase local
    ControlShingan en namespace anonimo; sus simbolos en symbols.txt usan
    `@unnamed@EnemyTrista_cpp@` (nombre de nuestro fichero) y lbl_80789B10/80 -> su vtable y RTTI.
  - NOMBRE REAL del fichero: enemy_trista.cpp (el RTTI dice `@unnamed@enemy_trista_cpp@::ControlShingan`).
    Pendiente: renombrar la unidad (afecta a splits/game_units) para que el nombre RTTI y los
    simbolos de ControlShingan casen; no hecho.
  - Inlines inventados en el .cpp: ResetCamera_unk, FireEvent_unk, StartEvent_unk, ClampHp_unk,
    HasHit_unk, Side_unk, FrontDir_unk, PlayMotion_unk, PlayEffect_unk; structs Field_unk/
    FieldCamera_unk/FieldLook_unk/TristaCamParam_unk (campos de lbl_8087F430 0x4D8-0x9A0).
  - fn_800F8C6C devuelve int (balas lanzadas, se resta de m594).
- Hallazgos MWCC:
  - Un bloque con orden de pila ascendente dentro de una funcion = inline propio (FireEvent_unk).
  - `float scale = g->m3A4; m -= scale;` da el orden de registros del original para `m -= g->m3A4`.
  - Un global cargado antes de una llamada que calcula otro argumento (`fn_8004ED34(lbl_8087EE98, ...,
    fn_80179D44() | 0x80000000, ...)`) = local `void* world = lbl_8087EE98;` (anade un GPR salvado).
  - El vector diferencia con nombre (`Vector3 d = a - b; d.Length_unk()`) en vez de temporal sube su
    hueco por encima de los temporales.
  - El dtor implicito de la clase y el de la clase local salen al final del .o en ese orden.

## game/SpecialEnemyBase (commit 25f764f)
- Resultado: 15/22 al 100%, `matching: false`, `main.dol: OK` (regress lost: 0). Unidad nueva en
  game_units.json (antes no tenia entrada).
- NONMATCHING:
  - fn_805A3D68 99.9%: registro del literal ".csv".
  - fn_805A45F8 98.2%: registros/orden de cargas de los dos bucles de personajes.
  - fn_805A4A54 66.3%: aproximacion (test de punteria de la camara con rectangulo en pantalla).
  - fn_805A5078 98.1%: orden de cargas (copia z-primero de `dir`).
  - fn_805A5220 99.6%: solo offsets del pool (el original tiene "" y "basic_string: out_of_range"
    antes de "position").
  - fn_805A52F4 99.4%: datos de la tabla de nombres (entrada "").
  - fn_805A5374 73.3% (fabrica de enemigos): registros salvados y el "" del new (el original usa un
    objeto aparte de 4 bytes tras la tabla, lbl_807637DC).
- Decisiones sin el usuario:
  - Split sin cambios.
  - game_units.json: entrada nueva game/SpecialEnemyBase.cpp (matching false).
  - symbols.txt: nombres de las funciones (pairnames + fn_805A45F8/fn_805A4A54 a mano) y
    lbl_807632A0 -> @stringBase0. fn_805A5078, fn_805A52F4 y fn_805A5374 quedan como extern "C"
    (los llamadores en src ya usan esos nombres: EnemyTask.cpp, db/CharaIdTable_unk.cpp).
  - CABECERA COMPARTIDA include/game/SpecialEnemyBase.h: miembros nuevos declarados
    (fn_805A4188, fn_805A45F8, fn_805A4980/49E4/4A1C con SpecialFlag_unk, fn_805A4A54 con
    SpecialAim_unk, fn_805A5220); nombres de parametros de fn_805A4F1C corregidos (speed, blend;
    misma firma). Ver tambien los cambios de vf110/vf114/vf118 en la seccion de Em305VampireMp.
  - include/game/Em201ZanGurg.h: `int m170C` (la fabrica reserva 0x1710 bytes).
  - La fabrica llama a los constructores no decompilados como `extern "C" void* fn_XXXXXXXX(void* self,
    atn::Task* parent, EnemyInfo_unk* info | void* data)` tras `::operator new(size, 3, "", "", 0)`;
    los decompilados con `new (3, "", "", 0) Clase(...)`. Los tamanos de clase coinciden con los
    del original (comprobado al compilar los new de las clases existentes).
  - Inventados: GimmickObj_unk/GimmickList_unk (lbl_8087F4A0), Field_unk (m74/m80/m13C/m868),
    SpecialFlag_unk, SpecialAim_unk, SpecialRect_unk.
- Hallazgos MWCC:
  - El vtable de SpecialEnemyBase mide 0x11C (el 0 siguiente es relleno antes de la jumptable);
    vf11C la introduce Em305Vampire.
  - Un `if (p != 0) { p = ctor(p, ...); } enemy = p;` reproduce el patron de `new T(...)` con
    constructores llamados como funciones.
- Pendientes para otras colas: Em305Vampire (sin decompilar) tiene su .rodata partida: su pool
  0x80747F70 cae fuera de su split (0x80747F60-0x80747F70).
