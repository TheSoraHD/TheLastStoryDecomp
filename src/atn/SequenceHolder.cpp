// atn/SequenceHolder.cpp: table of the game sequences (atn::SequenceHolder::SequenceUnit, created by the
// SequenceUnitParam1<> templates and looked up by atn::SequenceControl).
#include "atn/SequenceControl.h"
#include "atn/SequenceHolder.h"

#include "atn/LambdaPlaceholders_unk.h"

// inferred: lookups used through a void* by SequenceControl (see include/atn/SequenceControl.h)
extern "C" atn::SequenceHolder::SequenceUnitBase* fn_800C3D74(atn::SequenceHolder* holder, const char* name);
extern "C" atn::SequenceHolder::SequenceUnitBase* fn_800C3FB0(atn::SequenceHolder* holder, int index);

namespace atn {

// inferred: id of the sequence called `name` (0 if there is none)
int SequenceControl::fn_800C37FC(const char* name) {
    SequenceHolder::SequenceUnitBase* unit = fn_800C3D74((SequenceHolder*)mTable, name);
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

// inferred: fn_800C3D74 (by name) and fn_800C3E84 (by id) are std::find_if with boost::lambda predicates
// (the lambda placeholders are the .sbss objects set up by the __sinit). The boost::lambda headers are not
// vendored: the predicates are written as functors that keep the placeholder (one byte) and the value.
template <class It, class Pred>
inline It __FindIf_unk(It first, It last, Pred pred) {
    while (first != last && !pred(*first)) {
        ++first;
    }
    return first;
}

// inferred: std::equal
inline bool __Equal_unk(const char* first, const char* last, const char* other) {
    for (; first != last; ++first, ++other) {
        if (*first != *other) {
            return false;
        }
    }
    return true;
}

// inferred: `_1->mName == name` (the C strings are equal)
struct __NameIs_unk {
    __NameIs_unk(const char* name) : mArg(_1), mName(name) {}
    bool operator()(atn::SequenceHolder::SequenceUnitBase* unit) const {
        const char* a = unit->mName;
        unsigned long n = strlen(a);
        return mName[n] == 0 && __Equal_unk(a, a + n, mName);
    }
    LambdaPlaceholder1_unk mArg;
    const char* mName;
};

// noinline: fn_800C37FC calls it with bl (MWCC would inline it here).
// NONMATCHING: 86.8% - functor stand-in for the boost::lambda predicate (the original copies the lambda
// functor four times on the stack, with the placeholder byte read through _1)
extern "C" __declspec(noinline) atn::SequenceHolder::SequenceUnitBase* fn_800C3D74(atn::SequenceHolder* holder,
                                                                                 const char* name) {
    atn::SequenceHolder::SequenceUnitBase** end = holder->mUnits.end();
    atn::SequenceHolder::SequenceUnitBase** it = __FindIf_unk(holder->mUnits.begin(), end, __NameIs_unk(name));
    return it != end ? *it : 0;
}

// inferred: `bind(&SequenceUnitBase::fn_800C3D6C, _1) == id` (the id through a member pointer)
struct __IdIs_unk {
    typedef int (atn::SequenceHolder::SequenceUnitBase::*Getter_unk)() const;
    __IdIs_unk(Getter_unk get, int id) : mGet(get), mArg(_1), mId(id) {}
    bool operator()(atn::SequenceHolder::SequenceUnitBase* unit) const { return (unit->*mGet)() == mId; }
    Getter_unk mGet;
    LambdaPlaceholder1_unk mArg;
    int mId;
};

// NONMATCHING: 39.6% - functor stand-in for the boost::lambda predicate (the original copies the lambda
// functor, member pointer and placeholder byte, seven times on the stack)
extern "C" atn::SequenceHolder::SequenceUnitBase* fn_800C3E84(atn::SequenceHolder* holder, int id) {
    atn::SequenceHolder::SequenceUnitBase** it = __FindIf_unk(
        holder->mUnits.begin(), holder->mUnits.end(), __IdIs_unk(&atn::SequenceHolder::SequenceUnitBase::fn_800C3D6C, id));
    return it != holder->mUnits.end() ? *it : 0;
}

extern "C" atn::SequenceHolder::SequenceUnitBase* fn_800C3FB0(atn::SequenceHolder* holder, int index) {
    int i = 0;
    for (atn::SequenceHolder::SequenceUnitBase** it = holder->mUnits.begin(); it != holder->mUnits.end(); ++it, ++i) {
        if (i == index) {
            return *it;
        }
    }
    return 0;
}

namespace atn {

Task* SequenceHolder::SequenceUnit::vf0C(Task* parent) {
    return mFunc(parent);
}


} // namespace atn
