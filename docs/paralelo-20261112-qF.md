# Lote paralelo 2026-11-12, cola F (Efectos)

Inicio: 2026-11-12 16:30 CET. Rama `agent-F`. Unidades: `atn/EffectSystem_unk`, `atn/EffectManager`.

## Resultado por unidad

- `atn/EffectSystem_unk` (commit 1): 0 % 24 -> 18; al 100 % nuevas: fn_8022C2A0 (liberacion), fn_8022D078
  (update), fn_8022D328 (filtro de capa/pasada), fn_8022D478 (dibujado de una capa), fn_8022D754 (entrada del
  fichero de efectos, extern "C"), fn_80232ED4 (parar por etiqueta).
- `atn/EffectSystem_unk` (commit 2): 0 % 18 -> 14; al 100 % nuevas: fn_80230034 (reduccion de la emision con la
  distancia a la camara; funcion libre C++ que recibe el sistema), fn_80231508 (avance de un slot en su
  direccion), fn_80231754 (avance de los slots de una instancia), fn_802328EC (avanzar una instancia N frames).

## NONMATCHING

## Decisiones tomadas sin el usuario

- **Cabecera muy incluida**: `include/atn/EffectSystem_unk.h` la incluyen 67 unidades (via `BattleTask.h`,
  `EffectManager.h`). Los metodos nuevos de `EffectSystem_unk` (fn_8022D078, fn_8022D478, fn_802301EC) son
  miembros (regla 10) y cada declaracion renumera los `@N` (+5 en total). Se reetiquetaron con
  `pdbatch.sh` las 67 unidades; en 7 (GameEventScript, GameTask, Em204BigMulue, Em314Ivy,
  Em203GreaterDaemonRedMp, PacketPack, HomingBullet) pairdata dio ambiguedades y se reetiquetaron con un
  script propio que toma el par objetivo->nuestro de los `DIFF_ARG_MISMATCH` de objdiff, por mayoria y solo
  dentro del rango de la unidad (`/home/claude/scratch-F/tools/relabel_shift.py`). `pairdata` renombro mal
  `lbl_8087D6E8/EC` (tramo auto_09) a `@STRING@Resize_unk...`: revertido a mano. Resultado: ~2350 lineas de
  `@N` cambiadas en `symbols.txt`; **al integrar puede chocar con otras colas que reetiqueten las mismas
  unidades** (repetir pdbatch tras el merge si hace falta).
- Tipos movidos/renombrados: `EffectSystemPool_unk` pasa a ser un asignador de bloques de `EffectFade_unk`
  (`mBlockSize`, `mBlockCount`, `SimpleArray<EffectFade_unk> mItems`, mapa de bits `vector<unsigned int>`);
  se quita `EffectSystemElem_unk`. Miembros de `EffectSystem_unk` 0x00-0x14 con nombre (pool de instancias,
  lista libre, slots). `EffectEntry_unk` de `LH_Effect.cpp` pasa a `include/atn/EffectFileEntry_unk.h`
  (`EffectFileEntry_unk`, typedef en LH_Effect). La vista de `lbl_8087EFA8` de EffectManager pasa a
  `include/atn/EffectParams_unk.h` (compartida con EffectSystem_unk).
- `EffectDesc_unk.cpp` llamaba a fn_8022D078/fn_8022D478 como `extern "C"` con `self`: ahora llama a los
  miembros (`fn_8022D478((int)arg)`: el argumento de fn_802368B0 es la capa; no se cambio su firma).
- `fn_80230034` escrita como funcion libre C++ (`fn_80230034(EffectSystem_unk*, ...)`) y no como miembro: no se
  sabe si es metodo, y un miembro nuevo renumeraria otra vez las 67 unidades. Revisar si se prefiere miembro.
- `fn_802301EC` renombrado en `symbols.txt` al nombre del miembro declarado (sin escribir aun).

## Hallazgos

- fn_8022D478: el original pasa a `RenderManager::fn_800C122C` una segunda esfera que solo se asigna en la
  pasada 4 (se lee antes de escribirla); reproducido con `world` fuera del bucle.
- fn_8022D478: `rm` en r31 sale con una referencia a la camara (`Camera& camera = instance_->mCamera`), no con
  un puntero a RenderManager.
- `Matrix3x4::fn_8013A13C` (rotacion y inline) en vez de `PSMTXRotRad(.., 'y', ..)` hace que la carga del
  angulo use r3 como el original (fn_80231508); `Matrix3x4::GetTrans_unk()` en vez de `Vector3(m[0][3]...)`
  arregla el orden de registros (fn_80230034).
- fn_8022D078: la posicion se pasa por puntero a fn_8022C9C8 y la copia previa (`move = old - new`) no se usa.
