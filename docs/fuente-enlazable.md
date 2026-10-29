# Fuente enlazable (regla 10 de CLAUDE.md)

Una unidad solo cuenta de verdad cuando pasa a `matching: true` y se enlaza en el `.dol`. Hasta
entonces `main.dol: OK` no la valida (no se enlaza) y objdiff compara función a función, así que
un fuente puede dar muchos 100% y aun así no poder enlazar nunca. Esto recoge los patrones que lo
impiden, vistos en la revisión de 2026-10-29 (`docs/revision-2026-10-29.md`), y cómo evitarlos.

## 1. Orden del `.dol`

Con `-ipa file` MWCC emite las funciones en el orden del fuente. Si el `.cpp` agrupa por tipo
(stubs, setters, ctors al final) el `.text` sale en otro orden y la unidad no enlaza aunque cada
función esté al 100%.

- `python3 tools/decomp/ordercheck.py <unidad>`: funciones fuera de orden, que faltan y extra
  (ignora copias débiles de inline/plantillas). Debe dar `0 out of order` en cada commit.
- `python3 tools/decomp/reorder.py <unidad>`: reescribe el `.cpp` en orden; mantiene declaraciones
  y datos antes de su primer uso y añade prototipos para las funciones libres movidas. Después:
  `cmp.py` (mismos OK/DIFF), `ninja` y `main.dol: OK`.
- Excepción conocida: thunks que el compilador genera al final (tras `__sinit`), p. ej.
  `std/allocator_char fn_800E2A10`; no se arreglan con el orden.

## 2. Nada de nombres mangled en `extern "C"` ni vtables a mano

Mal (enlaza solo porque el nombre coincide; los tipos pueden ni corresponder al mangling):

```cpp
extern "C" void HBMRsoCreate__FR11HBMDataInfoRPUcRPUc(HomeButtonData_unk* d, void* a, void* b);
extern "C" void __ct__Q23atn11LayerShadowFv(void* layer);
extern char __vt__Q32AI6Script18AI_archer_attack01[];
AI_archer_attack01::AI_archer_attack01() { *(void**)this = __vt__Q32AI6Script18AI_archer_attack01; }
```

Bien: declarar la función o la clase en C++ (con los tipos que dice el mangling) y dejar que el
compilador genere el nombre, el ctor y la vtable:

```cpp
void HBMRsoCreate(HBMDataInfo& info, unsigned char*& soundData, unsigned char*& soundBuf); // game/HBMRso.h
new (p) atn::LayerShadow();                 // con su cabecera incluida
class AI_archer_attack01 : public CharaAi { // la vtable y el RTTI salen al definir sus virtuales
    virtual int vf08();
    ...
};
```

Si la clase aún no se conoce, declararla `_unk` con relleno (`unsigned char m00[0x40];`) y
`// inferred:`; la vtable solo se emite si se define su primera virtual no inline
(`docs/trucos-mwcc.md`).

## 3. Métodos como miembros y un tipo por objeto

- Una función que recibe el objeto como primer argumento y cuyo símbolo es mangled o está en una
  vtable es un método: va dentro de la clase (`vfXX`, `fn_<addr>`). `extern "C"` con nombre
  automático solo para funciones sin clase conocida.
- Accesos por offset (`*(int*)((char*)self + 0x9C)`) y `reinterpret_cast` entre vistas parciales del
  mismo objeto: definir el tipo una vez en `include/` y usarlo en todas partes. Ejemplo pendiente:
  `lbl_8087EFB4` está descrito con tres structs distintos en `atn/LayerShadow.cpp`,
  `atn/ModelDraw_unk.cpp` y `atn/RenderSystem_unk.cpp`.
- Macros que generan código (`#define D2A_DRAW_DUAL(...)`) no son algo que un desarrollador
  habría escrito así en este código: preferir funciones `inline`.

## 4. Datos del propio split

Si `splits.txt` asigna a la unidad `.data/.rodata/.bss/.sdata/.sdata2`, esos objetos tienen que
salir del fuente: literales (`"..."`, `1.0f`), estáticas, tablas, `switch` (jumptables), objetos
globales con destructor (generan `__sinit` y `.ctors`). Un `extern lbl_XXXXXXXX` que cae dentro del
propio split queda sin definir al enlazar. `poolcheck.py` comprueba floats y cadenas.

## 5. Proceso

- Guardar `build/SLSP01/report.json` antes de empezar y comparar `regress.py`/`fuzzdrop.py` contra
  esa copia. Un informe regenerado después del cambio no detecta pérdidas fuera de la unidad.
- Subir el fuzzy no es el objetivo: una función a medias que no va a llegar al 100% no acerca la
  unidad a enlazar.
- Codex y otros agentes leen `AGENTS.md`, que remite a `CLAUDE.md`: mismas reglas, rama propia y
  merge tras la checklist, nunca commits directos en `main`.
