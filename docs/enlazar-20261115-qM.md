# enlazar-20261115, cola M: deuda de la regla 10 de `atn/D2A_TaskInstanceBase_64_8`

## Resultado por unidad

- `atn/D2A_TaskInstanceBase_64_8`: 51/51 -> 51/51 al 100 %. Los 14 `extern "C"` del grupo `<32, 8>`
  (`fn_801F64D0`..`fn_801F8598`) son ahora miembros de `atn::D2A_TaskInstanceBase<N, M>` y
  `symbols.txt` tiene su nombre mangled. **Sigue sin enlazar** (`matching: false`, ver Pendientes).
- 28 llamadores pasados a llamadas de miembro, todos sin bajadas (precommit: lost 0, fuzzy drops 0,
  labelcheck/rep100 igual, `main.dol: OK`, 15578 funciones como la referencia).
- `game/CosEditPalette_unk`: 3 funciones que bajaban (0x801D7560, 0x801D77A4, 0x801D7AC0) vuelven al
  100 % con `SetPos_unk(name, inst, rect)` (el nombre primero) y un `atn::D2A_DrawRect rect` local en los
  bucles; `fn_801D6C1C` sube de 91,07 a 92,13 %.

Commit: `025ca04` (codigo + `symbols.txt`).

## Unidades que enlazan (bytes)

Ninguna. Con `matching: true` el `.dol` no sale identico (ver Pendientes).

## NONMATCHING con motivo

Sin cambios.

## Cambios de splits

Ninguno. `symbols.txt`: 15 renombrados de funciones (los 14 `fn_801F...` y `SetValues2_unk<64,8>`, que
cambia de parametro) y ~950 `@N` re-emparejadas en 51 unidades (las que incluyen las cabeceras tocadas).

## Para el port

- `include/atn/D2A_TaskInstanceC.h`: ya no declara `extern "C"`; solo `typedef atn::D2A_TaskInstanceBase<32, 8>
  D2A_TaskInstance32_unk`. Las 14 funciones son miembros: `fn_801F64D0` -> `Create`, `fn_801F69A8` ->
  `Update`, `fn_801F6A38` -> `Draw`, `fn_801F6C10` -> `GetData`, `fn_801F6C2C` -> `GetEndFrame`,
  `fn_801F6C38` -> `GetTrack_unk`, `fn_801F6C80` -> `SetValue0_unk`, `fn_801F6D7C` -> `SetValue1_unk`,
  `fn_801F6E78` -> `SetValues1_unk`, `fn_801F7590` -> `SetValues2_unk`, `fn_801F791C` -> `SetValue3_unk`,
  `fn_801F7DF0` -> `SetColor_unk(name, r, g, b)`, `fn_801F837C` -> `SetText_unk`, `fn_801F8598` ->
  `SetNumber_unk`.
- `D2A_TaskInstanceBase::SetValues1_unk(const char*, D2A_DrawRect r)` -> `(const char*, D2A_DrawRect& r)`
  (const& da 99,28 % en el llamado: adelanta la carga; por valor obligaria a copiar en los llamadores que
  pasan un puntero).
- `D2A_TaskInstanceBase::SetValues2_unk(const char*, D2A_DrawColor c)` -> `(const char*, atn::Vector4_unk uv)`
  (por valor; la copia de los llamadores es la de `Vector4_unk`, `__Vec4Copy`). Afecta tambien a `<64, 8>`
  (GameUI: `atn::Color(...)` -> `atn::Vector4_unk(...)`, y el `uv` de `fn_803E0E10` es `atn::Vector4_unk`).
- `SetEnd_unk`/`IsEnd_unk` son inline en la clase (antes especializaciones de `<32, 8>` en `D2A_TaskInstanceC.h`).
- `D2A_TaskInstance.h` incluye `atn/Vector4_unk.h`.
- Vistas UV locales -> `typedef atn::Vector4_unk`: `ShopUV_unk` (ShopBase, ShopBuySell, ShopColor, ShopIdentify,
  ShopUpGrade), `NewCampItemUV_unk`, `MenuUV_unk`, `CosEditUV_unk`, `CosEditListUV_unk` (campos `x, y, z, w`
  en vez de `u0, v0, u1, v1`).
- Las vistas del objeto (`ShopLayout_unk`, `MenuLayout_unk`, `ResultLayout_unk`, `GameLayout_unk`,
  `D2AInstance_unk`, `atn::Task*`...) siguen; se convierten con `(D2A_TaskInstance32_unk*)` en cada llamada.
  Unificarlas con el tipo real queda pendiente (regla 10, "un objeto, un tipo").
- `fn_801F6A38(task, 1)` de `D3AnimeInstance::Draw` pasa a `Draw()` sin argumento (el original no lo pasa;
  la funcion sigue en 99,03 %).

## Decisiones tomadas sin el usuario

- Tipos de parametro de `SetValues1_unk` (referencia no const) y `SetValues2_unk` (`atn::Vector4_unk` por valor)
  elegidos por medicion: son los unicos que dejan al 100 % el llamado y todos los llamadores.
- Re-emparejado de `@N` con un script propio (`/home/claude/scratch-M/relbl.py`): compila el fuente de HEAD
  (`git archive` a `scratch-M/base`) y mapea cada `@N` del `.o` viejo al nuevo por (seccion, offset), y
  renombra en `symbols.txt` solo dentro de los rangos de datos de la unidad. `pdbatch.sh` no basta: con
  constantes repetidas (`0.0f`) `pairdata --relabel` las da por ambiguas.
- Los llamadores con vistas propias usan casts a `D2A_TaskInstance32_unk*` (no se cambia el tipo de los
  miembros de otras clases para no tocar layouts del port).

## Hallazgos (una linea cada uno)

- `template class X<N, M>;` emite todos los miembros (tambien inline y los no usados) y el enlazador de este
  proyecto **no** los quita: `.text` crece 0x4320; no vale para `D2A_TaskInstanceBase` (corrige lo de qI).
- Un parametro `const T&` hace que MWCC adelante las cargas del objeto (no hay alias); `T&` y `T` por valor no.
- Pasar un struct por valor a traves de un inline con el parametro por valor no equivale a pasarlo directo:
  la direccion de la copia se iza a un registro en los bucles.
- `objdiff-cli diff` (fnstat) y `report.json` no dan los mismos porcentajes: comparar siempre report con report
  (precommit), no fnstat con el `report.json` de referencia.
- Tras tocar `symbols.txt`, `configure.py` no re-parte: hace falta `ninja build/SLSP01/config.json` (dtk split)
  antes de medir con objdiff.

## Pendientes

- **Enlazar `atn/D2A_TaskInstanceBase_64_8`**: con `matching: true` (lista de miembros) `.text` sale 0x240 mas
  largo y desordenado:
  1. Las copias debiles `Text_unk::Text_unk()`/`~Text_unk()` de cada grupo salen justo despues de `Create`
     (en el original van al final del grupo: 0x801F6450, 0x801F8914, 0x801F9474).
  2. `SetFloat_unk<8, 2>` (0x230) se queda en el `.dol`; en el original no esta, pero su `L"%f\0"` si esta en
     el pool de cadenas anchas. El enlazador de este proyecto no quita funciones sin referencias (ni debiles).
  Con `template class` el orden de los grupos sale bien pero se quedan ~58 funciones de mas. Falta una forma
  de que MWCC emita las copias de `Text_unk` al final de cada grupo sin instanciar la clase entera, y de dejar
  el literal `%f` en el pool sin emitir `SetFloat_unk`.
- Unificar las vistas de la instancia en los llamadores (`ShopLayout_unk`, `MenuLayout_unk`, `ResultLayout_unk`,
  `GameLayout_unk`, `atn::D2AInstance_unk`, `CosEditLayout_unk` ya es typedef) con `D2A_TaskInstance32_unk`.
