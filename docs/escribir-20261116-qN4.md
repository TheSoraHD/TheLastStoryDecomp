# escribir-20261116 cola N4 (unidades a dos funciones de enlazar)

Inicio: 2026-11-17 06:25 CET. Tope 08:55.

## Tabla de la cola

| unidad / funcion | antes | despues |
|---|---|---|
| chr/ControlJumpAt ControlGrappleThrown::vf0C | 99,82 | 100 |
| chr/ControlJumpAt ControlJumpAt::vf10 | 99,93 (etiqueta) | 100 |
| chr/ControlJumpAt ControlGrapple::vf10 | 99,96 (etiqueta) | 100 |
| chr/ControlJumpAt ControlGrappled::vf10 | 99,44 | 99,89 (NONMATCHING) |
| chr/ControlChant ControlMagic::vf0C | 93,6 | 95,0 (NONMATCHING; if/switch del original) |
| chr/ControlCliffhang fn_801AB9E4 | 96,4 | 100 |
| chr/ControlPushedDown ControlPushedDown::vf0C | 99,29 | 99,99 (NONMATCHING: f0/f3 de un fabs) |
| chr/ControlPushedDown ControlPushedAvoid::vf0C | 99,28 | 99,39 |
| game/NewCampPartsTop vf14 | 99,72 | 99,82 |
| chr/ControlCliffhang vf0C | 99,69 | 99,83 |
| game/Em001Necromancer ctor | 99,03 (comentario `draft` erroneo) | 99,03 (comentario corregido) |

## Para el port
- Sin cambios de comportamiento. ControlJumpAt: inline nueva `HitPos_unk(const atn::ColliResult_unk&)` (copia por valor de m10).

## Decisiones tomadas sin el usuario
- (ninguna)

## Hallazgos
- Copia `Vector3 pos = result.m10` con dst/src en r3/r4 cruzados: copiar a traves de una inline que devuelve el Vector3 por valor (`HitPos_unk(result)`) da los registros del original (ControlGrappleThrown::vf0C). (escribir-20261116 qN4)
- Cada inline nueva en un .cpp renumera los @N de toda la unidad aunque se defina al final; pd.sh no renombra los ptmf de `.data` direccionados desde una base `...data.0`: renombrarlos por direccion con pdfix.py. (escribir-20261116 qN4)
- Huecos de pila de una Mtx en un bloque: con la matriz como temporal (retorno de `Matrix3x4::RotY_unk`) el orden de pila sale como el original (la Mtx con nombre se coloca antes de los temporales bind_t); falta una forma sin llamada fuera de linea (ControlGrappled::vf10). (escribir-20261116 qN4)

- Orden de pila: MWCC da los huecos altos a todos los locales con nombre (en orden de declaracion, sea cual sea el bloque) y despues a los temporales en orden de aparicion (resultado antes que operandos, `a + b*k`: suma 0x80, producto 0x74). Si en el original un vector va entre temporales, era un temporal: `Dot_unk(up, (b - a).GetNormalize())`, `&atn::Vector3(0,1,0)` como argumento, `Cross_unk(a, b).y`, `&(p + side * 5.0f)` (ControlCliffhang fn_801AB9E4 96,4 -> 100; ControlGrappled::vf10 con RotY_unk). (escribir-20261116 qN4)
- Comparar `b.y > a.y` y despues `(b - a)`: con `const Vector3& pa = a->mPos` el original reaprovecha las y ya cargadas (resta y primero); con `a->mPos` directo se recargan. (escribir-20261116 qN4)
- Calculo de dos punteros `&tabla[link->mA]`, `&tabla[link->mB]`: el orden de los registros del original salio escribiendo primero `b` y despues `a`. (escribir-20261116 qN4)
- Un `switch` de pocos casos que el original compila como cadena `cmpwi 2; cmpwi 1; cmpwi 6; cmpwi 9; cmpwi 8` era una cadena de `if (t == 2 || t == 1 ...) else if (t == 8)`; un rango `subi; cmplwi 3` probado antes de `== 0x179A` era un `switch` con `case 0x179A` y `case 0x179B..0x179E` (ControlMagic::vf0C). (escribir-20261116 qN4)

- FPR salvados de constantes y frame (ControlPushedDown::vf0C): declarar arriba `float frame; float zero = 0.0f; float one = 1.0f;` (frame sin inicializar, asignado en el case) da f30/f31/f29 del original; `perm.py`-style sobre las 6 permutaciones. `atn::Abs_unk(a)` en vez de `(float)__fabs(a)` cambia el registro del fabs. (escribir-20261116 qN4)

- Constantes 0.0f/1.0f sacadas de un bucle (NewCampPartsTop vf14): `float zero; float one = 1.0f; zero = 0.0f;` antes del bucle da el orden de carga y los FPR del original (declarar `zero` primero fija el registro, asignarlo despues fija el orden de las cargas). (escribir-20261116 qN4)

- Copias `chara->mPos = pos` con dst/src cruzados (r3/r4/r5): escribir por el accesor debil `*mChara->fn_8013C38C() = pos` (inline en el .cpp) da los registros del original (ControlPushedDown estado 3, ControlCliffhang estado 0). (escribir-20261116 qN4)

## Bloqueos
- ControlGrappled::vf10 (99,89): (1) Mtx del segundo bloque por debajo del bind_t del primero (probado else if, else anidado, bloque interior, `boost::function` con nombre, Mtx declarada antes del dir, RotY_unk: arregla la pila pero la llama fuera de linea); (2) la base r31 de los ptmf es `...data.0` en el nuestro y `@13439` en el original porque el original emite primero en `.data` el `@STRING@allocate` del bind_t de 4 argumentos (198); el nuestro lo emite el tercero de los cuatro al final.

- NewCampPartsTop ctor (98,88): el bucle de las 8 entradas (entrada/sInfo/desplazamiento) y la copia de `mInfo` (r0, r3, r0 en el original): referencia, sin local, copia miembro a miembro, `info` puntero: igual o peor. vf14: texto/layout de `select00_text` (local `text`, `text`+`layout`, llamada directa) y el f29 de `enableLoad` (local con nombre arriba): igual o peor.
- Cliffhang vf0C (99,83): FPR de `speed` del estado 1 (f30 en el original) con `scale`/`speed` declarados arriba: igual.
- Cliffhang vf0C (99,69): `r` como temporal, `dir` en inline por valor (95), `Transform_unk` por referencia (96), `speed` en una expresion (95): peor. El original tiene `dir` (0x38) por debajo de los temporales de `dir * 4 * speed`.
- OnOff fn_803F8324: `__VecCopy`/`__Vec2Copy`+z para `off`, referencia de la matriz: igual o peor (99,2).
- ControlMagic::vf0C: copia de `off` en SpreadTarget_unk (local con nombre + copia da la forma, pero mueve las sumas del angulo; 94,9) y la direccion de la Mtx en r15 (Matrix3x4 miembro, puntero, MulVec_unk): sin cambio.

## Cambios de splits/symbols
- symbols.txt: re-emparejado de las etiquetas de chr/ControlJumpAt (pd.sh + pdfix.py 0x80780BA0/BB8/BC4/BD0).
