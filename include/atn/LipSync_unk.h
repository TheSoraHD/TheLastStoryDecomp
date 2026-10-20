#ifndef ATN_LIPSYNC_UNK_H
#define ATN_LIPSYNC_UNK_H

// inferred: lip sync of the characters (atn/LipSync_unk.cpp, file name invented). The data comes from
// a tab CSV loaded by GameVoice: a key count, a second value and one line per key (vowel, time).

struct Model_unk;

// inferred: one key of the lip sync (constructor 0x8007A748, also used by Event::Dialog)
struct LipSyncKey_unk {
    LipSyncKey_unk();
    int mVowel;  // inferred: 0-4 = A, I, U, E, O; 5 = mouth closed
    float mTime; // inferred: target weight of the mouth motion
};

// inferred: lip sync data (fn_8007A530 called by GameVoice::fn_803E8A50)
struct LipSyncData_unk {
    void fn_8007A530(const char* text, int size); // inferred: parses the CSV

    unsigned int mCount;
    LipSyncKey_unk* mKeys;
    int m08; // inferred: second field of the CSV
};

// inferred: object at CharaTask+0x1154 (constructor 0x8007A75C called out of line; class unknown)
struct Chara1154_unk {
    Chara1154_unk();
    void fn_8007A7BC(Model_unk* model); // inferred: per-frame update (CharaTask::fn_80145334)

    LipSyncData_unk* m00; // inferred: data being played (0 = none)
    float m04;            // inferred: current key (fractional)
    float m08;            // inferred: step per frame
    float m0C;            // inferred: speed of the mouth weight
    float m10;            // inferred: key offset
    float m14[6];         // inferred: weight per vowel
    float m2C;            // inferred: largest change of the weight per frame
    void* m30;            // inferred: int (*)(int vowel) -> motion (CharaTask sets fn_80218260)
};

#endif
