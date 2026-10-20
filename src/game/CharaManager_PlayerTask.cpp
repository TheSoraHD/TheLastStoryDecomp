#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/EnemyTask.h"

extern "C" void fn_80089AD4(void* arg);
extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task
extern "C" int fn_80219544(int name);         // inferred: character id of a name id

// inferred: list helpers of each character class (names invented)
inline void ListAppend_unk(PlayerTask* head, PlayerTask* chara) {
    head->fn_8054A640(chara);
}
inline void ListUnlink_unk(PlayerTask* head, PlayerTask* chara) {
    head->fn_8054A65C(chara);
}
inline void ListAppend_unk(EnemyTask* head, EnemyTask* chara) {
    head->fn_8035B808(chara);
}
inline void ListUnlink_unk(EnemyTask* head, EnemyTask* chara) {
    head->fn_8035B824(chara);
}

template <class T>
CharaManager<T>::CharaManager(atn::Task* parent) : atn::Task(parent), mHead(0), mCount(0), mNextId(0) {}

template <class T>
void CharaManager<T>::vf10() {
    for (T* chara = mHead; chara != 0; chara = chara->mNext) {
        if (chara->mMotion.mFlag2_unk) {
            chara->mMotion.fn_8008B140();
        }
        if ((chara->mFlags & 4) == 4) {
            chara->m1418++;
        } else {
            chara->m1418 = 0;
        }
    }
}

template <class T>
void CharaManager<T>::vf14() {
    for (T* chara = mHead; chara != 0; chara = chara->mNext) {
        if (chara->mFlagB22) {
            fn_80089AD4(chara->mMotion.m104);
            chara->mFlagB22 = 0;
        }
    }
}

template <class T>
void CharaManager<T>::Add_unk(T* chara) {
    mCount++;
    if (mHead != 0) {
        ListAppend_unk(mHead, chara);
    } else {
        mHead = chara;
    }
}

template <class T>
void CharaManager<T>::Remove_unk(T* chara) {
    mCount--;
    if (chara == mHead) {
        mHead = mHead->mNext;
    } else {
        ListUnlink_unk(mHead, chara);
    }
}

template <class T>
void CharaManager<T>::vf1C() {
    for (T* chara = mHead; chara != 0; chara = chara->mNext) {
        chara->fn_8016E970(2);
        chara->fn_800D246C(0);
    }
}

template <class T>
void CharaManager<T>::DeleteAll_unk() {
    for (T* chara = mHead; chara != 0; chara = chara->mNext) {
        fn_800D2338(chara);
    }
}

template <class T>
T* CharaManager<T>::Last_unk() {
    T* last = 0;
    if (mHead != 0 && mHead->mNext != 0) {
        last = mHead;
        while (last->mNext != 0) {
            last = last->mNext;
        }
    }
    return last;
}

template <class T>
T* CharaManager<T>::FindByChara_unk(int id) {
    for (T* chara = mHead; chara != 0; chara = chara->mNext) {
        if (id == chara->m50) {
            return chara;
        }
    }
    return 0;
}

template <class T>
T* CharaManager<T>::Find_unk(int id) {
    for (T* chara = mHead; chara != 0; chara = chara->mNext) {
        if (id == chara->m58) {
            return chara;
        }
    }
    return 0;
}

template <class T>
T* CharaManager<T>::FindByIndex_unk(int index) {
    int i = 0;
    for (T* chara = mHead; chara != 0; chara = chara->mNext, i++) {
        if (index == i) {
            return chara;
        }
    }
    return 0;
}

template <class T>
T* CharaManager<T>::FindByName_unk(int name) {
    int id = fn_80219544(name);
    for (T* chara = mHead; chara != 0; chara = chara->mNext) {
        if (id == chara->m50) {
            return chara;
        }
    }
    return 0;
}

// inferred: explicit instantiations of the members in use (in this order); the vtables and the
// inline members follow at the end of the unit
template CharaManager<PlayerTask>::CharaManager(atn::Task*);
template void CharaManager<PlayerTask>::vf10();
template void CharaManager<PlayerTask>::vf14();
template void CharaManager<PlayerTask>::Add_unk(PlayerTask*);
template void CharaManager<PlayerTask>::Remove_unk(PlayerTask*);
template void CharaManager<PlayerTask>::vf1C();
template void CharaManager<PlayerTask>::DeleteAll_unk();
template PlayerTask* CharaManager<PlayerTask>::Last_unk();
template PlayerTask* CharaManager<PlayerTask>::FindByChara_unk(int);
template PlayerTask* CharaManager<PlayerTask>::Find_unk(int);
template PlayerTask* CharaManager<PlayerTask>::FindByIndex_unk(int);
template PlayerTask* CharaManager<PlayerTask>::FindByName_unk(int);
template CharaManager<EnemyTask>::CharaManager(atn::Task*);
template void CharaManager<EnemyTask>::vf10();
template void CharaManager<EnemyTask>::vf14();
template void CharaManager<EnemyTask>::Add_unk(EnemyTask*);
template void CharaManager<EnemyTask>::Remove_unk(EnemyTask*);
template void CharaManager<EnemyTask>::vf1C();
template void CharaManager<EnemyTask>::DeleteAll_unk();
template EnemyTask* CharaManager<EnemyTask>::Last_unk();
template EnemyTask* CharaManager<EnemyTask>::Find_unk(int);

// inferred: included here, NpcManager.h instantiates CharaManager<NpcTask> after the other two
// (the vtables and the inline members at the end of the unit are in the reverse order)
#include "game/NpcTask.h"

inline void ListAppend_unk(NpcTask* head, NpcTask* chara) {
    head->fn_80547DF8(chara);
}
inline void ListUnlink_unk(NpcTask* head, NpcTask* chara) {
    head->fn_80547E14(chara);
}

template CharaManager<NpcTask>::CharaManager(atn::Task*);
template void CharaManager<NpcTask>::vf10();
template void CharaManager<NpcTask>::vf14();
template void CharaManager<NpcTask>::Add_unk(NpcTask*);
template void CharaManager<NpcTask>::Remove_unk(NpcTask*);
template void CharaManager<NpcTask>::vf1C();
template NpcTask* CharaManager<NpcTask>::Last_unk();
template NpcTask* CharaManager<NpcTask>::Find_unk(int);
