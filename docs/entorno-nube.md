# Entorno en la nube y lotes en paralelo

## Montaje (unos 5 minutos, sin el PC)

Todo lo que no se descarga está en el repo privado `TheSoraHD/tls-build` (~180 MB): `orig/SLSP01/sys/main.dol`, `build/compilers/` (todas las versiones GC/Wii; el juego usa `GC/{1.2.5n,1.3.2,3.0a3,3.0a5.2}` y `Wii/{1.0,1.0a,1.1,1.3}`) y `build/tools/sjiswrap.exe`. Sus `dtk.exe`/`objdiff-cli.exe` son de Windows: en Linux se usan las releases de GitHub. `files.decomp.dev`, Discord y decomp.me están bloqueados desde la nube; las releases de GitHub y `raw.githubusercontent.com`, no. Probado el 2026-10-30 con `main` 4e8111f: `main.dol: OK`.

```bash
# add_repo TheSoraHD/tls-build; add_repo TheSoraHD/TheLastStoryDecomp (con access "push" si hay que subir; clona en minúsculas)
cd /home/claude && git clone --depth 1 https://github.com/TheSoraHD/tls-build
git clone https://github.com/TheSoraHD/TheLastStoryDecomp.git
mkdir -p tools && cd tools
curl -sSLfo dtk https://github.com/encounter/decomp-toolkit/releases/download/v1.8.3/dtk-linux-x86_64
curl -sSLfo objdiff-cli https://github.com/encounter/objdiff/releases/download/v3.6.1/objdiff-cli-linux-x86_64
curl -sSLfo wibo https://github.com/decompals/wibo/releases/download/1.0.3/wibo-x86_64
chmod +x dtk objdiff-cli wibo
apt-get update -q && apt-get install -y -q binutils-powerpc-linux-gnu ninja-build
pip install -q --break-system-packages pyelftools
B=/home/claude/tls-build; R=/home/claude/TheLastStoryDecomp
mkdir -p $R/orig/SLSP01/sys && cp $B/orig/SLSP01/sys/main.dol $R/orig/SLSP01/sys/
mkdir -p /home/claude/binutils && for f in /usr/bin/powerpc-linux-gnu-*; do ln -sf $f /home/claude/binutils/powerpc-eabi-${f#/usr/bin/powerpc-linux-gnu-}; done
cd $R && mkdir -p build/tools && ln -sfn /home/claude/binutils build/binutils
for t in dtk objdiff-cli wibo; do ln -sf /home/claude/tools/$t build/tools/$t; done
ln -sf $B/build/tools/sjiswrap.exe build/tools/sjiswrap.exe     # tools/decomp/try.sh lo busca ahí
python3 configure.py --compilers $B/build/compilers --dtk /home/claude/tools/dtk --objdiff /home/claude/tools/objdiff-cli --sjiswrap $B/build/tools/sjiswrap.exe --wrapper /home/claude/tools/wibo
ninja && build/tools/dtk shasum -c config/SLSP01/build.sha1     # ~3 min, debe dar OK
```

- Si `tls-build` no estuviera disponible, plan B con el PC enlazado: `device_request_folder_access` sobre `~/Documents/GitHub/TheLastStoryDecomp` y `device_stage_files` de `orig/SLSP01/sys/main.dol`, `build/tools/sjiswrap.exe` y las ocho carpetas de compiladores de arriba (aparecen en `/mnt/user-data/uploads/TheLastStoryDecomp/...`).
- `git push` necesita permiso explícito del usuario en la sesión; sin él, entregar con `git bundle create x.bundle main..<rama>`.
- El clon de `add_repo` es superficial: antes de subir `git fetch origin main`, y tras el primer push `git fetch origin <rama>:refs/remotes/origin/<rama>` (si no, el hook sigue avisando de "commits sin subir"). Las ramas `agent-X` de las copias son locales: no se suben.
- Copia local del PC: puede ir por detrás de `main`; hacer `git pull` antes de usar su build.

## Lote en paralelo (receta probada)

**Solo con permiso explícito del usuario en la sesión** (regla 12 de `CLAUDE.md`); por defecto se trabaja de forma secuencial.

1. Rama de integración desde `main`; guardar `build/SLSP01/report.json` como referencia. Listar objetivos con `targets.py` y repartirlos en colas por familia (`chr`+`Em*`, `gmk`, `atn`, `db`, resto de `game`/`Event`/`MP`) y, dentro de cada familia, por tramos de direcciones contiguos, de menor a mayor. Trabajo sobre `splits.txt`: por **rangos de direcciones contiguos**. Colas pequeñas (2–6 unidades), primera tanda con colas no adyacentes; con los huecos del final, segundas pasadas sobre colas ya integradas.
2. Una copia por agente: `cp -a TheLastStoryDecomp workX && rm workX/.ninja_deps` (si no, tocar cabeceras no recompila), rama `agent-X`. Máquina de 2 núcleos: `ninja -j1` y compilar solo la unidad al iterar. **Crear la copia de cada cola nueva desde la rama de integración ya actualizada** y decir al agente que busque con `grep` en las notas de las colas vecinas ya integradas lo que afecte a sus unidades (no leerlas enteras).
3. Prompt de agente **corto** (en un fichero, pasarle la ruta). Los agentes trabajan dentro de una copia del repo, así que **cargan solos el `CLAUDE.md` del repo**: no copiar reglas ni trucos en el prompt; basta con indicar `docs/trucos-mwcc.md` para `grep`. El prompt lleva solo: su cola; que no toque unidades de otras colas ni límites ajenos (anotarlos como pendientes); commit por unidad con `main.dol: OK`, `regress.py` lost: 0, `fuzzdrop.py` sin bajadas y `ordercheck.py` sin funciones fuera de orden; fichero de notas `docs/<lote>-qN.md` (resultado por unidad, NONMATCHING con motivo, **decisiones tomadas sin el usuario**, hallazgos) actualizado tras cada unidad; subcarpeta de scratch propia; si renombra en `symbols.txt` algo que otros `.cpp` usan como `extern "C"`, actualizar esos llamadores; no contar como ganada una función que `report.json` da al 100% y `objdiff-cli diff` no.
4. Al terminar cada agente: `git fetch <copia> agent-X:agent-X && git merge --no-ff agent-X`. `game_units.json` choca casi siempre: unión de entradas por `name` (script con `git show :1:/:2:/:3:`). Conflictos de `symbols.txt`: `tools/decomp/symresolve.py`. Luego `configure` + `ninja`, `main.dol: OK` y `regress.py` → `lost: 0` (y `fuzzdrop.py`) contra la integración anterior **y** contra el `report.json` final del agente. Compilar tras **cada** merge. Tras cambiar splits, build limpio (`rm -rf build/SLSP01`).
5. Fallos típicos al integrar: un renombrado que otra cola usa como `extern "C"` (`undefined: fn_…`) o la misma función con otra firma (`illegal function overloading`); en enemigos, la factoría de `game/SpecialEnemyBase.cpp` (conservar todos los `#include`, quitar los `extern` sustituidos) y un tipo definido en dos cabeceras (moverlo a una común).
6. Lanzar un agente nuevo en cuanto uno acaba (4 a la vez). Si el contenedor se reinicia, los ficheros sobreviven y el agente se puede reanudar.
7. Cierre: build limpio, `regress.py`, `fuzzdrop.py`, `poolcheck.py` sobre las unidades del lote, avisos de dtk frente a un clon limpio de `main`, ninguna unidad con fuente pierde datos sin anotarlo, ningún `scope:local` nuevo fuera de unidades; actualizar "Estado" de `CLAUDE.md` (solo sus líneas), una fila en `docs/historial.md` y `docs/<lote>.md`; push de la rama; clon limpio de la rama subida con `main.dol: OK`; resumen al usuario con las decisiones a revisar.

## Consumo de tokens

- `CLAUDE.md` se carga en cada turno de la sesión principal y de cada agente: mantenerlo corto (sin historial ni trucos).
- Leer `docs/trucos-mwcc.md` y las notas de lotes con `grep`, no enteros. No volcar ficheros `.s` o `.cpp` grandes: `fasm.py`/`odiff2.py` sobre la función concreta.
- Salidas largas de `ninja`, `regress.py` o `report.json`: filtrarlas (`| tail`, `grep`) antes de que entren en el contexto.
