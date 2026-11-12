# port-propuestas3-20261112: cambios del port de PC (fase 2) para la decomp

Rama desde `main` 545192a. Referencia: build limpio de `main` (`main.dol: OK`, 15057 / 18109).
Resultado: **15058 / 18109**, `main.dol: OK`, `regress.py` lost 0, `fuzzdrop.py` 0 bajadas,
`poolcheck.py` sin avisos en las unidades tocadas, `ordercheck.py` sin funciones fuera de orden
(el `extra Get_unk__Q23atn10Reader_unkFv` de LoadManager_unk ya estaba en `main`).

## Cambios

1. **`CXReadUncompLZ` con tres argumentos** (`atn/LoadManager_unk`): la del SDK es
   `CXReadUncompLZ(CXUncompContextLZ*, const void* data, u32 len)`. Con el tercer argumento
   (`slot->mLength`) `fn_8046BBF0` pasa de 99,95 % a **100 %** (el registro r5 que faltaba).
   Devuelve `int` (s32 del SDK). `cmp.py` OK y `mnemcheck.py` sin avisos.
2. **`Decoder_unk` con los campos de `CXUncompContextLZ`** (`destp`, `destCount`, `forceDestCount`,
   `length`, `lengthFlg`/`flags`/`flagIndex`/`headerSize`/`exFormat`): misma talla (0x18) en la
   Wii; en PC el puntero crece y la estructura sigue siendo la del SDK de Aurora.
3. **Un solo `PfsArchive_unk`/`PfsEntry_unk`** (regla 10): `atn/LoadManager_unk.cpp` tenía una
   copia local; ahora incluye `atn/PfsArchive_unk.h`. Renombrados en la cabecera: `PfsEntry_unk`
   `m08`/`m0C` -> `mSize`/`mPackedSize`, `PfsArchive_unk::m24` -> `mEntry` (entrada DVD del fichero
   de datos; `FileSystem_unk` la escribe con `DVDConvertPathToEntrynum`).
4. **Una declaración de `fn_800E0000`** (`include/atn/WordWrap_unk.h`, con `MeasureChar_unk`): los
   llamadores la declaraban con los argumentos en tres órdenes distintos. En la Wii (y en SysV
   x86-64) los enteros y los floats van en registros separados y da igual; en Windows x64 los
   argumentos van por posición y el port pasaba mal los argumentos. Neutro para MWCC en GameUI,
   MenuCommonTask, D2ACommonDialog y StrapTask.
   - `atn/D2A_Message` **se queda con su declaración**: con la común (y el callback con tipo
     `fn_801F1418`) `vf18` pasa de 92,51 a 92,50 en `report.json` (objdiff-cli da 93,74 al
     símbolo, pero manda el report). En el port va con `#if TARGET_PC`.
5. **Vista `StringView_unk` de `atn/FileList` con `__msl_size_t`**: las palabras de
   `std::string` son `__msl_size_t` (propuestas2); con `unsigned long` la vista leía mal las cadenas
   largas en Windows (LLP64: `unsigned long` de 4 bytes). Era la causa de que el port de Windows no
   encontrara los `.gly` de las fuentes. Typedef de `unsigned long` en la Wii: neutro.

6. **`fn_8006D7B0` (texto con la fuente) con la firma de su llamador**: `atn/DrawHelper` la declara
   con la vista como segundo argumento, y el ensamblador lee el texto de r5. La definición de
   `atn/Font_unk` no la tenía: en PC tomaba la vista por el texto y los menús salían sin letras.
   Objdiff de la función 74,018 -> 74,024 %; sin bajadas.

## Decisiones sin el usuario

- D2A_Message fuera de la propuesta (ver 4).
- No se lleva nada con `BE()`/`PTR32()` ni lo que va con `#if TARGET_PC` en el port.
