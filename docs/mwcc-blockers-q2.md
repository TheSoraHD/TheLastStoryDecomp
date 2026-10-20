# mwcc-blockers q2: ctor inline de `atn::CsvU` con `this` en registro salvado

Agente 2, rama `agent-2`, 2026-10-20/21. Cabecera: `include/atn/Csv.h`. Scratch: `/home/claude/scratch/agent-2/`
(`try.sh` propio con `-i inc` para probar variantes de `Csv.h` sin tocar el repo; `t1.cpp` = copia de
`CampSaveLoad::fn_8011B8D4`, `t3.cpp` = cargador pequeno tipo `GameSaveBanner`, `t2.cpp` = control con `atn::Csv`
de `char`; `m*.cpp` = modelos minimos; `v*.h` = variantes de la cabecera; `inv.py` = inventario).

## El patron (asm original frente al nuestro)

En `fn_8011B8D4` (CampSaveLoad, 0x8011B8D4) el original hace:

```
addi r27, r1, 0x10          <- direccion del CsvU en registro salvado (creado ANTES de los memset)
addi r3, r1, 0x20 ; li r4,0 ; li r5,0x800 ; stw vt ; stw 0 x4 ; bl memset
addi r3, r1, 0xc20 ; ... ; bl memset
cmpwi r29,0 ; mr r5,r29 ; beq ; subi r5,r29,1
cmpwi r29,0 ; mr r3, r27 ; beq ; addi r4,r25,2 ; b ; mr r4,r25     <- `this` = r27
lwz r12,0(r3) ; lwz r12,8(r12) ; mtctr ; bctrl                      <- vf08 virtual
lis/addi r3 = __vt__CsvU ; stw r3,0x10(r1) ; mr r3, r27 ; bl fn_8005BCE8   <- `this` = r27 otra vez
... despues del ctor: SIEMPRE `addi r3, r1, 0x10` (r27 se reutiliza para otra cosa)
```

Nosotros: identico salvo que no existe r27 y cada `mr r3, r27` es `addi r3, r1, 0x10`; un registro salvado menos
(`stmw r27` frente a `stmw r25`), frame 0x10 menor, y todos los registros desplazados. En `TextTable_unk`
(dos CsvU seguidos) pasa lo mismo dos veces (r27 y luego r26).

Lo mismo en `CsvTabU` (GameSaveBanner 0x803B9324: `addi r24, r1, 0x108`). En `atn::Csv`/`CsvTab` de `char`
(127+78 sitios) el original NO guarda la direccion: `addi r3, r1, off` antes de `lwz r12` y antes de `fn_8005BCE8`,
como nosotros. La copia debil del ctor de CsvU (`__ct__Q23atn4CsvUFPCwi` 0x80209048, en db/CharaIdTable_unk) y
la de `atn::Csv` (0x8004203C) son identicas a lo que generamos: el fuente del ctor es correcto, el problema es solo
como queda `this` al inlinear en wchar_t.

Diferencia clave entre CsvU/CsvTabU y Csv/CsvTab: en las wchar_t los argumentos del ctor base pasan por los
ternarios `length != 0 ? text + 1 : text, length != 0 ? length - 1 : length` (salto del BOM). En el modelo minimo
(`m1.cpp`): con los dos ternarios el original tendria el registro; con `B(t, n)`, `B(n ? t+1 : t, n)` o
`B(t, n ? n-1 : n)` nosotros ya coincidimos con el original de `char`. Es decir, el registro aparece solo cuando
hay dos ternarios (dos temporales condicionales vivos a la vez al llamar a `vf08`) y aun asi no en nuestra build.

## Inventario (sitios donde el original inlinea un ctor de Csv*: referencia a `__vt__Q23atn*`)

Total 231 sitios, 82 al 100%. Solo listo los que no estan al 100%. "sin fuente" = unidad todavia en asm.

### CsvU: 24 sitios, 1 al 100%, 12 sin fuente (asm), 11 con fuente y <100%

| unidad | funcion | direccion | % |
|---|---|---|---|
| MP/Text | fn_80505FC4 | 0x80505FC4 | sin fuente |
| MP/Text | fn_80506D94 | 0x80506D94 | sin fuente |
| atn/D2A_MessageTable_unk | fn_801F36C8 | 0x801F36C8 | 93.61489 |
| atn/ptr_string_base_wchar_t_0 | __ct__13TextTable_unkFv | 0x8020C0BC | 95.498024 |
| db/CharaIdTable_unk | __ct__Q23atn4CsvUFPCwi | 0x80209048 | sin fuente |
| db/EquipParam_unk | fn_8020EB0C | 0x8020EB0C | 96.22152 |
| db/ItemParam_unk | fn_80211244 | 0x80211244 | sin fuente |
| db/MapIdTable_unk | fn_80216CD0 | 0x80216CD0 | 98.2471 |
| db/MapLink_unk | fn_802150F0 | 0x802150F0 | sin fuente |
| db/OptionParts_unk | fn_8020D688 | 0x8020D688 | sin fuente |
| db/PaletteTable_unk | fn_8020B2BC | 0x8020B2BC | 96.323586 |
| db/SkillParam_unk | fn_80219C30 | 0x80219C30 | 87.8042 |
| db/SpText_unk | fn_8021B060 | 0x8021B060 | sin fuente |
| db/SpText_unk | fn_8021B2F8 | 0x8021B2F8 | sin fuente |
| db/SpText_unk | fn_8021C3F4 | 0x8021C3F4 | sin fuente |
| db/TutorialTable_unk | fn_8021ED08 | 0x8021ED08 | 97.70874 |
| db/WeaponParam_unk | fn_8020689C | 0x8020689C | sin fuente |
| game/CampSaveLoad | fn_8011B8D4__12CampSaveLoadFPCwi | 0x8011B8D4 | 92.1 |
| game/GameChoiceWindow | fn_803B35FC | 0x803B35FC | sin fuente |
| game/LibraryTask | fn_80469D60__11LibraryTaskFPCvUii | 0x80469D60 | 97.33779 |
| game/LibraryTask | fn_8046B798__8BookTaskFPCvUi | 0x8046B798 | 97.02892 |
| game/NewCampPartsOption | fn_80522938 | 0x80522938 | sin fuente |
| game/NewStaffRoll | fn_80531394__12NewStaffRollFv | 0x80531394 | 97.756096 |

### CsvTabU: 1 sitios, 0 al 100%, 0 sin fuente (asm), 1 con fuente y <100%

| unidad | funcion | direccion | % |
|---|---|---|---|
| game/GameSaveBanner | fn_803B9324__14GameSaveBannerFv | 0x803B9324 | 97.46354 |

### Csv: 127 sitios, 33 al 100%, 82 sin fuente (asm), 12 con fuente y <100%

| unidad | funcion | direccion | % |
|---|---|---|---|
| MP/D2AMpMenu | fn_804B4400 | 0x804B4400 | sin fuente |
| MP/Text | fn_805061C8 | 0x805061C8 | sin fuente |
| MP/Text | fn_80507310 | 0x80507310 | sin fuente |
| atn/AudioDevice | fn_8004A3FC | 0x8004A3FC | sin fuente |
| atn/CsvBase_char_124_13_34 | fn_802078C4 | 0x802078C4 | sin fuente |
| atn/ptr_string_base_char_0 | fn_8021F47C | 0x8021F47C | 72.4058 |
| db/ArrowSet_unk | fn_80207310 | 0x80207310 | 99.27707 |
| db/ChaseParam_unk | fn_8020A90C | 0x8020A90C | 97.94118 |
| db/EventIdTable_unk | fn_8020F248__16EventIdTable_unkFv | 0x8020F248 | 98.03595 |
| db/ItemParam_unk | fn_80210ACC | 0x80210ACC | sin fuente |
| db/MapIdTable_unk | fn_80216CD0 | 0x80216CD0 | 98.2471 |
| db/MapIdTable_unk | fn_802174F4 | 0x802174F4 | 97.289856 |
| db/MapLink_unk | fn_802150F0 | 0x802150F0 | sin fuente |
| db/OptionParts_unk | fn_8020C5D4 | 0x8020C5D4 | sin fuente |
| db/OptionParts_unk | fn_8020D688 | 0x8020D688 | sin fuente |
| db/OptionParts_unk | fn_8020DB58 | 0x8020DB58 | sin fuente |
| db/PaletteTable_unk | fn_8020B2BC | 0x8020B2BC | 96.323586 |
| db/PaletteTable_unk | fn_8020BB0C | 0x8020BB0C | 99.40833 |
| db/PlayerParam_unk | fn_80218BD4 | 0x80218BD4 | 88.65568 |
| db/WeaponParam_unk | fn_80204E54 | 0x80204E54 | sin fuente |
| db/WeaponParam_unk | fn_80206F44 | 0x80206F44 | sin fuente |
| game/AreaEffConfig | __ct__Q23atn3CsvFPCci | 0x8004203C | sin fuente |
| game/ChaserGuard | __ct__11ChaserGuardFPQ23atn4TaskP13EnemyInfo_unk | 0x8017CB9C | 96.08541 |
| game/ChaserNpc | __ct__9ChaserNpcFPQ23atn4TaskP13EnemyInfo_unk | 0x801805A4 | 99.90795 |
| game/Em001Necromancer | fn_80244228 | 0x80244228 | sin fuente |
| game/Em003LesserDaemon | fn_8024A650 | 0x8024A650 | sin fuente |
| game/Em003LesserDaemon | fn_8024AD30 | 0x8024AD30 | sin fuente |
| game/Em005DoppelGanger | fn_80250A24 | 0x80250A24 | sin fuente |
| game/Em005DoppelGanger | fn_80250ECC | 0x80250ECC | sin fuente |
| game/Em101ElderZesya | fn_802558B0 | 0x802558B0 | sin fuente |
| game/Em101ElderZesya | fn_80256384 | 0x80256384 | sin fuente |
| game/Em101ElderZesyaSecond | fn_8025A060 | 0x8025A060 | sin fuente |
| game/Em101ElderZesyaSecond | fn_8025A67C | 0x8025A67C | sin fuente |
| game/Em101LittleZesya | fn_8025C998 | 0x8025C998 | sin fuente |
| game/Em101LittleZesya | fn_8025CFF4 | 0x8025CFF4 | sin fuente |
| game/Em101LittleZesyaSecond | fn_8025FE60 | 0x8025FE60 | sin fuente |
| game/Em101LittleZesyaSecond | fn_80260324 | 0x80260324 | sin fuente |
| game/Em201ZanGurg | fn_80262938 | 0x80262938 | sin fuente |
| game/Em201ZanGurg | fn_80262E6C | 0x80262E6C | sin fuente |
| game/Em201ZanGurgFinal | fn_80264DB4 | 0x80264DB4 | sin fuente |
| game/Em201ZanGurgFinal | fn_8026580C | 0x8026580C | sin fuente |
| game/Em202ForestSpider | fn_8027278C | 0x8027278C | sin fuente |
| game/Em202ForestSpider | fn_80272FB8 | 0x80272FB8 | sin fuente |
| game/Em202MothersSpider | fn_802799C0 | 0x802799C0 | sin fuente |
| game/Em202Spider | fn_8027C990 | 0x8027C990 | sin fuente |
| game/Em202Spider | fn_8027CF28 | 0x8027CF28 | sin fuente |
| game/Em202SpiderBase | fn_80282AE8 | 0x80282AE8 | sin fuente |
| game/Em202SpiderBase | fn_80283004 | 0x80283004 | sin fuente |
| game/Em203GreaterDaemon | fn_80287E7C | 0x80287E7C | sin fuente |
| game/Em203GreaterDaemonRed | fn_80293738 | 0x80293738 | sin fuente |
| game/Em203GreaterDaemonRedMp | fn_8029CB00 | 0x8029CB00 | sin fuente |
| game/Em204Mulue | fn_802A4BBC | 0x802A4BBC | sin fuente |
| game/Em204Mulue | fn_802A51F4 | 0x802A51F4 | sin fuente |
| game/Em204MulueMp | fn_802B23E8 | 0x802B23E8 | sin fuente |
| game/Em204MulueMp | fn_802B28BC | 0x802B28BC | sin fuente |
| game/Em205Ogre | fn_802B7AF8 | 0x802B7AF8 | sin fuente |
| game/Em209SmallGolem | fn_802CD3BC | 0x802CD3BC | sin fuente |
| game/Em301LShade | fn_802D0038 | 0x802D0038 | sin fuente |
| game/Em302Kraken | fn_802D7480 | 0x802D7480 | sin fuente |
| game/Em302KrakenMp | fn_802DDDB8 | 0x802DDDB8 | sin fuente |
| game/Em303KrakenFoot | fn_802E2640 | 0x802E2640 | sin fuente |
| game/Em303KrakenFootMp | fn_802E5CD0 | 0x802E5CD0 | sin fuente |
| game/Em305Vampire | fn_802E8580 | 0x802E8580 | sin fuente |
| game/Em305Vampire | fn_802E8B88 | 0x802E8B88 | sin fuente |
| game/Em305VampireMp | fn_802EC670 | 0x802EC670 | sin fuente |
| game/Em305VampireMp | fn_802EC9A8 | 0x802EC9A8 | sin fuente |
| game/Em306GurgBeast | fn_802EE9A8 | 0x802EE9A8 | sin fuente |
| game/Em308InvertCrab | fn_802F1524 | 0x802F1524 | sin fuente |
| game/Em308InvertCrab | fn_802F1A2C | 0x802F1A2C | sin fuente |
| game/Em308ParmCrab | fn_802F6E64 | 0x802F6E64 | sin fuente |
| game/Em308ParmCrab | fn_802F73FC | 0x802F73FC | sin fuente |
| game/Em309Ghoul | fn_802FC850 | 0x802FC850 | sin fuente |
| game/Em309GhoulSimple | fn_80306078 | 0x80306078 | sin fuente |
| game/Em310Gargoyle | fn_8030A144 | 0x8030A144 | sin fuente |
| game/Em312DarkServant | fn_8030DEB0 | 0x8030DEB0 | sin fuente |
| game/Em314Ivy | fn_80310C64 | 0x80310C64 | sin fuente |
| game/Em315Coffin | fn_80313AAC | 0x80313AAC | sin fuente |
| game/Em316LaserSphere | fn_803165EC | 0x803165EC | sin fuente |
| game/Em316LastLaser | fn_8031B6E0 | 0x8031B6E0 | sin fuente |
| game/EnemyJill | fn_8032E3DC | 0x8032E3DC | sin fuente |
| game/EnemyQuarkFirst | fn_80334FDC | 0x80334FDC | sin fuente |
| game/EnemyTasha | fn_80355DD8 | 0x80355DD8 | sin fuente |
| game/EnemyTrista | fn_8035C1B4 | 0x8035C1B4 | sin fuente |
| game/GameCamera_unk | fn_8037E12C | 0x8037E12C | sin fuente |
| game/LastCocoonQuadruped | fn_8045FE70 | 0x8045FE70 | sin fuente |
| game/MiniMapTask | fn_804AA490 | 0x804AA490 | sin fuente |
| game/NewCampPartsMapSelect | fn_80520634 | 0x80520634 | sin fuente |
| game/NewCampTask | fn_805303D0 | 0x805303D0 | sin fuente |
| game/NewCampTask | fn_805305D0 | 0x805305D0 | sin fuente |
| game/NewTitleTask | fn_80536CA8 | 0x80536CA8 | sin fuente |
| game/ShopBase | fn_80581A54 | 0x80581A54 | sin fuente |
| game/ShopColor | vf38__9ShopColorFPCci | 0x80587624 | 99.41298 |
| game/TutorialTask | fn_805BB654 | 0x805BB654 | sin fuente |
| game/TutorialTask | fn_805BCF04 | 0x805BCF04 | sin fuente |

### CsvTab: 78 sitios, 48 al 100%, 12 sin fuente (asm), 18 con fuente y <100%

| unidad | funcion | direccion | % |
|---|---|---|---|
| atn/LH_MotionSet | fn_80476ED4__Q23atn13LoadMotionSetFv | 0x80476ED4 | 95.559525 |
| atn/LipSync_unk | fn_8007A530__15LipSyncData_unkFPCci | 0x8007A530 | 99.51492 |
| atn/Model | fn_8008A00C | 0x8008A00C | sin fuente |
| atn/Motion_unk | fn_800A005C | 0x800A005C | sin fuente |
| chr/CharaEquip_unk | fn_80564190 | 0x80564190 | sin fuente |
| chr/CharaEquip_unk | fn_80564340 | 0x80564340 | sin fuente |
| chr/CharaEquip_unk | fn_80565064 | 0x80565064 | sin fuente |
| game/Arms | fn_8004439C__4ArmsFv | 0x8004439C | 99.83607 |
| game/CharaFaceControl | fn_8011EF14 | 0x8011EF14 | sin fuente |
| game/CharaTask | fn_801371AC__9CharaTaskFv | 0x801371AC | 99.67305 |
| game/CharaTask | fn_80138528__9CharaTaskFv | 0x80138528 | 99.4079 |
| game/GimmickStateCommand | fn_803EFCA8 | 0x803EFCA8 | sin fuente |
| game/MapTask | fn_8049ED90 | 0x8049ED90 | sin fuente |
| gmk/ArenaFloor | vf20__Q23gmk10ArenaFloorFv | 0x8041E89C | 89.59375 |
| gmk/Bat | vf20__Q23gmk3BatFv | 0x804038D0 | 99.80247 |
| gmk/CharaLight | vf20__Q23gmk10CharaLightFv | 0x80422F78 | 97.68041 |
| gmk/DropItem | fn_8040AA10 | 0x8040AA10 | sin fuente |
| gmk/FallingTower | vf20__Q23gmk12FallingTowerFv | 0x80410028 | 99.90196 |
| gmk/FragileObject | fn_803F2CD0 | 0x803F2CD0 | sin fuente |
| gmk/FragileObject | fn_803F33A4 | 0x803F33A4 | sin fuente |
| gmk/Leaf | vf20__Q23gmk4LeafFv | 0x8040134C | 99.35028 |
| gmk/LemonBucket | vf20__Q23gmk11LemonBucketFv | 0x80430428 | 99.13216 |
| gmk/MapLift | vf20__Q23gmk7MapLiftFv | 0x8042A360 | 83.40506 |
| gmk/MotionObject | vf24__Q23gmk12MotionObjectFv | 0x80414328 | 99.87805 |
| gmk/Ocean | vf20__Q23gmk5OceanFv | 0x804184BC | 99.82143 |
| gmk/RandomItem | vf20__Q23gmk10RandomItemFv | 0x804334EC | 98.78591 |
| gmk/RandomMerchant | vf20__Q23gmk14RandomMerchantFv | 0x8043239C | 99.31718 |
| gmk/RotaryRoom | vf20__Q23gmk10RotaryRoomFv | 0x804259B8 | 91.50467 |
| gmk/SwingObject | LoadSwing_unk__FPCciPQ23gmk14SwingParam_unkPiPQ23atn13EfpHandle_unk | 0x803FDFC0 | 90.12705 |
| gmk/WaitObject | vf20__Q23gmk10WaitObjectFv | 0x80419C00 | sin fuente |

### CsvPipe: 1 sitios, 0 al 100%, 1 sin fuente (asm), 0 con fuente y <100%

| unidad | funcion | direccion | % |
|---|---|---|---|
| atn/CsvBase_char_124_13_34 | fn_802078C4 | 0x802078C4 | sin fuente |

## Banco de pruebas

`t1.cpp` (CampSaveLoad::fn_8011B8D4, 92.1%), `t3.cpp` (cargador pequeno con `CsvU`, forma de GameSaveBanner),
`t2.cpp` (control con `atn::Csv` char: no debe cambiar). Verificacion en unidades reales: CampSaveLoad,
ptr_string_base_wchar_t_0 (TextTable_unk 95.5%), GameSaveBanner (fn_803B9324 97.5%), NewStaffRoll (97.8%),
TutorialTable_unk (97.7%), EquipParam_unk (96.2%).

## Intentos (todos sobre copia de `Csv.h` en scratch salvo que se diga)

Leyenda: "=" sin cambio respecto a la build actual; "vcall" = `mr r3, rX` antes de `lwz r12,0(r3)` (this en
registro salvado para la llamada virtual); "sep" = `mr r3, rX` antes de `bl fn_8005BCE8`.

1. `Csv` (char) con los mismos ternarios que `CsvU`: el char sigue sin registro (=). Los ternarios solos no bastan.
2. `memset((char*)this + 0x10, ...)` y `+0x610` en el ctor base: =.
3. Ternarios dentro del ctor base (`CsvBase(text, length, skip)` con `vf08(length ? text+skip : text, ...)`): =.
4. Helpers miembro `SkipBom(text,length)`/`SkipLen(length)` en la lista de inicializacion: PEOR, el ajuste se
   calcula ANTES de los memset (la forma `n ? t+1 : t` directa en la lista es la unica que deja los ternarios
   justo antes de la vcall, como el original).
5. `CsvBase& self = *this;` en el ctor base y `self.mField`, `self.m610`, `self.vf08`: vcall OK (`addi r30,r1,8`
   creado antes de los memset, `mr r3,r30` en la vcall), pero `fn_8005BCE8` sigue con `addi r3,r1,off` (falta
   el segundo uso). El char (t2) no cambia. Es el "cast de this" de basic_string: medio camino.
6. Referencias a los arrays (`T (&f)[0x400] = mField`): =.
7. (no usado)
8. Ceros via `unsigned long* w = (unsigned long*)this; w[1..3] = 0` en vez de la lista de inicializacion: =
   (y ademas cambia el orden de los stores de m630).
9. `CsvBase* self = this` con `self->...`: igual que 5.
10. `self` solo en la vcall (memset directos): = (el registro no aparece).
11. `this->mField`, `this->vf08`: =.
12. Bucle de ceros por puntero a palabra (como `__zero` de basic_string): =.
13. 5 + en `CsvU` `CsvU& self = *this; self.fn_8005BCE8(...)`: igual que 5 (el uso en el derivado no se suma).
14. Ctor de `CsvU` definido fuera de la clase con `inline`: =.
15. Ctor de `CsvBase` definido fuera de la clase (plantilla miembro, `inline`): =.
16. 5 + `CsvU* self = this` en el derivado: igual que 5.
17. Ctor base delega en `Init_unk(text,length)` inline: =.
18. 5 + `CsvBase<...>& self = *this` en el derivado (tipo base): igual que 5.
19. Solo el `self` de tipo base en el derivado: =.
20. `self` en la vcall + `self` en fn_8005BCE8 (sin memset via self): =.
21. Ctor de `CsvU` fuera de clase SIN `inline`: no se inlinea (sale `__ct__Q23atn4CsvUFPCwi` y la llamada); el
    original si lo inlinea. Descartado.
22. Ctor base fuera de clase sin `inline`: = (las plantillas miembro se autoinlinean igual).
23. `CsvBase<...>::fn_8005BCE8(x)` cualificado: =.
24. `((CsvBase*)this)->fn_8005BCE8(x)`: =.
25. `static void __Init(CsvBase* p, text, length)` con todo el cuerpo: =.
26. 5 + `static __SetSep(CsvBase* p, seps)` en el derivado: igual que 5.
27. Bucle de ceros via `unsigned long (&w)[4] = *(unsigned long (*)[4])this`: =.
28. Igual con puntero: =.
29. Ctor base `inline` definido despues de las clases derivadas: =.
30. Ctor base sin `inline` definido despues: =.
31. 5 + `SetSeparators_unk(seps)` miembro de la base con `self`: igual que 5.
32. Ctor base con tercer parametro `seps` y `self.fn_8005BCE8(seps)` dentro: vcall y sep en registro, PERO el
    store de `__vt__CsvU` queda despues de fn_8005BCE8 (orden incorrecto). Descartado.
33. Sin el `vf08` override en CsvU: = (el registro no depende del override).
34. `CsvU` sin ternarios: pierde los ternarios (obvio); =.
35. Clase intermedia `CsvUBase_unk` (ternarios) + `CsvU` (seps): =.
36. Ambos ctores fuera de clase sin `inline`: el de CsvU no se inlinea. Descartado.
37. Solo el base fuera de clase sin inline: =.
38. **`~CsvBase() {}` (destructor vacio en la base)**: vcall Y sep por registro salvado (`addi r30,r1,8` antes de
    los memset, `mr r3,r30` en los dos usos), exactamente como el original; el char (t2, y los 200+ sitios de
    `Csv`/`CsvTab`) no cambia. VALIDADO con `ninja -j1` completo: main.dol OK, lost 0, 0 bajadas, 12 subidas,
    +3 al 100% (GameSaveBanner::fn_803B9324, db/TutorialTable_unk fn_8021ED08, db/EquipParam_unk fn_8020EB0C).

Modelos minimos (`m*.cpp`, clase `B<T>`/`D` reducida) que acotan la regla:
- `m1`: `D : B(n ? t+1 : t, n ? n-1 : n)` sin destructor = sin registro; la forma de los ternarios no importa
  (`m17`: `bom()`/`boml()` inline los mueven antes de los memset; `n > 0`, `n == 0 ? :` cambian los branches).
- `m2`: ternarios dentro del ctor base (`if (sizeof(T)==2 && n)`): =.
- `m5`: en una clase de 3 palabras sin virtuales, `__zero()` (array ref) y el cast a `unsigned long*` SI crean
  el registro, `mL.a = 0; ...` no: confirma el hallazgo de basic_string, pero aqui el memset no se comporta igual.
- `m6`/`m7`: con `self` en el derivado y 2, 3 o 4 llamadas: el registro nunca aparece en el derivado (todas
  `addi r3,r1,8`); escrituras `self.m630 = ...` tampoco. El registro solo lo crea el ctor BASE.
- `m8`: en el base, `self` en `vf08` (solo o con un memset) crea el registro; `self` solo en los memset no.
- `m9`: locales `self` no usadas: =.
- `m10`: `vf08` llamado en el cuerpo del derivado (base sin texto): sin registro; con `B* p = this` si (pero el
  store de `__vt__D` se adelanta: orden distinto del original).
- `m12`/`m13`: pasar `this`/`*this` como parametro extra al ctor base: =. `B*& self` (por referencia a puntero)
  lo guarda en PILA, no en registro.
- `m15`: ctor de D fuera de clase sin `inline` no se autoinlinea (ni con `-inline auto`); con `inline` si.
- `m16`: envolver D en otra clase E: =.
- `m18`: ctores como plantillas miembro: =.
- `m20`: `~B() {}` vacio: registro en vcall y sep (=38). `~D() {}` vacio en el derivado: no.

## Hallazgo MWCC (una linea, estilo CLAUDE_lite)

Un destructor vacio definido en la clase base (`~CsvBase() {}`) hace que, al inlinear el ctor de una derivada
cuyo ctor base recibe dos ternarios (`n ? t+1 : t, n ? n-1 : n`), la direccion del objeto se calcule UNA vez en
un registro salvado antes de los memset y se reutilice en la llamada virtual y en la siguiente llamada del
derivado (`mr r3, rX`), como en 0x8011B8D4; sin ternarios (lectores `char`) el destructor no cambia nada.
Probablemente el original tenia `virtual ~CsvBase() {}`... no: la vtable solo tiene RTTI, 0, vf08 (y un 0 final
en la base), sin destructor; un destructor no virtual vacio (o implicito pero "no trivial" por algun miembro) es
lo compatible con el DOL.

## Lo que NO funciona (no repetir)

- Cast de `this` a `unsigned long*`/array de palabras para los ceros (la receta de basic_string): nada.
- `memset` sobre `(char*)this + off`: nada.
- Alias `CsvBase& self = *this` / `CsvBase* self = this` en el ctor base: solo la vcall (medio camino), y los
  alias en el derivado (`CsvU& self`) no se suman.
- Helpers inline/static para los ternarios o para `fn_8005BCE8`: nada o peor (ternarios antes de los memset).
- Ctores fuera de la clase (con y sin `inline`, antes o despues de las derivadas), plantillas miembro, clase
  intermedia, pasar `this` como parametro al ctor base, bucles de ceros: nada.
- Ctor base con los separadores como parametro: registro OK pero orden de stores incorrecto.
