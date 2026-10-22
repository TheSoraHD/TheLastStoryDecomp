# Lote enemies-finish, cola q6

Inicio: 2026-10-22 13:21 CEST. Rama agent-q6 (sale de main 64df7a6 + merge q7; referencia start-q6: 6940 funciones al 100%).
Cola: game/Em316LaserSphere, game/Em316LastLaser, game/Em401_01_WhiteLeoPard.

## Unidades

- `game/Em316LaserSphere` (unidad SIN FUENTE hasta ahora): 0/40 -> 14/40. Fuente nueva
  `src/game/Em316LaserSphere.cpp` y cabecera nueva `include/game/Em316LaserSphere.h` (subclase de EnemyTask,
  layout 0x14B0-0x1CA0 deducido del ctor; nombres inventados). Al 100%: ctor (tokenizer con `it->c_str()`,
  operator-> del iterador), dtor implicito, vf14, vf3C, vfE4, fn_80317EA0, fn_80318A30, fn_80318F9C,
  fn_803191FC, fn_80319578, fn_80319C10 y las copias debiles `__ct__Q23atn9Matrix3x4Fv` (0x803165E0) y
  `__defctor__15EffectModel_unkFv` (0x803165E4), fn_80318F98 (vacia).
  NONMATCHING (escritas): vf0C 99.52% (el original usa `this` como variable de induccion del ultimo bucle),
  vf40 99.16% y fn_80318648 99.80% (pila del inline y offset de "spine1"), vf80 99.69% (pila), fn_80319384
  99.67% (pila del lerp), fn_803199A4 99.90% (solo pool), fn_8031A484 99.23% (FPR de la gravedad, __VecCopy),
  vfEC 94.2% (mascara 0x80000080 en r0, __VecCopy), fn_80318594 88.7% (orden de stores de la capsula),
  fn_80318720 96.4% (marco 0x30 mayor: temporales de rotacion no identificados) y vf10 0% (presupuesto de
  inline: el original llama fuera de linea a todos los helpers; aqui se inlinea todo, incluidos los estados).
  Sin escribir: fn_803176D0 (2 KB, push_back de vector), fn_80317EF8, fn_80318AA4, fn_8031958C,
  fn_80319C90, fn_8031A670, fn_8031ACC0 (insert de vector) y las copias debiles de vf10.

## Decisiones tomadas sin el usuario

- **symbols.txt (Em316LaserSphere)**: renombrados ctor, dtor, vf0C/vf10/vf14/vf3C/vf40/vf80/vfE4/vfEC y los
  metodos no virtuales `fn_XXXXXXXX__16Em316LaserSphereF...`; `lbl_8074959C` -> `@stringBase0` (scope:local);
  `fn_803165E0` -> `__ct__Q23atn9Matrix3x4Fv`, `fn_803165E4` -> `__defctor__15EffectModel_unkFv`,
  `fn_802BABC0` (copia debil en game/Em206CocoonMp, sin fuente) -> `__ct__9ResHandleFv`. Ningun .cpp los
  llamaba por `extern "C"` salvo la factoria.
- `game/SpecialEnemyBase.cpp`: la factoria crea los tipos 45 y 49 con `new (3, "", "", 0)
  Em316LaserSphere(parent, info)` (se quita el `extern "C"` fn_803161F8). fn_805A5374 sin cambios en el report.
- **Cabecera compartida `include/atn/EffectModel_unk.h`**: el ctor recibe un argumento por defecto
  (`flags = 9`) para poder declarar arrays `EffectModel_unk m[3]` (el `__defctor` 0x803165E4 hace `li r4, 9`).
  No cambia el codigo de nadie (regress/fuzzdrop 0).

## Hallazgos MWCC

- El `__defctor__` de un array de objetos con constructor de argumento por defecto se emite como copia debil
  justo despues del ctor que construye el array.

## Pendientes para otras colas
