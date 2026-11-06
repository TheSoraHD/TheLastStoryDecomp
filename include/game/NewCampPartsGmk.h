#ifndef GAME_NEWCAMPPARTSGMK_H
#define GAME_NEWCAMPPARTSGMK_H

#include "game/NewCampPartsBase.h"
#include "atn/EfpHandle_unk.h"

// inferred: one gimmick of the camp (0x40 bytes)
struct NewCampGmkInfo_unk {
    NewCampGmkInfo_unk() {
        m00 = 0;
        mModel = 0;
        m3C = 0;
        mMtx.SetIdentity();
        m04 = 0;
    }

    int m00;
    short m04;
    NewCampPartsModel_unk* mModel; // 0x08
    atn::Matrix3x4 mMtx;      // 0x0C
    unsigned char m3C;
};

// inferred: gimmicks of the camp scene (parts at NewCampTask::m11EC)
class NewCampPartsGmk : public NewCampPartsBase {
public:
    NewCampPartsGmk(atn::Task* parent);
    static NewCampPartsGmk* fn_805128C0(atn::Task* parent);
    virtual ~NewCampPartsGmk();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C(NewCampScene_unk* scene);
    virtual void vf24();
    virtual void vf28();
    virtual int vf4C();
    void SetModel_unk(NewCampPartsModel_unk* model, int i);
    void fn_80512C90(int i);
    void fn_80513020(int color, int i);

    ArrayPtr_unk<NewCampGmkInfo_unk> mInfo; // 0xD4
    atn::EfpFile mEfp[8];             // 0xD8
};

#endif
