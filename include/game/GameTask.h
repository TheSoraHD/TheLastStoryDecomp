#ifndef GAME_GAMETASK_H
#define GAME_GAMETASK_H

#include "atn/Task.h"
#include "math/Vec.h"
#include "game/GameCamera.h"
#include "game/FinishCamera_unk.h"
#include "game/GameMessageList.h"
#include "atn/SoundRef.h"
#include "atn/ResRef_unk.h"
#include "atn/LH_Xsca.h"
#include "atn/ColorMatrixParam_unk.h"
#include "MP/SaveLoad.h"

class GameScene;
struct GameSceneRoute_unk;
class GimmickCommon;
class MapTask;
class PlayerTask;
class CharaTask;
class SelectMember;
struct GameTaskData_unk;
namespace atn {
class FadeCommon;
class FadeCapture;
class FadeColor;
}

class GamePauseTask;
class TutorialTask;
class TutorialPopup;
class GameSaveBackground;
struct GameSaveParty_unk;
struct ReplayRecorder_unk;

// inferred: binary table of the field at GameTask + 0x550C (0x50 bytes; built inline by the GameTask
// constructor, its data is given back to the loader by the destructor). Names invented.
struct GameTaskTable_unk {
    atn::LH_Binary mFile; // inferred: file list of the next map (flist)
    int mType;            // 0x08: inferred: next map
    int mId;
    int mSub;
    int m14;
    int mPrevType;        // 0x18: inferred: map being left
    int mPrevId;
    int mPrevSub;
    int m24;
    GameTaskTable_unk() : m14(0), mScene(0), mState(0), m30(0), m34(0) {}
    GameScene* mScene;    // 0x28: inferred: scene of the next map
    int mState;           // 0x2C: inferred: state of the map change (GameTask::vf10)
    int m30;              // inferred: save data to restore
    int m34;
    atn::SimpleVector<unsigned int> m38; // inferred: resource hashes of the characters kept
    atn::SimpleVector<unsigned int> m44; // inferred: cache entries of the characters kept
};

// inferred from __vt__8GameTask (0x8078A3D0): the field task (lbl_8087F430). Other units still use
// partial views of it (Field_unk...). Method names invented unless noted.
class GameTask : public atn::Task {
public:
    // inferred: start options of the game task (0x24 bytes; names invented)
    struct Option {
        Option(int type, int map, int sub, int pos, int m10_, int m14_, void (*callback)(void*)) {
            mType = type;
            mMap = map;
            mSub = sub;
            mPos = pos;
            m10 = m10_;
            m14 = m14_;
            m18 = 0;
            m1C = 0;
            mCallback = callback;
        }
        int mType;
        int mMap;
        int mSub;
        int mPos;
        int m10;
        int m14;
        int m18;
        int m1C;
        void (*mCallback)(void*);
    };

    void fn_8036E430(int a, int b);
    void fn_8036E92C(int arg);          // inferred: opens the save menu
    void fn_8036E964();                 // inferred: closes the save menu
    void fn_8036E994(int type, int a, int b); // inferred: opens the party member menu
    void fn_8036EA04();                 // inferred: opens the library
    void fn_8036EA3C();                 // inferred: leaves the library
    void fn_8036EA6C(int menu);
    void fn_8036E89C();                 // inferred: closes the shops
    int fn_8036DAA8(CharaTask* chara); // inferred: the special skill is allowed here
    void fn_8036EE0C();
    int fn_80370C2C();                  // inferred: the fade is finished
    void fn_80370C6C();
    void fn_80370320(int flag, int value, int arg); // inferred: SetGlobalFlag
    inline void StartFinish_unk(CharaTask* chara, int id);
    void fn_80371790();
    void fn_80374D64();
    void fn_80370B78(unsigned int color, int frames, int arg, float speed); // inferred: fade out
    void fn_80370BD0(int frames, int arg, float speed);                     // inferred: fade in
    void fn_80371320(PlayerTask* player); // inferred: new leader
    void fn_8037177C();
    void fn_803731E8();
    void fn_80373164(CharaTask* owner, const atn::Vector3* pos, int id);
    void fn_80373F78();
    // RSO name; inline (weak copy 0x80373148). Defined in game/EnemyQuarkSecond.cpp, the only unit known
    // to inline it (a body here would need GameScene.h in every user of this header).
    MapTask* GetCurrentMap();
    int fn_80375184();        // inferred: saving is allowed
    int fn_80376150();
    void fn_80376194(int on);
    int fn_803761A4();
    int fn_803761AC();
    int fn_803761CC();        // inferred: the voices are enabled
    void fn_803761E4(int visible);
    int fn_803761EC();
    void fn_803761F4(int value);
    int fn_8036823C();
    void fn_80374710(int arg); // inferred: starts the battle camera (camera mode 6)
    void fn_803748E0(int arg); // inferred: ends the grapple camera
    int fn_8036DAF4(GameSceneRoute_unk* route, CharaTask* chara, int check, int point); // inferred: hide link ahead of `chara`
    int fn_8036DB24(GameSceneRoute_unk* route, CharaTask* chara, int point, int exclude, const atn::Vector3* dir); // inferred: next hide link
    int fn_8036DB3C(atn::Vector3* pos, int* index, CharaTask* chara);
    void fn_80375194(int id, int kind, float rot); // inferred: sets m5644/m5640 and moves the party to the leader
    int fn_803754C4(); // inferred: the task at 0x567C exists and is not stopped
    int fn_803761BC(); // inferred: debug field
    int fn_803761DC(); // inferred: a mode compared with 1, 2 and 3
    GimmickCommon* fn_8036E098(); // inferred: save point in front of the leader
    void fn_8036FB4C(int entry);        // inferred: moves to an entry of the current map
    void fn_8036FC58();                 // inferred: back from the map move (undoes fn_8036FB4C) // inferred: hide link held by `chara`
    void fn_80373118(); // inferred: one more frame of delay for the fades // inferred: nonzero on some fields
    struct StrategyAiEntry_unk* fn_80375254(); // inferred: AI strategy row of the field (default id 0)
    void fn_8037529C(unsigned int color0, unsigned int color1, int delay, int wait, int duration); // inferred: starts the capture fade
    int fn_803761FC();
    int fn_80376210();
    int fn_80376224();        // inferred: nonzero in the second state (night?)
    int fn_80376238();
    void fn_8037624C(int value);
    void fn_803766E4();
    inline int IsDungeonCleared_unk(int type, int id, int sub);
    inline void SetDungeonLevel_unk(int type, int id, int sub);
    int fn_8036FD4C();
    void fn_80370CD0();
    int fn_8036E170();
    void fn_8036E494();
    void fn_8036CFA4();
    void fn_8036CC90();
    void fn_80373AE8();
    void fn_803737C0();
    void fn_803727DC();
    void fn_80376568();
    // inferred: former extern "C" functions of game/GameTask.cpp (block 2 of s12h-20261107)
    void fn_80370094(int chapter);
    int fn_80370174(unsigned int index);
    int fn_80370A78(unsigned int index);
    void fn_80370AE4(unsigned int index, int value);
    void fn_80370F8C(int count, void* save, int stay);
    void fn_80371164();
    void fn_803712BC();
    void fn_80371334();
    void fn_80371674();
    void fn_803729B4(CharaTask* chara);
    void fn_80373FC4();
    void fn_803743AC();
    void fn_80374964(int kind);
    int fn_803752F4(const char* id);
    int fn_803754F0();
    int fn_8037587C(int type, int id, int sub);
    int fn_80375B78(int arg);
    int fn_80375D0C();
    void fn_80376254(int delay, int frames);
    void fn_80376324(int id);
    void fn_80376730();
    void fn_803750E4(int id); // inferred: sets a global flag once
    void fn_80367A40(int arg);
    void fn_8036CA60(int arg);

    GameTask(atn::Task* parent, const Option& option); // 0x803658B8
    virtual ~GameTask();  // 0x80366454
    virtual int vf0C();   // 0x80366E54
    virtual void vf10();  // 0x80368280
    virtual void vf14();  // 0x8036B7F4

    Option mOption;     // 0x48: inferred: mType 1, or 2 with mMap 0x44 and mSub 1 (fn_8036823C)
    GameCamera mCamera; // 0x6C
    FinishCamera_unk mFinish; // 0xD18
    int m10D0; // inferred: value compared with the ranges of the script events (GameEventScript::fn_80398530)
    struct ChapterEntry_unk* m10D4; // inferred: chapter of the field (fn_80208634/fn_802085E0)
    GameScene* m10D8;        // inferred: scene of the field
    int m10DC;
    GamePauseTask* m10E0;
    int mGlobalFlags[0x1000]; // 0x10E4: inferred: "GetGlobalFlag" (fn_80370174)
    int mLocalFlags[0x100];   // 0x50E4: inferred: "GetLocalFlag" (fn_80370A78)
    int m54E4;                  // inferred: state of the field (8-11 while an event runs)
    int m54E8;
    int m54EC;
    int m54F0;                  // inferred: fade direction (1 out, 2 in)
    GameMessageList mMessages;  // 0x54F4: inferred: system messages
    GameTaskTable_unk m550C;
    int m555C;
    int m5560;
    int m5564;
    int m5568;
    unsigned char m556C;
    unsigned char m556D[0x558C - 0x556D];
    MP::Ptr_unk<ReplayRecorder_unk> m558C;
    GameTaskData_unk* m5590;
    int m5594;
    int m5598;
    unsigned char m559C[0x55A4 - 0x559C];
    int m55A4;
    atn::SoundRef m55A8;
    int m55AC;
    int m55B0;
    int m55B4;
    int m55B8;
    int m55BC;
    float m55C0; // inferred: frame speed saved by fn_80373FC4
    ColorMatrixParam_unk m55C4;
    int m5614;
    int m5618;
    int m561C;
    atn::FadeCommon* m5620;  // inferred: fade of the field
    atn::FadeCapture* m5624; // inferred: screen capture fade of the field
    GameSaveParty_unk* m5628;
    MP::Ptr_unk<MP::SaveWork_unk> m562C; // inferred: copy of the save data the field starts from
    int m5630;
    GameSaveBackground* m5634;
    int m5638;
    int m563C;
    int m5640; // inferred: kind passed to fn_8021CF08 (fn_80375254)
    int m5644; // inferred: id passed to fn_8021CF08
    int m5648;
    int m564C;
    int m5650;
    int m5654;
    float m5658;
    int m565C;
    int m5660;
    int m5664;
    int m5668;
    int m566C;
    int m5670;
    int m5674;
    unsigned char m5678[0x567C - 0x5678];
    atn::Task* m567C; // inferred: task of the field (fn_803754C4)
    int m5680;
    int m5684;
    int m5688;
    int m568C;
    ResRef_unk m5690; // inferred: "preload/game_start_dg.pk"
    int m5694;
    int m5698;
    int m569C;
    int m56A0;
    int m56A4;
    MotionBlurParam_unk m56A8; // inferred: motion blur saved while the blur effect runs (vf10)
    float m56CC;
    int m56D0;
    CharaTask* m56D4;
    CharaTask* m56D8;
    atn::LH_Xsca m56DC; // inferred: battle camera animation
    int m56E4;
    int m56E8; // inferred: entry of the map to move to (fn_8036FB4C)
    TutorialTask* m56EC;
    TutorialPopup* m56F0;
    MP::Queue_unk<int, 8> m56F4; // inferred: tutorial ids waiting for the UI (fn_80370320)
    MP::Queue_unk<int, 8> m571C; // inferred: help ids shown after the current one (fn_80376324)
    unsigned int m5744; // inferred: play time in frames (30 per second, vf14)
    int m5748;
    int m574C;
    SelectMember* m5750;
    int m5754;
    int m5758;
    atn::FadeColor* m575C; // inferred: fade of the time limit (vf10)
    int m5760; // inferred: counter of the frames waited (fn_80373118)
    int m5764;
    int m5768;
    unsigned char m576C[0x5770 - 0x576C];
    int m5770;
    int m5774;
};

#endif
