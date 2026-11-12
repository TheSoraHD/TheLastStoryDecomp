# Lote paralelo 2026-11-12, cola B2 (vestuario y campamento, segunda pasada)

Inicio: 2026-11-12 21:50 CET (tope 23:35).

## Resultado por unidad (funciones al 0 % / sin escribir, antes -> despues)

- `game/NewCampTask`: 13 -> 8 (sin contar las copias debiles que dependen del presupuesto). `report.json`
  15200 -> 15204. Escritas `fn_8052CA6C` (cierre del campamento, 77,6 %), `NewCampView_unk::fn_8052E1A0`
  (guarda escena/camara/post efectos, 71 %) y `fn_8052FA44` (los aplica, 73 %; el original lo inlinea en
  fn_8052CA6C y aqui tambien). Con fn_8052E1A0 escrita, `vf0C` pasa el presupuesto R3 como el original:
  33 % -> 99,5 % y `RenderSetting_unk::ResetPostFx_unk` (0x8052B7F8) se emite: 96,6 %; al 100 % nuevas las
  copias debiles `fn_8052B7F0`, `ResHandle::Load` (0x8052BC90), `vector<NewCampElem_unk>::operator[]`/`size`.

## NONMATCHING

- `fn_8052CA6C` 77,6 %, `fn_8052E1A0` 71 %, `fn_8052FA44` 73 %: el `atn::Scene::operator=` implicito pasa el
  presupuesto en el original (R3/R5) y llama a los `operator=` de Texture, GodRay, HazeParam_unk y
  ColorMatrixParam_unk (0x8052E8D4, 0x8052BBEC, 0x8047C7FC, 0x80453DBC); aqui se inlinean porque los de
  `vector<Light>`/`vector<Camera>`/`CharaLightData_unk` solo estan declarados (no son candidatos). Arreglarlo
  pide definir esos `operator=` en cabeceras de atn/std (otra cola): pendiente.
- `vf0C` 99,5 %: registros del grupo del menu de depuracion (r27/r28); probado declarar `sub` antes: igual.
- `ResetPostFx_unk` 96,6 %: registros de las constantes del Dof.

## Decisiones tomadas sin el usuario

- `symbols.txt`: renombrados `fn_8052BC90` -> `Load__9ResHandleFPCc`, `fn_8052BCE4` ->
  `__vc__Q23std25vector<15NewCampElem_unk>FUi`, `fn_8052BCF4` -> `size__Q23std25vector<15NewCampElem_unk>CFv`;
  dos etiquetas de `.sdata2` de NewCampTask que `pd.sh` no renumero (`@16438` -> `@17601`, `@16623` -> `@17786`).
- El color de borrado de `NewCampView_unk` (0x238) se lee con un cast `*(RsColor_unk*)m238`: con un miembro
  `RsColor_unk` el constructor de NewCampView_unk bajaba (llama al constructor de RsColor_unk).

## Hallazgos

- Escribir una funcion grande no inline del mismo fichero puede subir otras: con `-ipa file` es candidata a
  autoinline de sus llamadores y los pasa del presupuesto (vf0C 33 % -> 99,5 % al escribir fn_8052E1A0).
