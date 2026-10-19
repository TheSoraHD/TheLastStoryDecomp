#include "math/Vec.h"

// Minimal declarations of the NW4R classes used here (the game build does not include the NW4R
// headers). LinkListImpl::Insert/Erase return an iterator in NW4R; the result is unused here and
// the mangled names do not include it.
namespace nw4r {
namespace db {
void Panic(const char* file, int line, const char* fmt, ...);
} // namespace db

namespace ut {
struct LinkListNode {
    LinkListNode* mNext;
    LinkListNode* mPrev;
};
namespace detail {
class LinkListImpl {
public:
    class Iterator {
    public:
        explicit Iterator(LinkListNode* p) : mPointer(p) {}
        LinkListNode* mPointer;
    };
    Iterator GetEndIter() { return Iterator(&mEndNode); }
    void Insert(Iterator it, LinkListNode* p);
    void Erase(LinkListNode* p);

    unsigned long mSize;
    LinkListNode mEndNode;
};
} // namespace detail
} // namespace ut

namespace snd {
class SoundHandle {
public:
    SoundHandle() : mSound(0) {}
    ~SoundHandle() { DetachSound(); }
    void Stop(int frames);
    void DetachSound();

    void* mSound; // detail::BasicSound*
};
} // namespace snd
} // namespace nw4r

// NW4R functions without a symbol name yet
extern "C" void fn_80715B20(void* self);                    // inferred: nw4r::snd::Sound3DListener::Sound3DListener
extern "C" void fn_80715BA0(void* self, const Mtx m);       // inferred: nw4r::snd::Sound3DListener::SetMatrix
extern "C" void fn_80714930(void* self, void* player, void* manager); // inferred: nw4r::snd::Sound3DActor ctor
extern "C" void fn_80709AD0(void* sound, int frames);       // inferred: nw4r::snd::detail::BasicSound::Stop
extern "C" void C_MTXLookAt(Mtx m, const atn::Vector3* camPos, const atn::Vector3* camUp, const atn::Vector3* target);

// The constants 0.0f and 1.0f are shared with the code that follows this split (0x800C8278), so
// they are referenced by address.
extern "C" const float lbl_80881168; // 0.0f
extern "C" const float lbl_8088116C; // 1.0f
// inferred: initial values of the three SoundCore fades (.sdata: 1.0f, 0.0f, 0.0f). They are left
// outside the split: MWCC places zero-initialised variables in .sbss, not in .sdata.
extern "C" float lbl_8087D930;
extern "C" float lbl_8087D934;
extern "C" float lbl_8087D938;

namespace nw4r {
namespace snd {

// inferred: 0x6C bytes, list node at 0x64. The constructor has no symbol name yet, so it calls
// fn_80715B20 directly.
class Sound3DListener {
public:
    Sound3DListener() { fn_80715B20(this); }

    unsigned char m00[0x64];
    ut::LinkListNode mLinkNode; // 0x64
};

// inferred: 0x84 bytes. The constructor has no symbol name yet (fn_80714930).
class Sound3DActor {
public:
    Sound3DActor(void* player, void* manager) { fn_80714930(this, player, manager); }
    ~Sound3DActor();

    unsigned char m00[0x84];
};

} // namespace snd
} // namespace nw4r

inline void nw4r::snd::SoundHandle::Stop(int frames) {
    if (mSound != 0) {
        fn_80709AD0(mSound, frames);
    }
}

namespace atn {

// inferred: nw4r::snd::Sound3DManager (listener list at 0x0C)
struct Snd3DManager_unk {
    unsigned char m00[0x0C];
    nw4r::ut::detail::LinkListImpl mListenerList; // 0x0C

    // inferred: inline ut::LinkList<Sound3DListener, 0x64> members with the NW4R pointer assert
    static nw4r::ut::LinkListNode* GetNodeFromPointer(nw4r::snd::Sound3DListener* p) {
        if (p == 0) {
            nw4r::db::Panic("LinkList.h", 0x233, "NW4R:Pointer must not be NULL (p)");
        }
        return &p->mLinkNode;
    }
    void Insert(nw4r::ut::detail::LinkListImpl::Iterator it, nw4r::snd::Sound3DListener* p) {
        mListenerList.Insert(it, GetNodeFromPointer(p));
    }
    void PushBack(nw4r::snd::Sound3DListener* p) { Insert(mListenerList.GetEndIter(), p); }
    void Erase(nw4r::snd::Sound3DListener* p) { mListenerList.Erase(GetNodeFromPointer(p)); }
};

// inferred: sound engine of the game (lbl_8087EE90); only the members used here
struct SoundEngine_unk {
    unsigned char m00[0x1D8];
    unsigned char mPlayer[0x364 - 0x1D8];  // 0x1D8: nw4r::snd::SoundArchivePlayer
    Snd3DManager_unk m364;                 // 0x364: nw4r::snd::Sound3DManager
};

} // namespace atn

extern "C" atn::SoundEngine_unk* lbl_8087EE90;

namespace atn {

class Sound3dListener {
public:
    Sound3dListener();
    virtual ~Sound3dListener();
    void fn_800C7F08(const Vector3& pos, const Vector3& rot); // inferred: sets the position and rotation

    nw4r::snd::Sound3DListener mListener; // 0x04
    Vector3 mPos;                // 0x70
    Vector3 mRot;                // 0x7C
};

class Sound3dActor {
public:
    Sound3dActor() : mActor(lbl_8087EE90->mPlayer, &lbl_8087EE90->m364) {}
    virtual ~Sound3dActor();

    nw4r::snd::Sound3DActor mActor; // 0x04
};

// inferred: fade value (0x20 bytes); names invented
struct SoundFade_unk {
    SoundFade_unk(const float& value)
        : m00(lbl_80881168), m04(lbl_80881168), m08(lbl_8088116C), m0C(false), m10(value), m14(value),
          m18(value), m1C(m14 - m10) {}

    float m00;
    float m04;
    float m08;
    bool m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
};

class SoundCore {
public:
    SoundCore();
    virtual ~SoundCore();

    nw4r::snd::SoundHandle mHandle; // 0x04
    Sound3dActor m08;
    int m90;
    int m94;
    int m98;
    int m9C;
    int mA0;
    int mA4;
    int mA8;
    int mAC;
    int mB0;
    int mB4;
    float mB8;
    float mBC;
    float mC0;
    int mC4;
    int mC8;
    float mCC;
    int mD0;
    int mD4;
    float mD8;
    float mDC;
    float mE0;
    float mE4;
    int mE8;
    SoundFade_unk mEC;
    SoundFade_unk m10C;
    SoundFade_unk m12C;
};

Sound3dListener::Sound3dListener()
    : mPos(lbl_80881168, lbl_80881168, lbl_80881168), mRot(lbl_80881168, lbl_80881168, lbl_80881168) {
    lbl_8087EE90->m364.PushBack(&mListener);
}

Sound3dListener::~Sound3dListener() {
    lbl_8087EE90->m364.Erase(&mListener);
}

void Sound3dListener::fn_800C7F08(const Vector3& pos, const Vector3& rot) {
    mPos = pos;
    mRot = rot;
    Matrix3x4 m;
    Matrix3x4 rotY;
    Matrix3x4 view;
    Vector3 dir(lbl_80881168, lbl_80881168, lbl_8088116C);
    rotY.fn_8013A13C(mRot.y);
    PSMTXMultVec(rotY.m, &dir, &dir);
    Vector3 target;
    Vector3 up(lbl_80881168, lbl_8088116C, lbl_80881168);
    target.Set_unk(mPos.x + dir.x, mPos.y + dir.y, mPos.z + dir.z);
    C_MTXLookAt(view.m, &mPos, &up, &target);
    m = view;
    fn_80715BA0(&mListener, m.m);
}

Sound3dActor::~Sound3dActor() {}

SoundCore::SoundCore()
    : m90(-1), m98(0), m9C(0), mA0(0), mA4(0), mA8(0), mAC(0), mB0(0), mB4(0), mB8(lbl_8088116C),
      mBC(lbl_8088116C), mC0(lbl_80881168), mC8(0), mCC(lbl_80881168), mD4(0), mD8(lbl_80881168),
      mDC(lbl_80881168), mE0(lbl_80881168), mE4(lbl_80881168), mE8(0), mEC(lbl_8087D930), m10C(lbl_8087D934),
      m12C(lbl_8087D938) {}

SoundCore::~SoundCore() {
    mHandle.Stop(0);
    mHandle.DetachSound();
    m9C = 0;
    mA0 = 0;
    mA4 = 0;
}

} // namespace atn
