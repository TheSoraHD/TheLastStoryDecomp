#ifndef GAME_GAMESCENE_H
#define GAME_GAMESCENE_H

#include "atn/Task.h"
#include "atn/LH_Binary.h"
#include "atn/Array_unk.h"
#include "math/Vec.h"

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

// The element types of the scene file (.gs). NpcSetPoint and EventBox are confirmed by the mangled
// names of their SimpleArray<T>::operator[] copies; the other names are the section names of the file.
namespace gs {

// inferred: "WarpPoint" (0x28 bytes)
struct WarpPoint {
    WarpPoint() {}
    int mId;
    atn::Vector3 mPos;
    unsigned char m10[0x28 - 0x10];
};

// "NpcSetPoint"/"EnemySetPoint"/"FriendSetPoint" (0x148 bytes)
struct NpcSetPoint {
    int mId;
    unsigned char m04[0x140 - 0x04];
    atn::SimpleArray<int> m140;
};

// inferred: "MovePoint" (0x30 bytes)
struct MovePoint {
    MovePoint() {}
    int mId;
    unsigned char m04[0x30 - 0x04];
};

// inferred: "HidePoint" (0x40 bytes)
struct HidePoint {
    HidePoint() {}
    int mId;
    unsigned char m04[0x40 - 0x04];
};

// inferred: (0x18 bytes)
struct HideLink_unk {
    HideLink_unk() {}
    unsigned char m00[0x18];
};

// inferred: (0xC bytes)
struct HideLink2_unk {
    unsigned char m00[0xC];
};

// inferred: "InteractionPoint" (0x30 bytes)
struct InteractionPoint {
    InteractionPoint() {}
    int mId;
    unsigned char m04[0x30 - 0x04];
};

// inferred: "InteractionConnection" (8 bytes)
struct InteractionConnection {
    int m00;
    int m04;
};

// inferred: "NpcAi" (0x20 bytes)
struct NpcAi {
    NpcAi();
    int m00;
    int m04;
    int m08;
    int m0C;
    float m10;
    int m14;
    int m18;
    int m1C;
};

// inferred: "SceneCosntrainModel" (0x20 bytes)
struct ConstrainModel {
    ConstrainModel();
    int m00;
    atn::Model* m04; // inferred: model being loaded
    unsigned char m08[0x20 - 0x08];
};

// "EventBox" (0x48 bytes)
struct EventBox {
    EventBox() {}
    int mId;
    unsigned char m04[0x24 - 0x04];
    atn::SimpleArray<int> m24; // inferred: "SceneScript"
    unsigned char m2C[0x48 - 0x2C];
};

// inferred: "EventSphere" (0x28 bytes)
struct EventSphere {
    EventSphere() {}
    int mId;
    unsigned char m04[0x18 - 0x04];
    atn::SimpleArray<int> m18; // inferred: "SceneScript"
    unsigned char m20[0x28 - 0x20];
};

// inferred: "AiArea" (0x30 bytes)
struct AiArea {
    AiArea() {}
    int mId;
    unsigned char m04[0x30 - 0x04];
};

// inferred: "AttentionPoint" (0x40 bytes)
struct AttentionPoint {
    AttentionPoint() {}
    int mId;
    unsigned char m04[0x40 - 0x04];
};

// inferred: "Gimmick" (0x80 bytes)
struct Gimmick {
    Gimmick() {}
    int mId;
    unsigned char m04[0x80 - 0x04];
};

// inferred: "CameraBox" (0x58 bytes)
struct CameraBox {
    CameraBox() {}
    int mId;
    unsigned char m04[0x58 - 0x04];
};

// inferred: "BgmBox" (0x50 bytes)
struct BgmBox {
    BgmBox() {}
    int mId;
    unsigned char m04[0x24 - 0x04];
    int mBgm; // 0x24: inferred: BGM id
    unsigned char m28[0x50 - 0x28];
};

// inferred: "MovePointGroup"/"HidePointGroup"/"WarpPointGroup" (0x94 bytes, one copy of the members
// per point type)
template <class T>
struct PointGroup {
    PointGroup() {}
    int mId;
    unsigned char m04[0x94 - 0x04];
};

// inferred: "EnemyGroup"/"FriendGroup" (0xA0 bytes)
template <class T>
struct CharaGroup {
    CharaGroup() {}
    int mId;
    unsigned char m04[0xA0 - 0x04];
};

// inferred: (unknown rows)
struct Unk148_unk {
    unsigned char m00[4];
};

} // namespace gs

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
    void fn_803C3894(const char* text, int length, int file); // inferred: reads a .gs file
    void fn_803CA0F4();
    void fn_803CA604();
    void fn_803CAABC();

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
    int m190;
    unsigned char m194[0x994 - 0x194];
    ResRef_unk m994;                       // inferred: levels/%s.pk
    ResRef_unk m998[6];                    // inferred: levels/%s_tmp%d.pk (the last one: town_sp)
    int m9B0;
};

#endif
