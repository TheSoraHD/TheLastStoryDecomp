# splits-auto, agente A (rama agent-A, rango .text 0x80004000-0x800F7250)

Solo particion de tramos `auto_` (sin decompilar). Cada grupo verificado con `main.dol: OK` y `regress.py` -> `lost: 0`.

## Grupo 1

### 0x800081C0-0x8000CBD0 -> `Event/EventCommandDef_unk.cpp` (nuevo, nombre inventado)
- fn_800081C0/fn_80008210 buscan en la tabla 0x80775208 (28 bytes x N, contador en .sdata 0x8087D6C0); fn_800082AC (0x4924) es el
  `__sinit` (en .ctors 0x8072D2A4) que rellena la tabla con hashes de nombres ("name", "frame_start", "actor.motion", "dialog.text"...).
- Datos exclusivos: .rodata 0x8072D480-0x8072F890 (pool de nombres de comandos/parametros de eventos), .data 0x807704A0-0x807759B0,
  .sdata 0x8087D6C0-0x8087D6C8, .sdata2 0x808805A0-0x80880668. Todo usado solo por estas 3 funciones.
- Las dos funciones de busqueda y el `__sinit` comparten tabla => mismo fichero.

### game/AiTarget ampliada: .text hasta 0x8001A268 (antes 0x800185B4)
- 0x800185B4-0x8001A268 (RouteSearch_unk, fn_80018608...) usa .sdata2 0x808806B8-0x80880760 continuo con el de AiTarget y
  .bss 0x807C66C0 (0x200). Sin constante duplicada que indique corte. Termina justo antes de std/runtime_error (enlazada).
- .bss 0x807C66C0-0x807C68C0 y .sdata2 desde 0x808806B8 (antes 0x808806C0; 0x808806B8 lo usa fn_80014564).

### 0x8001A444-0x8001BF44 -> `AI/AiCommon_unk.cpp` (nuevo, nombre inventado)
- Helpers de IA (fn_8001A444... fn_8001BEE0) llamados desde AI_archer_attack01 y el tramo 0x80035EB4.
- Corte en 0x8001BF44: double int->float propio 0x8072FF50 (+ "neck") frente al de AI_archer en 0x8072FF60; .sdata2 propio
  0x80880760-0x80880798 (100.0f, 0.0f...) frente a 0x80880798 de AI_archer; jumptable .data 0x80775BE8.

### AI/AI_archer_attack01 ampliada: .text 0x8001BF44-0x8003E918 (antes 0x80025DE0-0x80035EB4)
- Fichero de todos los AI::Script::* (65 clases): las vtables .data 0x80775C84-0x80777230 y RTTI/nombres .rodata ya eran de la unidad;
  los creadores (fn_8001E0A8..fn_8001E720, uno por clase), el registro fn_8001C284 y los metodos de las clases restantes (0x8001EE58...)
  estan en 0x8001BF44-0x80025DE0 y 0x80035EB4-0x8003E688. `__sinit` fn_8003E688 (.ctors 0x8072D2A8) registra el objeto global
  0x807C6B84 que usa fn_8001C284 => fin del fichero en 0x8003E6B8 (+ su dtor fn_8003E6B8..0x8003E918, weak).
- Constantes compartidas por todo el tramo: double 0x8072FF60 (252 refs en los tres trozos), .sdata2 0x80880798-0x808807C0.
- Datos ampliados: .ctors 0x8072D2A8, .rodata 0x8072FF60-0x80730C68 (incl. "AI::Script::CharaAi" y tabla 0x807307A0),
  .data 0x80775C48-0x807772B0 (jumptable, RTTI CharaAi, "Memory allocation failure"), .bss 0x807C68C0-0x807C6B90,
  .sdata 0x8087D6F0-0x8087D6F8, .sdata2 0x80880798-0x808807C0.

### 0x8003E918-0x8003EA3C -> `game/TargetPos_unk.cpp` (nuevo, nombre inventado)
- fn_8003E918 (init de un objetivo, 0.0f propio en .sdata2 0x808807C0 con relleno a 8) y fn_8003E950 (posicion del objetivo).
  El 0.0f del fichero siguiente esta en 0x808807D0 => ficheros distintos. Llamados desde AiTarget/TacView.

### 0x8003EA3C-0x80041A80 -> `game/DamageCalc_unk.cpp` (nuevo, nombre inventado)
- Calculo de dano/estado (llama a CharaStatus::fn_80133E24..., HitInfo). `__sinit` fn_80041A50 (.ctors 0x8072D2AC) inicializa
  0x807C6B90 que usan fn_8003F030/fn_8003F1E4 => fin del fichero. Incluye vfA8__9CharaTask (weak) y fn_80041A28.
- Datos: .rodata 0x80730C68-0x80730C78 (double int->float propio + objeto), .bss 0x807C6B90-0x807C6BC8, .sbss 0x8087EE70-0x8087EE78,
  .sdata2 0x808807C8-0x80880830 (1.0f propio frente al 1.0f de AreaEffConfig en 0x80880830).

### game/AreaEffConfig ampliada: .text hasta 0x8004318C (antes 0x80042E20)
- fn_80042E20..fn_80043130 solo se llaman desde fn_80041D44 (AreaEffConfig) y usan .sdata 0x8087D6F8 propio; atn::Task vf10/vf14 weak.
- Datos: .rodata 0x80730C78-0x80730D48 ("atn::Csv", tabla 0x80730CB4), .data 0x807772B0-0x80777358 (vtable/RTTI atn::Csv y
  dos "Memory allocation failure"), .sdata 0x8087D6F8-0x8087D700.

### atn/AudioDevice ampliada: .text 0x80046220-0x8004AD9C (antes 0x800463FC-0x80046EE0)
- 0x80046220-0x800463FC: fn_80046220/24 llamadas desde AudioDevice, fn_80046228/98 callbacks de DvdSoundArchive, fn_800463A8 llama al
  ctor. 0x80046EE0-0x8004AD9C usa la tabla de cadenas 0x80730F54 ("stream", "rsid.csv", "SoundError: LoadError Bank[%s]"...)
  igual que el ctor. Se pega a atn/Bgm (enlazada) sin hueco.
- Datos: .rodata 0x80730F38-0x80731028 (double, nombre RTTI, tabla de cadenas), .data 0x80777520-0x80777570, .sdata 0x8087D718-0x8087D720,
  .sdata2 0x80880870-0x80880890.

### 0x800C37FC-0x800C41D0 -> `atn/SequenceHolder.cpp` (nuevo)
- Clases atn::SequenceHolder::SequenceUnit / SequenceUnitBase (vtables 0x80779308/0x80779330); `__sinit` fn_800C41B4 (.ctors 0x8072D2C0)
  inicializa .sbss 0x8087EFC8-0x8087EFDC (usada por fn_800C3D74). Orden alfabetico: SequenceControl < SequenceHolder < Shader < SoftKeyboard.
- Datos: .rodata 0x80733810-0x80733878 (nombres RTTI + "vector length error"), .data 0x807792F8-0x80779348, .sbss 0x8087EFC8-0x8087EFE0.

### 0x800C41D0-0x800C6128 -> `atn/Shader_unk.cpp` (nuevo, nombre inventado)
- Carga de shaders ("Shader '%s' has invalid texture '%s'", tabla de 0x130 con "color.shader", "default.shader"...). Datos propios:
  .rodata 0x80733878-0x80733FF8, .data 0x80779348-0x807794A8, .bss 0x807C75A0-0x807C75B0, .sdata 0x8087D920-0x8087D930,
  .sdata2 0x80881140-0x80881158. Termina en fn_800C607C (cache de 4 shaders 0x807C75A0).

### atn/SoftKeyboardBase_wchar_t_0 ampliada: .text 0x800C6128-0x800C7DC4 (antes 0x800C63D8-0x800C6E60)
- 0x800C6128-0x800C63D8: helpers llamados solo desde SoftKeyboardBase::fn_800C65F0, boost::function0 weak (dummy/dtor) y el ctor
  fn_800C62B4 (escribe la vtable SoftKeyboardBase). 0x800C6E60-0x800C7DC4: funciones usadas por SoftKeyboardBase/SoftKeyboardU y
  boost::function invokers weak; ptmf 0x807794B4-0x807794D8. Se pega a atn/Sound3dListener (enlazada).
- Datos: .data 0x807794A8-0x807794F8 (4 ptmf + vtable/RTTI), .sdata2 0x80881158-0x80881160 (-1.0f propio).

### 0x800C8278-0x800CAD9C -> `atn/Sound3dListener_cont.cpp` (nuevo, `_cont`)
- Resto del fichero real de Sound3dListener (SoundCore/Sound3dActor: comparte 0.0f/1.0f de .sdata2 0x80881168/0x8088116C con la parte enlazada).
- Datos: .rodata 0x8073416C-0x80734188 ("SoundError: %s [%s]"), .data 0x80779570-0x807795C0 (cadenas SoundHandle.h), .sdata2 0x80881160-0x80881180.

## Decisiones tomadas sin el usuario
- Nombres inventados: `Event/EventCommandDef_unk.cpp`, `AI/AiCommon_unk.cpp`, `game/TargetPos_unk.cpp`, `game/DamageCalc_unk.cpp`, `atn/Shader_unk.cpp`.
- `atn/Sound3dListener_cont.cpp`: deberia fusionarse con atn/Sound3dListener al decompilar. dtk no deja que la parte `_cont` tenga
  .rodata ANTES que la unidad enlazada (ciclo de orden de enlace), asi que la tabla de errores de sonido 0x8073401C-0x80734130
  ("success", "low priority"... + tabla de punteros 0x807340F0 + doubles 0x80734120/28), que usan las funciones de `_cont`, queda en auto.
  Tampoco se le dan 0x80881168/0x8088116C (compartidos con la enlazada, ya referenciados por direccion en su fuente).
- AI/AI_archer_attack01 pasa a cubrir todo el fichero de scripts de IA (0x8001BF44-0x8003E918) en una sola unidad; el tramo
  0x80035EB4-0x8003E688 no se parte en ficheros por clase: comparte el double 0x8072FF60 y el .sdata2 con el resto.

## Hallazgos
- La lista .ctors de 0x8072D2A4 en adelante da el orden y el final de cada fichero con `__sinit` (0x800082AC, 0x8003E688, 0x80041A50,
  0x80077058, 0x80081DC8, 0x800A291C, 0x800B6684, 0x800C41B4, 0x800D5714, 0x800E29B4 en este rango).
- Una unidad `_cont` situada despues de una enlazada no puede tener datos en una seccion antes que los de la enlazada (dtk: "Cyclic dependency").
