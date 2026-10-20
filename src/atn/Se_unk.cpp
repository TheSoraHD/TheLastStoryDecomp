// inferred: sound effect helpers (play a SE by name, load/release sound groups) on top of the sound
// engine lbl_8087EE90 and atn::SoundManager lbl_8087EFE8. File name invented.
#include "atn/SoundRef.h"
extern "C" unsigned long strlen(const char* s);

// inferred: sound engine of the game (lbl_8087EE90; see atn/Sound3dListener.cpp); only the members used here
struct SoundEngineSe_unk {
    void* m00;
    void* m04; // inferred: nonzero once the sound archive is open
};

// inferred: atn::SoundManager (lbl_8087EFE8); only the members used here
struct SoundManagerSe_unk {
    unsigned char m00[0x34A0];
    int m34A0;  // 0x34A0: inferred: 3D sounds enabled
    unsigned char m34A4[0x34B4 - 0x34A4];
    int m34B4;  // 0x34B4: inferred: SE enabled (volume 1 or 0)
    int m34B8;  // 0x34B8: inferred: groups may be loaded
};

// inferred: sound object created by the manager (atn::SoundObject plus the fields used here)
struct SoundObjectSe_unk {
    void* m00;
    void* mSound; // 0x04
    unsigned char m08[0xA8 - 0x08];
    int mA8; // 0xA8
};

extern "C" SoundEngineSe_unk* lbl_8087EE90;
extern "C" SoundManagerSe_unk* lbl_8087EFE8;

extern "C" int fn_80049D74(SoundEngineSe_unk* engine, const char* group);  // inferred: group id
extern "C" int fn_800489F4(SoundEngineSe_unk* engine, const char* group);  // inferred: loads a group
extern "C" void fn_80048F8C(SoundEngineSe_unk* engine, int arg);
extern "C" void fn_80049268(SoundEngineSe_unk* engine, int arg);
extern "C" void fn_8004937C(SoundEngineSe_unk* engine, const char* group); // inferred: releases a group
extern "C" void fn_80049654(SoundEngineSe_unk* engine);
extern "C" int fn_800496C0(SoundEngineSe_unk* engine, const char* group);
extern "C" void fn_800498A4(SoundEngineSe_unk* engine);
extern "C" void fn_800498D4(SoundEngineSe_unk* engine);
extern "C" unsigned int fn_80049B74(SoundEngineSe_unk* engine, const char* name); // inferred: sound id of a name
extern "C" int fn_80049C3C(SoundEngineSe_unk* engine, const char* name);
extern "C" float fn_80049E44(SoundEngineSe_unk* engine, SoundObjectSe_unk* obj);
extern "C" SoundObjectSe_unk* fn_800D0AB0(SoundManagerSe_unk* mgr, unsigned int id);
extern "C" int fn_800CFDA0(SoundManagerSe_unk* mgr, int arg);
extern "C" int fn_800D0C1C(SoundManagerSe_unk* mgr, SoundObjectSe_unk* obj);
extern "C" void fn_800D19FC(SoundManagerSe_unk* mgr, SoundObjectSe_unk* obj, float arg);
extern "C" void fn_800CA7D4(SoundObjectSe_unk* obj, int arg);
extern "C" void fn_800C9010(SoundObjectSe_unk* obj, unsigned int id);
extern "C" void fn_800CA0AC(SoundObjectSe_unk* obj, float volume, int arg);
extern "C" void fn_800CA2FC(SoundObjectSe_unk* obj, float x, float y, float z);
extern "C" void fn_800C8BA4(SoundObjectSe_unk* obj, const char* name, const void* pos);
extern "C" void fn_800C8780(SoundObjectSe_unk* obj, const char* name);
extern "C" void fn_80709AD0(void* sound, int frames); // inferred: nw4r::snd::detail::BasicSound::Stop

extern "C" atn::SoundRef fn_800C32E0(unsigned int id, int arg2, int arg3, float volume);

extern "C" int fn_800C3094(const char* group) {
    // inferred: the engine returns index + 1 (0 when the group does not exist)
    bool valid = fn_80049D74(lbl_8087EE90, group) - 1 != -1;
    if (valid) {
        if (lbl_8087EFE8 != 0 && lbl_8087EFE8->m34B8 != 0) {
            return fn_800489F4(lbl_8087EE90, group);
        }
    }
    return 0;
}

extern "C" void fn_800C310C(int arg) {
    fn_80048F8C(lbl_8087EE90, arg);
}

extern "C" void fn_800C3118(int arg) {
    fn_80049268(lbl_8087EE90, arg);
}

extern "C" void fn_800C3124(const char* group) {
    // inferred: the engine returns index + 1 (0 when the group does not exist)
    bool valid = fn_80049D74(lbl_8087EE90, group) - 1 != -1;
    if (valid) {
        fn_8004937C(lbl_8087EE90, group);
    }
}

extern "C" void fn_800C317C() {
    fn_80049654(lbl_8087EE90);
}

extern "C" int fn_800C3184(const char* group) {
    // inferred: the engine returns index + 1 (0 when the group does not exist)
    bool valid = fn_80049D74(lbl_8087EE90, group) - 1 != -1;
    if (valid) {
        return fn_800496C0(lbl_8087EE90, group);
    }
    return 0;
}

extern "C" void fn_800C31E4() {
    fn_800498A4(lbl_8087EE90);
}

extern "C" void fn_800C31EC() {
    fn_800498D4(lbl_8087EE90);
}

// inferred: sound system ready
inline bool IsReady_unk() {
    return lbl_8087EE90->m04 != 0 && lbl_8087EFE8 != 0;
}

// inferred: non empty name
inline bool IsValidName_unk(const char* name) {
    bool valid = false;
    if (name != 0 && strlen(name) != 0) {
        valid = true;
    }
    return valid;
}

// inferred: plays the sound effect `name` (arg3 is not used)
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume) {
    atn::SoundRef ref;
    if (lbl_8087EE90->m04 == 0 || lbl_8087EFE8 == 0 || !IsValidName_unk(name)) {
        return ref;
    }
    return fn_800C32E0(fn_80049B74(lbl_8087EE90, name), arg2, -1, volume);
}

// inferred: plays the sound effect `id`
extern "C" atn::SoundRef fn_800C32E0(unsigned int id, int arg2, int arg3, float volume) {
    atn::SoundRef ref;
    if (lbl_8087EE90->m04 == 0 || lbl_8087EFE8 == 0 || id == 0) {
        return ref;
    }

    SoundObjectSe_unk* obj = fn_800D0AB0(lbl_8087EFE8, id);
    if (obj == 0) {
        return ref;
    }
    obj->mA8 = fn_800CFDA0(lbl_8087EFE8, arg3 < 0 ? 1 : arg3);
    fn_800CA7D4(obj, arg2);
    fn_800C9010(obj, id);
    fn_800CA0AC(obj, volume, 0);
    if (lbl_8087EFE8->m34B4 != 0) {
        fn_800CA2FC(obj, 1.0f, 1.0f, 1.0f);
    } else {
        fn_800CA2FC(obj, 0.0f, 0.0f, 0.0f);
    }
    float f = fn_80049E44(lbl_8087EE90, obj);
    if (fn_800D0C1C(lbl_8087EFE8, obj) == 0) {
        if (obj->mSound != 0) {
            fn_80709AD0(obj->mSound, 0);
        }
    } else {
        fn_800D19FC(lbl_8087EFE8, obj, f);
        ref = (atn::SoundObject*)obj;
    }
    return ref;
}

// inferred: plays the 3D sound effect `name` at `pos`
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume) {
    atn::SoundRef ref;
    if (lbl_8087EE90->m04 == 0 || lbl_8087EFE8 == 0 || !IsValidName_unk(name)) {
        return ref;
    }

    SoundObjectSe_unk* obj = fn_800D0AB0(lbl_8087EFE8, fn_80049B74(lbl_8087EE90, name));
    if (obj == 0) {
        return ref;
    }
    obj->mA8 = fn_800CFDA0(lbl_8087EFE8, arg3 < 0 ? 1 : arg3);
    fn_800CA7D4(obj, arg2);
    if (lbl_8087EFE8->m34A0 != 0) {
        fn_800C8BA4(obj, name, pos);
    } else {
        fn_800C8780(obj, name);
    }
    fn_800CA0AC(obj, volume, 0);
    fn_800CA2FC(obj, 1.0f, 1.0f, 1.0f);
    if (fn_80049C3C(lbl_8087EE90, name) != 0) {
        fn_800CA2FC(obj, 0.0f, 0.0f, 0.0f);
    }
    float f = fn_80049E44(lbl_8087EE90, obj);
    if (fn_800D0C1C(lbl_8087EFE8, obj) == 0) {
        if (obj->mSound != 0) {
            fn_80709AD0(obj->mSound, 0);
        }
    } else {
        fn_800D19FC(lbl_8087EFE8, obj, f);
        ref = (atn::SoundObject*)obj;
    }
    return ref;
}
