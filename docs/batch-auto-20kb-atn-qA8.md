# batch-auto-20kb-atn qA8: Font_unk + MindowsCommon

Cola: `atn/Font_unk` (0x8006C6A0), `atn/MindowsCommon` (0x80084FD8).

| Unidad | 100% | matching |
|---|---|---|
| `atn/Font_unk` | 13/23 | false |
| `atn/MindowsCommon` | 50/57 | false |

Total de la cola: +62 funciones al 100% frente a la integracion (6260 -> 6322 en el report), `main.dol: OK`,
`lost: 0` contra main y la integracion, sin bajadas de fuzzy.

## atn/Font_unk (commits df434f0, 02fb0c6)
- Resultado: 13/23 al 100%, `matching: false` (fn_8006D7B0 es un borrador).
- Contenido: gestor de fuentes (singleton `lbl_8087EEC8`, 0x34AA4 bytes): texturas `[7][2][4]`, tablas de
  glifos "gly" (`FontInfo_unk`, 0x1C), `LH_Binary` de recarga, 64 quads de vertices pendientes (0x1260),
  tablas SJIS->unicode (0x3260) y unicode->SJIS (0x13260) de "cp932_unicode.tbl", matriz 3x4 (0x33268).
  Crear/destruir, nombres de textura, carga inicial y recarga de la fuente 1, busqueda binaria de glifo,
  anchura de texto/caracter, conversiones SJIS<->wide (char*, std::string, std::wstring) y el dibujo GX.
- NONMATCHING:
  - `fn_8006CABC` 93%: huecos de pila de los temporales `dir + "/" + name` y registros salvados.
  - `fn_8006D0C8` 93%: el original no guarda `&name` en un registro salvado (lo recalcula en cada uso).
  - `fn_8006D54C` 43%: planificacion de las conversiones de la cabecera "gly" (probadas 5 variantes).
  - `fn_8006D7B0` 53%: borrador (pasadas de sombra/contorno 4 y 8 direcciones, union de U+2015 repetidos,
    vaciado de 64 quads con GXBegin escrito como macro porque el original no llama a nada). Sin ajustar.
  - `fn_8006EF48` 90%, `fn_8006F2F0` 91%, `fn_8006F420` 96%: registros y forma de los bucles.
  - `fn_8006F17C` 99.7%: f3/f6 intercambiados (el `0.0f` va por un local `k`, si no peor).
  - `fn_8006F5E8` 85%: el original calcula `buf + strlen(buf)` con una copia (`mr r7,r6`); la version
    wchar_t (`fn_8006F548`) con el mismo codigo si casa.
  - `fn_8006F684` 67%: el original calcula ambas direcciones de tabla antes del test y deja codigo muerto
    tras el primer store; no encontrada la forma del if/else.
  - `__replace_unk<wchar_t>` 86%: el mismo bloqueo conocido que la version char (docs/msl_cpp.md).
- **Decisiones sin el usuario**
  - Splits: sin cambios (pendiente: `.sbss 0x8087EEC8-0x8087EED0` deberia ser de esta unidad; el hueco
    contiene ademas `lbl_8087EED0`, usado solo por GameConfig_unk).
  - `symbols.txt`: `fn_8006C8F8` -> `__ct__Q23atn8Font_unkFv`, `fn_8006CA14`/`fn_8006CA20` ->
    ctor/dtor de `atn::FontInfo_unk`. Las demas funciones conservan su `fn_` como `extern "C"` (las
    llaman ~37 ficheros asi).
  - `include/std/string.h`: anadido `operator+(const basic_string&, const T*)` como friend definido en la
    clase (la copia debil 0x8006D008 no lleva argumentos de plantilla en el nombre). Casa al 100% con dos
    `replace` encadenados.
  - Tipos inventados: `Font_unk`, `FontInfo_unk`, `FontGlyph_unk`, `FontGlyphHeader_unk`, `FontGlyphRef_unk`,
    `FontVertex_unk`, `FontCodePair_unk`. `FontGlyph_unk::m02-m06` son `short` (leidos con `lha`).
  - El ctor lleva 6 `((void)0)` (asserts de release): sin ellos se inlinea en fn_8006C7FC.
  - `fn_8006F684` lleva locales extra (hi/lo/uhi/ulo): sin ellos se autoinlinea en fn_8006CABC.
  - El fichero incluye `std/string_members.h` para la copia debil wchar_t del range replace (sus mensajes
    de error estan en el pool de este fichero).
- Hallazgos MWCC
  - Una cadena de dos `__replace_unk` sin `mr r3` entre ellas = llamadas encadenadas (`a.replace(..).replace(..)`).
- Pendientes: .sbss (arriba); el literal "cp932_unicode.tbl" del original es un objeto suelto en .rodata
  (0x80731510) apuntado por `lbl_8087D730` en .sdata: no comprobado como se emite aqui.

## atn/MindowsCommon (commit cc2e33c)
- Resultado: 50/57 al 100%, `matching: false`.
- Contenido: menu de depuracion con teclado USB: `atn::MindowsCommon` y subclases Int/UInt/Float/Bool/
  Flag/Radian/Folder (vf0C empezar edicion, vf10 aplicar texto, vf14 tecla/conmutar, vf18 dibujo, vf1C
  lineas, vf20 item por linea, vf24 quitar hijo, vf28 es carpeta, vf2C callback), creadores
  (`fn_800874C8` AddInt ... `fn_80088FAC` RGB, `fn_8008937C` AddFolder), creacion del menu
  (`fn_80089620`, `Mindows_unk` de 0xF4 con carpetas "Mindows Root"/"Mindows Temp") y borrado (`fn_800897D8`).
- NONMATCHING: los 7 `vf18` (dibujo) 96.6-99.2%: asignacion de FPR del calculo de `y` (16.0f en f5 y la
  constante int->float en f1 en el original) y orden de evaluacion de los argumentos de sprintf. Probadas
  4 formas de la expresion y un `const float`.
- **Decisiones sin el usuario**
  - Splits: anadido `.sbss 0x8087EF10-0x8087EF18` (`lbl_8087EF10`, la instancia del menu, en el hueco entre
    SmallAllocator_8 y Model; solo la define este fichero).
  - `symbols.txt`: metodos virtuales y dtors con nombre mangled (pairnames + correccion manual de la cola,
    donde pairnames emparejaba mal por el orden de las copias debiles); `vf1C` es `const` (CFv);
    `lbl_807321F0` -> `@stringBase0`. Los `Add*` conservan `fn_` (`extern "C"` con `MindowsFolder* self`)
    porque los llaman ~70 ficheros de gmk/game con ese nombre.
  - `vf18` recibe `const MindowsDrawCtx_unk*` (tipo inventado: x, y, ancho, primera/ultima linea, cursor,
    editando).
  - `Mindows_unk::Mindows_unk` definido fuera de la clase y no inline: sus literales van asi a
    `@stringBase0` como en el original; emite una copia fuera de linea sin llamadores (el enlazador la
    eliminaria). "DebugMenu" (ultimo literal del pool) no lo usa ninguna funcion conservada.
  - Bool y Flag pasan `realtime = 1` al ctor base (m4C): llaman al callback al conmutar.
- Hallazgos MWCC
  - `char* p = buf + strlen(buf); p[0] = c; p[1] = 0;` da `stbux`; con indice da `stbx + add`. Para borrar el
    ultimo, `p[-1] = 0` con puntero rematerializa el cero (`li r0,0`) tras la llamada.
- Pendientes: orden de emision de las copias debiles (vf28/vf0C/vf10 de Common detras de fn_80089620 y dtors
  Radian..Int al final): comprobar al enlazar si algun dia la unidad llega al 100%.
