#include "atn/SoundRef.h"
#include "math/Vec.h"

namespace atn {

class SoundCore {
public:
    int fn_800CA028();
    void fn_800CA098();
    void fn_800CA0AC(float, int);
    void fn_800CA1D4(int, unsigned long, int);
    void fn_800CA2FC(float, float, float);
    void fn_800CA42C(float, int);
    void fn_800CA5E8(float, int);
    void fn_800CA798(int, float);
    void fn_800CA7E8(const Vector3*);
    void fn_800C82A0(int);
    void fn_800CA144(int);
    void fn_800CA514(int);
    void fn_800CA6C4(int);
    void fn_800CA3D4();
};
} // namespace atn
extern "C" void fn_80709F20(void*);
extern "C" int fn_8070ABE0(void*);
extern "C" void fn_80709AD0(void*, int);
extern "C" void fn_8070AD70(void*, float);
extern "C" char lbl_807C7030[];

namespace atn {

SoundRef::SoundRef() : mPtr(0) {}

// NONMATCHING: 61.54% - el constructor de copia emite secuencia de limpieza distinta.
SoundRef::SoundRef(const SoundRef& other) : mPtr(other.mPtr) {
    if (mPtr != 0) {
        ++mPtr->mRefCount;
    }
}

SoundRef::~SoundRef() {
    Reset();
}

// NONMATCHING: 99.33% - difiere la asignacion de registros del contador de referencias.
SoundRef& SoundRef::operator=(SoundObject* p) {
    Reset();
    mPtr = p;
    if (mPtr != 0) {
        ++mPtr->mRefCount;
    }
    return *this;
}

// NONMATCHING: 72.19% - el orden de lectura y ajuste del puntero no coincide.
SoundRef& SoundRef::operator=(const SoundRef& other) {
    return *this = other.mPtr;
}

void SoundRef::Reset() {
    if (mPtr != 0) {
        --mPtr->mRefCount;
        mPtr = 0;
    }
}
} // namespace atn

extern "C" int fn_800CB4A4(atn::SoundRef* self) {
    return self->mPtr != 0 ? ((self->mPtr->m9C >> 3) & 1) : 0;
}

// NONMATCHING: 59.00% - no coincide la normalizacion del estado booleano.
extern "C" bool fn_800CB4C4(atn::SoundRef* self) {
    if (self->mPtr == 0) return false;
    return self->mPtr->mA0;
}

extern "C" int fn_800CB4EC(atn::SoundRef* self) {
    return self->mPtr != 0 ? reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA028() : 0;
}

extern "C" void fn_800CB504(atn::SoundRef* self) {
    if (self->mPtr != 0) reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA098();
}

extern "C" void fn_800CB518(atn::SoundRef* self) {
    if (self->mPtr != 0 && self->mPtr->mSound != 0) fn_80709F20(self->mPtr->mSound);
}

// NONMATCHING: 57.14% - la conversion del resultado preparado queda distinta.
extern "C" bool fn_800CB538(atn::SoundRef* self) {
    if (self->mPtr == 0) return false;
    if (self->mPtr->mSound == 0) return false;
    return fn_8070ABE0(self->mPtr->mSound);
}

// NONMATCHING: 59.00% - no coincide la normalizacion del puntero de sonido.
extern "C" bool fn_800CB58C(atn::SoundRef* self) {
    if (self->mPtr == 0) return false;
    return self->mPtr->mSound;
}

extern "C" void fn_800CB5B4(atn::SoundRef* self, float a1, int a2) {
    if (self->mPtr != 0) reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA0AC(a1, a2);
}

extern "C" void fn_800CB5C8(atn::SoundRef* self, int frames, int detach) {
    if (self->mPtr != 0 && self->mPtr->mSound != 0) {
        fn_80709AD0(self->mPtr->mSound, frames);
    }
    if (detach == 0) self->Reset();
}

extern "C" void fn_800CB640(atn::SoundRef* self, int a1, unsigned long a2, int a3) {
    if (self->mPtr != 0) reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA1D4(a1, a2, a3);
}

extern "C" void fn_800CB654(atn::SoundRef* self, float a1, float a2, float a3) {
    if (self->mPtr != 0) reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA2FC(a1, a2, a3);
}

extern "C" void fn_800CB668(atn::SoundRef* self, float value) {
    if (self->mPtr != 0 && self->mPtr->mSound != 0) fn_8070AD70(self->mPtr->mSound, value);
}

extern "C" void fn_800CB688(atn::SoundRef* self, int a1, float a2) {
    if (self->mPtr != 0) reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA42C(a2, a1);
}

extern "C" void fn_800CB69C(atn::SoundRef* self, int a1, float a2) {
    if (self->mPtr != 0) reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA5E8(a2, a1);
}

extern "C" void fn_800CB6B0(atn::SoundRef* self, int a1, float a2) {
    if (self->mPtr != 0) reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA798(a1, a2);
}

extern "C" void* fn_800CB6C4(atn::SoundRef* self) {
    return self->mPtr != 0 ? reinterpret_cast<char*>(self->mPtr) + 0x70 : lbl_807C7030;
}

extern "C" void fn_800CB6E4(atn::SoundRef* self, const void* value) {
    if (self->mPtr != 0) reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA7E8(static_cast<const atn::Vector3*>(value));
}

extern "C" int fn_800CB6F8(atn::SoundRef* self) {
    return self->mPtr != 0 ? self->mPtr->m98 : 0;
}

extern "C" void fn_800CB714(void* self, int a1, unsigned long a2, int a3) {
    reinterpret_cast<atn::SoundCore*>(self)->fn_800CA1D4(a1, a2, a3);
}
extern "C" void fn_800CB7F0(void* self, int value);
extern "C" void fn_800CB7F4(void* self, int value);
extern "C" void fn_800CB7F8(void* self, int value);
extern "C" void fn_800CB7FC(void* self);

// NONMATCHING: 94.29% - MWCC genera otra secuencia para convertir frames == 0 a int.
extern "C" void fn_800CB718(atn::SoundObject* self, int frames, int onlyWhenUnused) {
    if (onlyWhenUnused && self->mRefCount > 0) return;
    if (self->mSound != 0) fn_80709AD0(self->mSound, frames);
    reinterpret_cast<atn::SoundCore*>(self)->fn_800C82A0(frames == 0);
}

// NONMATCHING: 89.62% - falta reproducir el test de flags y la conversion a int del original.
extern "C" void fn_800CB788(atn::SoundObject* self, int frames) {
    if ((self->m9C & 8) != 0) {
        if (self->mSound != 0) fn_80709AD0(self->mSound, frames);
        reinterpret_cast<atn::SoundCore*>(self)->fn_800C82A0(frames == 0);
    }
}

extern "C" void fn_800CB7F0(void* self, int value) { reinterpret_cast<atn::SoundCore*>(self)->fn_800CA144(value); }

extern "C" void fn_800CB7F4(void* self, int value) { reinterpret_cast<atn::SoundCore*>(self)->fn_800CA514(value); }

extern "C" void fn_800CB7F8(void* self, int value) { reinterpret_cast<atn::SoundCore*>(self)->fn_800CA6C4(value); }

extern "C" void fn_800CB7FC(void* self) { reinterpret_cast<atn::SoundCore*>(self)->fn_800CA3D4(); }

void operator delete(void*);

extern "C" void fn_800CDD40(char* state) {
    *state = 0;
}

extern "C" void* fn_800CDD4C(void* ptr, int shouldDelete) {
    if (ptr != 0 && shouldDelete > 0) {
        ::operator delete(ptr);
    }
    return ptr;
}
extern "C" int fn_800D089C(void* manager, int category, int onlyIdle);

extern "C" void fn_800D0180(void* closure, void* arg) {
    typedef void (*Callback)(void*, void*);
    Callback callback = reinterpret_cast<Callback>(*(void**)closure);
    callback(arg, *(void**)((char*)closure + 4));
}

extern "C" void fn_800D07E8(void* closure, void* arg) {
    typedef void (*Callback)(void*);
    Callback callback = reinterpret_cast<Callback>(*(void**)closure);
    callback(arg);
}

// inferred: each pool entry contains two SoundCore records, each 0x14C bytes.
// NONMATCHING: 85.17% - la validacion del indice y el registro del recorrido difieren.
extern "C" int fn_800D089C(void* manager, int category, int onlyIdle) {
    char* categoryParam;
    if (category < 0) goto invalidCategory;
    if (category >= 0x20) goto invalidCategory;
    categoryParam = static_cast<char*>(manager) + 0x2B98 + category * 0x18;
    goto categoryReady;
invalidCategory:
    categoryParam = 0;
categoryReady:

    int count = 0;
    char* core = static_cast<char*>(manager) + 4;
    for (int i = 0; i < 0x10; ++i, core += 0x298) {
        if (*(void**)(core + 4) != 0 &&
            (*(unsigned int*)(core + 0x9C) & 8) == 8 &&
            (categoryParam == 0 || *(void**)(core + 0xAC) == categoryParam) &&
            (onlyIdle == 0 || *(int*)(core + 0xA0) == 0)) {
            ++count;
        }

        char* secondCore = core + 0x14C;
        if (*(void**)(secondCore + 4) != 0 &&
            (*(unsigned int*)(secondCore + 0x9C) & 8) == 8 &&
            (categoryParam == 0 || *(void**)(secondCore + 0xAC) == categoryParam) &&
            (onlyIdle == 0 || *(int*)(secondCore + 0xA0) == 0)) {
            ++count;
        }
    }
    return count;
}
