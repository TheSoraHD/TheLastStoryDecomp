#include "game/LastCocoonChrysalis.h"
#include "atn/Csv.h"
#include "game/EnemyManager.h"
#include "game/PlayerTask.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/tokenizer.hpp>
#include "atn/DrawHelper.h"

extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: field (lbl_8087F430): m10D8 is nonzero once the route tables are loaded; the event flags
// are read with fn_80370A78 and written with fn_80370AE4
struct Field_unk {
    unsigned char m0000[0x10D8];
    void* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);             // inferred: reads an event flag
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag
extern "C" void fn_801CCD00(LastCocoonPart_unk* chara, int arg);
// inferred: debug drawing (lbl_8087EEB0) and the font used to measure the text (lbl_8087EEC8)
extern "C" void* lbl_8087EEC8;
extern "C" float fn_8006EF48(void* font, const wchar_t* text, float size, float f2, int a, int b); // inferred: width
struct Camera_unk;
extern "C" Camera_unk* lbl_8087EFB4;
extern "C" void fn_800BFAC8(atn::Vector3* out, Camera_unk* camera, const atn::Vector3* pos); // inferred: to screen
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: message table (lbl_8087F1E4); name invented (as in game/CharaTask.cpp)
struct MsgTable_unk {
    const wchar_t* Get_unk(int id) {
        if (mText[id] != 0) {
            return mText[id];
        }
        return lbl_808813D0;
    }
    const wchar_t* mText[1];
};
extern "C" MsgTable_unk* lbl_8087F1E4;

// inferred: std::string concatenation (weak copy 0x8006AFF8; not in include/std/string.h yet)
static std::string Concat_unk(const std::string& a, const std::string& b) {
    std::string result;
    std::string::size_type n1 = a.size();
    std::string::size_type n2 = b.size();
    result.reserve(n1 + n2);
    result.replace(0, 0, a.data(), a.data() + n1);
    result.replace(n1, 0, b.data(), b.data() + n2);
    return result;
}

// NONMATCHING: the original is over the inlining budget and calls the std::string, tokenizer and
// iterator members, the LH_Binary and ResHandle constructors out of line (same open case as
// CharaTask::vf2C, docs/inlining.md)
LastCocoonChrysalis::LastCocoonChrysalis(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14C0(0), m14C4(0), m14E4(0) {
    mStatus.m18 |= 0x01000040;
    m14A8 |= 0x00400000;
    std::string dir("database/chara/special/");
    std::string name("db_last_cocoon.csv");
    std::string params(info->m2C);
    typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
    Tokenizer tok(params, boost::char_separator<char>(";"));
    for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
        if (strncmp((*it).c_str(), "db_name=", 8) == 0) {
            name.assign((*it).c_str() + 8);
        }
    }
    m14B0.vf0C(Concat_unk(dir, name).c_str());
}

LastCocoonChrysalis::~LastCocoonChrysalis() {}

int LastCocoonChrysalis::vf0C() {
    if (fn_8013655C() == 0 && m14B0.fn_80473F50() == 0) {
        fn_80087994(m14E0.Load("LastCocoonChrysalis"), "DebugDisp", &m14E4, 0, 0);
        if (m14B0.fn_80470580() != 0 && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0) {
            atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
            do {
                csv.fn_8005B9CC();
            } while (csv.fn_8005BBF8());
        }
        m14D0 = 2;
        m14D8 = 5;
        m14C8 = 6;
        m14D4 = lbl_8087F8A0->Find_unk(m14D0);
        m14DC = (LastCocoonPart_unk*)EnemyManager::sInstance->Find_unk(m14D8);
        m14CC = (LastCocoonPart_unk*)EnemyManager::sInstance->Find_unk(m14C8);
        m14A8 &= ~0x04000000;
        return 1;
    }
    return 0;
}

void LastCocoonChrysalis::vf10() {
    m14C0++;
    if (mC58.mC0 == 0) {
        fn_8016E970(3);
    }
    if (fn_80370A78(lbl_8087F430, 200) == 1) {
        if (m14CC != 0) {
            m14CC->m14B4 = (CharaTask*)m14D4;
        }
        fn_80370AE4(lbl_8087F430, 200, 2);
    }
    if (fn_80370A78(lbl_8087F430, 230) == 1) {
        CharaTask* leader = (CharaTask*)lbl_8087F8A0->mHead;
        leader->fn_8016DF3C(0);
        leader->fn_80154654(leader->mC50, 1);
        leader->fn_8015495C(0, 1, 1, 1);
        leader->mC58.mC4 = 0;
        leader->fn_8016DF3C(3);
        fn_80370AE4(lbl_8087F430, 230, 0);
    } else if (fn_80370A78(lbl_8087F430, 230) == 2) {
        CharaTask* leader = (CharaTask*)lbl_8087F8A0->mHead;
        leader->fn_8015495C(0, 1, 1, 1);
        leader->mC58.mC4 = 0;
        leader->fn_8016DF3C(0);
        fn_80370AE4(lbl_8087F430, 230, 0);
    }
    if (fn_80370A78(lbl_8087F430, 231) == 1) {
        m14CC->fn_8016E4C4(1);
        m14CC->m14B8 = m14DC;
        fn_801CCD00(m14CC, 2);
        m14DC->m54C |= 8;
        fn_80370AE4(lbl_8087F430, 231, 2);
    } else if (fn_80370A78(lbl_8087F430, 231) == 2) {
        m14DC->fn_80178668(fn_80219E6C(0x1395), 0, 2, m14DC, 0);
        atn::Vector3 pos = m14DC->fn_80178164();
        const wchar_t* text = lbl_8087F1E4->Get_unk(127);
        float size = 12.0f;
        float w = fn_8006EF48(lbl_8087EEC8, text, size, 0.0f, 1, 1);
        atn::Vector3 screen;
        fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
        if (screen.z > 0.0f && screen.z < 1.0f) {
            lbl_8087EEB0->fn_80061824(screen.x - 0.5f * w, screen.y, 0.0f, size, size, 0.0f, 0.0f, 0.0f, text, 0xA000FFFF, 1, 1, 0, 0, 0xFF000000);
        }
        CharaTask* leader = (CharaTask*)lbl_8087F8A0->mHead;
        if (leader->m55C == 6 && leader->m560 == 10) {
            fn_80370AE4(lbl_8087F430, 231, 3);
        }
        if (m14DC->m55C == 6 && m14DC->m560 == 10 && fn_80370A78(lbl_8087F430, 75) == 0) {
            fn_80370AE4(lbl_8087F430, 75, 1);
        }
    } else if (fn_80370A78(lbl_8087F430, 231) == 3) {
        m14DC->fn_80178668(fn_80219E6C(0x1395), 0, 2, m14DC, 0);
        if (((CharaTask*)lbl_8087F8A0->mHead)->fn_80158BB4()) {
            fn_80370AE4(lbl_8087F430, 91, 1);
        }
    }
}

void LastCocoonChrysalis::vf14() {}

void LastCocoonChrysalis::vf3C(HitInfo_unk* hit) {}

void LastCocoonChrysalis::vf40(HitInfo_unk* hit) {}
