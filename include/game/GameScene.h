#ifndef GAME_GAMESCENE_H
#define GAME_GAMESCENE_H

#include "atn/Task.h"
#include "atn/LH_Binary.h"
#include "atn/Array_unk.h"
#include "math/Vec.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/FixedArray.h"

namespace atn {
class Model;
}

// inferred: reference to a pfs archive of the load manager (4 bytes here: GameScene builds an array
// of six with a weak constructor that only clears mRes, 0x803BF818; the destructor is the weak copy
// __dt__10ResRef_unkFv 0x80117E34 of game/CampSaveLoad.cpp)
struct ResRef_unk {
    ResRef_unk() : mRes(0) {}
    ~ResRef_unk() { fn_80470528(); }
    void fn_80470364(const char* name, int a, int b, int c); // inferred: starts loading `name`
    void fn_80470528();                                      // inferred: drops the reference

    void* mRes;
};

// inferred: run-length encoded array of u16 (see game/U16Table_unk.cpp)
struct U16Table_unk;
extern "C" void fn_803CD918(U16Table_unk* table);

struct U16Table_unk {
    ~U16Table_unk() { fn_803CD918(this); }
    unsigned short mCount; // inferred: number of values
    unsigned short mRuns;  // inferred: number of runs
    void* mData;
};

// inferred: message of the scene (dtor 0x80366364, game/GameTask.cpp)
struct GameMessage {
    ~GameMessage();
    unsigned char m00[0xC];
};

// inferred: message table of the scene (functions 0x803B3B38-0x803B3CA8 of game/GameChoiceWindow.cpp);
// name invented
struct GameMessageTable_unk {
    atn::SimpleArray<int> m00;
    atn::SimpleArray<GameMessage> m08;
    atn::LH_Binary m10;
};

namespace gs {

// inferred: helpers of atn::SimpleArray used by the scene loader (weak copies of this file). They may
// be members of atn::SimpleArray, but declaring anything new in that template renumbers the @N labels
// of every unit that includes atn/Array_unk.h, so they are free templates here. Names invented.
template <class T>
inline T* GetData_unk(const atn::SimpleArray<T>& a) {
    return a.mData;
}
template <class T>
inline void Grow_unk(atn::SimpleArray<T>& a, unsigned int n, int heap, const char* name) {
    T* data;
    unsigned int old = a.mSize;
    if (n == old) {
        return;
    }
    a.mSize = n;
    data = a.mData;
    if (a.mSize != 0) {
        a.mData = new ((MemoryTag)heap, "", "", 0) T[a.mSize];
        if (data != 0 && old != 0) {
            for (unsigned int i = 0; i < (old < (unsigned int)a.mSize ? old : (unsigned int)a.mSize); i++) {
                a.mData[i] = data[i];
            }
        }
    } else {
        a.mData = 0;
    }
    if (data != 0) {
        delete[] data;
    }
}
template <class T>
inline int FindIndex_unk(const atn::SimpleArray<T>& a, int id) {
    for (unsigned int i = 0; i < (unsigned int)a.mSize; i++) {
        if (id == a.mData[i].mId) {
            return i;
        }
    }
    return -1;
}

} // namespace gs

// The element types of the scene file (.gs). NpcSetPoint and EventBox are confirmed by the mangled
// names of their SimpleArray<T>::operator[] copies; the other names are the section names of the file.
namespace gs {

// inferred: id and position at the start of every point of the file (read by GameScene::fn_803C3724)
struct PointBase_unk {
    int mId;
    atn::Vector3 mPos;
};

// inferred: "WarpPoint" (0x28 bytes)
struct WarpPoint : public PointBase_unk {
    WarpPoint() {}
    int mIndex; // 0x10: id plus the offset of the file
    float m14;
    int m18[3];
    int m24;
};

// "NpcSetPoint"/"EnemySetPoint"/"FriendSetPoint" (0x148 bytes)
struct NpcSetPoint : public PointBase_unk {
    int mIndex;
    float m14;
    int m18;
    int m1C;
    int m20;
    int m24;
    int m28;
    char mName[0x100]; // 0x2C
    int m12C;
    int m130[3];
    int m13C;
    atn::SimpleArray<int> m140; // inferred: script ids
};

// inferred: "MovePoint" (0x30 bytes)
struct MovePoint : public PointBase_unk {
    MovePoint() {}
    int mIndex;
    float m14;
    int m18;
    int m1C;
    int m20;
    int m24;
    int m28;
    int m2C;
};

// inferred: "HidePoint" (0x40 bytes)
struct HidePoint : public PointBase_unk {
    HidePoint() {}
    int mIndex;
    int m14;
    int m18;
    int m1C;
    int m20;
    int m24;
    int m28[4];
    int m38;
    int m3C;
};

// inferred: link between two hide points (0x18 bytes)
struct HideLink_unk {
    HideLink_unk() {}
    int mFrom;
    int mTo;
    int m08;
    atn::Vector3 mMid; // inferred: middle of the two points
};

// inferred: (0xC bytes)
struct HideLink2_unk {
    int mFrom;
    int mTo;
    int m08;
};

// inferred: "InteractionPoint" (0x30 bytes)
struct InteractionPoint : public PointBase_unk {
    InteractionPoint() {}
    int mIndex;
    int m14;
    int m18;
    int m1C[4];
    int m2C;
};

// inferred: "InteractionConnection" (8 bytes)
struct InteractionConnection {
    int m00;
    int m04;
};

// inferred: "NpcAi" (0x20 bytes)
struct NpcAi {
    NpcAi() : m00(0), m04(0), m08(0), m0C(0.0f), m10(0.0f), m14(0), m18(0), m1C(0) {}
    int m00;
    int m04;
    int m08;
    float m0C;
    float m10;
    int m14;
    int m18;
    int m1C;
};

// inferred: "SceneCosntrainModel" (0x20 bytes)
struct ConstrainModel {
    ConstrainModel() : m00(0), m04(0), m08(0), m0C(0), m10(0.0f, 0.0f, 0.0f), m1C(1) {}
    int m00;
    atn::Model* m04; // inferred: model being loaded
    int m08;
    int m0C;
    atn::Vector3 m10;
    int m1C;
};

// "EventBox" (0x48 bytes)
struct EventBox : public PointBase_unk {
    EventBox() {}
    int mIndex;
    float m14;
    float m18;
    float m1C;
    float m20;
    atn::SimpleArray<int> m24; // inferred: "SceneScript"
    int m2C;
    int m30;
    int m34;
    int m38;
    int m3C;
    int m40;
    int m44;
};

// inferred: "EventSphere" (0x28 bytes)
struct EventSphere : public PointBase_unk {
    EventSphere() {}
    int mIndex;
    float m14;
    atn::SimpleArray<int> m18; // inferred: "SceneScript"
    int m20;
    int m24;
};

// inferred: "AiArea" (0x30 bytes)
struct AiArea : public PointBase_unk {
    AiArea() {}
    int mIndex;
    float m14;
    float m18;
    float m1C;
    float m20;
    int m24;
    int m28;
    int m2C;
};

// inferred: "AttentionPoint" (0x40 bytes)
struct AttentionPoint : public PointBase_unk {
    AttentionPoint() {}
    int mIndex;
    float m14;
    int m18;
    int m1C[4];
    int m2C[3];
    int m38;
    int m3C;
};

// inferred: "Gimmick" (0x80 bytes)
struct Gimmick : public PointBase_unk {
    Gimmick() {}
    int mIndex;
    float m14;
    int m18;
    int m1C;
    int m20[8];
    float m40[8];
    int m60[4];
    int m70[3];
    int m7C;
};

// inferred: "CameraBox" (0x58 bytes)
struct CameraBox : public PointBase_unk {
    CameraBox() {}
    int mIndex;
    float m14;
    float m18;
    float m1C;
    float m20;
    int m24;
    int m28;
    int m2C;
    int m30;
    int m34;
    int m38[4];
    int m48[3];
    int m54;
};

// inferred: "BgmBox" (0x50 bytes)
struct BgmBox : public PointBase_unk {
    BgmBox() {}
    int mIndex;
    float m14;
    float m18;
    float m1C;
    float m20;
    int mBgm; // 0x24: inferred: BGM id
    float m28;
    int m2C;
    int m30[4];
    int m40[3];
    int m4C;
};

// inferred: "MovePointGroup"/"HidePointGroup"/"WarpPointGroup" (0x94 bytes, one copy of the members
// per point type)
template <class T>
struct PointGroup : public PointBase_unk {
    PointGroup() {}
    T* mPoints[32];
    T* m90; // inferred: main point
};

// inferred: "EnemyGroup"/"FriendGroup" (0xA0 bytes)
template <class T>
struct CharaGroup : public PointBase_unk {
    CharaGroup() {}
    T* mPoints[32];
    int m90;  // inferred: id of the leader
    T* m94;   // inferred: leader
    int m98;
    int m9C;
};

// inferred: (unknown rows)
struct Unk148_unk {
    unsigned char m00[4];
};

// inferred: rows x columns of T (MovePointConnection, HideVsHideConnection); names invented
template <class T>
struct Table2D_unk {
    Table2D_unk() : mData(0), mRows(0), mCols(0) {}
    ~Table2D_unk() {
        if (mData != 0) {
            delete[] mData;
        }
    }
    void Alloc(int rows, int cols) {
        if (mData != 0) {
            delete[] mData;
        }
        mRows = rows;
        mCols = cols;
        mData = new ((MemoryTag)0, "", "", 0) T[rows * cols];
    }
    T* operator[](int row) { return &mData[row * mCols]; }

    T* mData;
    int mRows;
    int mCols;
};

} // namespace gs

class CharaTask;

// inferred: pair queued by GameScene::fn_803CCC84 (names invented)
struct GameScenePair_unk {
    int m00;
    int m04;
};

// inferred from __vt__9GameScene (0x8078BC48): the scene of a map, read from the .gs files
// (0x9B8 bytes)
class GameScene : public atn::Task {
public:
    static GameScene* fn_803BEBAC(atn::Task* parent, int area, int map, int sub); // inferred: creates the scene
    GameScene(atn::Task* parent, int area, int map, int sub); // 0x803BEC40
    virtual ~GameScene();     // 0x803BF9D4
    virtual int vf0C();       // 0x803C01C0
    virtual void vf10();      // 0x803C1950
    virtual void vf14();      // 0x803C221C

    void fn_803C0634(unsigned int index);
    void fn_803C0938(unsigned int index);
    void fn_803C0C2C(unsigned int index);
    void fn_803C0F48();
    void fn_803C11A4();
    void fn_803C1338();
    void fn_803C1560();
    void fn_803C17FC();
    void fn_803C1840();
    void fn_803C1884();
    void fn_803C18DC();
    void fn_803C1CAC();
    void fn_803C1E3C();
    void fn_803C2048();
    void fn_803C2090();
    void fn_803C3724(gs::PointBase_unk* point, atn::Csv& csv); // inferred: id and position
    void fn_803C3798(int* values, atn::Csv& csv);              // inferred: four integers
    void fn_803C380C(int* values, atn::Csv& csv);              // inferred: a range (start, value, end)
    void fn_803C3894(const char* text, int length, int file); // inferred: reads a .gs file
    template <class T>
    void fn_803C74E0(atn::SimpleArray<T>& points, atn::Csv& csv, int file); // inferred: chara set points
    template <class G, class T>
    void fn_803C606C(atn::SimpleArray<G>& groups, atn::SimpleArray<T>& points, atn::Csv& csv); // inferred: chara groups
    template <class T>
    T* FindPoint_unk(atn::SimpleArray<T>& points, int id); // inferred: point with the given id
    template <class G, class T>
    void fn_803C6FE4(atn::SimpleArray<G>& groups, atn::SimpleArray<T>& points, atn::Csv& csv); // inferred: point groups
    void fn_803CA0F4();
    void fn_803CA530();
    void fn_803CA604();
    void fn_803CAABC();
    void fn_803CAED0();
    void fn_803CB044();
    void fn_803CB474();
    void fn_803CB618();
    void fn_803CB820();
    int fn_803CC198(int a, int b, CharaTask* chara);
    int fn_803CC1A4(int a, int b, CharaTask* chara, float radius, float scale);
    void fn_803CC3BC();
    void fn_803CC5E4();
    void* fn_803CC6B4(int id); // inferred: message `id` (>= 10000: common messages of the field)
    gs::ConstrainModel* fn_803CC6D0(int id); // inferred: constrained model with the given id
    void fn_803CC718(unsigned int key, int value);
    atn::Vector3 fn_803CC754(int index); // inferred: position of a NPC set point
    int fn_803CC774(int arg);
    void fn_803CC77C();
    void fn_803CC900();
    int fn_803CCA34(int arg);
    int fn_803CCA4C(int arg);
    int fn_803CCA64(int a, int b);
    void fn_803CCA84();
    int fn_803CCC4C(); // inferred: the event script is running
    void fn_803CCC70(int value);
    void fn_803CCC84(int a, int b); // inferred: queues a pair
    void fn_803CCCC0();
    void fn_803CCD98();
    void fn_803CCDAC();

    int m48;                            // inferred: loading step
    atn::LH_Binary m4C[3];              // inferred: the .gs files (main, _sub2, _sub3)
    void* m64;                          // inferred: map data (fn_8049D704)
    int m68;                            // "NpcNum"
    int m6C;                            // "EnemyNum"
    int m70;                            // "PlayerStartPoint"
    int m74;                            // inferred: number of move points
    atn::SimpleArray<gs::WarpPoint> m78;
    atn::SimpleArray<gs::NpcSetPoint> m80; // "NpcSetPoint"
    atn::SimpleArray<gs::NpcSetPoint> m88; // "EnemySetPoint"
    atn::SimpleArray<gs::NpcSetPoint> m90; // "FriendSetPoint"
    atn::SimpleArray<gs::MovePoint> m98;
    atn::SimpleArray<U16Table_unk> mA0;    // inferred: "MovePointConnection"
    int mA8;                               // inferred: number of hide points
    atn::SimpleArray<gs::HidePoint> mAC;
    atn::SimpleArray<gs::HideLink_unk> mB4;
    atn::SimpleArray<gs::HideLink2_unk> mBC;
    atn::SimpleArray<gs::InteractionPoint> mC4;
    atn::SimpleArray<gs::InteractionConnection> mCC;
    atn::SimpleArray<gs::NpcAi> mD4;
    atn::SimpleArray<gs::ConstrainModel> mDC;
    atn::SimpleArray<gs::EventBox> mE4;
    atn::SimpleArray<gs::EventSphere> mEC;
    atn::SimpleArray<gs::AiArea> mF4;
    atn::SimpleArray<gs::AttentionPoint> mFC;
    atn::SimpleArray<gs::Gimmick> m104;
    atn::SimpleArray<gs::CameraBox> m10C;
    atn::SimpleArray<gs::BgmBox> m114;
    GameMessageTable_unk m11C;
    atn::Task* m134;                       // inferred: event script task (fn_80393610)
    int m138;
    float m13C;
    gs::BgmBox* m140;                      // inferred: current BGM box
    void* m144;                            // inferred: area data (fn_8021771C)
    atn::SimpleArray<gs::Unk148_unk> m148;
    atn::SimpleArray<gs::Unk148_unk> m150;
    atn::SimpleArray<gs::PointGroup<gs::MovePoint> > m158;
    atn::SimpleArray<gs::PointGroup<gs::HidePoint> > m160;
    atn::SimpleArray<gs::PointGroup<gs::WarpPoint> > m168;
    atn::SimpleArray<gs::CharaGroup<gs::NpcSetPoint> > m170; // "FriendGroup"
    atn::SimpleArray<gs::CharaGroup<gs::NpcSetPoint> > m178; // "EnemyGroup"
    atn::SimpleArray<int> m180;            // "EventIdList"
    atn::SimpleArray<gs::Unk148_unk> m188;
    atn::FixedArray_unk<GameScenePair_unk, 0x100> m190; // inferred: pairs queued by fn_803CCC84
    ResRef_unk m994;                       // inferred: levels/%s.pk
    ResRef_unk m998[6];                    // inferred: levels/%s_tmp%d.pk (the last one: town_sp)
    int m9B0;
};

#endif
