# mwcc-blockers q1: `std::vector::push_back` inline (agente 1, 2026-10-21)

Bloqueo: el `push_back` reconstruido en `include/std/vector.h` no casa con el original ni como copia
debil (0x800DB3EC, 0x2F4 bytes, 23 copias de ese tamano en el DOL) ni inlineado en funciones del
juego. Scratch: `/home/claude/scratch/agent-1/` (herramientas abajo).

Resultado: la copia debil 0x800DB3EC queda igualada instruccion a instruccion (salvo el offset del
literal) y `Em002_01BlackSkelton::fn_80248A4C` llega al 100% con la variante `vector_a7.h`, pero esa
variante baja 0,02-0,04 puntos de fuzzy en 11 funciones (todas por el reparto r3/r4 de size/cap en
el camino rapido inlineado dentro de bucles), asi que por la condicion 8 NO se ha commiteado. Queda
documentada entera para la proxima sesion (diff al final).

## Herramientas del scratch (`/home/claude/scratch/agent-1/`)

- `try2.sh f.cpp [incdir]`: como `tools/decomp/try.sh` pero con `inc/` (copia de `include/std`) por
  delante, para probar variantes de `vector.h` sin tocar el repo.
- `norm.py` + `diffpb.sh [incdir]`: compila `pb.cpp` (fuerza la copia debil de `vector<Foo*>::push_back`)
  y la compara normalizada con `ref_ptr.txt` (asm de 0x800DB3EC). Lo unico que debe quedar son 3
  lineas `addi r4,r4,1` (offset del literal en el pool).
- `inl.cpp`/`inl.sh`, `inl2.cpp`, `inl3.cpp`, `inl4.cpp`, `inl5.cpp`: bancos de inline con la cabecera
  real: `em002` (x miembro, sin bucle: 0x80248B30), `loop` (vector<int> en bucle: Rebuild_unk
  0x804B8048), `inl3::read` (x en registro, bucle: NewCampPartsHint 0x805156D8), `inl4` (ChaserGuard).
- `pbvar.sh base.h < cuerpos`: sustituye el cuerpo entero de `vector<T>::push_back` y muestra los dos
  caminos rapidos (`reg`, `mem`). `ptrvar.sh`, `intvar.sh`: lo mismo con el cuerpo de `__push_fast_unk`
  de punteros / int. `recvar*.sh`, `deadvar.sh`: lo mismo con `__recommend_unk` y el bloque muerto.
- `bench.sh` / `bench2.sh [hdr]`: copia `inc/std/vector.h` al repo y pasa `cmp.py` por las unidades
  afectadas (2 s / 40 s). `fuzzcmp.py [new] [ref]`: bajadas/subidas de fuzzy por `virtual_address`.
- `inc/std/vector_aN.h`: todas las variantes probadas (a3, a4, a7 ... a18, `vector_m_*` para los max).

## Que difiere exactamente (asm original vs nuestro, copia debil 0x800DB3EC, `vector<T*>`)

Camino rapido. Original:
```
lwz r6, 0x4(r3)      ; size
lwz r5, 0x8(r3)      ; cap   (cap en r5: se crea DESPUES de size y recibe el registro mas bajo)
cmplw r6, r5
bge slow
addi r5, r6, 1       ; size + 1 en un registro nuevo (no `size++` sobre el mismo)
stw r5, 0x4(r3)
subi r0, r5, 1
lwz r3, 0x0(r3)
slwi r0, r0, 2
lwz r4, 0x0(r4)      ; el elemento se carga de la referencia DESPUES del store de size
stwx r4, r3, r0
```
Nuestro (`main`): `lwz r6,0x8; lwz r5,0x4; cmplw r5,r6; addi r5,r5,1; ...` (cap primero, size++ en
el mismo registro). En el camino lento el original usa `subf r0, r5, r0` (cap sigue en r5).

Inlineado con x miembro y sin bucle (Em002 fn_80248A4C, 0x80248B30): `lwz r3,size; lwz r4,cap;
cmplw r3,r4; addi r3,r3,1; stw; subi r0,r3,1; lwz r3,data; slwi; lwz r4,x(r30); stwx`: recarga `x`
(miembro `CharaTask* m14B8`) tras el store del tamano aunque ya lo tenia en r0 para el `cmpwi`.

Inlineado en bucle (x en registro o miembro, punteros o int; NewCampPartsHint 0x805156D8, Friend
0x804B8048, ChaserGuard 0x8017CCF0, ChaserNpc 0x801806C8, ArrowSet 0x802074E8): `lwz r4,size;
lwz r3,cap; cmplw r4,r3; bge; addi r4,r4,1; lwz r3,data; slwi r0,r4,2; stw r4,size; add r3,r3,r0;
stw rX,-4(r3)` (en ChaserNpc/ArrowSet size r5, cap r4). Es decir: en bucles size recibe el registro
MAS ALTO que cap y size+1 reutiliza el registro de size; fuera de bucles (Em002) es al reves.

`max_size()/3` dividido en tiempo de ejecucion (`mulhwu 0xAAAAAAAB`) solo aparece en
Event/Channel/Section/Cut (elementos `boost::shared_ptr`, 40%), NO en punteros ni int (constantes
plegadas `lis 0x1555; addi 0x5555`). No se ha tocado (sin banco de pruebas barato).

Bloque muerto (ChaserGuard 0x8017CD44, CrowdManager 0x801EB2E4) = `__recommend(1)` tras el check de
longitud, con las cargas eliminadas pero no los `cmplwi r0,1` ni los saltos: `...mulhwu; srwi;
cmplwi r0,1; bge L; b L; b L; L2: subi; cmplw cap; bge L; addi; srwi; cmplwi r0,1; cmplwi r0,1; L:`.
En Channel (shared_ptr) el bloque muerto conserva los `stw` de los temporales de `max` y `lwz
r0,0x8(r1)` (el `1` almacenado en pila): alli `max` recibe referencias. En ChaserGuard no hay store
del 1 (`cmplwi` inmediato). Es un residuo del DCE de MWCC que depende del tamano de la funcion: en
`inl4.cpp` (funcion pequena) el mismo codigo elimina el bloque entero.

## Inventario (fuzzy de `/home/claude/ref/report.json`, main 6ace171)

| unidad | funcion | dir | % |
|---|---|---|---|
| Event/Channel | fn_8053A510 | 0x8053A510 | 40.48 |
| Event/Cut | fn_8053B030 / fn_8053B828 | 0x8053B030 / 0x8053B828 | 41.52 / 40.46 |
| Event/Section | fn_8053C788 | 0x8053C788 | 41.23 |
| MP/D2ACommonDialog | fn_804AD1EC | 0x804AD1EC | 93.09 |
| MP/D2AMpMenuFriend | fn_804B6F08 / fn_804B7F88 | 0x804B6F08 / 0x804B7F88 | 99.90 / 99.92 |
| MP/D2AMpMenuFriendRoom | fn_804BB2CC | 0x804BB2CC | 95.34 |
| atn/CompositeCache_unk | fn_800D8698 | 0x800D8698 | 97.99 |
| atn/D2A_MessageTable_unk | fn_801F36C8 | 0x801F36C8 | 93.61 |
| atn/LH_Material | vf0C (LoadMaterial) | 0x804742F4 | 95.52 |
| atn/LH_MotionSet | fn_80476ED4 | 0x80476ED4 | 95.56 |
| atn/SequenceHolder | fn_800C3990 | 0x800C3990 | 97.59 |
| db/ArrowSet_unk | fn_80207310 | 0x80207310 | 99.28 |
| game/ChaserGuard | ctor | 0x8017CB9C | 96.09 |
| game/ChaserNpc | ctor | 0x801805A4 | 99.91 |
| game/CrowdManager | fn_801EB188 | 0x801EB188 | 98.73 |
| game/Em002_01BlackSkelton | fn_80248A4C | 0x80248A4C | 99.51 |
| game/EnvSeBase | fn_80363108 | 0x80363108 | 99.85 |
| game/LibraryTask | fn_80469D60 | 0x80469D60 | 97.34 |
| game/MenuCommonTask | fn_804A3C24 | 0x804A3C24 | 93.38 |
| game/NewCampPartsHint | vf20 / vf30 / fn_80516004 | 0x80515318 / 0x80515AC8 / 0x80516004 | 99.54 / 99.79 / 96.98 |
| game/Np131LivingArmor | vf10 / fn_80546B84 | 0x80546228 / 0x80546B84 | 96.81 / 96.40 |
| game/ShopColor | vf38 | 0x80587624 | 99.41 |
| game/U16Table_unk | fn_803CCF68 / fn_803CD598 | 0x803CCF68 / 0x803CD598 | 89.63 / 88.46 |
| gmk/MapLift | vf20 | 0x8042A360 | 83.41 |

Ya probado antes (comentario de vector.h, q5): accesores, comparacion invertida, if/else, helper para
el check de longitud, 8 formas de la comparacion desde el .cpp.

## Intentos (cabecera; sondas sueltas `tNN.cpp` aparte)

1. `size_type n = mSize; if (n < mCap) { mSize = n + 1; mData[mSize - 1] = x; return; }` en
   `push_back`: la copia debil queda IDENTICA (size antes que cap, cap en r5, `addi r5,r6,1`, `subf
   r0,r5,r0`). PERO push_back deja de autoinlinarse en todo el banco (fn_80248A4C 271 vs 100 instrs):
   el `n` local supera el presupuesto (~13 sentencias).
2. Igual pasando `n` a `__push_fast_unk(n, x)`: lo mismo (copia OK, inline perdido).
3. 2 + partir `buf.mData = __allocate(cap); buf.mCap = cap; buf.mStart = mSize;` en un helper
   `__vec_set_buf_unk(buf, cap)`: copia OK y push_back vuelve a inlinarse. `ninja` completo: OK, lost
   0, 100% 5375 = 5375, 13 subidas y 11 BAJADAS de 0,02-0,04 (ver lista en el intento 7).
4. 3 + `const T& y = x;` (segunda referencia) para forzar la recarga de `x`: sin efecto en el banco.
   Sondas t19-t21: una referencia con nombre SI fuerza `addi r4,this,16 ... lwz r4,0(r4)` en una
   struct suelta, pero el original recarga por desplazamiento (`lwz r4,0x14b8(r30)`): no es eso.
5. Helper con la firma antigua (`mSize++`) y solo `n` para la comparacion: la copia debil vuelve a
   cap-primero (hace falta `mSize = n + 1`, no solo la comparacion con un local).
6. 3 + `size_type cap = mCap;` local: nada cambia.
7. **3 + en la especializacion de punteros `((void**)this->mData)[this->mSize - 1] = x;`**
   (`vector_a7.h`): el store a traves de `void**` invalida el `x` cargado antes del `cmpwi`
   (regla MWCC: un store por `void*`/puntero recarga los punteros, un store por `unsigned` no,
   sondas t17/t29/t30/t35/t37) y **fn_80248A4C de Em002 llega al 100%**. `ninja` completo: OK, lost
   0, 100% 5375 -> 5376, 13 subidas (SequenceHolder 97.59->97.64, LH_Material 95.52->95.63, ...) y
   las mismas 11 bajadas que en el intento 3: ChaserNpc ctor 99.91->99.87, ArrowSet fn_80207310
   99.28->99.25, LibraryTask fn_80469D60 97.34->97.31, Friend fn_804B6F08 99.90->99.88 y fn_804B7F88
   99.92->99.89, FriendRoom fn_804BB2CC 95.34->95.32, NewCampPartsHint vf20/vf30/fn_80516004,
   Np131 vf10 96.81->96.79, ShopColor vf38 99.41->99.40. Todas por lo mismo: en bucles el original
   da size r4 / cap r3 y ahora damos size r3 / cap r4 (antes dabamos cap r4 / size r3 cargando cap
   primero, que casualmente coincidia en una instruccion mas). Para LibraryTask/ShopColor/U16Table
   hay que cambiar la firma de sus `__push_fast_unk` especializados (`unsigned int n, const X&`).
8. `__recommend_unk` con `max` por valor (`g < n ? n : g`, `__vec_maxv_unk`): ChaserGuard ctor
   244 -> 34 lineas distintas pero el bloque muerto desaparece del todo (el original lo conserva),
   y el camino vivo pierde los temporales de pila. Las 8 formas de `max` por referencia (`if/else`,
   puntero, `>=`, `>`, `<=`, invertida, funcion de comparacion, anidada) dan lo mismo que la actual.
   `max` como plantilla NO inline (autoinline): igual; por valor no inline: peor (293).
9. Solo el store `void**` sin el `n` local: la copia debil vuelve a cap-primero.
10. Camino lento en un helper `__push_slow_unk(x)` (con/sin `cap` por parametro): identico al 7.
11. `cap` local pasado al check, al `__recommend_unk(1, cap)` y al `__grow_cap_unk`: ChaserGuard 127,
    pero el helper deja de inlinarse (`@STRING@__push_slow_unk`) y todo lo demas empeora.
12. 11 con `this->mCap` recargado solo en `__grow_cap_unk`: todo empeora (Em002 67/271).
13. `n` pasado tambien al camino lento (`__push_slow_n_unk(x, n)`): `n` pasa a registro salvado
    (r15/r29) en ambos contextos. No.
14. Helper `bool __try_push_unk(x)` con el camino rapido dentro (y la forma `n >= cap` negada): cap
    acaba en r0 (`cmplw r3,r0`): no.
15. Accesores `size()`/`capacity()`/referencias `__size_ref_unk()`/`__cap_ref_unk()`: cap en r0 o
    carga invertida. `mCap > n`, `mCap <= n`, `n >= mCap`, `bool full`, `mCap - n`: compare
    invertido o perdida del camino rapido.
16. `__recommend_unk(this->mSize + 1 - this->mSize)`, `(this->mSize + 1)`, `(const size_type& n)`,
    `(void)`, variable muerta, `if (... > max_size())`, envoltorios de uno y dos niveles: el bloque
    muerto no cambia (o empeora).
17. `__push_fast_unk` por referencia `const unsigned int& n` o `const size_type& n = mSize` en
    push_back: igual al 7 o carga invertida.
18. `mSize = mSize + 1` / `unsigned& sz = mSize; sz = sz + 1` en el helper, con `if (mSize < mCap)`:
    cap primero.
19. Formas con punteros `end()`/`begin() + mCap` para la comparacion: generan `srawi/addze` o `add`
    (no es libc++ con punteros, confirmado).
20. Cuerpos del helper de punteros (ptrvars*.txt, 30 formas): `[n] = x` / `*(data + n)` / puntero
    local a `data + n` dan size r4 cap r3 (lo que queremos en bucles) PERO con `addi r0,r4,1; stw r0;
    slwi r0,r4,2; stwx` (el original usa el tamano nuevo para la direccion: `add; stw -4`); `n++;
    mSize = n; [n-1]` y `++n` se pliegan a lo mismo; `mSize = n + 1; [mSize - 1]` (a7) da la forma
    buena pero size r3. `volatile`, `(void)n`, `d` local, `m = n + 1` local: sin cambio util.
21. Cuerpos del helper de int (intvars.txt, 10 formas): mismo comportamiento que los punteros.
22. Mantener `n` vivo tras el camino rapido (`unsigned& n` en el helper, `n = 0` despues, uso en el
    camino lento, `if (n != mSize)`): sin cambio (a19/pbvars8).

Sondas de orden de carga en structs sueltas (t1-t3, t25, t33, t41, t42, t48): el orden de las cargas
en una funcion suelta es size,cap con `n = s` y cap,size con `s++` sobre el miembro; con el camino
lento inlineado y `x` en registro (t48, 8 formas) size sale en r4 y cap en r0, new size r5: la
asignacion depende del contexto (presion de registros), no de la forma local de la comparacion.

## Hallazgos MWCC (una linea cada uno)

- `n = mSize; if (n < mCap) { mSize = n + 1; mData[mSize - 1] = x; }`: carga size antes que cap, cap
  en el registro bajo, `addi` a un registro nuevo y `subf r0,cap,r0` despues; `mSize++` directo
  carga cap primero. El `mSize - 1` tras el store NO se pliega al `n` local (reenvio del store).
- Un store a traves de `void**` (o de cualquier puntero) invalida un puntero miembro ya cargado
  (`o->mX` se recarga); un store a `unsigned`/`int`/`char`/`float` por miembro no. Asi se obtiene la
  recarga `lwz r4,0x14b8(r30)` tras `stw size` del original para elementos puntero.
- Una variable local mas en `push_back` lo saca del autoinline; se compensa sacando 3 sentencias a un
  helper (`__vec_set_buf_unk`).
- El bloque muerto `__recommend(1)` solo sobrevive en funciones grandes; en una funcion pequena MWCC
  lo elimina del todo con el mismo fuente.

## Lo que NO funciona (no repetir)

- Cualquier forma de la comparacion que no empiece por `n = mSize` seguida de `mSize = n + 1`.
- `n = mSize` en `push_back` sin compensar sentencias (pierde el autoinline).
- `const T& y = x` / `T const* py = &x` para la recarga del elemento (genera `lwz r4,0(r4)`).
- `max` por valor, no inline, `if/else`, invertido, por puntero: no reproduce el bloque muerto.
- `cap`/`n` pasados al camino lento (acaban en registros salvados o rompen el inline).
- Accesores/referencias para size/cap (cap en r0), comparaciones negadas (compare invertido),
  formas con `end()`/punteros.
- Indexar por el `n` local en el store (`[n] = x`): forma `stwx` con `addi r0`, el original usa el
  tamano nuevo.

## Siguiente paso sugerido

El unico residuo en punteros/int es el reparto r3/r4 de size/cap en bucles (11 funciones a 0,02 del
estado actual, Em002 ya al 100%). Hay dos salidas: (a) aceptar `vector_a7.h` relajando la condicion
8 para esas 11 bajadas de centesimas (decision del usuario), o (b) encontrar la forma que de size r4
cap r3 en bucles manteniendo `addi r4,r4,1; ...; add; stw -4` (con `x` en registro) y size r3 cap r4
fuera de bucles. Las sondas t48 sugieren que el registro de size sube cuando `n` sigue vivo despues
del `addi` (sin plegarse): probar un uso posterior de `n` que no se pliegue (p. ej. en el helper de
`__vec_split_unk::push_back` o en `__swap_out_unk` a traves de una referencia).

## Diff de `vector_a7.h` sobre `include/std/vector.h` (main 6ace171)

```
-    void __push_fast_unk(const T& x) {
-        new (this->mData + this->mSize) T(x);
+    void __push_fast_unk(unsigned int n, const T& x) {
+        new (this->mData + n) T(x);
...
-    void __push_fast_unk(T* const& x) {
-        this->mSize++;
-        this->mData[this->mSize - 1] = x;
+    void __push_fast_unk(unsigned int n, T* const& x) {
+        this->mSize = n + 1;
+        ((void**)this->mData)[this->mSize - 1] = x;
...
-    void __push_fast_unk(const int& x) {
-        this->mSize++;
+    void __push_fast_unk(unsigned int n, const int& x) {
+        this->mSize = n + 1;
...
+    void __vec_set_buf_unk(__vec_split_unk<T>& buf, size_type cap) {
+        buf.mData = __allocate(cap);
+        buf.mCap = cap;
+        buf.mStart = this->mSize;
+    }
...
-    if (this->mSize < this->mCap) {
-        this->__push_fast_unk(x);
+    size_type n = this->mSize;
+    if (n < this->mCap) {
+        this->__push_fast_unk(n, x);
...
-    size_type cap = __grow_cap_unk(this->mSize + 1 - this->mCap);
-    buf.mData = __allocate(cap);
-    buf.mCap = cap;
-    buf.mStart = this->mSize;
+    __vec_set_buf_unk(buf, __grow_cap_unk(this->mSize + 1 - this->mCap));
```
Mas la firma `(unsigned int n, const X& x)` con `this->mSize = n + 1;` en los `__push_fast_unk`
especializados de `include/game/LibraryTask.h`, `include/game/ShopColor.h` y
`src/game/U16Table_unk.cpp` (sin cambios de % en esas tres unidades).

## Tabla final (sin commit: antes = despues)

Ninguna funcion cambia en la rama. Con `vector_a7.h` seria: 100% 5375 -> 5376 (+Em002
fn_80248A4C), 13 subidas de fuzzy, 11 bajadas de 0,02-0,04.

## Decisiones tomadas sin el usuario

- No commitear `vector_a7.h` por las 11 bajadas de centesimas (condicion 8). La variante queda en
  `/home/claude/scratch/agent-1/inc/std/vector_a7.h` y reproducida arriba.
- No se ha tocado el caso `shared_ptr` (Event/*) ni `__recommend` en tiempo de ejecucion.
