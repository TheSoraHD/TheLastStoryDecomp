# port-decomp-20261114, cola A (Interfaz: HUD de la partida)

Inicio: 2026-11-14 02:59 CET. Rama `agent-A`. Unidades: `game/GameUI`, `game/PopupTask`, `game/TacView`.

## Resultado por funcion

| Funcion | Antes | Despues | Firma final |
|---|---|---|---|
| `GameUI::fn_803D83A8` (10148 B) | 0 % (sin escribir) | escrita, NONMATCHING (ver %) | `void GameUI::fn_803D83A8()` |
| `GameUI::fn_803DAB4C` (4600 B) | 0 % (sin escribir) | escrita, NONMATCHING (ver %) | `void GameUI::fn_803DAB4C(int show, int keep)` |

- `fn_803D83A8`: actualizacion por cuadro del HUD (llamada por GameTask): congela todas las partes; iconos de estado
  sobre los enemigos cercanos (`m2788`), medidores de magia (`fn_803E2410` de jugadores y enemigos), lineas de odio de
  la vista tactica (hacia el personaje bajo el cursor, `m15B4`), lineas de punteria de los jugadores hacia
  `AIManager::m458` (`m1170`, `m1174[]`, efectos `m1158`/`m1164`), icono de habilidad usada (`m2564`, cola `m2568`),
  avisos (`m2550`, sonidos `lbl_80750618[7..9]`), contadores sobre personajes (`m25CC`/`m2610`), marcador del objetivo
  (`m2628`, nodo "spine") y marcador de subida de nivel (`m110C`: estadisticas antes/despues desde
  `Inventory::mExpEntries` o `mDF30`, y habilidades aprendidas `m1134`).
  Diferencias: marco 0x50 mayor, copia de colores (el original copia `GameUIColor_unk` campo a campo con lfs/stfs,
  como un constructor de copia), registros.
- `fn_803DAB4C`: iconos de la vista tactica: anillo del cursor que gira (`mDD0`, envuelto a -pi..pi con `fmod`),
  dibujado con `DrawHelper::fn_80064AC4` y los colores del efecto bajo el cursor (`SkillEffectManager`), ventana de
  orden "TARGET?/RETURN?/ORDER?" (`m2544`/`m2548`, estado `m2540`); `keep == 0` llama a `fn_803DBD44`.
  Diferencias: copia de colores (lfs/stfs intercalados), registros.

## Cambios de firma / layout (para el port)

- `GameUI.h`: `m1134` pasa de `unsigned char[0xC]` a `GameUISkill_unk* m1134[3]` (habilidades aprendidas, contador
  `m1130`); `m1170` de `int` a `GameUIAimTarget_unk*`; `m15B4` de `int` a `CharaTask*`. Tipos nuevos declarados en
  `GameUI.h` y definidos en `GameUI.cpp`: `GameUISkill_unk` (nombre `m0C`, descripcion `m14`), `GameUIAimTarget_unk`
  (escala `m28`, radio `m5C`).
- `GameUIField_unk` (vista local de `lbl_8087F430` en GameUI.cpp) gana `int m54E4` (= `GameTask::m54E4`).
- `fn_803DAB4C(int, int)`: parametros nombrados `show`, `keep` (sin cambio de tipos).

## Decisiones

- `GameUIAimTarget_unk` es una vista del objetivo de `AiTarget` (`m0C`, tipo `CharaTask*` en `AiTarget.h`, otra
  familia): deuda de la regla 10, pendiente de llevar a `game/AiTarget.h`.
- `fn_803DBD44` declarado `extern "C" void fn_803DBD44(GameUI*)` (el simbolo no tiene nombre C++; sin escribir).
- Tras `pd.sh`, 13 etiquetas `@N` de `.sdata2` quedaban obsoletas (labelcheck peor); se renombraron por valor
  (cada valor es unico en nuestro .o) con un script de una sola vez. `GetNodePos_unk` y la declaracion de
  `lbl_8087F0A8` suben mas arriba en GameUI.cpp (inline/declaracion, sin cambio de codigo).

## Hallazgos

- Una cadena literal dentro de una funcion inline de fichero sale como `@STRING@<inline>` y no en `@stringBase0`:
  pasar los nombres desde la funcion que llama.
- `20.0f * 3.5f` se pliega a 70.0f; el original multiplica (`float radius = 20.0f; radius * 3.5f` no se pliega).
