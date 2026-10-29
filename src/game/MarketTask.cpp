#include "game/MarketTask.h"
#include "atn/Memory.h"
#include "game/Input_unk.h"

extern "C" int rand();
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7); // inferred: debug menu int
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step); // inferred: debug menu float
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" void fn_800CB518(atn::SoundRef* sound, int frames);          // inferred: fades the sound in
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: plays a sound

// inferred: item data (fn_802114E0 returns the item of an index, fn_802114D8 the count)
struct ItemData_unk {
    unsigned char m00[0x4];
    int m04; // inferred: id
    unsigned char m08[0xBC - 0x08];
    short mBC; // inferred: kind (9: sold in the market)
    unsigned char mBE[0xC8 - 0xBE];
    int mC8;
};
extern "C" ItemData_unk* fn_802114E0(int index);
extern "C" ItemData_unk* fn_80211480(int id);           // inferred: item of an id
extern "C" float fn_802179BC(int level, int step);      // inferred: price rate (percent)
extern "C" int fn_802114D8();
extern "C" int fn_802179B4(); // inferred: number of price levels

// inferred: field status (lbl_8087F430, as in game/CharaTask.cpp); only the members used here
struct Field_unk : public atn::Task {
    unsigned char m48[0x10D0 - 0x48];
    int m10D0; // inferred: story progress
    unsigned char m10D4[0x10D8 - 0x10D4];
    atn::Task* m10D8;
    unsigned char m10DC[0x54E4 - 0x10DC];
    int m54E4;
    unsigned char m54E8[0x5744 - 0x54E8];
    unsigned int m5744; // inferred: time counter
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370174(Field_unk* field, int id);                   // inferred: event flag
extern "C" void fn_80370320(Field_unk* field, int id, int value, int arg); // inferred: sets an event flag
extern "C" void* lbl_8087F0A8;
extern "C" int lbl_8087FA20;

MarketTask* MarketTask::sInstance;

MarketTask* MarketTask::fn_804A1DDC(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new ((MemoryTag)3, "", "", 0) MarketTask(parent);
    }
    return sInstance;
}

MarketTask::MarketTask(atn::Task* parent)
    : atn::Task(parent), m48(0), m4C(5400), m50(1.0f), m54(17001), m240(0) {
    int flag = 251;
    int flag2 = 350;
    int flag3 = 470;
    for (int i = 0; i < fn_802114D8(); i++) {
        if (m58.size() >= 30) {
            break;
        }
        ItemData_unk* item = fn_802114E0(i);
        if (item->mBC == 9 && item->mC8 > 0) {
            m58.push_back(MarketItem_unk(item->m04, flag, flag2, flag3));
            flag++;
            flag2++;
            flag3++;
        }
    }
}

MarketTask::~MarketTask() {
    sInstance = 0;
    fn_800CB5C8(&m23C, 0, 0);
}

int MarketTask::vf0C() {
    if (lbl_8087F430 != 0) {
        MarketItem_unk* item = &m58[0];
        for (unsigned int i = 0; i < m58.size(); i++) {
            int levels = fn_802179B4();
            int base = rand() % levels * 58;
            int offset = rand() % 58;
            fn_80370320(lbl_8087F430, item->mFlag, offset + base, 1);
            item++;
        }
    }
    void* group = m244.Load("Market");
    fn_800874C8(group, "Interval", &m4C, 1, 999999, 1, 0, 0);
    fn_8008771C(group, "Power", &m50, 0, 0, 0.0f, 10.0f, 0.1f);
    fn_800874C8(group, "MessageId", &m54, 0, 999999, 1, 0, 0);
    return 1;
}

void MarketTask::vf10() {
    if (m240 > 0) {
        m240--;
    }
    if (m240 <= 0 && m23C.mPtr != 0) {
        fn_800CB5C8(&m23C, 90, 0);
    }
    if (fn_80370174(lbl_8087F430, 250) != 1 && lbl_8087F430->m5744 != 0 &&
        lbl_8087F430->m5744 % m4C == 0) {
        fn_804A233C();
    }
}

void MarketTask::vf14() {}

void MarketTask::fn_804A233C() {
    if (lbl_8087F430 != 0) {
        MarketItem_unk* item = &m58[0];
        for (unsigned int i = 0; i < m58.size(); i++) {
            int value = fn_80370174(lbl_8087F430, item->mFlag) + 1;
            if (value % 58 == 0) {
                int levels = fn_802179B4();
                value = rand() % levels * 58;
            }
            fn_80370320(lbl_8087F430, item->mFlag, value, 0);
            item++;
        }
    }
    Field_unk* field = lbl_8087F430;
    if (field != 0 && field->IsFlag_unk(2)) {
        if (field->m10D8 == 0 || !field->m10D8->IsFlag_unk(2)) {
            return;
        }
        if (field->m54E4 != 0 || ((Input_unk*)((char*)lbl_8087F0A8 + 0x48C))->fn_80124B60() != 0) {
            return;
        }
        if (lbl_8087FA20 != 0) {
            fn_800CB5C8(&m23C, 15, 0);
            m23C = fn_800C31F4("SE_VRTWN_017", 0, -1, 1.0f);
            fn_800CB518(&m23C, 60);
            m240 = 240;
        }
    }
}

// inferred: item data of `id` (ids <= 0 have none)
static inline ItemData_unk* __GetItem_unk(int id) {
    return id > 0 ? fn_80211480(id) : 0;
}

int MarketTask::fn_804A24C4(int id) {
    if (Find_unk(id) != 0) {
        return 1;
    }
    return 0;
}

int MarketTask::fn_804A251C(int id) {
    ItemData_unk* data = __GetItem_unk(id);
    if (data != 0 && lbl_8087F430 != 0) {
        MarketItem_unk* item = Find_unk(id);
        if (item != 0) {
            int value = fn_80370174(lbl_8087F430, item->mFlag);
            float rate = fn_802179BC(value / 58, value % 58);
            if (rate > 100.0f) {
                rate = (rate - 100.0f) * m50 + 100.0f;
            }
            int progress = lbl_8087F430->m10D0;
            if (progress >= 300000 && 400000 > progress) {
                rate *= 3.0f;
            } else if (progress >= 400000) {
                rate *= 4.0f;
            }
            int price = (int)(data->mC8 * rate / 100.0f);
            return price / 10 * 10;
        }
        return data->mC8;
    }
    return 0;
}

int MarketTask::fn_804A26DC() {
    return fn_80370174(lbl_8087F430, 251) % 11;
}

int MarketTask::fn_804A2724(int id) {
    if (__GetItem_unk(id) != 0 && lbl_8087F430 != 0) {
        MarketItem_unk* item = Find_unk(id);
        if (item != 0) {
            int total = fn_80370174(lbl_8087F430, item->mFlag2);
            int num = fn_80370174(lbl_8087F430, item->mFlag3);
            if (num <= 0) {
                return 0;
            }
            return total / num;
        }
    }
    return 0;
}

void MarketTask::fn_804A2800(int id, int num) {
    if (__GetItem_unk(id) != 0 && lbl_8087F430 != 0) {
        MarketItem_unk* item = Find_unk(id);
        if (item != 0) {
            int total = fn_80370174(lbl_8087F430, item->mFlag2);
            int price = fn_804A251C(id);
            total += price * num;
            fn_80370320(lbl_8087F430, item->mFlag2, total, 0);
            int count = fn_80370174(lbl_8087F430, item->mFlag3);
            count += num;
            fn_80370320(lbl_8087F430, item->mFlag3, count, 0);
        }
    }
}

void MarketTask::fn_804A28F8(int id) {
    if (__GetItem_unk(id) != 0 && lbl_8087F430 != 0) {
        MarketItem_unk* item = Find_unk(id);
        if (item != 0) {
            fn_80370320(lbl_8087F430, item->mFlag2, 0, 0);
            fn_80370320(lbl_8087F430, item->mFlag3, 0, 0);
        }
    }
}
