// atn/SequenceHolder.cpp: table of the game sequences (atn::SequenceHolder::SequenceUnit, created by the
// SequenceUnitParam1<> templates and looked up by atn::SequenceControl).
#include "atn/SequenceControl.h"
#include "atn/SequenceHolder.h"

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

// inferred: lookups used through a void* by SequenceControl (see include/atn/SequenceControl.h)

namespace atn {

// inferred: id of the sequence called `name` (0 if there is none)
int SequenceControl::fn_800C37FC(const char* name) {
    SequenceHolder::SequenceUnitBase* unit = mTable->fn_800C3D74(name);
    if (unit != 0) {
        return unit->mId;
    }
    return 0;
}

// NONMATCHING: 99.44% - the new storage and the moved count of the inlined reserve swap r29/r30
SequenceHolder::SequenceHolder() : mCount(0) {
    mUnits.reserve(0x80);
}

// NONMATCHING: 97.25% - the inlined push_back loads the capacity before the size (known std/vector.h
// problem, see vector::push_back) and the file pool has "" before "vector length error"
int SequenceHolder::fn_800C3990(const SequenceUnit& unit) {
    SequenceUnit* copy = new ((MemoryTag)1, "", "", 0) SequenceUnit(unit);
    copy->mId = ++mCount;
    mUnits.push_back(copy);
    return copy->mId;
}

int SequenceHolder::SequenceUnitBase::fn_800C3D6C() const {
    return mId;
}

} // namespace atn

// inferred: std::equal
inline bool __Equal_unk(const char* first, const char* last, const char* other) {
    for (; first != last; ++first, ++other) {
        if (*first != *other) {
            return false;
        }
    }
    return true;
}

// inferred: the units are compared with a name by boost::lambda (`*_1 == name`)
namespace atn {
inline bool operator==(const SequenceHolder::SequenceUnitBase& unit, const char* name) {
    const char* str = unit.mName;
    unsigned long n = strlen(str);
    return name[n] == '\0' && __Equal_unk(str, str + n, name);
}
} // namespace atn

// inferred: std::find_if (MSL C++ <algorithm> is not reconstructed); the predicate is copied once more
// on the way in
template <class It, class Pred>
inline It __FindIf2_unk(It first, It last, Pred pred) {
    while (first != last && !pred(*first)) {
        ++first;
    }
    return first;
}
template <class It, class Pred>
inline It __FindIf_unk(It first, It last, Pred pred) {
    return __FindIf2_unk(first, last, pred);
}

namespace atn {

// inferred: lookup of the first unit that satisfies the predicate
template <class Pred>
inline SequenceHolder::SequenceUnitBase* __Find_unk(SequenceHolder* holder, Pred pred) {
    SequenceHolder::SequenceUnitBase** it = holder->mUnits.begin();
    SequenceHolder::SequenceUnitBase** end = holder->mUnits.end();
    it = __FindIf2_unk(it, end, pred);
    return it != end ? *it : 0;
}

// noinline: fn_800C37FC calls it with bl (MWCC would inline it here).
__declspec(noinline) SequenceHolder::SequenceUnitBase* SequenceHolder::fn_800C3D74(const char* name) {
    return __Find_unk(this, *boost::lambda::_1 == name);
}

// NONMATCHING: 98.07% - the end of the vector gets r28 and the member pointer r29-r31 (the original
// keeps the end in r31)
SequenceHolder::SequenceUnitBase* SequenceHolder::fn_800C3E84(unsigned int id) {
    SequenceUnitBase** it = __FindIf_unk(mUnits.begin(), mUnits.end(),
                                         boost::lambda::bind(&SequenceUnitBase::fn_800C3D6C, boost::lambda::_1) == id);
    return it != mUnits.end() ? *it : 0;
}

SequenceHolder::SequenceUnitBase* SequenceHolder::fn_800C3FB0(int index) {
    int i = 0;
    for (SequenceUnitBase** it = mUnits.begin(); it != mUnits.end(); ++it, ++i) {
        if (i == index) {
            return *it;
        }
    }
    return 0;
}

Task* SequenceHolder::SequenceUnit::vf0C(Task* parent) {
    return mFunc(parent);
}


} // namespace atn
