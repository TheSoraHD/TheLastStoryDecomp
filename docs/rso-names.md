# rso-names: nombres del `LastWorld_tools.rso` de la versión USA

Rama `rso-names` (2026-10-29). Origen: repo `Aelbannan/last-story-decomp` (USA `SLSEXJ`, sin código decompilado; su `docs/lastworld_tools_symbols.txt` es el volcado de símbolos del RSO) y los RSO de las tres versiones (solo el USA trae el módulo completo; el EU, `ReleaseMaster`, son las entradas `Tools_*` vacías).

## El RSO USA

- `c:\programs\lw\trunk\lastworld\_bin\LastWorld_tools - Release\LastWorld_tools.plf`, 2,2 MB. `.text` 0xBB7F4 (2333 funciones exportadas), 11821 exportaciones, 948 importaciones (símbolos del `.dol`), relocalizaciones internas y externas completas.
- Sin DWARF: las secciones de depuración están vacías y solo quedan los nombres `.dwarf_*`. No hay layouts.
- Mismo código base que el `.dol` EU: de 263 funciones con el mismo nombre en ambos, 240 son idénticas byte a byte con relocalizaciones enmascaradas (el resto, porque el RSO pega funciones locales sin exportar detrás).
- Las herramientas de depuración (`Viewer*`, `LevelEditor`, `SimpleMenu`, `SoundTest`...) **no** están en el `.dol` EU.

## Herramientas (`tools/decomp/rso/`)

El RSO no se sube: `orig/SLSEXJ/LastWorld_tools.rso` (o `RSO_USA=<ruta>`). Salidas en `build/rso/`.

```bash
python3 tools/decomp/rso/vtmatch.py     # vtables RSO <-> EU (cadena RTTI o nombre ya conocido) -> nombre de cada hueco
python3 tools/decomp/rso/rsomatch.py    # funciones idénticas (únicas, >= 16 bytes) + propagación por bl/lis+addi reubicados
python3 tools/decomp/rso/classify.py    # same / apply / in_source / differs / tools_class
python3 tools/decomp/rso/apply.py [cat] [--dry]   # renombra en symbols.txt (por defecto: apply)
```

- `apply`: símbolos anónimos (`fn_`/`lbl_`/`dtor_`...) que no aparecen en `src/`, `include/` ni `libs/`: se renombran sin tocar código.
- `in_source`/`differs`: tocan fuente; revisar a mano (abajo).
- `tools_class`: el nombre lleva una clase que solo existe en las herramientas (78 identificadores): código idéntico de **otra** instancia de plantilla; no usar.
- `apply.py` descarta nombres `nw4hbm`/`homebutton`/`HBM` fuera de 0x805C0000–0x805E5000 (código idéntico de NW4R).

## Hecho en la rama

- 290 renombrados en `symbols.txt` (HBM/`nw4hbm`/`homebutton`, WPAD, `std` iostream/`num_put`/`numpunct`, `Metrowerks::bufferedbuf`, inlines de `atn::Camera`/`Model`/`Task`/`RenderConfig`, `CharaTask::GetProtector`/`SetFlag`...). Sin cambios de código.
- C: `HBMCreate`/`Delete`/`Init`/`Calc`/`CreateSound`/`DeleteSound`, `WPADProbe`, `WPADDisconnect`, `GXSetTexCoordCylWrap` (`symbols.txt` y sus llamadores `extern "C"` en `HomeButton_unk`, `WPad_unk`, `GraphicsDevice`).
- C++: `atn::Vector3::GetNormalized_unk` → `GetNormalize` (58 ficheros), `atn::FileList::GetName_unk` → `GetStrings` const (y `std::vector::operator[] const` en `include/std/vector.h`).
- C++: `CharaTask::IsPlayer_unk` → `IsPlayer() const`.
- Resultado: 8180 / 18215 igual que `main`, `main.dol: OK`, lost 0; fuzzdrop 1 aceptado (abajo).

## Decisiones tomadas sin el usuario

- `CharaTask::IsPlayer_unk` → `IsPlayer() const` (nombre real, decisión del usuario): baja el fuzzy de `Em303KrakenFoot::fn_802E3EE8` (NONMATCHING) de 99,16 a 98,97; aceptado, se arreglará al volver a esa función.
- `std::ios_base::throw_failure` (0x8068B39C) no se renombra: `allocator_char.cpp` lo declara como función libre C++ con otro nombre mangled; hay que declararlo en su clase al trabajar la unidad.
- Los nombres que sustituyen clases `_unk` (p. ej. `GhoulKeyboard_unk` = `atn::Keyboard`, `StringBuf_unk` = `std::basic_stringbuf`, `Vector_unk` = `atn::SimpleVector`, `EfpHandle_unk` = `atn::EfpFile`, `FaceKey_unk` = `CharaFaceControl::MotionPart`, `NewCampStatusParty_unk::GetStatus_unk` = `Inventory::GetPlayerStatusInfo`) o virtuales `vfXX` (`CharaTask::vfC8` = `GetTacViewPosition() const`, `GimmickCommon::vf28` = `Start`, `CsvBase::vf08` = `SetData(const char*, unsigned int)`) se dejan para cuando se trabaje cada unidad (regla 10: clase real en su cabecera, comprobar que nada baja).
- No usar (código plegado de funciones distintas): `DWCi_IsError` ≠ `Tools_CharaSelectIsOpened`, `gpiProfilesTableCompare` ≠ `std::operator-`, `nw4r::ut::LinkListImpl` ≠ `nw4hbm::ut::LinkListImpl`, `EqualsResName` ≠ `EqualsPaneName`, `__dt__CharWriter::ColorMapping` ≠ `__dt__CharWriter`, `LoadHandle::operator=` ≠ `Copy`.

## Segunda tanda (`rso-names-2`)

Los ~60 nombres reales de símbolos usados en el fuente, en tres colas: `docs/rso-names-qA.md` (std/MSL), `docs/rso-names-qB.md` (motor `atn`) y `docs/rso-names-qC.md` (clases del juego); cada nota tiene el detalle por nombre, las decisiones y los candidatos que no estaban en la cola.

- Clases reales: `atn::Keyboard`, `atn::Model` (+ `ModelBase`), `atn::MemoryManager`, `atn::EfpFile`, `atn::SimpleVector`, `atn::static_string`, `atn::MotionCommandData(::Command)`, `Inventory`, `db::SkillBase::Condition`, `CharaFaceControl::MotionPart`/`MotionSet`, `std::basic_stringbuf`/`basic_ostream`/`basic_streambuf`/`ios_base`/`locale` (`include/std/`).
- Métodos y virtuales: `GetTacViewPosition() const` (era `vfC8`), `GimmickCommon::Start` (`vf28`), `CsvBase::SetData` (`vf08`), `CharaTask::SetFlag`, `Task::FreezeTask`, `GetMotionFrame`/`SetMotionSpeed`/`SetMotionFrame`/`GetMotionSpeed`, `CalcMatrixStack`, `FileFreeBuffer`, `do_replace`, `operator+` de `basic_string`, `get_deleter`, `setstate`, `throw_failure`.
- Unidades (decisión del usuario): `atn/WordWrap_unk` + `std/char_traits_char` + `std/allocator_char` = un solo fichero (`atn/WordWrap_unk.cpp`, 25/44); los destructores de `basic_istream`/`basic_iostream`/`basic_stringstream` y sus 5 thunks con nombre real; los thunks pasan a `game/GameEventScript_cont2` (con las vtables) y `auto_03_803B27AC_text` pasa a `auto_03_803B27F8_text` (lost 2 aceptado: `fn_803B27D4`/`fn_803B27F0`).
- Bajada aceptada: `basic_ostream::sentry::sentry` (0x800E14A4, NONMATCHING) 57,35 → 35,05.
- Tras la tanda, `classify.py`: 771 de 784 nombres coinciden. Queda `std::__red_black_tree::balance_insert` (0x8003E120, AI_archer_attack01: necesita la cabecera `tree` de MSL y el `std::map` de scripts) y los emparejamientos que no se deben usar (arriba).

## Diccionario

Las 948 importaciones y las ~4700 exportaciones de función del RSO (las ~760 importaciones sin dirección EU solo se usan desde código de herramientas) siguen valiendo como diccionario de nombres reales y firmas mangled: 198 clases que no teníamos con nombre (`Inventory`, `GameCamera`, `CharaSightControl`, `GameSaveData`, `atn::Pad`, `atn::Scene`, `atn::XmlElement`...). Lista completa: `docs/lastworld_tools_symbols.txt` del repo USA, o `strings` del RSO.
