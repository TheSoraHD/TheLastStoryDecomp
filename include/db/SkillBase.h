#ifndef DB_SKILLBASE_H
#define DB_SKILLBASE_H

namespace db {

// Name from the RSO (db::SkillBase and its nested types Effect, Condition, StatusChange, Hate and
// Doping). Only Condition is used for now.
struct SkillBase {
    // inferred: condition flag word of a hit/skill description (HitInfo_unk::m90); the layout
    // beyond the first word is unknown
    struct Condition {
        // RSO: Check__Q32db9SkillBase9ConditionCFi (weak copy 0x80151168)
        bool Check(int mask) const { return (mask & mFlags) == mask; }

        int mFlags; // inferred: status flags (CharaStatus::fn_8012F440, CharaTask effect mask)
    };
};

} // namespace db

#endif
