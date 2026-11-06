#ifndef GAME_NEWCAMPPARTSMAPSELECT_H
#define GAME_NEWCAMPPARTSMAPSELECT_H

#include "game/NewCampPartsBase.h"
#include "atn/LH_Binary.h"
#include "atn/EfpHandle_unk.h"
#include "atn/ResLoader_unk.h"
#include "atn/FixedArray.h"
#include <string.h>

// inferred: pair of ids initialized to -1 (8 bytes); names invented
struct MapSelectPair_unk {
    MapSelectPair_unk() : mA(-1), mB(-1) {}
    int mA;
    int mB;
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
    int m00;  // inferred: id (500 and over: battle points)
    int m04;
    int m08;  // inferred: battle point when positive (fn_80521010)
    int m0C;
    int m10;
    int m14;  // inferred: number of the board model
    NewCampPartsEntry_unk* m18;
    NewCampPartsEntry_unk* m1C; // inferred: battle mark (mBattles), cleared by vf1C (with m20, m24)
    NewCampPartsEntry_unk* m20; // inferred: "go" mark (mGoes)
    NewCampPartsEntry_unk* m24; // inferred: cursor (mCursors)
    float m28;
    char mThumbnail[0x6C - 0x2C]; // 0x2C: inferred: texture of the thumbnail
    MapSelectPair_unk m6C[5];
};

// inferred: route of the map (0x1C bytes, at 0x3ED4); names invented
struct MapSelectRoute_unk {
    MapSelectRoute_unk() : m00(0) {}
    MapSelectRoute_unk(MapSelectPoint_unk* point, int id, int state, int mapType, int mapId, int mapSubId, int entry)
        : m00(point), mId(id), m08(state), mMapType(mapType), mMapId(mapId), mMapSubId(mapSubId), mEntry(entry) {}
    MapSelectPoint_unk* m00; // inferred: point of the route
    int mId; // 0x04: inferred: compared with the camp id (m48->m4C)
    int m08;       // inferred: state (0: closed, 1: open, 2: new)
    int mMapType;  // 0x0C: inferred: destination map (arguments of fn_8036F268)
    int mMapId;    // 0x10
    int mMapSubId; // 0x14
    int mEntry;    // 0x18

};

// inferred: 0x100 bytes cleared by the constructor (at 0x45D4); names invented
struct MapSelectFlags_unk {
    MapSelectFlags_unk() { memset(m00, 0, sizeof(m00)); }
    int m00[0x40]; // inferred: ids of the routes (count at m46D4)
};

// inferred: offset of a background board (0x14 bytes, one per entry of m1910); names invented
struct MapSelectBack_unk {
    atn::Vector3 mPos; // 0x00
    float mFrame;      // 0x0C
    int m10;
};

// inferred from __vt__21NewCampPartsMapSelect (0x80793508): map selection of the camp (0x48BC bytes)
class NewCampPartsMapSelect : public NewCampPartsBase {
public:
    static NewCampPartsMapSelect* fn_8051B920(atn::Task* parent); // inferred: creates the parts
    NewCampPartsMapSelect(atn::Task* parent);                      // 0x8051B984
    virtual ~NewCampPartsMapSelect();                              // 0x8051C2A8
    virtual void vf1C(NewCampScene_unk* scene);                    // 0x8051CA74
    virtual void vf20();                                           // 0x8051EC50
    virtual void vf24();                                           // 0x8051EF20
    virtual void vf30();                                           // 0x8051FF5C
    virtual void vf40(void* menu);                                 // 0x8051C3DC
    virtual void vf44();                                           // 0x80521C9C
    virtual void vf48();                                           // 0x80521D44
    virtual int vf4C();                                            // 0x8051C750

    void fn_80520634(); // inferred: reads map_param.csv
    void fn_805201EC(int* point);
    void fn_80521908(int reset); // inferred: places the background boards
    void fn_8051FB7C(int state); // inferred: changes the state of the cursor (m1998)
    void fn_80521A00(int dir);
    void fn_80520098();
    int fn_80521010(); // inferred: places the marks of the routes; returns 0 when ready
    void fn_80520B04();
    void fn_8051FD20(); // inferred: input of the cursor state
    void fn_805212BC(); // inferred: goes to the map of the selected route
    // inferred: moves the camera to `point` (inlined in vf20 and vf30)
    void SetCursorPoint_unk(MapSelectPoint_unk* point);
    // inferred: index of the route of the camp `id` (-1: none)
    int FindRouteIndex_unk(int id) {
        for (unsigned int i = 0; i < mRoutes.mSize; i++) {
            if (id == mRoutes.mData[i].mId) {
                return i;
            }
        }
        return -1;
    }
    MapSelectRoute_unk* FindRoute_unk(int id) {
        for (unsigned int i = 0; i < mRoutes.mSize; i++) {
            if (id == mRoutes.mData[i].mId) {
                return &mRoutes.mData[i];
            }
        }
        return 0;
    }
    MapSelectPoint_unk* FindPoint_unk(int id) {
        for (unsigned int i = 0; i < m19CC; i++) {
            if (id == mPoints[i].m00) {
                return &mPoints[i];
            }
        }
        return 0;
    }
    // inferred: frees the files of the points (destructor and vf1C)
    void FreeFiles_unk() {
        if (m198C != 0) {
            delete[] m198C;
            delete[] m1990;
            m198C = 0;
            m1990 = 0;
        }
    }

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
    MapSelectBack_unk m1914[6];
    NewCampPartsFile_unk* m198C;         // inferred: files of the points (m4C)
    char (*m1990)[0x20];                 // inferred: their names (two per point: model, anime)
    atn::Task* mUiCursor;                // 0x1994: ui_cursor.d2a
    int m1998;
    int m199C;
    int m19A0;
    int m19A4;
    int m19A8;
    int m19AC;
    int m19B0;
    int m19B4;
    float m19B8;
    float m19BC;
    float m19C0;
    atn::LH_Binary mParam;               // 0x19C4: map_param.csv
    int m19CC;                           // inferred: number of points read
    MapSelectPoint_unk mPoints[64];      // 0x19D0
    atn::FixedArray_unk<MapSelectRoute_unk, 64> mRoutes; // 0x3ED0: inferred: routes (push_back/erase in fn_80520B04)
    MapSelectFlags_unk m45D4;
    int m46D4;
    atn::FixedArray_unk<int, 64> m46D8;  // inferred: values m04 of the points
    atn::EfpFile m47DC;                  // menu_map00.efp
    atn::EfpFile m47E8;                  // be010_004.efp
    atn::EffFile m47F4;                  // be010_009.eff
    atn::Vector3 m4800;
    atn::Vector3 m480C;
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
