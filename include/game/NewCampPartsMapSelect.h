#ifndef GAME_NEWCAMPPARTSMAPSELECT_H
#define GAME_NEWCAMPPARTSMAPSELECT_H

#include "game/NewCampPartsBase.h"
#include "atn/LH_Binary.h"
#include "atn/EfpHandle_unk.h"
#include "atn/ResLoader_unk.h"
#include <string.h>

// inferred: pair of ids initialized to -1 (8 bytes); names invented
struct MapSelectPair_unk {
    MapSelectPair_unk() : mA(-1), mB(-1) {}
    int mA;
    int mB;
};
// inferred: ids of the point (0x28 bytes at 0x6C of the point); names invented
struct MapSelectIds_unk {
    MapSelectIds_unk() : mA(-1), mB(-1) {}
    int mA;
    int mB;
    MapSelectPair_unk mPairs[4];
};

// inferred: point of the map read from map_param.csv (0x94 bytes, 64 of them at 0x19D0); names invented
struct MapSelectPoint_unk {
    MapSelectPoint_unk() {
        m10 = -1;
        m14 = 0;
        m18 = 0;
        m1C = 0;
        m20 = 0;
        m24 = 0;
        m28 = 0.0f;
    }
    unsigned char m00[0x10];
    int m10;
    int m14;
    int m18;
    int m1C;
    int m20;
    int m24;
    float m28;
    unsigned char m2C[0x6C - 0x2C];
    MapSelectIds_unk m6C;
};

// inferred: route of the map (0x1C bytes, at 0x3EF0); names invented
struct MapSelectRoute_unk {
    MapSelectRoute_unk() : m00(0) {}
    int m00;
    unsigned char m04[0x1C - 0x04];
};

// inferred: 0x100 bytes cleared by the constructor (at 0x45D4); names invented
struct MapSelectFlags_unk {
    MapSelectFlags_unk() { memset(m00, 0, sizeof(m00)); }
    unsigned char m00[0x100];
};

// inferred from __vt__21NewCampPartsMapSelect (0x80793508): map selection of the camp (0x48BC bytes)
class NewCampPartsMapSelect : public NewCampPartsBase {
public:
    static NewCampPartsMapSelect* fn_8051B920(atn::Task* parent); // inferred: creates the parts
    NewCampPartsMapSelect(atn::Task* parent);                      // 0x8051B984
    virtual ~NewCampPartsMapSelect();                              // 0x8051C2A8
    virtual void vf40(void* menu);                                 // 0x8051C3DC
    virtual void vf44();                                           // 0x80521C9C
    virtual void vf48();                                           // 0x80521D44

    unsigned char mD4;
    unsigned char mD5;
    unsigned char mD6;
    unsigned char mD7;
    int mD8;
    int mDC;
    NewCampPartsEntry_unk mBattles[32];  // 0xE0: map_board01_select_battle_b.d2a
    NewCampPartsEntry_unk mGoes[32];     // 0x8E0: map_board01_select_go_b.d2a
    NewCampPartsEntry_unk mCursors[32];  // 0x10E0: map_board01_select_cursor_b.d2a
    atn::D3Anime* mBattle;               // 0x18E0: map_board01_select_battle.d2a
    atn::D3Anime* mGo;                   // 0x18E4: map_board01_select_go.d2a
    atn::Task* mBattleBoard;             // 0x18E8: map_board01_battle.d2a
    atn::Task* mLogo;                    // 0x18EC: map_logo.d2a
    atn::Vector3 m18F0;
    unsigned char m18FC[0x1900 - 0x18FC];
    float m1900;
    atn::Task* mMapName;                 // 0x1904: map_name.d2a
    atn::Task* m1908;                    // inferred: deleted by the destructor (fn_800D2338)
    int m190C;                           // inferred: number of entries of m1910
    ArrayPtr_unk<NewCampPartsEntry_unk> m1910;
    unsigned char m1914[0x198C - 0x1914];
    unsigned char* m198C;
    unsigned char* m1990;
    atn::Task* mUiCursor;                // 0x1994: ui_cursor.d2a
    int m1998;
    int m199C;
    int m19A0;
    int m19A4;
    int m19A8;
    unsigned char m19AC[0x19B4 - 0x19AC];
    int m19B4;
    float m19B8;
    float m19BC;
    float m19C0;
    atn::LH_Binary mParam;               // 0x19C4: map_param.csv
    int m19CC;
    MapSelectPoint_unk mPoints[64];      // 0x19D0
    int m3ED0;
    int m3ED4;
    unsigned char m3ED8[0x3EF0 - 0x3ED8];
    MapSelectRoute_unk mRoutes[63];      // 0x3EF0
    MapSelectFlags_unk m45D4;
    int m46D4;
    int m46D8;
    unsigned char m46DC[0x47DC - 0x46DC];
    atn::EfpFile m47DC;                  // menu_map00.efp
    atn::EfpFile m47E8;                  // be010_004.efp
    atn::EffFile m47F4;                  // be010_009.eff
    atn::Vector3 m4800;
    unsigned char m480C[0x4818 - 0x480C];
    float m4818;
    float m481C;
    float m4820;
    float m4824;
    atn::Vector3 m4828[5];               // inferred: angles (degrees in the constructor, then radians)
    atn::Vector3 m4864;
    atn::Vector3 m4870;
    float m487C;
    float m4880;
    float m4884;
    float m4888[4];                      // inferred: colour
    float m4898[4];                      // inferred: colour
    int m48A8;
    float m48AC;
    float m48B0;
    float m48B4;
    float m48B8;
};

#endif
