// game/BattleTask.cpp (0x800F13BC-0x8010D98C): battle manager (BattleTask, HomingBullet). Only the
// effect start/stop functions are reconstructed so far.
#include "game/BattleTask.h"
#include "atn/Memory.h"
#include "atn/EffectManager.h"
#include "atn/EffectSystem_unk.h"
#include "math/Vec.h"
#include "atn/SoundRef.h"
#include "game/CharaTask.h"
#include "game/GameConfig_unk.h"
#include "game/HitInfo_unk.h"
#include "game/LogMessageTask.h"
#include "macros.h"
#include "game/PlayerManager.h"
#include "game/PlayerTask.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/PopupTask.h"
#include "std/new.h"
#include "game/SetConfig.h"
#include "game/GameTask.h"
#include "game/MapTask.h"
#include "game/GameScene.h"
#include "game/BattleCharaTask.h"
#include "atn/Camera_unk.h"
#include "atn/RenderManager.h"
#include "atn/DrawHelper.h"
#include "atn/Color.h"
#include "atn/D2A_Task.h"
#include "atn/ResLoader_unk.h"
#include "atn/Texture_unk.h"
#include "game/GimmickCommon.h"
#include "atn/NodeChunk.h"
#include "game/MapManager.h"
#include "game/Arms.h"
#include "chr/ControlBase.h"
#include "game/Inventory.h"
#include "game/GameUI.h"
#include "game/FriendManager.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include "game/AiTarget.h"
#include "game/GimmickManager.h"
#include "game/GameVoice.h"
#include "game/ArenaTask.h"
#include "math/VecAssign_unk.h"
#include "atn/SoundManager.h"
#include "chr/CharaEquip_unk.h"
#include "db/EquipParam_unk.h"
#include "game/SkillEffectManager.h"
#include "game/HomeButtonMenu.h"
#include "game/CrossbowMenuTask.h"
#include "atn/GraphicsDevice.h"
#include "atn/Font_unk.h"
#include "game/PickTask.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

extern "C" BattleTask* lbl_8087F048; // inferred: the battle manager
extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" void* fn_80210220(int id); // inferred: record of an attract-hate parameter (as in game/CharaTask.cpp)
extern "C" PlayerManager* lbl_8087F8A0;
extern "C" GameTask* lbl_8087F430;
extern "C" int rand();
extern "C" void fn_80102EAC(BattleTask* mgr, CharaTask* chara, CharaTask* attacker, int value, int); // inferred: hate
// inferred: multiplayer manager (as Arena_unk of game/CharaTask.cpp); only the member used here
struct Arena_unk {
    unsigned char m000[0x4FC];
    int m4FC; // inferred: state (0x1E: in play; fn_801004FC)
    unsigned char m500[0x50C - 0x500];
    int m50C; // inferred: compared with 5 by fn_8010220C
    unsigned char m510[0x540 - 0x510];
    int m540; // inferred: mode (2: teams)
    // inferred: modes 0 and 1 (name invented)
    bool IsFreeMode_unk() const {
        bool r = true;
        if (m540 != 0 && m540 != 1) {
            r = false;
        }
        return r;
    }
};
extern "C" Arena_unk* lbl_8087F610; // inferred: multiplayer manager (null in single player)
extern "C" atn::EfpFile* fn_800EFBC4(int id); // inferred: battle effect of row `id`
extern "C" atn::EfpFile* fn_800EFDA0(int id); // inferred: skill effect of row `id`
extern "C" const char* fn_800EFE3C(int id);   // inferred: sound of row `id`
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" int fn_80219558(int id); // inferred: party member kind of a character id
extern "C" int fn_80239D58(atn::EffectManager* mgr, void* owner, int id); // inferred: the effect is playing

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: node callback of a model (as game/CharaTask.cpp: the functor manager uses the default allocator of
// boost::function3, std::allocator<function_base>)
typedef boost::function<void(atn::Matrix3x4*, const atn::NodeChunk&, const atn::ModelNodeCallbackContext&),
                        std::allocator<boost::function_base> >
    NodeCallback_unk;
// inferred: sets the node callback of a model (weak copy 0x800F29D0 of an inline of atn::Model that the
// other files call by this name; ModelBase::mB8 is declared with another signature)
extern "C" void fn_800F29D0(atn::Model* model, NodeCallback_unk callback);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" void fn_8023A8B4(atn::EffectManager* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const EffectColor_unk& color, int arg7,
                            int arg8, int arg9, float scale);

// inferred: plays an EFP effect at the model, without offset (as gmk/Coffin.cpp)
inline void PlayEfp_unk(atn::EfpFile* efp, atn::Model* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    EffectColor_unk color;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

BattleTask* BattleTask::fn_800F13BC(atn::Task* parent) {
    if (parent != 0 && lbl_8087F048 == 0) {
        lbl_8087F048 = new ((MemoryTag)1, "", "", 0) BattleTask(parent);
    }
    return lbl_8087F048;
}

extern "C" PopupTask* fn_8054A6E8(atn::Task* parent); // inferred: creates the popup task (game/PopupTask.cpp)
extern "C" const char* const lbl_80881464; // inferred: "ef_neb01o.model" (.sdata2 of this file)

BattleTask::BattleTask(atn::Task* parent)
    : atn::Task(parent), mBulletId(0), mCB34(0), mCharaCount(0), m363B4(0), m363B8(0), m363BC(40),
      m367B0(2), m367B4(0), mHideLines(1), m38A00(0), m38A04(0.0f), m38A08(0), m38A0C(120.0f), m38A10(0.333f), m38A14(80.0f),
      m38A18(0.0f), m38A1C(80.0f), m38A20(20.0f), m38A24(10.0f), m38A28(0.0f), m38A2C(0.0f), m38A30(0.0f),
      m38A34(0.0f), m38A38(1.5f), m38A3C(1.0f), m38A40(1.0f), m38A44(0.006f), m38A48(0.01f), m38A4C(0.05f),
      m38A50(0.012f), m38A54(0.005f), m38C74(0), m38C78(0.0f), m38C7C(0.0f), m38C80(0.0f), m38C84(0.0f),
      m38C88(0.0f), m38C8C(1.0f), m3E2E0(0), m3E2E4(0), m3E2E8(0), m3E2EC(0), m3E2F8(0), m3E2FC(0.1f), m3E300(0),
      mStunCount(0), m3E390(0), m3E394(0), m3E398(0), m3E39C(0), m3E3A0(4.0f), m3E3A4(0.1f), m3E3A8(6.0f) {
    SkillEffectManager::fn_8059AB98(this);
    fn_8054A6E8(this);
    LogMessageTask::fn_80478714(this);
    PickTask::fn_80548D54(this);
    fn_800F39A4();
    fn_80100BF0();
    for (unsigned int i = 0; i < 8; i++) {
        mSlots[i].mModel.fn_8008AD4C(lbl_80881464, 0);
        mSlots[i].mUsed = 0;
        mSlots[i].mChara = 0;
    }
    mCB38.x = 0.0f;
    mCB38.y = 0.0f;
    mCB40.x = 0.0f;
    mCB40.y = 0.0f;
    mCB50.x = 0.0f;
    mCB50.y = 0.0f;
    mCB48.x = 0.0f;
    mCB48.y = 0.0f;
    mCB58 = 0;
    mCB5C = 0.0f;
    mCB64 = atn::Vector3(0.0f, 0.0f, 1.0f);
    atn::VecAssign_unk(mCB70, atn::Vector3(0.0f, 0.0f, 1.0f));
    m3E304 = EffectColor_unk(1.0f, 1.0f, 1.0f, 0.0f);
}

BattleTask::~BattleTask() {
    lbl_8087F048 = 0;
}

// inferred: setup: waits for the effects and the d2anime tasks, then binds the node callback of the effect
// models (the nodes follow the character the model is drawn on)
int BattleTask::vf0C() {
    int ready = 1;
    if (fn_800D3FA4() == 0) {
        ready = 0;
    }
    if (fn_800F3490() != 0) {
        ready = 0;
    }
    if (fn_800F3B4C() != 0) {
        ready = 0;
    }
    if (ready != 0) {
        mCB30->fn_800D246C(0);
        mCB30->mRepeat = 0;
        mCB30->FreezeTask();
        if (mCB60 != 0) {
            mCB60->fn_800D246C(0);
            mCB60->mRepeat = 1;
            mCB60->FreezeTask();
        }
        m3E2F4->fn_800D246C(0);
        m3E2F4->mRepeat = 0;
        m3E2F4->FreezeTask();
        for (unsigned int i = 0; i < 8; i++) {
            atn::Model* model = mSlots[i].mModel.GetModel_unk();
            if (model->m20C_31_24 == 8) {
                fn_800F29D0(model, boost::bind(&BetoModel::fn_8010D854, &mSlots[i], _1, _2, _3));
                model->fn_80091CFC("head");
                model->fn_80091CFC("lefthand");
                model->fn_80091CFC("leftleg");
                model->fn_80091CFC("righthand");
                model->fn_80091CFC("rightleg");
                model->fn_80091CFC("spine1");
                model->fn_80091CFC("waist");
            }
        }
        fn_800D246C(1);
    }
    return ready;
}

// inferred: sets the node callback of a model (ModelBase::mB8 is declared as boost::function<void()> in
// atn/Model_unk.h; its real signature is the one of NodeCallback_unk)
extern "C" void fn_800F29D0(atn::Model* model, NodeCallback_unk callback) {
    *(NodeCallback_unk*)&model->mB8 = callback;
}

// ---- fn_800F72F4: target marker of a character (over the inlining budget: every helper below is called
// out of line, and their weak copies follow it at 0x800F7F60-0x800F8548) ----

// BattleMarker_unk and BattleMarkers_unk are in game/BattleTask.h (BattleTask::mMarkers)
// inferred: weak copy 0x800F8538; name invented
inline BattleMarker_unk* BattleMarkers_unk::fn_800F8538(int i) {
    return &mItems[i];
}

extern "C" const char* lbl_808813F4; // inferred: "/effect/hateline/hate_line_01a.eff" (.sdata of this file)
extern "C" const char* lbl_808813F8; // inferred: "/effect/hateline/hate_line_01b.eff"
extern "C" const char* lbl_808813FC; // inferred: "/effect/hateline/hate_line_07a.eff"
extern "C" const char* lbl_80881400; // inferred: "/effect/hateline/hate_line_07b.eff"
extern "C" const char* lbl_80881404; // inferred: "/effect/hateline/hate_line_03a.eff"
extern "C" const char* lbl_80881408; // inferred: "/effect/hateline/hate_line_03b.eff"
extern "C" const char* lbl_808813D4; // inferred: "/data/d2anime/ui_seek.d2a"
extern "C" const char* lbl_808813D8; // inferred: "/data/d2anime/ui_seek_arow.d2a"
extern "C" const char* const lbl_80735988[3]; // inferred: splash effects (.rodata of this file)
extern "C" const char* fn_8020BD78(int index); // inferred: name of an effect palette
extern "C" int sprintf(char* s, const char* format, ...);

extern "C" void fn_800D58A4(atn::Texture* tex); // inferred: releases the texture (as game/GameUI.cpp)
// inferred: names of the battle effect files (const pointers of .sdata2 of this file to strings of .rodata)
extern "C" const char* const lbl_808813DC;
extern "C" const char* const lbl_808813E0;
extern "C" const char* const lbl_808813E4;
extern "C" const char* const lbl_808813E8;
extern "C" const char* const lbl_808813EC;
extern "C" const char* const lbl_808813F0;
extern "C" const char* const lbl_8088140C;
extern "C" const char* const lbl_80881410;
extern "C" const char* const lbl_80881414;
extern "C" const char* const lbl_80881418;
extern "C" const char* const lbl_8088141C;
extern "C" const char* const lbl_80881420;
extern "C" const char* const lbl_80881424;
extern "C" const char* const lbl_80881428;
extern "C" const char* const lbl_8088142C;
extern "C" const char* const lbl_80881430;
extern "C" const char* const lbl_80881434;
extern "C" const char* const lbl_80881438;
extern "C" const char* const lbl_8088143C;
extern "C" const char* const lbl_80881440;
extern "C" const char* const lbl_80881444;
extern "C" const char* const lbl_80881448;
extern "C" const char* const lbl_8088144C;
extern "C" const char* const lbl_80881450;
extern "C" const char* const lbl_80881454;
extern "C" const char* const lbl_80881458;
extern "C" const char* const lbl_8088145C;
extern "C" const char* const lbl_80881460;
extern "C" const char* const lbl_80881468;
extern "C" const char* const lbl_8088146C;
extern "C" const char* const lbl_80881470;
extern "C" const char* const lbl_80881474;
extern "C" const char* const lbl_80735A94[3]; // inferred: status effect files (.rodata of this file)
extern "C" const char* const lbl_80735A48[7]; // inferred: effect files of mEfp365E8 (.rodata of this file)

// inferred: load requests of the battle effects, once (m3E394); fn_800F3170 releases them. C name, called by
// GameTask as extern "C"
extern "C" void fn_800F2C9C(BattleTask* mgr) {
    if (mgr->m3E394 != 0) {
        return;
    }
    mgr->m3E394 = 1;
    char path[0x100];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 7; j++) {
            if (i == 0) {
                sprintf(path, "/effect/hit/ef_hg%03d.eff\0", j);
            } else if (i == 1) {
                sprintf(path, "/effect/hit/ef_hs%03d.eff\0", j);
            } else {
                sprintf(path, "/effect/ef_hm001c.eff\0");
            }
            mgr->mHitEff[i][j].LoadRequest(path);
        }
    }
    for (int i = 0; i < 5; i++) {
        sprintf(path, "/effect/hit/ef_hs000_%02d.eff\0", i);
        (&mgr->mEff3651C[i])->LoadRequest(path);
        sprintf(path, "/effect/hit/ef_hs001_%02d.eff\0", i);
        (&mgr->mEff36558[i])->LoadRequest(path);
    }
    for (int i = 0; i < 3; i++) {
        (&mgr->mEff36594[i])->LoadRequest(lbl_80735A94[i]);
    }
    mgr->mEfp365B8.LoadRequest(lbl_80881474);
    mgr->mEfp365C4.LoadRequest("ef_bs072h.efp");
    mgr->mEfp363C0.LoadRequest(lbl_808813E0);
    mgr->mEfp363CC.LoadRequest(lbl_808813E4);
    mgr->mEfp363D8.LoadRequest(lbl_808813E8);
    mgr->mEfp363E4.LoadRequest(lbl_808813EC);
    mgr->mCB7C.fn_800D5908(lbl_808813DC);
    mgr->mEfp364EC.LoadRequest(lbl_808813F0);
    mgr->mEfp38AF4.LoadRequest(lbl_8088140C);
    mgr->mEfp38B00.LoadRequest("ef_bu044a.efp");
    mgr->mEfp38B0C[0].LoadRequest(lbl_80881410);
    mgr->mEfp38B0C[1].LoadRequest(lbl_80881414);
    mgr->mEfp38B0C[2].LoadRequest(lbl_80881418);
    mgr->mEfp38B30.LoadRequest("ef_bu044d.efp");
    mgr->mEfp38B3C.LoadRequest(lbl_8088141C);
    mgr->mEfp38B48.LoadRequest("ef_bu044b.efp");
    mgr->mEfp38B54.LoadRequest(lbl_80881420);
    mgr->mEfp38B60.LoadRequest("ef_bu044e.efp");
    mgr->mEfp38B6C[0].LoadRequest(lbl_80881424);
    mgr->mEfp38B6C[1].LoadRequest(lbl_80881428);
    mgr->mEfp38B6C[2].LoadRequest(lbl_8088142C);
    mgr->mEfp38B90[0].LoadRequest(lbl_80881424);
    mgr->mEfp38B90[1].LoadRequest(lbl_80881428);
    mgr->mEfp38B90[2].LoadRequest(lbl_8088142C);
    mgr->mEfp38BB4.LoadRequest(lbl_80881434);
    mgr->mEfp38BC0.LoadRequest(lbl_80881438);
    mgr->mEfp38BCC.LoadRequest(lbl_80881438);
    mgr->mEfp38BD8.LoadRequest(lbl_8088143C);
    mgr->mEfp38BE4.LoadRequest(lbl_80881440);
    mgr->mEfp38BF0.LoadRequest(lbl_80881444);
    mgr->mEfp38BFC.LoadRequest(lbl_80881448);
    mgr->mEfp38C08.LoadRequest(lbl_8088144C);
    mgr->mEfp38C20.LoadRequest(lbl_80881450);
    mgr->mEfp38C2C.LoadRequest(lbl_80881454);
    mgr->mEfp38C38.LoadRequest(lbl_80881458);
    mgr->mEfp38C44.LoadRequest(lbl_8088145C);
    mgr->m389F4.LoadRequest(lbl_80881430);
    mgr->mEfp38C50.LoadRequest(lbl_80881460);
    mgr->mEfp38C5C.LoadRequest("ef_uc067.efp");
    mgr->mEfp38C68.LoadRequest(lbl_80881468);
    mgr->mEfp365D0.LoadRequest(lbl_8088146C);
    mgr->mEfp365DC.LoadRequest(lbl_80881470);
    for (int i = 0; i < 7; i++) {
        mgr->mEfp365E8[i].LoadRequest(lbl_80735A48[i]);
    }
    mgr->mStunEfp[0][0].LoadRequest("ef_ui018a.efp");
    mgr->mStunEfp[0][1].LoadRequest("ef_ui018b.efp");
    mgr->mStunEfp[0][2].LoadRequest("ef_ui018c.efp");
    mgr->mStunEfp[1][0].LoadRequest("ef_ui008a.efp");
    mgr->mStunEfp[1][1].LoadRequest("ef_ui008b.efp");
    mgr->mStunEfp[1][2].LoadRequest("ef_ui008c.efp");
    lbl_8087F9E8->fn_8059AFD8();
}


// inferred: releases the battle effects once (m3E394) and the effect files of the skills. C name, called by
// GameTask as extern "C"
extern "C" void fn_800F3170(BattleTask* mgr) {
    if (mgr->m3E394 == 0) {
        return;
    }
    mgr->m3E394 = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 7; j++) {
            mgr->mHitEff[i][j].Destroy();
        }
    }
    for (int i = 0; i < 5; i++) {
        (&mgr->mEff3651C[i])->Destroy();
        (&mgr->mEff36558[i])->Destroy();
    }
    for (int i = 0; i < 3; i++) {
        (&mgr->mEff36594[i])->Destroy();
    }
    mgr->mEfp365B8.Destroy();
    mgr->mEfp365C4.Destroy();
    mgr->mEfp363C0.Destroy();
    mgr->mEfp363CC.Destroy();
    mgr->mEfp363D8.Destroy();
    mgr->mEfp363E4.Destroy();
    fn_800D58A4(&mgr->mCB7C);
    mgr->mEfp364EC.Destroy();
    mgr->mEfp38AF4.Destroy();
    mgr->mEfp38B00.Destroy();
    for (int i = 0; i < 3; i++) {
        (&mgr->mEfp38B0C[0])[i].Destroy();
    }
    mgr->mEfp38B30.Destroy();
    mgr->mEfp38B3C.Destroy();
    mgr->mEfp38B48.Destroy();
    mgr->mEfp38B54.Destroy();
    mgr->mEfp38B60.Destroy();
    for (int i = 0; i < 3; i++) {
        (&mgr->mEfp38B6C[0])[i].Destroy();
        (&mgr->mEfp38B90[0])[i].Destroy();
    }
    mgr->mEfp38BB4.Destroy();
    mgr->mEfp38BE4.Destroy();
    mgr->mEfp38BF0.Destroy();
    mgr->mEfp38BFC.Destroy();
    mgr->mEfp38BC0.Destroy();
    mgr->mEfp38BCC.Destroy();
    mgr->mEfp38BD8.Destroy();
    mgr->mEfp38C08.Destroy();
    mgr->mEfp38C20.Destroy();
    mgr->mEfp38C2C.Destroy();
    mgr->mEfp38C38.Destroy();
    mgr->mEfp38C44.Destroy();
    mgr->m389F4.Destroy();
    mgr->mEfp38C50.Destroy();
    mgr->mEfp38C5C.Destroy();
    mgr->mEfp38C68.Destroy();
    mgr->mEfp365D0.Destroy();
    mgr->mEfp365DC.Destroy();
    for (int i = 0; i < 7; i++) {
        mgr->mEfp365E8[i].Destroy();
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            mgr->mStunEfp[i][j].Destroy();
        }
    }
    lbl_8087F9E8->fn_8059B0F0();
}

extern "C" int fn_800EF91C(); // inferred: nonzero while the effects of game/BattleEfp_unk load

// inferred: nonzero while the battle effects (the same as fn_800F3170 releases), the models of mSlots, the stun
// effects or the skill effect files load
int BattleTask::fn_800F3490() {
    int loading = 0;
    if (fn_800EF91C()) {
        loading = 1;
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 7; j++) {
            if (mHitEff[i][j].IsLoading()) {
                loading = 1;
            }
        }
    }
    for (int i = 0; i < 5; i++) {
        if ((&mEff3651C[i])->IsLoading()) {
            loading = 1;
        }
        if ((&mEff36558[i])->IsLoading()) {
            loading = 1;
        }
    }
    for (int i = 0; i < 3; i++) {
        if ((&mEff36594[i])->IsLoading()) {
            loading = 1;
        }
    }
    if (mEfp365B8.IsLoading() || mEfp365C4.IsLoading()) {
        loading = 1;
    }
    if (mEfp363C0.IsLoading()) {
        loading = 1;
    }
    if (mEfp363CC.IsLoading()) {
        loading = 1;
    }
    if (mEfp363D8.IsLoading()) {
        loading = 1;
    }
    if (mEfp363E4.IsLoading()) {
        loading = 1;
    }
    // inferred: atn::Texture "is loading" (0x800D59B8, named as GimmickUvOwner_unk like gmk/AutomaticDoor.cpp)
    if (((GimmickUvOwner_unk*)&mCB7C)->fn_800D59B8()) {
        loading = 1;
    }
    if (mEfp364EC.IsLoading()) {
        loading = 1;
    }
    if (mEfp38AF4.IsLoading() || mEfp38B00.IsLoading()) {
        loading = 1;
    }
    for (int i = 0; i < 3; i++) {
        if ((&mEfp38B0C[0])[i].IsLoading()) {
            loading = 1;
        }
    }
    if (mEfp38B30.IsLoading()) {
        loading = 1;
    }
    if (mEfp38B3C.IsLoading() || mEfp38B54.IsLoading()) {
        loading = 1;
    }
    if (mEfp38B48.IsLoading() || mEfp38B60.IsLoading()) {
        loading = 1;
    }
    for (int i = 0; i < 3; i++) {
        if ((&mEfp38B6C[0])[i].IsLoading() || (&mEfp38B90[0])[i].IsLoading()) {
            loading = 1;
        }
    }
    if (mEfp38BB4.IsLoading() || mEfp38BC0.IsLoading() || mEfp38BCC.IsLoading() ||
        mEfp38BD8.IsLoading() || mEfp38BE4.IsLoading() || mEfp38BF0.IsLoading() ||
        mEfp38BFC.IsLoading() || mEfp38C08.IsLoading() || mEfp38C20.IsLoading() ||
        mEfp38C2C.IsLoading() || mEfp38C38.IsLoading() || mEfp38C44.IsLoading()) {
        loading = 1;
    }
    if (m389F4.IsLoading()) {
        loading = 1;
    }
    if (mEfp38C50.IsLoading()) {
        loading = 1;
    }
    if (mEfp38C5C.IsLoading()) {
        loading = 1;
    }
    if (mEfp38C68.IsLoading()) {
        loading = 1;
    }
    if (mEfp365D0.IsLoading()) {
        loading = 1;
    }
    if (mEfp365DC.IsLoading()) {
        loading = 1;
    }
    for (int i = 0; i < 7; i++) {
        if (mEfp365E8[i].IsLoading()) {
            loading = 1;
        }
    }
    for (unsigned int i = 0; i < 8; i++) {
        if (mSlots[i].mModel.fn_8008B140()) {
            loading = 1;
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            if (mStunEfp[i][j].IsLoading()) {
                loading = 1;
            }
        }
    }
    if (lbl_8087F9E8->fn_8059B1B4()) {
        loading = 1;
    }
    return loading;
}

// NONMATCHING: 97.9% - the first string pointer is loaded before the register saves in the original; string pool
// offsets differ (file not complete)
// inferred: load requests of the battle resources (called by the constructor): the target lines, the
// d2anime tasks, the splash and palette effects
void BattleTask::fn_800F39A4() {
    mEnemyLine.mBase.LoadRequest(lbl_808813F4);
    mEnemyLine.mHead.LoadRequest(lbl_808813F8);
    mOwnLine.mBase.LoadRequest(lbl_808813FC);
    mOwnLine.mHead.LoadRequest(lbl_80881400);
    mAppearLine.mBase.LoadRequest(lbl_80881404);
    mAppearLine.mHead.LoadRequest(lbl_80881408);
    mLineTex.fn_800D5908("/battle/hatemove.texture");
    mCB30 = fn_801F3FF8(this, lbl_808813D4, 0);
    mCB30->fn_800D246C(1);
    mCB60 = fn_801F3FF8(this, lbl_808813D8, 0);
    mCB60->fn_800D246C(1);
    m3E2F4 = fn_801F3FF8(this, "data/d2anime/ui_hp_up.d2a", 0);
    m3E2F4->fn_800D246C(1);
    for (int i = 0; i < 3; i++) {
        mSplashEfp[i].LoadRequest(lbl_80735988[i]);
    }
    mEfp38C14.LoadRequest("data/eff/ef_sc024.efp");
    for (unsigned int i = 0; i < 31; i++) {
        char path[256];
        sprintf(path, "/data/efp/ef_%s.efp", fn_8020BD78(i));
        mEfp3663C[i].LoadRequest(path);
    }
}

// inferred: both effects of a line pair are loading (inlined); name invented
inline bool IsLoading_unk(BattleLineEff_unk& line) {
    bool loading = false;
    if (line.mBase.IsLoading() || line.mHead.IsLoading()) {
        loading = true;
    }
    return loading;
}

// inferred: nonzero while the resources requested by fn_800F39A4 load
int BattleTask::fn_800F3B4C() {
    int loading = 0;
    if (IsLoading_unk(mEnemyLine) || IsLoading_unk(mOwnLine)) {
        loading = 1;
    }
    if (IsLoading_unk(mAppearLine)) {
        loading = 1;
    }
    // inferred: the texture is seen through GimmickUvOwner_unk, as in the other callers of fn_800D59B8
    if (((GimmickUvOwner_unk*)&mLineTex)->fn_800D59B8()) {
        loading = 1;
    }
    if (!mCB30->IsFlag_unk(2)) {
        loading = 1;
    }
    if (!mCB60->IsFlag_unk(2)) {
        loading = 1;
    }
    if (!m3E2F4->IsFlag_unk(2)) {
        loading = 1;
    }
    for (int i = 0; i < 3; i++) {
        if (mSplashEfp[i].IsLoading()) {
            loading = 1;
        }
    }
    if (mEfp38C14.IsLoading()) {
        loading = 1;
    }
    for (unsigned int i = 0; i < 31; i++) {
        if (mEfp3663C[i].IsLoading()) {
            loading = 1;
        }
    }
    return loading;
}

// NONMATCHING: 92.9% - registers: in the original the marker of entry i is reached through a separate offset (i * 0x78
// added to this + 0x30000) and its zero stores use their own register
// inferred: resets the battle (GameTask::vf10 at a map change): removes the bullets and clears the characters,
// their hate, the target markers and the effect slots. C name, called by game/GameTask.cpp as extern "C"
extern "C" void fn_800F3D28(BattleTask* mgr) {
    mgr->fn_801011B8();
    for (int i = 0; i < 0x48; i++) {
        mgr->mCharas[i].mChara = 0;
        mgr->mCharas[i].m92D = 0;
        mgr->mCharas[i].m248 = 1.0f;
        mgr->mCharas[i].m92E = 0;
        mgr->mCharas[i].m92F = 0;
        mgr->mCharas[i].fn_801081D8();
        memset(mgr->m391E0[i].m, 0, sizeof(mgr->m391E0[i].m));
        mgr->mMarkers.mItems[i].m70 = 0;
        mgr->mMarkers.mItems[i].mFlags = 0;
        mgr->mMarkers.mItems[i].mMode = 0;
    }
    mgr->mCharaCount = 0;
    mgr->mTimers.mCount = 0;
    mgr->mStunCount = 0;
    for (int i = 0; i < 8; i++) {
        mgr->mSlots[i].mChara = 0;
        mgr->mSlots[i].mUsed = 0;
    }
}

// inferred: player character as seen from here (as game/CharaTask.cpp)
struct Player_unk : public BattleCharaTask {
    // inferred: weak copy 0x800F7FB4 (name invented)
    int GetM146C_unk() { return m146C; }
};

// inferred: the "Hate" block of the game config (from 0x350); names as in GameConfig_unk
struct HateConfig_unk {
    int mTargetedNumDefault;       // 0x00
    int mHideDown;                 // 0x04
    int mTargetSightDown;          // 0x08
    int mHateRatioDown;            // 0x0C
    int mHateLineFadeout;          // 0x10
    unsigned int mLineColorNormal; // 0x14
    unsigned int mLineColorSword;  // 0x18
    unsigned int mLineColorArcher; // 0x1C
    unsigned int mLineColorMagic;  // 0x20
    int mPlayerMinusHate;          // 0x24
    int mIgnoreLongHate;           // 0x28
    int mIgnoreLongHateValue[10];  // 0x2C: by character kind (CharaData_unk::m121)
    int mIgnoreLongHateRand[10];   // 0x54
};

// inferred: segment clipped by the frustum (empty constructor: weak copy 0x800F8544); names invented
struct LineSeg_unk {
    LineSeg_unk() {}
    atn::Vector3 mStart;
    atn::Vector3 mEnd;
};

// inferred: vertex of the texture commands (0x18, as atn/DrawHelper.cpp)
namespace atn {
struct DrawTexVertex_unk {
    DrawTexVertex_unk() {}
    Vector3 mPos;
    unsigned int mColor;
    Vec2_unk mUV;
};
} // namespace atn

extern "C" PlayerManager* fn_8000D9E8(); // inferred: player manager (weak copy of another file)
// inferred: first player of the list (weak copy of another file)
inline PlayerTask* PlayerManager::GetHead_unk() {
    return mHead;
}
extern "C" const char* const lbl_80735DD0[]; // inferred: sound names of the markers (.rodata of this file)
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);

// inferred: weak copies of game/BattleTask.cpp (C names, called out of line by other files too)
extern "C" inline void* fn_800F7F90() {
    return lbl_8087F610;
}
extern "C" inline void fn_800F7F98(atn::EffectManager* mgr, int replaying) {
    mgr->mSystem.mReplaying = replaying;
}
extern "C" inline atn::EffectManager* fn_800F7FA0() {
    return lbl_8087F3C0;
}
extern "C" inline void fn_800F7FA8(atn::EffectManager* mgr, void* owner, int group) {
    fn_80232B7C((unsigned int)owner, group);
}
// inferred: center of the collision (the player keeps its own copy)
extern "C" inline const atn::Vector3* fn_800F7FBC(BattleCharaTask* chara) {
    if (chara->mFlagC31) {
        return &chara->m148C;
    }
    return &chara->mColli.m48;
}
// inferred: radius of the collision
extern "C" inline float fn_800F7FF8(BattleCharaTask* chara) {
    if (chara->mFlagC31) {
        return chara->m1498;
    }
    return chara->m5B0;
}
// inferred: colour of a 0xAARRGGBB value
extern "C" inline atn::Color fn_800F8014(unsigned int rgba) {
    return atn::Color(0.003921569f * (float)((rgba >> 16) & 0xFF), 0.003921569f * (float)((rgba >> 8) & 0xFF),
                      0.003921569f * (float)(rgba & 0xFF), 0.003921569f * (float)(rgba >> 24));
}
extern "C" inline HateConfig_unk* fn_800F8098(GameConfig_unk* config) {
    return (HateConfig_unk*)&config->mTargetedNumDefault;
}
extern "C" inline const Frustum_unk* fn_800F80A0(atn::Camera* camera) {
    return &camera->mFrustum;
}
// inferred: translation matrix by value
extern "C" inline atn::Matrix3x4 fn_800F80A8(const atn::Vector3* pos) {
    atn::Matrix3x4 m;
    PSMTXTrans(m.m, pos->x, pos->y, pos->z);
    return m;
}
// inferred: mtx = mtx * rot_z * rot_y * rot_x
extern "C" inline void fn_800F80B8(MtxPtr mtx, const atn::Vector3* rot) {
    ((atn::Matrix3x4*)mtx)->RotateZYX_unk(*rot);
}
// inferred: mtx = mtx * scale
extern "C" inline void fn_800F8290(MtxPtr mtx, float x, float y, float z) {
    Mtx s;
    PSMTXScale(s, x, y, z);
    Mtx t;
    PSMTXConcat(mtx, s, t);
    __MtxCopy(mtx, t);
}
extern "C" inline void fn_800F831C(atn::DrawTexVertex_unk* v, const atn::Vector3* pos, unsigned int color,
                                   const atn::Vec2_unk* uv) {
    v->mPos = *pos;
    v->mColor = color;
    v->mUV = *uv;
}
extern "C" inline atn::Model* fn_800F84C8(CharaTask* chara) {
    return (atn::Model*)&chara->mMotion;
}
extern "C" inline void fn_800F84D0(atn::EffectManager* mgr, int layer) {
    mgr->mSystem.mLayer = layer;
}
// inferred: operators of atn::Vec2_unk (inlined; names as the Vector3 ones)
inline atn::Vec2_unk operator-(const atn::Vec2_unk& a, const atn::Vec2_unk& b) {
    return atn::Vec2_unk(a.x - b.x, a.y - b.y);
}
inline atn::Vec2_unk operator*(const atn::Vec2_unk& a, float t) {
    return atn::Vec2_unk(a.x * t, a.y * t);
}
inline atn::Vec2_unk operator+(const atn::Vec2_unk& a, const atn::Vec2_unk& b) {
    return atn::Vec2_unk(a.x + b.x, a.y + b.y);
}
extern "C" inline atn::Vec2_unk fn_800F84D8(const atn::Vec2_unk& a, const atn::Vec2_unk& b, float t) {
    return (b - a) * t + a;
}
extern "C" inline float fn_800F8524(const float* a, const float* b, float t) {
    return t * (*b - *a) + *a;
}
extern "C" inline GameConfig_unk* fn_800F52F8() {
    return lbl_8087F0A8;
}
extern "C" inline int fn_800F5300(CharaTask* chara) {
    return chara->mFlag28;
}
extern "C" inline CharaC58_unk* fn_800F530C(CharaTask* chara) {
    return &chara->mC58;
}
extern "C" inline atn::DrawHelper* fn_800F7250() {
    return lbl_8087EEB0;
}
inline atn::Camera* atn::RenderManager::fn_800F7258() {
    return &mCamera;
}

// inferred: character that can be aimed at (can act and not the 0x2000 flag of m54C); inlined in vf10
inline bool IsBattleTarget_unk(CharaTask* chara) {
    return chara->CanAct_unk() && (chara->m54C & 0x2000) != 0x2000;
}
// inferred: an enemy can still fight (inlined in vf10)
inline bool HasBattleEnemy_unk() {
    for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
        if (e->CanAct_unk() && (e->m54C & 0x2000) != 0x2000) {
            return true;
        }
    }
    return false;
}

inline unsigned int BattleTargets_unk::size() const {
    return mSize;
}
inline void BattleTargets_unk::clear() {
    mSize = 0;
}
inline void BattleTargets_unk::push_back(CharaTask* const& chara) {
    new (&mData[mSize]) CharaTask*(chara);
    mSize++;
}
inline BattleShots_unk::iterator::iterator(BattleShots_unk* ring, unsigned int index) : mRing(ring), mIndex(index) {}
inline BattleShot_unk& BattleShots_unk::iterator::operator*() const {
    return (*mRing)[mIndex];
}
inline BattleShot_unk* BattleShots_unk::iterator::operator->() const {
    return &**this;
}
inline BattleShots_unk::iterator& BattleShots_unk::iterator::operator++() {
    mIndex++;
    return *this;
}
inline bool BattleShots_unk::iterator::operator!=(iterator o) const {
    return mIndex != o.mIndex;
}
inline BattleShots_unk::iterator BattleShots_unk::begin() {
    return iterator(this, 0);
}
inline BattleShots_unk::iterator BattleShots_unk::end() {
    return iterator(this, mCount);
}
inline BattleShot_unk& BattleShots_unk::operator[](unsigned int i) {
    return mItems[(mHead + i) & 31];
}
inline BattleShots_unk::iterator BattleShots_unk::erase(iterator it) {
    for (unsigned int i = it.mIndex; i < mCount - 1; i++) {
        (*this)[i] = (*this)[i + 1];
    }
    mCount = mCount - 1;
    return it;
}

extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);

// inferred: values of the panes of a d2anime layout (names invented, as game/BattleResult.cpp)
inline void SetPaneValue_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}
inline void SetPaneValue_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
extern "C" float fn_8006EF48(atn::Font_unk* font, float size, const wchar_t* text, float f2, int a, int b);
extern "C" void fn_8006F2F0(atn::Font_unk* font, wchar_t* dst, const char* src, int n); // inferred: to wide text
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...);                      // inferred: wide sprintf
extern "C" int swprintf(wchar_t* s, unsigned int n, const wchar_t* format, ...);
extern "C" double sin(double x);
extern "C" double asin(double x);

// inferred: an enemy that notices the leader in the field plays the "alert" sound once (inlined in vf10;
// name invented)
inline void BattleTask::UpdateAlert_unk(CharaTask* e, PlayerTask* leader) {
    BattleChara_unk* entry = &mCharas[FindChara_unk(e)];
    if (leader->mFlag31) {
        if ((e->GetStatus_unk()->m0C & 0x800) == 0x800 && entry->m804 <= 0 && entry->m92C == 0 &&
            entry->mChara->vfC4()) {
            entry->m92C = 1;
            atn::Vector3 d = e->mPos - leader->mPos;
            if (PSVECSquareMag(&d) < 6400.0f) {
                entry->m804 = 150;
                entry->mChara->fn_801656A4(150);
                fn_800C31F4(lbl_80735DD0[22], 0, -1, 1.0f);
            }
        }
    } else {
        entry->m92C = 0;
    }
}

inline void BattleTask::BetoModel::SetMtx_unk(const atn::Matrix3x4& mtx) {
    *(atn::Matrix3x4*)mModel.mMtx.m = mtx;
    atn::Vector3 scale(atn::Vector3(mtx.m[0][0], mtx.m[1][0], mtx.m[2][0]).Length_unk(),
                       atn::Vector3(mtx.m[0][1], mtx.m[1][1], mtx.m[2][1]).Length_unk(),
                       atn::Vector3(mtx.m[0][2], mtx.m[1][2], mtx.m[2][2]).Length_unk());
    mModel.m54 = MAX(scale.x, MAX(scale.y, scale.z));
}

// inferred: update of the battle every frame: rebuilds the lock-on targets (mTargets: enemies, near NPCs and
// friends), the nearest opponent of every enemy and friend (BattleChara_unk::m04), the "alert" sound of the
// enemies near the leader, the mE0_12 flag of the friends, updates the bullets, the timers of mTimers.mItems, the hate,
// the morale gauge, the debug shot records and the models of mSlots
// NONMATCHING: 98.5% - registers of the shot ring loop (the original reloads the iterator from the stack), base
// register of the slot model (r29) and stack slots of the column temporaries
void BattleTask::vf10() {
    if (lbl_8087F8A0 == 0 || EnemyManager::sInstance == 0) {
        return;
    }
    mTargets.clear();
    PlayerTask* leader = lbl_8087F8A0->mHead;
    for (int i = 0; i < 72; i++) {
        m36290[i] = 0;
    }

    for (CharaTask* e = EnemyManager::sInstance->mHead; e != 0; e = ((EnemyTask*)e)->mNext) {
        if (e->CanAct_unk() && (e->m54C & 0x2000) != 0x2000) {
            int added = 0;
            float best = 1.0e16f;
            CharaTask* nearest = 0;
            for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
                if (!p->CanAct_unk() || (p->m54C & 0x2000) == 0x2000) {
                    continue;
                }
                atn::Vector3 d = p->mPos - e->mPos;
                if (added == 0) {
                    if (!e->fn_8016F3D0(leader, 1) && mTargets.size() < 64) {
                        mTargets.push_back(e);
                    }
                    added = 1;
                }
                if (PSVECSquareMag(&d) < best) {
                    best = PSVECSquareMag(&d);
                    nearest = p;
                }
            }
            int index = FindChara_unk(e);
            if (index >= 0) {
                mCharas[index].m04 = nearest;
            }
            fn_80103E60(e);
            fn_80103600(e);
            if (e->m55C != 7) {
                UpdateAlert_unk(e, leader);
            }
            e->fn_80154488();
        }
    }

    for (CharaTask* n = NpcManager::sInstance->mHead; n != 0; n = ((NpcTask*)n)->mNext) {
        if (!n->CanAct_unk() || (n->m54C & 0x2000) == 0x2000) {
            continue;
        }
        atn::Vector3 d = leader->mPos - n->mPos;
        if (PSVECMag(&d) < 150.0f && ((NpcTask*)n)->m1428 != 0 && mTargets.size() < 64) {
            mTargets.push_back(n);
        }
    }

    for (CharaTask* f = FriendManager::sInstance->fn_8036554C(); f != 0; f = ((PlayerTask*)f)->mNext) {
        if (!f->CanAct_unk() || (f->m54C & 0x2000) == 0x2000) {
            continue;
        }
        atn::Vector3 d = leader->mPos - f->mPos;
        int near = 0;
        CharaTask* nearest = 0;
        float best = 1.0e16f;
        if (PSVECMag(&d) < 150.0f) {
            if (!f->fn_8016F3D0(leader, 1) || (leader->GetStatus_unk()->m0C & 0x200000) == 0x200000 ||
                lbl_8087F0A8->m0CC != 0) {
                if (mTargets.size() < 64) {
                    mTargets.push_back(f);
                }
            }
            near = 1;
        }
        for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
            if (!e->CanAct_unk() || (e->m54C & 0x2000) == 0x2000 || e->fn_8016F368(f->mType)) {
                continue;
            }
            atn::Vector3 v = f->mPos - e->mPos;
            if (near == 0 && PSVECMag(&v) < 150.0f) {
                near = 1;
            }
            if (PSVECSquareMag(&v) < best) {
                best = PSVECSquareMag(&v);
                nearest = e;
            }
        }
        for (CharaTask* o = FriendManager::sInstance->fn_8036554C(); o != 0; o = ((PlayerTask*)o)->mNext) {
            if (!o->CanAct_unk() || (o->m54C & 0x2000) == 0x2000 || o->fn_8016F368(f->mType)) {
                continue;
            }
            atn::Vector3 v = f->mPos - o->mPos;
            if (PSVECSquareMag(&v) < best) {
                best = PSVECSquareMag(&v);
                nearest = o;
            }
        }
        int index = FindChara_unk(f);
        if (index >= 0) {
            mCharas[index].m04 = nearest;
        }
        fn_80103E60(f);
        fn_80103600(f);
        if (fn_800F52F8()->m098 != 0) {
            if (f->fn_8016E484()) {
                int leaderOn = 0;
                int aiOn = 0;
                int gaugeOn = 0;
                int off = lbl_8087F430->fn_803761BC() == 0;
                unsigned int status = f->GetStatus_unk()->m0C;
                if ((status & 2) == 2 || (status & 0x100) == 0x100 || (status & 0x10000) == 0x10000) {
                    off = 1;
                }
                if (lbl_8087F0A8->m194 != 0) {
                    off = 1;
                }
                PlayerTask* head = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
                if (head != 0 && head->CanAct_unk() && f->fn_8016F3D0(head, 1)) {
                    leaderOn = fn_800F5300(head);
                }
                if (lbl_8087EE68->m7D8 != 0) {
                    aiOn = lbl_8087EE68->mE0.size() != 0;
                }
                if (lbl_8087F0A8->m090 != 0) {
                    float gauge = lbl_8087F048->m38A04;
                    switch (fn_80219558(f->m50)) {
                    case 2:
                        gaugeOn = fn_800F530C(f)->mE0_12 ? gauge >= 100.0f : gauge >= 50.0f;
                        break;
                    case 3:
                        gaugeOn = fn_800F530C(f)->mE0_12 ? gauge >= 50.0f : gauge >= 10.0f;
                        break;
                    case 6:
                        aiOn = 0;
                        gaugeOn = fn_800F530C(f)->mE0_12 ? gauge >= 80.0f : gauge >= 50.0f;
                        break;
                    }
                }
                if (off) {
                    fn_800F530C(f)->mE0_12 = 0;
                } else {
                    fn_800F530C(f)->mE0_12 = !leaderOn && !aiOn && !gaugeOn;
                }
                if (lbl_8087F430->fn_80370174(905) == 1) {
                    fn_800F530C(f)->mE0_12 = 1;
                }
            } else {
                fn_800F530C(f)->mE0_12 = 0;
            }
        } else {
            fn_800F530C(f)->mE0_12 = 0;
        }
        f->fn_80154488();
    }

    fn_80103E60(lbl_8087F8A0->mHead);
    if (lbl_8087F8A0->mHead->m55C == 2) {
        fn_80103600(lbl_8087F8A0->mHead);
        lbl_8087F8A0->mHead->fn_80154488();
    }

    for (unsigned int i = 0; i < 256; i++) {
        if (mBullets[i].HasFlag_unk(0x2000000)) {
            mBullets[i].m04 &= ~0x2000000;
        }
        if (mBullets[i].HasFlag_unk(0x10000)) {
            mBullets[i].vf10();
        }
    }
    for (unsigned int i = 0; i < 32; i++) {
        if (mHomings[i].HasFlag_unk(0x10000)) {
            mHomings[i].vf10();
        }
    }

    BattleTimer_unk* it = mTimers.mItems;
    while (it != mTimers.mItems + mTimers.mCount) {
        if (--it->m04 <= 0) {
            unsigned int i = it - mTimers.mItems;
            for (; i < mTimers.mCount - 1; i++) {
                mTimers.mItems[i] = mTimers.mItems[i + 1];
            }
            mTimers.mCount = mTimers.mCount - 1;
        } else {
            it++;
        }
    }

    fn_80103FD4();
    fn_801055A8(0);
    fn_80109F7C();

    if (lbl_8087F430->fn_80375184() && HasBattleEnemy_unk()) {
        m38A04 = MAX(m38A04 - m38A10 * (fn_800F5300(lbl_8087F8A0->mHead) ? m38A3C : 1.0f), 0.0f);
    }

    fn_8010ACCC();
    fn_8010AEF4();
    fn_8010C3E4();
    fn_80107584();
    fn_8010A868();

    for (BattleShots_unk::iterator shot = mShots.begin(); shot != mShots.end();) {
        shot->m24++;
        if (shot->m24 >= 60) {
            shot = mShots.erase(shot);
        } else {
            ++shot;
        }
    }

    for (unsigned int i = 0; i < 8; i++) {
        if (mSlots[i].mChara != 0 && !mSlots[i].mChara->CanAct_unk()) {
            mSlots[i].mChara = 0;
            mSlots[i].mUsed = 0;
        }
        if (mSlots[i].mUsed != 0) {
            CharaTask* chara = mSlots[i].mChara;
            if (chara != 0) {
                mSlots[i].SetMtx_unk(chara->GetModel_unk()->mMtx);
                mSlots[i].mModel.CalcMatrixStack(0, atn::ModelNodeCallback_unk());
            }
        }
    }
}

// inferred: inline form of fn_8010C948 (the character `chara` hates most), inlined in vf14; name invented
inline CharaTask* BattleTask::GetHated_unk(CharaTask* chara) {
    int i = FindChara_unk(chara);
    if (i < 0) {
        return 0;
    }
    int best = 0;
    int found = -1;
    int* row = m391E0[i].m;
    for (int j = 0; j < mCharaCount; j++) {
        if (best < row[j]) {
            best = row[j];
            found = j;
        }
    }
    if (found < 0 || mCharaCount <= found) {
        return 0;
    }
    return mCharas[found].mChara;
}

// inferred: draw of the battle every frame: the reticle (d2anime mCB30 and the arrow mCB60 turned toward the
// aimed direction), the debug displays (morale gauge, enemies aiming at the leader, hate tables, hate range,
// shot records), the models of mSlots and the end of the d2anime m3E2F4
// NONMATCHING: 85% - register allocation of the debug loops (the original keeps the base of mCharas and the
// constants in other registers, index registers in the shot loop) and stack layout (0x100 bytes more)
void BattleTask::vf14() {
    atn::Vector3 aim = mCB70;
    if (mCB34 != 0) {
        mCB30->MeltTask();
        if (m3E2E0 != 0 || m3E2E4 != 0 || m3E2EC != 0) {
            mCB30->mSpeed = 1.0f;
        } else {
            mCB30->mSpeed = -1.0f;
        }
        mCB60->MeltTask();
        int arrow = mCB58;
        if (m3E2E0 != 0 || m3E2E4 != 0 || m3E2EC != 0) {
            arrow = 0;
        }
        if (arrow != 0) {
            float t = mCB5C + 0.1f;
            mCB5C = 1.0f < t ? 1.0f : t;
        } else {
            float t = mCB5C - 0.1f;
            mCB5C = 0.0f > t ? 0.0f : t;
            aim = atn::Vector3(0.0f, 0.0f, 1.0f);
        }
        SetPaneValue_unk(&mCB60->mData.m10, "arrowA", mCB5C);
    } else {
        mCB30->mFrame = 0.0f;
        mCB30->FreezeTask();
        mCB38.x = 0.0f;
        mCB38.y = 0.0f;
        mCB40.x = 0.0f;
        mCB40.y = 0.0f;
        mCB50.x = 0.0f;
        mCB50.y = 0.0f;
        mCB48.x = 0.0f;
        mCB48.y = 0.0f;
        mCB60->mFrame = 0.0f;
        mCB60->FreezeTask();
        mCB5C = 0.0f;
        aim = atn::Vector3(0.0f, 0.0f, 1.0f);
    }

    mCB64 = atn::Lerp_unk(mCB64, aim, 0.15f);
    float rot = 0.0f;
    if (!mCB64.IsZero_unk()) {
        atn::Vector3 r(mCB64);
        r.fn_80139F60();
        rot = 57.295776f * r.y;
    }
    SetPaneValue_unk(&mCB30->mData.m10, "arrowRot", rot);
    SetPaneValue_unk(&mCB60->mData.m10, "arrowRot", rot);
    mCB34 = 0;
    mCB58 = 0;

    if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 0 && m38A08 != 0) {
        float width = 580.0f;
        float height = 15.0f;
        fn_800F7250()->fn_80060D58(30.0f, 30.0f, 20.0f, width, height, 0xFFFF0033);
        fn_800F7250()->fn_80060D58(30.0f, 30.0f, 20.0f, width * m38A04 / m38A0C, height, 0xFF0066FF);
        char text[16];
        sprintf(text, "%.2f", m38A04);
        fn_800F7250()->fn_800616C0(32.0f + width * m38A04 / m38A0C, 30.0f, 20.0f, 14.0f, 14.0f, text, 0xFFFFFFFF,
                                   0.0f, 1, 1, 0);
    }

    PlayerTask* leader = lbl_8087F8A0->mHead;
    if (m367B0 > 0 && m367B4 != 0) {
        atn::Vector3 pos = leader->mPos;
        pos.y += 18.0f;
        pos = atn::RenderManager::instance_->GetScreenPosition(pos);
        int total = 0;
        int aimed = 0;
        for (CharaTask* e = EnemyManager::sInstance->mHead; e != 0; e = ((EnemyTask*)e)->mNext) {
            if (e->CanAct_unk()) {
                total++;
                if (e->mC58.mC4 == leader) {
                    aimed++;
                }
            }
        }
        wchar_t text[32];
        swprintf(text, 32, L"%2d/%2d", aimed, total);
        float width = fn_8006EF48(lbl_8087EEC8, 12.0f, text, 0.0f, 1, 1);
        fn_800F7250()->fn_80061824(pos.x - 0.5f * width, pos.y, 10.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, text,
                                   0xFFFFFFFF, 1, 1, 0, 0, 0xFF000000);
    }

    int mode = lbl_8087F0A8->mDrawHateInfo;
    if (mode > 0) {
        if (mode == 1 || mode == 2) {
            for (int i = 0; i < mCharaCount; i++) {
                BattleChara_unk* entry = &mCharas[i];
                if (entry->mChara == 0 || !entry->mChara->IsActive_unk()) {
                    continue;
                }
                atn::Vector3 v = atn::RenderManager::instance_->fn_800F7258()->mPos - entry->mChara->mPos;
                if (PSVECSquareMag(&v) > 90000.0f) {
                    continue;
                }
                atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(entry->mChara->mPos);
                if (screen.z < 0.0f || 1.0f < screen.z) {
                    continue;
                }
                int count = 0;
                for (int j = 0; j < mCharaCount; j++) {
                    CharaTask* other = mCharas[j].mChara;
                    if (other != 0 && other->IsActive_unk() && !other->fn_8016F3D0(entry->mChara, 1)) {
                        count++;
                    }
                }
                float x = 50.0f + screen.x;
                float y = (screen.y - 50.0f) - 12.0f * (float)count;
                char text[256];
                wchar_t wtext[256];
                if (mode == 1) {
                    sprintf(text, "Timer:%d/%d", entry->m6DC, entry->m6E0);
                    fn_8006F2F0(lbl_8087EEC8, wtext, text, 256);
                    fn_800F7250()->fn_80061824(60.0f + x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, wtext, 0xAFFFFFFF, 1,
                                               1, 0, 0, 0xFF000000);
                    y += 12.0f;
                    for (int j = 0; j < mCharaCount; j++) {
                        CharaTask* other = mCharas[j].mChara;
                        if (other == 0 || !other->IsActive_unk() || other->fn_8016F3D0(entry->mChara, 1)) {
                            continue;
                        }
                        unsigned int color = 0xAFFFFFFF;
                        if (entry->mChara->mC58.mC4 == mCharas[j].mChara) {
                            color = 0xAFAFFFAF;
                        }
                        swprintf(wtext, 256, L"%d", fn_80103484(entry->mChara, mCharas[j].mChara));
                        fn_800F7250()->fn_80061824(x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, wtext, color, 1, 1, 0,
                                                   0, 0xFF000000);
                        sprintf(text, "[%d: %s]", mCharas[j].mChara->m58, mCharas[j].mChara->m60->m0C);
                        fn_8006F2F0(lbl_8087EEC8, wtext, text, 256);
                        fn_800F7250()->fn_80061824(30.0f + x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, wtext, color, 1,
                                                   1, 0, 0, 0xFF000000);
                        y += 12.0f;
                    }
                } else if (mode == 2) {
                    for (int j = 0; j < mCharaCount; j++) {
                        CharaTask* other = mCharas[j].mChara;
                        if (other == 0 || !other->IsActive_unk() || other->fn_8016F3D0(entry->mChara, 1)) {
                            continue;
                        }
                        unsigned int color = 0xAFFFFFFF;
                        if (mCharas[j].mChara->mC58.mC4 == entry->mChara) {
                            color = 0xAFFFAFAF;
                        }
                        swprintf(wtext, 256, L"%d", fn_80103484(mCharas[j].mChara, entry->mChara));
                        fn_800F7250()->fn_80061824(x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, wtext, color, 1, 1, 0,
                                                   0, 0xFF000000);
                        sprintf(text, "[%d: %s]", mCharas[j].mChara->m58, mCharas[j].mChara->m60->m0C);
                        fn_8006F2F0(lbl_8087EEC8, wtext, text, 256);
                        fn_800F7250()->fn_80061824(30.0f + x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, wtext, color, 1,
                                                   1, 0, 0, 0xFF000000);
                        y += 12.0f;
                    }
                }
            }
        } else if (mode == 3) {
            for (int i = 0; i < mCharaCount; i++) {
                CharaTask* chara = (i < 0 || mCharaCount <= i) ? 0 : mCharas[i].mChara;
                if (chara == 0 || chara->IsFlag_unk(4) || chara->mType != 3) {
                    continue;
                }
                atn::Vector3 v = atn::RenderManager::instance_->fn_800F7258()->mPos - chara->mPos;
                if (PSVECSquareMag(&v) > 90000.0f) {
                    continue;
                }
                atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(chara->mPos);
                if (screen.z < 0.0f || 1.0f < screen.z) {
                    continue;
                }
                int count = 0;
                for (int j = 0; j < mCharaCount; j++) {
                    CharaTask* other = (j < 0 || mCharaCount <= j) ? 0 : mCharas[j].mChara;
                    if (other != 0 && !other->IsFlag_unk(4) && (other->m54C & 0x2000) != 0x2000 &&
                        other->fn_8016F3D0(chara, 1)) {
                        count++;
                    }
                }
                float x = 50.0f + screen.x;
                float y = (screen.y - 50.0f) - 12.0f * (float)count;
                CharaTask* hated = GetHated_unk(chara);
                for (int j = 0; j < mCharaCount; j++) {
                    CharaTask* other = (j < 0 || mCharaCount <= j) ? 0 : mCharas[j].mChara;
                    if (other == 0 || other->IsFlag_unk(4) || (other->m54C & 0x2000) == 0x2000 ||
                        !other->fn_8016F3D0(chara, 1)) {
                        continue;
                    }
                    unsigned int color = hated == other ? 0xAFFFAFAF : 0xAFFFFFFF;
                    wchar_t wtext[256];
                    swprintf(wtext, 256, L"%d", m391E0[i].m[j]);
                    fn_800F7250()->fn_80061824(x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, wtext, color, 1, 1, 0, 0,
                                               0xFF000000);
                    char text[256];
                    sprintf(text, "[%d: %s]", other->m58, other->m60->m0C);
                    fn_8006F2F0(lbl_8087EEC8, wtext, text, 256);
                    fn_800F7250()->fn_80061824(60.0f + x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, wtext, color, 1, 1,
                                               0, 0, 0xFF000000);
                    y += 12.0f;
                }
            }
        }
    }

    if (lbl_8087F0A8->mDrawHateAttInfo != 0) {
        PlayerTask* head = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
        char text[256];
        if (head != 0) {
            int range = m363BC;
            atn::Vector3 pos = head->mPos;
            fn_800F7250()->fn_80062C8C(32, 0x2288AAFF, pos.x, pos.y, pos.z, 0.0f, (float)range, 20.0f);
            atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(pos + atn::Vector3(0.0f, 10.0f, 0.0f));
            if (m363BC <= 300) {
                sprintf(text, "Range: %d", m363BC);
            } else {
                sprintf(text, "Range: Infinity");
            }
            fn_800F7250()->fn_800616C0(15.0f + screen.x, screen.y, 0.0f, 14.0f, 14.0f, text, 0xFFFFFFFF, 0.0f, 1, 1,
                                       0);
        }
        for (int i = 0; i < mCharaCount; i++) {
            CharaTask* chara = mCharas[i].mChara;
            if (chara == 0 || !(chara->mStatus.m430 > 0)) {
                continue;
            }
            atn::Vector3 screen =
                atn::RenderManager::instance_->GetScreenPosition(chara->mPos + atn::Vector3(0.0f, 10.0f, 0.0f));
            if (screen.z < 0.0f || 1.0f < screen.z) {
                continue;
            }
            sprintf(text, "HateAtt: Lv%d", mCharas[i].mChara->mStatus.m430);
            fn_800F7250()->fn_800616C0(15.0f + screen.x, screen.y, 0.0f, 14.0f, 14.0f, text, 0xFFFFFFFF, 0.0f, 1, 1,
                                       0);
        }
    }

    if (mShots.mEnabled != 0) {
        for (BattleShots_unk::iterator shot = mShots.begin(); shot != mShots.end(); ++shot) {
            if (shot->mData == 0) {
                continue;
            }
            float fade = shot->m24 < 45 ? 1.0f : (float)(60 - shot->m24) / 15.0f;
            unsigned int alpha = (unsigned int)(255.0f * fade) << 24;
            if (shot->mKind == 0) {
                unsigned int color = alpha | 0xFFFF00;
                HitInfo_unk* data = shot->mData;
                float angle = *(float*)&data->m50[0];
                atn::Vector3 top = shot->mPos;
                atn::Vector3 bottom = shot->mPos;
                top.y = top.y + shot->mRadius;
                bottom.y = bottom.y - shot->mRadius;
                fn_800F7250()->fn_80063484(16, color, top.x, top.y, top.z, shot->mDir.y, 0.0f, shot->mRadius, angle);
                fn_800F7250()->fn_80063484(16, color, bottom.x, bottom.y, bottom.z, shot->mDir.y, 0.0f,
                                           shot->mRadius, angle);
                fn_800F7250()->fn_80063764(top, bottom, 0.0f, color);
                atn::Vector3 v(0.0f, 0.0f, shot->mRadius);
                atn::Matrix3x4 m;
                PSMTXRotRad(m.m, 'y', shot->mDir.y - 0.5f * angle);
                PSMTXMultVec(m.m, &v, &v);
                fn_800F7250()->fn_80063764(top + v, bottom + v, 0.0f, color);
                PSMTXRotRad(m.m, 'y', angle);
                PSMTXMultVec(m.m, &v, &v);
                fn_800F7250()->fn_80063764(top + v, bottom + v, 0.0f, color);
                float width = *(float*)&data->m50[4];
                if (width > 0.0f && angle > 0.0f && shot->mRadius * (float)sin(0.5f * angle) > width) {
                    float inner = width / (float)sin(0.5f * angle);
                    float side = (float)asin(width / shot->mRadius);
                    atn::Vector3 a(0.0f, 0.0f, inner);
                    PSMTXRotRad(m.m, 'y', shot->mDir.y - 0.5f * angle);
                    PSMTXMultVec(m.m, &a, &a);
                    atn::Vector3 b(0.0f, 0.0f, shot->mRadius);
                    PSMTXRotRad(m.m, 'y', shot->mDir.y - side);
                    PSMTXMultVec(m.m, &b, &b);
                    fn_800F7250()->fn_80063764(top + a, top + b, 0.0f, color);
                    fn_800F7250()->fn_80063764(bottom + a, bottom + b, 0.0f, color);
                    fn_800F7250()->fn_80063764(top + a, bottom + a, 0.0f, color);
                    fn_800F7250()->fn_80063764(top + b, bottom + b, 0.0f, color);
                    PSMTXRotRad(m.m, 'y', angle);
                    PSMTXMultVec(m.m, &a, &a);
                    PSMTXRotRad(m.m, 'y', 2.0f * side);
                    PSMTXMultVec(m.m, &b, &b);
                    fn_800F7250()->fn_80063764(top + a, top + b, 0.0f, color);
                    fn_800F7250()->fn_80063764(bottom + a, bottom + b, 0.0f, color);
                    fn_800F7250()->fn_80063764(top + a, bottom + a, 0.0f, color);
                    fn_800F7250()->fn_80063764(top + b, bottom + b, 0.0f, color);
                }
            } else if (shot->mKind == 1) {
                fn_800F7250()->fn_80063D3C(shot->mPos, shot->mRadius, 0.0f, alpha | 0xFFFF);
            }
            atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(shot->mPos);
            if (0.0f <= screen.z && screen.z <= 1.0f) {
                wchar_t wtext[256];
                fn_800DD3FC(wtext, L"%d: %s", shot->mData->m04, shot->mData->m08);
                fn_800F7250()->fn_80061824(screen.x, screen.y, 0.0f, 14.0f, 14.0f, 0.0f, 0.0f, 0.0f, wtext,
                                           0xFFFFFFFF, 1, 1, 0, 0, 0xFF000000);
            }
        }
    }

    for (unsigned int i = 0; i < 8; i++) {
        if (mSlots[i].mUsed != 0 && mSlots[i].mChara != 0) {
            mSlots[i].mModel.GetModel_unk()->fn_8008CD60();
        }
    }

    if (!m3E2F4->IsFlag_unk(4) && lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 8) {
        m3E2F4->FreezeTask();
        m3E2F4->mFrame = 0.0f;
    }
    if (m3E2F4->mFrame >= m3E2F4->mData.mEndFrame) {
        m3E2F4->FreezeTask();
        m3E2F4->mFrame = 0.0f;
    }
}

// DRAFT: written for the weak copies; the drawing of the target line is incomplete
// NONMATCHING: draft - draws the target line of `chara` (base and head models, the line through the
// clipped segment and the appearance animation)
void BattleTask::fn_800F72F4(BattleCharaTask* chara, int arg) {
    if (chara->fn_800EE180() == 0) {
        return;
    }
    CharaC58_unk* info = fn_800F530C(chara);
    CharaTask* target = info->mC4;
    if (target == 0 || target->IsPartyMember_unk()) {
        return;
    }
    int index = fn_80102890(chara);
    fn_80102890(target);
    BattleMarkers_unk* markers = &mMarkers;
    BattleMarker_unk* marker = markers->fn_800F8538(index);
    marker->mFlags |= 1;
    int changed = 0;
    int mode = 0;
    if (mHideLines != 0 || (target->IsPlayer() && fn_800F5300(target) != 0) || fn_800F7F90() != 0) {
        mode = 1;
    } else if (fn_800F5300(fn_8000D9E8()->GetHead_unk()) != 0) {
        mode = 3;
    } else if (arg != 0) {
        mode = 2;
    }
    if (marker->mMode != mode) {
        changed = 1;
    }
    CharaTask* last = (CharaTask*)info->mC8;
    if (last != target && last != 0) {
        changed = 1;
    }
    if (changed) {
        fn_800F7F98(fn_800F7FA0(), 1);
        fn_80239DAC(fn_800F7FA0(), (unsigned int)marker, 0, 0);
        fn_800F7F98(fn_800F7FA0(), 0);
        marker->m70 = 12;
        marker->mFlags &= ~0x10;
    } else if (chara->vfE4()) {
        if ((marker->mFlags & 0x10) != 0x10) {
            fn_800F7F98(fn_800F7FA0(), 1);
            fn_800F7FA8(fn_800F7FA0(), marker, 0);
            fn_8023A680(fn_800F7FA0(), (EffectRes_unk*)&mAppearLine.mBase, -1, 5, (int)marker, 0, 0, 0, 0, -1, 1, 1.0f);
            fn_8023A680(fn_800F7FA0(), (EffectRes_unk*)&mAppearLine.mHead, -1, 5, (int)&marker->mMtx2, 0, 0, 0, 0, -1, 1, 2.0f);
            if (((Player_unk*)chara)->GetM146C_unk() == 0x12 || ((Player_unk*)chara)->GetM146C_unk() == 0x11) {
                fn_8023A680(fn_800F7FA0(), (EffectRes_unk*)&mAppearLine.mBase, -1, 5, (int)marker, 0, 0, 0, 0, -1, 1, 1.0f);
            }
            fn_800F7F98(fn_800F7FA0(), 0);
        }
        marker->mFlags |= 0x10;
    } else {
        marker->mFlags &= ~0x10;
    }
    atn::Vector3 from(*fn_800F7FBC(chara));
    atn::Vector3 to(*fn_800F7FBC((BattleCharaTask*)target));
    atn::Vector3 dir = (to - from).GetNormalize();
    from += dir * fn_800F7FF8(chara);
    to -= dir * fn_800F7FF8((BattleCharaTask*)target) * 0.3f;
    dir = to - from;
    float scale = (dir.Length_unk() - 4.0f) / 50.0f;
    dir.Normalize_unk();
    atn::Vector3 rot = dir.ToRotation_unk();
    atn::Color color;
    switch (mode) {
    case 1:
        if (chara->GetStatus_unk()->fn_80134270()) {
            color = fn_800F8014(fn_800F8098(fn_800F52F8())->mLineColorMagic);
        } else if (chara->GetStatus_unk()->fn_80134250()) {
            color = fn_800F8014(fn_800F8098(fn_800F52F8())->mLineColorArcher);
        } else {
            color = fn_800F8014(fn_800F8098(fn_800F52F8())->mLineColorSword);
        }
        if ((unsigned int)(((Player_unk*)chara)->GetM146C_unk() - 0x11) <= 1) {
            color = fn_800F8014(fn_800F8098(fn_800F52F8())->mLineColorMagic);
        }
        break;
    case 2:
        color = fn_800F8014(fn_800F8098(fn_800F52F8())->mLineColorMagic);
        break;
    default:
        color = fn_800F8014(fn_800F8098(fn_800F52F8())->mLineColorNormal);
        break;
    }
    marker->mMode = mode;
    float size = 1.0f;
    switch (((Player_unk*)chara)->GetM146C_unk()) {
    case 0x11:
        size = 1.5f;
        break;
    case 0x12:
    case 0x3B:
        size = 2.0f;
        break;
    }
    marker->m70--;
    if (mHideLines == 0 && marker->m70 >= 0) {
        float t = (float)(marker->m70 - 6) / 6.0f;
        size = 0.0f > t ? 0.0f : (float)(marker->m70 - 6) / 6.0f;
        to = atn::Lerp_unk(to, marker->m60, size);
        dir = to - from;
        scale = (dir.Length_unk() - 4.0f) / 50.0f;
        scale = fn_800F8524(&scale, &marker->m6C, size);
        dir.Normalize_unk();
        rot = dir.ToRotation_unk();
        LineSeg_unk seg;
        LineSeg_unk clipped;
        seg.mStart = from;
        seg.mEnd = to;
        float len;
        if (fn_80050420(fn_800F80A0(fn_8008B964()->fn_800F7258()), &seg.mStart, &clipped.mStart)) {
            len = ((clipped.mEnd - clipped.mStart).Length_unk() - 4.0f) / 50.0f;
        } else {
            len = 0.001f;
        }
        scale = len;
        marker->mMtx = fn_800F80A8(&clipped.mStart);
        fn_800F80B8(marker->mMtx.m, &rot);
        fn_800F8290(marker->mMtx.m, 4.0f, 4.0f, scale);
        marker->mMtx2 = fn_800F80A8(&(to - dir * 4.0f));
        fn_800F80B8(marker->mMtx2.m, &rot);
        atn::DrawTexVertex_unk verts[7];
        fn_800F831C(&verts[0], &from, color.ToU32(), &atn::Vec2_unk(0.0f, 1.0f));
        int count = marker->m70 < 6 ? marker->m70 : 6;
        for (int i = 0; i < count; i++) {
            atn::DrawTexVertex_unk* v = &verts[i + 1];
            float f = (float)(count - i - 1) / 6.0f;
            v->mPos = atn::Lerp_unk(to, marker->m60, f);
            v->mPos += (v->mPos - from).GetNormalize() * (4.0f * (1.0f - f * f));
            verts[i].mColor = color.ToU32();
            v->mUV = fn_800F84D8(atn::Vec2_unk(1.0f, 0.0f), atn::Vec2_unk(0.0f, 0.0f), (float)i / (float)count);
        }
        fn_800F7250()->fn_80064F00(verts, count + 1, 0xA0, 0, (int)&mLineTex, 0.0f);
        if (marker->m70 == 8) {
            if (info->mC4->IsPlayer()) {
                if (fn_800F5300(info->mC4)) {
                    fn_800C31F4(lbl_80735DD0[0], 0, -1, 2.0f);
                } else {
                    fn_800C31F4(lbl_80735DD0[0], 0, -1, 1.0f);
                }
            } else {
                fn_800C31F4(lbl_80735DD0[1], 0, -1, 1.0f);
            }
        }
        if (fn_800F7F90() != 0 && marker->m70 == 1) {
            fn_801065F4(target, fn_800F84C8(target), (const ColorF_unk*)&color);
        }
    } else {
        float last = scale;
        LineSeg_unk seg;
        LineSeg_unk clipped;
        seg.mStart = from;
        seg.mEnd = to;
        if (fn_80050420(fn_800F80A0(fn_8008B964()->fn_800F7258()), &seg.mStart, &clipped.mStart)) {
            scale = ((clipped.mEnd - clipped.mStart).Length_unk() - 4.0f) / 50.0f;
        } else {
            scale = 0.001f;
        }
        marker->mMtx = fn_800F80A8(&clipped.mStart);
        fn_800F80B8(marker->mMtx.m, &rot);
        if (mHideLines != 0) {
            fn_800F8290(marker->mMtx.m, 3.0f, 3.0f, scale);
        } else {
            fn_800F8290(marker->mMtx.m, size, size, scale);
        }
        marker->mMtx2 = fn_800F80A8(&(to - dir * 4.0f));
        fn_800F80B8(marker->mMtx2.m, &rot);
        marker->m60 = to;
        marker->m6C = last;
    }
    if (fn_80239D58(fn_800F7FA0(), marker, 0) == 0) {
        atn::EffFile* res = (mode == 1 || mode == 3) ? &mOwnLine.mBase : &mEnemyLine.mBase;
        fn_800F7F98(fn_800F7FA0(), 1);
        fn_800F84D0(fn_800F7FA0(), 5);
        fn_800F7FA8(fn_800F7FA0(), marker, 0);
        fn_8023A680(fn_800F7FA0(), (EffectRes_unk*)&res[0], -1, 5, (int)marker, 0, 0, (const EffectColor_unk*)&color, 0, -1, 1, 1.0f);
        fn_8023A680(fn_800F7FA0(), (EffectRes_unk*)&res[1], -1, 5, (int)&marker->mMtx2, 0, 0, (const EffectColor_unk*)&color, 0, -1, 1,
                    1.0f);
        fn_800F84D0(fn_800F7FA0(), 0);
        fn_800F7F98(fn_800F7FA0(), 0);
    }
}

// inferred: next bullet id (wraps after 99999); C name, called by many files as extern "C"
extern "C" int fn_800F8548(BattleTask* mgr) {
    int id = mgr->mBulletId;
    mgr->mBulletId = id + 1;
    if (mgr->mBulletId > 100000) {
        mgr->mBulletId = 0;
    }
    return id;
}

extern "C" const char* fn_800EFF04(int id);       // inferred: firing sound of row `id`
extern "C" atn::EffFile* fn_800EFD04(int id);     // inferred: bullet effect of row `id`
extern "C" bool fn_8021A888(HitInfo_unk* data);   // inferred: db/ParseUtil_unk (takes the record as BulletParam_unk)
extern "C" float fn_8010B250(BattleTask* mgr, int level, BattleCalcParam_unk* param);

// inferred: fires the `data->m48` bullets of `data` from `pos` along `dir` (homing bullets with `homing`), spread
// randomly by the aim instability of `owner` (m12CC, raised by 0.33 up to 1 after the shot); plays the firing sound
// (louder for the player with fn_8021A888). C name, called by enemies as extern "C"
// NONMATCHING: 99.3% - stack slots of the rotation temporaries (allocated the other way), of the rotation copy
extern "C" int fn_800F8574(BattleTask* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                           const atn::Vector3* dir, const HomingParam_unk* homing, unsigned int flags, int arg,
                           float a, float b) {
    if (data == 0) {
        return 0;
    }
    const char* sound = fn_800EFF04(data->mB0);
    if (sound != 0) {
        float volume = 1.0f;
        if (owner->mType == 0 && fn_8021A888(data)) {
            volume = 1.75f;
        }
        fn_800C344C(sound, pos, 0, -1, volume);
    }
    atn::Vector3 rot = dir->ToRotation_unk();
    atn::Matrix3x4 m;
    atn::Matrix3x4* p = &m;
    p->SetIdentity();
    if (0.0f != rot.z) {
        atn::Matrix3x4::RotateAxis_unk(*p, 'z', rot.z);
    }
    if (0.0f != rot.y) {
        atn::Matrix3x4::RotateAxis_unk(*p, 'y', rot.y);
    }
    if (0.0f != rot.x) {
        atn::Matrix3x4::RotateAxis_unk(*p, 'x', rot.x);
    }
    for (int i = 0; i < data->m48; i++) {
        atn::Vector3 v(0.0f, 0.0f, 1.0f);
        float spread = 0.017453292f * (owner->mStatus.mParamB.m2C * (1.0f - owner->m12CC));
        Mtx ry;
        PSMTXRotRad(ry, 'y', spread * ((float)(rand() % 1001) / 1000.0f));
        PSMTXMultVec(ry, &v, &v);
        Mtx rz;
        PSMTXRotRad(rz, 'z', 6.2831855f * ((float)(rand() % 1001) / 1000.0f));
        PSMTXMultVec(rz, &v, &v);
        PSMTXMultVec(m.m, &v, &v);
        BulletShot_unk shot;
        shot.m00 = 0.1f;
        shot.m04 = 0;
        shot.m08 = 0;
        shot.m10 = 0;
        shot.m14 = 0.0f;
        shot.m18 = 1.0f;
        shot.m00 = fn_8010B250(mgr, data->m3C, 0);
        shot.m04 = (int)fn_800EFD04(data->mB0);
        shot.m08 = (int)fn_800EFDA0(data->mB0);
        shot.m0C = data;
        shot.m10 = arg;
        shot.m14 = a;
        shot.m18 = b;
        if (homing == 0) {
            Bullet* bullet = lbl_8087F048->fn_801010A0();
            if (bullet == 0) {
                break;
            }
            bullet->fn_8010EE78(&shot, pos, &v, owner, -1, flags, -1.0f);
        } else {
            HomingBullet* bullet = lbl_8087F048->fn_801010E8();
            if (bullet == 0) {
                break;
            }
            bullet->vf28(&shot, pos, &v, owner, homing, flags);
        }
    }
    float t = owner->m12CC + 0.33f;
    if (t > 1.0f) {
        t = 1.0f;
    }
    owner->m12CC = t;
    return 1;
}

// inferred: a character reached by a melee attack, sorted by mScore (fn_800F8C6C); names invented
struct MeleeTarget_unk {
    CharaTask* mChara;
    int mPart;    // inferred: hit sphere reached (fn_800FBA9C)
    float mScore; // inferred: distance weighted by the angle to the facing of the attacker
};
// inferred: std::sort predicate of fn_800F8C6C
struct MeleeTargetLess_unk {
    bool operator()(const MeleeTarget_unk& a, const MeleeTarget_unk& b) const { return a.mScore < b.mScore; }
};

template <class T>
inline void MeleeSwap_unk(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

// inferred: MSL __sort3 (median of three of std::sort, 0x800FA9F4; iterators by reference)
// NONMATCHING: 76.8% - order of the comparisons and of the swaps of the 12-byte elements
template <class T, class Compare>
void MeleeMedian3_unk(T*& a, T*& b, T*& c, Compare& comp) {
    bool ac = !comp(*c, *a);
    bool bc = !comp(*b, *c);
    if (ac && bc) {
        return;
    }
    if (!ac && !bc) {
        MeleeSwap_unk(*a, *b);
        return;
    }
    if (comp(*b, *a)) {
        MeleeSwap_unk(*a, *b);
    }
    if (ac) {
        MeleeSwap_unk(*b, *c);
    } else {
        MeleeSwap_unk(*a, *c);
    }
}

// inferred: MSL __sort (std::sort, 0x800FA4FC): selection sort up to 20 elements, else quicksort around the
// median of three entries picked with a rotating offset (as Sort_unk of atn/RenderSystem_unk.cpp)
// NONMATCHING: 64.5% - the original keeps the iterators in memory (references) and inlines the selection sort
// differently
template <class T, class Compare>
void MeleeSort_unk(T*& first, T*& last, Compare& comp) {
    static int sOffset = -4; // inferred: lbl_8087D978
    while (true) {
        int n = last - first;
        if (n <= 1) {
            return;
        }
        if (n <= 20) {
            if (first != last) {
                for (T* it = first; it != last - 1; it++) {
                    T* min = it;
                    if (it != last) {
                        for (T* j = it + 1; j != last; j++) {
                            if (comp(*j, *min)) {
                                min = j;
                            }
                        }
                    }
                    if (min != it) {
                        MeleeSwap_unk(*min, *it);
                    }
                }
            }
            return;
        }
        T* a = first + (n / 4 + sOffset % 5);
        if (++sOffset >= 5) {
            sOffset = -4;
        }
        T* b = first + (n * 3 / 4 + sOffset % 5);
        if (++sOffset >= 5) {
            sOffset = -4;
        }
        T* pivot = last - 1;
        MeleeMedian3_unk(a, b, pivot, comp);
        T* i = first;
        T* j = pivot;
        while (comp(*i, *pivot)) {
            i++;
        }
        while (i != --j && !comp(*j, *pivot)) {
        }
        if (i < j) {
            MeleeSwap_unk(*i, *j);
            i++;
            while (true) {
                while (comp(*i, *pivot)) {
                    i++;
                }
                while (!comp(*--j, *pivot)) {
                }
                if (i >= j) {
                    break;
                }
                MeleeSwap_unk(*i, *j);
                i++;
            }
        }
        if (i == first) {
            T* back = last - 1;
            MeleeSwap_unk(*i, *pivot);
            i++;
            if (!comp(*first, *back)) {
                while (i != last && !comp(*first, *i)) {
                    i++;
                }
                if (i < back) {
                    MeleeSwap_unk(*i, *back);
                }
            }
            j = back;
            if (i < j) {
                while (true) {
                    while (!comp(*first, *i)) {
                        i++;
                    }
                    while (comp(*first, *--j)) {
                    }
                    if (i >= j) {
                        break;
                    }
                    MeleeSwap_unk(*i, *j);
                    i++;
                }
            }
            first = i;
        } else if (i - first < last - i) {
            MeleeSort_unk(first, i, comp);
            first = i;
        } else {
            MeleeSort_unk(i, last, comp);
            last = i;
        }
    }
}

extern "C" int fn_800FBA9C(BattleTask* mgr, CharaTask* attacker, CharaTask* chara, HitInfo_unk* data, int id,
                           float angle);
extern "C" void fn_800FC410(BattleTask* mgr, CharaTask* attacker, CharaTask* chara, int index, HitInfo_unk* hit, int id,
                            int frames);
extern "C" int fn_800FAB80(BattleTask* mgr, CharaTask* attacker, HitInfo_unk* data, int id, const atn::Vector3* pos,
                           const atn::Vector3* rot, int any, int time, int, int type, float f1, float f2);
extern "C" float PSVECSquareMag(const atn::Vector3* v);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" double cos(double x);
extern "C" double sqrt(double x);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);

// inferred: the counter statuses of the attacker; names invented
inline bool HasStatus_unk(CharaTask* chara, int id) {
    return chara->mStatus.fn_80133EE8(id) != 0 || chara->mStatus.fn_80134134(id) != 0;
}
// inferred: plays the effect and the sound of row `id` at `pos` (color 1, scale `scale`)
inline void PlayRowEffect_unk(int id, atn::Model* model, const atn::Vector3& pos, const atn::Vector3& rot,
                              const atn::Vector3& soundPos, float scale) {
    EffectColor_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    atn::EfpFile* efp = fn_800EFDA0(id);
    if (efp != 0) {
        fn_80232B7C(0, 0);
        fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, scale);
    }
    const char* sound = fn_800EFE3C(id);
    if (sound != 0) {
        fn_800C344C(sound, &soundPos, 0, -1, 1.0f);
    }
}

// NONMATCHING: 89% - stack slots of the locals (the frame and the inlined push_back are the same), the original
// clears x/y of the vectors of the debug record first (a constructor, as in fn_800FAB80), and registers
// inferred: melee attack `data` of `chara` (arc turned by `angle` from its facing): every player and enemy reached
// (fn_800FBA9C) is sorted by its distance weighted by the angle (3 - 2 cos) and hit in that order (fn_800FC410), at
// most `max` of them (all when negative), writing the attacker and the damage of each hit in `out` (up to
// `outCount`). For each hit the counter statuses of the attacker fire their bullets (0x28, 0x2F, 0x32, 0x3E), play
// effects (0x34) or heal (7-9: the attacker, and the players with status 8). Then the gimmicks inside the arc get
// a hit message, the gauge of a player rises and the attack is recorded in the debug ring. Returns the number of
// characters hit. C name, called by the enemies as extern "C"
extern "C" int fn_800F8C6C(BattleTask* mgr, HitResult_unk* out, int outCount, CharaTask* chara, HitInfo_unk* data,
                           int id, float angle, int time, int max) {
    if (data == 0) {
        return 0;
    }
    std::vector<MeleeTarget_unk> targets;
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    PSMTXRotRad(m.m, 'y', angle + chara->mRot.y);
    PSMTXMultVec(m.m, &front, &front);
    for (PlayerTask* c = lbl_8087F8A0->mHead; c != 0; c = c->mNext) {
        int part = fn_800FBA9C(mgr, chara, c, data, id, angle);
        if (part < 0) {
            continue;
        }
        CharaTask::Item14_unk* node = c->fn_80148B0C(part);
        atn::Vector3 d = chara->mPos - node->m04.mCenter;
        float score = PSVECMag(&d) - node->m04.mRadius;
        d.y = 0.0f;
        if (d.IsZero_unk()) {
            d.y = 1.0f;
        }
        PSVECNormalize(&d, &d);
        score *= 3.0f - 2.0f * PSVECDotProduct(&d, &front);
        MeleeTarget_unk t;
        t.mChara = c;
        t.mPart = part;
        t.mScore = score;
        targets.push_back(t);
    }
    for (EnemyTask* c = EnemyManager::sInstance->mHead; c != 0; c = c->mNext) {
        int part = fn_800FBA9C(mgr, chara, c, data, id, angle);
        if (part < 0) {
            continue;
        }
        CharaTask::Item14_unk* node = c->fn_80148B0C(part);
        atn::Vector3 d = chara->mPos - node->m04.mCenter;
        float score = PSVECMag(&d) - node->m04.mRadius;
        d.y = 0.0f;
        if (d.IsZero_unk()) {
            d.y = 1.0f;
        }
        PSVECNormalize(&d, &d);
        score *= 3.0f - 2.0f * PSVECDotProduct(&d, &front);
        MeleeTarget_unk t;
        t.mChara = c;
        t.mPart = part;
        t.mScore = score;
        targets.push_back(t);
    }
    {
        MeleeTarget_unk* first = targets.begin();
        MeleeTarget_unk* last = targets.begin() + targets.size();
        MeleeTargetLess_unk comp;
        MeleeSort_unk(first, last, comp);
    }
    int count = 0;
    for (unsigned int i = 0; i < targets.size(); i++, count++) {
        if (max >= 0 && count >= max) {
            break;
        }
        CharaTask* target = targets[i].mChara;
        fn_800FC410(mgr, chara, target, targets[i].mPart, data, id, time);
        int result = mgr->m38A5C.m64;
        if (result == 1 || result == 4 || result == 5) {
            if (targets[i].mChara->fn_80148B0C(targets[i].mPart) != 0) {
                atn::Vector3 hitPos = mgr->m38A5C.m1C;
                atn::Vector3 dir(0.0f, 0.0f, 1.0f);
                atn::Matrix3x4 rm;
                PSMTXRotRad(rm.m, 'y', targets[i].mChara->mRot.y);
                PSMTXMultVec(rm.m, &dir, &dir);
                atn::Vector3 partPos = targets[i].mChara->fn_80148B0C(targets[i].mPart)->m04.mCenter + dir;
                if (HasStatus_unk(chara, 0x28)) {
                    int bullet = fn_800F8548(lbl_8087F048);
                    fn_800FAB80(mgr, chara, fn_80219E6C(0x2329), bullet, &hitPos, &chara->mRot, 0, 30, -1, -1, 0.0f,
                                1.0f);
                }
                if (HasStatus_unk(chara, 0x2F)) {
                    int bullet = fn_800F8548(lbl_8087F048);
                    fn_800FAB80(mgr, chara, fn_80219E6C(0x232A), bullet, &partPos, &chara->mRot, 0, 30, -1, -1, 0.0f,
                                1.0f);
                }
                if (HasStatus_unk(chara, 0x32)) {
                    CharaTask* t = targets[i].mChara;
                    if (t->m50 == 0x4A31F && t->fn_80148B0C(2) != 0) {
                        int bullet = fn_800F8548(lbl_8087F048);
                        atn::Vector3 front2(0.0f, 0.0f, 1.0f);
                        atn::Matrix3x4 m2;
                        PSMTXRotRad(m2.m, 'y', t->mRot.y);
                        PSMTXMultVec(m2.m, &front2, &front2);
                        atn::Vector3 up(0.0f, t->fn_80148B0C(2)->m04.mRadius, 0.0f);
                        atn::Vector3 base = t->fn_80148B0C(2)->m04.mCenter - up;
                        atn::Vector3 pos = base + front2;
                        fn_800FAB80(mgr, chara, fn_80219E6C(0x232B), bullet, &pos, &chara->mRot, 0, 30, -1, -1, 0.0f,
                                    1.0f);
                    } else {
                        int bullet = fn_800F8548(lbl_8087F048);
                        atn::Vector3 front2(0.0f, 0.0f, 1.0f);
                        atn::Matrix3x4 m2;
                        PSMTXRotRad(m2.m, 'y', t->mRot.y);
                        PSMTXMultVec(m2.m, &front2, &front2);
                        atn::Vector3 pos = t->mPos + front2;
                        fn_800FAB80(mgr, chara, fn_80219E6C(0x232B), bullet, &pos, &chara->mRot, 0, 30, -1, -1, 0.0f,
                                    1.0f);
                    }
                }
                if (chara->mStatus.fn_80133EE8(0x34) != 0) {
                    PlayRowEffect_unk(0x232D, 0, partPos, chara->mRot, partPos, 1.0f);
                }
                if (chara->mStatus.fn_80133EE8(7) != 0 || chara->mStatus.fn_80133EE8(8) != 0 ||
                    chara->mStatus.fn_80133EE8(9) != 0) {
                    atn::Vector3 d = chara->mPos - targets[i].mChara->mPos;
                    float dist = PSVECMag(&d);
                    PlayRowEffect_unk(0x232F, 0, chara->mPos, chara->mRot, partPos, dist / 30.0f);
                    atn::FixedArray_unk<CharaTask*, 16> healed;
                    healed.push_back(chara);
                    if (chara->mStatus.fn_80133EE8(8) != 0) {
                        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
                            if (p->CanAct_unk() && (p->m54C & 0x2000) != 0x2000) {
                                healed.push_back(p);
                            }
                        }
                    }
                    for (unsigned int k = 0; k < healed.size(); k++) {
                        CharaTask* h = healed[k];
                        PlayRowEffect_unk(0x2330, h->GetModel_unk(), lbl_807C7030, lbl_807C7030, h->mPos, 1.0f);
                    }
                }
                CharaStatusEntry_unk* entry = chara->mStatus.fn_80133EE8(0x3E);
                if (entry != 0 && fn_80219E6C(entry->mValue) != 0) {
                    int bullet = fn_800F8548(lbl_8087F048);
                    fn_800FAB80(mgr, chara, fn_80219E6C(entry->mValue), bullet, &hitPos, &chara->mRot, 0, 30, -1, -1,
                                0.0f, 1.0f);
                }
            }
        }
        if (count < outCount) {
            out[count].mChara = chara;
            out[count].mFlags = mgr->m38A5C.m94;
        }
    }
    int power = data->m14;
    if (chara != 0) {
        power += (int)(data->m28 * chara->mStatus.mParamB.m00);
    }
    float range = data->m40 + chara->mStatus.mParamB.m28;
    int level = data->m3C + chara->mStatus.mParamB.m30;
    if (power > 0) {
        for (GimmickCommon* gimmick = GimmickManager::instance_->mHead; gimmick != 0; gimmick = gimmick->m5C) {
            atn::Vector3 d = gimmick->vf54() - chara->mPos;
            float dist = PSVECMag(&d) - gimmick->vf58();
            atn::Vector3 flat(d.x, 0.0f, d.z);
            float sq = PSVECSquareMag(&flat);
            if (sq > 0.0f) {
                PSVECNormalize(&flat, &flat);
            }
            atn::Vector3 dir(0.0f, 0.0f, 1.0f);
            atn::Matrix3x4 rm;
            PSMTXRotRad(rm.m, 'y', angle + chara->mRot.y);
            PSMTXMultVec(rm.m, &dir, &dir);
            float dot = PSVECDotProduct(&flat, &dir);
            if (!(dist <= range)) {
                continue;
            }
            if (!(dot >= (float)cos(0.5f * *(float*)&data->m50[0]))) {
                continue;
            }
            float height = *(float*)&data->m50[4];
            if (height > 0.0f) {
                float s = sqrt(1.0f - dot * dot);
                float r = sqrt(sq);
                if (r * s > height + gimmick->vf58()) {
                    continue;
                }
            }
            atn::Vector3 n;
            if (0.0f == d.x && 0.0f == d.y && 0.0f == d.z) {
                n = atn::Vector3(0.0f, 1.0f, 0.0f);
            } else {
                n = d;
                PSVECNormalize(&n, &n);
            }
            atn::Vector3 rot = n.ToRotation_unk(); // inferred: computed but not used
            (void)rot;
            GimmickHitMsg_unk msg;
            memset(&msg, 0, sizeof(msg));
            msg.mType = power;
            msg.mDir = d;
            msg.mSender = chara;
            switch (gimmick->m50) {
            case 1:
                if (level >= 2) {
                    gimmick->vf44(&msg);
                }
                break;
            case 0x10:
                if (chara->mType == 0) {
                    gimmick->vf44(&msg);
                }
                break;
            }
        }
    }
    if (count > 0 && chara != 0 && chara->mType == 0) {
        float add = mgr->m38A14 * (chara->mFlag28 ? mgr->m38A38 : 1.0f);
        float zero;
        mgr->m38A04 = MAX(zero = 0.0f, MIN(mgr->m38A04 + add, mgr->m38A0C));
    }
    if (mgr->mShots.mEnabled != 0) {
        BattleShot_unk shot;
        shot.mKind = 0;
        shot.mData = data;
        shot.mPos = chara->mPos;
        shot.mDir = chara->mRot;
        shot.mDir.y += angle;
        shot.mRadius = data->m40 + chara->mStatus.mParamB.m28;
        shot.m24 = 0;
        mgr->mShots.Push_unk(shot);
    }
    if (chara != 0) {
        chara->mStatus.mEntries.mCount = 0;
    }
    return count;
}


extern "C" int fn_800FBE70(BattleTask* mgr, CharaTask* attacker, CharaTask* chara, HitInfo_unk* data, int id,
                           const atn::Vector3* pos, int any, int type);
extern "C" void fn_800FB1BC(BattleTask* mgr, CharaTask* chara, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* rot, unsigned int flags, int self, float scale);
extern "C" bool fn_8021A7A0(HitInfo_unk* data); // inferred: db/ParseUtil_unk (takes the record as BulletParam_unk)
// inferred: damage of the bullet `data` to the sphere `index` of `chara`
extern "C" void fn_800FCD24(BattleTask* mgr, CharaTask* attacker, CharaTask* chara, int index, HitInfo_unk* data, int id,
                            const atn::Vector3* pos, int time, BattleCalcParam_unk* out, float, float rate);
extern "C" void* memset(void* dst, int c, unsigned long n);

// inferred: push of the debug ring (drops the oldest record when full)
inline void BattleShots_unk::Push_unk(const BattleShot_unk& shot) {
    if (mCount >= 32) {
        mHead = (mHead + 1) & 31;
        mCount--;
    }
    mItems[(mHead + mCount) & 31] = shot;
    mCount++;
}
// inferred: the bullet kind does not hit the gimmicks (names invented)
inline bool HitsGimmicks_unk(int kind) {
    bool hits = true;
    if ((unsigned int)(kind - 0x1F7) <= 2 || (unsigned int)(kind - 0x201) <= 2) {
        hits = false;
    }
    return hits;
}
// inferred: bullets that do not break the gimmicks of type 1
inline bool IsSoftBullet_unk(const HitInfo_unk* data) {
    if (data == 0) {
        return false;
    }
    int id = data->m04;
    if (id == 0x136 || id == 0x139 || id == 0x13C || id == 0x146 || id == 0x15A || id == 0x232C) {
        return true;
    }
    return false;
}

// inferred: hit test of the bullet `data` of `attacker` at `pos`: damages every player and enemy whose hit sphere it
// reaches (fn_800FBE70, fn_800FCD24), sends a hit message to the gimmicks in range, plays the hit effects
// (fn_800FB1BC), raises the gauge for a player and records the test when the debug ring is enabled. Returns
// whether a character was hit. Called as extern "C" by game/Bullet.
// NONMATCHING: 96.9% - float registers of the scale, range and distance; the original clears x/y of the record
// vectors first (a constructor) and reaches the ring through its address; .sdata2 pool
extern "C" int fn_800FAB80(BattleTask* mgr, CharaTask* attacker, HitInfo_unk* data, int id, const atn::Vector3* pos,
                           const atn::Vector3* rot, int any, int time, int, int type, float f1, float f2) {
    fn_800E2A24(&mgr->m38A5C);
    if (data == 0) {
        return 0;
    }
    int hit = 0;
    for (PlayerTask* chara = lbl_8087F8A0->mHead; chara != 0; chara = chara->mNext) {
        int index = fn_800FBE70(mgr, attacker, chara, data, id, pos, any, type);
        if (index >= 0) {
            fn_800FCD24(mgr, attacker, chara, index, data, id, pos, time, 0, f1, f2);
            hit = 1;
        }
    }
    for (EnemyTask* chara = EnemyManager::sInstance->mHead; chara != 0; chara = chara->mNext) {
        int index = fn_800FBE70(mgr, attacker, chara, data, id, pos, any, type);
        if (index >= 0) {
            fn_800FCD24(mgr, attacker, chara, index, data, id, pos, time, 0, f1, f2);
            hit = 1;
        }
    }
    float scale = 1.0f;
    if (fn_8021A7A0(data) && attacker != 0) {
        scale = attacker->fn_8017AC24();
    }
    if (HitsGimmicks_unk(data->m04)) {
        float range = data->m58 * scale;
        int power = data->m1C;
        if (attacker != 0) {
            power += (int)(data->m2C * attacker->mStatus.mParamB.m04);
        }
        int level = data->m3C;
        for (GimmickCommon* gimmick = GimmickManager::instance_->mHead; gimmick != 0; gimmick = gimmick->m5C) {
            if ((gimmick->mFlags & 2) != 2 || (gimmick->mFlags & 0xDEAD0000) == 0xDEAD0000) {
                continue;
            }
            atn::Vector3 d = gimmick->vf54() - *pos;
            float dist = PSVECMag(&d);
            if (!(dist - gimmick->vf58() <= range)) {
                continue;
            }
            GimmickHitMsg_unk msg;
            memset(&msg, 0, sizeof(msg));
            msg.mType = power;
            msg.mDir = d;
            msg.mPos = *pos;
            *(HitInfo_unk**)msg.m2C = data; // inferred: the hit data of the bullet
            msg.mSender = attacker;
            switch (gimmick->m50) {
            case 1:
                if (level >= 2) {
                    int sub = gimmick->mId % 1000;
                    if (sub == 0x3A || sub == 0x6B || sub == 0x7B) {
                        gimmick->vf44(&msg);
                    } else if (!IsSoftBullet_unk(data)) {
                        gimmick->vf44(&msg);
                    }
                }
                break;
            case 0x1C:
                if (level >= 2) {
                    gimmick->vf44(&msg);
                }
                break;
            case 0x10:
            case 0x22:
            case 0x30:
                if (level > 0) {
                    gimmick->vf44(&msg);
                }
                break;
            }
        }
    }
    fn_800FB1BC(mgr, attacker, data, pos, rot, mgr->m38A5C.m94, 0, scale);
    if (hit != 0 && attacker != 0 && attacker->mType == 0) {
        float add = mgr->m38A1C * (attacker->mFlag28 ? mgr->m38A38 : 1.0f);
        float zero;
        mgr->m38A04 = MAX(zero = 0.0f, MIN(mgr->m38A04 + add, mgr->m38A0C));
    }
    if (mgr->mShots.mEnabled != 0) {
        BattleShot_unk shot;
        shot.mKind = 1;
        shot.mData = data;
        shot.mPos = *pos;
        shot.mDir = *rot;
        shot.mRadius = data->m58;
        shot.m24 = 0;
        mgr->mShots.Push_unk(shot);
    }
    if (attacker != 0) {
        attacker->mStatus.mEntries.mCount = 0;
    }
    return hit;
}

// inferred: event 15 of the character listeners (a hit effect was played); names invented
struct HitEvent_unk {
    int mId;          // inferred: id of the hit data
    atn::Vector3 mPos;
    atn::Vector3 mRot;
    int mSelf;        // inferred: the hit data is its own skill (see fn_800FB1BC)
};
// inferred: listeners of the characters (function-local static; see gmk/Item.cpp)
inline CharaListeners_unk* fn_8014F8F4() {
    static CharaListeners_unk sListeners;
    return &sListeners;
}
extern "C" atn::EfpFile* fn_800EFDA0(int id);
// inferred: casts a skill (SkillEffectManager, lbl_8087F9E8; as gmk/HealPoint.cpp)
extern "C" SkillEffect* fn_8059B66C(void* mgr, HitInfo_unk* skill, CharaTask* user, CharaTask* target,
                                    const atn::Vector3* pos, const atn::Vector3* rot,
                                    boost::function<void(SkillEffect*)> callback, float rate);

// inferred: effects of a hit of `chara` with `data` at `pos`: the skill effect lit by the map, its sound, the
// sub skill (the data itself when `self`, else the bullet data->m6C) unless `flags` has 0x20000000, and event
// 15 to the character listeners. Called as extern "C" by game/Bullet, game/DamageCalc_unk and others.
// NONMATCHING: 98.3% - stack slots of the sphere and the light swapped, registers of the listener loop, .sdata2 pool
extern "C" void fn_800FB1BC(BattleTask* mgr, CharaTask* chara, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* rot, unsigned int flags, int self, float scale) {
    if (data == 0) {
        return;
    }
    atn::EfpFile* efp = fn_800EFDA0(data->mB0);
    if (efp != 0) {
        MapColor_unk light = MapManager::instance_->fn_80491D50(atn::Sphere(*pos, 10.0f));
        lbl_8087F3C0->mSystem.m78 = 1;
        fn_80232B7C(0, 0);
        // inferred: the light of the map is passed as the color of the effect (same RGBA floats)
        fn_8023A8B4(lbl_8087F3C0, efp, 0, 0, *pos, lbl_807C7030, *(const EffectColor_unk*)&light, -1, -1, 1,
                    scale);
        lbl_8087F3C0->mSystem.m78 = 0;
    }
    const char* sound = fn_800EFE3C(data->mB0);
    if (sound != 0) {
        float volume = 1.0f;
        if (data->mB0 == 0x6D4) {
            volume = 0.6f;
        }
        if (lbl_8087EFE8 != 0 && (unsigned int)(data->mB0 - 0x148) <= 2) {
            lbl_8087EFE8->m34C8 = 0;
        }
        fn_800C344C(sound, pos, 0, -1, volume);
        if (lbl_8087EFE8 != 0) {
            lbl_8087EFE8->m34C8 = 1;
        }
    }
    if ((flags & 0x20000000) != 0x20000000) {
        HitInfo_unk* skill = self ? data : fn_80219E6C(data->m6C);
        if (skill != 0 && skill->m04 != 0) {
            fn_8059B66C(lbl_8087F9E8, skill, chara, 0, pos, rot, boost::function<void(SkillEffect*)>(), scale);
        }
    }
    HitEvent_unk event;
    event.mId = data->m04;
    event.mPos = *pos;
    event.mRot = *rot;
    event.mSelf = self;
    CharaListeners_unk* listeners = fn_8014F8F4();
    if (listeners->mEnabled != 0) {
        for (int i = 0; i < (int)listeners->mList.size(); i++) {
            CharaListener_unk& listener = listeners->mList[i];
            if (listener.mId == -1 || listener.mId == 15) {
                listener.mFunc(15, chara, &event);
            }
        }
    }
}

extern "C" bool fn_8021AA48(HitInfo_unk* data); // inferred: db/ParseUtil_unk (takes BulletParam_unk)
extern "C" bool fn_8021A9CC(HitInfo_unk* data); // inferred: db/ParseUtil_unk (takes BulletParam_unk)
// NONMATCHING: 93.0% - stack slots (the rotation temporaries go the other way, one vector less), .sdata2 pool
extern "C" int fn_800FD658(BattleTask* mgr, CharaTask* attacker, CharaTask* chara, HitInfo_unk* data, int a, int id,
                           int b);

// inferred: blast of `attacker` with `data` at `pos`: hits every player and enemy in range data->m44 that the
// hit reaches (fn_800FD658, or CharaTask::fn_80178668 for the kinds of fn_8021A9CC), with the effects when
// `effect`; a hit by a player raises the gauge m38A04. Returns whether something was hit. Called as extern "C"
// by game/Bullet, game/EnemyTrista, SkillEffectManager and the controls.
// NONMATCHING: 97.6% - the original keeps the addresses of the attacker position and rotation in registers
// before the closing effect and sound; .sdata2 pool
extern "C" int fn_800FB4B0(BattleTask* mgr, CharaTask* attacker, HitInfo_unk* data, int id, const atn::Vector3* pos,
                           int a, int b, int, int effect) {
    if (data == 0 || attacker == 0) {
        return 0;
    }
    bool hit = false;
    float range = data->m44;
    for (PlayerTask* chara = lbl_8087F8A0->mHead; chara != 0; chara = chara->mNext) {
        if (chara->fn_801533C8(id, data) && !fn_8021AA48(data)) {
            continue;
        }
        if (data->m64 == 4 && attacker->fn_8016F3D0(chara, 1)) {
            continue;
        }
        if (data->m64 == 2 && !attacker->fn_8016F3D0(chara, 0)) {
            continue;
        }
        if (data->m64 == 0 && attacker != chara) {
            continue;
        }
        atn::Vector3 d = *pos - chara->mPos;
        if (!(PSVECSquareMag(&d) < range * range)) {
            continue;
        }
        if (fn_8021A9CC(data)) {
            if (chara->fn_80178668(data, 0, data->m5C, attacker, 0)) {
                hit = true;
            }
        } else {
            hit |= fn_800FD658(mgr, attacker, chara, data, a, id, b);
            if (effect != 0) {
                EffectColor_unk color(1.0f, 1.0f, 1.0f, 1.0f);
                mgr->fn_80107168(data->mB0, 0, 0, chara->GetModel_unk(), &lbl_807C7030, &lbl_807C7030, &color, 1.0f);
            }
        }
    }
    for (EnemyTask* chara = EnemyManager::sInstance->mHead; chara != 0; chara = chara->mNext) {
        if (chara->fn_801533C8(id, data) && !fn_8021AA48(data)) {
            continue;
        }
        if (data->m64 == 4 && attacker->fn_8016F3D0(chara, 1)) {
            continue;
        }
        if (data->m64 == 2 && !attacker->fn_8016F3D0(chara, 1)) {
            continue;
        }
        if (data->m64 == 0 && attacker != chara) {
            continue;
        }
        atn::Vector3 d = *pos - chara->mPos;
        if (!(PSVECSquareMag(&d) < range * range)) {
            continue;
        }
        if (fn_8021A9CC(data)) {
            if (chara->fn_80178668(data, 0, data->m5C, attacker, 0)) {
                hit = true;
            }
        } else {
            hit |= fn_800FD658(mgr, attacker, chara, data, a, id, b);
            if (effect != 0) {
                EffectColor_unk color(1.0f, 1.0f, 1.0f, 1.0f);
                mgr->fn_80107168(data->mB0, 0, 0, chara->GetModel_unk(), &lbl_807C7030, &lbl_807C7030, &color, 1.0f);
            }
        }
    }
    if (effect != 0) {
        EffectColor_unk color(1.0f, 1.0f, 1.0f, 1.0f);
        mgr->fn_801070C8(data->mB0, 0, 0, 0, &attacker->mPos, &attacker->mRot, &color, 1.0f);
        mgr->fn_80107208(data->mB0, &attacker->mPos, 1.0f);
    }
    if (hit && attacker != 0 && attacker->mType == 0) {
        float add = mgr->m38A1C * (attacker->mFlag28 ? mgr->m38A38 : 1.0f);
        float zero;
        mgr->m38A04 = MAX(zero = 0.0f, MIN(mgr->m38A04 + add, mgr->m38A0C));
    }
    if (attacker != 0) {
        attacker->mStatus.mEntries.mCount = 0;
    }
    return hit;
}

// inferred: hit test of a bullet of `chara` on behalf of `user` (fn_800FAB80); returns whether a character was hit
// (SkillEffectManager)
extern "C" int fn_800FBA30(BattleTask* mgr, CharaTask* chara, CharaTask* user, HitInfo_unk* data, int id,
                           const atn::Vector3* pos, const atn::Vector3* rot) {
    mgr->m3E398 = user;
    int hit = fn_800FAB80(mgr, chara, data, id, pos, rot, 0, 30, -1, -1, 0.0f, 1.0f);
    mgr->m3E398 = 0;
    return hit;
}

// inferred: the sphere `item` is excluded from the hit `data` (name invented)
inline bool IsSkipped_unk(const HitInfo_unk* data, const CharaTask::Item14_unk* item) {
    bool skip = false;
    int flags = item->m00;
    if (data->m04 == 0xCB) {
        if ((flags & 4) == 4) {
            skip = true;
        }
    } else {
        if (data->m01 == 1 && (flags & 2) == 2) {
            skip = true;
        }
        if (data->m01 == 0 && (flags & 1) == 1) {
            skip = true;
        }
    }
    return skip;
}

extern "C" double acos(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" double sqrt(double x);
// inferred: squared distance (name invented)
inline float SquareDist_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 d = a - b;
    return PSVECSquareMag(&d);
}
// inferred: squared length on the ground plane (name invented)
inline float SquareMagXZ_unk(const atn::Vector3& v) {
    atn::Vector3 flat(v.x, 0.0f, v.z);
    return PSVECSquareMag(&flat);
}

// inferred: index of the hit sphere of `chara` reached by the melee arc of `attacker` (reach data->m40 plus
// the reach of the attacker, angle data->m50 around the facing of the attacker turned by `angle`, height
// data->m54) nearest to the attacker; -1 when nothing is reached. Called as extern "C" by the enemies.
// NONMATCHING: 99.96% - register of the absolute height (f3/f4)
extern "C" int fn_800FBA9C(BattleTask* mgr, CharaTask* attacker, CharaTask* chara, HitInfo_unk* data, int id,
                           float angle) {
    if (chara->fn_801533C8(id, data)) {
        return -1;
    }
    if (attacker != 0 && data != 0) {
        if (data->m64 == 4 && attacker->fn_8016F3D0(chara, 1) && !attacker->fn_80154E38(chara)) {
            return -1;
        }
        if (data->m64 == 2 && !attacker->fn_8016F3D0(chara, 1)) {
            return -1;
        }
    }
    if (chara->m55C == 6 && chara->m560 == 0x41) {
        return -1;
    }
    int found = -1;
    float best = 1e16f;
    float reach = data->m40 + attacker->mStatus.mParamB.m28;
    // inferred: m50/m54 are floats (arc angle, height); HitInfo_unk keeps them as bytes (as game/Em202Spider)
    float arc = *(float*)&data->m50[0];
    float height = *(float*)&data->m50[4];
    for (unsigned int i = 0; i < chara->m624.mSize; i++) {
        CharaTask::Item14_unk* item = chara->fn_80148B0C(i);
        if (item == 0 || IsSkipped_unk(data, item)) {
            continue;
        }
        float range = reach + item->m04.mRadius;
        atn::Vector3 d = item->m04.mCenter - attacker->mPos;
        float dy = __fabs(d.y);
        if (dy > range) {
            continue;
        }
        float dist2 = SquareMagXZ_unk(d);
        if (dist2 > range * range) {
            continue;
        }
        d.y = 0.0f;
        if (!d.IsZero_unk()) {
            PSVECNormalize(&d, &d);
            atn::Vector3 dir(0.0f, 0.0f, 1.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', angle + attacker->mRot.y);
            PSMTXMultVec(m, &dir, &dir);
            float dot = PSVECDotProduct(&d, &dir);
            float theta = acos(dot);
            if (theta > 0.5f * arc) {
                continue;
            }
            if (height > 0.0f) {
                float s = sqrt(1.0f - dot * dot);
                float dist = sqrt(dist2);
                if (dist * s > height + item->m04.mRadius) {
                    continue;
                }
            }
        }
        float dist = SquareDist_unk(item->m04.mCenter, attacker->mPos);
        if (dist < best) {
            found = i;
            best = dist;
        }
    }
    return found;
}

extern "C" bool fn_8021A7A0(HitInfo_unk* data); // inferred: db/ParseUtil_unk (takes the record as BulletParam_unk)
extern "C" int fn_800551BC(atn::ColliResult_unk* r, const atn::ColliCapsuleShape_unk* capsule,
                           const atn::ColliSphereShape_unk* sphere); // inferred: capsule against sphere

// inferred: index of the hit sphere of `chara` reached by the hit `data` at `pos` (as fn_800FC1D0, the sphere
// radius grows by data->m58, scaled by the attacker for some bullets; the first sphere tests the collision
// capsule of the character when mFlagB29 is set); -1 when nothing is reached. Em403WarLion calls it.
extern "C" int fn_800FBE70(BattleTask* mgr, CharaTask* attacker, CharaTask* chara, HitInfo_unk* data, int id,
                           const atn::Vector3* pos, int any, int type) {
    if (chara->fn_801533C8(id, data)) {
        return -1;
    }
    if (any == 0) {
        if (attacker != 0) {
            if (data->m64 == 4 && attacker->fn_8016F3D0(chara, 1) && !attacker->fn_80154E38(chara)) {
                return -1;
            }
            if (data->m64 == 2 && !attacker->fn_8016F3D0(chara, 1)) {
                return -1;
            }
            if (data->m64 == 0 && attacker != chara) {
                return -1;
            }
        } else {
            if (data->m64 == 4 && chara->fn_8016F368(type)) {
                return -1;
            }
            if (data->m64 == 2 && !chara->fn_8016F368(type)) {
                return -1;
            }
        }
    }
    if (chara->m55C == 6 && chara->m560 == 0x41) {
        return -1;
    }
    float best = 1e16f;
    float scale = 1.0f;
    int found = -1;
    if (fn_8021A7A0(data) && attacker != 0) {
        scale = attacker->fn_8017AC24();
    }
    for (unsigned int i = 0; i < chara->m624.mSize; i++) {
        CharaTask::Item14_unk* item = chara->fn_80148B0C(i);
        if (item != 0 && IsSkipped_unk(data, item)) {
            continue;
        }
        if (i == 0 && chara->mFlagB29) {
            // inferred: the capsule of the character is copied as one shape (as atn/atn_colli.cpp)
            atn::ColliCapsuleShape_unk capsule(*(const atn::ColliCapsuleShape_unk*)&chara->mColli.m3C);
            atn::ColliSphereShape_unk sphere;
            sphere.mCenter = *pos;
            sphere.mRadius = data->m58 * scale;
            atn::ColliResult_unk result;
            if (fn_800551BC(&result, &capsule, &sphere)) {
                best = 0.0f;
                found = i;
            }
        } else if (item != 0) {
            float radius = data->m58 * scale + item->m04.mRadius;
            atn::Vector3 d = *pos - item->m04.mCenter;
            float dist = PSVECSquareMag(&d);
            if (dist < radius * radius && dist < best) {
                found = i;
                best = dist;
            }
        }
    }
    return found;
}

// inferred: index of the hit sphere of `chara` nearest to `pos` (-1 when the bullet `id` already hit it, the
// side of the hit `data->m64` does not reach it, or every sphere is excluded); `any` skips the side tests, `type`
// is the side of a bullet without attacker
int BattleTask::fn_800FC1D0(CharaTask* attacker, CharaTask* chara, HitInfo_unk* data, int id,
                            const atn::Vector3* pos, int any, int type) {
    if (chara->fn_801533C8(id, data)) {
        return -1;
    }
    if (any == 0) {
        if (attacker != 0) {
            if (data->m64 == 4 && attacker->fn_8016F3D0(chara, 1) && !attacker->fn_80154E38(chara)) {
                return -1;
            }
            if (data->m64 == 2 && !attacker->fn_8016F3D0(chara, 1)) {
                return -1;
            }
            if (data->m64 == 0 && attacker != chara) {
                return -1;
            }
        } else {
            if (data->m64 == 4 && chara->fn_8016F368(type)) {
                return -1;
            }
            if (data->m64 == 2 && !chara->fn_8016F368(type)) {
                return -1;
            }
        }
    }
    float best = 1e16f;
    int found = -1;
    for (unsigned int i = 0; i < chara->m624.mSize; i++) {
        CharaTask::Item14_unk* item = chara->fn_80148B0C(i);
        if (item != 0) {
            if (!IsSkipped_unk(data, item)) {
                atn::Vector3 d = *pos - item->m04.mCenter;
                float dist = PSVECSquareMag(&d);
                if (dist < best) {
                    found = i;
                    best = dist;
                }
            }
        }
    }
    return found;
}

// inferred: damage record shown by fn_80108C10 (local view, as StatusDamage_unk/DaemonDamage_unk of other files)
struct BattleDamage_unk {
    int m00; // inferred: kind (1, 4 and 5 are shown)
    int m04; // inferred: value (negative: recovery)
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int m1C_28 : 4; // inferred: cleared by fn_800FDB20 (the rest of the word is left as it is)
    unsigned int m1C_0 : 28;
};
extern "C" void fn_80108C10(BattleTask* mgr, BattleDamage_unk* damage, const atn::Vector3* pos, CharaTask* attacker,
                            CharaTask* chara, unsigned int flags, int skipEmpty);
extern "C" bool fn_8021A8D0(HitInfo_unk* data); // inferred: db/ParseUtil_unk (takes BulletParam_unk)
extern "C" float fn_8010B250(BattleTask* mgr, int level, BattleCalcParam_unk* param);

// inferred: center of the hit sphere `index` of `chara` (the middle height of its collision capsule for the first
// sphere when mFlagB29 is set); name invented
inline atn::Vector3 HitCenterAt_unk(CharaTask* chara, int index) {
    atn::Vector3 center;
    if (index == 0 && chara->mFlagB29) {
        atn::Vector3 start = chara->mColli.m3C;
        atn::Vector3 end = chara->mColli.m48;
        center = chara->mColli.m3C;
        float half = 0.5f * (end.y - start.y);
        center.y += half;
    } else {
        center = chara->fn_80148B0C(index)->m04.mCenter;
    }
    return center;
}

// inferred: direct hit of `attacker` with `hit` (id `id`, `frames`) on the sphere `index` of `chara`: the attack
// parameters m38A5C (point on the surface of the sphere toward `attacker`; level of the attacker plus the hit
// against `chara`, raised by the critical and combo states and the count of the control, lowered by some states
// of `chara`, or the fixed level of the hit; push away from `attacker`, scaled by GameConfig::mForceRatio with a
// minimum for the characters with mFlag10) go through fn_800FDF14. C name, called by CharaTask as extern "C"
// NONMATCHING: 99.3% - stack slots of the rotation temporaries (allocated the other way, as in fn_800FD658)
extern "C" void fn_800FC410(BattleTask* mgr, CharaTask* attacker, CharaTask* chara, int index, HitInfo_unk* hit, int id,
                            int frames) {
    fn_800E2A24(&mgr->m38A5C);
    mgr->m38A5C.m00 = (int)attacker; // inferred: the field is an int (see fn_800FD658)
    mgr->m38A5C.m04 = chara;
    mgr->m38A5C.m40 = index;
    mgr->m38A5C.m08 = hit;
    atn::Vector3 dir = attacker->mPos - HitCenterAt_unk(chara, index);
    if (!((float)__fabs(PSVECSquareMag(&dir)) < 1.1920929e-7f)) {
        PSVECNormalize(&dir, &dir);
        dir *= chara->m5B0;
        mgr->m38A5C.m1C = dir + HitCenterAt_unk(chara, index);
    } else {
        float radius = chara->m5B0;
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = radius;
        atn::Matrix3x4 m;
        atn::Matrix3x4* p = &m;
        p->SetIdentity();
        if (0.0f != chara->mRot.z) {
            atn::Matrix3x4::RotateAxis_unk(*p, 'z', chara->mRot.z);
        }
        if (0.0f != chara->mRot.y) {
            atn::Matrix3x4::RotateAxis_unk(*p, 'y', chara->mRot.y);
        }
        if (0.0f != chara->mRot.x) {
            atn::Matrix3x4::RotateAxis_unk(*p, 'x', chara->mRot.x);
        }
        PSMTXMultVec(p->m, &dir, &dir);
        mgr->m38A5C.m1C = dir + HitCenterAt_unk(chara, index);
    }
    atn::Vector3 n = dir;
    PSVECNormalize(&n, &n);
    mgr->m38A5C.m28 = atn::Vector3(-n.x, -n.y, -n.z);
    mgr->m38A5C.m34 = id;
    mgr->m38A5C.m38 = frames;
    if (attacker != 0 && attacker->m648 != 0) {
        mgr->m38A5C.m3C = attacker->m648->m04;
    }
    mgr->m38A5C.m44 = 0;
    int level = attacker->mStatus.mParamB.m30 + hit->m3C - chara->mStatus.mParamB.m34;
    mgr->m38A5C.m54 = attacker->fn_8017A228(0);
    mgr->m38A5C.m58 = attacker->fn_8017A228(1);
    bool fixed = false;
    if (attacker->m59D) {
        if (attacker->m598 == 0) {
            level = 1;
            fixed = true;
        } else if (attacker->m598 == 1 && lbl_8087F610 == 0) {
            level++;
        }
    }
    if (attacker->mStatus.fn_80134234()) {
        int count = 1;
        if (attacker->mControl.get() != 0) {
            count = attacker->mControl->vf2C();
        }
        if (count > 0) {
            if (attacker->m598 + 1 < count) {
                level = 1;
                fixed = true;
            } else if (lbl_8087F610 == 0) {
                level++;
            }
        }
    }
    if (attacker->m59E) {
        mgr->m38A5C.m0C |= 0x80;
        if (!fixed) {
            level++;
        }
    }
    if (attacker->m59F) {
        mgr->m38A5C.m0C |= 0x4000;
    }
    if (level <= 4 && chara != 0 && chara->mFlagB16) {
        int lower = level - 1;
        level = 1;
        if (lower > 1) {
            level = lower;
        }
    }
    if (attacker != 0 && chara != 0 && chara->fn_801789D8(0, 0x400000)) {
        if (level <= 4) {
            level = MAX(level - 2, 0);
        } else {
            level = MAX(level - 1, 0);
        }
    }
    if (hit != 0 && (hit->mAC & 0x400000) == 0x400000) {
        level = hit->m3C;
    }
    mgr->m38A5C.m5C = attacker->m5A0;
    float scale = fn_8010B250(mgr, level, &mgr->m38A5C);
    mgr->m38A5C.m10 = chara->mPos - attacker->mPos;
    if (!((float)__fabs(PSVECSquareMag(&mgr->m38A5C.m10)) < 1.1920929e-7f)) {
        PSVECNormalize(&mgr->m38A5C.m10, &mgr->m38A5C.m10);
    } else {
        atn::Vector3 v(0.0f, 0.0f, 1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', chara->mRot.y);
        PSMTXMultVec(m, &v, &v);
        mgr->m38A5C.m10 = atn::Vector3(-v.x, -v.y, -v.z);
    }
    mgr->m38A5C.m10 *= scale;
    if (chara->mFlag10 && PSVECSquareMag(&mgr->m38A5C.m10) > 0.01f) {
        mgr->m38A5C.m10 *= lbl_8087F0A8->mForceRatio;
        if (PSVECMag(&mgr->m38A5C.m10) < 0.11f) {
            PSVECNormalize(&mgr->m38A5C.m10, &mgr->m38A5C.m10);
            mgr->m38A5C.m10 *= 0.11f;
        }
    }
    mgr->fn_800FDF14(&mgr->m38A5C);
}

// inferred: the bullet `data` (id `id`) of `attacker` hits the sphere `index` of `chara` at `pos`: the attack
// parameters m38A5C (point on the surface of the sphere toward `pos`, or along the facing of `chara`; push away
// from `pos` scaled by the level of the bullet against `chara`; no push for the bullet 0x139 of a character with
// mFlag31) go through fn_800FDF14 and are copied to `out`. In multiplayer a guarding character takes the bullets
// 0x131 and 0x142 as a "guard" popup. C name
// NONMATCHING: 98.0% - stack slots of the rotation temporaries (allocated the other way), copy of the record pairs
extern "C" void fn_800FCD24(BattleTask* mgr, CharaTask* attacker, CharaTask* chara, int index, HitInfo_unk* data, int id,
                            const atn::Vector3* pos, int time, BattleCalcParam_unk* out, float, float rate) {
    if (lbl_8087F610 != 0 && (chara->mStatus.m14 & 1u) == 1 && data != 0 && (data->m04 == 0x142 || data->m04 == 0x131)) {
        BattleDamage_unk damage;
        damage.m08 = 0;
        damage.m0C = 0;
        damage.m10 = 0;
        damage.m14 = -1;
        damage.m1C_28 = 0;
        damage.m18 = -1;
        damage.m04 = 0;
        damage.m00 = 1;
        fn_80108C10(mgr, &damage, pos, attacker, chara, 0x108, 0);
        return;
    }
    fn_800E2A24(&mgr->m38A5C);
    mgr->m38A5C.m00 = (int)attacker; // inferred: the field is an int (see fn_800FD658)
    mgr->m38A5C.m04 = chara;
    mgr->m38A5C.m40 = index;
    mgr->m38A5C.m08 = data;
    mgr->m38A5C.m5C = rate;
    atn::Vector3 dir = *pos - HitCenterAt_unk(chara, index);
    if (!((float)__fabs(PSVECSquareMag(&dir)) < 1.1920929e-7f)) {
        PSVECNormalize(&dir, &dir);
        dir *= chara->m5B0;
        mgr->m38A5C.m1C = dir + HitCenterAt_unk(chara, index);
    } else {
        float radius = chara->m5B0;
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = radius;
        atn::Matrix3x4 m;
        atn::Matrix3x4* p = &m;
        p->SetIdentity();
        if (0.0f != chara->mRot.z) {
            atn::Matrix3x4::RotateAxis_unk(*p, 'z', chara->mRot.z);
        }
        if (0.0f != chara->mRot.y) {
            atn::Matrix3x4::RotateAxis_unk(*p, 'y', chara->mRot.y);
        }
        if (0.0f != chara->mRot.x) {
            atn::Matrix3x4::RotateAxis_unk(*p, 'x', chara->mRot.x);
        }
        PSMTXMultVec(p->m, &dir, &dir);
        mgr->m38A5C.m1C = dir + HitCenterAt_unk(chara, index);
    }
    atn::Vector3 n = dir;
    PSVECNormalize(&n, &n);
    mgr->m38A5C.m28 = atn::Vector3(-n.x, -n.y, -n.z);
    mgr->m38A5C.m34 = id;
    mgr->m38A5C.m38 = time;
    if (attacker != 0 && attacker->m648 != 0) {
        mgr->m38A5C.m3C = attacker->m648->m04;
    }
    mgr->m38A5C.m44 = 2;
    mgr->m38A5C.m48 = 0;
    int level = data->m3C - chara->mStatus.mParamB.m34;
    if (attacker != 0 && chara != 0 && chara->fn_801789D8(0, 0x400000)) {
        if (level <= 4) {
            level = MAX(level - 2, 0);
        } else {
            level = MAX(level - 1, 0);
        }
    }
    float scale = fn_8010B250(mgr, level, &mgr->m38A5C);
    mgr->m38A5C.m10 = chara->mPos - *pos;
    if (!((float)__fabs(PSVECSquareMag(&mgr->m38A5C.m10)) < 1.1920929e-7f)) {
        PSVECNormalize(&mgr->m38A5C.m10, &mgr->m38A5C.m10);
    } else {
        atn::Vector3 v(0.0f, 0.0f, 1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', chara->mRot.y);
        PSMTXMultVec(m, &v, &v);
        mgr->m38A5C.m10 = atn::Vector3(-v.x, -v.y, -v.z);
    }
    mgr->m38A5C.m10 *= scale;
    if (attacker != 0 && chara->mFlag31 && mgr->m38A5C.m08->m04 == 0x139) {
        mgr->m38A5C.m10 *= 0.0f;
    }
    if (mgr->m38A5C.m08 != 0 && mgr->m38A5C.m08->m04 == 0xD5) {
        mgr->m38A5C.m0C |= 0x80;
    }
    mgr->fn_800FDF14(&mgr->m38A5C);
    if (out != 0) {
        *out = mgr->m38A5C;
    }
}

// inferred: center of the hit body of `chara`: the middle height of its collision capsule when mFlagB29 is
// set, else its first hit sphere (name invented)
inline atn::Vector3 HitCenter_unk(CharaTask* chara) {
    atn::Vector3 center;
    if (chara->mFlagB29) {
        atn::Vector3 start = chara->mColli.m3C;
        atn::Vector3 end = chara->mColli.m48;
        center = chara->mColli.m3C;
        float half = 0.5f * (end.y - start.y);
        center.y += half;
    } else {
        center = chara->fn_80148B0C(0)->m04.mCenter;
    }
    return center;
}

// inferred: hit of `attacker` on `chara` with `data`: fills the attack parameters m38A5C (the point of the hit
// on the surface of `chara`, toward the attacker or else along the facing of `chara`, and the direction) and
// runs them through fn_800FDF14. Called as extern "C" by CharaTask and Em314Ivy.
// NONMATCHING: 93.0% - stack slots (the rotation temporaries go the other way, one vector less), .sdata2 pool
extern "C" int fn_800FD658(BattleTask* mgr, CharaTask* attacker, CharaTask* chara, HitInfo_unk* data, int a, int flag,
                           int b) {
    fn_800E2A24(&mgr->m38A5C);
    mgr->m38A5C.m00 = (int)attacker; // inferred: the field is an int (BattleTask::fn_800FDE60 takes it as int)
    mgr->m38A5C.m04 = chara;
    mgr->m38A5C.m08 = data;
    mgr->m38A5C.m0C = 0;
    if (flag != 0) {
        mgr->m38A5C.m0C |= 0x40000;
    }
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    if (attacker != 0) {
        dir = attacker->mPos - chara->mPos;
        if (!((float)__fabs(PSVECSquareMag(&dir)) < 1.1920929e-7f)) {
            PSVECNormalize(&dir, &dir);
            dir *= chara->m5B0;
            mgr->m38A5C.m1C = dir + HitCenter_unk(chara);
        } else {
            float radius = chara->m5B0;
            dir.x = 0.0f;
            dir.y = 0.0f;
            dir.z = radius;
            atn::Matrix3x4 m;
            atn::Matrix3x4* p = &m;
            p->SetIdentity();
            if (0.0f != chara->mRot.z) {
                atn::Matrix3x4::RotateAxis_unk(*p, 'z', chara->mRot.z);
            }
            if (0.0f != chara->mRot.y) {
                atn::Matrix3x4::RotateAxis_unk(*p, 'y', chara->mRot.y);
            }
            if (0.0f != chara->mRot.x) {
                atn::Matrix3x4::RotateAxis_unk(*p, 'x', chara->mRot.x);
            }
            PSMTXMultVec(p->m, &dir, &dir);
            mgr->m38A5C.m1C = dir + HitCenter_unk(chara);
        }
    }
    atn::Vector3 n = dir;
    PSVECNormalize(&n, &n);
    mgr->m38A5C.m28 = -n;
    mgr->m38A5C.m34 = a;
    mgr->m38A5C.m38 = b;
    mgr->m38A5C.m44 = 3;
    return mgr->fn_800FDF14(&mgr->m38A5C);
}


// inferred: bullet `data` (id `id`) of `attacker` hits `chara` at `pos` toward `dir`: a guarding character takes
// a "guard" popup (returns 4); else the attack parameters m38A5C (level of the bullet against the level of
// `chara`, lowered by some states and by the status 0x43 of the attacker) go through fn_800FDF14, and a hit by
// a player raises the gauge m38A04. Called as extern "C" by game/Bullet and some enemies.
// NONMATCHING: 99.95% - register of the word of the bitfield of the damage record (r3/r10)
extern "C" int fn_800FDB20(BattleTask* mgr, CharaTask* attacker, CharaTask* chara, int arg, HitInfo_unk* data, int id,
                           const atn::Vector3* pos, const atn::Vector3* dir, int arg8) {
    if (chara->fn_801533C8(id, data)) {
        return 0;
    }
    if ((chara->mStatus.m14 & 1u) == 1 && !fn_8021A8D0(data) && data != 0 && (data->mAC & 0x10000000) != 0x10000000) {
        if (attacker != 0 && attacker->m60->m24 == 0x14) {
            lbl_8087F430->fn_803750E4(0x13C);
        }
        BattleDamage_unk damage;
        damage.m08 = 0;
        damage.m0C = 0;
        damage.m10 = 0;
        damage.m14 = -1;
        damage.m1C_28 = 0;
        damage.m18 = -1;
        damage.m04 = 0;
        damage.m00 = 1;
        fn_80108C10(mgr, &damage, pos, attacker, chara, 0x108, 0);
        return 4;
    }
    fn_800E2A24(&mgr->m38A5C);
    mgr->m38A5C.m00 = (int)attacker; // inferred: the field is an int (see fn_800FD658)
    mgr->m38A5C.m04 = chara;
    mgr->m38A5C.m40 = arg;
    mgr->m38A5C.m08 = data;
    mgr->m38A5C.m1C = *pos;
    mgr->m38A5C.m28 = *dir;
    mgr->m38A5C.m34 = id;
    mgr->m38A5C.m38 = arg8;
    mgr->m38A5C.m3C = 1;
    mgr->m38A5C.m44 = 1;
    mgr->m38A5C.m48 = 0;
    int level = data->m3C - chara->mStatus.mParamB.m34;
    if (attacker != 0 && chara != 0 && chara->fn_801789D8(0, 0x400000)) {
        if (level <= 4) {
            level = MAX(level - 2, 0);
        } else {
            level = MAX(level - 1, 0);
        }
    }
    if (attacker != 0 && attacker->mStatus.fn_80134168(0x43, -1) > 15) {
        level /= (attacker->mStatus.fn_80134168(0x43, -1) + 10) / 10;
    }
    float scale = fn_8010B250(mgr, level, &mgr->m38A5C);
    atn::VecAssign_unk(mgr->m38A5C.m10, mgr->m38A5C.m28);
    mgr->m38A5C.m10 *= scale;
    mgr->fn_800FDF14(&mgr->m38A5C);
    if (attacker != 0 && attacker->mType == 0) {
        float add = mgr->m38A18 * (attacker->mFlag28 ? mgr->m38A38 : 1.0f);
        float zero;
        mgr->m38A04 = MAX(zero = 0.0f, MIN(mgr->m38A04 + add, mgr->m38A0C));
    }
    return 0;
}

void BattleTask::fn_800FDE60(int effect, CharaTask* chara, const atn::Vector3& dir) {
    fn_800E2A24(&m38A5C);
    m38A5C.m00 = effect;
    m38A5C.m04 = chara;
    m38A5C.m08 = fn_80219E6C(10000);
    m38A5C.m44 = 3;
    m38A5C.m28 = dir;
    m38A5C.m48 = 0;
    m38A5C.m4C = 0;
    m38A5C.m0C |= 0x10000;
    fn_800FDF14(&m38A5C);
}


extern "C" double cos(double x);
extern "C" ArenaTask* lbl_8087EE78; // inferred: the arena (ArenaTask's sInstance)
extern "C" int fn_80444020(Inventory* party, int item, int a, int b, int c, int d, int* outA, int* outB,
                           int* outC); // inferred: adds an item (as in game/BattleResult.cpp)
extern "C" int fn_8021E48C(int item, int arg); // inferred: count of the item given at once
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text); // inferred: text of a pane
extern "C" int rand();

// inferred: camp message table (lbl_8087F1E4, as in game/CosEditItemList_unk.cpp)
struct BattleMsg_unk {
    struct Pair_unk {
        int m00;
        const wchar_t* mText;
    };
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    // inferred: text of an (id, text) pair of the table (as in game/CosEditItemList_unk.cpp)
    const wchar_t* GetPair_unk(int id) {
        Pair_unk* pairs = (Pair_unk*)mText;
        return pairs[id].mText != 0 ? pairs[id].mText : lbl_808813D0;
    }
    const wchar_t* mText[1];
};
extern "C" BattleMsg_unk* lbl_8087F1E4;
// inferred: item data (its name at +8, as MpItem_unk of MP/D2AMpMenuResult.cpp); names invented
struct BattleItem_unk {
    unsigned char m00[0x8];
    const wchar_t* mName;
    unsigned char m0C[0xEC - 0xC];
    ArmsCond_unk mConds[2]; // 0xEC: inferred: status conditions of the arrow (fn_8010B398)
};
extern "C" BattleItem_unk* fn_80211480(int id);

// inferred: characters whose head shots do not count (bosses); name invented
static const char sNoHead0_unk[] = "em002_bs00_03";
static const char sNoHead1_unk[] = "em002_bs00_13";
static const char sNoHead2_unk[] = "np131_bs00_03";
static const char sNoHead3_unk[] = "np131_bs00_07";
static const char sNoHead4_unk[] = "np131_bs00_11";
static const char sNoHead5_unk[] = "np131_bs00_18";
static const char* const sNoHead_unk[] = {sNoHead0_unk, sNoHead1_unk, sNoHead2_unk,
                                          sNoHead3_unk, sNoHead4_unk, sNoHead5_unk};
// inferred: the character is one of sNoHead_unk (inlined; name invented)
inline bool IsNoHead_unk(CharaTask* chara) {
    if (chara == 0) {
        return false;
    }
    CharaInfo_unk* info = chara->m60;
    if (info == 0) {
        return false;
    }
    for (int i = 0; i < 6; i++) {
        if (strcmp(info->m10, sNoHead_unk[i]) == 0) {
            return true;
        }
    }
    return false;
}
// inferred: the flag is set (or cleared) in both flag words of the attack (names invented)
inline void SetHitFlag_unk(BattleCalcParam_unk* p, unsigned int flag) {
    p->m0C |= flag;
    p->m94 |= flag;
}
inline void ClearHitFlag_unk(BattleCalcParam_unk* p, unsigned int flag) {
    p->m0C &= ~flag;
    p->m94 &= ~flag;
}
// inferred: the attack is one of the special bullets (names invented)
inline bool IsSpecialHit_unk(HitInfo_unk* hit) {
    if (hit != 0) {
        switch (hit->m04) {
        case 0xCC:
        case 0xD1:
        case 0xD5:
        case 0x5BC:
        case 0x658:
            return true;
        }
    }
    return false;
}
// inferred: center of the hit sphere `index` of `chara`, or its position (names invented)
inline atn::Vector3 HitPos_unk(CharaTask* chara, int index) {
    if (chara->fn_80148B0C(index) != 0) {
        return chara->fn_80148B0C(index)->m04.mCenter;
    }
    return chara->mPos;
}
// inferred: damage record of fn_80108C10 built with its inline constructor (name invented)
struct HitDamage_unk : public BattleDamage_unk {
    HitDamage_unk() {
        m04 = 0;
        m08 = 0;
        m0C = 0;
        m10 = 0;
        m14 = -1;
        m1C_28 = 0;
        m18 = -1;
        m00 = 1;
    }
};

// NONMATCHING: written from the assembly, not compared yet
// inferred: applies the attack `param` (attacker m00, target m04, bullet m08): back attacks (m0C 0x10), head shots
// (0x80000000), guard/critical/status flags, steal of the target's item (status 0x3C), drain (0x31, 0x23, 7, 8, 9,
// 0x47), the damage popup, the effect and the sound (fn_801018E4/fn_80101CDC), hate (fn_80102EAC), the tension
// gauges (fn_8012F034) and the morale m38A04. Returns 0 when the attack does not apply
int BattleTask::fn_800FDF14(BattleCalcParam_unk* p) {
    CharaTask* target = p->m04;
    CharaTask* attacker = (CharaTask*)p->m00;
    HitInfo_unk* hit = p->m08;
    if (target == 0 || hit == 0) {
        return 0;
    }
    if ((target->m54C & 0x2000) == 0x2000 && (hit->mAC & 0x20000) != 0x20000) {
        return 0;
    }
    if (!fn_8010B180(attacker, target)) {
        return 0;
    }
    if (p->m44 == 0) {
        atn::Vector3 front(0.0f, 0.0f, target->mFlag31 ? 1.0f : -1.0f);
        atn::Matrix3x4 m;
        m.fn_8013A13C(target->mRot.y);
        PSMTXMultVec(m.m, &front, &front);
        float dot = PSVECDotProduct(&front, &p->m28);
        if (dot < (float)cos(2.0943952f)) {
            p->m0C |= 0x10;
        }
    }
    if (p->m44 == 1 && attacker != 0 && attacker->mType == 0) {
        MtxPtr head = target->GetModel_unk()->GetNodeMtx_unk("head");
        if (head != 0 && target->mFlag22 != 1 && !IsNoHead_unk(target)) {
            atn::Vector3 pos(head[0][3], head[1][3], head[2][3]);
            atn::Vector3 d = pos - p->m1C;
            float radius = target->m5B4 * 0.25f;
            if (PSVECSquareMag(&d) < radius * radius) {
                SetHitFlag_unk(p, 0x80000000);
            }
        }
    }
    if (target->fn_801789D8(0x4F60, 0)) {
        SetHitFlag_unk(p, 0x100);
    }
    if ((unsigned int)(hit->m04 - 0x1FA) <= 2) {
        ClearHitFlag_unk(p, 0x80000000);
    }
    if (fn_8010B398(p)) {
        SetHitFlag_unk(p, 0x2000000);
    }
    if (attacker != 0 && !attacker->fn_8016F3D0(target, 1)) {
        if (attacker != 0 && target != 0 && attacker->mStatus.fn_80133EE8(0x3C)) {
            int item = target->m5C != 0 ? target->m5C->mD0 : 0;
            if (item > 0) {
                int index = FindChara_unk(target);
                BattleChara_unk* entry = index >= 0 ? &mCharas[index] : 0;
                if (entry != 0 && entry->m92E == 0) {
                    unsigned int flags = 0x51;
                    if (target->mFlag22 == 1) {
                        flags |= 0x20;
                    }
                    int num = fn_8021E48C(item, flags);
                    if (num >= 0) {
                        int got = 0;
                        int count = 0;
                        int rest = 0;
                        Inventory::instance_->mDB10 = 0;
                        if (fn_80444020(Inventory::instance_, item, 0, num, 0, 0, &got, &count, &rest)) {
                            BattleItem_unk* data = fn_80211480(got > 0 ? got : -1);
                            if (got > 0 && count > 0 && data != 0) {
                                entry->m92E = 1;
                                fn_80109864((SpEffect_unk*)attacker->mStatus.fn_80133EE8(0x3C), attacker);
                                wchar_t text[256];
                                fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x1F), attacker->m60->m04, data->mName, count);
                                if (lbl_8087F0A8->m06C > 0 && LogMessageTask::sInstance != 0) {
                                    LogMessageTask::sInstance->fn_8047961C(text, -0x3301, 0, 0);
                                }
                                fn_800C31F4(lbl_80735DD0[21], 0, -1, 1.0f);
                                if (lbl_8087F490 != 0) {
                                    lbl_8087F490->fn_803E5E64(got, count, rest);
                                }
                                SetHitFlag_unk(p, 0x2000200);
                            }
                        }
                        Inventory::instance_->mDB10 = 1;
                    }
                }
            }
        }
        if (attacker->mStatus.fn_80133EE8(0x31)) {
            p->m10 *= 2.0f;
        }
    }
    if (attacker != 0 && ((hit->m01 == 0 && p->m44 == 0) || IsSpecialHit_unk(hit))) {
        unsigned int state = attacker->fn_801799BC();
        if ((state & 0x20) == 0x20) {
            p->m0C |= 0x800000;
        }
        if ((state & 0x40) == 0x40) {
            p->m0C |= 0x1000000;
        }
        if ((state & 0x200) == 0x200) {
            p->m0C |= 0x4000000;
        }
        if ((state & 0x400) == 0x400) {
            p->m0C |= 0x8000000;
        }
        if ((state & 0x1000) == 0x1000) {
            p->m0C |= 4;
        }
        p->m94 |= p->m0C & 0xD800004;
    }
    if (attacker != 0 && (p->m50 & 0x80u) != 0x80) {
        float rate = 0.0f;
        rate += (float)attacker->mStatus.fn_80134168(4, -1) / 100.0f;
        rate += attacker->mStatus.mParamB.m18;
        if ((float)(rand() % 1001) / 1000.0f < rate) {
            p->m10 *= 1.5f;
            p->m0C |= 0x80;
        }
    }
    if (attacker != 0) {
        if (attacker->mStatus.fn_80133EE8(0x30)) {
            p->m0C &= ~0x80;
        }
        if (attacker->m55C == 6 && (attacker->m560 == 6 || attacker->m560 == 7 || attacker->m560 == 0xB)) {
            p->m0C &= ~0x80;
        }
    }
    if ((unsigned int)(hit->m04 - 0x1FA) <= 2) {
        ClearHitFlag_unk(p, 0x80);
    }
    if (attacker != 0) {
        if ((p->m0C & 0x80) != 0x80) {
            attacker->mStatus.fn_80133E24(-1, 0x13);
        } else {
            CharaStatusEntry_unk* entry = attacker->mStatus.mEntries.mData;
            for (unsigned int i = 0; i < attacker->mStatus.mEntries.mCount; i++, entry++) {
                if (entry->mSub == 0x13) {
                    fn_80109864((SpEffect_unk*)entry, attacker);
                }
            }
        }
    }
    if ((hit->mAC & 0x80000) == 0x80000) {
        p->m4C = 0;
    }
    if ((attacker != 0 && attacker->mType == 0) || (target != 0 && target->mType == 0)) {
        p->m0C |= 0x40000;
    }
    if ((hit->mAC & 0x40000) == 0x40000) {
        p->m0C |= 0x40000;
    }
    if (attacker != 0 && attacker->mStatus.fn_80133EE8(0x40)) {
        p->m0C |= 0x8000;
    }
    target->vf3C((HitInfo_unk*)p);
    if (attacker != 0) {
        if (attacker->mStatus.fn_80133F6C(-1, 0x1A) > 0) {
            if (!(p->m80 >> 31)) {
                attacker->mStatus.fn_80133E24(-1, 0x1A);
            }
            if (attacker->mStatus.fn_80133F6C(-1, 0x1A) > 0) {
                SetHitFlag_unk(p, 0x2000000);
            }
        }
        if (attacker->mStatus.fn_80133F6C(0x18, -1) > 0) {
            unsigned int flags = p->m94;
            if ((flags & 8) == 8 || (flags & 0x400) == 0x400 || (flags & 1) == 1 || (flags & 0x20) == 0x20) {
                attacker->mStatus.fn_80133E24(0x18, -1);
            }
            if (attacker->mStatus.fn_80133F6C(-1, -1) <= 0) {
                ClearHitFlag_unk(p, 0x2000000);
            }
        }
        if (attacker->mStatus.fn_80133F6C(0x23, -1) > 0) {
            if (p->m80 >> 31) {
                attacker->mStatus.fn_80133E24(0x23, -1);
            }
            if (attacker->mStatus.fn_80133F6C(-1, -1) <= 0) {
                ClearHitFlag_unk(p, 0x2000000);
            }
        }
        if (p->m64 != 1 || (p->m94 & 0x8000) == 0x8000) {
            attacker->mStatus.mEntries.mCount = 0;
            ClearHitFlag_unk(p, 0x2000000);
        }
    }
    if (attacker != 0 && (hit->mAC & 8)) {
        HitDamage_unk damage;
        damage.m04 = -(int)(0.5f * p->m68);
        attacker->mStatus.fn_8012DF7C(damage.m04, 0, 0);
        if (p->m90 != 0) {
            atn::Vector3 pos = HitPos_unk(attacker, p->m40);
            pos += atn::Vector3(0.0f, 15.0f, 0.0f);
            fn_80108C10(this, &damage, &pos, attacker, attacker, 0, 0);
        }
    }
    if (attacker != 0 && attacker->mFlag28 && p->m68 > 0 && lbl_8087F0A8->mHateAbsorbHpRate > 0.0f) {
        int heal = -(int)(p->m68 * lbl_8087F0A8->mHateAbsorbHpRate);
        HitDamage_unk damage;
        damage.m04 = heal;
        attacker->mStatus.fn_8012DF7C(heal, 0, 0);
        if (lbl_8087F0A8->mShowPlayerUI != 0 && p->m90 != 0) {
            if (heal != m3E2F8) {
                m3E2F4->Rewind_unk();
                m3E2F8 = heal;
            }
            wchar_t num[8];
            fn_800DD3FC(num, L"%d", -heal);
            fn_801FEE08(&m3E2F4->mData.m10, fn_800DC6B4("hpUpNum"), num);
            fn_801FEE08(&m3E2F4->mData.m10, fn_800DC6B4("hpUpSubNum"), num);
            SetPaneValue_unk(&m3E2F4->mData.m10, "hpRate",
                             attacker->mStatus.m04 / (float)attacker->mStatus.mParamB.m84);
            m3E2F4->MeltTask();
        }
    }
    if (attacker != 0 && attacker->mStatus.fn_80133EE8(0x23) && !target->mStatus.IsState_unk(0x20)) {
        HitDamage_unk damage;
        int value = attacker->mStatus.fn_80133F18(0x23, -1);
        float rate = value > 0 ? (float)value / 100.0f : 2.0f;
        damage.m04 -= (int)(p->m68 * rate);
        target->mStatus.fn_8012DF7C(damage.m04, 0, 0);
        atn::Vector3 pos = HitPos_unk(target, p->m40);
        pos += atn::Vector3(0.0f, 10.0f, 0.0f);
        fn_80108C10(this, &damage, &pos, target, target, 0, 0);
    }
    if (attacker != 0) {
        CharaStatus* status = &attacker->mStatus;
        FixedArray_unk<CharaTask*, 16> list;
        int value = 0;
        int add = 0;
        if (status->fn_80133EE8(7)) {
            value = (int)(p->m68 * ((float)status->fn_80133F18(7, -1) / 100.0f));
            list.PushBack_unk(attacker);
        } else if (status->fn_80133EE8(8)) {
            value = (int)(p->m68 * ((float)status->fn_80133F18(8, -1) / 100.0f));
            for (PlayerTask* c = lbl_8087F8A0->mHead; c != 0; c = c->mNext) {
                if (c->CanAct_unk() && (c->m54C & 0x2000) != 0x2000) {
                    list.PushBack_unk(c);
                }
            }
        } else if (status->fn_80133EE8(9)) {
            status->fn_80133EE8(9);
            add = 1;
            list.PushBack_unk(attacker);
        } else if (status->fn_80133EE8(0x47)) {
            value = (int)((float)(status->mParamB.m84 * status->fn_80133F18(0x47, -1)) / 100.0f);
            list.PushBack_unk(attacker);
        }
        unsigned int count = list.mCount;
        if (count != 0) {
            if (value > 0) {
                HitDamage_unk damage;
                damage.m04 = -value;
                for (unsigned int i = 0; i < count; i++) {
                    CharaTask* c = list.mData[i];
                    c->mStatus.fn_8012DF7C(damage.m04, 0, 0);
                    if (p->m90 != 0) {
                        atn::Vector3 pos = HitPos_unk(c, p->m40);
                        pos += atn::Vector3(0.0f, 10.0f, 0.0f);
                        fn_80108C10(this, &damage, &pos, c, c, 0, 0);
                    }
                }
            }
            if (add > 0) {
                for (unsigned int i = 0; i < list.mCount; i++) {
                    CharaStatus& s = list.mData[i]->mStatus;
                    s.m224 = MIN(MAX(s.m224 + add, 0), s.mParamB.m98);
                }
            }
        }
    }
    if (p->m4C != 0 && p->m90 != 0) {
        atn::Vector3 pos = HitPos_unk(target, p->m40);
        pos += atn::Vector3(0.0f, 15.0f, 0.0f);
        fn_80108C10(this, (BattleDamage_unk*)&p->m64, &pos, attacker, target, p->m94, 0);
    }
    fn_801018E4(p);
    fn_80101CDC(p);
    if (lbl_8087F610 != 0) {
        m38A5C.m8C = 0;
    }
    if (m38A5C.m8C != 0) {
        int value = 0;
        if (p->m44 == 0) {
            value = 100;
            if (attacker != 0 && attacker->mType == 0 && attacker->m648 != 0 && attacker->m648->m04 != 1) {
                value = 200;
            }
            if ((m38A5C.m94 & 0x10) == 0x10) {
                value <<= 1;
            }
            if (attacker != 0 && attacker->mFlag28) {
                if (!target->mStatus.IsState_unk(0x800)) {
                    target->fn_8017A5D8(1);
                    attacker->mStatus.fn_8012F188(lbl_8087F0A8->mExplsnAddHateAtt);
                }
                int index = FindChara_unk(target);
                BattleChara_unk* entry = index >= 0 ? &mCharas[index] : 0;
                if (entry != 0) {
                    entry->m930 = lbl_8087F430 != 0 ? lbl_8087F430->m5744 : 0;
                }
            }
        } else if (p->m44 == 2) {
            if (attacker->mType == 0) {
                value = 20;
            } else {
                value = 100;
            }
        } else if (p->m44 == 1) {
            value = 10;
            if (attacker != 0 && attacker->mFlag28 && !target->mStatus.IsState_unk(0x800)) {
                target->fn_8017A5D8(1);
                attacker->mStatus.fn_8012F188(lbl_8087F0A8->mExplsnAddHateAtt);
            }
        }
        fn_80102EAC(this, target, attacker, value, -1);
        fn_80210220(lbl_8087F0A8->mParamIndex);
        int a = FindChara_unk(attacker);
        int t = FindChara_unk(target);
        if (t >= 0 && a >= 0) {
            BattleChara_unk& entry = mCharas[a];
            CharaTask* c = entry.mChara;
            if (c != 0 && c->CanAct_unk()) {
                entry.fn_8010895C(t, 0);
            }
        }
        if (attacker != 0 && attacker->mFlag28) {
            int t = FindChara_unk(target);
            int a = FindChara_unk(attacker);
            if (a >= 0) {
                if (t >= 0) {
                    mCharas[t].m6E4[a] = 0.0f;
                } else {
                    for (int i = 0; i < mCharaCount; i++) {
                        mCharas[i].m6E4[a] = 0.0f;
                    }
                }
            }
        }
    }
    if (attacker != 0 && !attacker->fn_8016F3D0(target, 1)) {
        if (p->m80 >> 31) {
            target->m13B0 = (int)attacker;
            if (attacker->mType == 3) {
                lbl_8087F490->fn_803E2110(4, attacker);
                lbl_8087F498->fn_803EA77C(attacker, 0x1C, 0xF, 1.0f, 10.0f);
            }
            for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
                if (g->m50 == 0x3A) {
                    GimmickMsg_unk msg(3);
                    msg.m10 = (int)attacker;
                    g->vf40(&msg);
                }
            }
        }
        if (p->m44 == 0 || p->m44 == 2) {
            target->mStatus.fn_8012F188(lbl_8087F0A8->mExplsnAddDamage);
        } else {
            target->mStatus.fn_8012F188(0.5f * lbl_8087F0A8->mExplsnAddDamage);
        }
        if (lbl_8087F430 != 0 && lbl_8087F430->fn_80376238()) {
            float rate = 1.0f;
            float gauge = 0.0f;
            if (attacker->mType == 0) {
                if (attacker->mFlag28) {
                    for (int i = 0; i < mCharaCount; i++) {
                        CharaTask* c = mCharas[i].mChara;
                        if (c != 0 && c->CanAct_unk() && !c->fn_8016F3D0(attacker, 1) && c->mC58.mC4 == attacker) {
                            rate += 0.25f;
                        }
                    }
                    rate = MIN(2.0f, rate);
                }
            } else if (attacker->mType == 3) {
                PlayerTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
                if (leader != 0 && leader->mFlag28) {
                    rate = 1.5f;
                }
            }
            if ((unsigned int)p->m44 <= 1) {
                gauge += m38A44;
            } else {
                gauge += m38A48;
            }
            if (p->m80 >> 31) {
                gauge += m38A4C;
            }
            if (target->mFlag22 == 1) {
                gauge += m38A54;
            }
            attacker->mStatus.fn_8012F034(gauge * rate);
            target->mStatus.fn_8012F034(m38A50);
            if ((target->mType == 0 || target->mType == 3) && (p->m80 >> 31)) {
                int kind = fn_80219558(target->m50);
                for (PlayerTask* c = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0; c != 0; c = c->mNext) {
                    float add = 0.0f;
                    int own = fn_80219558(c->m50);
                    switch (own) {
                    case 0:
                        if (kind == 1 || (unsigned int)(kind - 9) <= 1) {
                            add = 0.3f;
                        } else if (kind != own) {
                            add = 0.1f;
                        }
                        break;
                    case 1:
                    case 9:
                    case 10:
                        if (kind == 0) {
                            add = 0.3f;
                        } else if (kind != own) {
                            add = 0.1f;
                        }
                        break;
                    case 2:
                        if (kind == 0) {
                            add = 0.25f;
                        } else if (kind != own) {
                            add = 0.1f;
                        }
                        break;
                    case 4:
                        if (kind != own) {
                            add = 0.1f;
                        }
                        break;
                    case 6:
                        if (kind == 1 || kind == 9 || kind == 10 || kind == 5) {
                            add = 0.2f;
                        } else if (kind == 3) {
                            add = 0.3f;
                        } else if (kind != own) {
                            add = 0.1f;
                        }
                        break;
                    case 3:
                        if (kind == 6) {
                            add = 0.3f;
                        } else if (kind != own) {
                            add = 0.1f;
                        }
                        break;
                    case 5:
                        if (kind != own) {
                            add = 0.15f;
                        }
                        break;
                    }
                    c->mStatus.fn_8012F034(add);
                }
            }
        }
    }
    if (lbl_8087EE78 != 0 && (p->m80 >> 31)) {
        lbl_8087EE78->fn_80043BF4(1, attacker, target);
    }
    if (attacker != 0 && attacker->mType == 0) {
        if (p->m80 >> 31) {
            float add = m38A20 * (attacker->mFlag28 ? m38A38 : 1.0f);
            m38A04 = MAX(0.0f, MIN(m38A04 + add, m38A0C));
        }
    } else if (target != 0 && target->mType == 0) {
        float add = m38A24 * (target->mFlag28 ? m38A38 : 1.0f);
        m38A04 = MAX(0.0f, MIN(m38A04 + add, m38A0C));
    }
    if (attacker != 0 && !(p->m80 >> 31) && p->m44 == 0 && (target->m5C->mParam.m9C & 0x8000) == 0x8000) {
        attacker->mStatus.fn_8012F440(0x400000, 0, 0, 0, 0, 0, 1);
    }
    if (lbl_8087F610 != 0 && attacker != 0 && attacker->m648 != 0 && p->m44 == 0 && attacker->m648->m28C != 0) {
        attacker->m648->m288 = MAX(attacker->m648->m288 - 1, 0);
    }
    return 1;
}

// inferred: character of the camera targets ahead of `chara` (facing `rot->y`) with the nearest hit sphere inside
// `range` (half of it while fn_801789D8 holds, unless the target has mFlag22) and the cone `theta`, the distance
// weighted by the angle; a sphere at the reference point wins at once. `all` = 0 skips the party, `aimed` skips the
// targets with mFlagB6 and favours the enemy that targets `chara`. Two maps limit the height difference
// NONMATCHING: 97.1% - registers of the arguments permuted, the height sum with the operands swapped
CharaTask* BattleTask::fn_800FFE68(CharaTask* chara, const atn::Vector3* rot, int all, int aimed, float range,
                                    float theta) {
    if (chara == 0) {
        return 0;
    }
    float cosLimit = cos(0.5f * theta);
    float best = 1e16f;
    CharaTask* found = 0;
    int busy = chara->fn_801789D8(0, 0x440000);
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', rot->y);
    PSMTXMultVec(m, &dir, &dir);
    atn::Vector3 pos = chara->mPos;
    pos.y += chara->m5B0;
    if (chara->mFlagB9) {
        pos -= atn::Vector3(0.0f, 24.0f, -2.0f);
    }
    for (unsigned int i = 0; i < mTargets.size(); i++) {
        if (all == 0 && mTargets.mData[i]->IsPartyMember_unk()) {
            continue;
        }
        if (!fn_8010B180(chara, mTargets.mData[i])) {
            continue;
        }
        if (aimed != 0 && mTargets.mData[i]->mFlagB6) {
            continue;
        }
        if (lbl_8087F430->GetCurrentMap() != 0) {
            int type = lbl_8087F430->GetCurrentMap()->mType;
            int id = lbl_8087F430->GetCurrentMap()->mId;
            int sub = lbl_8087F430->GetCurrentMap()->mSubId;
            if (type == 2 && id == 0x11 && sub == 2 && (float)__fabs(chara->mPos.y - mTargets.mData[i]->mPos.y) > 25.0f) {
                continue;
            }
            if (type == 2 && id == 10 && sub == 2 && mTargets.mData[i]->m60->m24 == 0x27 &&
                (float)__fabs(chara->mPos.y - mTargets.mData[i]->mPos.y) > 18.0f) {
                continue;
            }
        }
        float limit = range;
        CharaTask* target = mTargets.mData[i];
        if (busy != 0 && target->mFlag22 != 1) {
            limit *= 0.5f;
        }
        for (unsigned int j = 0; j < target->m624.mSize; j++) {
            if ((target->m624.mData[j].m00 & 8) == 8) {
                continue;
            }
            atn::Vector3 d = target->m624.mData[j].m04.mCenter - pos;
            float dist = PSVECMag(&d);
            if (0.0f == dist) {
                best = dist;
                found = mTargets.mData[i];
                continue;
            }
            dist -= target->m624.mData[j].m04.mRadius;
            if (aimed != 0 && !mTargets.mData[i]->IsPartyMember_unk() && mTargets.mData[i]->mC58.mC4 == chara &&
                ((BattleCharaTask*)mTargets.mData[i])->vfE4()) {
                dist *= 0.001f;
            }
            PSVECNormalize(&d, &d);
            float dot = PSVECDotProduct(&d, &dir);
            if (dot > cosLimit && dist < limit) {
                dist += dist * (1.0f - dot);
                if (dist < best) {
                    best = dist;
                    found = mTargets.mData[i];
                }
            }
        }
    }
    return found;
}

// NONMATCHING: 99.7% - stack: the original puts the difference at 0x14 and the scaled sum at 0x20 (swapped)
// inferred: the score of an entry is its distance to the surface plus a penalty that grows away from `dir`
CharaTask* BattleTask::fn_801002DC(CharaTask* chara, const atn::Vector3* pos, const atn::Vector3* dir, int all,
                                    float range, float angle) {
    if (chara == 0) {
        return 0;
    }
    float cosLimit = cos(0.5f * angle);
    float best = 1e16f;
    CharaTask* found = 0;
    for (unsigned int i = 0; i < mTargets.size(); i++) {
        if (all == 0 && mTargets.mData[i]->IsPartyMember_unk()) {
            continue;
        }
        atn::Vector3 d = (mTargets.mData[i]->mColli.m3C + mTargets.mData[i]->mColli.m48) * 0.5f - *pos;
        float dist = PSVECMag(&d);
        if (dist < 10.0f || dist > range) {
            continue;
        }
        float score = dist - mTargets.mData[i]->m5B0;
        PSVECNormalize(&d, &d);
        float dot = PSVECDotProduct(dir, &d);
        if (dot > cosLimit) {
            score += 3000.0f * (1.0f - dot * dot * dot * dot);
            if (score < best) {
                best = score;
                found = mTargets.mData[i];
            }
        }
    }
    return found;
}

// inferred: result of the aim searches (0x18 bytes, cleared by BattleCharaTask before the call); names invented
struct BattleAim_unk {
    CharaTask* mChara;
    int m04;
    int mIndex;        // inferred: hit sphere of mChara
    atn::Vector3 mPos; // inferred: center of the sphere
};
// inferred: aim of `chara` at the nearest hit sphere of the character found ahead by fn_800FFE68 (2 * `range`,
// full turn): within `range` (on the ground plane with a height limit in multiplayer modes 0/1) and inside the cone
// `angle` unless mFlagB9 (then the reference point is 24 lower). C name, called by BattleCharaTask as extern "C"
// NONMATCHING: 97.3% - register allocation (integer and float registers permuted)
extern "C" int fn_801004FC(BattleTask* mgr, BattleAim_unk* out, CharaTask* chara, const atn::Vector3* rot,
                           float range, float angle) {
    CharaTask* target = mgr->fn_800FFE68(chara, rot, 0, 0, 2.0f * range, 6.2831855f);
    if (target == 0) {
        return 0;
    }
    atn::Vector3 center = chara->mPos;
    int found = -1;
    center.y += chara->m5B0;
    float best = 2.0f * range;
    if (chara->mFlagB9) {
        center.y -= 24.0f;
    }
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', rot->y);
    PSMTXMultVec(m, &dir, &dir);
    for (unsigned int i = 0; i < target->m624.mSize; i++) {
        if ((target->m624.mData[i].m00 & 8) == 8) {
            continue;
        }
        atn::Vector3 d = target->m624.mData[i].m04.mCenter - center;
        bool flat = false;
        if (lbl_8087F610 != 0 && lbl_8087F610->IsFreeMode_unk()) {
            flat = true;
        }
        float dist;
        bool inside;
        if (flat) {
            atn::Vector3 v(d.x, 0.0f, d.z);
            dist = PSVECMag(&v) - target->m624.mData[i].m04.mRadius;
            float dy = (float)__fabs(d.y) - chara->mStatus.mParamB.m28;
            inside = false;
            if (dist < range && dy < 0.0f) {
                inside = true;
            }
        } else {
            float dy = (float)__fabs(d.y) - range;
            if (0.0f > dy) {
                dy = 0.0f;
            }
            atn::Vector3 v(d.x, dy, d.z);
            dist = PSVECMag(&v) - target->m624.mData[i].m04.mRadius;
            inside = dist < range;
        }
        if (!inside) {
            continue;
        }
        if ((float)__fabs(PSVECSquareMag(&d)) < 1.1920929e-7f) {
            d.x = 0.0f;
            d.y = 0.0f;
            d.z = 0.1f;
        }
        PSVECNormalize(&d, &d);
        if (!chara->mFlagB9) {
            float c = cos(angle);
            if (!(PSVECDotProduct(&d, &dir) > c)) {
                continue;
            }
        }
        if (dist < best) {
            best = dist;
            found = i;
        }
    }
    if (found < 0) {
        return 0;
    }
    if (lbl_8087F610 != 0 && lbl_8087F610->m4FC != 0x1E) {
        return 0;
    }
    out->mChara = target;
    out->mIndex = found;
    out->mPos = target->m624.mData[found].m04.mCenter;
    return 1;
}

extern "C" GameUI* lbl_8087F490;

// inferred: aim of `chara` among the characters near the player (GameUI::m85C): the hit sphere ahead of the facing
// `rot->y`, within `range` and the cone `angle`, with the smallest distance weighted by the angle (a sphere at the
// center of `chara` wins at once). C name, called by BattleCharaTask as extern "C"
// NONMATCHING: 99% - integer registers of the loops (list, counters, target and index permuted)
extern "C" int fn_8010089C(BattleTask* mgr, BattleAim_unk* out, CharaTask* chara, const atn::Vector3* rot,
                           float range, float angle) {
    float cosMax;
    float best = 2.0f * range;
    FixedArray_unk<CharaTask*, 4>* list = &lbl_8087F490->m85C;
    int found = -1;
    cosMax = cos(0.5f * angle);
    float yaw = rot->y;
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    CharaTask* target = 0;
    Mtx m;
    PSMTXRotRad(m, 'y', yaw);
    PSMTXMultVec(m, &dir, &dir);
    atn::Vector3 center = chara->mPos;
    center.y += chara->m5B0;
    for (unsigned int i = 0; i < list->mCount; i++) {
        CharaTask* other = list->mData[i];
        for (unsigned int j = 0; j < other->m624.mSize; j++) {
            atn::Vector3 d = other->m624.mData[j].m04.mCenter - center;
            float dist = PSVECMag(&d);
            if (0.0f == dist || d.x * d.x + d.z * d.z < 0.00001f) {
                best = dist;
                target = list->mData[i];
                found = j;
                continue;
            }
            dist -= other->m624.mData[j].m04.mRadius;
            d.y = 0.0f;
            PSVECNormalize(&d, &d);
            float dot = PSVECDotProduct(&d, &dir);
            atn::Vector3 facing(0.0f, 0.0f, 1.0f);
            Mtx m2;
            PSMTXRotRad(m2, 'y', chara->mRot.y);
            PSMTXMultVec(m2, &facing, &facing);
            dist += 16.0f * (1.0f - PSVECDotProduct(&dir, &facing));
            if (dot > cosMax && dist < range) {
                dist += dist * (1.0f - dot);
                if (dist < best) {
                    best = dist;
                    target = list->mData[i];
                    found = j;
                }
            }
        }
    }
    if (target == 0) {
        return 0;
    }
    out->mChara = target;
    out->mIndex = found;
    out->mPos = target->m624.mData[found].m04.mCenter;
    return 1;
}

int BattleTask::fn_80100BB8(CharaTask** out, CharaTask* chara, const atn::Vector3* dir, float range, float theta) {
    return 0;
}

void BattleTask::fn_80100BC0(void* arg, void*) {
    ((BattleTask*)arg)->fn_801055A8(0);
    lbl_8087F3C0->fn_8023A614(5);
}

// inferred: items of the debug menu (atn/MindowsCommon.h; not included here because game/ResHandle.h declares
// lbl_8087EF10 and fn_8008937C with other types)
extern "C" void fn_800874C8(void* folder, const char* name, int* value, int min, int max, int step,
                            void (*func)(void*, void*), void* arg); // inferred: AddInt
extern "C" void fn_8008771C(void* folder, const char* name, float* value, float min, float max, float step,
                            void (*func)(void*, void*), void* arg); // inferred: AddFloat
extern "C" void fn_80087994(void* folder, const char* name, int* value, void (*func)(void*, void*),
                            void* arg); // inferred: AddBool

// inferred: debug menu "Battle": target lines, reticle, morale (gauge) and tension parameters
// NONMATCHING: 99.4% - only the offsets of the strings in @stringBase0 (strings of unwritten functions missing)
void BattleTask::fn_80100BF0() {
    void* folder = m38A58.Load("Battle");
    fn_800874C8(folder, "DrawHateArrow", &m367B0, 0, 3, 1, fn_80100BC0, this);
    fn_80087994(folder, "DrawHateNumber", &m367B4, 0, 0);
    fn_8008771C(folder, "ReticleBlur", &m3E3A0, 1.0f, 300.0f, 1.0f, 0, 0);
    fn_8008771C(folder, "ReticleCenterLerp", &m3E3A4, 0.0f, 1.0f, 1.0f, 0, 0);
    fn_8008771C(folder, "ReticleCenterOfs", &m3E3A8, 0.0f, 300.0f, 1.0f, 0, 0);
    void* morale = fn_8008937C(folder, "Morale");
    fn_80087994(morale, "DebugMode", &m38A08, 0, 0);
    fn_8008771C(morale, "Max", &m38A0C, 1.0f, 300.0f, 1.0f, 0, 0);
    fn_8008771C(morale, "DownSpeed", &m38A10, -100.0f, 100.0f, 0.01f, 0, 0);
    fn_8008771C(morale, "SwordAttack", &m38A14, -100.0f, 100.0f, 1.0f, 0, 0);
    fn_8008771C(morale, "CrossbowAttack", &m38A18, -100.0f, 100.0f, 1.0f, 0, 0);
    fn_8008771C(morale, "MagicAttack", &m38A1C, -100.0f, 100.0f, 1.0f, 0, 0);
    fn_8008771C(morale, "Defeat", &m38A20, -100.0f, 100.0f, 1.0f, 0, 0);
    fn_8008771C(morale, "Damage", &m38A24, -100.0f, 100.0f, 1.0f, 0, 0);
    fn_8008771C(morale, "Replay", &m38A28, -100.0f, 100.0f, 1.0f, 0, 0);
    fn_8008771C(morale, "AttractHate", &m38A2C, -100.0f, 100.0f, 1.0f, 0, 0);
    fn_8008771C(morale, "WaitMin", &m38A30, -1000.0f, 1000.0f, 1.0f, 0, 0);
    fn_8008771C(morale, "WaitMax", &m38A34, -1000.0f, 1000.0f, 1.0f, 0, 0);
    fn_8008771C(morale, "HateUpBoost", &m38A38, -1000.0f, 1000.0f, 1.0f, 0, 0);
    fn_8008771C(morale, "HateDownBoost", &m38A3C, -1000.0f, 1000.0f, 1.0f, 0, 0);
    fn_8008771C(morale, "EnemyRatio", &m38A40, -1000.0f, 1000.0f, 1.0f, 0, 0);
    void* tension = fn_8008937C(folder, "Tension");
    fn_8008771C(tension, "Attack", &m38A44, -1.0f, 1.0f, 0.01f, 0, 0);
    fn_8008771C(tension, "MagicAttack", &m38A48, -1.0f, 1.0f, 0.01f, 0, 0);
    fn_8008771C(tension, "Defeat", &m38A4C, -1.0f, 1.0f, 0.01f, 0, 0);
    fn_8008771C(tension, "Damage", &m38A50, -1.0f, 1.0f, 0.01f, 0, 0);
    fn_8008771C(tension, "Boss", &m38A54, -1.0f, 1.0f, 0.01f, 0, 0);
    fn_80087994(folder, "DrawAttackLog", &mShots.mEnabled, 0, 0);
    fn_80087994(folder, "DrawPick", &m3E390, 0, 0);
}

Bullet* BattleTask::fn_801010A0() {
    for (int i = 0; i < 256; i++) {
        if (!mBullets[i].HasFlag_unk(0x10000)) {
            return &mBullets[i];
        }
    }
    return 0;
}

HomingBullet* BattleTask::fn_801010E8() {
    for (int i = 0; i < 32; i++) {
        if (!mHomings[i].HasFlag_unk(0x10000)) {
            return &mHomings[i];
        }
    }
    return 0;
}

void BattleTask::fn_80101138() {
    for (unsigned int i = 0; i < 256; i++) {
        if (mBullets[i].HasFlag_unk(0x10000)) {
            mBullets[i].vf14(0);
        }
    }
}

void BattleTask::fn_801011B8() {
    for (unsigned int i = 0; i < 256; i++) {
        if (mBullets[i].HasFlag_unk(0x10000)) {
            mBullets[i].vf0C(0, 1);
        }
    }
    for (unsigned int i = 0; i < 32; i++) {
        if (mHomings[i].HasFlag_unk(0x10000)) {
            mHomings[i].vf0C(0, 1);
        }
    }
}

void BattleTask::fn_8010129C() {
    for (unsigned int i = 0; i < 256; i++) {
        mBullets[i].vf18();
    }
    for (unsigned int i = 0; i < 32; i++) {
        mHomings[i].vf18();
    }
}

// inferred: defined in this file (follows fn_8010129C, which calls it on every bullet)
void Bullet::vf18() {
    m04 &= ~0x10000;
}

int BattleTask::fn_80101340(CharaTask* chara) {
    int count = 0;
    for (int i = 0; i < 256; i++) {
        if (mBullets[i].HasFlag_unk(0x10000) && mBullets[i].HasFlag_unk(0x20000) && mBullets[i].m9C == chara) {
            count++;
        }
    }
    return count;
}

// inferred: hit effect of level `level` (1..) and kind `kind` (mHitEff[level - 1][kind]);
// C name, called by other files as extern "C"
extern "C" void fn_80101434(BattleTask* mgr, const atn::Vector3* pos, const atn::Vector3* rot, int level, int kind) {
    if (level <= 0 || kind < 0) {
        return;
    }
    bool flag = (unsigned)kind <= 2;
    fn_800F7FA8(fn_800F7FA0(), mgr, 0);
    if (flag) {
        fn_800F7FA0()->mSystem.m78 = 1;
    }
    fn_8023A680(fn_800F7FA0(), (EffectRes_unk*)&mgr->mHitEff[level - 1][kind], -1, 0, 0, pos, rot, 0, 0, -1, 1, 1.0f);
    if (flag) {
        fn_800F7FA0()->mSystem.m78 = 0;
    }
}

// inferred: effects and sounds of a change of the status flags of a character from `old` to `flags` at `pos`:
// effects of the poison (1), the curse (0x400000) and 0x40 states, the stone effect (0x1800000, on the model of
// `chara`), and one sound for the states gained or else for the states lost (flags 0x20 mutes it). C name, called by
// CharaStatus and chr::ControlCdnStone as extern "C"
// NONMATCHING: 98.95% - the original loads the effect manager before choosing the stone effect; string pool offsets
extern "C" void fn_80101524(BattleTask* mgr, const atn::Vector3* pos, const atn::Vector3* rot, unsigned int old,
                            unsigned int flags, CharaTask* chara) {
    if ((flags & 0x20) != 0) {
        return;
    }
    unsigned int changed = old ^ flags;
    unsigned int added = flags & changed;
    unsigned int removed = old & changed;
    added &= 0xFFDFE7DF;
    removed &= 0xFFDFE7DF;
    if (added == 0 && removed == 0) {
        return;
    }
    if (added & 1) {
        fn_80232B7C((unsigned int)mgr, 0);
        fn_8023A680(lbl_8087F3C0, (EffectRes_unk*)&mgr->mEff36594[0], -1, 0, 0, pos, rot, 0, 0, -1, 1, 1.0f);
    }
    if (added & 0x400000) {
        fn_80232B7C((unsigned int)mgr, 0);
        fn_8023A680(lbl_8087F3C0, (EffectRes_unk*)&mgr->mEff36594[1], -1, 0, 0, pos, rot, 0, 0, -1, 1, 1.0f);
    }
    if (added & 0x40) {
        fn_80232B7C((unsigned int)mgr, 0);
        fn_8023A680(lbl_8087F3C0, (EffectRes_unk*)&mgr->mEff36594[2], -1, 0, 0, pos, rot, 0, 0, -1, 1, 1.0f);
    }
    if ((added & 0x1800000) && chara != 0) {
        fn_80232B7C((unsigned int)mgr, 0);
        PlayEfp_unk((added & 0x80000) ? &mgr->mEfp365C4 : &mgr->mEfp365B8, chara->GetModel_unk());
    }
    added &= 0xFE5FE7DF;
    removed &= 0xFE5FE7DF;
    if (added & 1) {
        fn_800C344C("SE_BTCOM_113", pos, 0, -1, 1.0f);
    } else if (added & 0x400000) {
        fn_800C344C("SE_BTCOM_086", pos, 0, -1, 1.0f);
    } else if (added & 0x8000) {
        fn_800C344C("SE_BTCOM_111", pos, 0, -1, 1.0f);
    } else if (added != 0) {
        fn_800C344C("SE_BTCOM_073", pos, 0, -1, 1.0f);
    }
    if (removed & 0x400001) {
        fn_800C344C("SE_BTCOM_087", pos, 0, -1, 1.0f);
    } else if (removed & 0x40) {
        fn_800C344C("SE_BTCOM_072", pos, 0, -1, 1.0f);
    } else if (removed != 0) {
        fn_800C344C("SE_BTCOM_074", pos, 0, -1, 1.0f);
    }
}

// inferred: hit effect of the attack parameters `param` at the hit point, oriented along the hit direction: the
// guard effect of the level of the attacker when the target guards (CharaStatus flag 0x20), the effect m88 of
// kind 1, nothing for kind 2, else the normal or strong (m0C 0x80) hit effect of the level of the attacker
// NONMATCHING: 98.5% - stack slots and registers of the inlined Vector3::fn_80139F60
void BattleTask::fn_801018E4(BattleCalcParam_unk* param) {
    CharaTask* attacker = (CharaTask*)param->m00; // inferred: the field is an int (see fn_800FD658)
    CharaTask* chara = param->m04;
    bool show = false;
    if (param->m44 == 0) {
        show = true;
    } else if (param->m44 == 2 && param->m08 != 0 && (param->m08->mAC & 0x08000000) == 0x08000000) {
        show = true;
    }
    if (!show) {
        return;
    }
    atn::Vector3 rot = param->m28;
    rot.fn_80139F60();
    if (chara != 0 && (chara->mStatus.m0C & 0x20) == 0x20) {
        int level = attacker != 0 ? attacker->fn_8017A160() : 0;
        fn_80232B7C((unsigned int)this, 0);
        fn_8023A680(lbl_8087F3C0, (EffectRes_unk*)&mEff36558[level], -1, 0, 0, &param->m1C, &rot, 0, 0, -1, 1,
                    1.0f);
        return;
    }
    int kind = param->m84;
    if (kind == 2) {
        return;
    }
    if (kind == 1) {
        int effect = param->m88;
        if (effect <= 0) {
            return;
        }
        fn_80232B7C((unsigned int)this, 0);
        fn_8023A680(lbl_8087F3C0, (EffectRes_unk*)&mHitEff[effect - 1][5], -1, 0, 0, &param->m1C, &rot,
                    0, 0, -1, 1, 1.0f);
        return;
    }
    int strong = (param->m0C >> 7) & 1;
    int level = attacker != 0 ? attacker->fn_8017A160() : 0;
    fn_80232B7C((unsigned int)this, 0);
    fn_8023A680(lbl_8087F3C0, strong ? (EffectRes_unk*)&mEff36558[level] : (EffectRes_unk*)&mEff3651C[level], -1, 0,
                0, &param->m1C, &rot, 0, 0, -1, 1, 1.0f);
}

extern "C" void fn_800CB688(atn::SoundRef* sound, int arg, float pitch); // inferred: sets the pitch

// inferred: hit sound of the attack parameters `param`: for a hit of kind 0 the sound of the weapon of the attacker
// (the left one in the double attack control) with a random pitch, plus the critical (m94 0x80) and m78 sounds;
// for a bullet of kind 2 without effect the bullet sounds
void BattleTask::fn_80101CDC(BattleCalcParam_unk* param) {
    CharaTask* attacker = (CharaTask*)param->m00; // inferred: the field is an int (see fn_800FD658)
    if (param->m44 == 0) {
        if (param->m84 == 1) {
            fn_800C344C(lbl_80735DD0[17 + rand() % 3], &param->m1C, 0, -1, 1.0f);
        } else if (param->m84 == 0) {
            int sound = rand() % 3 + 2;
            float pitch = 1.0f;
            Arms* arms;
            if (attacker == 0 || attacker->m648 == 0) {
                arms = 0;
            } else {
                arms = (Arms*)attacker->m648;
                if (attacker->m55C == 6 && attacker->m560 == 4 && attacker->m64C != 0 && attacker->mControl.get() != 0 &&
                    attacker->mControl->vf30() % 2 == 1) {
                    arms = (Arms*)attacker->m64C;
                }
            }
            if (arms != 0 && arms->m274 != 0) {
                switch (arms->m274->m80 % 1000) {
                case 0x253:
                    sound = 5;
                    break;
                case 0x255:
                    sound = 10;
                    break;
                case 0x257:
                    sound = 6;
                    break;
                case 0x258:
                    sound = 9;
                    break;
                case 0x259:
                    sound = 13;
                    break;
                case 0x25A:
                    sound = 12;
                    break;
                case 0x25B:
                    sound = 11;
                    break;
                case 0x25E:
                case 0x2FD: case 0x2FE: case 0x2FF: case 0x300: case 0x301: case 0x302: case 0x303:
                    sound = 6;
                    break;
                case 0x25F:
                case 0x304: case 0x305: case 0x306: case 0x307: case 0x308: case 0x309: case 0x30A:
                    sound = 13;
                    break;
                case 0x260:
                case 0x30B: case 0x30C: case 0x30D: case 0x30E: case 0x30F: case 0x310: case 0x311:
                    sound = 13;
                    break;
                case 0x261:
                case 0x312: case 0x313: case 0x314: case 0x315: case 0x316: case 0x317: case 0x318:
                    sound = 14;
                    break;
                case 0x263: case 0x264: case 0x265:
                case 0x320: case 0x321: case 0x322: case 0x323: case 0x324: case 0x325: case 0x326: case 0x327:
                case 0x328: case 0x329: case 0x32A: case 0x32B: case 0x32C: case 0x32D: case 0x32E: case 0x32F:
                case 0x330: case 0x331: case 0x332: case 0x333: case 0x334:
                    sound = 15;
                    break;
                case 0x254:
                case 0x256:
                case 0x25C: case 0x25D:
                case 0x262:
                case 0x319: case 0x31A: case 0x31B: case 0x31C: case 0x31D: case 0x31E: case 0x31F:
                    break;
                default:
                    goto fixed; // inferred: the other weapons keep the pitch
                }
                pitch = 1.0f + (0.15f * ((rand() % 1001) / 1000.0f) - 0.075f);
            fixed:
                if (attacker->m50 == 0x4A25A) {
                    sound = 16;
                }
            }
            atn::SoundRef ref = fn_800C344C(lbl_80735DD0[sound], &param->m1C, 0, -1, 1.0f);
            fn_800CB688(&ref, 0, pitch);
            if ((param->m94 & 0x80u) == 0x80) {
                fn_800C344C(lbl_80735DD0[7], &param->m1C, 0, -1, 1.0f);
            }
            if (param->m78 >= 0) {
                fn_800C344C(lbl_80735DD0[8], &param->m1C, 0, -1, 1.0f);
            }
        }
    } else if (param->m44 == 2 && param->m08 != 0 && param->m08->mB0 <= 0) {
        if (param->m84 == 1) {
            fn_800C344C(lbl_80735DD0[20], &param->m1C, 0, -1, 1.0f);
        } else if (param->m84 == 0) {
            if (param->m08->m58 < 10.0f) {
                fn_800C344C(lbl_80735DD0[5], &param->m1C, 0, -1, 1.0f);
            } else {
                fn_800C344C(lbl_80735DD0[6], &param->m1C, 0, -1, 1.0f);
            }
        }
    }
}

void BattleTask::fn_801021B4(atn::ColliBase* obj, int a, int b) {
    m3E2E8 = obj;
    m3E2E0 = 0;
    m3E2E4 = 0;
    m3E2EC = b;
    m3E2F0 = a;
    if (obj != 0) {
        if ((obj->mFlags & 2) == 2) {
            m3E2E0 = obj->m0C;
        } else if ((obj->mFlags & 8) == 8) {
            m3E2E4 = (CharaTask*)obj->m0C;
        }
    }
}

extern "C" atn::GraphicsDevice_unk* lbl_8087EEE0;
extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg); // inferred: stick axis
extern "C" atn::Vector2_unk lbl_807C7028; // inferred: zero 2D vector (atn/Math_unk.cpp)
// inferred: playing with less than 5 ... (m50C) in multiplayer (inlined; name invented)
inline bool IsReticleShown_unk(Arena_unk* arena) {
    bool r = false;
    if (arena->m4FC == 0x1E && arena->m50C < 5) {
        r = true;
    }
    return r;
}

// inferred: reticle of the camera (mCB30, mCB60): color and blink by the target under the cursor, the outer rings
// follow the right stick (scaled by ReticleBlur), the center moves toward the opposite of the stick (ReticleCenterOfs,
// ReticleCenterLerp). In multiplayer only while playing (m4FC 0x1E) with m50C below 5. C name, called by
// BattleCharaTask as extern "C"
// NONMATCHING: 99.0% - volatile registers permuted (flag of the arena test, screen size, copies of the center)
extern "C" void fn_8010220C(BattleTask* mgr, int) {
    if (lbl_8087F610 != 0 && !IsReticleShown_unk(lbl_8087F610)) {
        return;
    }
    float cx = 0.5f * lbl_8087EEE0->mWidth;
    float cy = 0.5f * lbl_8087EEE0->mHeight;
    float r;
    float g;
    float b;
    float on = 0.0f;
    if (mgr->m3E2E0 != 0 || mgr->m3E2E4 != 0 || mgr->m3E2EC != 0) {
        if ((mgr->m3E2E0 != 0 && mgr->m3E2F0 != 0) || mgr->m3E2EC != 0) {
            r = 195.0f;
            g = 65.0f;
            b = 80.0f;
            on = 1.0f;
        } else if (mgr->m3E2E4 != 0) {
            r = 195.0f;
            g = 65.0f;
            b = 80.0f;
        } else {
            r = 24.0f;
            g = 85.0f;
            b = 243.0f;
            on = 1.0f;
        }
    } else {
        r = 104.0f;
        g = 121.0f;
        b = 166.0f;
    }
    mgr->mCB34 = 1;
    SetPaneValue_unk(&mgr->mCB30->mData.m10, "colorR", r);
    SetPaneValue_unk(&mgr->mCB30->mData.m10, "colorG", g);
    SetPaneValue_unk(&mgr->mCB30->mData.m10, "colorB", b);
    SetPaneValue_unk(&mgr->mCB30->mData.m10, "on", on);
    atn::Vector2_unk stick(0.0f, 0.0f);
    if (HomeButtonMenu::sInstance->m84 != 1) {
        stick.x = -fn_800A56A8(lbl_8087EF70, 0, 0, 0);
        stick.y = fn_800A56A8(lbl_8087EF70, 0, 1, 0);
    }
    SetPaneValue_unk(&mgr->mCB30->mData.m10, "posOuter2", 0,
                                   mgr->mCB40.x + (0.75f * (mgr->mCB38.x * mgr->m3E3A0) + (stick.x * mgr->m3E3A0 + cx)));
    SetPaneValue_unk(&mgr->mCB30->mData.m10, "posOuter2", 1,
                                   mgr->mCB40.y + (0.75f * (mgr->mCB38.y * mgr->m3E3A0) + (stick.y * mgr->m3E3A0 + cy)));
    SetPaneValue_unk(&mgr->mCB30->mData.m10, "posOuter1", 0, mgr->mCB38.x + (0.75f * (stick.x * mgr->m3E3A0) + cx));
    SetPaneValue_unk(&mgr->mCB30->mData.m10, "posOuter1", 1, mgr->mCB38.y + (0.75f * (stick.y * mgr->m3E3A0) + cy));
    mgr->mCB40 = mgr->mCB38;
    mgr->mCB38 = stick;
    if (mgr->m3E2E0 != 0 || mgr->m3E2E4 != 0) {
        mgr->mCB50 = lbl_807C7028;
    } else {
        mgr->mCB50 = stick * -mgr->m3E3A8;
    }
    mgr->mCB48 = atn::Lerp2_unk(mgr->mCB48, mgr->mCB50, mgr->m3E3A4);
    SetPaneValue_unk(&mgr->mCB30->mData.m10, "centerPos", 0, mgr->mCB48.x);
    SetPaneValue_unk(&mgr->mCB30->mData.m10, "centerPos", 1, mgr->mCB48.y);
    SetPaneValue_unk(&mgr->mCB60->mData.m10, "centerPos", 0, 320.0f + mgr->mCB48.x);
    SetPaneValue_unk(&mgr->mCB60->mData.m10, "centerPos", 1, 224.0f + mgr->mCB48.y);
}

void BattleTask::fn_801027A4(CharaTask* chara) {
    if (FindChara_unk(chara) < 0) {
        mCharas[mCharaCount].mChara = chara;
        mCharaCount++;
    }
}

void BattleTask::fn_80102824(CharaTask* chara, PlayerTask* leader) {
    int i = FindChara_unk(chara);
    if (i >= 0) {
        mCharas[i].mChara = (CharaTask*)leader;
    }
}

int BattleTask::fn_80102890(CharaTask* chara) {
    if (chara == 0) {
        return -1;
    }
    for (int i = 0; i < mCharaCount; i++) {
        if (mCharas[i].mChara == chara) {
            return i;
        }
    }
    return -1;
}

void BattleTask::fn_801028E4(CharaTask* chara, CharaTask* other, int value) {
    int a = FindChara_unk(chara);
    int b = FindChara_unk(other);
    if (b < 0) {
        return;
    }
    if (a >= 0) {
        mCharas[a].m08[b] = value;
    } else {
        for (int i = 0; i < 0x48; i++) {
            mCharas[i].m08[b] = value;
        }
    }
}

// inferred: entry of db/HateParam_unk (0x80210220); only the field read here (the type is local to that file)
struct HateParamView_unk {
    unsigned char m00[0x34];
    float m34; // inferred: distance at which the attack hate falls to `base`
};

// inferred: hate added by an attack of `target` with damage `damage` (only from 100000 and outside multiplayer):
// 200, or with `base` >= 0 from 200 at distance 0 down to `base` at HateParam m34, for `chara` or, without it,
// for every character; the hate does not drop below 0
// NONMATCHING: 99.4% - registers of this, base and value permuted (r26-r28)
void BattleTask::fn_80102A40(CharaTask* chara, CharaTask* target, int damage, int base) {
    if (lbl_8087F610 == 0 && damage >= 100000) {
        int value = 200;
        HateParamView_unk* param = (HateParamView_unk*)fn_80210220(lbl_8087F0A8->mParamIndex);
        int a = FindChara_unk(chara);
        int b = FindChara_unk(target);
        if (b < 0) {
            return;
        }
        if (a >= 0) {
            BattleChara_unk* entry = &mCharas[a];
            if (entry->mChara != 0 && entry->mChara->CanAct_unk()) {
                if (base >= 0) {
                    atn::Vector3 d = entry->mChara->mPos - target->mPos;
                    float dist = PSVECMag(&d);
                    float t = MIN(MAX(dist / param->m34, 0.0f), 1.0f);
                    value = base + (int)((1.0f - t) * (value - base));
                }
                entry->m08[b] += value;
                if (entry->m08[b] < 0) {
                    entry->m08[b] = 0;
                }
            }
        } else {
            for (int i = 0; i < mCharaCount; i++) {
                BattleChara_unk* entry = &mCharas[i];
                if (entry->mChara != 0 && entry->mChara->CanAct_unk()) {
                    if (base >= 0) {
                        atn::Vector3 d = entry->mChara->mPos - target->mPos;
                        float dist = PSVECMag(&d);
                        float t = MIN(MAX(dist / param->m34, 0.0f), 1.0f);
                        value = base + (int)((1.0f - t) * (value - base));
                    }
                    entry->m08[b] += value;
                    if (entry->m08[b] < 0) {
                        entry->m08[b] = 0;
                    }
                }
            }
        }
    }
}

// NONMATCHING: 99.6% - registers of the entry and of the list pointer swapped (r29/r30)
// inferred: adds hate `value` of `chara` toward `attacker`, and of the characters that follow `chara` (mode 0x11)
// on its side; a player hit outside multiplayer rounds the value; the last argument is not read (-1 from
// Em101ElderZesya). C name, called by other files as extern "C"
extern "C" void fn_80102EAC(BattleTask* mgr, CharaTask* chara, CharaTask* attacker, int value, int) {
    fn_80210220(lbl_8087F0A8->mParamIndex);
    if (attacker != 0 && attacker->mType == 0 && lbl_8087F610 == 0) {
        value = (int)(value * 1.0f);
    }
    int a = mgr->FindChara_unk(chara);
    int b = mgr->FindChara_unk(attacker);
    if (b >= 0 && a >= 0) {
        BattleChara_unk* entry = &mgr->mCharas[a];
        if (entry->mChara != 0 && entry->mChara->CanAct_unk()) {
            entry->fn_801088B8(b, value);
            BattleChara_unk* p = mgr->mCharas;
            for (int i = 0; i < mgr->mCharaCount; i++) {
                if (p->mChara != 0 && p->mChara->CanAct_unk() && p->mChara->fn_8016F3D0(chara, 1) &&
                    p->mChara->mC58.mMode == 0x11 && p->mChara->mC58.mCC == chara) {
                    p->fn_801088B8(b, value);
                }
                p++;
            }
        }
        if (entry->m804 > 0 && entry->mChara->m55C == 6 && entry->mChara->m560 == 0x60) {
            entry->m804 = 0;
            entry->mChara->fn_8016E970(entry->mChara->m564);
        }
    }
}

void BattleTask::fn_801031D0(CharaTask* chara, CharaTask* other, int all) {
    fn_80210220(lbl_8087F0A8->mParamIndex);
    int a = FindChara_unk(chara);
    int b = FindChara_unk(other);
    if (b >= 0 && a >= 0) {
        BattleChara_unk* entry = &mCharas[a];
        if (entry->mChara != 0 && entry->mChara->CanAct_unk()) {
            entry->fn_8010895C(b, all);
        }
    }
}

void BattleTask::fn_8010337C(CharaTask* chara, CharaTask* other) {
    int a = FindChara_unk(chara);
    int b = FindChara_unk(other);
    if (b < 0) {
        return;
    }
    if (a >= 0) {
        mCharas[a].m6E4[b] = 0.0f;
    } else {
        for (int i = 0; i < mCharaCount; i++) {
            mCharas[i].m6E4[b] = 0.0f;
        }
    }
}

// NONMATCHING: 99.5% - registers of the index of `other` and of the count base swapped
int BattleTask::fn_80103484(CharaTask* chara, CharaTask* other) {
    int b;
    int a = FindChara_unk(chara);
    b = FindChara_unk(other);
    if (b < 0) {
        return -1;
    }
    if (a >= 0) {
        BattleChara_unk* entry = &mCharas[a];
        if ((entry->mChara->mFlags & 1) == 1) {
            return -1;
        }
        return entry->fn_801083D8(b);
    }
    BattleChara_unk* entry = mCharas;
    int sum = 0;
    int count = 0;
    for (int i = 0; i < mCharaCount; i++) {
        if ((entry->mChara->mFlags & 1) != 1) {
            sum += entry->fn_801083D8(b);
            count++;
        }
        entry++;
    }
    if (count > 0) {
        return sum / count;
    }
    return -1;
}

// inferred: chooses the target of `chara` (mC58.mC4) among the entries of mCharas by hate, keeping the current one
// unless another is hated by more than 30; the characters linked to a BattleCharaTask that already has enough
// attackers are skipped (unless no other is left). Keeps the previous target in mC58.mC8.
void BattleTask::fn_80103600(CharaTask* chara) {
    if (chara == 0) {
        return;
    }
    fn_80210220(lbl_8087F0A8->mParamIndex);
    if ((CharaTask*)chara->mC58.mC8 != chara->mC58.mC4) {
        CharaTask* prev = (CharaTask*)chara->mC58.mC8;
        if (prev != 0 && prev->fn_8016F4D8(chara)) {
            ((CharaTask*)chara->mC58.mC8)->fn_8016F5EC(chara);
        }
        chara->mFC0 = chara->mC58.mC4;
        int detect = lbl_8087F0A8->m050;
        if (detect > 0 && chara->mC58.mC4 != 0 && chara->mC58.mC8 != 0 && !chara->mFlag2) {
            chara->fn_80164F54(detect);
        }
    }
    CharaTask* current = chara->mC58.mC4;
    chara->mC58.mC8 = (int)current;
    // inferred: only the characters in battle mode choose a target (BattleCharaTask::m1454)
    if (((BattleCharaTask*)chara)->m1454 != 1) {
        return;
    }
    chara->mC58.mC4 = 0;
    bool linked = false;
    CharaTask* partner = chara->mC58.mD4 != 0 ? chara->mC58.mD4->GetChara_unk() : 0;
    if (chara->mC58.m114 == 6 && partner != 0 && partner->CanAct_unk()) {
        linked = true;
    }
    int self = FindChara_unk(chara);
    BattleChara_unk* entry = &mCharas[self];
    if (self < 0) {
        return;
    }
    if (chara->mC58.mC0 <= 0) {
        return;
    }
    if (entry->m804 > 0) {
        return;
    }
    fn_80210220(lbl_8087F0A8->mParamIndex);
    int best = -1;
    int pick = -1;
    int bestAll = -1;
    int pickAll = -1;
    bool any = false;
    for (int i = 0; i < mCharaCount; i++) {
        if (fn_80103B58(chara, mCharas[i].mChara, 1)) {
            any = true;
            break;
        }
    }
    if (chara->mType == 3 && (chara->mStatus.m0C & 0x100) == 0x100 && lbl_8087F8A0->mHead != 0 &&
        lbl_8087F8A0->mHead != chara) {
        chara->mC58.mC4 = lbl_8087F8A0->mHead;
    } else {
        for (int i = 0; i < mCharaCount; i++) {
            if (!fn_80103B58(chara, mCharas[i].mChara, any)) {
                continue;
            }
            int hate = entry->fn_801083D8(i);
            if (bestAll < hate) {
                bestAll = hate;
                pickAll = i;
            }
            CharaLink_unk* link = chara->mC58.mD4;
            if (link != 0) {
                BattleCharaTask* other = (BattleCharaTask*)mCharas[i].mChara;
                int count;
                int limit = link->m00[0x245];
                count = other->m102C;
                if (linked) {
                    limit = 1;
                }
                if (other->fn_800EEFDC()) {
                    if (other == chara->mC58.mC4) {
                        count--;
                    }
                    if (count >= limit) {
                        continue;
                    }
                }
            }
            if (best < hate) {
                best = hate;
                pick = i;
            }
        }
    }
    pick = pick >= 0 ? pick : (pickAll >= 0 ? pickAll : -1);
    if (pick < 0) {
        return;
    }
    int old = FindChara_unk(current);
    if (old >= 0 && current->CanAct_unk() && !current->IsFlag54C_unk(0x2000) && !current->fn_8016F3D0(chara, 1) &&
        !current->mFlagB9) {
        chara->mC58.mC4 = current;
        if (entry->fn_801083D8(pick) > entry->fn_801083D8(old) + 30) {
            chara->mC58.mC4 = mCharas[pick].mChara;
            m36290[pick]++;
        }
    } else {
        chara->mC58.mC4 = mCharas[pick].mChara;
        m36290[pick]++;
    }
    fn_80103E60((CharaTask*)chara->mC58.mC8);
    fn_80103E60(chara->mC58.mC4);
}

// inferred: `chara` may take `target` as its target: `target` can act, is not on its side, is not hidden or held,
// both are battle entries, `target` is not being removed (m808), still has hate value when `checkHp`, has an AI
// area of its own and, when both have an area id, the area of `chara` allows it
int BattleTask::fn_80103B58(CharaTask* chara, CharaTask* target, int checkHp) {
    if (chara == 0 || target == 0) {
        return 0;
    }
    if (!target->CanAct_unk() || target->IsFlag54C_unk(0x2000) || target->fn_8016F3D0(chara, 1) ||
        target->mFlagB9) {
        return 0;
    }
    if (target->m55C == 6 && (target->m560 == 0x11 || target->m560 == 0x3B)) {
        return 0;
    }
    int a = FindChara_unk(chara);
    int b = FindChara_unk(target);
    if (a < 0 || b < 0) {
        return 0;
    }
    if (mCharas[b].m808 > 0) {
        return 0;
    }
    if (checkHp != 0 && mCharas[b].m248 <= 0.0f) {
        return 0;
    }
    if (target->mC58.mC0 <= 0 || (target->mC58.mD8 & 1) == 1) {
        return 0;
    }
    if (target->mC58.m11C.fn_8011D1FC() && !target->mC58.m11C.fn_8011D21C()) {
        return 0;
    }
    int area = chara->mC58.mB4;
    if (area > 0 && target->mC58.mB4 > 0 && area != target->mC58.mB4) {
        if (chara->mC58.m114 == 1 || chara->mC58.m114 == 6) {
            return 0;
        }
        gs::AiArea* ai = lbl_8087F430->m10D8->fn_803C1884(area);
        if (ai != 0 && ai->m28 >= 0 && ai->m28 != target->mC58.mB4) {
            return 0;
        }
    }
    return 1;
}

// NONMATCHING: 99.3% - registers of the entry pointer and of the flags of the inlined CanAct_unk swapped
void BattleTask::fn_80103E60(CharaTask* target) {
    if (target == 0) {
        return;
    }
    CharaTask* c;
    BattleChara_unk* p;
    int count = 0;
    p = mCharas;
    for (int i = 0; i < mCharaCount; i++, p++) {
        c = p->mChara;
        if (c != 0 && c->CanAct_unk() && (c->m54C & 0x2000) != 0x2000 && (c->mType == 0 || c->mC58.m00 != 0) &&
            c->mC58.mC4 == target) {
            count++;
        }
    }
    target->m102C = count;
}

BattleChara_unk* BattleTask::fn_80103F60(CharaTask* chara) {
    int i = FindChara_unk(chara);
    if (i >= 0) {
        return &mCharas[i];
    }
    return 0;
}

// inferred: hate of every character of mCharas toward the others, once per frame: clears the entries of the
// characters that cannot act; for the others, the timer of the "IgnoreLongHate" settings, the distance (m24C),
// the hate by distance (m36C), the bonus of the ranged attackers (m48C/m5AC) and of the AI orders (m80C), the
// chosen target (fn_8017A5D8, hate-attract level) and the smoothed visibility m248
// NONMATCHING: 98.2% - registers of the CanAct_unk flags at the top of the loop, the count of mCharas reloaded by
// the inlined FindChara_unk, order of the loads of the linked character (rotation)
void BattleTask::fn_80103FD4() {
    HateConfig_unk* hate = fn_800F8098(lbl_8087F0A8);
    fn_80210220(lbl_8087F0A8->mParamIndex);
    if (m363B4 > 0) {
        m363B4--;
    }
    for (int i = 0; i < mCharaCount; i++) {
        CharaTask* target = 0;
        CharaTask* guard = 0;
        BattleChara_unk* entry = &mCharas[i];
        BattleMarker_unk* marker = &mMarkers.mItems[i];
        CharaTask* chara = entry->mChara;
        if (!chara->CanAct_unk() || (chara->m54C & 0x2000) == 0x2000) {
            int idx = FindChara_unk(chara);
            if (idx >= 0) {
                for (int k = 0; k < 72; k++) {
                    mCharas[k].m08[idx] = 0;
                }
            }
            if (entry->m92F) {
                entry->fn_801081D8();
            }
            bool used = marker->mFlags & 1;
            marker->m70 = 0;
            marker->mFlags = 0;
            marker->mMode = 0;
            if (used) {
                marker->mFlags |= 1;
            }
            if (entry->mChara->IsGrappled_unk()) {
                entry->m808 = 60;
            }
            entry->mChara->fn_8017A5D8(0);
        } else {
            entry->m92F = 1;
            if (entry->m808 > 0) {
                entry->m808--;
            }
            if (entry->m804 > 0) {
                if (entry->mChara->m55C == 6 && entry->mChara->m560 == 0x60) {
                    entry->m804--;
                } else {
                    entry->m804 = 0;
                }
            }
            if (hate->mIgnoreLongHate != 0 && entry->mChara != 0) {
                int kind = (signed char)entry->mChara->m5C->m120[1];
                if (hate->mIgnoreLongHateValue[kind] > 0) {
                    if (entry->mChara->mC58.mC4 != (CharaTask*)entry->mChara->mC58.mC8) {
                        entry->m6DC = 0;
                        entry->m6E0 = hate->mIgnoreLongHateValue[kind];
                        if (hate->mIgnoreLongHateRand[kind] > 0) {
                            entry->m6E0 += rand() % hate->mIgnoreLongHateRand[kind];
                        }
                    } else {
                        entry->m6DC++;
                    }
                }
            }
            for (int j = 0; j < mCharaCount; j++) {
                CharaTask* other = mCharas[j].mChara;
                if (other == 0) {
                    continue;
                }
                if (entry->m6E0 > 0) {
                    if (other == entry->mChara->mC58.mC4) {
                        entry->m6E4[j] = MIN(MAX((float)entry->m6DC / (float)entry->m6E0, 0.0f), 1.0f);
                    } else if (entry->m6E4[j] > 0.0f) {
                        entry->m6E4[j] = MIN(MAX(entry->m6E4[j] - (1.0f / 300.0f), 0.0f), 1.0f);
                    }
                } else {
                    entry->m6E4[j] = 0.0f;
                }
                atn::Vector3 d = entry->mChara->mPos - other->mPos;
                entry->m24C[j] = PSVECMag(&d);
                if (other->mFlag28) {
                    float minDist = 12000.0f;
                    float maxDist = 15000.0f;
                    float near = entry->m24C[j] - minDist;
                    if (0.0f > near) {
                        near = 0.0f;
                    }
                    float t = near / (maxDist - minDist);
                    if (1.0f < t) {
                        t = 1.0f;
                    }
                    float f = 1.0f - t;
                    if (other != entry->mChara->mC58.mC4 && entry->m6E4[j] > 0.05f) {
                        f = 0.0f;
                    }
                    if (f > 0.0f) {
                        int value = (int)(1000.0f * f + 2000.0f) + entry->m6CC;
                        float shade = (float)other->fn_80164EF4();
                        float rate = 1.0f;
                        if (lbl_8087F0A8->mTimeReset != 0) {
                            float s = shade - 240.0f;
                            if (0.0f > s) {
                                s = 0.0f;
                            }
                            s = s / 60.0f;
                            if (1.0f < s) {
                                s = 1.0f;
                            }
                            rate = 1.0f - s;
                        }
                        entry->m36C[j] = (int)((float)value * rate);
                        if (entry->mChara != 0 && (entry->mChara->mStatus.m0C & 0x800) != 0x800) {
                            if (entry->m24C[j] < (float)m363BC || entry->mChara->mC58.mC4 == other ||
                                (entry->mChara->m137C & 0x1000) == 0x1000 || lbl_8087F430->fn_80370174(10) > 0) {
                                target = other;
                            }
                        } else {
                            target = other;
                        }
                        if (1.0f == shade) {
                            entry->m6CC = (int)(70.0f * ((float)rand() / 32767.0f));
                        }
                    } else {
                        entry->m36C[j] = 0;
                    }
                } else {
                    entry->m36C[j] = 0;
                }
                if (other->mFlagB16 && entry->m24C[j] < 1.5f * (float)m363BC && entry->m930 < (unsigned int)other->m138C) {
                    guard = other;
                }
                CharaLink_unk* link = entry->mChara->mC58.mD4;
                CharaTask* linked = link != 0 ? link->GetChara_unk() : 0;
                if (linked != 0 && linked->CanAct_unk() && linked->mFlag28 &&
                    (entry->mChara->mC58.m114 == 3 || entry->mChara->mC58.m114 == 1)) {
                    atn::Vector3 v(0.0f, 0.0f, 25.0f);
                    atn::Vector3 pos = linked->mPos;
                    Mtx m;
                    PSMTXRotRad(m, 'y', linked->mRot.y);
                    PSMTXMultVec(m, &v, &v);
                    pos += v;
                    atn::Vector3 e = pos - other->mPos;
                    float s = PSVECMag(&e) - 50.0f;
                    if (0.0f > s) {
                        s = 0.0f;
                    }
                    s = s / 100.0f;
                    if (1.0f < s) {
                        s = 1.0f;
                    }
                    float f = 1.0f - s;
                    float rate = 1.0f;
                    if (f > 0.0f) {
                        int value = (int)(100.0f * f + 50.0f) + entry->m6CC;
                        entry->m36C[j] += (int)((float)value * rate);
                    }
                }
                if (other->mFB8 > 10.0f) {
                    if (lbl_8087F610 == 0) {
                        if (other->mType == 2) {
                            entry->m48C[j] = 0;
                            entry->m5AC[j] = 0;
                        } else {
                            if (other->m638 != 0) {
                                entry->m48C[j] = *(int*)other->m638->mA8;
                            } else {
                                entry->m48C[j] = 220;
                            }
                            if (entry->mChara->m648 != 0 && entry->mChara->m648->m04 != 1) {
                                entry->m48C[j] = (int)(0.4f * (float)entry->m48C[j]);
                            }
                            int kind = fn_80219558(other->m50);
                            if ((unsigned int)(kind - 9) <= 1 || kind == 1) {
                                entry->m48C[j] = (int)(0.7f * (float)entry->m48C[j]);
                            }
                            entry->m5AC[j] = 600;
                        }
                    } else {
                        entry->m48C[j] = 220;
                        entry->m5AC[j] = 200;
                    }
                }
                if (entry->m5AC[j] > 0) {
                    entry->m5AC[j]--;
                }
                if ((other->mStatus.m0C & 0x20) == 0x20) {
                    entry->m48C[j] = 0;
                    entry->m5AC[j] = 0;
                }
                if (other->mType == 0 && other->mFlag31 && !other->HasC48_unk()) {
                    entry->m48C[j] = (int)(0.8f * (float)entry->m48C[j]);
                    for (int k = 0; k < entry->mHateCount; k++) {
                        if (entry->mHateValues[k] > 0 && j == entry->mHateIds[k]) {
                            entry->mHateValues[k] = (short)(0.8f * (float)entry->mHateValues[k]);
                        }
                    }
                }
                if (entry->mChara->mC58.mMode == 0x11 && entry->mChara->mC58.mCC != 0 &&
                    entry->mChara->mC58.mCC == other->mC58.mC4) {
                    entry->m24C[j] = entry->m24C[j] * 0.5f;
                }
                entry->m80C[j] = 0;
                if (entry->mChara->mType == 3) {
                    AiTarget* redirect = lbl_8087EE68 != 0 ? lbl_8087EE68->fn_800185B4(entry->mChara, 0) : 0;
                    AiTarget* aimed = lbl_8087EE68 != 0 ? lbl_8087EE68->fn_80018394(other) : 0;
                    PlayerTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
                    if (redirect != 0 && redirect->m08 == 0 && redirect->m0C == other) {
                        entry->m80C[j] += 2000;
                    } else if (aimed != 0) {
                        entry->m80C[j] += (aimed->m2C + 1) * 200;
                    } else if (leader != 0 && (leader->mFC0 == other || other->mFDC == leader)) {
                        if (leader->mFlag28) {
                            int kind = entry->mChara->m5C != 0 ? entry->mChara->m5C->m122 : -1;
                            if (kind != 3) {
                                if (kind == 2) {
                                    entry->m80C[j] += 70;
                                } else {
                                    entry->m80C[j] += 35;
                                }
                            }
                        }
                        if (leader == other->mFDC && entry->mChara->fn_801789D8(0, 0x4000000)) {
                            entry->m80C[j] += 175;
                        }
                    }
                }
                if (guard != 0 && guard == other) {
                    entry->m80C[j] += 3000;
                }
            }
            if (target != 0 && entry->mChara->mType == 2 && (entry->mChara->m137C & 0x2000) == 0x2000 &&
                lbl_8087F430 != 0 && lbl_8087F430->GetCurrentMap() != 0 && lbl_8087F8A0 != 0) {
                MapTask* map = lbl_8087F430->GetCurrentMap();
                if (map->mType == 2 && map->mId == 13) {
                    PlayerTask* p = lbl_8087F8A0->FindByName_unk(8);
                    if (p != 0 && p->CanAct_unk() && (p->m54C & 0x2000) != 0x2000) {
                        atn::Vector3 a = entry->mChara->mPos - target->mPos;
                        float da = PSVECSquareMag(&a);
                        atn::Vector3 b = entry->mChara->mPos - p->mPos;
                        if (da > 0.8f * PSVECSquareMag(&b)) {
                            int idx = FindChara_unk(target);
                            target = 0;
                            entry->m36C[idx] = 0;
                        }
                    }
                }
            }
            if (target != 0 && guard != 0 && entry->mChara->mType == 2) {
                if ((unsigned int)target->mFD8 < (unsigned int)guard->m138C) {
                    int idx = FindChara_unk(target);
                    target = 0;
                    entry->m36C[idx] = 0;
                } else {
                    entry->m80C[FindChara_unk(guard)] = 0;
                }
            }
            if ((entry->mChara->mStatus.m0C & 0x800) == 0x800) {
                if (target == 0) {
                    entry->mChara->fn_8017A5D8(0);
                    PlayerTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
                    if (leader != 0) {
                        CharaTask* self = entry->mChara;
                        fn_80210220(lbl_8087F0A8->mParamIndex);
                        int a = FindChara_unk(self);
                        int b = FindChara_unk(leader);
                        if (b >= 0 && a >= 0) {
                            BattleChara_unk* e = &mCharas[a];
                            if (e->mChara != 0 && e->mChara->CanAct_unk()) {
                                e->fn_8010895C(b, 1);
                            }
                        }
                    }
                    entry->m930 = 0;
                }
            } else if (target != 0 && (target->m54C & 4) != 4) {
                if (entry->mChara->mType == 2) {
                    entry->mChara->fn_80165284(target);
                }
                entry->mChara->fn_8017A5D8(1);
                if ((entry->mChara->mStatus.m0C & 0x800) == 0x800) {
                    target->mStatus.fn_8012F188(lbl_8087F0A8->mExplsnAddHateAtt);
                }
                if (entry->m804 > 0 && entry->mChara->m55C == 6 && entry->mChara->m560 == 0x60) {
                    entry->m804 = 0;
                    entry->mChara->fn_8016E970(entry->mChara->m564);
                }
                entry->m930 = target->mFD8;
            }
        }
        if (entry->mChara->mType != 2 && entry->mChara->mFlag31 && !entry->mChara->HasC48_unk()) {
            entry->m248 = MIN(MAX(entry->m248 - (1.0f / 15.0f), 0.0f), 1.0f);
        } else if (entry->mChara->mType == 0 && entry->mChara->m55C == 5) {
            entry->m248 = MIN(MAX(entry->m248 - 0.013333335f, 0.0f), 1.0f);
        } else {
            entry->m248 = MIN(MAX((1.0f / 15.0f) + entry->m248, 0.0f), 1.0f);
        }
    }
}

void BattleTask::fn_80105510(int value) {
    if (value > 300) {
        if (m363BC < value) {
            m363BC = 9999;
        } else {
            m363BC = 300;
        }
    } else if (value < 20) {
        m363BC = 20;
    } else {
        m363BC = value;
    }
    if (lbl_8087F430 == 0) {
        return;
    }
    if (m363BC == 9999) {
        ((GameTask*)lbl_8087F430)->fn_80370320(10, 1, 0);
    } else {
        ((GameTask*)lbl_8087F430)->fn_80370320(10, 0, 0);
    }
}

// inferred: next character of the FriendManager list (as game/AiTarget.cpp); names invented
struct BattleFriendNode_unk {
    unsigned char m0000[0x14AC];
    BattleCharaTask* mNext;
};
inline BattleCharaTask* NextFriend_unk(BattleCharaTask* chara) {
    return ((BattleFriendNode_unk*)chara)->mNext;
}

// inferred: draws the target lines of the frame (option SetConfig::mBattleDrawHateLine: 1 only while the leader
// has mFlag28, 2 always, 3 friends and enemies but not the leader) and stops the effects of the lines not drawn
// this frame; `arg` hides the lines (mHideLines)
void BattleTask::fn_801055A8(int arg) {
    mHideLines = arg;
    for (unsigned int i = 0; i < 72; i++) {
        BattleMarker_unk* marker = &mMarkers.mItems[i];
        if ((marker->mFlags & 1) == 1) {
            marker->mFlags |= 2;
        } else {
            marker->mFlags &= ~2;
        }
        marker->mFlags &= ~1;
    }
    PlayerTask* head = lbl_8087F8A0->mHead;
    if (m367B0 > 0 && SetConfig::sInstance->mDrawHateLine != 0) {
        int mode = SetConfig::sInstance->mBattleDrawHateLine;
        if (mode == 1) {
            if (head->mFlag28) {
                if ((head->mStatus.m0C & 0x100) == 0x100) {
                    fn_800F72F4(head, 0);
                }
                for (BattleCharaTask* c = (BattleCharaTask*)FriendManager::sInstance->fn_8036554C(); c != 0;
                     c = NextFriend_unk(c)) {
                    if (c->mF14 != 0 || m38A00 != 0) {
                        fn_800F72F4(c, 0);
                    }
                }
                for (EnemyTask* c = EnemyManager::sInstance->mHead; c != 0; c = c->mNext) {
                    fn_800F72F4(c, 0);
                }
            }
        } else if (mode == 2) {
            if ((head->mStatus.m0C & 0x100) == 0x100) {
                fn_800F72F4(head, 0);
            }
            for (BattleCharaTask* c = (BattleCharaTask*)FriendManager::sInstance->fn_8036554C(); c != 0;
                 c = NextFriend_unk(c)) {
                if (c->mF14 != 0 || m38A00 != 0) {
                    fn_800F72F4(c, 0);
                }
            }
            for (EnemyTask* c = EnemyManager::sInstance->mHead; c != 0; c = c->mNext) {
                fn_800F72F4(c, 0);
            }
        } else if (mode == 3) {
            for (BattleCharaTask* c = (BattleCharaTask*)FriendManager::sInstance->fn_8036554C(); c != 0;
                 c = NextFriend_unk(c)) {
                fn_800F72F4(c, 0);
            }
            for (EnemyTask* c = EnemyManager::sInstance->mHead; c != 0; c = c->mNext) {
                fn_800F72F4(c, 0);
            }
        }
    }
    for (unsigned int i = 0; i < 72; i++) {
        BattleMarker_unk* marker = &mMarkers.mItems[i];
        if ((marker->mFlags & 2) == 2 && (marker->mFlags & 1) != 1) {
            lbl_8087F3C0->mSystem.mReplaying = 1;
            fn_80239DAC(lbl_8087F3C0, (unsigned int)marker, 0, 0);
            lbl_8087F3C0->mSystem.mReplaying = 0;
        }
    }
}

void BattleTask::fn_801058F8() {
    fn_80103FD4();
    for (PlayerTask* player = lbl_8087F8A0->mHead; player != 0; player = player->mNext) {
        fn_80103600(player);
    }
}

void BattleTask::fn_80105950(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 0);
    PlayEfp_unk(&mEfp363C0, model);
}

void BattleTask::fn_801059F8(CharaTask* chara) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)chara, 0, 1);
}

void BattleTask::fn_80105A08(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 0);
    PlayEfp_unk(&mEfp363CC, model);
}

void BattleTask::fn_80105AB0(void* owner, atn::Model* model, const EffectColor_unk& color) {
    fn_80232B7C((unsigned int)owner, 0);
    fn_8023A8B4(lbl_8087F3C0, &mEfp363D8, model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, 1.0f);
}

void BattleTask::fn_80105B3C(CharaTask* chara) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)chara, 0, 0);
}

void BattleTask::fn_80105B4C(void* owner, atn::Model* model, const EffectColor_unk* color) {
    fn_80232B7C((unsigned int)owner, 0);
    fn_8023A8B4(lbl_8087F3C0, &mEfp363E4, model, 0, lbl_807C7030, lbl_807C7030, *color, -1, -1, 1, 1.0f);
}

void BattleTask::fn_80105BD8(atn::Model* model) {
    lbl_8087F3C0->mSystem.m78 = 1;
    fn_80232B7C(0, 0);
    PlayEfp_unk(&mEfp364EC, model);
    lbl_8087F3C0->mSystem.m78 = 0;
}

void BattleTask::fn_80105C9C(CharaTask* chara, atn::Model* model) {
    fn_80232B7C((unsigned int)chara, 3);
    if (fn_80219558(chara->m50) == 2) {
        PlayEfp_unk(&mEfp38B00, model);
    } else {
        PlayEfp_unk(&mEfp38AF4, model);
    }
}

// NONMATCHING: 90% - the original loads the effect manager before the clamp of the level
void BattleTask::fn_80105DCC(CharaTask* chara, atn::Model* model, int level) {
    fn_80232B7C((unsigned int)chara, 4);
    if (fn_80219558(chara->m50) == 2) {
        PlayEfp_unk(&mEfp38B30, model);
    } else {
        PlayEfp_unk(&mEfp38B0C[0] + MIN((unsigned int)MAX(level, 0), 3u), model);
    }
}

void BattleTask::fn_80105F3C(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 4, 1);
}

void BattleTask::fn_80105F4C(CharaTask* chara, atn::Model* model) {
    fn_80232B7C((unsigned int)chara, 3);
    if (fn_80219558(chara->m50) == 2) {
        PlayEfp_unk(&mEfp38B48, model);
    } else {
        PlayEfp_unk(&mEfp38B3C, model);
    }
}

void BattleTask::fn_8010607C(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 3, 1);
}

void BattleTask::fn_8010608C(CharaTask* chara, atn::Model* model) {
    fn_80232B7C((unsigned int)chara, 3);
    if (fn_80219558(chara->m50) == 2) {
        PlayEfp_unk(&mEfp38B60, model);
    } else {
        PlayEfp_unk(&mEfp38B54, model);
    }
}

extern "C" void fn_800CB688(atn::SoundRef* sound, int arg, float pitch); // inferred: sets the pitch
extern "C" void fn_800CB518(atn::SoundRef* sound, int frames);            // inferred: fades the sound in

// inferred: level up of a status of `chara` (CharaStatus): the effect of the level (red for the party members of
// kind 2), the level up sounds (the second one a semitone higher per level) and the "level up" popup over the
// neck of the model. C name, called by game/CharaStatus as extern "C"
// NONMATCHING: offset of "neck" in the string pool of the file (incomplete)
extern "C" void fn_801061BC(BattleTask* mgr, CharaTask* chara, atn::Model* model, int level) {
    fn_80232B7C((unsigned int)chara, 30);
    lbl_8087F3C0->mSystem.m78 = 1;
    if (fn_80219558(chara->m50) == 2) {
        fn_8023A8B4(lbl_8087F3C0, &mgr->mEfp38B90[0] + level, model, 0, lbl_807C7030, lbl_807C7030,
                    EffectColor_unk(1.0f, 0.0f, 0.0f, 1.0f), -1, -1, 1, 1.0f);
    } else {
        PlayEfp_unk(&mgr->mEfp38B6C[0] + level, model);
    }
    lbl_8087F3C0->mSystem.m78 = 0;
    if (level >= 1) {
        fn_800C31F4(lbl_80735DD0[24], 0, -1, 1.0f);
        atn::SoundRef sound = fn_800C31F4(lbl_80735DD0[25], 0, -1, 0.75f);
        float pitch = 1.0f;
        for (int i = 0; i < level; i++) {
            pitch *= 1.122462f;
        }
        fn_800CB688(&sound, 0, pitch);
        fn_800CB518(&sound, 10);
    }
    if (lbl_8087F8A8 != 0 && level >= 1) {
        wchar_t text[2] = L"";
        atn::Vector3 pos(0.0f, 3.0f, 0.0f);
        if (model != 0 && model->GetNodeMtx_unk("neck") != 0) {
            // inferred: the node matrix is read as an atn::Matrix3x4
            pos += ((atn::Matrix3x4*)model->GetNodeMtx_unk("neck"))->GetTrans_unk();
        }
        lbl_8087F8A8->fn_8054D794(6, text, &pos, level == 2, 0, 1, 0, 0, 0, 1, -1, -1);
    }
}

void BattleTask::fn_8010652C(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 30, 1);
}

void BattleTask::fn_8010653C(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 6);
    PlayEfp_unk(&mEfp38C50, model);
}

void BattleTask::fn_801065E4(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 6, 1);
}

void BattleTask::fn_801065F4(CharaTask* chara, atn::Model* model, const ColorF_unk* color) {
    fn_80232B7C((unsigned int)chara, 7);
    fn_8023A8B4(lbl_8087F3C0, &mEfp38C5C, model, 0, lbl_807C7030, lbl_807C7030, *(const EffectColor_unk*)color,
                -1, -1, 1, 1.0f);
}

void BattleTask::fn_80106680(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 8);
    PlayEfp_unk(&mEfp38C68, model);
}

void BattleTask::fn_80106728(int arg, atn::Model* model) {
    fn_80232B7C(0, 0);
    PlayEfp_unk(&mEfp365D0, model);
}

void BattleTask::fn_801067D0(CharaTask* chara, atn::Model* model) {
    fn_80232B7C(0, 0);
    PlayEfp_unk(&mEfp365DC, model);
}

extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path (as game/CharaTask.cpp)
extern "C" void fn_8023A108(atn::EffectManager* mgr, unsigned int owner, int group, int frame); // inferred: effect frame

// inferred: the weapon has a condition of the kinds (inlined; name invented)
inline bool HasCond_unk(Arms* arms, int kind, int kind2) {
    return arms->fn_80045694(kind, -1, kind2, -1) != 0;
}

// inferred: effect of the attack motion of `chara` (the motion of track 0 among seven "na000_ats" motions, effect
// mEfp365E8[kind]), plus effect and sound 0x232C when the right weapon has the condition (2, 0x22); the effects of
// group 9 follow the frame and the speed of the motion. C name, called by chr::ControlAttackBase as extern "C"
// NONMATCHING: 97.7% - frame 0x10 larger (stack slots of the strings and the second color), registers r26-r28
extern "C" void fn_80106878(BattleTask* mgr, CharaTask* chara) {
    atn::Model* model = chara->GetModel_unk();
    atn::LoadHandle* handle = model->fn_80097D40(model->mTracks[0].m00);
    if (handle == 0) {
        return;
    }
    std::string name = fn_8006B174(std::string(handle->fn_80473F18()));
    int kind = 7;
    if (name == "na000_ats00_00.motion") {
        kind = 0;
    } else if (name == "na000_ats00_01.motion") {
        kind = 1;
    } else if (name == "na000_ats00_02.motion") {
        kind = 2;
    } else if (name == "na000_ats00_04.motion") {
        kind = 3;
    } else if (name == "na000_ats00_05.motion") {
        kind = 4;
    } else if (name == "na000_ats08_00.motion") {
        kind = 0;
    } else if (name == "na000_ats08_01.motion") {
        kind = 6;
    }
    if (kind == 7) {
        return;
    }
    fn_80232B7C((unsigned int)chara, 9);
    PlayEfp_unk(&mgr->mEfp365E8[kind], model);
    if (chara->m648 != 0 && HasCond_unk((Arms*)chara->m648, 2, 0x22)) {
        EffectColor_unk white(1.0f, 1.0f, 1.0f, 1.0f);
        mgr->fn_801070C8(0x232C, (unsigned int)chara, 9, model, &lbl_807C7030, &lbl_807C7030, &white, 1.0f);
        mgr->fn_80107208(0x232C, &chara->mPos, 1.0f);
    }
    if (model->mTracks[0].m08 > 0.0f) {
        fn_8023A108(lbl_8087F3C0, (unsigned int)chara, 9, (int)model->mTracks[0].m08);
    }
    lbl_8087F3C0->fn_8023A254((unsigned int)chara, 9, model->mTracks[0].m0C);
}

void BattleTask::fn_801070C8(int id, unsigned int owner, int group, atn::Model* model, const atn::Vector3* pos,
                             const atn::Vector3* rot, const EffectColor_unk* color, float scale) {
    atn::EfpFile* efp = fn_800EFBC4(id);
    if (efp != 0) {
        fn_80232B7C(owner, group);
        fn_8023A8B4(lbl_8087F3C0, efp, model, 0, *pos, *rot, *color, -1, -1, 1, scale);
    }
}

void BattleTask::fn_80107168(int id, unsigned int owner, int group, atn::Model* model, const atn::Vector3* pos,
                             const atn::Vector3* rot, const EffectColor_unk* color, float scale) {
    atn::EfpFile* efp = fn_800EFDA0(id);
    if (efp != 0) {
        fn_80232B7C(owner, group);
        fn_8023A8B4(lbl_8087F3C0, efp, model, 0, *pos, *rot, *color, -1, -1, 1, scale);
    }
}

void BattleTask::fn_80107208(int id, const atn::Vector3* pos, float scale) {
    const char* name = fn_800EFE3C(id);
    if (name != 0) {
        fn_800C344C(name, pos, 0, -1, scale);
    }
}

int BattleTask::fn_80107274(CharaTask* chara) {
    for (int i = 0; i < 8; i++) {
        if (mSlots[i].mUsed != 0 && mSlots[i].mChara == chara) {
            return 0;
        }
    }
    for (int i = 0; i < 8; i++) {
        if (mSlots[i].mUsed == 0) {
            mSlots[i].mUsed = 1;
            mSlots[i].mChara = chara;
            return 1;
        }
    }
    return 0;
}

void BattleTask::fn_80107380(CharaTask* chara) {
    for (int i = 0; i < 8; i++) {
        if (mSlots[i].mUsed != 0 && mSlots[i].mChara == chara) {
            mSlots[i].mUsed = 0;
            mSlots[i].mChara = 0;
        }
    }
}

void BattleTask::fn_8010743C(const atn::Vector3* pos, int changed, float f) {
    m3E300 = 1;
    lbl_8087F3C0->mSystem.mReplaying = 1;
    if (changed != 0) {
        fn_80239DAC(lbl_8087F3C0, (unsigned int)&m389F4, 0, 0);
    }
    if (changed != 0 || !fn_80239D58(lbl_8087F3C0, &m389F4, 0)) {
        EffectColor_unk color = m3E304;
        color.a = m3E2FC;
        fn_80232B7C((unsigned int)&m389F4, 0);
        fn_8023A680(lbl_8087F3C0, (EffectRes_unk*)&m389F4, -1, 6, (int)pos, 0, 0, &color, 0, -1, 1, f / 15.0f);
    }
    lbl_8087F3C0->mSystem.mReplaying = 0;
}

void BattleTask::fn_80107574() {
    m3E300 = 2;
}

void BattleTask::fn_80107584() {
    lbl_8087F3C0->mSystem.mReplaying = 1;
    switch (m3E300) {
    case 1: {
        m3E2FC = MIN(1.0f, m3E2FC + 0.1f);
        EffectColor_unk color;
        color.r = m3E304.r;
        color.g = m3E304.g;
        color.b = m3E304.b;
        color.a = m3E2FC;
        lbl_8087F3C0->fn_8023A184((unsigned int)&m389F4, 0, &color);
        break;
    }
    case 2: {
        m3E2FC = MAX(0.0f, m3E2FC - 0.1f);
        EffectColor_unk color;
        color.r = m3E304.r;
        color.g = m3E304.g;
        color.b = m3E304.b;
        color.a = m3E2FC;
        lbl_8087F3C0->fn_8023A184((unsigned int)&m389F4, 0, &color);
        if (m3E2FC < 0.001f) {
            fn_80239DAC(lbl_8087F3C0, (unsigned int)&m389F4, 0, 0);
            m3E300 = 0;
        }
        break;
    }
    }
    lbl_8087F3C0->mSystem.mReplaying = 0;
}

void BattleTask::fn_801076D0(void* owner, atn::Model* model) {
    if (fn_80239D58(lbl_8087F3C0, owner, 20) == 0) {
        fn_80232B7C((unsigned int)owner, 20);
        PlayEfp_unk(&mEfp38BB4, model);
    }
}

void BattleTask::fn_80107798(void* owner, int flag) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 20, flag);
}

void BattleTask::fn_801077A8(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 21);
    PlayEfp_unk(&mEfp38BC0, model);
}

void BattleTask::fn_80107850(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 21, 1);
}

void BattleTask::fn_80107860(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 22);
    PlayEfp_unk(&mEfp38BD8, model);
}

void BattleTask::fn_80107908(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 23);
    PlayEfp_unk(&mEfp38BE4, model);
}

void BattleTask::fn_801079B0(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 23, 1);
}

void BattleTask::fn_801079C0(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 24);
    PlayEfp_unk(&mEfp38BFC, model);
}

void BattleTask::fn_80107A68(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 24, 1);
}

void BattleTask::fn_80107A78(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 25);
    PlayEfp_unk(&mEfp38BCC, model);
}

void BattleTask::fn_80107B20(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 25, 1);
}

void BattleTask::fn_80107B30(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 26);
    PlayEfp_unk(&mEfp38C08, model);
}

void BattleTask::fn_80107BD8(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 26, 1);
}

void BattleTask::fn_80107BE8(void* owner, atn::Model* model) {
    lbl_8087F3C0->mSystem.mLayer = 2;
    fn_80232B7C((unsigned int)owner, 27);
    PlayEfp_unk(&mEfp38C14, model);
    lbl_8087F3C0->mSystem.mLayer = 0;
}

void BattleTask::fn_80107CA8(void* owner, int flag) {
    lbl_8087F3C0->mSystem.mLayer = 2;
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 27, flag);
    lbl_8087F3C0->mSystem.mLayer = 0;
}

void BattleTask::fn_80107CEC(void* owner, atn::Model* model, float scale) {
    fn_80232B7C((unsigned int)owner, 28);
    EffectColor_unk color(1.0f, 1.0f, 1.0f, 1.0f);
    fn_8023A8B4(lbl_8087F3C0, &mEfp38C20, model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, scale);
}

void BattleTask::fn_80107D94(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 28, 1);
}

void BattleTask::fn_80107DA4(void* owner, atn::Model* model, float scale, const atn::Vector3& pos) {
    fn_80232B7C((unsigned int)owner, 29);
    EffectColor_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, &mEfp38C2C, model, 0, pos, lbl_807C7030, color, -1, -1, 1, scale);
}

void BattleTask::fn_80107E58(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 29, 1);
}

void BattleTask::fn_80107E68(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 30);
    PlayEfp_unk(&mEfp38BF0, model);
}

void BattleTask::fn_80107F10(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 30, 1);
}

void BattleTask::fn_80107F20(void* owner, atn::Model* model, float scale) {
    fn_80232B7C((unsigned int)owner, 31);
    EffectColor_unk color(1.0f, 1.0f, 1.0f, 1.0f);
    fn_8023A8B4(lbl_8087F3C0, &mEfp38C38, model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, scale);
}

void BattleTask::fn_80107FC8(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 31, 1);
}

void BattleTask::fn_80107FD8(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 32);
    PlayEfp_unk(&mEfp38C44, model);
}

void BattleTask::fn_80108080(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 32, 1);
}

// NONMATCHING: 99% - the "" literals of the allocations are in the string pool of the file (not complete yet)
BattleChara_unk::BattleChara_unk() : mHateValues(0), mHateIds(0), mHateCount(0) {
    int count = 10;
    if (lbl_8087F610 == 0) {
        count = 5;
    }
    mHateCount = count;
    mHateValues = new ((MemoryTag)3, "", "", 0) short[mHateCount];
    mHateIds = new ((MemoryTag)3, "", "", 0) short[mHateCount];
    mChara = 0;
    m92D = 0;
    m248 = 1.0f;
    m92E = 0;
    m92F = 0;
    fn_801081D8();
}

BattleChara_unk::~BattleChara_unk() {
    if (mHateValues != 0) {
        delete[] mHateValues;
        mHateValues = 0;
    }
    if (mHateIds != 0) {
        delete[] mHateIds;
        mHateIds = 0;
    }
    mHateCount = 0;
}

void BattleChara_unk::fn_801081D8() {
    for (int i = 0; i < 72; i++) {
        m08[i] = 0;
        m128[i] = 1.0f;
        m48C[i] = 0;
        m5AC[i] = 0;
        m36C[i] = 0;
        m24C[i] = 0.0f;
        m80C[i] = 0;
        m6E4[i] = 0.0f;
    }
    for (int i = 0; i < mHateCount; i++) {
        mHateValues[i] = 0;
        mHateIds[i] = -1;
    }
    m6CC = (int)(70.0f * (rand() / 32767.0f));
    m6DC = 0;
    m6E0 = 0;
    m804 = 0;
    m808 = 0;
    m92F = 0;
    m930 = 0;
}

int BattleTask::fn_80108378(CharaTask* chara) {
    BattleTask* battle = lbl_8087F048;
    if (battle != 0) {
        if (chara == 0) {
            return -1;
        }
        for (int i = 0; i < battle->mCharaCount; i++) {
            if (battle->mCharas[i].mChara == chara) {
                return i;
            }
        }
        return -1;
    }
    return -1;
}

// inferred: character of the entry `id` of the battle manager, null when there is none (name invented)
static inline CharaTask* GetBattleChara_unk(BattleTask* battle, int id) {
    CharaTask* chara;
    if (id < 0 || battle->mCharaCount <= id) {
        chara = 0;
    } else {
        chara = battle->mCharas[id].mChara;
    }
    return chara;
}

// inferred: hate of this entry toward the entry `id`. For a friend that follows the AI (m118 1) without the 0x800
// status: the base value plus closeness to its leader (within 120) and half of the recorded hits, minus the hate
// of the players; else the base value plus the recent damage m24C, the hits, the 0x800 and m48C bonuses, minus
// the players' hate outside multiplayer, scaled by m128. Outside multiplayer the result is scaled by m248 and
// m6E4 of the other entry, plus m80C
// NONMATCHING: 93.7% - register allocation (the config in r31 in the original) and scheduling of the scale
int BattleChara_unk::fn_801083D8(int id) {
    int result = -1;
    if (id >= 0) {
    int hate = m08[id];
    fn_80210220(lbl_8087F0A8->mParamIndex);
    GameConfig_unk* config = lbl_8087F0A8;
    CharaTask* other = lbl_8087F048 != 0 ? GetBattleChara_unk(lbl_8087F048, id) : 0;
    CharaTask* chara = mChara;
    CharaTask* leader = chara->mC58.m114 == 6 ? (chara->mC58.mD4 != 0 ? chara->mC58.mD4->GetChara_unk() : 0) : chara;
    if (chara != 0 && (chara->mStatus.m0C & 0x800) != 0x800 && chara->mC58.m118 == 1 && leader != 0 &&
        leader->CanAct_unk()) {
        atn::Vector3 d = leader->mPos - other->mPos;
        float near = 120.0f;
        float dist = PSVECMag(&d);
        if (dist < near) {
            hate += (int)(3.0f * (near - dist));
        }
        for (int i = 0; i < mHateCount; i++) {
            if (mHateValues[i] > 0 && id == mHateIds[i] && dist < near) {
                hate += mHateValues[i] / 2;
            }
        }
        if (other->mType == 0) {
            hate = MAX(hate - config->mPlayerMinusHate, 0);
        }
        if (lbl_8087F048 != 0) {
            hate = (int)(hate * (lbl_8087F048->mCharas[id].m248 * (1.0f - m6E4[id])));
        }
        return hate;
    }
    float limit = 500.0f;
    if ((chara->mStatus.m0C & 0x2000) == 0x2000) {
        limit *= 0.1f;
    }
    if (chara->mC58.m118 == 1) {
        limit = 50.0f;
    }
    float damage = m24C[id];
    if (damage < 500.0f) {
        hate += (int)(100.0f * (500.0f - damage) / 500.0f);
    }
    for (int i = 0; i < mHateCount; i++) {
        if (mHateValues[i] > 0 && id == mHateIds[i] && damage < limit) {
            hate += mHateValues[i];
        }
    }
    if ((chara->mStatus.m0C & 0x800) == 0x800) {
        hate += m36C[id];
    }
    if (damage < limit && m5AC[id] > 0) {
        hate += m48C[id];
    }
    if (other->mType == 0 && lbl_8087F610 == 0 && fn_80219558(other->m50) != 2) {
        hate = MAX(hate - config->mPlayerMinusHate, 0);
    }
    int value = MAX(hate + other->mStatus.fn_80134168(0x33, -1), 0);
    value = (int)(value * m128[id] + 0.1f);
    if (lbl_8087F048 != 0 && lbl_8087F610 == 0) {
        value = (int)(value * (lbl_8087F048->mCharas[id].m248 * (1.0f - m6E4[id])));
    }
    result = value + m80C[id];
    }
    return result;
}

void BattleChara_unk::fn_801088B8(int id, int value) {
    int n = 0;
    for (int i = 0; i < mHateCount; i++) {
        if (id == mHateIds[i]) {
            n++;
        }
    }
    if (n >= 10) {
        return;
    }
    for (int i = 0; i < mHateCount - 1; i++) {
        mHateIds[i] = mHateIds[i + 1];
        mHateValues[i] = mHateValues[i + 1];
    }
    mHateIds[mHateCount - 1] = id;
    mHateValues[mHateCount - 1] = value;
}

void BattleChara_unk::fn_8010895C(int id, int all) {
    for (int i = 0; i < mHateCount; i++) {
        if (id == mHateIds[i]) {
            mHateIds[i] = -1;
            mHateValues[i] = 0;
            if (all == 0) {
                break;
            }
        }
    }
    int skip = 1;
    for (int i = 0; i < mHateCount; i++) {
        int hateId = mHateIds[i];
        if (hateId >= 0 && mHateValues[i] > 0) {
            skip = 0;
        }
        if (skip == 0 && (hateId < 0 || mHateValues[i] == 0)) {
            for (int j = i + 1; j < mHateCount; j++) {
                mHateIds[j - 1] = mHateIds[j];
                mHateValues[j - 1] = mHateValues[j];
            }
        }
    }
}

void BattleTask::fn_80108A54(CharaTask* chara) {
    fn_80108A5C(chara, &chara->mRot);
}

// NONMATCHING: 99.5% - the config pointer is allocated below the parameters in the original
void BattleTask::fn_80108A5C(CharaTask* chara, const atn::Vector3* dir) {
    CharaTask* target = chara->mFC0;
    GameConfig_unk* config = lbl_8087F0A8;
    if (target != 0) {
        if (!target->CanAct_unk()) {
            target = 0;
        } else {
            atn::Vector3 d = target->mPos - chara->mPos;
            if (d.LengthSq_unk() > config->mLockOnReleaseLen * config->mLockOnReleaseLen &&
                !(target->m55C == 6 && target->m560 == 30)) {
                target = 0;
            }
        }
    }
    if (lbl_8087F430 != 0 && lbl_8087F430->GetCurrentMap() != 0 && lbl_8087F430->GetCurrentMap()->mId == 0x3F &&
        lbl_8087F430->GetCurrentMap()->mSubId == 2) {
        target = 0;
    }
    if (target == 0) {
        CharaTask* found = fn_800FFE68(chara, dir, 0, 1, config->mLockOnLen, config->mLockOnTheta);
        if (found != 0) {
            target = found;
        }
    }
    chara->mFC0 = target;
}

// inferred: the early return as !(a) || !(b) gives the bne over a blr of the original
// inferred: damage record shown by fn_80108C10 (as StatusDamage_unk of game/CharaStatus.cpp); only the members
// used here
extern "C" int fn_804EB7C4(Arena_unk* arena, CharaTask* chara); // inferred: player of a multiplayer session
extern "C" int fn_804EB814(Arena_unk* arena, CharaTask* chara); // inferred: team of a multiplayer session
extern "C" int swprintf(wchar_t* s, unsigned int n, const wchar_t* format, ...);

// inferred: the kind of damage has a popup (inlined); name invented
inline bool IsPopupKind_unk(int kind) {
    return kind == 1 || kind == 4 || kind == 5;
}

// NONMATCHING: 97.8% - the early return for an empty damage is a bne/b pair in the original; lbl_8087F610 is reloaded
// before its mode test; the format string is in the wide string table of the file (not complete)
// inferred: shows the damage popup of `damage` dealt by `attacker` to `chara` at `pos`, unless the options hide the
// popups of that side. C name, called by other files as extern "C"
extern "C" void fn_80108C10(BattleTask* mgr, BattleDamage_unk* damage, const atn::Vector3* pos, CharaTask* attacker,
                            CharaTask* chara, unsigned int flags, int skipEmpty) {
    if (!IsPopupKind_unk(damage->m00)) {
        return;
    }
    if (skipEmpty != 0 && damage->m04 == 0 && damage->m0C == 0) {
        return;
    }
    if (lbl_8087F610 == 0) {
        if (chara != 0 && (chara->mType == 0 || chara->mType == 3) && SetConfig::sInstance->mDrawFriendPopup == 0) {
            return;
        }
        if (chara != 0 && chara->mType == 2 && SetConfig::sInstance->mDrawEnemyPopup == 0) {
            return;
        }
    } else {
        PlayerTask* head = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
        if (attacker != 0 && !attacker->mFlag2) {
            if (lbl_8087F610->m540 != 2) {
                if (head != 0) {
                    if (chara != 0 && chara->fn_8016F3D0(head, 1) && SetConfig::sInstance->mDrawFriendPopup == 0) {
                        return;
                    }
                    if (chara != 0 && !chara->fn_8016F3D0(head, 1) && SetConfig::sInstance->mDrawEnemyPopup == 0) {
                        return;
                    }
                }
            } else {
                if (fn_804EB7C4(lbl_8087F610, chara) != 0 && SetConfig::sInstance->mDrawFriendPopup == 0) {
                    return;
                }
                if (fn_804EB814(lbl_8087F610, chara) != 0 && SetConfig::sInstance->mDrawEnemyPopup == 0) {
                    return;
                }
            }
        } else if (head != 0) {
            if (attacker != 0 && !attacker->fn_8016F3D0(head, 1) && SetConfig::sInstance->mDrawFriendPopup == 0) {
                return;
            }
            if (attacker != 0 && attacker->fn_8016F3D0(head, 1) && SetConfig::sInstance->mDrawEnemyPopup == 0) {
                return;
            }
        }
    }
    int value = damage->m04;
    int recover = 0;
    if (value < 0) {
        recover = 1;
        value = -value;
    }
    int big = 1;
    if (attacker != 0 && attacker->mType == 0 && recover != 1) {
        big = 0;
    }
    if ((flags & 0x200000) == 0x200000) {
        flags &= ~8;
        if (chara != 0 && chara->mType == 0) {
            big = 0;
        }
    }
    wchar_t text[32];
    memset(text, 0, sizeof(text));
    swprintf(text, 32, L"%d", value);
    lbl_8087F8A8->fn_8054D794(recover, text, pos, 0, flags, damage->m10, damage->m0C != 0, attacker, chara, big,
                              damage->m14, damage->m18);
}


// inferred: popup with the name of the status effect `status` (one bit) at `pos`. C name, called by other files
// as extern "C"
extern "C" void fn_80108F38(BattleTask* mgr, const atn::Vector3* pos, int status) {
    const wchar_t* text = 0;
    int kind = 3;
    switch (status) {
    case 0x1:
        text = lbl_8087F1E4->Get_unk(0x41);
        break;
    case 0x400000:
        text = lbl_8087F1E4->Get_unk(0x61);
        break;
    case 0x2:
        text = lbl_8087F1E4->Get_unk(0x43);
        break;
    case 0x4:
        text = lbl_8087F1E4->Get_unk(0x45);
        break;
    case 0x10:
        text = lbl_8087F1E4->Get_unk(0x47);
        break;
    case 0x20:
        text = lbl_8087F1E4->Get_unk(0x49);
        break;
    case 0x40:
        text = lbl_8087F1E4->Get_unk(0x4B);
        break;
    case 0x80:
        text = lbl_8087F1E4->Get_unk(0x4D);
        break;
    case 0x100:
        text = lbl_8087F1E4->Get_unk(0x4F);
        break;
    case 0x4000:
        text = lbl_8087F1E4->Get_unk(0x51);
        break;
    case 0x8000:
        text = lbl_8087F1E4->Get_unk(0x53);
        break;
    case 0x10000:
        text = lbl_8087F1E4->Get_unk(0x55);
        break;
    case 0x40000:
        text = lbl_8087F1E4->Get_unk(0x57);
        break;
    case 0x80000:
        text = lbl_8087F1E4->Get_unk(0x77);
        break;
    case 0x8:
    case 0x200:
        text = lbl_8087F1E4->Get_unk(0x5B);
        break;
    case 0x800000:
        text = lbl_8087F1E4->Get_unk(0x5D);
        break;
    case 0x400:
        text = lbl_8087F1E4->Get_unk(0x5F);
        break;
    case 0x1000000:
        text = lbl_8087F1E4->Get_unk(0x63);
        break;
    case 0x2000000:
        text = lbl_8087F1E4->Get_unk(0x65);
        kind = 4;
        break;
    }
    if (text != 0) {
        lbl_8087F8A8->fn_8054D794(kind, text, pos, -0xBC, 0, 1, 0, 0, 0, 1, -1, -1);
    }
}

// inferred: weapon data (only the kind is read; its name is the entry 0xEB + kind of the message table)
struct BattleWeapon_unk {
    unsigned char m00[0xB8];
    int mKind;
};
// inferred: status effect data (its name at +0xC)
struct BattleState_unk {
    unsigned char m00[0xC];
    const wchar_t* mName;
};
// inferred: log message definitions (.rodata of this file, 0x80735AB0): message of the table, kind and flags
// passed to LogMessageTask::fn_8047961C; names invented
struct BattleLogDef_unk {
    int mMsg;
    int mKind;
    int mFlags;
};
extern "C" const BattleLogDef_unk lbl_80735AB0[];
extern "C" BattleItem_unk* fn_80211480(int id);
extern "C" BattleWeapon_unk* fn_80206C50(int id);
extern "C" BattleState_unk* fn_8021AF50(int id);
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);

// inferred: battle log message `id` about `chara` (formats of the message table, pairs of 8 bytes); `value` is the
// item, weapon, skill or status, `arg` a count. The level flag is set for the items, the weapons, the money and the
// messages 0x43/0x44. C name, called by other files as extern "C"
// NONMATCHING: 96.2% - operand order and reuse of the table lookups, the name of the character loaded later
extern "C" void fn_801092C8(BattleTask* mgr, int id, CharaTask* chara, int value, int arg) {
    if (!(lbl_8087F0A8->m06C > 0) || !(LogMessageTask::sInstance != 0)) {
        return;
    }
    int level;
    const BattleLogDef_unk* def = &lbl_80735AB0[id];
    level = 0;
    wchar_t text[0x100];
    memset(text, 0, sizeof(text));
    switch (id) {
    case 0x15:
        if (value == 0x43 || value == 0x44) {
            level = 1;
        }
        wcscpy(text, lbl_8087F1E4->GetPair_unk(value));
        break;
    case 1: {
        level = 1;
        BattleItem_unk* item = fn_80211480(value);
        if (item != 0) {
            fn_800DD3FC(text, lbl_8087F1E4->GetPair_unk(def->mMsg), item->mName, arg);
        } else {
            fn_800DD3FC(text, lbl_8087F1E4->GetPair_unk(0x17), value);
        }
        break;
    }
    case 2: {
        level = 1;
        BattleWeapon_unk* weapon = fn_80206C50(value);
        if (weapon != 0) {
            fn_800DD3FC(text, lbl_8087F1E4->GetPair_unk(def->mMsg), lbl_8087F1E4->GetPair_unk(weapon->mKind + 0xEB),
                        arg);
        } else {
            fn_800DD3FC(text, lbl_8087F1E4->GetPair_unk(0x17), value);
        }
        break;
    }
    case 3:
    case 4:
        level = 1;
        fn_800DD3FC(text, lbl_8087F1E4->GetPair_unk(def->mMsg), value);
        break;
    case 5:
        fn_800DD3FC(text, lbl_8087F1E4->GetPair_unk(def->mMsg));
        break;
    case 0xF:
        level = 1;
        fn_800DD3FC(text, lbl_8087F1E4->GetPair_unk(def->mMsg));
        break;
    case 6:
    case 7:
        return;
    case 8:
    case 9:
    case 10:
    case 11:
    case 0x16:
        fn_800DD3FC(text, lbl_8087F1E4->GetPair_unk(def->mMsg), chara->m60->m04);
        break;
    case 0xC:
    case 0xD: {
        HitInfo_unk* skill = fn_80219E6C(value);
        if (skill != 0 && chara != 0 && chara->m60 != 0) {
            fn_800DD3FC(text, lbl_8087F1E4->GetPair_unk(def->mMsg), chara->m60->m04, skill->m08);
        } else {
            fn_800DD3FC(text, lbl_8087F1E4->GetPair_unk(0x18), value);
        }
        break;
    }
    case 0xE: {
        BattleState_unk* state = fn_8021AF50(value);
        if (state != 0 && chara != 0 && chara->m60 != 0) {
            fn_800DD3FC(text, lbl_8087F1E4->GetPair_unk(def->mMsg), chara->m60->m04,
                        state->mName != 0 ? state->mName : lbl_808813D0);
        } else {
            fn_800DD3FC(text, lbl_8087F1E4->GetPair_unk(0x18), value);
        }
        break;
    }
    case 0x10:
        if (chara != 0 && chara->m60 != 0) {
            fn_800DD3FC(text, lbl_8087F1E4->GetPair_unk(def->mMsg), chara->m60->m04, value);
        }
        break;
    case 0x11:
    case 0x12:
    case 0x13:
        fn_800DD3FC(text, lbl_8087F1E4->GetPair_unk(def->mMsg), value, arg);
        break;
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x1B:
    case 0x1C:
    case 0x1D:
    case 0x1E:
    case 0x1F:
        fn_800DD3FC(text, lbl_8087F1E4->GetPair_unk(def->mMsg), chara->m60->m04, value);
        break;
    case 0x14: {
        BattleState_unk* state = fn_8021AF50(value);
        if (chara != 0 && chara->m60 != 0 && state != 0) {
            fn_800DD3FC(text, lbl_8087F1E4->GetPair_unk(def->mMsg), chara->m60->m04,
                        state->mName != 0 ? state->mName : lbl_808813D0);
        }
        break;
    }
    }
    LogMessageTask::sInstance->fn_8047961C(text, def->mKind, def->mFlags, level);
}

void BattleTask::fn_80109828(const wchar_t* text) {
    if (!(lbl_8087F0A8->m06C > 0) || !(LogMessageTask::sInstance != 0)) {
        return;
    }
    if (text != 0) {
        LogMessageTask::sInstance->fn_8047961C(text, -0x3301, 0, 0);
    }
}

extern "C" bool fn_8021BE98(wchar_t* buf, SpEffect_unk* effect); // inferred: text of a status effect

// inferred: log message "<character> <effect>" (format 0x8B of the message table)
void BattleTask::fn_80109864(SpEffect_unk* effect, CharaTask* chara) {
    if (!(lbl_8087F0A8->m06C > 0) || !(LogMessageTask::sInstance != 0)) {
        return;
    }
    wchar_t text[0x100];
    memset(text, 0, sizeof(text));
    const wchar_t* format = lbl_8087F1E4->Get_unk(0x8B);
    wchar_t name[0x100];
    memset(name, 0, sizeof(name));
    if (fn_8021BE98(name, effect)) {
        fn_800DD3FC(text, format, chara->m60->m04, name);
    }
    if (text[0] != 0) {
        LogMessageTask::sInstance->fn_8047961C(text, -0x333334, 2, 0);
    }
}

// inferred: view of the battle manager under the name the callers use (CharaTask::fn_80145334 and
// Em315Coffin call fn_8013A194()->fn_80109950(this)); `this` is not read
struct Effect_unk {
    atn::Vector3 fn_80109950(CharaTask* chara);
};

// NONMATCHING: 97.5% - register allocation (center/ring and the loop pointers, best/offset floats permuted) and
// the index loop keeps its pointer out of the loop
// inferred: place of `chara` in the ring of characters around its leader (mC58.mC4; the ring is m125C of the leader,
// entries {angle, character}): the characters between the nearest (or the player-side one) and `chara` take
// their width (vfA0) scaled to a full turn, on one side or the other; its own position when it is not in the ring
atn::Vector3 Effect_unk::fn_80109950(CharaTask* chara) {
    CharaTask* center = chara->mC58.mC4;
    if (center == 0) {
        return chara->mPos;
    }
    unsigned int count = center->m125C.mCount;
    FixedArray_unk<CharaPair_unk, 8>& ring = center->m125C;
    if (count <= 1) {
        return chara->mPos;
    }
    int found = 0;
    for (CharaPair_unk* it = ring.begin(); it != ring.end(); it++) {
        if (it->m04 == chara) {
            found = 1;
            break;
        }
    }
    if (!found) {
        return chara->mPos;
    }
    float rot = center->mRot.y;
    float best = 1e16f;
    CharaTask* nearest = 0;
    float angle = 0.0f;
    CharaPair_unk* it = ring.begin();
    for (unsigned int i = 0; i < ring.mCount; i++, it++) {
        atn::Vector3 d = it->m04->mPos - center->mPos;
        float dist = PSVECSquareMag(&d);
        CharaTask* c = it->m04;
        if (c->mType == 0) {
            angle = it->m00;
            nearest = c;
            break;
        }
        if (c->mType == 2 && c->m50 < 100) {
            angle = it->m00;
            nearest = c;
            break;
        }
        if (dist < best) {
            best = dist;
            angle = it->m00;
            nearest = c;
        }
    }
    float total = 0.0f;
    it = ring.begin();
    for (unsigned int i = 0; i < ring.mCount; i++, it++) {
        total += it->m04->vfA0();
    }
    if (total < 6.2831855f) {
        return chara->mPos;
    }
    float scale = 6.2831855f / total;
    if (nearest == chara) {
        return chara->mPos;
    }
    int index;
    for (index = (int)count - 1; index >= 0; index--) {
        if (ring.mData[index].m04 == nearest) {
            index--;
            break;
        }
    }
    float offset = 0.5f * (scale * nearest->vfA0());
    int hit = 0;
    for (;;) {
        if (index < 0) {
            index = count - 1;
        }
        CharaPair_unk& e = ring.mData[index];
        if (atn::WrapAngle_unk(angle - e.m00) < 0.0f) {
            break;
        }
        offset += 0.5f * (scale * e.m04->vfA0());
        if (e.m04 == chara) {
            hit = 1;
            break;
        }
        offset += 0.5f * (scale * e.m04->vfA0());
        index--;
    }
    if (hit) {
        atn::Vector3 v(0.0f, 0.0f, chara->m64->m30);
        atn::Matrix3x4 m;
        m.fn_8013A13C(atn::WrapAngle_unk(rot + angle - offset));
        PSMTXMultVec(m.m, &v, &v);
        atn::Vector3 pos(center->mPos.x + v.x, chara->mPos.y, center->mPos.z + v.z);
        return pos;
    }
    for (index = 0; index < (int)count; index++) {
        if (ring.mData[index].m04 == nearest) {
            index++;
            break;
        }
    }
    offset = 0.5f * (scale * nearest->vfA0());
    hit = 0;
    for (;;) {
        if (index >= (int)count) {
            index = 0;
        }
        CharaPair_unk& e = ring.mData[index];
        if (atn::WrapAngle_unk(angle - e.m00) >= 0.0f) {
            break;
        }
        offset += 0.5f * (scale * e.m04->vfA0());
        if (e.m04 == chara) {
            hit = 1;
            break;
        }
        offset += 0.5f * (scale * e.m04->vfA0());
        index++;
    }
    if (hit) {
        atn::Vector3 v(0.0f, 0.0f, chara->m64->m30);
        atn::Matrix3x4 m;
        m.fn_8013A13C(atn::WrapAngle_unk(offset + (rot + angle)));
        PSMTXMultVec(m.m, &v, &v);
        atn::Vector3 pos(center->mPos.x + v.x, chara->mPos.y, center->mPos.z + v.z);
        return pos;
    }
    return chara->mPos;
}

extern "C" void* lbl_8087EE98; // inferred: collision manager
extern "C" int fn_8004ED34(void* colli, atn::ColliResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test

// inferred: the character is in one of the busy controls (6: 4..11, 29, 30); inlined, name invented
inline bool IsBusy_unk(CharaTask* chara) {
    if (chara->m55C == 6) {
        bool busy = false;
        if (chara->m560 >= 4 && chara->m560 < 12) {
            busy = true;
        }
        if (busy || (unsigned int)(chara->m560 - 29) <= 1) {
            return true;
        }
    }
    return false;
}
// inferred: the character lost hit points; inlined, name invented
inline bool IsHurt_unk(CharaTask* chara) {
    return (float)chara->mStatus.mParamB.m84 - chara->mStatus.m04 > 0.01f;
}
// inferred: guard kinds that can be woken up; inlined, name invented
inline bool IsWakeKind_unk(int kind) {
    switch (kind) {
    case 0x17:
    case 0x48:
    case 0x45:
    case 2:
        return true;
    }
    return kind < 1;
}

// NONMATCHING: 89% - the kind test is a binary search in the original (values below 1 pass too, so it is not a
// plain switch on 0); registers of the record and of the list pointer differ
// inferred: wakes up (mC58.mC0 > 0, fn_80171DB0) the waiting enemies that are near the player, are hurt, or have
// near them an ally of their side that is busy or hurt
void BattleTask::fn_80109F7C() {
    if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 != 0 && lbl_8087F430->fn_80375184() != 0) {
        return;
    }
    PlayerTask* head = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (head == 0) {
        return;
    }
    for (int i = 0; i < mCharaCount; i++) {
        CharaTask* chara = mCharas[i].mChara;
        CharaRes_unk* res = chara->m64;
        if (!chara->CanAct_unk() || chara->mType != 2 || chara->mC58.mC0 > 0 || res == 0 ||
            chara->mC58.m11C.fn_8011D1FC() != 0 || !IsWakeKind_unk(((BattleCharaTask*)chara)->m146C)) {
            continue;
        }
        int wake = 0;
        atn::Vector3 d = head->mPos - chara->mPos;
        if (PSVECSquareMag(&d) < res->m2C * res->m2C) {
            wake = 1;
        }
        if (IsHurt_unk(chara)) {
            wake = 1;
        }
        if (wake <= 0) {
            for (int j = 0; j < mCharaCount; j++) {
                CharaTask* other = mCharas[j].mChara;
                if (other == chara || other->IsFlag_unk(4) || other->mC58.mC0 == 0) {
                    continue;
                }
                atn::Vector3 d2 = chara->mPos - other->mPos;
                if (!(PSVECSquareMag(&d2) < res->m28 * res->m28)) {
                    continue;
                }
                if (IsBusy_unk(other)) {
                    wake = 1;
                    break;
                }
                if (chara->fn_8016F3D0(other, 1) && IsHurt_unk(other)) {
                    wake = other->mC58.mC0;
                    break;
                }
            }
        }
        if (wake > 0) {
            chara->mC58.mC0 = wake;
            chara->fn_80171DB0();
        }
    }
}

// NONMATCHING: 97.1% - the original picks the kind with a branch per value (li 0; b / li 1); every form
// tried gives a preset 1 or mfcr (if/else, ternary, inline, switch); `else if (size >= 6.0f)` adds a compare
// inferred: splash when the segment from..to crosses water (attribute 0x1B); `size` picks the effect and
// its scale. C name, called by other files as extern "C"
extern "C" void fn_8010A308(BattleTask* mgr, const atn::Vector3* from, const atn::Vector3* to, float size) {
    atn::ColliResult_unk result;
    if (fn_8004ED34(lbl_8087EE98, &result, from, to, 0x80000000, 0, 0) != 0 && result.m34 != 0 &&
        *(unsigned int*)result.m34 == 0x1B) {
        int kind = 1;
        if (size < 6.0f) {
            kind = 0;
        }
        if (lbl_8087F430 != 0 && lbl_8087F430->GetCurrentMap() != 0 && lbl_8087F430->GetCurrentMap()->mType == 1) {
            kind = 2;
        }
        float scale = 1.0f;
        if (size > 8.0f) {
            scale = size / 8.0f;
        }
        fn_800F7FA8(fn_800F7FA0(), 0, 0);
        fn_8023A8B4(lbl_8087F3C0, &mgr->mSplashEfp[kind], 0, 0, result.m04, lbl_807C7030,
                    EffectColor_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, scale);
        fn_800C344C(lbl_80735DD0[23], &result.m04, 0, -1, 1.0f);
    }
}

// inferred: push_back of mStuns (inlined); name invented
inline void PushStun_unk(BattleTask* mgr, const BattleStun_unk& stun) {
    new (&mgr->mStuns[mgr->mStunCount]) BattleStun_unk(stun);
    mgr->mStunCount++;
}

// inferred: plays an EFP effect at the model with a scale (inlined); name invented
inline void PlayEfpScaled_unk(atn::EfpFile* efp, atn::Model* model, float scale) {
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, lbl_807C7030, lbl_807C7030, EffectColor_unk(1.0f, 1.0f, 1.0f, 1.0f), -1,
                -1, 1, scale);
}

// inferred: removes an entry of mStuns (inlined); name invented
inline void EraseStun_unk(BattleTask* mgr, BattleStun_unk* it) {
    for (unsigned int i = it - mgr->mStuns; i < mgr->mStunCount - 1; i++) {
        mgr->mStuns[i] = mgr->mStuns[i + 1];
    }
    mgr->mStunCount--;
}

// NONMATCHING: 95.7% - the original reloads mStunCount for the push after storing the new entry; the height is loaded in
// the other register order; string pool offsets differ (file not complete)
// inferred: the character becomes stunned: restarts its timer if it is in mStuns, else adds it, starts the first
// effect of its row, shows the popup over its neck and counts the stuns of the players (global flag 0x442). C name,
// called by game/CharaTask.cpp as extern "C"
extern "C" void fn_8010A4A8(BattleTask* mgr, CharaTask* chara) {
    for (unsigned int i = 0; i < mgr->mStunCount; i++) {
        if (mgr->mStuns[i].mChara == chara) {
            mgr->mStuns[i].mTimer = 1;
            return;
        }
    }
    if (mgr->mStunCount >= 4) {
        return;
    }
    BattleStun_unk stun;
    stun.mChara = chara;
    stun.mTimer = 1;
    stun.mStarted = 0;
    PushStun_unk(mgr, stun);
    bool enemy;
    if (lbl_8087F8A0 != 0 && lbl_8087F8A0->mHead != 0) {
        enemy = !chara->fn_8016F3D0(lbl_8087F8A0->mHead, 1);
    } else {
        enemy = chara->mType == 2;
    }
    if (chara->mStatus.IsState_unk(0x10000) || chara->mStatus.IsState_unk(0x100)) {
        enemy = !enemy;
    }
    int row = !enemy;
    float scale = MAX(1.0f, chara->mColli.m54 / 5.0f);
    fn_800F7FA8(fn_800F7FA0(), chara, 0x3E9);
    PlayEfpScaled_unk(&mgr->mStunEfp[row][0], fn_800F84C8(chara), scale);
    PopupEntry_unk e;
    e.m10 = 5;
    e.m20 = lbl_8087F1E4->Get_unk(0x7D);
    e.m60.x = 0.0f;
    e.m60.y = 3.0f;
    e.m60.z = 0.0f;
    atn::Model* model = fn_800F84C8(chara);
    e.mB0 = (int)model->GetNodeMtx_unk("neck");
    if (e.mB0 == 0) {
        e.mB0 = (int)&chara->mMotion.mMtx;
        e.m60.y += chara->m5B4;
    }
    e.m04 = 40.0f;
    e.m78 = enemy ? 0xFF6496E6 : 0xFFFF2020;
    e.m1C = 2;
    lbl_8087F8A8->fn_8054E0FC(&e);
    if (chara->mType == 0 && lbl_8087F430 != 0 && lbl_8087F430->fn_80370174(0x442) >= 7) {
        lbl_8087F430->fn_803750E4(0xB4);
    }
}

// inferred: counts down the timed effects of mStuns: starts the loop effect once, and at the end stops it, plays
// the end effect and removes the entry. The effects of the enemies of the player use the second row
void BattleTask::fn_8010A868() {
    BattleStun_unk* it = mStuns;
    while (it != mStuns + mStunCount) {
        bool enemy;
        if (lbl_8087F8A0 != 0 && lbl_8087F8A0->mHead != 0) {
            enemy = !it->mChara->fn_8016F3D0(lbl_8087F8A0->mHead, 1);
        } else {
            enemy = it->mChara->mType == 2;
        }
        if (it->mChara->mStatus.IsState_unk(0x10000) || it->mChara->mStatus.IsState_unk(0x100)) {
            enemy = !enemy;
        }
        if (it->mTimer <= 0) {
            int row = !enemy;
            float scale = MAX(1.0f, it->mChara->mColli.m54 / 5.0f);
            fn_80239DAC(lbl_8087F3C0, (unsigned int)it->mChara, 0x3E9, 0);
            fn_80239DAC(lbl_8087F3C0, (unsigned int)it->mChara, 0x3EA, 0);
            fn_800F7FA8(fn_800F7FA0(), 0, 0);
            PlayEfpScaled_unk(&mStunEfp[row][2], fn_800F84C8(it->mChara), scale);
            EraseStun_unk(this, it);
        } else {
            it->mTimer--;
            if (it->mStarted == 0 && fn_80239D58(lbl_8087F3C0, it->mChara, 0x3E9) == 0) {
                int row = !enemy;
                float scale = MAX(1.0f, it->mChara->mColli.m54 / 5.0f);
                fn_800F7FA8(fn_800F7FA0(), it->mChara, 0x3EA);
                PlayEfpScaled_unk(&mStunEfp[row][1], fn_800F84C8(it->mChara), scale);
                it->mStarted = 1;
            }
            it++;
        }
    }
}

// inferred: the 0.0f bound goes through a variable (MAX evaluates it twice); with the literal f2/f3 come out swapped
void BattleTask::fn_8010AB34() {
    float add = m38A28;
    float zero;
    m38A04 = MAX(zero = 0.0f, MIN(m38A04 + add, m38A0C));
}

// inferred: as fn_8010AB34
void BattleTask::fn_8010AB94() {
    float add = m38A2C;
    float zero;
    m38A04 = MAX(zero = 0.0f, MIN(m38A04 + add, m38A0C));
}

void BattleTask::fn_8010ABF4() {
    m38A04 = 0.0f;
    fn_8010ACCC();
    m38CB8 = m38C90;
    fn_8010AEF4();
}

void BattleTask::fn_8010AC60() {
    m38CB8.m00 = 0;
    m38CB8.m04 = 0;
    m38CB8.m08 = 0;
    m38CB8.m0C = 0;
    m38CB8.m10 = 0;
    BattleChara_unk* p = mCharas;
    for (int i = 0; i < mCharaCount; i++) {
        CharaTask* c = p->mChara;
        if (c != 0 && c->mC58.mD0 != 0) {
            c->mC58.mD0 = 0;
        }
        p->m92E = 0;
        p++;
    }
    fn_80105510(40);
}

// NONMATCHING: 99.5% - registers r5/r6 swapped (life of the player and the base of the battle task)
// inferred: totals of the battle: keeps the previous ones in m38CA4 and sums the scaled life (m00) and the
// number (m08) of the players, and the life (m04) and number (m0C) of the enemies
void BattleTask::fn_8010ACCC() {
    m38CA4 = m38C90;
    m38C90.m00 = 0;
    m38C90.m04 = 0;
    m38C90.m08 = 0;
    m38C90.m0C = 0;
    m38C90.m10 = 0;
    for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
        if (!p->IsFlag_unk(4) && p->mStatus.m224 > 0 && !p->IsFlag54C_unk(0x2000) && p->mC54 != 0) {
            float rate = (float)p->mStatus.m224 / (float)p->mStatus.mParamB.m98;
            float base = p->mStatus.mParamB.m78;
            m38C90.m08++;
            m38C90.m00 += (int)(rate * base);
        }
    }
    for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
        if ((e->CanAct_unk() || e->IsFlag54C_unk(0x80000)) && !e->IsFlag54C_unk(0x2000) && e->mC54 != 0) {
            m38C90.m04 += e->mStatus.mParamB.m78;
            if (e->mFlag22 == 1) {
                m38C90.m10 = 1;
            }
            m38C90.m0C++;
        }
    }
}

inline bool BattleSel_unk::IsValid_unk() const {
    bool valid = false;
    if (m00 > 0 && m04 > 0) {
        valid = true;
    }
    return valid;
}

// inferred: compares the totals with the stored ones: m38CCC is 1 when the enemies fell to half or the players
// doubled, 3 in the opposite case; the stored totals follow
void BattleTask::fn_8010AEF4() {
    m38CD0 = m38CCC;
    if (lbl_8087F430 != 0 && lbl_8087F430->fn_80375184() != 0) {
        if (m38C90.IsValid_unk() && m38CB8.IsValid_unk() && m38C90.m10 == 0) {
            if ((m38CB8.m0C >= 3 && m38CB8.m04 / 2 >= m38C90.m04) ||
                (m38CB8.m08 >= 2 && m38CB8.m00 * 2 <= m38C90.m00)) {
                m38CCC = 1;
                m38CB8 = m38C90;
            } else if ((m38CB8.m08 >= 3 && m38CB8.m00 / 2 >= m38C90.m00) ||
                       (m38CB8.m0C >= 2 && m38CB8.m04 * 2 <= m38C90.m04)) {
                m38CCC = 3;
                m38CB8 = m38C90;
            }
            if (!m38CB8.IsValid_unk() && m38C90.IsValid_unk()) {
                m38CB8 = m38C90;
            }
        }
    } else {
        m38CCC = 2;
        if (m38CB8.IsValid_unk()) {
            m38CB8.m00 = 0;
            m38CB8.m04 = 0;
            m38CB8.m08 = 0;
            m38CB8.m0C = 0;
            m38CB8.m10 = 0;
        }
    }
}

int BattleTask::fn_8010B180(CharaTask* a, CharaTask* b) {
    if (a == 0 || b == 0 || lbl_8087F430->fn_80376210() == 0) {
        return 1;
    }
    GameScene* scene = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
    if (scene == 0) {
        return 1;
    }
    int areaA = a->mC58.mB4;
    int areaB = b->mC58.mB4;
    if (areaA > 0 && areaB > 0 && areaA != areaB) {
        gs::AiArea* area = scene->fn_803C1884(areaA);
        if (area != 0 && area->m28 >= 0 && area->m28 != areaB) {
            return 0;
        }
    }
    return 1;
}

// inferred: scale of a bullet by level (.rodata of this file)
extern "C" const float lbl_80735ED0[16] = {0.0f,  0.2f,  4.0f,  6.0f,  8.0f,  10.0f, 12.0f, 14.0f,
                                           16.0f, 18.0f, 20.0f, 22.0f, 24.0f, 26.0f, 28.0f, 30.0f};

// inferred: scale of a bullet of level `level`, moved by the status 0x37 of the attacker of `param` (a
// positive value lowers the level, a negative one raises it); C name, called by other files as extern "C"
extern "C" float fn_8010B250(BattleTask* mgr, int level, BattleCalcParam_unk* param) {
    if (param != 0 && param->m08 != 0 && param->m00 != 0 && param->m04 != 0 && param->m44 != 1) {
        int rate = param->m04->mStatus.fn_80134168(0x37, -1);
        if (rate > 0) {
            if (rate > rand() % 100) {
                if (level > 0) {
                    int down = level - 1;
                    level = 1;
                    if (down > 1) {
                        level = down;
                    }
                } else {
                    level = 0;
                }
            }
        } else if (rate < 0) {
            if (-rate > rand() % 100) {
                level += 2;
            }
        }
    }
    return lbl_80735ED0[(level > 0 ? level : 0) < 15 ? (level > 0 ? level : 0) : 15];
}

// inferred: character of the entry `i` of mCharas, null out of range (inlined); name invented
inline CharaTask* GetCharaAt_unk(BattleTask* mgr, int i) {
    if (i < 0 || mgr->mCharaCount <= i) {
        return 0;
    }
    return mgr->mCharas[i].mChara;
}

extern "C" atn::Task* lbl_8087FA20; // inferred: town task (null outside the towns; as game/GameTask.cpp)

// inferred: weapon of `chara` used by the attack (the left one in the double attack control), as in fn_80101CDC
// (name invented)
inline Arms* AttackArms_unk(CharaTask* chara) {
    Arms* arms;
    if (chara == 0 || chara->m648 == 0) {
        arms = 0;
    } else {
        arms = (Arms*)chara->m648;
        if (chara->m55C == 6 && chara->m560 == 4 && chara->m64C != 0 && chara->mControl.get() != 0 &&
            chara->mControl->vf30() % 2 == 1) {
            arms = (Arms*)chara->m64C;
        }
    }
    return arms;
}

// inferred: the hit `hit` is one of the magic bullets that apply the status 0x3F (name invented)
inline bool IsStatusBullet_unk(HitInfo_unk* hit) {
    bool r;
    if (hit != 0 && (hit->m04 == 0xCC || hit->m04 == 0xD1 || hit->m04 == 0xD5 || hit->m04 == 0x5BC ||
                     hit->m04 == 0x658)) {
        r = true;
    } else {
        r = false;
    }
    return r;
}

// inferred: status effects given by the hit `param`: the statuses 0x30, 0x3E, 0x3F and 0x42 of the attacker (or of
// the user of the bullet, m3E398), the counter statuses 0x40 and 0x23 of the target with their chance, the
// conditions of the weapon, of the selected arrow and of the weapons 399-405; then the target loses the statuses
// its state forbids, and each status is logged. Nonzero when one of them is shown
// NONMATCHING: 98.8% - register allocation (attacker, target and their statuses permuted)
int BattleTask::fn_8010B398(BattleCalcParam_unk* param) {
    CharaTask* attacker = (CharaTask*)param->m00; // inferred: the field is an int (see fn_800FD658)
    CharaTask* chara = param->m04;
    int result = 0;
    if (attacker == 0 || chara == 0) {
        return 0;
    }
    int kind = param->m44;
    atn::Vector3 pos = param->m1C;
    unsigned int flags = param->m0C;
    Arms* arms = AttackArms_unk(attacker);
    if (arms == 0 || arms->m274 == 0) {
        return 0;
    }
    HitInfo_unk* hit = param->m08;
    CharaStatus* status = &attacker->mStatus;
    CharaStatus* target = &chara->mStatus;
    if (m3E398 != 0) {
        attacker = m3E398;
        arms = AttackArms_unk(attacker);
    }
    status->mEntries.mCount = 0;
    if (m3E39C != 0) {
        return 0;
    }
    if (param->m08 != 0 && (param->m08->mAC & 0x8000) == 0x8000 && !param->m08->m90.Check(0x2000000)) {
        return 0;
    }
    if (lbl_8087FA20 != 0) {
        return 0;
    }
    if (status->fn_80134134(0x30)) {
        status->mEntries.push_back(*status->fn_80134134(0x30)->mEntry);
    }
    if (status->fn_80134134(0x3E)) {
        status->mEntries.push_back(*status->fn_80134134(0x3E)->mEntry);
    }
    if (status->fn_80134134(0x3F) && (kind == 0 || IsStatusBullet_unk(hit))) {
        status->mEntries.push_back(*status->fn_80134134(0x3F)->mEntry);
    }
    if (status->fn_80134134(0x42) && hit != 0 && (hit->mAC & 0x2000000) == 0x2000000) {
        status->mEntries.push_back(*status->fn_80134134(0x42)->mEntry);
    }
    if (target->fn_80134134(0x40)) {
        CharaStatusRef_unk* ref = target->fn_80134134(0x40);
        int chance = target->fn_801341B8(0x40, -1);
        if (chance > rand() % 100 && hit != 0 && (hit->mAC & 4) != 4) {
            status->mEntries.push_back(*ref->mEntry);
        }
    }
    if (target->fn_80134134(0x23)) {
        for (unsigned int i = 0; i < target->mRefs.mCount; i++) {
            CharaStatusEntry_unk* entry = target->mRefs[i].mEntry;
            if (entry->mId == 0x23 && entry->m10 > rand() % 100 && (entry->mSub != 9 || (hit->mAC & 4) != 4)) {
                status->mEntries.push_back(*entry);
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        // inferred: the third argument of the condition tests is the position of the hit
        if (arms->fn_80044E98(i, kind, (int)&pos, flags, attacker, chara, hit)) {
            ArmsCond_unk* cond = &arms->m274->mC4[i];
            if (cond->m10 > rand() % 100) {
                status->mEntries.push_back(*(CharaStatusEntry_unk*)cond);
            }
        }
    }
    if (attacker->mType == 0 && attacker->fn_8014FCE8() && CrossbowMenuTask::sInstance != 0) {
        BattleItem_unk* arrow =
            fn_80211480(CrossbowMenuTask::sInstance->m4C.mData[CrossbowMenuTask::sInstance->m150]);
        if (arrow != 0) {
            ArmsCond_unk* cond = arrow->mConds;
            for (int i = 0; i < 2; i++, cond++) {
                if (Arms::fn_80044FBC(cond, kind, (int)&pos, flags, attacker, chara, 0) && cond->m10 > rand() % 100) {
                    status->mEntries.push_back(*(CharaStatusEntry_unk*)cond);
                }
            }
        }
    }
    if (arms->m274->m78 == 0) {
        int id = arms->m274->m80 % 1000;
        if (0x18F <= id && id <= 0x195) {
            ArmsCond_unk cond;
            cond.m00 = 0x3F;
            cond.m04 = 7;
            cond.m08 = 2;
            cond.m0C = 0;
            cond.m10 = 100;
            if (Arms::fn_80044FBC(&cond, kind, (int)&pos, flags, attacker, chara, hit)) {
                status->mEntries.push_back(*(CharaStatusEntry_unk*)&cond);
            }
        }
    }
    if (status->fn_80133EE8(0x11) && (target->m18 & 0x10000)) {
        status->fn_80133E24(0x11, -1);
    }
    if (status->fn_80133EE8(0x12) && ((target->m18 & 0x400000) || (target->m18 & 1) || chara->mFlag22 == 1)) {
        status->fn_80133E24(0x12, -1);
    }
    if (status->fn_80133EE8(0x13) && (target->m18 & 0x80)) {
        status->fn_80133E24(0x13, -1);
    }
    if (status->fn_80133EE8(0x14) && ((target->m18 & 1) || chara->mFlag22 == 1)) {
        status->fn_80133E24(0x14, -1);
    }
    if (status->fn_80133EE8(0x15) && (target->m18 & 0x40)) {
        status->fn_80133E24(0x15, -1);
    }
    if (status->fn_80133EE8(0x16) && (target->m18 & 0x10)) {
        status->fn_80133E24(0x16, -1);
    }
    if (status->fn_80133EE8(0x17) && (target->m18 & 0x8000)) {
        status->fn_80133E24(0x17, -1);
    }
    if (status->fn_80133EE8(0x19) && (target->m18 & 4)) {
        status->fn_80133E24(0x19, -1);
    }
    if (status->fn_80133EE8(0x18) && (target->m18 & 0x4000)) {
        status->fn_80133E24(0x18, -1);
    }
    if (status->fn_80133EE8(0x13) &&
        ((target->m0C & 0x40) == 0x40 || (target->m0C & 0x100) == 0x100 || (target->m0C & 0x4000) == 0x4000 ||
         (target->m0C & 0x10000) == 0x10000 || (target->m0C & 0x10) == 0x10)) {
        status->fn_80133E24(0x13, -1);
    }
    if (status->fn_80133EE8(0x11) &&
        ((target->m0C & 0x40) == 0x40 || (target->m0C & 0x100) == 0x100 || (target->m0C & 0x4000) == 0x4000 ||
         (target->m0C & 0x80) == 0x80 || (target->m0C & 0x10) == 0x10)) {
        status->fn_80133E24(0x11, -1);
    }
    if (status->fn_80133EE8(1) && chara->mFlag22 == 1) {
        status->fn_80133E24(1, -1);
    }
    for (unsigned int i = 0; i < status->mEntries.mCount; i++) {
        CharaStatusEntry_unk* entry = &status->mEntries[i];
        bool shown = true;
        if (entry->mSub == 0x13) {
            shown = false;
        }
        if (entry->mId == 0x22) {
            shown = false;
        }
        if (entry->mId == 0x3F) {
            shown = false;
        }
        if (entry->mSub == 0x11) {
            shown = false;
        }
        if (entry->mId == 0x3C) {
            shown = false;
        }
        if (entry->mId == 0x42 && !(param->m08 != 0 && (param->m08->mAC & 0x2000000) == 0x2000000)) {
            shown = false;
        }
        if (entry->mSub == 0x1A) {
            shown = false;
        }
        if (shown) {
            result = 1;
        }
        if (entry->mId != 0x3C) {
            fn_80109864((SpEffect_unk*)entry, attacker);
        }
    }
    return result;
}

// inferred: applies the status `ref` of an equipment condition: the conditions 0x1D, 0x1F and 0x21 go to every player
// on the side of `chara` (without the 0x2000 flag), the others to `chara` (name invented)
inline void ApplyEquipStatus_unk(CharaTask* chara, int kind, const CharaStatusRef_unk& ref) {
    if (kind == 0x1D || kind == 0x1F || kind == 0x21) {
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            if ((p->m54C & 0x2000) != 0x2000 && p->fn_8016F3D0(chara, 1)) {
                p->mStatus.fn_80133FBC(ref, 1);
            }
        }
    } else {
        chara->mStatus.fn_80133FBC(ref, 1);
    }
}

// inferred: statuses of the equipment of `chara` (a player or a kind 3 character) whose conditions hold: the two
// weapons (key: the weapon), the shield conditions 0x3D of a weapon with m78 (and in multiplayer the third part)
// and the four accessories (key: their equip_param row). C name, called by CharaTask, CosEditBase and ShopUpGrade
// as extern "C"
// NONMATCHING: 97.5% - register allocation of the loops; the accessory loop walks a pointer instead of i * 4
extern "C" void fn_8010C00C(BattleTask* mgr, CharaTask* chara) {
    if ((chara->m54C & 0x2000) == 0x2000) {
        return;
    }
    if (chara->mType != 0 && chara->mType != 3) {
        return;
    }
    chr::CharaPart_unk* weapons[2];
    weapons[0] = chara->m648;
    weapons[1] = chara->m64C;
    for (unsigned int i = 0; i < 2; i++) {
        Arms* arms = (Arms*)weapons[i];
        if (arms == 0 || arms->m274 == 0) {
            continue;
        }
        for (int j = 0; j < 2; j++) {
            if (arms->fn_80044F54(j, chara)) {
                ArmsCond_unk* cond = &arms->m274->mC4[j];
                CharaStatusRef_unk ref((CharaStatusEntry_unk*)cond, (unsigned int)arms, chara, 1, 0);
                ApplyEquipStatus_unk(chara, cond->m00, ref);
            }
        }
        if (arms->m274->m78 == 1 && chara->m674 >= 1) {
            Arms* shield = (Arms*)(chara->m678 != 0 ? chara->m678 : chara->mParts.mData[0]);
            if (shield != 0) {
                for (int j = 0; j < 2; j++) {
                    if (shield->fn_80044F54(j, chara)) {
                        ArmsData_unk* data = shield->m274;
                        if (data->mC4[j].m00 == 0x3D) {
                            CharaStatusRef_unk ref((CharaStatusEntry_unk*)&data->mC4[j], (unsigned int)data, chara, 1,
                                                   0);
                            chara->mStatus.fn_80133FBC(ref, 1);
                        }
                    }
                }
            }
            if (lbl_8087F610 != 0) {
                Arms* part = (chara->m678 == 0 && chara->mParts.mCount >= 3) ? (Arms*)chara->mParts.mData[2] : 0;
                if (part != 0 && part->m274 != 0 && part->m274->m78 == 0) {
                    for (int j = 0; j < 2; j++) {
                        if (part->fn_80044F54(j, chara)) {
                            ArmsData_unk* data = part->m274;
                            if (data->mC4[j].m00 == 0x3D) {
                                CharaStatusRef_unk ref((CharaStatusEntry_unk*)&data->mC4[j], (unsigned int)data, chara,
                                                       1, 0);
                                chara->mStatus.fn_80133FBC(ref, 1);
                            }
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        chr::CharaEquip_unk* equip = chara->m680[i];
        if (equip == 0 || equip->m414 == 0) {
            continue;
        }
        for (int j = 0; j < 2; j++) {
            // inferred: m414 is a row of equip_param.csv; its conditions mE4 have the layout of ArmsCond_unk
            ArmsCond_unk* cond = (ArmsCond_unk*)&((EquipParamEntry_unk*)equip->m414)->mE4[j];
            if (Arms::fn_80045510(cond, chara)) {
                CharaStatusRef_unk ref((CharaStatusEntry_unk*)cond, (unsigned int)equip->m414, chara, 1, 0);
                ApplyEquipStatus_unk(chara, cond->m00, ref);
            }
        }
    }
}

// inferred: hate table m391E0: for every character that can act, the value toward each other one not on its side
// (wounds of the other, nearness within 200, bonus 200 for the other kind of character, the attackers already on
// a BattleCharaTask and the party member kinds); 1 at least toward the players
// NONMATCHING: 98.98% - register allocation of the loops (rows, characters and counters permuted)
void BattleTask::fn_8010C3E4() {
    for (int i = 0; i < mCharaCount; i++) {
        CharaTask* chara = GetCharaAt_unk(this, i);
        int* row = m391E0[i].m;
        if (!chara->CanAct_unk()) {
            memset(row, 0, sizeof(m391E0[i].m));
            continue;
        }
        for (int j = 0; j < mCharaCount; j++) {
            CharaTask* other = GetCharaAt_unk(this, j);
            row[j] = 0;
            if (chara == other || (other->mFlags & 4) == 4 || other->IsFlag54C_unk(0x2000) ||
                !chara->fn_8016F3D0(other, 1) || !chara->fn_8016F3D0(other, 1)) {
                continue;
            }
            int value = 0;
            if (other->CanAct_unk()) {
                float rate = other->mStatus.mParamB.m84 > 0 ? other->mStatus.m04 / other->mStatus.mParamB.m84 : 0.0f;
                value = (int)(100.0f * (1.0f - rate));
            }
            atn::Vector3 d = other->mPos - chara->mPos;
            float dist = PSVECMag(&d);
            value += (int)(100.0f * MIN(MAX((200.0f - dist) / 200.0f, 0.0f), 1.0f));
            if (chara->m5C != 0 && other->m5C != 0) {
                int kind = chara->m5C->m11C;
                int otherKind = other->m5C->m11C;
                if (kind == 0 || kind == 2) {
                    if (otherKind != 0 && otherKind != 2 && other->mType != 0) {
                        value += 200;
                    }
                    value += (int)(100.0f * MIN(MAX(other->m102C / 3.0f, 0.0f), 1.0f));
                } else if (otherKind == 0 || otherKind == 2 || other->mType == 0) {
                    value += 200;
                }
            }
            if (chara->mType == 3 && other->mType == 3) {
                int member = fn_80219558(chara->m50);
                int otherMember = fn_80219558(other->m50);
                if (member == 1 || (unsigned int)(member - 9) <= 1) {
                    if (otherMember == 0) {
                        value += 200;
                    }
                } else if (member == 6) {
                    if (otherMember == 1 || otherMember == 9 || otherMember == 10 || otherMember == 5) {
                        value += 100;
                    } else if (otherMember == 3) {
                        value += 200;
                    }
                } else if (member == 3) {
                    if (otherMember == 6) {
                        value += 200;
                    }
                }
            }
            if (value == 0 && other->mType == 0) {
                value = 1;
            }
            row[j] = value;
        }
    }
}

CharaTask* BattleTask::fn_8010C948(CharaTask* chara) {
    int i = FindChara_unk(chara);
    if (i < 0) {
        return 0;
    }
    int best = 0;
    int found = -1;
    int* row = m391E0[i].m;
    for (int j = 0; j < mCharaCount; j++) {
        if (best < row[j]) {
            best = row[j];
            found = j;
        }
    }
    if (found < 0 || mCharaCount <= found) {
        return 0;
    }
    return mCharas[found].mChara;
}

float BattleTask::fn_8010CA24() {
    return 2.4f;
}

float BattleTask::fn_8010CA2C() {
    return 0.15f;
}

float BattleTask::fn_8010CA34() {
    return 1.5f;
}

float BattleTask::fn_8010CA3C(CharaTask* chara, float rate) {
    rate += chara->mStatus.fn_80134168(27, -1) / 100.0f;
    if (chara->mStatus.fn_80134134(46) != 0) {
        rate *= 0.7f;
    }
    if (((unsigned int)chara->mStatus.m14 & 0x2000) == 0x2000) {
        rate *= 1.5f;
        if (chara->mType == 2 && chara->mFlag5) {
            rate *= 1.5f;
        }
    }
    if (chara->mType == 0) {
        rate = MIN(2.2f, rate);
    }
    return rate;
}

float BattleTask::fn_8010CB2C(CharaTask* chara, float rate) {
    rate *= 1.0f + chara->mStatus.mParamB.m94 / 2000.0f;
    if (chara->mType == 0 || chara->mType == 3) {
        rate *= chara->fn_801789D8(0x4FBF, 0) != 0 ? 2.0f : 1.4f;
    }
    rate *= chara->mStatus.fn_8012DB04();
    rate += rate * chara->mStatus.fn_80134168(0x2B, -1) / 100.0f;
    if (((unsigned int)chara->mStatus.m14 & 0x2000) == 0x2000) {
        rate *= 2.4f;
    }
    if (chara->mType == 0 || chara->mType == 3) {
        PlayerTask* leader;
        if ((lbl_8087F430 != 0 && lbl_8087F430->fn_803761BC() == 0) ||
            (lbl_8087F8A0 != 0 && (leader = lbl_8087F8A0->mHead) != 0 &&
             (leader->mFlag28 || leader != lbl_8087F8A0->fn_8054A33C(0)))) {
            rate *= 2.0f;
        }
        if (chara->mType == 0 && lbl_8087F610 != 0) {
            rate *= 1.4f;
        }
    }
    return rate;
}

float BattleTask::fn_8010CCD0() {
    return 0.75f;
}

float BattleTask::fn_8010CCD8(CharaTask* chara) {
    if (chara == 0 || chara->mType == 2) {
        return 0.0f;
    }
    float gain = 2.0f * m38A48;
    CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (leader != 0 && leader->mFlag28) {
        gain *= 1.5f;
    }
    return gain;
}

// inferred: random roll of the status effect `kind` (2/3, 7/8, 4 and 5); names invented
int BattleTask::fn_8010CD3C(int kind, CharaTask* chara, CharaTask* other) {
    switch (kind) {
    case 2:
    case 3:
        if (chara != 0 && chara->m55C != 6 && chara->mStatus.fn_801346C8(kind, 0) && rand() % 100 < 15) {
            return 1;
        }
        break;
    case 4:
        if (chara != 0 && other != 0 && chara->mStatus.fn_801346C8(4, 0) &&
            (other->m5C != 0 ? other->m5C->mD0 : 0) > 0 && rand() % 100 < 5) {
            return 1;
        }
        break;
    case 7:
    case 8:
        if (chara != 0 && (chara->m55C == 1 || chara->m55C == 2) && (unsigned int)chara->m5C->m11C <= 2 &&
            (chara->mFlagB16 || rand() % 100 < 15)) {
            return 1;
        }
        break;
    case 5:
        if (chara != 0 && chara->mStatus.fn_801346C8(5, 0) && rand() % 100 < 30) {
            return 1;
        }
        break;
    }
    return 0;
}

extern "C" int fn_80444020(Inventory* party, int item, int a, int b, int c, int d, int* outA, int* outB,
                           int* outC); // inferred: adds an item (as in game/BattleResult.cpp)
extern "C" int fn_8021E48C(int item, int arg); // inferred: count of the item given at once

// inferred: effect `kind` of a special hit of `chara` on `target`: 4 steals the item of `target` once (message, sound
// and item notice), 5 knocks `chara` back with an effect, 7/8/0x2E/0x2F only show the message (not for the
// player). C name, called by HomingBullet and CharaTask as extern "C"
// NONMATCHING: 94.1% - stack slots of the knock back vectors and the color, registers of the manager and the kind
extern "C" void fn_8010CF28(BattleTask* mgr, int kind, CharaTask* chara, CharaTask* target) {
    switch (kind) {
    case 4: {
        if (chara == 0 || target == 0) {
            break;
        }
        int item = target->m5C != 0 ? target->m5C->mD0 : 0;
        if (item <= 0) {
            break;
        }
        int index = mgr->FindChara_unk(target);
        BattleChara_unk* entry = index >= 0 ? &mgr->mCharas[index] : 0;
        if (entry == 0 || entry->m92E != 0) {
            break;
        }
        int got = 0;
        int count = 0;
        if (!fn_80444020(Inventory::instance_, item, 0, fn_8021E48C(item, 1), 0, 0, &got, &count, 0)) {
            break;
        }
        if (got <= 0 || count <= 0) {
            break;
        }
        entry->m92E = 1;
        wchar_t text[256];
        BattleItem_unk* data = fn_80211480(got);
        if (data != 0) {
            fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x1F), chara->m60->m04, data->mName, count);
        } else {
            fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x2F), got);
        }
        if (lbl_8087F0A8->m06C > 0 && LogMessageTask::sInstance != 0) {
            LogMessageTask::sInstance->fn_8047961C(text, -0x3301, 0, 0);
        }
        fn_800C31F4(lbl_80735DD0[21], 0, -1, 1.0f);
        if (lbl_8087F490 != 0) {
            lbl_8087F490->fn_803E5E64(got, count, 1);
        }
        break;
    }
    case 7:
    case 8:
    case 0x2E:
    case 0x2F:
        if (chara != 0 && chara->mType != 0) {
            fn_801092C8(mgr, 0xE, chara, kind, 0);
        }
        break;
    case 5: {
        if (chara == 0) {
            break;
        }
        atn::Vector3 dir(0.0f, 0.0f, 1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', chara->mRot.y);
        PSMTXMultVec(m, &dir, &dir);
        chara->fn_8015E1B8(atn::Vector3(-dir.x, -dir.y, -dir.z) * 0.2f);
        EffectColor_unk color(1.0f, 1.0f, 1.0f, 1.0f);
        mgr->fn_801070C8(0x2331, 0, 0, chara->GetModel_unk(), &lbl_807C7030, &lbl_807C7030, &color, 1.0f);
        mgr->fn_80107208(0x2331, &chara->mPos, 1.0f);
        fn_801092C8(mgr, 0xE, chara, kind, 0);
        break;
    }
    }
}

extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);

// inferred: direction on the screen from the centre of the view to `pos` (called by game/GameUI.cpp): the
// difference of the two directions from the camera, turned by the inverse of the yaw of the view, on the x/y plane
extern "C" void fn_8010D320(BattleTask* mgr, atn::Vector3 pos) {
    mgr->mCB58 = 1;
    atn::Camera& cam = atn::RenderManager::instance_->mCamera;
    atn::Vector3 dir;
    dir = cam.mTarget - cam.mPos;
    dir.Normalize_unk();
    atn::Vector3 to = pos - cam.mPos;
    if (to.IsZero_unk()) {
        to.x = 0.0f;
        to.y = 0.0f;
        to.z = 1.0f;
    }
    to.Normalize_unk();
    atn::Vector3 rot = dir.ToRotation_unk();
    atn::Matrix3x4 m = atn::Matrix3x4::RotY_unk(rot.y);
    atn::Vector3 diff = to - dir;
    to = diff;
    atn::Matrix3x4 inv(m);
    PSMTXInverse(inv.m, inv.m);
    PSMTXMultVec(inv.m, &to, &to);
    to.z = 0.0f;
    if (to.IsZero_unk()) {
        to.x = 0.0f;
        to.y = 1.0f;
        to.z = 0.0f;
    }
    to.Normalize_unk();
    mgr->mCB70.Set_unk(-to.x, 0.0f, to.y);
}

int BattleTask::fn_8010D770(HitInfo_unk* hit) {
    if (hit != 0) {
        int id = hit->m04;
        if (id == 0xCC || id == 0xD1 || id == 0xD5 || id == 0x5BC || id == 0x658) {
            return 1;
        }
    }
    return 0;
}

int BattleTask::fn_8010D7B4(HitInfo_unk* hit) {
    if (hit == 0) {
        return 0;
    }
    int id = hit->m04;
    if (id == 0x136 || id == 0x139 || id == 0x13C || id == 0x146 || id == 0x15A || id == 0x232C) {
        return 1;
    }
    return 0;
}

BattleTask::BetoModel::BetoModel() : mUsed(0), mModel(0x505), mChara(0) {
}

// inferred: the node takes the position of the node of the same name of the character, scaled by vf60
void BattleTask::BetoModel::fn_8010D854(atn::Matrix3x4* mtx, const atn::NodeChunk& chunk,
                                        const atn::ModelNodeCallbackContext& context) {
    if (mChara != 0) {
        float scale = mChara->vf60();
        MtxPtr node = mChara->mMotion.GetNodeMtx_unk(chunk.mName);
        if (node != 0) {
            atn::Vector3 pos = ((atn::Matrix3x4*)node)->GetTrans_unk();
            mtx->fn_801479E4(pos);
            atn::Vector3 s(scale, scale, scale);
            atn::Matrix3x4 r;
            Mtx m;
            PSMTXScale(m, s.x, s.y, s.z);
            PSMTXConcat(mtx->m, m, r.m);
            *mtx = r;
        }
    }
}
