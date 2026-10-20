#ifndef STD_IOS_INIT_UNK_H
#define STD_IOS_INIT_UNK_H

// inferred: the static objects that MSL <iostream> defines in every file that includes it: the
// first builds cin/cout/cerr on __files (0x8068B770), the second the wide streams (0x8068BD60).
// Each file registers them in its __sinit. Names invented except ios_base::Init.
namespace std {
class ios_base {
public:
    class Init {
    public:
        Init();
        ~Init();
    };
};
class __wInit_unk {
public:
    __wInit_unk();
    ~__wInit_unk();
};
static ios_base::Init __msl_ios_init_unk;
static __wInit_unk __msl_wios_init_unk;
} // namespace std

#endif
