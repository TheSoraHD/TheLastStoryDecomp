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

## Grupo 2 (0x8004AE84-0x8007708C)

Pista general: los ficheros reales del motor se llaman `atn_<nombre>.cpp` (cadenas `atn_graphics_device.cpp`, `atn_nand.cpp`,
`atn_video_thp.cpp` en el .dol) y van en orden alfabetico: atn_audio_device < atn_bgm < atn_camera < atn_colli* < atn_curve (SinusCurve)
< atn_draw_helper < (f...) < atn_fade < atn_file_list < atn_font < atn_geometry < atn_graphics_device < atn_lens_flare.

- `atn/Bgm_cont.cpp` (nuevo, `_cont`) 0x8004AE84-0x8004B290: metodos de atn::Bgm (GameBgm::fn_8037D274(Bgm*) llama a fn_8004AE84);
  .sdata2 propio 0x80880890 (0.0f/1.0f) distinto del de la camara (1.0f/0.0f en 0x80880898) => corte en 0x8004B290.
- `atn/Camera_unk.cpp` (nuevo, inventado) 0x8004B290-0x8004C9B4: Camera_unk (ctor/dtor/fn_8004B378), .sdata2 0x80880898-0x808808D0.
- `atn/ColliBaseFunc_unk.cpp` (nuevo, inventado) 0x8004C9B4-0x8004F45C: virtuales de atn::ColliBase (fn_8004CF34..fn_8004D310, en la vtable
  0x807776A0) y fn_8004CB70/fn_8004CBB0 (los usan los ctor/dtor de ColliBase/ColliObject/ColliTree), test de rayo fn_8004D388/fn_8004ED34.
  -1.0f en 0x808808D0 (la camara tiene su -1.0f en 0x808808A4) y .rodata 0x80731038-0x80731048 (double int->float + objeto).
- `atn/Frustum_unk.cpp` (nuevo, inventado) 0x8004F45C-0x80050900: planos (cross/normalize/dot) llamados desde Camera_unk; 0.0f propio 0x80880934.
- `atn/ColliTest_unk.cpp` (nuevo, inventado) 0x80050900-0x80056DB8: tests de interseccion; 0.0f/1.0f/-1.0f propios en 0x80880940; .rodata 0x80731048.
- `atn/ColliCalc_unk.cpp` (nuevo, inventado) 0x80059784-0x8005C248: funciones llamadas desde atn_colli (fn_80058120->fn_80059B5C,
  fn_80058C20->fn_8005AAD4), 0.0f propio en 0x80880980 (atn_colli tiene el suyo en 0x80880960). Al final, funciones debiles de
  plantilla atn::CsvBase<...> (fn_8005B3CC..fn_8005BFCC) y dos funciones pequenas (fn_8005C1F8, fn_8005C220) sin datos: se quedan en esta unidad.
  .sbss 0x8087EEA0, .bss 0x807C6BC8-0x807C6C08.
- `atn/ComDrawTexture` ampliada a 0x8005DED8-0x80068DB0: es el resto del fichero de atn::DrawHelper (enlazada): comparte el 0.0f/1.0f
  0x808809A0 con DrawHelper::fn_8005DD60 y la pool 0x80731340 ("/common/sys_white.texture"...). Incluye las 7 clases DrawHelper::ComDraw*
  y sus dtors debiles al final (0x80068BF0-0x80068DB0). Datos: .rodata 0x80731170-0x80731390 (nombres RTTI + pool), .data 0x807777D8-0x80777B08
  (vtables/RTTI ComDraw*, ComDrawLine, DrawCommand). 0x80731140-0x80731160 (tabla y doubles) y .sdata2 0x808809A0 quedan en auto porque
  van antes que DrawHelper o los usa DrawHelper.
- `atn/FileUtil_unk.cpp` (nuevo, inventado) 0x80068DB0-0x8006A09C: compresion de color (fn_80068DB0..), carga de ficheros ("LoadError: ",
  "dvdroot") e init fn_80069E50 (llamada desde main). Sin pruebas para partirlo mas: se deja entero. .rodata 0x80731390-0x80731408, .sdata2 0x808809E8-0x80880A08.
- `atn/FileList` ampliada (fuente, no enlazada) a 0x8006A950-0x8006C6A0: lo anunciaba su propio fuente (pool 0x8073148C usada por
  0x8006AC08-0x8006C3F0). Datos: .rodata 0x80731460-0x80731510, .data 0x80777BC0-0x80777BF8, .sbss 0x8087EEC0-0x8087EEC8, .sdata2 0x80880A18-0x80880A38
  ("_jp".."_xx"). symbols.txt: lbl_8073148C -> @stringBase0 (scope:local data:string_table).
- `atn/Font_unk.cpp` (nuevo, inventado) 0x8006C6A0-0x8006FAB0: "font_%02d_%02d%s_jp.texture", "[ConvertUnicode]...", "cp932_unicode.tbl".
  .rodata 0x80731510-0x807316A0, .sdata 0x8087D730-0x8087D738, .sdata2 0x80880A38-0x80880A58 (0.0f propio; el siguiente en 0x80880A5C).
- `atn/Geometry_unk.cpp` (nuevo, inventado) 0x8006FAB0-0x80070E14: AABB_unk::Merge_unk y afines; .rodata 0x807316A0-0x807316B8, .sdata2 0x80880A58-0x80880A70.
  Corte final dudoso: fn_80070E14/fn_80071088 no usan datos (fn_80071088 solo la llama graphics_device); puse el corte tras el ultimo usuario de sus constantes.
- `atn/GraphicsDevice.cpp` (nuevo; nombre real `atn_graphics_device.cpp` por la cadena de su pool) 0x80070E14-0x8007708C, `__sinit` 0x80077058
  (.ctors 0x8072D2B0) construye TextureCacheManager8x32/4x128 en 0x807C6C08/0x807C6DB4. .rodata 0x807316B8-0x80731838, .data 0x80777C18-0x80777D08
  (jumptables), .sdata 0x8087D738-0x8087D740, .sbss 0x8087EED8-0x8087EEF0, .bss 0x807C6C08-0x807C7028, .sdata2 0x80880A70-0x80880AD0.
- Sin tocar: 0x8007708C-0x80077160 (una funcion de mascara de botones llamada desde MindowsCommon, entre el `__sinit` de graphics_device y LensFlare):
  podria ser fichero propio o el inicio de LensFlare; sin pruebas.
- Decisiones: `atn/Bgm_cont.cpp` (fusionar con Bgm al decompilar); nombres inventados arriba; renombrado @stringBase0 de FileList.

## Grupo 3 (0x80077160-0x800A5B40)

- `atn/LensFlare` (fuente, no enlazada): .sdata ampliada a 0x8087D740-0x8087D760 (6 floats usados solo por fn_80077548).
- `atn/Light_unk.cpp` (nuevo, inventado) 0x80079044-0x80079ECC: Light_unk (ctor) + funciones con 0.0f/1.0f propios (.sdata2 0x80880B00-0x80880B40),
  .rodata 0x807318A0-0x807318F0.
- `atn/LightUtil_unk.cpp` (nuevo, inventado) 0x80079ECC-0x8007A530: 1e16f/0.0f propios en 0x80880B40, .rodata 0x807318F0. fn_8007A4CC (sin datos) va aqui sin prueba.
- `atn/LipSync_unk.cpp` (nuevo, inventado) 0x8007A530-0x8007A994: cadena ancha "AIUEO" y Chara1154_unk (ctor/fn_8007A7BC), usada por
  Event::Dialog; 0.0f/1.0f propios en 0x80880B48. .rodata 0x807318F8-0x80731910, .sdata 0x8087D760-0x8087D768.
- `atn/MaterialParser_unk.cpp` (nuevo, inventado) 0x8007A994-0x80081DC8: parser de materiales ("Shader", "MatColor", "Opacity", "TexColor1"...),
  .rodata 0x80731910-0x80732048, .data 0x80777D40-0x80778550 (tablas + 14 jumptables), .sdata 0x8087D768-0x8087D798, .sdata2 0x80880B70-0x80880BC8.
  Los 0.0f repetidos 0x80880B90/94/98 son un mismo array const (no candidatos de corte). fn_80081D00 (sin datos) va aqui.
- `atn/Math_unk.cpp` (nuevo, inventado) 0x80081DC8-0x80081E54: solo el `__sinit` (.ctors 0x8072D2B4) que rellena las constantes
  globales 0x807C7028-0x807C7090 (vector cero, identidades...) con su propio 0.0f/1.0f (0x80880BC8). Nombre real probable `atn_math.cpp`.
- `atn/MindowsCommon` (inferida) ampliada: .rodata 0x80732140-0x80732248 y .data 0x80778610-0x80778840 (vtables/RTTI MindowsFolder y
  MindowsRadian, tabla 0x807321F0): solo los usan funciones de su .text.
- `atn/Model.cpp` (nuevo) 0x800899DC-0x8009A5CC: atn::Model / atn::ModelBase (vtables 0x80778888/0x807788D0, nombres RTTI y pool 0x80732368).
  .rodata 0x80732248-0x80732638, .data 0x80778840-0x80778930, .sdata 0x8087D798-0x8087D818, .sbss 0x8087EF18-0x8087EF28, .bss 0x807C7268,
  .sdata2 0x80880BF8-0x80880C38. Las 4 primeras funciones (0x800899DC-0x80089C28, sin datos) van aqui sin prueba (podrian ser de MindowsCommon).
- `atn/BoneCalc_unk.cpp` (nuevo, inventado) 0x8009A5CC-0x8009B93C: calculo recursivo de matrices de huesos (fn_8009A5CC/fn_8009ADA4,
  usado por ModelPose_unk y PBoneBasic); 0.0f/40.74f propios en 0x80880C38 (Model tiene 0.0f en 0x80880BF8). .data 0x80778930-0x80778958.
- `atn/ModelDraw_unk.cpp` (nuevo, inventado) 0x8009B93C-0x8009E5A0: dibujo GX (GXLoadPosMtxImm..., "common/mip64.texture"...);
  fn_8009E448 la llama fn_8009B93C => fn_8009E1E4/fn_8009E448 son del mismo fichero. .rodata 0x80732638-0x807326B8, .sdata 0x8087D818,
  .sbss 0x8087EF28-0x8087EF40, .bss 0x807C7270-0x807C73A0, .sdata2 0x80880C48-0x80880C78.
- `atn/Motion_unk.cpp` (nuevo, inventado) 0x8009F70C-0x800A2978: LH_Motion (ctor y metodos), MotionPose_unk, MotionCmdData_unk y `__sinit`
  0x800A291C (.ctors 0x8072D2B8). Empieza tras ModelInstance porque el double int->float 0x807326D0 va despues del nombre RTTI de ModelInstance.
  No hay pruebas para separar 0x8009F70C-0x800A03A0 (lo llaman gimmicks) del resto: se deja junto. .rodata 0x807326D0-0x807327A0,
  .data 0x80778990-0x807789D8, .sbss 0x8087EF48-0x8087EF68, .bss 0x807C73A0-0x807C7400, .sdata2 0x80880C88-0x80880CA0.
- `atn/NandManager` (inferida) ampliada a 0x800A2978-0x800A5508: 12 funciones de 0x3C solo llamadas desde NandManager; .rodata hasta 0x80732898
  (pool 0x807327B4), .data 0x807789D8-0x80778AD8 (2 jumptables).
- `atn/Pad_unk.cpp` (nuevo, inventado) 0x800A5508-0x800A5B40: tabla de botones 0x80732898-0x80732988 y 0.0f propio 0x80880CB0
  (NandManager tiene el suyo en 0x80880CA0). Incluye fn_800A5B30/fn_800A5B38 (8 bytes, sin datos) antes de ParseTree.

## Grupo 4 (0x800A981C-0x800C3610)

- 0x800A981C-0x800B1B28 -> 8 unidades `atn/PostFx<Clase>.cpp` (Bloom, ColorMatrix, Damage, Dof, Gamma, GodRay, MotionBlur, Rain), en orden
  alfabetico. Cada una con su double int->float, su nombre RTTI y su vtable/RTTI en .data (0x80778CE0-0x80778EF8, 0x40 por clase);
  cortes en los ctors salvo fn_800ACFC8 (va con Gamma: usa su primer .sdata2 0x80880DC8) y fn_800AB774 (va con Damage: .rodata 0x80732B88).
  PostFxBloom incluye el RTTI debil de atn::IPostFx.
- `atn/LayerOcclusionCPU` (fuente, no enlazada) ampliada como ya decia su nota: .text hasta 0x800B66A4 (funciones de debug + `__sinit`
  0x800B6684, .ctors 0x8072D2BC), .rodata desde 0x80732E28, .data desde 0x807790A8, .bss 0x807C74A0-0x807C75A0, .sdata 0x8087D888-0x8087D8F8.
- `atn/LayerShadow` (fuente, no enlazada): .rodata hasta 0x80732F00 (cadena "Shadow" que usa su ctor).
- `atn/RenderSetting_unk.cpp` (nuevo, inventado) 0x800B95C4-0x800BC468: menu de debug de render ("Render", "VFilter", "ScreenShot", "Gamma"...,
  pool 0x80732F00), 0.0f/1.0f propios 0x80880FB8.
- `atn/RenderSystem_unk.cpp` (nuevo, inventado) 0x800BC468-0x800C16C0: crea las capas (Layer2D3D, ZPrePass, Prerender...), fn_800BDBC8 llamada
  desde main; 0.0f propio 0x80881088. .rodata 0x807336A8-0x807337A8, .data 0x807791E0-0x807792C0, .sdata 0x8087D8F8-0x8087D918, .sbss 0x8087EFB8.
- `atn/CharaLightData_unk.cpp` (nuevo, inventado) 0x800C16C0-0x800C3094: CharaLightData_unk, dtors debiles de LH_Scene/NewCampScene2_unk;
  0.0f propio 0x808810D0, .rodata 0x807337A8-0x807337F0.
- `atn/Se_unk.cpp` (nuevo, inventado) 0x800C3094-0x800C3610: envoltorios de sonido (atn::SoundRef, dispositivo 0x8087EE90), 1.0f/0.0f propios
  0x80881138. El corte 0x800C3094 se apoya en el contenido (todo sonido a partir de ahi), no en datos.

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
