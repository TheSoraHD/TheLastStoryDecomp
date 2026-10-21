# batch-auto-20kb-atn qA1: atn_colli + ColliCalc_unk

Cola: `atn/atn_colli` (0x80056DB8), `atn/ColliCalc_unk` (0x80059784). Al partir ColliCalc_unk sale una
unidad nueva `atn/Csv_unk` (0x8005B3CC).

| Unidad | 100% | matching |
|---|---|---|
| `atn/atn_colli` | 44/51 | false |
| `atn/ColliCalc_unk` | 12/15 | false |
| `atn/Csv_unk` (nueva) | 10/15 | false |

Total del lote en esta cola: +66 funciones al 100% (5961 -> 6027 en el report), `main.dol: OK`, `lost: 0`
contra main y la integracion, sin bajadas de fuzzy.

## atn/atn_colli (commits b7060b6, 0d745e8)
- Resultado: 44/51 al 100%, `matching: false`.
- Contenido: ctor/dtor de `atn::ColliBase`, `fn_80056EB0` (rango de celdas, lo usa ColliBaseFunc_unk), las
  virtuales vf0C-vf1C de ColliSphere/ColliCapsule/ColliBox/ColliObject/ColliTree, el montaje de la caja
  (`ColliBox::fn_800575BC` matriz + `fn_800577D8` 12 triangulos + `fn_80057A78` triangulo), carga de
  ColliObject/ColliTree, dibujo de depuracion (`fn_800584B4/fn_800584D8`, `fn_80059240/fn_80059264`, GX
  directo), `ColliObject::fn_800588FC` (esfera envolvente en mundo), `ColliTree::fn_80058EE4` (caja global
  de los arboles) y los destructores debiles al final.
- NONMATCHING:
  - `ColliCapsule::vf0C` 99.77%, `vf10` 99.78%: y/z de `mEnd - mStart` (SetUpright_unk) con registros cruzados.
  - `ColliCapsule::vf1C` 84.74%: orden de carga del 0.5f y de los stores muertos (planificacion).
  - `ColliBox::fn_800575BC` 98.16%: los tres bloques de rotacion salen en la pila en orden inverso (z abajo en
    el nuestro, arriba en el original) y el 0.0f se carga despues del angulo en y/x.
  - `ColliBox::vf1C` 99.08%: f9/f10 (tamano de celda / inverso) cruzados.
  - `ColliObject::fn_800588FC` 99.83%: orden de stores de la escala (el original guarda y antes que x).
  - `ColliTree::fn_80059264` 99.78%: orden de los dos incrementos del bucle (tris antes que i).
- **Decisiones sin el usuario**
  - Splits: `atn_colli` acaba en 0x80059660 (antes 0x80059784): `fn_80059660` va detras de los destructores
    debiles (`__dt__ColliObject`/`__dt__ColliTree`), asi que es del fichero siguiente (ColliCalc_unk, que la
    llama).
  - `configure.py`: quitada la linea `Object(NonMatching, "atn/atn_colli.cpp")` de la lib `game` (duplicaba la
    entrada nueva de `game_units.json`, que es la que manda como en el resto de unidades).
  - `symbols.txt`: pairnames (36 nombres: vf0C..vf1C de las 5 clases, `__dt__` de Sphere/Capsule/Box,
    `fn_800575BC__Q23atn8ColliBox...`, `fn_800577D8__Q23atn8ColliBoxFv`, `fn_80057A78__...`, `fn_80056EB0__...`,
    `fn_800584B4/584D8/59240/59264/58EE4__...`); `fn_800584D8`/`fn_80059264` renombradas a mano tras tipar el
    nodo (`P16ColliObjNode_unk`, `P17ColliTreeNode_unk`).
  - Cabecera `include/atn/ColliBase.h` reescrita (para ColliBaseFunc_unk/AudioDevice):
    - Orden de clases del fichero: Base, Sphere, Capsule, Box, Object, Tree (vtables al reves en .data).
    - Firmas de las virtuales: `vf0C(ColliResult_unk*, int max, const ColliSphereShape_unk*, unsigned mask,
      int attr)`, vf10 igual con `ColliCapsuleShape_unk`, vf14 con `const void*` (barrido: misma forma que la
      capsula), `vf18(int, int, int, int)` (dibujo), `vf1C(const ColliGrid_unk*)` (celda m10-m18). Las de
      ColliBase estan en ColliBaseFunc_unk (devuelven 0 / vacias): solo declaradas.
    - Sin destructores declarados en las subclases (implicitos): con `virtual ~X() {}` los debiles salen en
      orden inverso (Tree..Sphere); implicitos salen Sphere, Capsule, Box, Object, Tree como el original.
    - `ColliSphere::Sphere_unk` es ahora `typedef ColliSphereShape_unk` (mismos ctors).
    - `ColliBox::m3C` pasa de `unsigned char[]` a `ColliBoxShape_unk` (tamano + matriz) y
      `ColliTriangle_unk mTriangles[12]` en 0x78. `ColliTriangle_unk` es POD (floats + accesores Vector3&):
      con miembros Vector3 el ctor inline de ColliBox genera un bucle de construccion que el original no tiene
      (rompia `gmk::WaterLevel` y el .dol).
    - `ColliObject`: `ColliSphereShape_unk mBound` en 0x78 (antes `m78[0x10]`). `ColliTree::m48` pasa a
      `Matrix3x4 mMtx`.
    - Tipos nuevos: `ColliResult_unk` (0x50; otros ficheros tienen su `CollResult_unk`), `ColliGrid_unk`,
      `ColliTreeBounds_unk`, `ColliCapsuleShape_unk::SetUpright_unk`, `ColliBase::SetCell_unk` (inline),
      `ColliBox::GetBound_unk`, `Transform_unk`.
  - Nueva cabecera `include/atn/ColliData_unk.h`: formato de los datos cargados (nodos de ColliObject con
    transformaciones y listas de triangulos de 0x44, octree de ColliTree, atributos id/flags/color).
  - Llamadores actualizados (antes `extern "C"`): `gmk/FloatingBox` y `gmk/WaterLevel` llaman a
    `mColli.fn_800575BC(...)`, `gmk/OnOff` a `colli->fn_800577D8()`; `FloatingBox` pasa `&mColli.m3C`;
    `game/EnemyFieldMagician` hace cast a `ColliResult_unk*`/`ColliCapsuleShape_unk*` en `vf10`. Mismo codigo.
  - `fn_8005AAD4/AD74/B05C` reciben tambien mask/attr (ColliTree::vf0C-vf14 los dejan en r7/r8).
- Hallazgos MWCC
  - `virtual ~X() {}` en la clase: los dtors debiles salen en el orden de las vtables (inverso a la declaracion);
    sin declarar (implicitos) salen en orden de declaracion.
  - Un array miembro de un tipo con ctor vacio inline (`Vector3`) genera bucle de construccion en el ctor
    inline del contenedor: para un array sin bucle en el original, el tipo del elemento no tiene ctor.
  - `a + b` con el operador miembro de Vector3 evalua `a` antes que `b` (fn_80058EE4); un conversor
    miembro a miembro (`r.x = v.x; ...`) carga x primero, `Vector3(x, y, z)` carga z primero.
  - `Set_unk(f(x), f(y), f(z))` (args de derecha a izquierda) da el orden z, y, x de los calculos sin
    temporal en la pila (centro del triangulo de fn_80057A78).
  - Una copia de una estructura con Vector3 por copia implicita (`T c(*(const T*)&m3C)`) carga en el orden de
    los miembros; el ctor `T(a, b, r)` adelanta la carga del float.
  - Locales de una inline que retorna por valor (`GetBound_unk`) se colocan por debajo de los de la inline
    llamada despues (SetCell_unk): sirve para invertir el orden de temporales en la pila.

## atn/ColliCalc_unk (commit 0d745e8)
- Resultado: 12/15 al 100%, `matching: false`.
- Contenido: hojas por lista de triangulos (`fn_80059660` esfera, `fn_80059784` capsula con triangulo estatico
  transformado, `fn_80059A40` barrido), recorridos del arbol de nodos de ColliObject (`fn_80059BAC`,
  `fn_8005A0D4`, `fn_8005A5FC`: tres copias identicas, con sus entradas `fn_80059B5C/8005A084/8005A5AC`) y del
  octree de ColliTree (`fn_8005AADC`, `fn_8005AD7C`, `fn_8005B064` con entradas de 8 bytes).
- NONMATCHING:
  - `fn_80059784` 97.40%: numeracion de GPR (puntero al triangulo fuente y atributo en r24, estatico en r18).
  - `fn_8005AADC` 99.86% y `fn_8005AD7C` 99.88%: registros/orden de carga en las pruebas de rango en z.
- **Decisiones sin el usuario**
  - Splits: empieza en 0x80059660 (ver atn_colli) y acaba en 0x8005B3CC; el resto pasa a `atn/Csv_unk`.
  - `symbols.txt`: `lbl_807C6BC8` -> `@LOCAL@fn_80059784__...@tri` y `lbl_8087EEA0` -> `@GUARD@...@tri` (estatico
    local de 0x40 con guarda, tipo con miembros Vector3).
  - Las funciones quedan `extern "C"` (las llaman las virtuales de atn_colli; sin clase conocida).
  - Hallazgo: la prueba de cada lista (esfera envolvente en mundo + llamada a la hoja) es una inline aparte
    (`__TestSphereList_unk`...): sin ella `c`/`bound` quedan por encima de los vectores de transformacion y
    `max - n` no se calcula antes de PSMTXMultVec. Los ayudantes de matriz son los de `atn/LH_ColliObject.cpp`.
  - Filtro de atributo escrito en linea (`(mask & a->mFlags) == 0 && (attr < 0 || attr == a->mId)`): como
    inline bool sale `li r3,1/cmpwi`.

## atn/Csv_unk (nueva, commit 0d745e8)
- Resultado: 10/15 al 100%, `matching: false`.
- Contenido: miembros fuera de linea de `atn::CsvBase` (`fn_8005B9CC` leer campo, `fn_8005BBF8` saltar linea,
  `fn_8005BCE8` separadores) instanciados para coma/tab (char y wchar_t) y pipe (solo fn_8005B9CC y
  fn_8005BCE8), mas `fn_8005C220` (prueba de bit, la usa atn/PacketPack; sin datos para ubicarla).
- NONMATCHING: los cinco `fn_8005B9CC` 99.68% (char) / 98.93% (wchar_t): numeracion de registros en el recorte
  de separadores iniciales.
- **Decisiones sin el usuario**
  - Unidad nueva `atn/Csv_unk.cpp` 0x8005B3CC-0x8005C248 (solo .text, sin datos): ninguna funcion de
    ColliCalc_unk usa CsvBase, asi que no son copias debiles de ese fichero. Nombre real probable `atn_csv.cpp`
    (entre atn_colli y atn_curve en el orden alfabetico).
  - `symbols.txt`: `fn_8005C1F8` -> `fn_8005BCE8__Q23atn20CsvBase<c,124,13,34>FPCc` (pairnames).
  - Ayudante `__IsLeadByte_unk` (byte inicial Shift-JIS 0x80-0x9F) sobrecargado para wchar_t (siempre falso):
    explica que la version wchar_t sea 0x44 bytes mas corta.
- Hallazgos MWCC
  - Leer otra vez `m04[pos]` en vez de usar el local `c` (lbz sin extsb, CSE) da el registro del original.
  - Orden de declaracion de los locales enteros = orden de asignacion r0, r3, r4, r5 en funciones hoja.

## Pendientes
- atn_colli/ColliCalc/Csv: los NONMATCHING de arriba (todos de registros/planificacion salvo capsula vf1C).
- `fn_8005C220` probablemente es de otro fichero (prueba de bit sobre palabras en +8).
- Revisar si `CollResult_unk` de gmk/game deberia pasar a `atn::ColliResult_unk` (no tocado).
