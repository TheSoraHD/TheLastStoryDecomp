# escribir-20261116 cola W3b (Event, segunda pasada)

Inicio: 2026-11-17 04:16 CET. Tope: 06:46.

| funcion | antes | despues |
|---|---|---|
| Sequence 0x80544650 `__vec_event_unk<Actor>::__grow_by_unk` | 99,90 % | **100 %** |
| Sequence 0x80544748 `__vec_event_unk<Path>::__grow_by_unk` | 99,90 % | **100 %** |
| Sequence 0x80545B90 `__vec_event_unk<Path>::__reserve_unk` | 99,86 % | **100 %** |
| Sequence 0x80545C3C `__vec_event_unk<Actor>::__reserve_unk` | 99,86 % | **100 %** |

## Para el port
- Sequence.cpp: instanciacion explicita `template std::string::basic_string(const std::string&, size_type, size_type);`
  entre fn_8053F818 y fn_8054011C (fuera de `namespace Event`). Sin cambio de comportamiento.

## Decisiones tomadas sin el usuario

## Hallazgos
- Un mensaje de `basic_string` que sale en el pool del original antes de lo que da la instanciacion implicita (al
  final de la unidad) se coloca con una instanciacion explicita del miembro que lo usa en ese punto del fuente
  (`template std::string::basic_string(const std::string&, size_type, size_type);`, como en atn/FileList y
  D2A_Manager): Event/Sequence "out_of_range" antes de "vector length error" (escribir-20261116 qW3b).

## Bloqueos

## Cambios de splits/symbols
