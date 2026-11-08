# Siguiente sesion (tras `s12h-20261107`, mergeada en `main`)

`main` (merge abdf617 de `s12h-20261107`): **14351 / 18116** en `report.json` (14348 en `objdiff diff`: los vf10
de ControlRevival, ControlRideBeast y ControlBoundShade quedan al 99,8-99,9% por el pool
`@stringBase0_8073B8E8`), lost 0, sin bajadas, clon limpio `main.dol: OK`. Crear rama nueva desde `main`.
Notas: `docs/s12h-20261107.md`.

## Lo primero

- Decisiones abiertas de `docs/s12h-20261107.md` (seccion Resultado): revisar al tocar esos ficheros.
- Tras `precommit.sh --pd`: devolver `lbl_8087D6E8`/`lbl_8087D6EC` en `symbols.txt` (pd los renombra a
  `@STRING@Resize_unk...`).

## Trucos nuevos que conviene seguir aplicando (barridos en `tools/decomp/`)

- `operator[]` del contenedor en lugar de `.mData[i]` (`opidx.py`): resuelve el cruce r3/r4 de `__VecCopy`.
- Locales con nombre para lecturas de miembros (`hoist.py`, `hoist2.py`), `const` de nivel superior en
  parametros por valor (`bvconst.py`).
- Huecos de pila en orden inverso: el codigo estaba en un inline (sus temporales van debajo). Casos resueltos:
  `EffOrigin_unk` (bloque pos/rot/color + `fn_8023A8B4`, `effz.py`), direccion frontal (`front.py`), distancia al
  cuadrado (`dsq.py`). `combo.py` prueba el producto de alternativas por linea sin ninja.

## Pendientes

1. GameTask 104/133: quedan las grandes (`vf10` 12,7 KB, `fn_80371790`, `fn_803731E8`, dtor, `fn_80367A40`...)
   y NONMATCHING casi iguales (lista en `docs/mwcc-blockers.md`, entrada s12h).
2. Unificacion de vistas: ~100 vistas `Field_unk` locales (fichero a fichero, >8 h); `fn_8013C504`,
   `GetState_unk`/`GetRoute_unk` duplicados en CharaTask y GameUI.
3. Constructor de `BattleTask` (0x800F1430, 2,7 KB) sin empezar; ctor de `TacView` al 99,5%.
4. `GameCamera_unk` 17/90: las funciones de modo chocan con el presupuesto de inlining de `fn_80392964`;
   `fn_8039328C` escrita (89,5%, solo offsets del pool hasta escribir las cadenas anteriores).
5. `Event/DebugInfo_unk` 2/58; `ut_TextWriterBase` (unica NW4R sin enlazar).
6. `fn_8017C9AC` (CharaTask) a 4 lineas con inlines para el segmento y el empuje (queda r6/r7).
