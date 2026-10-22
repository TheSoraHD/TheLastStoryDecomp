# Lote close-units - cola C (chr y game)

Una linea por unidad procesada.

- chr/ControlAttractHateCharge: 7/8. Subida `ControlOverdrive::ControlOverdrive` (0x801B77DC): la llamada a fn_8021FD7C es el metodo `VoiceEntry_unk::fn_8021FD7C(int) const` (atn/ptr_string_base_char_0.cpp), declarado localmente y llamado sobre `(VoiceEntry_unk*)mChara->m7C`. Pendiente `ControlOverdrive::vf0C` 99.87%: la comparacion muerta (0.2 contra el tiempo de frame) usa f0/f1 al reves; 10 variantes probadas (orden de operandos, >=, !<=, ternario, helpers min, locales para 0.1/0.2, resta separada), ninguna. No enlaza.
