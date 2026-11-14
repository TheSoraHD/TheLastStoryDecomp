# Cola M (cabeceras-20261114): segundas pasadas en GameCamera_unk, FinishCamera_unk y Event (Sequence, Manager)

Inicio 20:06. Referencia: `/home/claude/ref-M` (15447 funciones).

## Resultados

- `atn/Camera_unk.h`: `Frustum_unk::mMin/mMax` -> `Frustum_unk::Bounds_unk mBounds` (struct con `mMin`, `mMax`;
  mismo offset 0x7C y tamano 0x18). La copia implicita de `atn::Camera` copia las dos esquinas a traves de un
  puntero (`addi r6, r4, 0x17c` + `psq_l 0x0/0xc(r6)`): solo sale si son un subobjeto. Suben al 100 %:
  `GameCamera::fn_80392D2C` (98,8), `RenderManager::SetCamera_unk` (copia en Event/Path, 99,96),
  `atn::Camera::Camera(const Camera&)` (copia en MiniMapTask, 98,13), `fn_8052C6CC` (NewCampTask, 99,96).
  `fn_80392A04` 98,01 -> 99,42 (quedan los registros r6-r9 de los punteros de esquinas y planos, en orden inverso).
  Renumera +2 `@N` en las 253 unidades que incluyen la cabecera (cualquier struct con miembros no triviales
  consume dos numeros; tambien `atn::AABB_unk` o un struct a nivel de espacio de nombres): re-emparejado en lote
  (`pdbatch.sh`) de las 216 no enlazadas, luego de las 37 enlazadas y las que seguian en labelcheck, y `lblmap.py`
  en las que quedaban; en Em206CocoonMp `lblmap` repitio `@18115` en tres direcciones: vuelto a "nombre de HEAD
  + 2" (el desplazamiento es uniforme: los `@N` posteriores al include suben 2). Devueltas por linea las
  etiquetas protegidas (`lbl_80783CA8`, `lbl_8087D6E8/EC`, `lbl_80885CB0`, que pairdata convertia en
  `@STRING@...`/`sInstance__9SetConfig` y rompian el enlace). Resultado: labelcheck y rep100 iguales a la
  referencia, 15447 -> 15452, lost 0.
- Bajadas aceptadas (fuzzdrop): `NewCampView_unk::fn_8052E1A0` 71,31 -> 70,81 y `fn_8052FA44` 73,22 -> 73,12;
  son las copias de `atn::Scene`/`atn::Camera` que el original llama (por encima del presupuesto) y aqui se
  inlinean (NONMATCHING estructural ya anotado); el cambio de la copia de la camara mueve su fuzzy.
- `Event::Sequence::fn_8053E8D0`: 99,95 -> 100 %. El lider del grupo no es una local: `lbl_8087F8A0->mHead != 0`
  dentro de la condicion y `&lbl_8087F8A0->mHead->mPos` en una local `const atn::Vector3*`.

- `GameCamera::fn_8038C5B0` 48,6 -> 99,6 %: el desplazamiento lateral y el objetivo van antes del primer `MIN`
  (el `MIN` cierra el bloque basico), `MIN(1.0f, x)` con los argumentos al reves, `Cross_unk(front, up) * side`
  (temporal del inline, no local con nombre), `VecAssign_unk(target, LerpV_unk(m818, target, t))` y un segundo
  `t2 = MIN(...)` para la fovy. Queda el registro del 50.0f y de la negacion de `front`. Etiquetas de la unidad
  re-emparejadas (`lblmap.py` desde las de HEAD; `pd.sh` y `lblfix.py` las dejaban peor).

## NONMATCHING (intentado, sin 100 %)

- `FinishCamera_unk::fn_80379104` (99,70): registros r26/r27 de la matriz y del vector girado; `Mtx rot` fuera
  del bucle: igual; `AddCurve_unk` escrito a mano: baja.
- `fn_80392A04` y `PostSequence::vf10`: registros de los punteros del bucle de copia de `mPlanes` (origen antes
  que destino en el original); `Plane_unk::operator=` explicito: igual.
- `GameCamera::fn_8038C5B0`: `-front`, vector construido en sitio y otro orden de `pos.y += 50`: igual o peor.

- `GameCamera::fn_80382A1C` (99,92 en report): el original llama a `fn_8013A13C` sobre un temporal sin
  constructor: `static Matrix3x4 fn_8013A13C(float)` que devuelve la matriz (ya lo dice qB2). Cambiar la firma
  toca CharaTask y unos 10 llamadores: no es neutro, no hecho.
- `Sequence::fn_8053F818` (99,93) y `atn/FileList` (`fn_8006AC08`, `fn_8006AD24`, `fn_8006B2D8`): misma causa,
  registros de `p` y `pos` cruzados en el `rfind` inline de `std/string.h`. Probado: `size_type i = pos` (dentro
  o fuera del `if`), `sz` antes que `p`, `__min`, bucle `do/while (q-- > p)`: todos peor o igual.
- `Sequence::fn_8053F1B4` (99,18): el original vuelve a leer `m98` tras guardar `m194 = 9` antes de borrar
  `m98_11`; probado `(m98 >> 20) & 1`, `m98 &= ~0x100000` y el orden de las dos asignaciones: igual o peor.
- `Sequence::fn_80541F18` (99,48): registros del id y del actor de `FindActor_unk` (r7/r8); asignacion en la
  condicion: igual.
- `Sequence::fn_8053E8D0` antes de acertar: local `leader` declarada fuera del `if` o al principio del bloque:
  igual; con `?:` baja.

- `GameCamera::fn_8038D4D4` (80,8): con `LerpV_unk` en las dos mezclas sube a 85,7 pero renumera los `@N` de
  la unidad (-4); no dejado. La pila del original pone `head` y `offset` por debajo de los temporales (como si
  fueran locales de un inline o de un bloque anidado): sin probar del todo.
- `GameCamera::fn_8038952C` (85,0): solo el orden del prologo (el original copia `chara` a r30 antes de sacar
  `&mMotion`); `GetNodeMtx_unk("hips")`, `GetModel_unk()`, `const_cast` a una local: igual o peor.
- `Manager::fn_8047EFD8` (99,76): registros del puntero y del indice del bucle de `m74`; `int i` fuera: igual.
- `Manager::fn_8048BD64/C884/D3A4`, `fn_80482400`, `Clear_unk`: bloqueos conocidos (recursion de `~Curve`,
  `push_back`, `std/vector.h`), no tocados.

## Cifras

- Referencia 15447 -> 15452 funciones al 100 % (lost 0); labelcheck y rep100 iguales a la referencia.
- GameCamera_unk 49 -> 50/90 al 100 % (fn_8038C5B0 48,6 -> 99,6), Sequence 84 -> 85/119, Manager 138/185 y
  FinishCamera_unk 10/12 sin cambios; al 0 %: sin cambios (0, 19, 5, 0). Fuera de la cola, por la cabecera:
  Event/Path `SetCamera_unk`, MiniMapTask `Camera(const Camera&)`, NewCampTask `fn_8052C6CC`.

## Decisiones tomadas sin el usuario

- `Bounds_unk` anidado en `Frustum_unk` (y no `atn::AABB_unk`, que tiene la misma forma): con `AABB_unk` el
  resultado es el mismo pero la cabecera arrastra sus inline a 253 unidades. Si se demuestra que es `AABB_unk`,
  cambiarlo es solo el tipo.
- Re-emparejado completo de etiquetas de las 216 unidades no enlazadas que incluyen `atn/Camera_unk.h`
  (toca lineas `@N` de `symbols.txt` de unidades de otras colas).

## Para el port

- `atn/Camera_unk.h`: `Frustum_unk::mMin`, `Frustum_unk::mMax` -> `Frustum_unk::mBounds.mMin`,
  `Frustum_unk::mBounds.mMax` (tipo nuevo `Frustum_unk::Bounds_unk`, mismo layout). Cambian
  `atn/Frustum_unk.cpp` (fn_8004F5B8, Frustum_unk_Overlaps, Frustum_unk_SegOverlaps) y `atn/LayerShadow.cpp`
  (`AABB_unk fbox(frustum.mBounds.mMin, frustum.mBounds.mMax)`). En las unidades del port no cambia nada mas.
- `Event/Sequence.cpp` `fn_8053E8D0`: solo la forma del final (sin cambio de firma).

## Hallazgos

- Un struct con miembros de clase (aunque no tenga inline propios) declarado en una cabecera consume dos `@N`
  en cada unidad que la incluye; un struct POD anonimo, uno.
- La copia implicita de un subobjeto struct se hace con un puntero base (`addi rX, r4, off`); la de miembros
  sueltos o de un array, con offsets directos.
