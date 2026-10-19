#ifndef DB_GROWTHPARSEGLOBAL_H
#define DB_GROWTHPARSEGLOBAL_H

#include "atn/ParseTree.h"

namespace db {

// inferred: parser of /boot/growth_curve.csv; "Level" is the only variable
class GrowthParseGlobal : public atn::ParseTree {
public:
    GrowthParseGlobal() : m18(0) {}
    virtual atn::BaseNode* vf0C(const char* name);

    float* m18; // inferred: variable of the formula being parsed
};

// inferred: one growth formula and the value of its variable (read by CharaStatus through
// fn_8020FCE8)
class GrowthParse {
public:
    GrowthParse() : m08(0.0f) {}
    virtual ~GrowthParse() {}

    atn::ParseCode_unk m04;
    float m08; // inferred: value of the variable "Level"
};

// inferred: one row of the table (0x4C bytes); name invented
struct GrowthEntry_unk {
    GrowthEntry_unk(); // inferred: defined at the end of the file (0x8020FD30)

    int mId;
    GrowthParse m04;
    GrowthParse m10;
    GrowthParse m1C;
    GrowthParse m28;
    GrowthParse m34;
    GrowthParse m40;
};

} // namespace db

extern "C" void fn_8020F84C();
extern "C" db::GrowthEntry_unk* fn_8020FCE8(int id);

#endif
