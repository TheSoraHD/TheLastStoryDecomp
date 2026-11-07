#ifndef DB_STRATEGYAITABLE_UNK_H
#define DB_STRATEGYAITABLE_UNK_H

// inferred: one row of /boot/strategy_ai_table.csv (0x20 bytes, no constructor; db/StrategyAiTable_unk.cpp)
struct StrategyAiEntry_unk {
    int mId;
    int mKind; // inferred: index in sKinds (2 if not found)
    int m08[6];
};

// inferred: row with id `id` and kind `kind` (db/StrategyAiTable_unk.cpp)
extern "C" StrategyAiEntry_unk* fn_8021CF08(int id, int kind);

#endif
