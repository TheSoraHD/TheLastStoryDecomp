#ifndef GAME_MAPPARTSOCEAN_H
#define GAME_MAPPARTSOCEAN_H

#include "atn/Csv.h"
#include "game/MapPartsModel.h"

// inferred: moves the nodes "joint1" and "joint2" up and down with a sine wave each
class MapPartsOcean : public MapPartsModel {
public:
    struct Wave_unk {
        float mAmplitude;
        float mPeriod;
        float mPhase;
    };

    MapPartsOcean(atn::Task* parent, void* arg, atn::CsvTab* csv); // 0x8049C960
    virtual ~MapPartsOcean();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf1C(void* menu);

    static MapPartsOcean* fn_8049C8DC(atn::Task* parent, void* arg, atn::CsvTab* csv);

    float mTime;        // 0x44C
    float m450;
    Wave_unk mWave[2];  // 0x454
    int mJoint[2];      // 0x46C
    unsigned char m474[0x478 - 0x474];
};

#endif
