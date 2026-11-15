The Last Story — Decompilación
==============================

Decompilación de **The Last Story** (Wii, versión europea `SLSP01`). El objetivo es un código C/C++ que,
compilado con los compiladores originales de Metrowerks, produzca un `main.dol` idéntico byte a byte al del juego.

Este repositorio **no contiene** ningún fichero del juego. Para compilarlo hace falta una copia propia del disco
(`orig/SLSP01/sys/main.dol`, SHA-1 `d4275869483fbc681f2ce6eddfbd9742e40c6b64`).

Estado
------

A 15 de noviembre de 2026:

- Funciones al 100 %: **15631 / 18109**.
- Datos enlazados: **1075250 / 1432078 bytes (75,08 %)**; 698 unidades completas.
- `main.dol` reconstruido idéntico al original (`ninja` termina con `main.dol: OK`).

El historial de sesiones está en [`docs/historial.md`](docs/historial.md).

Compilar
--------

El proyecto usa la plantilla de [decomp-toolkit](https://github.com/encounter/decomp-toolkit) con
[ninja](https://ninja-build.org/) y [objdiff](https://github.com/encounter/objdiff).

1. Copia `main.dol` del disco en `orig/SLSP01/sys/main.dol`.
2. Consigue los compiladores de Metrowerks (GC/Wii) en `build/compilers/` y `sjiswrap.exe` en `build/tools/`.
3. `python3 configure.py` (en Linux/macOS, con `--wrapper` apuntando a [wibo](https://github.com/decompals/wibo) o wine).
4. `ninja`.

Montaje completo en Linux y en la nube: [`docs/entorno-nube.md`](docs/entorno-nube.md).
Reglas y flujo de trabajo: [`CLAUDE.md`](CLAUDE.md). Documentación de la plantilla: [`docs/getting_started.md`](docs/getting_started.md).

Estructura
----------

- `src/` — código del juego (motor `atn::`, `Event::`, `chr::`, `gmk::`, `game/`, `MP/`…).
- `include/` — cabeceras del juego y del motor.
- `libs/` — librerías de Nintendo, Metrowerks y terceros (ver Agradecimientos).
- `config/SLSP01/` — `splits.txt`, `symbols.txt`, `game_units.json`, `libs_ref.json`.
- `tools/decomp/` — scripts de apoyo (comparación, re-emparejado de etiquetas, comprobaciones).
- `docs/` — notas de cada sesión, trucos de MWCC y bloqueos conocidos.

Agradecimientos
---------------

Este proyecto se apoya en el trabajo de muchas otras decompilaciones y herramientas. Gracias a todos sus autores.

### Herramientas

- [decomp-toolkit](https://github.com/encounter/decomp-toolkit), [objdiff](https://github.com/encounter/objdiff)
  y [sjiswrap](https://github.com/encounter/sjiswrap), de encounter.
- [wibo](https://github.com/decompals/wibo), de decompals.
- [decomp.dev](https://decomp.dev) y la comunidad de decompilación de GameCube/Wii.

### Fuentes de librerías usadas en `libs/`

- [xbret/xenoblade](https://github.com/xbret/xenoblade) (CC0): RVL SDK, NW4R, MSL, Runtime, MetroTRK y NdevExi2A
  (`libs/`, ver `libs/LICENSE.xenoblade`).
- [SMGCommunity/Garigari](https://github.com/SMGCommunity/Garigari) (CC0), decompilación de Super Mario Galaxy 2:
  módulos del RVL SDK (BTE, VI, NAND, KPAD, NWC24, THP…) en `libs/smg2`.
- mscharged-decomp (CC0), decompilación de Mario Strikers Charged: KPAD, DWC, NHTTP, SO y GameSpy en `libs/msc`.
- [zeldaret/ss](https://github.com/zeldaret/ss) (CC0), decompilación de Skyward Sword: NW4R de 2010 en `libs/ss_nw4r`.
- [zeldaret/tp](https://github.com/zeldaret/tp) (CC0), decompilación de Twilight Princess: HOME Button Menu en
  `libs/tp_revolution`.
- [doldecomp/mkw](https://github.com/doldecomp/mkw) (CC0), decompilación de Mario Kart Wii: SO, DWC y GameSpy.
- [Swiftshine/key](https://github.com/Swiftshine/key) (CC0), decompilación de Kirby's Epic Yarn: cabeceras del SDK,
  WPAD y `HBMAxSound` en `libs/key`.
- [koopthekoopa/wii-ipl](https://github.com/koopthekoopa/wii-ipl) (CC0): SSL y AXFX.
- [revvmp/pokemon-rumble-decomp](https://github.com/revvmp/pokemon-rumble-decomp) (MIT): AXFX y `db_DbgPrintBase`.
- [nitrocaster/GameSpy](https://github.com/nitrocaster/GameSpy) (BSD-3, código de IGN): GameSpy OpenSDK, base de `libs/gamespy`.
- [Boost](https://www.boost.org/) 1.35 (Boost Software License 1.0): subconjunto en `libs/boost`.
- Bluedroid (Apache 2.0, Broadcom), a través de las decompilaciones anteriores: pila Bluetooth (BTE).
- [robojumper/sdk_2009-12-11](https://github.com/robojumper/sdk_2009-12-11), decompilación del Revolution SDK de
  diciembre de 2009 (robojumper y muff1n): cuerpo de `KPADGetMplsZeroDriftMode` en `libs/msc/src/RVL_SDK/kpad/KPAD.c`.
  El repositorio no declara licencia.

### Consultados como referencia (sin copiar código)

- [doldecomp/ogws](https://github.com/doldecomp/ogws) (Wii Sports), [Etrainquandt17/Whippy](https://github.com/Etrainquandt17/Whippy)
  y [riptl/wii-symbols](https://github.com/riptl/wii-symbols) (tamaños de símbolos de otros juegos).

Licencia
--------

El código propio de este repositorio se publica bajo CC0 1.0 (ver [`LICENSE`](LICENSE)). Las librerías de `libs/`
conservan la licencia de su origen (ver los `LICENSE` y `README` de cada carpeta).
