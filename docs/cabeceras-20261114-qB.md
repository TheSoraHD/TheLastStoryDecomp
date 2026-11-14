# cabeceras-20261114 - cola B: `std/string.h` y `std/string_members.h`

Agente B. Inicio 14:00. Referencia: `$REF` = `/home/claude/ref-B` (15424 / 18109, labelcheck 46 lineas, rep100 140).

## Resultado por punto de la cola

1. **`operator+(const basic_string&, const basic_string&)`** (0x805ADB28, wchar_t, game/TacView): 62 % -> **100 %**.
   Plantilla de espacio de nombres en `std/string.h` (junto a las otras dos `operator+`), declarada `friend` en la
   clase (sin nombres de parametro). Lee datos y tamano de cada cadena con una sola prueba; el orden de los
   registros sale declarando `lsz, rsz, lp, rp` en ese orden antes de las dos pruebas. Fuera la copia de
   `src/game/TacView.cpp`. La declaracion `friend` renumera las `@N` de todas las unidades que incluyen la cabecera
   (aun sin nombres de parametro).
2. **Accesores `is_long`**: no era la cabecera. El original vuelve a probar el bit largo en cada acceso porque lee
   el nombre por una inline que devuelve una referencia (`CommonData::GetName_unk()`, nueva en
   `include/Event/CommonData.h`): con una referencia devuelta por una inline MWCC no reutiliza la prueba entre
   accesos (con el miembro directo, o con `d[i].m08`, si). Resultados:
   - `Event::Sequence::fn_80541CFC`: 88,25 -> **100 %** (ademas `buf[n++] = c`: el contador queda junto al
     puntero reducido; `int value = atoi(buf); if (value == num)` para el orden del `cmpw`). mnemcheck limpio.
   - `Event::Manager::fn_80488E00`: 93,4 -> 94,6 % (queda el registro guardado de mas del bucle de acciones).
   - `Event::ActionVoicePlay::vf0C`: 97,4 -> 98,6 %.
   - `ActionSoundPlay::vf0C`: sin cambio (98,97 %), se deja con `m08`.
   - Probado y descartado: `is_long()` por el campo de bits, accesores con `if`/ternario largo primero, iterador:
     MWCC sigue guardando el booleano. `volatile` en `is_long()` reproduce el patron, pero rompe los sitios donde el
     original si comparte la prueba (`LoadMotion::LoadMotion`, `c_str()`/`size()` en la misma expresion).
3. **`do_replace`** (0x80013F78, Event/Puppeteer_CharaTask): 90,3 -> **99,3 %** con los dos cambios de la cola P
   (`size_type tsz = tmp.size();` antes de la llamada recursiva; `j1 = tmp.mFlags.mIsLong ? ... : ...`).
   NONMATCHING: el original vuelve a restar `j2 - j1` para el `memmove` de la rama que reserva (un registro guardado
   mas). Una inline con los punteros por referencia da la resta pero parte `cap` en dos rangos (97,3 %); con
   `__dist_unk` para `n2`, 3 formas del bucle de crecimiento y 5 firmas del helper: no sale. Ademas los offsets de
   las cadenas difieren (7 cadenas sin referencias en el pool de Puppeteer_CharaTask).
4. **`basic_string::reserve`** (0x80013DC4): sigue al 99,9 %: solo difiere el offset de su cadena (las mismas 7
   cadenas sin referencias del pool, de una funcion quitada por el enlazador). Sin codigo del que sacarlas: no se toca.

## Re-emparejado

- Las tres cabeceras (`std/string.h`, `std/string_members.h`, `Event/CommonData.h`) cambian 431 objetos (todas las
  unidades que incluyen `std/string.h`). `pdbatch.sh` sobre las 431.
- `@enum$6816ControlAppear_cpp` -> `@enum$6819ControlAppear_cpp` en `symbols.txt` (8 lineas de chr/ControlAppear;
  si otra cola cambia ese numero, hay que volver a ajustarlo tras el merge).
- Devueltos por linea a su nombre de `HEAD` (pairdata los habia cambiado): los 8 vigilados (`lbl_8087D6E8/EC`,
  `lbl_8087EE78`, `lbl_80885CB0`, `lbl_80783CA8`, `lbl_8078CB00/08/10`) y 17 renombrados a nombres globales que
  rompian el enlace: `sPath` (0x80885C98) y `__msl_ios_init_unk`/`__msl_wios_init_unk` (0x8087F3D0/D4) daban
  "multiply-defined"; `@STRING@__allocate__...`, `@STRING@Alloc/Reserve/Resize_unk...`, `sCrcTable_unk`, `sLwKeys`,
  `sSeLogName_unk` encogian el `.dol` (main.dol: FAILED, el enlazador funde las copias).
- `lblmap.py` en las 41 unidades que quedaban peor en labelcheck/rep100; despues 26 etiquetas `@N` duplicadas a
  menos de 0x40 bytes (restos de la numeracion vieja) renombradas al numero que falta en la secuencia (script de una
  vez: si los vecinos son N-1 y N+1, la duplicada pasa a N; 0x80885924 -> `@22395` y 0x80759748 -> `@18056` a mano).
  Tocan unidades de otras colas (BattleTask, GameCamera_unk, PacketPack, GameUI, GameTask...): solo `symbols.txt`.
- Resultado: `main.dol: OK`, 15424 -> **15426** en `report.json`, lost 0, fuzzdrop 0, labelcheck 46 -> 36 lineas,
  rep100 140 -> 140. poolcheck: los avisos de Event/Manager y Event/Sequence ya estaban en `HEAD`.
- Funciones al 0 %: sin cambios (ninguna de la cola estaba al 0 %).

## Decisiones tomadas sin el usuario

- `operator+` de dos cadenas en `std/string.h` (antes en TacView.cpp) aunque renumere: la cola lo pedia.
- `GetName_unk()` como miembro inline de `Event::CommonData` (nombre inventado) en vez de un helper libre en cada .cpp
  (regla 10): las unidades que incluyen CommonData.h ya se renumeraban por `std/string.h`.

## Para el port

- `include/std/string.h`: nueva plantilla `std::operator+(const basic_string&, const basic_string&)` (friend); la
  copia de `src/game/TacView.cpp` desaparece (TacView: `fn_805ADB28` ya no esta en el .cpp).
- `include/Event/CommonData.h`: nuevo `const std::string& GetName_unk() const { return m08; }` (sin cambio de layout).
  Lo usan `Sequence::fn_80541CFC`, `Manager::fn_80488E00` y `ActionVoicePlay::vf0C` (antes `->m08`).
- `Sequence::fn_80541CFC`: el bucle de digitos ahora es `buf[n++] = ...` (antes `*p++`), misma logica.
- `std/string_members.h` `do_replace`: `tmp.size()` antes de la llamada y `j1` por el bit largo (misma logica).

## Hallazgos

- Una inline que devuelve una referencia a un miembro (`const std::string& GetName_unk()`) impide que MWCC comparta
  entre sentencias la prueba de las inlines llamadas sobre ella (cada acceso recarga la palabra): Sequence 0x80541CFC.
- `buf[n++] = c` deja vivo el contador `n` junto al puntero reducido (`*p++` con `n++` aparte lo elimina): 0x80541CFC.
- Una declaracion `friend` de plantilla en una clase de cabecera renumera las `@N` aunque no tenga nombres de
  parametro.
