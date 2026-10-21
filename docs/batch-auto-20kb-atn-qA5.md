# batch-auto-20kb-atn qA5: ColliBaseFunc_unk + AudioDevice

Cola: `atn/ColliBaseFunc_unk` (0x8004C9B4), `atn/AudioDevice` (0x80046220).

| Unidad | 100% | matching |
|---|---|---|
| `atn/ColliBaseFunc_unk` | 15/17 | false |
| `atn/AudioDevice` | 42/56 | false |

Total de la cola: +57 funciones al 100% frente a la integracion (6088 -> 6145 en el report), `main.dol: OK`,
`lost: 0` contra main y la integracion, sin bajadas de fuzzy.

## atn/ColliBaseFunc_unk (commits f8c40ef y siguientes)
- Resultado: 15/17 al 100%, `matching: false`.
- Contenido: el gestor de colisiones (singleton `lbl_8087EE98`, 0x9038 bytes: todas las formas, las activas,
  0x100 resultados, la rejilla de los arboles) y las virtuales base de `atn::ColliBase` (vf0C-vf1C devuelven
  0 / vacias). Crear (`fn_8004C9B4`), anadir/quitar forma (`fn_8004CB70`/`fn_8004CBB0`), consulta de esfera
  (`fn_8004CCA0` + `fn_8004CD5C`), consulta de segmento (`fn_8004CF3C` + `fn_8004ED34`), reconstruir la lista
  de activas y la rejilla (`fn_8004D124`), dibujo de depuracion (`fn_8004D258`), esfera en movimiento
  (`fn_8004D388`, 6,4 KB, recursiva) y los envoltorios que solo devuelven la posicion (`fn_8004D314`,
  `fn_8004ECC0`).
- NONMATCHING:
  - `fn_8004ED34` 93.24%: numeracion de GPR salvados (resultados r24 / extremos del segmento r26-r27 en el
    original), forma de la referencia a `hit->m38` en los flags de lado (el original guarda `&hit->m38` en r4
    en cada rama) y planificacion del bloque de atributos. Probados: referencia local, inline con referencia,
    inline bool, `results[bi]` / alias por referencia.
  - `fn_8004D388` 85.80%: borrador completo (sondeo inicial con fn_8004ED34, pasos `MIN(MAX(n,1),8)`, eleccion
    del resultado mas cercano con prioridad `m00`, esquina entre dos paredes con fn_80050900/fn_80050C38,
    empuje de suelo, deslizamiento, flags de atributo, sondeo final y recursion con `mask | 0x81000000`); sin
    ajustar bloques/registros/pila.
- **Decisiones sin el usuario**
  - Splits: anadida `.sbss 0x8087EE98-0x8087EEA0` (el puntero `lbl_8087EE98`, antes en `auto_10_8087EE80_sbss`).
  - `symbols.txt`: pairnames (vf0C/vf10/vf14/vf18/vf1C `__Q23atn9ColliBase...`), `lbl_80731040` -> `@stringBase0`
    (size 0x1, pool `""` del `new (3, "", "", 0)`).
  - `include/atn/ColliBase.h`: `ColliResult_unk` pasa a tener ctor inline (pone a 0 m34-m40: lo usan el array
    del gestor y los resultados locales) y miembros `int m00; Vector3 m04, m10, m1C, m28; ...; Vector3 m44`
    (antes arrays de bytes); `m3C` unsigned. Mismo codigo en atn_colli/ColliCalc (build completo + regress).
  - Tipos locales inventados: `ColliManager_unk`, `ColliPtrArray_unk` (count + 0x800 punteros; `PushBack_unk`
    recibe `ColliBase* const&`, `Size_unk()` como en el original), `ColliCell_unk`, `ColliSegment_unk` (la forma
    de vf10 es un segmento sin radio), `ColliLine_unk`/`ColliLinePoints_unk` (argumentos de fn_80050900/C38).
  - Funciones `extern "C"` con el gestor como primer argumento (las llaman asi gmk/game); `fn_8004D388` y
    `fn_8004D314` reciben `const Vector3* pos` + `float radius` (antes los llamadores declaraban una esfera;
    no se han tocado sus declaraciones).
- Hallazgos MWCC
  - Las virtuales base no inline de ColliBase se definen intercaladas con las funciones del gestor (vf0C tras la
    consulta de esfera, vf10/vf14 tras la de segmento, vf1C tras fn_8004D124, vf18 tras el dibujo).
  - `if (Size() >= N) return 0; PushBack(x)` con `PushBack(const T&)` + placement new recarga el contador (con
    `T` por valor o sin el getter se reutiliza el registro).
  - Variables de una inline que hace la consulta (celda + llamada) quedan por debajo de los temporales del
    cuerpo; con una inline `Distance_unk(a, b)` el temporal `a - b` baja al fondo de la pila.

## atn/AudioDevice (commits 10f6855, ad7a29b, bf513de y siguientes)
- Resultado: 42/56 al 100%, `matching: false`.
- Contenido: `atn::AudioDevice` (singleton `lbl_8087EE90`, 0x92C bytes): archivo de sonido DVD, su player,
  tres SoundHeap (cola de bancos 0x2CC, bancos 0x300, banco extra 0x330), Sound3DManager, tres efectos AUX
  (reverb 0x410), mapas MSL (presets de reverb 0x8AC, ids de `rsid.csv` 0x8D0, etiquetas 0x8E8/0x8F4),
  `Sound3dEngine` (0x900) y la ruta de datos (0x91C). Init/Finalize, carga de bancos (por referencia, cola,
  extra), actualizacion de lecturas DVD, consultas de ids/etiquetas, volumen/modo de salida, reverb.
- NONMATCHING:
  - Destructor 29.45%, `fn_80047628` 24.03%, `fn_8004A03C` 16.21% y las tres `Destroy` de los mapas
    (0x8004A664/8FC/ABDC) 9.31%: el MWCC de aqui inlinea un nivel mas la recursion de `Destroy` (tres niveles
    en la copia fuera de linea, el original dos). Probado: definida dentro/fuera de la clase, funcion libre,
    estatica, comprobacion de nulo dentro, ayudante por hijo, nodo con destructor. Mismo bloqueo que
    `game/TownTask` y `atn/ptr_string_base_char_0`.
  - `fn_80046228` 91.96%: registros del bucle de callbacks de `boost::function` (como LoadHandle/main).
  - `fn_80048F8C` 92.06%, `fn_80049CDC` 95.08%, `fn_8004A1D4` 98.79%, `fn_80049E54` 99.75%, `fn_8004937C`
    99.40%, `fn_8004829C` 99.98%: registros / orden de operandos / forma de salto (ver comentarios).
  - `fn_8004A660` (blr de 4 bytes entre las funciones de los mapas): sin escribir (no se sabe que es).
- **Decisiones sin el usuario**
  - Splits: anadida `.sbss 0x8087EE80-0x8087EE98` (EE80-EE94 son de este fichero).
  - `symbols.txt`: ctor/dtor `__ct__/__dt__Q23atn11AudioDeviceFv`; pairnames (`__ct__Q23atn13AudioBank_unkFv`,
    dtors debiles `__dt__Q23atn12AudioFxA_unkFv`/B/C, `__dt__Q23atn18Sound3DManager_unkFv`, FindInsertPos/
    InsertNode/Destroy de `AudioMap_unk<...>`); `lbl_80730F54` -> `@stringBase0`; `lbl_8087D718/1C` ->
    `@STRING@reserve__Q23atn33Vector_unk<Q23atn13AudioBank_unk>FUii[@0]`; `lbl_80777538/54` ->
    `@STRING@__allocate__37AudioMap_unk<...>Fv`.
  - `symbols.txt`, nombres reales de NW4R (vtables con nombre en el DOL o layout evidente):
    `fn_8059D7D8/fn_8059D824` -> `__ct__/__dt__Q34nw4r3snd15DvdSoundArchiveFv`, `fn_8059E7EC/fn_8059EB1C` ->
    `__ct__/__dt__Q34nw4r3snd18SoundArchivePlayerFv`, `fn_80717680/fn_807176D0` ->
    `__ct__/__dt__Q34nw4r3snd9SoundHeapFv` (vtable + OSMutex + FrameHeap; vtable `lbl_807C6328` sin nombre).
  - Clases NW4R minimas locales (solo tamanos): DvdSoundArchive, SoundArchivePlayer, SoundHeap (con
    `GetCurrentLevel`/`GetFreeSize` via `AutoMutexLock_unk`), `nw4r::ut::detail::LinkListImpl`. Los efectos AUX
    son envoltorios `AudioFxA/B/C_unk` (ctor inline que llama a fn_8070E760/fn_8070DC20/fn_8070E190, dtor
    inline que pone la vtable de la libreria `lbl_807C5F28/...ED8/...F00` y llama a Shutdown/ReleaseWorkBuffer
    a traves de `AudioFxBase_unk`).
  - `AudioMap_unk<K, V, Extra>`: mapa MSL local (como el de ptr_string_base_char_0), con una palabra extra
    entre raiz y nodo mas a la izquierda en los mapas de 0x10 bytes (0x8AC/0x8D0).
  - `m04` es `volatile` (el original lo relee tras cada comprobacion).
  - `((void)0)` x4 en `fn_80047B54` para que no se autoinlinee en `fn_80046EE0` (el original lo llama).
  - Cadenas del pool con `"\0"` final ("SoundError: LoadError Bank[%s]\0", "...Extra Bank[%s]\0", "%s\0"): el
    pool del original tiene un `""` sin fusionar tras cada una.
  - `fn_800496C0` (banco extra) lee el nivel del heap de bancos (0x300), no el del extra: asi esta en el DOL.
  - Todas las funciones siguen `extern "C"` (las llaman main, Se_unk, SongManager, GameBgm, VideoTHP...).
- Hallazgos MWCC
  - `map::operator[]` de MSL: `V()` de un POD se construye en un temporal en la pila y se copia (stores
    muertos); con ctor en el valor el temporal desaparece.
  - Un struct con ctor por defecto se copia miembro a miembro con lfs/stfs (POD: lwz/stw).
  - Inline `GetCurrentLevel` con `AutoMutexLock` (referencia al mutex) deja el puntero al mutex en un GPR
    salvado; con Lock/Unlock explicitos se recalcula.
  - Dtor inline que pone una vtable ajena y llama virtuales: llamar a traves de una interfaz polimorfica
    (`((Base*)this)->f()`) da el `lwz r12` del original; con punteros a funcion a mano sale otro registro.

## Pendientes
- Recursion de `Destroy` de los mapas MSL (6 funciones de AudioDevice): buscar la forma del original.
- `fn_8004D388`: ajustar el borrador (85.80%).
- `fn_8004ED34`: forma de `&hit->m38` en r4 en las tres ramas.
- `fn_8004A660`: identificar la funcion vacia.
- Revisar las declaraciones de `fn_8004D388`/`fn_8004D314` en gmk/Brazier y game/CharaTask (pasan una esfera
  donde el original recibe `const Vector3*` + radio; codigo identico).
