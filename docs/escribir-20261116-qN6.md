# escribir-20261116 cola N6 (cerca de enlazar: atn, chr y game no repartidas)

Inicio: 2026-11-17 08:18 CET. Tope 10:48.

## Tabla de la cola

| unidad | funcion | antes | despues |
|---|---|---|---|
| atn/ColliCalc_unk | fn_8005AD7C | 99,9 | sin cambio (99,88; 5 variantes mas) |
| atn/ColliCalc_unk | fn_80059784 | 97,7 | equivalente NONMATCHING 98,54 (puntero al triangulo estatico en local) |
| atn/SoundManager | fn_800CE6E0 | 98,8 | equivalente NONMATCHING 99,72 (copia de SoundString por local) |
| atn/SoundManager | fn_800CECF0 | 98,3 | |
| game/Em312DarkServant | vf100 | 98,4 | |
| game/Em312DarkServant | fn_8030E5C8 | 98,2 | |
| game/Em305VampireMp | vf0C | 98,9 | |
| game/Em305VampireMp | ctor | 96,9 | |
| atn/FileList | fn_8006C3F0 | 98,5 | **100 %** |
| atn/FileList | fn_8006BD78 | 97,7 | |
| game/ShopBuySell | fn_8058621C | 98,8 | equivalente NONMATCHING 99,22 (**arreglo**: el sufijo se anade, `+=`) |
| game/ShopBuySell | fn_805866F4 | 94,1 | |
| atn/NetManager | fn_8050CFC8 | 98,5 | |
| atn/NetManager | Manager::Manager | 97,9 | |
| game/EnemyLizerdBlade | fn_80334460 | 93,4 | **100 %**, la unidad enlaza |
| atn/WPad_unk | fn_800DDD18 | 93,7 | |
| game/ColliAttrManager | fn_80183FFC | 92,7 | |
| atn/EffectLoader_unk | fn_80239030 | 93,3 | |

## Para el port
- (no es unidad del port) game/ShopBuySell fn_8058621C: el nombre del objeto de la lista lleva el sufijo `m08` anadido (`text += entry->m08`, do_replace(size, 0, ...)); antes lo sustituia (`text = entry->m08`). Cambia el comportamiento.
- atn/SoundManager.h: `SoundString_unk::operator=(const SoundString_unk&)` copia por un local y `*this = text` (mismo comportamiento).

## Decisiones tomadas sin el usuario

## Hallazgos
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
- SoundManager fn_800CE6E0 99,72: el primer erase() inlinado tiene indice/texto cruzados (r27/r28) y el segundo no; probados forma del bucle de erase (for/while, ++i, 1+i, referencias src/dst, begin()), operador de copia explicito de SoundLogEntry, variantes de SoundString::operator=, estructura del while (continue, for, inline estatica UpdateLog_unk).
- SoundManager fn_800CECF0 98,3: x/y por bloque empeora (97,5).
- NetManager fn_8050CFC8: el `== 1` de IsJoined_unk se pliega a `!= 0` (int/bool/unsigned, inline antes o despues, local `joined`): sin efecto.
- ShopBuySell fn_805866F4 93,58: la direccion `anime->mLayout` del primer SetPaneText se calcula antes del hash en el original; `SetTaskText_unk` (como MP) y locales sin efecto.
- FileList fn_8006BD78 97,7: `mr r7, r6` del final de la cadena en assign (replace con Traits::length, `e = s; e += n`, assign(first, last)): igual o peor; queda ademas el push_back.
- Em312DarkServant fn_8030E5C8: 6 ordenes de master/list/i arriba: 98,23-98,31.

## Cambios de splits/symbols
