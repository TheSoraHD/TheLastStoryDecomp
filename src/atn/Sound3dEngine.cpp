#include "math/Vec.h"

// Minimal declarations of the NW4R classes used here (the game build does not include the NW4R
// headers).
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
// inferred: layout of NW4R ut_LinkList.h (only what is used here)
class LinkListImpl {
public:
    class Iterator {
    public:
        explicit Iterator(LinkListNode* p) : mPointer(p) {}
        LinkListNode* operator->() const { return mPointer; }
        friend bool operator==(Iterator a, Iterator b) { return a.mPointer == b.mPointer; }

        LinkListNode* mPointer;
    };
    class ConstIterator {
    public:
        ConstIterator(Iterator it) : mPointer(it.mPointer) {}
        ConstIterator& operator++() {
            mPointer = mPointer->mNext;
            return *this;
        }
        const LinkListNode* operator->() const { return mPointer; }
        friend bool operator==(ConstIterator a, ConstIterator b) { return a.mPointer == b.mPointer; }

        const LinkListNode* mPointer;
    };

    Iterator GetBeginIter() { return Iterator(mEndNode.mNext); }
    Iterator GetEndIter() { return Iterator(&mEndNode); }
    unsigned long GetSize() const { return mSize; }

    unsigned long mSize;
    LinkListNode mEndNode;
};
} // namespace detail

// inferred: ut::LinkList<T, Ofs> with the NW4R pointer asserts (LinkList.h lines 0x1BF and 0x242)
template <typename T, int Ofs>
class LinkList : public detail::LinkListImpl {
public:
    class Iterator {
    public:
        explicit Iterator(detail::LinkListImpl::Iterator it) : it_(it) {}
        detail::LinkListImpl::Iterator it_;
    };
    class ConstIterator {
    public:
        ConstIterator(Iterator it) : it_(it.it_) {}
        const T& operator*() const {
            const T* p = this->operator->();
            if (p == 0) {
                db::Panic("LinkList.h", 0x1BF, "NW4R:Pointer must not be NULL (p)");
            }
            return *p;
        }
        const T* operator->() const { return GetPointerFromNode(it_.operator->()); }
        ConstIterator& operator++() {
            ++it_;
            return *this;
        }
        friend bool operator==(ConstIterator a, ConstIterator b) { return a.it_ == b.it_; }
        friend bool operator!=(ConstIterator a, ConstIterator b) { return !(a == b); }

        detail::LinkListImpl::ConstIterator it_;
    };

    static const T* GetPointerFromNode(const LinkListNode* p) {
        if (p == 0) {
            db::Panic("LinkList.h", 0x242, "NW4R:Pointer must not be NULL (p)");
        }
        return reinterpret_cast<const T*>(reinterpret_cast<const unsigned char*>(p) - Ofs);
    }

    Iterator GetBeginIter() { return Iterator(detail::LinkListImpl::GetBeginIter()); }
    Iterator GetEndIter() { return Iterator(detail::LinkListImpl::GetEndIter()); }
    ConstIterator GetBeginIter() const { return ConstIterator(const_cast<LinkList*>(this)->GetBeginIter()); }
    ConstIterator GetEndIter() const { return ConstIterator(const_cast<LinkList*>(this)->GetEndIter()); }
};
} // namespace ut

namespace math {
struct _VEC3 {
    float x, y, z;
};
struct VEC3 : public _VEC3 {
    VEC3() {}
};
} // namespace math

namespace snd {

// inferred: 0x6C bytes; only the members used here
class Sound3DListener {
public:
    unsigned char m00[0x30];
    atn::Vector3 mPosition;   // 0x30
    unsigned char m3C[0x28];
    ut::LinkListNode mLinkNode; // 0x64
};

class Sound3DManager {
public:
    typedef ut::LinkList<Sound3DListener, 0x64> ListenerList;

    const ListenerList& GetListenerList() const { return mListenerList; }
    int GetMaxPriorityReduction() const { return mMaxPriorityReduction; }

    unsigned char m00[0x0C];
    ListenerList mListenerList; // 0x0C
    int m18;
    int mMaxPriorityReduction; // 0x1C
};

// inferred: 0x28 bytes (constructor 0x80716000)
class Sound3DParam {
public:
    Sound3DParam();

    math::VEC3 position;          // 0x00
    math::VEC3 velocity;          // 0x0C
    unsigned long ctrlFlag;       // 0x18
    unsigned char decayCurve;     // 0x1C
    unsigned char decayRatio;     // 0x1D
    unsigned char dopplerFactor;  // 0x1E
    unsigned long soundUserParam; // 0x20
    void* userParam;              // 0x24
};

struct SoundAmbientParam {
    float volume;            // 0x00
    float pitch;             // 0x04
    float pan;               // 0x08
    float span;              // 0x0C
    unsigned long fxSend;    // 0x10
    float lpf;               // 0x14
    float biquadFilterValue; // 0x18
    int biquadFilterType;    // 0x1C
    int priority;            // 0x20
};

class SoundArchive {
public:
    struct Sound3DParam {
        unsigned long flags;
        unsigned char decayCurve;
        unsigned char decayRatio;
        unsigned char dopplerFactor;
    };
    bool detail_ReadSound3DParam(unsigned long id, Sound3DParam* param) const;
};

class Sound3DCalculator {
public:
    struct PanParam {
        float speakerAngleStereo;
        float frontSpeakerAngleDpl2;
        float rearSpeakerAngleDpl2;
        float initPan;
    };
    static void CalcVolumeAndPriority(const Sound3DManager& manager, const Sound3DListener& listener,
                                      const Sound3DParam& param, float* volume, int* priority);
    static void CalcPan(const Sound3DManager& manager, const Sound3DListener& listener,
                        const Sound3DParam& param, const PanParam& panParam, float* pan, float* span);
    static void CalcPitch(const Sound3DManager& manager, const Sound3DListener& listener,
                                      const Sound3DParam& param, float* pitch);
};

namespace detail {
// The weak copy of its destructor emitted here is unreferenced and stripped by the linker.
class Sound3DEngineInterface {
public:
    virtual ~Sound3DEngineInterface() {}
    virtual void UpdateAmbientParam(const Sound3DManager* manager, const Sound3DParam* param, unsigned long id,
                                    int voiceCount, SoundAmbientParam* ambientParam) = 0;
    virtual int GetAmbientPriority(const Sound3DManager* manager, const Sound3DParam* param,
                                   unsigned long id) = 0;
    virtual int GetRequiredVoiceOutCount(const Sound3DManager* manager, const Sound3DParam* param,
                                         unsigned long id) = 0;
};
} // namespace detail

class Sound3DEngine : public detail::Sound3DEngineInterface {
public:
    enum {
        UPDATE_VOLUME = 1 << 0,
        UPDATE_PAN = 1 << 1,
        UPDATE_SURROUND_PAN = 1 << 2,
        UPDATE_PRIORITY = 1 << 3,
        UPDATE_PITCH = 1 << 4
    };

    Sound3DEngine();
    virtual ~Sound3DEngine() {}
    virtual void UpdateAmbientParam(const Sound3DManager* manager, const Sound3DParam* param, unsigned long id,
                                    int voiceCount, SoundAmbientParam* ambientParam);
    virtual int GetAmbientPriority(const Sound3DManager* manager, const Sound3DParam* param, unsigned long id);
    virtual int GetRequiredVoiceOutCount(const Sound3DManager* manager, const Sound3DParam* param,
                                         unsigned long id);

protected:
    virtual void UpdateAmbientParam(const Sound3DManager* manager, const Sound3DParam* param, unsigned long id,
                                    unsigned long updateFlag, SoundAmbientParam* ambientParam);

    Sound3DCalculator::PanParam mPanParam; // 0x04
};

} // namespace snd
} // namespace nw4r

// inferred: SoundCore::GetVolume (Sound3dListener.cpp area), the product of its volume and fades
extern "C" float fn_800CA844(const void* core);

namespace atn {

// Only the members used here (the full classes are not reconstructed yet).
// inferred: AudioDevice is the sound engine of the game (lbl_8087EE90); it holds this engine at 0x900
class AudioDevice {
public:
    unsigned char m00[0x08];
    nw4r::snd::SoundArchive mArchive; // 0x08
    unsigned char m09[0x364 - 0x09];
    nw4r::snd::Sound3DManager m364;   // 0x364
};

// inferred: position and velocity of its Sound3dActor (0x08), sound id and flags
class SoundCore {
public:
    unsigned char m00[0x70];
    nw4r::math::VEC3 mPosition; // 0x70
    nw4r::math::VEC3 mVelocity; // 0x7C
    unsigned char m88[0x98 - 0x88];
    unsigned long mSoundId; // 0x98
    unsigned long mFlags;   // 0x9C
};

// inferred: user parameter of Sound3DParam (soundUserParam): an attenuation shape
struct SoundShape_unk {
    int mType;      // 0x00
    Vector3 mCenter; // 0x04
    float mRadius;  // 0x10
};

class Sound3dEngine : public nw4r::snd::Sound3DEngine {
public:
    typedef void (Sound3dEngine::*ShapeFunc)(const SoundShape_unk* shape, const Vector3& pos, float* volume);

    Sound3dEngine();
    virtual ~Sound3dEngine();

protected:
    virtual void UpdateAmbientParam(const nw4r::snd::Sound3DManager* manager, const nw4r::snd::Sound3DParam* param,
                                    unsigned long id, unsigned long updateFlag,
                                    nw4r::snd::SoundAmbientParam* ambientParam);

public:
    void fn_800CB09C(const SoundShape_unk* shape, const Vector3& pos, float* volume);
    void fn_800CB0A0(const SoundShape_unk* shape, const Vector3& pos, float* volume);
    void fn_800CB14C(const SoundShape_unk* shape, const Vector3& pos, float* volume);
    float fn_800CB1F4(AudioDevice* engine, const SoundCore* core);

    static const ShapeFunc sShapeFuncs[3];

    const nw4r::snd::Sound3DManager* mManager;   // 0x14
    const nw4r::snd::Sound3DListener* mListener; // 0x18
};

const Sound3dEngine::ShapeFunc Sound3dEngine::sShapeFuncs[3] = {
    &Sound3dEngine::fn_800CB09C,
    &Sound3dEngine::fn_800CB0A0,
    &Sound3dEngine::fn_800CB14C,
};

Sound3dEngine::Sound3dEngine() : mManager(0), mListener(0) {}

Sound3dEngine::~Sound3dEngine() {}

void Sound3dEngine::UpdateAmbientParam(const nw4r::snd::Sound3DManager* manager,
                                       const nw4r::snd::Sound3DParam* param, unsigned long id,
                                       unsigned long updateFlag, nw4r::snd::SoundAmbientParam* ambientParam) {
    if (updateFlag & UPDATE_VOLUME) {
        ambientParam->volume = 0.0f;
    }
    if (updateFlag & UPDATE_PRIORITY) {
        ambientParam->priority = -manager->GetMaxPriorityReduction();
    }

    const nw4r::snd::Sound3DManager::ListenerList& listenerList = manager->GetListenerList();
    if (listenerList.GetSize() != 1) {
        updateFlag &= ~(UPDATE_PAN | UPDATE_SURROUND_PAN | UPDATE_PITCH);
    }

    for (nw4r::snd::Sound3DManager::ListenerList::ConstIterator it = listenerList.GetBeginIter();
         it != listenerList.GetEndIter(); ++it) {
        const nw4r::snd::Sound3DListener& listener = *it;
        mManager = manager;
        mListener = &*it;

        if (updateFlag & (UPDATE_VOLUME | UPDATE_PRIORITY)) {
            float volume;
            int priority;
            nw4r::snd::Sound3DCalculator::CalcVolumeAndPriority(*manager, listener, *param, &volume, &priority);
            if (updateFlag & UPDATE_VOLUME) {
                const SoundShape_unk* shape = reinterpret_cast<const SoundShape_unk*>(param->soundUserParam);
                if (shape != 0) {
                    (this->*sShapeFuncs[shape->mType])(shape, mListener->mPosition, &volume);
                }
                ambientParam->volume = volume > ambientParam->volume ? volume : ambientParam->volume;
            }
            if (updateFlag & UPDATE_PRIORITY) {
                ambientParam->priority = priority > ambientParam->priority ? priority : ambientParam->priority;
            }
        }

        if (updateFlag & (UPDATE_PAN | UPDATE_SURROUND_PAN)) {
            float pan;
            float span;
            nw4r::snd::Sound3DCalculator::CalcPan(*manager, listener, *param, mPanParam, &pan, &span);
            if (updateFlag & UPDATE_PAN) {
                ambientParam->pan = pan;
            }
            if (updateFlag & UPDATE_SURROUND_PAN) {
                ambientParam->span = span;
            }
        }

        if (updateFlag & UPDATE_PITCH) {
            float pitch;
            nw4r::snd::Sound3DCalculator::CalcPitch(*manager, listener, *param, &pitch);
            ambientParam->pitch = pitch;
        }
    }
}

void Sound3dEngine::fn_800CB09C(const SoundShape_unk* shape, const Vector3& pos, float* volume) {}

void Sound3dEngine::fn_800CB0A0(const SoundShape_unk* shape, const Vector3& pos, float* volume) {
    Vector3 d = pos - shape->mCenter;
    float dist = PSVECMag(&d);
    if (shape->mRadius > 0.0f) {
        float t = dist / shape->mRadius;
        t = t < 1.0f ? t : 1.0f;
        *volume = 1.0f - t;
    } else {
        *volume = 1.0f;
    }
}

void Sound3dEngine::fn_800CB14C(const SoundShape_unk* shape, const Vector3& pos, float* volume) {
    Vector3 d = pos - shape->mCenter;
    float dist = PSVECMag(&d);
    if (shape->mRadius > 0.0f) {
        float t = dist / shape->mRadius;
        t = t < 1.0f ? t : 1.0f;
        *volume *= 1.0f - t;
    }
}

float Sound3dEngine::fn_800CB1F4(AudioDevice* engine, const SoundCore* core) {
    if (engine == 0 || core == 0) {
        return 0.0f;
    }
    float volume = 1.0f;
    if ((core->mFlags & 2) == 2) {
        nw4r::snd::Sound3DManager& manager = engine->m364;
        const nw4r::snd::Sound3DListener* listener = &*manager.GetListenerList().GetBeginIter();
        nw4r::snd::SoundArchive::Sound3DParam archiveParam;
        if (engine->mArchive.detail_ReadSound3DParam(core->mSoundId - 1, &archiveParam)) {
            nw4r::snd::Sound3DParam param;
            param.position = core->mPosition;
            param.velocity = core->mVelocity;
            param.ctrlFlag = archiveParam.flags;
            param.decayCurve = archiveParam.decayCurve;
            param.decayRatio = archiveParam.decayRatio;
            param.dopplerFactor = archiveParam.dopplerFactor;
            param.soundUserParam = 0;
            param.userParam = 0;
            int priority;
            nw4r::snd::Sound3DCalculator::CalcVolumeAndPriority(manager, *listener, param, &volume, &priority);
        }
    }
    return volume * fn_800CA844(core);
}

} // namespace atn
