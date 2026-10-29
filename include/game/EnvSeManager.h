#ifndef GAME_ENVSEMANAGER_H
#define GAME_ENVSEMANAGER_H

#include "atn/Array_unk.h"
#include "atn/Bgm.h"
#include "atn/Csv.h"
#include "atn/LH_Binary.h"
#include "atn/SoundRef.h"
#include "atn/Task.h"
#include "game/ResHandle.h"
#include "math/Vec.h"
#include "string.h"

class EnvSeBase;
class EnvSePoint;
struct LoadingEnvSe_unk;

// inferred: data of an environment sound or area (0x54 bytes, read from one line of the CSV by
// fn_80360B40). Names invented.
struct EnvSeData_unk {
    EnvSeData_unk(int type)
        : mType(type), mSoundId(0), mVolume(1.0f), mPos(0.0f, 0.0f, 0.0f), mDistance(0.0f), mLPF(0.0f), m40(0),
          m44(0), m48(0), m4C(0), m50(0) {
        memset(mName, 0, sizeof(mName));
    }
    void fn_80360B40(atn::Csv* csv); // inferred: reads the common fields of the line

    int mType;          // 0x00: inferred: 0 Point, 1 Line, 2 Sphere, 3 Box, 4 Global, 5 Area, 6 none
    char mName[0x20];   // 0x04
    unsigned int mSoundId; // 0x24: inferred: id of the name in the sound engine
    float mVolume;      // 0x28
    atn::Vector3 mPos;  // 0x2C
    float mDistance;    // 0x38
    float mLPF;         // 0x3C
    int m40;            // inferred: 1 = muted while lbl_8087F540->m70 is set; streamed sound
    int m44;            // inferred: plays only when the event flag m4C has the value m50
    int m48;            // inferred: kind of the flag (1: fn_80370174)
    int m4C;
    int m50;
};

// inferred: record of the manager (0x5C bytes): the task of the sound, its data and the sound playing
struct EnvSeRecord_unk {
    EnvSeRecord_unk() : mTask(0), mData(6) {}

    EnvSeBase* mTask;     // 0x00
    EnvSeData_unk mData;  // 0x04
    atn::SoundRef mSound; // 0x58
};

// inferred: area of the environment sounds (reverb and BGM), not a task (0xA8 bytes; names invented)
struct EnvSeArea_unk {
    EnvSeArea_unk() : mId(0), m04(0), mData(5), m5C(0), mReverb(0), mA0(1.0f) {}
    void fn_80362FA4(atn::Csv* csv);  // inferred: reads the line
    void fn_80363108(void* root);     // inferred: creates the debug menu
    static void fn_8036324C(EnvSeArea_unk* self); // inferred: debug menu callback

    int mId;               // 0x00
    unsigned int m04;      // inferred: id of the BGM name in the sound engine
    EnvSeData_unk mData;   // 0x08
    int m5C;               // inferred: enabled by its event flag
    atn::Vector3 mSize;    // 0x60
    atn::Matrix3x4 mMtx;   // 0x6C
    int mReverb;           // 0x9C
    float mA0;
    ResHandle mMenu;       // 0xA4
};

// inferred: pair of sound ids joined by a line (EnvSeConnection)
struct EnvSeLink_unk {
    EnvSeLink_unk() : mA(0), mB(0) {}
    int mA;
    int mB;
};

// inferred: BGM request of the areas (0x40 bytes; names invented)
struct EnvSeBgmReq_unk {
    EnvSeBgmReq_unk() : mSoundId(0), mVolume(1.0f), mLPF(0.0f), m2C(0), mReverb(0), m34(1.0f), mFade(0), m3C(1) {
        memset(mName, 0, sizeof(mName));
    }

    char mName[0x20];      // 0x00
    int mSoundId;          // 0x20
    float mVolume;         // 0x24
    float mLPF;            // 0x28
    int m2C;
    int mReverb;           // 0x30
    float m34;
    int mFade;             // 0x38
    int m3C;               // inferred: wait for the previous BGM before starting
};

// inferred from __vt__12EnvSeManager (0x80789CB8): environment sounds and area BGM of the map,
// singleton lbl_8087F418 created by fn_8035DF48 (0x1B0 bytes). Member names invented.
class EnvSeManager : public atn::Task {
public:
    EnvSeManager(atn::Task* parent);
    virtual ~EnvSeManager();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    static EnvSeManager* fn_8035DF48(atn::Task* parent); // inferred: creates the singleton
    int fn_8035E730();  // inferred: nonzero while loading
    void fn_8035E7CC(); // inferred: starts the sounds once the timer ran out
    void fn_8035E858(); // inferred: stops everything
    void fn_8035EA2C(); // inferred: picks the area of the listener
    void fn_8035ECCC(); // inferred: BGM state machine
    void fn_8035F33C(const char* path); // inferred: starts loading the CSV
    void fn_8035F3D4(); // inferred: parses the CSV
    EnvSePoint* fn_8035FF44(int id); // inferred: point sound by id
    void fn_8035FF94(int area, int fade, int wait); // inferred: applies an area
    void fn_803601E4(int reverb);
    int fn_80360230(int id);           // inferred: starts the sound of a record
    void fn_80360504(int id, int fade); // inferred: stops the sound of a record
    void fn_8036055C(int id);          // inferred: moves the sound of a record
    void fn_80360574(int id);          // inferred: the parameters of a record changed
    void fn_803605EC(int fade, float volume); // inferred: volume of the environment sounds
    void fn_803606CC(int on, unsigned int bits, int fade); // inferred: mutes the sounds
    void fn_80360780(int on, unsigned int bits, int fade); // inferred: disables the BGM
    void fn_80360848(int on, unsigned int bits, int fade);
    void fn_803608A4();
    void fn_803608A8(); // inferred: reports the playing sound to the field
    void fn_8036097C(int a, int b, int c, int d, int useFlag); // inferred: loading sound
    int fn_80360A2C();  // inferred: plays the loading sound, nonzero while preparing
    void fn_80360B00(); // inferred: stops the loading sound

    // inferred: lowers/restores the environment sounds (inlined by SongManager; names invented)
    void Lower_unk(int fade) {
        m18C = 1;
        fn_803605EC(fade, m190);
    }
    void Restore_unk(int fade) {
        m18C = 0;
        fn_803605EC(fade, m188);
    }
    // inferred: a BGM change is pending (inlined by GameBgm; name invented)
    bool IsBattle_unk() {
        bool r = false;
        if (mBgmState == 1 || mBgmState == 2) {
            r = true;
        }
        return r;
    }
    // inferred: area by id (inlined)
    EnvSeArea_unk* FindArea_unk(int id) {
        for (unsigned int i = 0; i < (unsigned int)mAreas.mSize; i++) {
            EnvSeArea_unk* area = &mAreas.mData[i];
            if (area->mId == id) {
                return area;
            }
        }
        return 0;
    }
    // inferred: swaps the two BGM players (inlined)
    void SwapBgm_unk() {
        mCur = &mBgm[mBgmIndex];
        mBgmIndex = (mBgmIndex + 1) % 2;
        mNext = &mBgm[mBgmIndex];
    }

    atn::LH_Binary mLoader;                      // 0x48
    int mState;                                  // 0x50: inferred: 2 loading, 3 tasks, 5 running
    atn::SimpleArray<EnvSeRecord_unk> mRecords;  // 0x54
    atn::SimpleArray<EnvSeArea_unk> mAreas;      // 0x5C
    atn::SimpleArray<EnvSeLink_unk> mLinks;      // 0x64
    int m6C;                                     // inferred: the sounds run
    int m70;
    int m74;                                     // inferred: start delay
    unsigned int m78;                            // inferred: mute bits of the sounds
    unsigned int m7C;                            // inferred: disable bits of the BGM
    unsigned int m80;                            // inferred: disable bits of the area BGM
    int m84;                                     // inferred: current area id
    int m88;                                     // inferred: previous area id
    atn::Bgm mBgm[2];                            // 0x8C
    atn::Bgm* mCur;                              // 0xB4
    atn::Bgm* mNext;                             // 0xB8
    int mBgmIndex;                               // 0xBC
    EnvSeBgmReq_unk mPrevReq;                    // 0xC0
    EnvSeBgmReq_unk mCurReq;                     // 0x100
    EnvSeBgmReq_unk mNextReq;                    // 0x140
    int mBgmState;                               // 0x180
    float m184;                                  // inferred: volume of the environment sounds
    float m188;
    int m18C;                                    // inferred: the environment sounds are lowered
    float m190;                                  // inferred: "ReduceVolume"
    atn::SoundRef m194;                          // inferred: loading sound
    int m198;
    int m19C;
    LoadingEnvSe_unk* m1A0;
    int m1A4;                                    // inferred: "DebugLevel"
    ResHandle m1A8;                              // inferred: debug menu "EnvSE"
    void* m1AC;
};

extern "C" EnvSeManager* lbl_8087F418;

#endif
