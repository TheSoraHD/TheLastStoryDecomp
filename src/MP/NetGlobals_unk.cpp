// inferred file name (splits-auto): only the <iostream> __sinit and three multiplayer globals
// that atn/PacketPack (MP::ManagerMP) writes and the MP menus read.
#include "std/ios_init.h"

extern "C" {
void* lbl_8087F628; // inferred: multiplayer data (MpData_unk* in the MP menus)
void* lbl_8087F62C; // inferred: set together with lbl_8087F628 by atn/PacketPack
void* lbl_8087F630; // inferred: read by atn/PacketPack and GameLoadTask
}
