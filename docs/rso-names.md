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
- Resultado: 8180 / 18215 igual que `main`, `main.dol: OK`, lost 0, fuzzdrop 0.

## Decisiones tomadas sin el usuario

- `CharaTask::IsPlayer_unk` → `IsPlayer() const` **revertido**: el `const` baja el fuzzy de `Em303KrakenFoot::fn_802E3EE8` (99,16 → 98,97). El nombre real queda en la tabla de pendientes.
- `std::ios_base::throw_failure` (0x8068B39C) no se renombra: `allocator_char.cpp` lo declara como función libre C++ con otro nombre mangled; hay que declararlo en su clase al trabajar la unidad.
- Los nombres que sustituyen clases `_unk` (p. ej. `GhoulKeyboard_unk` = `atn::Keyboard`, `StringBuf_unk` = `std::basic_stringbuf`, `Vector_unk` = `atn::SimpleVector`, `EfpHandle_unk` = `atn::EfpFile`, `FaceKey_unk` = `CharaFaceControl::MotionPart`, `NewCampStatusParty_unk::GetStatus_unk` = `Inventory::GetPlayerStatusInfo`) o virtuales `vfXX` (`CharaTask::vfC8` = `GetTacViewPosition() const`, `GimmickCommon::vf28` = `Start`, `CsvBase::vf08` = `SetData(const char*, unsigned int)`) se dejan para cuando se trabaje cada unidad (regla 10: clase real en su cabecera, comprobar que nada baja).
- No usar (código plegado de funciones distintas): `DWCi_IsError` ≠ `Tools_CharaSelectIsOpened`, `gpiProfilesTableCompare` ≠ `std::operator-`, `nw4r::ut::LinkListImpl` ≠ `nw4hbm::ut::LinkListImpl`, `EqualsResName` ≠ `EqualsPaneName`, `__dt__CharWriter::ColorMapping` ≠ `__dt__CharWriter`, `LoadHandle::operator=` ≠ `Copy`.

## Pendientes: nombres reales de símbolos usados en el fuente

Para usar al trabajar cada unidad (declarar el método en su clase real y renombrar en `symbols.txt`).

| Unidad | Dirección | Nombre actual | Nombre del RSO |
|---|---|---|---|
| `?` | `0x8007708c` | `fn_8007708C` | `CheckKeyState__Q23atn8KeyboardCFRCQ33atn8Keyboard8KeyStateUs` |
| `?` | `0x8068b39c` | `fn_8068B39C` | `throw_failure__Q23std8ios_baseFPCc` |
| `AI/AI_archer_attack01.cpp` | `0x8003e120` | `fn_8003E120` | `balance_insert__Q23std66__red_black_tree<PQ23std39__tree_node_base<Q23std13allocator<Pv>>>FPQ23std39__tree_node_base<Q23std13allocator<Pv>>PQ23std39__tree_node_base<Q23std13allocator<Pv>>` |
| `Event/Puppeteer_CharaTask.cpp` | `0x8000d430` | `fn_8000D430` | `CalcMatrixStack__Q23atn5ModelFQ25boost103function<FPQ23atn9Matrix3x4RCQ23atn9NodeChunkRCQ23atn24ModelNodeCallbackContext_v,Q23std12allocator<v>>` |
| `Event/Puppeteer_CharaTask.cpp` | `0x80013f78` | `__replace_unk__Q23std59basic_string<c,Q23std14char_traits<c>,Q23std12allocator<c>>FUlUlPCcPCcQ23std26random_access_iterator_tag` | `do_replace__Q23std59basic_string<c,Q23std14char_traits<c>,Q23std12allocator<c>>FUlUlPCcPCcQ23std26random_access_iterator_tag` |
| `atn/EfpHandle_unk.cpp` | `0x8023781c` | `fn_8023781C__Q23atn13EfpHandle_unkFv` | `Destroy__Q23atn7EfpFileFv` |
| `atn/FileList.cpp` | `0x8006af38` | `fn_8006AF38` | `__pl<c,Q23std14char_traits<c>,Q23std12allocator<c>>__3stdFPCcRCQ23std59basic_string<c,Q23std14char_traits<c>,Q23std12allocator<c>>_Q23std59basic_string<c,Q23std14char_traits<c>,Q23std12allocator<c>>` |
| `atn/FileList.cpp` | `0x8006bb6c` | `fn_8006BB6C` | `FileFreeBuffer__3atnFPcb` |
| `atn/Font_unk.cpp` | `0x8006d008` | `__pl__3stdFRCQ23std59basic_string<c,Q23std14char_traits<c>,Q23std12allocator<c>>PCc` | `__pl<c,Q23std14char_traits<c>,Q23std12allocator<c>>__3stdFRCQ23std59basic_string<c,Q23std14char_traits<c>,Q23std12allocator<c>>PCc_Q23std59basic_string<c,Q23std14char_traits<c>,Q23std12allocator<c>>` |
| `atn/Model.cpp` | `0x800902c0` | `fn_800902C0` | `CalcMatrixStack__Q23atn9ModelBaseFPCQ23atn12MotionBufferQ25boost103function<FPQ23atn9Matrix3x4RCQ23atn9NodeChunkRCQ23atn24ModelNodeCallbackContext_v,Q23std12allocator<v>>` |
| `atn/SmallAllocator_8.cpp` | `0x800827e0` | `fn_800827E0` | `Instance__Q23atn13MemoryManagerFv` |
| `atn/SmallAllocator_8.cpp` | `0x80083ad4` | `fn_80083AD4` | `Free__Q23atn13MemoryManagerFPv` |
| `atn/WordWrap_unk.cpp` | `0x800e0a64` | `fn_800E0A64` | `setstate__Q23std8ios_baseFQ33std8ios_base7iostate` |
| `auto_03_803B27AC_text.cpp` | `0x803b27dc` | `fn_803B27DC` | `@76@60@__dt__Q23std65basic_stringstream<c,Q23std14char_traits<c>,Q23std12allocator<c>>Fv` |
| `auto_03_80690D60_text.cpp` | `0x80690d60` | `fn_80690D60` | `showmanyc__Q210Metrowerks37bufferedbuf<c,Q23std14char_traits<c>>Fv` |
| `game/AreaEffConfig.cpp` | `0x800420f4` | `vf08__Q23atn19CsvBase<c,44,13,34>FPCci` | `SetData__Q23atn19CsvBase<c,44,13,34>FPCcUi` |
| `game/Arms.cpp` | `0x8004476c` | `vf08__Q23atn18CsvBase<c,9,13,34>FPCci` | `SetData__Q23atn18CsvBase<c,9,13,34>FPCcUi` |
| `game/BattleCharaTask.cpp` | `0x800ef2f0` | `vfC8__9CharaTaskFv` | `GetTacViewPosition__9CharaTaskCFv` |
| `game/BattleEfp_unk.cpp` | `0x800ef73c` | `__dt__Q23atn13EfpHandle_unkFv` | `__dt__Q23atn7EfpFileFv` |
| `game/BattleTask.cpp` | `0x800f7f80` | `IsPlayer_unk__9CharaTaskFv` | `IsPlayer__9CharaTaskCFv` |
| `game/CharaFaceControl.cpp` | `0x8011d320` | `__dt__11FaceKey_unkFv` | `__dt__Q216CharaFaceControl10MotionPartFv` |
| `game/CharaTask.cpp` | `0x80139550` | `fn_80139550__9Model_unkFi` | `GetMotionFrame__Q23atn5ModelCFi` |
| `game/CharaTask.cpp` | `0x80139f3c` | `fn_80139F3C__9Model_unkFif` | `SetMotionSpeed__Q23atn5ModelFif` |
| `game/CharaTask.cpp` | `0x80151168` | `HasFlags_unk__FRCiUi` | `Check__Q32db9SkillBase9ConditionCFi` |
| `game/Em001Necromancer.cpp` | `0x80244cac` | `fn_80244CAC` | `FreezeTask__Q23atn4TaskFv` |
| `game/Em203GreaterDaemonRedMp.cpp` | `0x802a4968` | `fn_802A4968` | `Press__Q23atn8KeyboardCFQ33atn8Keyboard4PortUs` |
| `game/EnemyQuarkSecond.cpp` | `0x803396ec` | `fn_803396EC` | `SetFlag__9CharaTaskFi` |
| `game/EnemyTasha.cpp` | `0x80357f38` | `fn_80357F38` | `SetMotionFrame__Q23atn5ModelFif` |
| `game/GameEventScript_cont.cpp` | `0x803940e8` | `fn_803940E8` | `__dt__Q23std40basic_iostream<c,Q23std14char_traits<c>>Fv` |
| `game/GameEventScript_cont.cpp` | `0x80394190` | `fn_80394190` | `__dt__Q23std39basic_istream<c,Q23std14char_traits<c>>Fv` |
| `game/GameEventScript_cont.cpp` | `0x80394208` | `fn_80394208` | `__dt__Q23std65basic_stringstream<c,Q23std14char_traits<c>,Q23std12allocator<c>>Fv` |
| `game/GimmickCommon.cpp` | `0x803edf5c` | `vf28__13GimmickCommonFv` | `Start__13GimmickCommonFv` |
| `game/MotionCommand_unk.cpp` | `0x801207bc` | `__vc__Q23atn32Vector_unk<18MotionCmdEntry_unk>FUi` | `__vc__Q23atn47SimpleVector<Q33atn17MotionCommandData7Command>Fi` |
| `game/MotionCommand_unk.cpp` | `0x80120f4c` | `__vc__Q23atn32Vector_unk<18MotionCmdElemD_unk>FUi` | `__vc__Q23atn64SimpleVector<Q23atn42static_string<32,c,Q23std14char_traits<c>>>Fi` |
| `game/NewCampPartsStatus.cpp` | `0x805282a8` | `GetStatus_unk__22NewCampStatusParty_unkFi` | `GetPlayerStatusInfo__9InventoryFi` |
| `std/allocator_char.cpp` | `0x800e0b18` | `fn_800E0B18` | `__dt__Q23std39basic_ostream<c,Q23std14char_traits<c>>Fv` |
| `std/allocator_char.cpp` | `0x800e0b90` | `fn_800E0B90` | `__dt__Q23std62basic_stringbuf<c,Q23std14char_traits<c>,Q23std12allocator<c>>Fv` |
| `std/allocator_char.cpp` | `0x800e0c20` | `dtor_800E0C20` | `__dt__Q23std41basic_streambuf<c,Q23std14char_traits<c>>Fv` |
| `std/allocator_char.cpp` | `0x800e14a4` | `fn_800E14A4` | `__ct__Q33std39basic_ostream<c,Q23std14char_traits<c>>6sentryFRQ23std39basic_ostream<c,Q23std14char_traits<c>>` |
| `std/allocator_char.cpp` | `0x800e1630` | `fn_800E1630` | `__dt__Q33std39basic_ostream<c,Q23std14char_traits<c>>6sentryFv` |
| `std/allocator_char.cpp` | `0x800e1a48` | `fn_800E1A48` | `flush__Q23std39basic_ostream<c,Q23std14char_traits<c>>Fv` |
| `std/allocator_char.cpp` | `0x800e1d4c` | `fn_800E1D4C` | `sync__Q23std41basic_streambuf<c,Q23std14char_traits<c>>Fv` |
| `std/allocator_char.cpp` | `0x800e1d54` | `fn_800E1D54` | `overflow__Q23std41basic_streambuf<c,Q23std14char_traits<c>>Fi` |
| `std/allocator_char.cpp` | `0x800e1d5c` | `fn_800E1D5C` | `getloc__Q23std8ios_baseCFv` |
| `std/allocator_char.cpp` | `0x800e1d88` | `underflow__13StringBuf_unkFv` | `underflow__Q23std62basic_stringbuf<c,Q23std14char_traits<c>,Q23std12allocator<c>>Fv` |
| `std/allocator_char.cpp` | `0x800e1de4` | `pbackfail__13StringBuf_unkFi` | `pbackfail__Q23std62basic_stringbuf<c,Q23std14char_traits<c>,Q23std12allocator<c>>Fi` |
| `std/allocator_char.cpp` | `0x800e1e64` | `overflow__13StringBuf_unkFi` | `overflow__Q23std62basic_stringbuf<c,Q23std14char_traits<c>,Q23std12allocator<c>>Fi` |
| `std/allocator_char.cpp` | `0x800e2064` | `fn_800E2064` | `seekoff__Q23std62basic_stringbuf<c,Q23std14char_traits<c>,Q23std12allocator<c>>FxQ33std8ios_base7seekdirQ33std8ios_base8openmode` |
| `std/allocator_char.cpp` | `0x800e22e8` | `fn_800E22E8` | `seekpos__Q23std62basic_stringbuf<c,Q23std14char_traits<c>,Q23std12allocator<c>>FQ23std7fpos<i>Q33std8ios_base8openmode` |
| `std/allocator_char.cpp` | `0x800e24cc` | `fn_800E24CC` | `imbue__Q23std41basic_streambuf<c,Q23std14char_traits<c>>FRCQ23std6locale` |
| `std/allocator_char.cpp` | `0x800e24d0` | `fn_800E24D0` | `setbuf__Q23std41basic_streambuf<c,Q23std14char_traits<c>>FPcl` |
| `std/allocator_char.cpp` | `0x800e24d4` | `fn_800E24D4` | `seekoff__Q23std41basic_streambuf<c,Q23std14char_traits<c>>FxQ33std8ios_base7seekdirQ33std8ios_base8openmode` |
| `std/allocator_char.cpp` | `0x800e24ec` | `fn_800E24EC` | `seekpos__Q23std41basic_streambuf<c,Q23std14char_traits<c>>FQ23std7fpos<i>Q33std8ios_base8openmode` |
| `std/allocator_char.cpp` | `0x800e2504` | `fn_800E2504` | `showmanyc__Q23std41basic_streambuf<c,Q23std14char_traits<c>>Fv` |
| `std/allocator_char.cpp` | `0x800e250c` | `fn_800E250C` | `xsgetn__Q23std41basic_streambuf<c,Q23std14char_traits<c>>FPcl` |
| `std/allocator_char.cpp` | `0x800e2608` | `fn_800E2608` | `underflow__Q23std41basic_streambuf<c,Q23std14char_traits<c>>Fv` |
| `std/allocator_char.cpp` | `0x800e2610` | `fn_800E2610` | `uflow__Q23std41basic_streambuf<c,Q23std14char_traits<c>>Fv` |
| `std/allocator_char.cpp` | `0x800e2668` | `fn_800E2668` | `pbackfail__Q23std41basic_streambuf<c,Q23std14char_traits<c>>Fi` |
| `std/allocator_char.cpp` | `0x800e2670` | `fn_800E2670` | `xsputn__Q23std41basic_streambuf<c,Q23std14char_traits<c>>FPCcl` |
| `std/runtime_error.cpp` | `0x8001a34c` | `get_deleter__Q43std3tr16detail57shared_ptr_deleter<c,Q33std6detail20default_delete<A0_c>>FRCQ23std9type_info` | `get_deleter__Q43std3tr16detail57shared_ptr_deleter<c,Q33std6detail20default_delete<A0_c>>CFRCQ23std9type_info` |

## Diccionario

Las 948 importaciones y las ~4700 exportaciones de función del RSO (las ~760 importaciones sin dirección EU solo se usan desde código de herramientas) siguen valiendo como diccionario de nombres reales y firmas mangled: 198 clases que no teníamos con nombre (`Inventory`, `GameCamera`, `CharaSightControl`, `GameSaveData`, `atn::Pad`, `atn::Scene`, `atn::XmlElement`...). Lista completa: `docs/lastworld_tools_symbols.txt` del repo USA, o `strings` del RSO.
