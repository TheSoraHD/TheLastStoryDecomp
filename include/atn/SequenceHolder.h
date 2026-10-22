#ifndef ATN_SEQUENCEHOLDER_H
#define ATN_SEQUENCEHOLDER_H

#include "atn/Task.h"
#include "atn/Memory.h"
#include "std/vector.h"
#include <boost/function.hpp>
#include <string.h>

namespace atn {

// inferred: how SequenceUnitParam1 receives its parameter (classes by reference, int by value, like
// boost::call_traits<P>::param_type; boost/call_traits.hpp is not in libs/boost)
template <class P>
struct SequenceParam_unk {
    typedef const P& type;
};
template <>
struct SequenceParam_unk<const int> {
    typedef const int type;
};
// inferred: P without the const (argument type of the boost::function)
template <class P>
struct SequenceArg_unk {
    typedef P type;
};
template <class P>
struct SequenceArg_unk<const P> {
    typedef P type;
};

// inferred: table of the game sequences (atn/SequenceHolder.cpp); the units are created by
// atn/SequenceUnitParam1_const_int.cpp (boot sequences) and looked up by atn::SequenceControl.
// Layout from the copies in fn_800C3990 (0x5C bytes per unit).
class SequenceHolder {
public:
    class SequenceUnitBase {
    public:
        // inferred: inlined in every SequenceUnit/SequenceUnitParam1 constructor (id -1, name copy)
        SequenceUnitBase(const char* name) : mId(-1) {
            if (name != mName) {
                memcpy(mName, name, strlen(name) + 1);
            }
        }
        virtual ~SequenceUnitBase() {} // inferred: inline, the derived destructors never call it
        virtual Task* vf0C(Task* parent) { return 0; } // weak copy 0x800C37D8 (named after SequenceControl)
        int fn_800C3D6C() const;

        int mId;          // 0x04
        char mName[0x40]; // 0x08
    };
    class SequenceUnit : public SequenceUnitBase {
    public:
        template <class F>
        SequenceUnit(const char* name, F f) : SequenceUnitBase(name), mFunc(f) {}
        virtual ~SequenceUnit() {} // inferred: inline (inlined in SequenceBoot_unk::SequenceBoot_unk)
        virtual Task* vf0C(Task* parent);

        boost::function<Task*(Task*)> mFunc; // 0x48
    };
    // inferred: unit whose creator receives a fixed parameter (P is `const T`)
    template <class P>
    class SequenceUnitParam1 : public SequenceUnitBase {
    public:
        template <class F>
        SequenceUnitParam1(const char* name, F f, typename SequenceParam_unk<P>::type param) : SequenceUnitBase(name), mFunc(f), mParam(param) {}
        virtual ~SequenceUnitParam1() {}
        virtual Task* vf0C(Task* parent) { return mFunc(parent, mParam); }

        boost::function<Task*(Task*, typename SequenceArg_unk<P>::type)> mFunc; // 0x48
        P mParam;                               // 0x5C
    };

    SequenceHolder();
    int fn_800C3990(const SequenceUnit& unit);
    // inferred: wrapper of fn_800C3990 (out-of-line copy 0x80572F40)
    int Regist_unk(const SequenceUnit& unit) { return fn_800C3990(unit); }
    // inferred: same body as fn_800C3990 for the other unit types
    template <class T>
    int Regist_unk(const T& unit);

    int mCount;                            // 0x00
    std::vector<SequenceUnitBase*> mUnits; // 0x04
};

// not inline and out of the class: its "" literals go to the file pool (@stringBase0)
template <class T>
int SequenceHolder::Regist_unk(const T& unit) {
    T* copy = new (1, "", "", 0) T(unit);
    copy->mId = ++mCount;
    mUnits.push_back(copy);
    return copy->mId;
}

} // namespace atn

#endif
