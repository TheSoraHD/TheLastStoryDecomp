#ifndef GAME_SONGMANAGER_H
#define GAME_SONGMANAGER_H

#include "atn/Task.h"
#include "atn/Bgm.h"
#include "game/EnvSeManager.h"
#include "math/Vec.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// inferred: scalar times vector (not in math/Vec.h)
inline atn::Vector3 operator*(float f, const atn::Vector3& v) {
    return atn::Vector3(v * f);
}

// inferred: value interpolated over a time (name invented): the time advances by mSpeed every frame
// and the value goes from mFrom to mTo
template <class T>
struct SongInterp_unk {
    SongInterp_unk(const T& value)
        : mTime(0.0f), mDuration(0.0f), mSpeed(1.0f), mActive(false), mFrom(value), mTo(value), mCur(value),
          mDelta(mTo - mFrom) {}

    void Finish_unk() {
        if (mSpeed >= 0.0f) {
            mCur = mTo;
            mTime = mDuration;
        } else {
            mCur = mFrom;
            mTime = 0.0f;
        }
        mActive = false;
    }

    void Start_unk(const T& to, float duration) {
        mTime = 0.0f;
        mDuration = duration;
        mSpeed = 1.0f;
        mActive = true;
        mFrom = mCur;
        mTo = to;
        mDelta = mCur - mFrom;
        if (mDuration <= 0.0f) {
            Finish_unk();
        }
    }

    void Lerp_unk() {
        float t = mTime / mDuration;
        T d = t * (mTo - mFrom);
        T cur = mFrom + d;
        mDelta = cur - mCur;
        mCur = cur;
    }

    void Update_unk() {
        if (mActive) {
            mTime += mSpeed;
            if (mSpeed >= 0.0f) {
                if (mTime < 0.0f) {
                    mCur = mFrom;
                    mTime = 0.0f;
                } else if (mTime < mDuration) {
                    Lerp_unk();
                } else {
                    Finish_unk();
                }
            } else {
                if (mTime < 0.0f) {
                    Finish_unk();
                } else if (mTime < mDuration) {
                    Lerp_unk();
                } else {
                    mCur = mTo;
                    mTime = mDuration;
                }
            }
        }
    }

    float mTime;     // 0x00
    float mDuration; // 0x04
    float mSpeed;    // 0x08
    bool mActive;    // 0x0C
    T mFrom;
    T mTo;
    T mCur;
    T mDelta;
};

// inferred: initial values of the interpolated distance and volumes (defined after the
// constructor: it reloads them and does not fold 0.1f)
extern float sSongDistance_unk;
extern float sSongMinVolume_unk;
extern float sSongMaxVolume_unk;

// inferred from __vt__11SongManager (0x80797330): a song played as a 3D sound in place of the BGM
class SongManager : public atn::Task {
public:
    static SongManager* fn_805A2240(atn::Task* parent); // inferred: creates the singleton
    static int fn_805A2404(int area, int map, int arg); // inferred: the map has a song
    SongManager(atn::Task* parent)
        : atn::Task(parent), m48(0), m4C(0), mPos(lbl_807C7030), mDistance(sSongDistance_unk),
          mMinVolume(sSongMinVolume_unk), mMaxVolume(sSongMaxVolume_unk), mF0(0), mF4(0), mBgm(0), mFC(0) {}
    virtual ~SongManager();
    virtual void vf10();
    virtual void vf14();
    void fn_805A2FD4(int state);         // inferred: starts/stops the song
    void fn_805A32C8(int song);          // inferred: changes the song
    void fn_805A3448(const atn::Vector3& pos);
    void fn_805A358C(float distance);
    void fn_805A3660(float volume);
    void fn_805A3734(float volume);
    void fn_805A3808(int lower);         // inferred: lowers the environment sounds

    // inferred: fade time of the changes
    int GetFade_unk() const { return mF4 > 0 ? mF4 : (IsPlaying_unk() ? 60 : 0); }
    bool IsPlaying_unk() const { return m48 == 1 && mFC == 0; }
    // inferred: lowers or restores the environment sounds (from mF0)
    void UpdateEnvSe_unk() {
        if (m48 != 1) {
            return;
        }
        EnvSeManager* env = lbl_8087F418;
        if (env == 0) {
            return;
        }
        if (mF0) {
            if (env->m18C == 0) {
                lbl_8087F418->Lower_unk(GetFade_unk());
            }
        } else if (env->m18C != 0) {
            lbl_8087F418->Restore_unk(GetFade_unk());
        }
    }

    int m48;                               // inferred: state (1 = playing)
    int m4C;                               // inferred: song index
    SongInterp_unk<atn::Vector3> mPos;     // 0x50: position of the sound
    SongInterp_unk<float> mDistance;       // 0x90: distance of the volume falloff
    SongInterp_unk<float> mMinVolume;      // 0xB0
    SongInterp_unk<float> mMaxVolume;      // 0xD0
    int mF0;                               // inferred: environment sounds lowered while playing
    int mF4;                               // inferred: fade time (0: default)
    atn::Bgm* mBgm;                        // 0xF8
    int mFC;                               // inferred: waiting for the stream

    static SongManager* sInstance;
};

#endif
