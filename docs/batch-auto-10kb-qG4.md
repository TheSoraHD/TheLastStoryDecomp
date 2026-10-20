# batch-auto-10kb qG4: game/MP (Gun, HomingBullet, main, MapPartsLocatorSet, MP/Text)

Cola: `game/Gun`, `game/HomingBullet`, `game/main`, `game/MapPartsLocatorSet`, `MP/Text`.

## game/Gun + game/HColliData_unk (commit 9c49c28)
- Resultado: `game/Gun` 5/6 al 100%, `matching: false`; `game/HColliData_unk` (nueva) 4/11 al 100%,
  `matching: false`. `main.dol: OK`, regress lost: 0.
- NONMATCHING:
  - `Gun::vf14` 99.91%: el original carga primero la z de la traslacion (pool .sdata2 3.7, 0.5, 0.0)
    pero guarda el vector empezando por x; probados vector con nombre, asignaciones z/y/x, Set_unk,
    helpers inline por valor/referencia/componentes. Por eso `.sdata2` queda al 50% en objdiff.
  - `ColliObjectData_unk::fn_8043F694` 38%, `fn_8043F908` 73%, `ColliTreeData_unk::ColliTreeData_unk`
    49%, `fn_8043FEE8` 60%, `fn_80440184` 45%, `fn_80440280` 62%, `fn_80440360` 56%: el original
    desenrolla los bucles de intercambio de bytes (`stwbrx`) y usa punteros de induccion separados
    para cargas y stores; Wii/1.3 (y 1.0/1.1/GC 3.0a5.2) nunca desenrolla un bucle cuyo cuerpo usa
    `__stwbrx` (ni con inline, static, plantilla ni puntero). Ademas el original inlinea una vez la
    recursion de `fn_80440360`.
  - `__sinit` de HColliData_unk (0x804405CC) no escrito: solo pone a 1 los guards compartidos
    0x8087F3CC/0x8087F3CD (ids de facetas `std::num_put`/`numpunct` de MSL, como el `__sinit` de
    `game/GameScene` 0x803CCF38); el fichero real incluye cabeceras de locale de MSL.
- **Decisiones sin el usuario**:
  - splits.txt: `game/Gun` cortada. Antes: .text 0x8043F028-0x804405F8, .ctors 0x8072D370-0x8072D374,
    .rodata 0x80754498-0x807544C8. Despues: `game/Gun` .text 0x8043F028-0x8043F5C4, .rodata
    0x80754498-0x807544B8 (.data/.sdata2 igual); nueva `game/HColliData_unk.cpp` .text
    0x8043F5C4-0x804405F8, .ctors 0x8072D370-0x8072D374, .rodata 0x807544B8-0x807544C8.
    Prueba: el pool de cadenas de Gun (`@stringBase0` 0x80754498) va al principio de .rodata y las
    constantes magicas "@HCB"/"@HOC" detras, y el `__sinit` cierra el tramo ColliData: dos ficheros.
  - Nombre inventado `game/HColliData_unk.cpp`: el orden de enlace de esta zona es alfabetico por
    nombre de fichero (CharaPathMove, Gun, ?, HomeButtonMenu...), asi que el real empieza entre
    "Gun" y "HomeButtonMenu". Clases `ColliObjectData_unk`/`ColliTreeData_unk` (ya declaradas en
    atn/LH_ColliObject.cpp y atn/LH_ColliTree.cpp, sin cambiar).
  - Firma de `Arms::vf18`/`Sword::vf18` cambiada a `(const atn::Vector3&, const atn::Vector3&, int, int)`
    (la de `chr::CharaPart_unk::vf18`, como la llama CharaTask): `Gun::vf18` lee el cuarto argumento
    (`fn_80219E6C(d)`). Cabeceras compartidas `include/game/Arms.h`, `include/game/Sword.h`,
    `src/game/Arms.cpp`, `src/game/Sword.cpp` tocadas; symbols.txt `vf18__4ArmsFv` ->
    `vf18__4ArmsFRCQ23atn7Vector3RCQ23atn7Vector3ii`, `vf18__5SwordFv` -> igual con `5Sword`. Sword
    sigue `matching: true` y el DOL OK.
  - symbols.txt: funciones de Gun por pairnames (+ `vf14__3GunFv`), `__vt__3Gun`, `__RTTI__3Gun`,
    `@stringBase0` 0x80754498 (scope:local, 0x1C); funciones de ColliObjectData_unk/ColliTreeData_unk
    0x8043F694-0x8044058C con nombres `fn_<addr>__<clase>`; lector de chunks del tramo HomingBullet
    renombrado: 0x80442B50 `__ct__13ChunkFile_unkFPvUi`, 0x80442E64 `__dt__13ChunkFile_unkFv`,
    0x80442EA4 `IsSwapped__13ChunkFile_unkCFv`, 0x80442EAC `GetChunkCount__13ChunkFile_unkCFv`,
    0x80442ECC `FindChunk__13ChunkFile_unkFiUi`.
  - Cabeceras nuevas: `include/game/Gun.h`, `include/game/ChunkFile_unk.h` (clase inventada
    `ChunkFile_unk`: lector de ficheros de chunks con offsets relativos y cambio de endianness).
  - game_units.json: entradas nuevas `game/Gun.cpp` y `game/HColliData_unk.cpp`.
- Hallazgos MWCC:
  - Wii/1.3 no desenrolla bucles cuyo cuerpo usa `__stwbrx`/`__lwbrx` (si los de stores normales).
  - Miembro con destructor llamado desde el dtor de la clase: `li r4,-1` (no 0 como la base).
  - `float sz = Col(2); float sy = Col(1); float sx = Col(0); Vector3 s(sx, sy, sz);` dentro de un
    inline da el orden de pila del original (s debajo de los tres temporales de columna).
- Pendientes: `__sinit` con guards de facetas MSL (si se quiere enlazar HColliData_unk).
