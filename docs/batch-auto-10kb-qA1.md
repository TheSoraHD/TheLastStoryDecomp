# batch-auto-10kb qA1: atn (camara, frustum, PostFx, luces)

Cola: `atn/Camera_unk`, `atn/Frustum_unk`, `atn/PostFxGamma`, `atn/PostFxGodRay`, `atn/CharaLightData_unk`.

## atn/Camera_unk (commit a5d920c)
- Resultado: 2/4 al 100% (ctor, dtor), `matching: false`, `main.dol: OK`.
- NONMATCHING: `Camera_unk::fn_8004B378` 98.28% (orden en pila de los temporales de rot_z*rot_x*rot_y y de
  algunos temporales pequenos; FPR del caso con roll); `Camera_unk::fn_8004BF64` 99.94% (r3/r4 intercambiados en
  las dos copias de vector tras las rotaciones).
- **Decisiones sin el usuario**:
  - Splits sin cambios.
  - `symbols.txt`: `fn_8004BF64` -> `fn_8004BF64__10Camera_unkFff` (metodo `bool Camera_unk::fn_8004BF64(float, float)`,
    camara libre de depuracion movida con el pad; la llaman game/NewCampTask y game/NewTitleTask, en asm).
  - Cabecera nueva `include/atn/Camera_unk.h`: `Camera_unk` (0x1F4: tipo, proyeccion, pos/target/up/angulos, roll,
    limites de proyeccion, mView 0x58, mProj 0x88, near/far, mInvView 0xD0) y `Frustum_unk` (0xF4 en +0x100, metodos
    0x8004F50C/0x8004F5B8). Los `.cpp` que ya declaraban su propio `Camera_unk` local no se han tocado.
  - Cabecera compartida `include/math/Vec.h`: `Matrix44_unk::operator=` (copia de 8 paired singles). regress: lost 0.
  - Helpers inline inventados en el .cpp (LookAt_unk, Perspective_unk, Ortho_unk, Rotate_unk, RotZYX_unk, RotZXY_unk,
    SetView_unk, MultVec_unk, Direction_unk, Neg_unk, Cross_unk, CosF_unk/SinF_unk, RadToDeg_unk).
  - Nombre real propuesto del fichero: `atn_camera.cpp`.
- Hallazgos MWCC:
  - `A*B + C*D` se emite como `fmuls C*D` + `fmadds A,B`: el primer producto del fuente es el fmadds.
  - Pasar un `Vector3(...)` temporal construido con `-f()` (float devuelto) mete `frsp`; con locales `float` no.
  - Una matriz devuelta por valor por una inline y asignada (`m = F()`) va a la zona de locales de inline; con
    `T x = F()` (NRVO) va con los locales con nombre: sirve para ordenar matrices grandes en la pila.
  - Proyeccion escrita como `inline void F(Matrix44& out, ...) { Matrix44 m; ...; out = m; }` deja sus 0x40 al final de
    la zona de inline (como el original), devolviendola por valor no.
  - `Direction_unk(a, b)` inline con la resta dentro baja el temporal de la resta a la zona de inline.
- Pendientes para otras unidades: `lbl_8087EFB4` (camara global) es otra clase que contiene un `Camera_unk` en +0x104
  (GeoCamera_unk de atn/Geometry_unk: vista 0x15C = 0x104+0x58, proyeccion 0x18C = 0x104+0x88).
