# escribir-20261116 cola N6 (cerca de enlazar: atn, chr y game no repartidas)

Inicio: 2026-11-17 08:18 CET. Tope 10:48.

## Tabla de la cola

| unidad | funcion | antes | despues |
|---|---|---|---|
| atn/ColliCalc_unk | fn_8005AD7C | 99,9 | sin cambio (99,88; 5 variantes mas) |
| atn/ColliCalc_unk | fn_80059784 | 97,7 | equivalente NONMATCHING 98,54 (puntero al triangulo estatico en local) |
| atn/SoundManager | fn_800CE6E0 | 98,8 | equivalente NONMATCHING 99,72 (copia de SoundString por local) |
| atn/SoundManager | fn_800CECF0 | 98,3 | sin cambio (ordenes de declaracion, bloques en inline: igual o peor) |
| game/Em312DarkServant | vf100 | 98,4 | sin cambio (depende de fn_8030E5C8) |
| game/Em312DarkServant | fn_8030E5C8 | 98,2 | sin cambio (recursion inlinada; bucle en inline peor) |
| game/Em305VampireMp | vf0C | 98,9 | sin cambio (push_back de std/vector.h, bloqueo conocido) |
| game/Em305VampireMp | ctor | 96,9 | sin cambio (std::string como Em201ZanGurg) |
| atn/FileList | fn_8006C3F0 | 98,5 | **100 %** |
| atn/FileList | fn_8006BD78 | 97,7 | sin cambio (assign + push_back; ver Bloqueos) |
| game/ShopBuySell | fn_8058621C | 98,8 | equivalente NONMATCHING 99,22 (**arreglo**: el sufijo se anade, `+=`) |
| game/ShopBuySell | fn_805866F4 | 94,1 | equivalente NONMATCHING 97,23 |
| atn/NetManager | fn_8050CFC8 | 98,5 | sin cambio (ver Bloqueos) |
| atn/NetManager | Manager::Manager | 97,9 | sin cambio |
| game/EnemyLizerdBlade | fn_80334460 | 93,4 | **100 %**, la unidad enlaza |
| atn/WPad_unk | fn_800DDD18 | 93,7 | **100 %**, la unidad enlaza |
| game/ColliAttrManager | fn_80183FFC | 92,7 | sin cambio (92,45 con inline, no commiteado) |
| atn/EffectLoader_unk | fn_80239030 | 93,3 | sin cambio (byteswap; 93,79 no commiteado) |

## Para el port
- atn/WPad_unk.h: `WPad_unk::mFlags` pasa de `WPadFlags2_unk[4]` (campos de bits, struct borrada) a `unsigned int[4]` con mascaras (bit 0 = 1, bit 1 = 2); fn_800DDD18 usa `&= ~1`, `|= 1`, `&= ~2`. Mismo comportamiento.
- atn/WPad_unk fn_800DDD18: zona muerta en una inline `DeadZone_unk(float&)` por eje (4 llamadas), `WPAD_MIN` con la forma estandar `((a) < (b) ? (a) : (b))` y argumentos (ancho, valor): mismo resultado.
- (no es unidad del port) game/ShopBuySell fn_8058621C: el nombre del objeto de la lista lleva el sufijo `m08` anadido (`text += entry->m08`, do_replace(size, 0, ...)); antes lo sustituia (`text = entry->m08`). Cambia el comportamiento.
- atn/SoundManager.h: `SoundString_unk::operator=(const SoundString_unk&)` copia por un local y `*this = text` (mismo comportamiento).

## Decisiones tomadas sin el usuario
- atn/WPad_unk.h: `mFlags` como `unsigned int[4]` con mascaras y fuera la struct de bits `WPadFlags2_unk` (sin otros usuarios; Pad_unk.cpp tiene su propia vista `m3C00`, pendiente de unificar por la regla 10).
- atn/WPad_unk.cpp: `WPAD_MIN` (macro ya existente; el original evalua dos veces el valor, como una macro) pasa a la forma estandar `((a) < (b) ? (a) : (b))` con (ancho, valor). Inlines nuevas `DeadZone_unk(float&)` y `ReadAll_unk(WPad_unk*)` con `// inferred:`.
- game/ShopBuySell fn_805866F4: rectangulo del icono por campos en el orden v0, u0, v1, u1 (el original guarda v0 primero); `void* layout = base->mLayout;` antes de los bucles de parametros.
- game/EnemyLizerdBlade fn_80334460: locales `yaw`, `target`, `mask` y `GetMotionFrame(0)` (accesor real de Model_unk.h).

## Hallazgos
- Bucle corto (4 vueltas) que el original desenrolla entero y el nuestro de 2 en 2: el cuerpo en una inline llamada una vez por elemento (WPad_unk fn_800DDD18 `DeadZone_unk`). (escribir-20261116 qN6)
- Contador y puntero de un bucle con los registros cruzados frente al original (coalescencia con variables posteriores): meter el bucle en una `static inline` que recibe el objeto (WPad_unk `ReadAll_unk`, 99,95 -> 100). (escribir-20261116 qN6)
- Puntero de induccion creado en otro orden (incrementos del final del bucle en otro orden): declarar el local que lo usa despues del primer `if` que no lo necesita (WPad `status`). (escribir-20261116 qN6)
- Campos de bits de un array de palabras que el original direcciona con la base comun de los otros arrays por puerto: palabra `unsigned int` con mascaras (WPad `mFlags`). (escribir-20261116 qN6)
- `1 << i` comun a las dos ramas de un `if` (calculado antes de la prueba en el original): local `unsigned int bit = 1 << i;` al principio del cuerpo. (escribir-20261116 qN6)
- Direccion de un campo array (`base->mLayout`) hoisteada antes de un bucle en el original: local `void* layout = base->mLayout;` antes del bucle. (escribir-20261116 qN6)
- `diff[0] = ...; diff[1] = ...; diff[2] = 0;` en orden natural frente a `diff[2] = 0` en medio: otro orden de stores (ShopBuySell). (escribir-20261116 qN6)
- Puntero de un bucle que el original reparte con otra variable (coalescencia): declarar esa otra variable (aqui `const char* name`) antes del bucle hace que compartan registro (atn/FileList fn_8006C3F0, 98,5 -> 100). (escribir-20261116 qN6)
- Lista enlazada con `tail`: `if ((*tail = new T) == 0) break; T* node = *tail; node->mNext = 0; ...; tail = &node->mNext;` da el `lwz` de recarga de *tail del original. (escribir-20261116 qN6)
- Constructor de un local (ColliResult) despues de una llamada en el original: el valor que usa la llamada (mascara de `fn_80179D44()`) en un local declarado antes del objeto. (escribir-20261116 qN6)
- Puntero miembro releido tarde (`lwz 0x14d8(r31)` en el ultimo registro volatil): leerlo en un local justo antes de usarlo (`CharaTask* target = m14D8;`). (escribir-20261116 qN6)
- `float yaw = v.ToRotation_unk().y;` en su propio local antes de `WrapAngle_unk(yaw - mRot.y)` cambia los FPR de la constante 0 de la inline (EnemyLizerdBlade). (escribir-20261116 qN6)
- `Vector3 from(p); Vector3 to(p); from.y += a; to.y -= a;` (las dos copias antes de los cambios) frente a copia+cambio alternos: otro orden de stores. (escribir-20261116 qN6)
- Operador de copia de una cadena fija inlinado en un bucle de erase: `const char* text = other.mText; return *this = text;` cambia el reparto de registros del bucle (SoundManager 98,8 -> 99,7). (escribir-20261116 qN6)
- Estatico local de funcion: guardar su direccion en un puntero local (`T* t = &tri;`) la deja en un registro salvado durante el bucle como el original (ColliCalc_unk). (escribir-20261116 qN6)
- semdiff.py de agentes/bin tiene `os.chdir` a work-G4 fijo: copiarlo cambiando la ruta (si no, compara otra copia). (escribir-20261116 qN6)

## Bloqueos
- ColliAttrManager fn_80183FFC: `static inline CopySe_unk(this, index, data)` + 4 asserts vacios (para no inlinear en fn_80183D00) da 92,45 (como qD); path arriba, return temprano, data en dos pasos: igual.
- EffectLoader_unk fn_80239030 93,31: el segundo byteswap (cuenta de new[]) sale en otro orden de rlwimi; las 24 permutaciones de terminos, inline Swap32_unk y mascaras antes/despues: iguales o peores (93,79 con `((n & 0xFF00) << 8)`, no commiteado). Header r26/res r27 rotados.
- NetManager ctor 97,9: bucle de constructores de NetPlayerInfo_unk generado por el compilador (puntero final r27 en el original); no tocado.
- SoundManager fn_800CE6E0 99,72: el primer erase() inlinado tiene indice/texto cruzados (r27/r28) y el segundo no; probados forma del bucle de erase (for/while, ++i, 1+i, referencias src/dst, begin()), operador de copia explicito de SoundLogEntry, variantes de SoundString::operator=, estructura del while (continue, for, inline estatica UpdateLog_unk).
- SoundManager fn_800CECF0 98,3: x/y por bloque empeora (97,5).
- NetManager fn_8050CFC8: el `== 1` de IsJoined_unk se pliega a `!= 0` (int/bool/unsigned, inline antes o despues, local `joined`): sin efecto.
- ShopBuySell fn_805866F4 93,58: la direccion `anime->mLayout` del primer SetPaneText se calcula antes del hash en el original; `SetTaskText_unk` (como MP) y locales sin efecto.
- FileList fn_8006BD78 97,7: `mr r7, r6` del final de la cadena en assign (replace con Traits::length, `e = s; e += n`, assign(first, last)): igual o peor; queda ademas el push_back.
- Em312DarkServant fn_8030E5C8: 6 ordenes de master/list/i arriba: 98,23-98,31.

## Cambios de splits/symbols
- Splits sin cambios. symbols.txt: solo renumerados `@N` de pd.sh (SoundManager, ColliCalc_unk, FileList, EnemyLizerdBlade, WPad_unk, ShopBuySell) y `lbl_8088129C/A0/A4/AC` (WPad_unk) pasan a `@N` (sin usuarios externos). game_units.json: `game/EnemyLizerdBlade` y `atn/WPad_unk` a `matching: true` (main.dol OK).
- Avisos de poolcheck en game/EnemyLizerdBlade (floats 0.5/0.7/15/2.44346/20 de nuestra .sdata2 que no estan en el .o original) ya existian; la unidad enlaza con main.dol OK.
