#ifndef ATN_DEBUGMESSAGES_UNK_H
#define ATN_DEBUGMESSAGES_UNK_H

// inferred: debug message list (atn/FileUtil_unk.cpp; instance lbl_8087EEB8); name invented
// (the full layout is in atn/FileUtil_unk.cpp, the only unit that builds it)
struct DebugMessages_unk {
    void fn_800697D8(const char* message); // inferred: adds a message (red)
    void fn_800698DC(const char*);         // inferred: "LoadError: " + path
    void fn_80069D48(const char* message); // inferred: adds a message (yellow)
    void fn_80069E50(int);                 // inferred: draws and ages the messages
};
extern "C" DebugMessages_unk* lbl_8087EEB8;

#endif
