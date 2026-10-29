// atn/SequenceHolder.cpp: table of the game sequences (atn::SequenceHolder::SequenceUnit, created by the
// SequenceUnitParam1<> templates and looked up by atn::SequenceControl).
#include "atn/SequenceControl.h"
#include "atn/SequenceHolder.h"

// inferred: boost::lambda placeholders (free1..3 and the references _1.._3, set up by the __sinit)
struct LambdaPlaceholder1_unk {};
struct LambdaPlaceholder2_unk {};
struct LambdaPlaceholder3_unk {};
namespace {
LambdaPlaceholder1_unk free1;
LambdaPlaceholder2_unk free2;
LambdaPlaceholder3_unk free3;
LambdaPlaceholder1_unk& _1 = free1;
LambdaPlaceholder2_unk& _2 = free2;
LambdaPlaceholder3_unk& _3 = free3;
} // namespace

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

// fn_800C3D74 (by name) and fn_800C3E84 (by id) are std::find_if with boost::lambda predicates
// (the lambda placeholders are the .sbss objects set up by the __sinit): not written.

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
