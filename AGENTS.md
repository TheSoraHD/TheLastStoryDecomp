# AGENTS.md

Instrucciones para Codex y cualquier otro agente que trabaje en este repo.

**Las reglas del proyecto están en `CLAUDE.md`: léelo entero antes de empezar y síguelo igual que
si estuviera escrito aquí.** Es corto y es la única fuente de verdad (este fichero no la repite para
que no se desincronice). Comunicación con el usuario en español.

Lo que más se ha incumplido, por si acaso:

- Nunca commits directos en `main`: rama propia, checklist de `CLAUDE.md`, merge después.
- Una función solo cuenta al 100% en objdiff; subir el fuzzy no es avance.
- El fuente tiene que poder enlazar (regla 10, `docs/fuente-enlazable.md`): orden del `.dol`
  (`tools/decomp/ordercheck.py`), clases reales en lugar de `extern "C"` con nombres mangled o
  vtables escritas a mano, métodos dentro de su clase y datos del propio split definidos en el
  fuente.
- Sin `asm` ni `#pragma` fuera de las excepciones aprobadas.
