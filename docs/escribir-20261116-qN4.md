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

## Bloqueos
- ControlGrappled::vf10 (99,89): (1) Mtx del segundo bloque por debajo del bind_t del primero (probado else if, else anidado, bloque interior, `boost::function` con nombre, Mtx declarada antes del dir, RotY_unk: arregla la pila pero la llama fuera de linea); (2) la base r31 de los ptmf es `...data.0` en el nuestro y `@13439` en el original porque el original emite primero en `.data` el `@STRING@allocate` del bind_t de 4 argumentos (198); el nuestro lo emite el tercero de los cuatro al final.

## Cambios de splits/symbols
- symbols.txt: re-emparejado de las etiquetas de chr/ControlJumpAt (pd.sh + pdfix.py 0x80780BA0/BB8/BC4/BD0).
