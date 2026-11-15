# enlazar-20261115, cola C (motor atn a una funcion)

Inicio 13:22 CET (reinicios del servidor a las 13:43 y ~14:08).

## Resultado por unidad

- `atn/SequenceUnitParam1_const_int`: **enlaza**. Funciones 34/37 -> 37/37. Los tres `Regist_unk<SequenceUnitParam1<...>>`
  (0x8057344C, 0x80573B14, 0x80574174) al 100 % con un nivel de inline mas alrededor de `push_back`
  (`SequenceHolder::Add_unk`). El `.dol` fallaba con `matching: true` por el orden de las copias debiles
  (ver Hallazgos); arreglado moviendo el registro de BootTask a una inline al principio y el ctor de
  `SequenceBoot_unk` detras de `Create_unk`.
- `atn/LH_ColliTree` (y las 11 copias de `Loader::Get_unk<T>`: LH_ColliObject, LH_MotionCommandData, LH_Scene,
  LH_Texture, LH_Xsca, LH_Efp...): sigue al 99,7 %. Mejora parcial no commiteada (ver Pendientes).
- `atn/D2A_Frame vf18`, `atn/Texture_unk fn_800D5A3C`, `atn/LightUtil_unk fn_8007A154`: probadas unas 5
  variantes cada una sin 100 % (detalles en NONMATCHING con motivo). Sin cambios en el fuente.

## Unidades que enlazan (bytes)

- `atn/SequenceUnitParam1_const_int`: 7164 bytes de datos (`measures.total_data`). Build limpio: `main.dol: OK`, complete_data 965894 -> 973058, complete_units 670 -> 671; poolcheck y poolorder limpios.

## NONMATCHING con motivo

- `Loader::Get_unk<T>` (include/atn/LoadManager.h): con la busqueda como inline `void Find_unk(T*& out, hash)`
  (resultado por parametro de salida, `out = core; return;` dentro del `if`) el recurso encontrado va directo
  a r30 y se conserva el `bne+b` del original: quedan 4 instrucciones (fin de lista r7/m48 r0 frente a r0/r6
  en el bucle de find_if). Probado sin exito para el bucle: predicado por valor, `const&`, swap de operandos,
  helper de comparacion, `last` con nombre / por referencia, comparar `mNode`, `while` con `end()` en la
  condicion (materializa un bool), find_if sobre el contenedor, `for` con `break` (bucle rotado), otro nivel
  de inline para find_if, `const_iterator`.
- `D2A_Frame::vf18` (99,85 %): ToU32, color con 4 floats (A queda en f3 como el original, pero Z/R cruzados y
  la etiqueta del 0.0f cambia), alpha con nombre, copia antes del test, ColorToU8_unk: no.
- `CsvBase_char_124_13_34 fn_802078C4` (99,48 %): con la fila en una `static inline ReadRow_unk(entry, csv, field)` el
  puntero de fila pasa a r24 como el original, pero el desplazamiento y la cadena de niveles/contador quedan
  r26/r27/r28 (original r27/r26/r26); probadas las 120 permutaciones de las declaraciones (minimo 9
  instrucciones distintas), `i`/`levels` declarados arriba, otra inline para la columna de niveles y sin `entry`
  local. Sin cambio en el fuente.
- `D2A_Manager __sinit` (98,55 %): copia de `D2A_IconUV_unk` con un nivel de inline mas (miembro o estatica):
  peor (cambia la planificacion entera). Sin cambio.
- `LH_Handle fn_80473FCC`: solo revisada (sin variantes nuevas).
- `fn_800D5A3C` (Texture_unk): accesores GetWidth/GetHeight empeoran; sin cambio.
- `LightList_unk::fn_8007A154`: con `size()`/`begin()`/`end()` aparece la relectura de mSize pero src/dst cruzan
  (ya documentado); sin cambio.

## Cambios de splits

Ninguno.

## Para el port

- `include/atn/SequenceHolder.h`: nueva inline plantilla `template <class T> void Add_unk(T* unit)` (push_back
  de mUnits); `Regist_unk<T>` la usa. Sin cambio de layout ni firmas.
- `include/atn/SequenceBoot_unk.h`: nueva declaracion `void RegistBoot_unk(const char*)` (inline en
  atn/SequenceUnitParam1_const_int.cpp). El ctor de SequenceBoot_unk se movio detras de fn_80572BA0 (mismo cuerpo).

## Decisiones tomadas sin el usuario

- `Add_unk` como plantilla y no como funcion normal: como funcion normal, cada unidad que incluye
  SequenceHolder.h instanciaba `vector<SequenceUnitBase*>::push_back` (copia debil nueva) y el `.dol` dejaba de
  salir identico. Re-emparejadas en lote (pdbatch) SequenceHolder, GameSystem_unk, Bullet y
  SystemErrorMessage_unk (renumeradas por el parametro con nombre); en Bullet dos etiquetas de `.sdata2`
  (0x80881640, 0x80881648) corregidas a mano porque pdbatch las dejo cruzadas.

## Hallazgos

- Una inline no usada de una cabecera que llama a una plantilla (`mUnits.push_back(x)`) la instancia en todas las
  unidades que la incluyen; como miembro plantilla no.
- `Regist_unk<T>` de SequenceHolder: un nivel de inline mas alrededor de `push_back` de punteros invierte los
  huecos de pila del crecimiento (n a 0x10) como el original.
- Orden de las copias debiles con `-ipa file`: una inline definida al principio del fichero que construye un
  `boost::function` emite alli invoker/manager (y su typeid va primero en `.rodata`), pero las plantillas de
  funcion que usa (ctor<F>) se emiten tras la primera funcion emitida que las usa.
- Un ctor no inline definido detras de su unico llamador (que lo inlinea) no se emite ni arrastra copias debiles
  a su posicion; definido delante, se emite con sus copias debiles detras (el enlazador quita el ctor, no ellas).
- Inline con un literal: va a `@STRING@`; para dejarlo en `@stringBase0`, pasar el literal desde la funcion no inline.
- `Get_unk<T>`: el resultado de una inline con dos salidas se une con r3 (la llamada); con un parametro de salida
  y `out = x; return;` se une con la variable del llamador y se conserva el `bne+b`.

## Pendientes

- `Loader::Get_unk<T>` (12 copias, ~8 unidades LH_*): mejora parcial en `include/atn/LoadManager.h`
  (parche `docs/enlazar-20261115-qC-getunk.patch`: `Find_unk(T*& out, hash)` con `out = (T*)core; return;`), 7 -> 4 instrucciones distintas;
  no commiteado (no llega al 100 %). Falta el registro del fin de lista en el bucle de find_if.
- El mismo problema de orden de copias debiles puede bloquear otras unidades con todo al 100 %: mirar el orden
  de las funciones W del `.o` frente a symbols.txt cuando `main.dol` falle.
