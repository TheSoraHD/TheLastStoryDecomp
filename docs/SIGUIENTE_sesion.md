# Siguiente sesion (tras `nw4rmath-20261107`)

Rama `nw4rmath-20261107` (**sin mergear**, la revisa el usuario): **14053 / 18116** (13987 en `main` b0bb426, +66),
lost 0, **656 enlazadas**, NW4R 77/78, build limpio y clon limpio `main.dol: OK`. Notas: `docs/nw4rmath-20261107.md`
(incluye la lista de tramos `auto_` que quedan).

## EN CURSO: MSL C++ (rama `msl-iostream-20261107`, WIP, sin enlazar)

- Unidad nueva `PowerPC_EABI_Support/src/MSL_C++/iostream.cpp` = todo 0x8068B39C-0x8069533C (una sola TU con
  `__sinit` al final), extab/extabindex/datos; absorbe las 11 `auto_03_806*` (quitadas de `configure.py`). Escrita
  desde el asm. **43/170 al 100%**, lost 3 (do_unshift<wchar_t>, ctype<char>::do_toupper rango, dispose del
  deleter de `__locale_imp`). Flags: Wii/1.1, `-Cpp_exceptions on -O4,p -opt nostrength -inline on -ipa file`.
- Seguir con `docs/msl-iostream-20261107.md` (layouts, hallazgos de `shared_ptr` y del presupuesto de IPA, lista
  de pendientes). No mergear hasta lost 0.

## A revisar antes de mergear (`nw4rmath-20261107`)

1. Excepciones de asm nuevas en la regla 2: NW4R math (`SinCosFIdx`, `MTX34Zero`) y el `lwz 0(r1)` de `VPanic`.
2. `DbgPrintBase<T>::Append` con `__declspec(noinline)` (funcion debil de plantilla).
3. `FrameHeap::GetSize_unk` (0x8070DC10): nombre inventado, no esta en las versiones publicas.

## Siguiente objetivo propuesto

- **MSL C++**: es todo el codigo que queda en tramos `auto_` (~158 funciones en 0x8068B47C-0x80695270 + copias
  debiles en 0x80006720-0x80007B94 + sus datos de `.rodata`/`.data`/`.sdata`/`.sbss` tras MSL C). Ver
  `docs/msl_cpp.md`.
- `ut_TextWriterBase`: los 2 bytes de `.sbss` son del propio objeto (descartados el corte de math y objetos
  posteriores); unica unidad NW4R sin enlazar.

## De la sesion anterior (`maraton-20261107`, sigue vigente)

### Siguientes objetivos

1. Unificar vistas con las clases reales: `Ledge_unk` = `GameSceneRoute_unk`; `Field_unk`/`GameField_unk`/
   `DiveField_unk` = `GameTask`; las vistas de `lbl_8087EFA8`. Hacerlo en un lote y un solo `pdbatch.sh`.
2. GameTask 0x8036555C-0x8036823C (primeras funciones, con cadenas): desbloquea el `@stringBase0` de los getters
   de flags (`fn_80370174`, `fn_80370A78`, `fn_80370AE4`, usados por ~50 unidades).
3. Constructores de `TacView` (0x805A7158) y `BattleTask` (0x800F1430): traen sus copias debiles.
4. `GameCamera_unk` 11/90 y `Event/DebugInfo_unk` 1/58: muchas funciones pequenas pendientes.
5. Trucos baratos sobre NONMATCHING de floats: el `0.0f` de una comparacion en un local (`float zero = 0.0f;`) y
   el producto en un local antes de construir un `Vector3` (+6 en minutos; ver `trucos-mwcc.md`).
6. Bloqueos conocidos (DebugInfo_unk ctor, GameCamera::fn_80389678 y la familia r0/rX) en `docs/mwcc-blockers.md` : no repetir.

### Procedimiento que funciono

- Tras tocar `GameTask.h`/`BattleTask.h`: `ninja`, `labelcheck.py $(cat $REF/units.txt)` contra
  `$REF/labelcheck.txt`, `pdbatch.sh` con esas unidades + `game/AiTarget game/BattleTask`, `tools/decomp/fixpd.sh` (etiquetas
  `lbl_8087D6E8/EC`, `lbl_80775AA4` que pd renombra mal), y `precommit.sh` sin `--pd`.
