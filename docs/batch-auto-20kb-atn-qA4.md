# batch-auto-20kb-atn qA4: atn (dibujo de modelos, Motion, NAND)

Cola: `atn/ModelDraw_unk`, `atn/Motion_unk`, `atn/NandManager`.
Las tres unidades quedan `matching: false` (no todas sus funciones al 100%); `main.dol: OK`, `lost: 0`
contra main y contra la integracion, sin bajadas de fuzzy.

## atn/ModelDraw_unk (commit 9d6da5b + dfc3b8d)
- Resultado: 1/14 al 100% (`fn_8009E448`), `matching: false`.
- Contenido: dibujo GX de las mallas de un modelo. `atn::Model` (fn_8008E818) construye en la pila un
  `DrawEntry_unk` (ctor 0x8009E1E4) y llama a fn_8009B93C; cada nodo visible dibuja sus partes con
  fn_8009D58C, que encola un comando en la camara (dos FixedArray de 0x1000: claves y comandos de 0x4C) o lo
  dibuja ya con fn_8009DE60 (canales, texturas, matrices indirectas, sombras, test de mipmaps).
- NONMATCHING:
  - `fn_8009BF3C` 35.93%: el color de material/ambiente se escala con codigo paired-single escrito a mano
    (mtspr GQR6, psq_l/psq_st cuantizados u8, ps_muls0): no se puede sin asm. Pendiente de aprobacion.
  - `fn_8009B93C` 98.37% (r4/r5 de la copia muerta de la matriz de la camara, familia __VecCopy),
    `fn_8009B9E8` 98.30%, `fn_8009CC28` 99.12%, `fn_8009D1A4` 99.84%, `fn_8009DE60` 97.48%,
    `fn_8009DCE0` 95.19%, `fn_8009C84C` 95.12%, `fn_8009C688` 92.89%, `fn_8009C508` 90.72%,
    `fn_8009BB9C` 82.64%: asignacion de registros (en el original las variables del cuerpo de los bucles
    reciben los registros mas altos y los parametros los mas bajos).
  - `fn_8009D58C` 83.57% (tests de flags y el test de capacidad de la cola con el idioma orc/srwi),
    ctor `DrawEntry_unk` 87.64% (orden de las copias del LightSet y del vector de escala).
- **Decisiones sin el usuario**
  - Splits: sin cambios.
  - `symbols.txt`: `fn_8009E1E4` -> `__ct__13DrawEntry_unkF...` (ctor; Model.s lo llama por nombre),
    nombres de datos: `sFogStart_unk` (0x8087D818, 0.8f), `sShadowRecv_unk`/`sSecondPass_unk`/`sLastMat_unk`/
    `sMipLoaded_unk` (.sbss 0x8087EF28-0x8087EF30), guardas y `@LOCAL@` de las 5 texturas estaticas de
    fn_8009D1A4, `@stringBase0` (0x80732648), constantes `.sdata2`/`.rodata` con sus `@NNNN` actuales.
    **Cambio mas alla de renombrar**: `lbl_807C7270` (0x40) partido en 5 registros de destructor `@42xx` de
    0xC (uno por textura estatica); `lbl_8087EF38` pasa de 8 a 1 byte (guarda).
  - Tipos inventados locales al fichero (vistas de estructuras ya conocidas): `ShaderMaterial_unk` (mismo
    layout que en atn/Shader_unk.cpp, con m08 como shader y los colores), `TexStage_unk` (0x18),
    `DrawShader_unk` (= ShaderTex_unk), `DrawDesc_unk` (= ShaderDesc_unk), `DrawScene_unk` (camara
    lbl_8087EFB4), `DrawParams_unk` (lbl_8087EFA8), `DrawLens_unk`, `DrawCmd_unk`, `DrawEntry_unk`...
  - `LightSet_unk` se redeclara en el fichero con `fn_8007A36C`/`fn_8007A41C` estaticas que devuelven el
    conjunto por valor: aqui se llaman sobre temporales sin construir (mismo simbolo que los miembros de
    include/atn/Light_unk.h). Probablemente son estaticas tambien en atn/LightUtil_unk (no tocado).
  - El bloque de color de fn_8009BF3C es una macro (`SET_MAT_COLOR_UNK`): el original lo tiene dos veces con
    una constante blanca propia cada una en .sdata2 (con una inline compartida sale una sola).
  - Las constantes por defecto de la transformacion UV (0x80880B88-0x80880B98, de MaterialParser_unk) se
    declaran como 5 `extern "C" const float`.
- Hallazgos MWCC
  - Estaticas de fichero con `= 0` explicito van en .sbss antes que las guardas de las estaticas locales.
  - Un agregado local (`GXColor white = {...}`) en una inline compartida da una sola constante en .sdata2.
- Pendientes: asm paired-single de fn_8009BF3C (necesita aprobacion); registros del resto.

## atn/NandManager (commit f5d20ee + dfc3b8d)
- Resultado: 32/34 al 100%, `matching: false`.
- Nombre real del fichero: `atn_nand.cpp` (cadena del pool). Clase `atn::NandManager` (Task, 0x82E0 bytes)
  en `include/atn/NandManager.h` (nuevo). Los llamadores externos siguen usando los `fn_` extern "C"
  (fn_800A48B8 guardar, fn_800A49C4 cargar, fn_800A4794/fn_800A4AD4 comprobar espacio, fn_800A4BE4 abrir,
  fn_800A4D00 seek+leer, fn_800A4E08 cerrar, fn_800A4F0C borrar, fn_800A5218 escritura sincrona...).
- NONMATCHING: `vf10` (0x800A2DE0, maquina de estados) 90.26% (el original guarda el puntero de mPath en un
  registro propio al construir el std::string: un registro salvado mas; offsets de los temporales);
  `fn_800A4244` 96.34% (un registro salvado de mas).
- **Decisiones sin el usuario**
  - Splits: anadida `.sbss 0x8087EF68-0x8087EF70` (instancia lbl_8087EF68, hueco sin unidad).
  - `symbols.txt`: callbacks 0x800A2978-0x800A2C0C -> `CheckDone_unk`...`MoveDone_unk` (static, locales),
    `fn_800A2D20` -> `__dt__Q23atn13NandEmpty_unkFv` (destructor vacio de un objeto local que usa
    Em001Necromancer; clase inventada), vf0C/vf10/fn_800A4244/fn_800A4494/fn_800A500C como miembros,
    `fn_800A54B0` -> `__dt__Q23atn11NandManagerFv`, `lbl_807327B4` -> `@stringBase0`.
  - Codigo que no esta en el DOL: `Stripped1_unk` (lectura sincrona inventada) para que "atn_nand.cpp" y
    "[NAND] Couldn't open a file." salgan en el pool tras "/" (el original la perdio en el enlazado); el ctor
    de NandManager y `OpenTmp_unk` se emiten fuera de linea (no inline: sus literales van a @stringBase0).
  - `TmpPath_unk` es una macro: el "/tmp/%s" esta en @stringBase0 (una inline lo manda a @STRING@).
  - `fn_800A4450` (texto de un pane como wstring) es una inline extern "C" con copia debil aqui.
  - Funciones NAND del SDK sin nombre declaradas con su nombre probable en comentario (NANDCheckAsync,
    NANDSafeOpenAsync, NANDGetType...).
- Hallazgos MWCC
  - `x == 2 || x == 3` no se convierte en rango; `(unsigned)(x - 2) <= 1` da `subi/cmplwi`.
  - Las ramas `if (err == 0) {...} else {error}` dejan el bloque de error al final; con `if (err) {...;break;}`
    al principio cambia la disposicion.
- Pendientes: los dos NONMATCHING.

## atn/Motion_unk (commit 8285e6f + dfc3b8d)
- Resultado: 11/30 al 100%, `matching: false`. 11 funciones sin escribir.
- Contenido: cache de modelos de mapa (vector estatico local; csv "FOLDER"/"MODEL"/"MODEL_LOD"), miembros
  fuera de linea de `atn::LH_Motion`, pose de un nodo de una motion (`fn_800A04A0`/`fn_800A0610`,
  euler->cuaternion inline), `MotionPose_unk`, `MotionCmdData_unk::fn_800A091C`, `__sinit` de <iostream>.
- Sin escribir: 0x800A0A68-0x800A1F84 (bancos de motions en `std::map` estaticos con clave de 64 bits: arbol
  de MSL no reconstruido) y 0x800A2664 (generador aleatorio de MP::CryptMP, boost::random mt19937 +
  uniform_real). Probablemente son otros ficheros, sin pruebas para cortar.
- NONMATCHING: `fn_8009F788` 99.82%, `fn_8009FE24` 98.95%, `fn_800A091C` 99.02% (orden de los incrementos
  del bucle, como LightUtil_unk), `fn_800A05AC` 97.25%, `fn_800A0610` 90.45%, `fn_800A005C` 84.11%, destructor
  debil de `std::vector<MapModel_unk*>` 96.77%.
- **Decisiones sin el usuario**
  - Splits: sin cambios.
  - `symbols.txt`: `fn_8009F70C` -> destructor del vector, `fn_800A291C` -> `__sinit_\Motion_unk_cpp`,
    guarda/`@LOCAL@` del vector estatico, `__msl_ios_init_unk`/`__msl_wios_init_unk`, registros de destructor
    `@3777`/`@3678`/`@3679`, `lbl_807326D8` -> `@stringBase0`.
  - `include/gmk/MapLift.h`: `MotionPose_unk::m10` pasa de `float[4]` a `atn::Quaternion_unk` (mismo layout).
  - `include/game/MotionCmdData_unk.h`: anadido `fn_800A091C`.
  - `MapModel_unk` (0x22C) inventado: LH_Binary como miembro (el original destruye sin llamada virtual).
  - `((void)0)` x10 en `fn_800A05AC`: el original la llama fuera de linea desde fn_800A09D0.
- Pendientes: las 11 funciones sin escribir y los NONMATCHING.
