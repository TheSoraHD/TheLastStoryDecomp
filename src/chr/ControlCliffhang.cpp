#include "chr/ControlCliffhang.h"
#include "game/CharaTask.h"
#include "atn/Memory.h"
#include <boost/bind.hpp>

extern "C" int rand();
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length

// inferred: point of the route tables (0x30 bytes); names invented
struct CliffPoint_unk {
    int m00;            // inferred: route link of the point
    atn::Vector3 mPos;  // 0x04
    unsigned char m10[0x18 - 0x10];
    int mType;          // 0x18: inferred: kind of ledge (motion set)
    unsigned char m1C[0x30 - 0x1C];
};
// inferred: link of type 5 (two points)
struct CliffLink_unk {
    int mA;
    int mB;
};
// inferred: route tables of the field (lbl_8087F430->m10D8); only the points used here
struct CliffTable_unk {
    unsigned char m00[0xC8];
    CliffPoint_unk* mPoints; // 0xC8
};
struct CliffField_unk {
    unsigned char m0000[0x10D8];
    CliffTable_unk* m10D8;
};
extern "C" CliffField_unk* lbl_8087F430;
extern "C" CliffLink_unk* fn_803C18DC(CliffTable_unk* table, int type, int link);

// inferred: game state (lbl_8087EFA8): m3A4 is the time scale
struct GameState_unk {
    unsigned char m000[0x3A4];
    float m3A4;
};
extern "C" GameState_unk* lbl_8087EFA8;

// inferred: pad (lbl_8087EF70); fn_800A55D4 tests a button of a player
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A55D4(void* pad, int player, int button);

// inferred: collision (lbl_8087EE98); fn_8004ECC0 casts a ray
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ECC0(void* colli, int arg, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, int arg5, int arg6);

// inferred: voice/sound player (lbl_8087F498); fn_803EA77C plays voice `id` of a character
extern "C" void* lbl_8087F498;
extern "C" void fn_803EA77C(void* player, CharaTask* chara, int id, int arg, float a, float b);

// inferred: linear interpolation with the product first (name invented)
inline atn::Vector3 LerpCliff_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

namespace chr {

ControlCliffhang::ControlCliffhang(Chara_unk* chara, int a, int flags, float f)
    : ControlBase(chara), m08(0), m0C(0), m7C(1), m80(flags), m94(0), m98(0) {
    chara->m560 = 0x3A;
    mChara->fn_8016DA4C();
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if ((m80 & 2) == 2) {
        m7C = 0;
        m98 = 1;
    }
    fn_801AB6B8(a, 1, f);
}

// NONMATCHING: 99.3% - stack slots of the vectors of states 0 and 2 and the FPR of the speed.
int ControlCliffhang::vf0C() {
    if (m0C == 0) {
        atn::Vector3 rot = mChara->mRot;
        Motion_unk* motion = &mChara->mMotion;
        switch (m7C) {
        case 0: {
            // inferred: named local (puts 0.0f first in the .sdata2 pool, as in the original)
            float t = 0.0f;
            float frame = motion->mTracks[0].m08;
            if (frame <= motion->fn_80097D7C(0) - 2.0f) {
                float frame = motion->mTracks[0].m08;
                t = frame / (motion->fn_80097D7C(0) - 2.0f);
                atn::Vector3 pos = LerpCliff_unk(m84, m14, t);
                mChara->mPos = pos;
                atn::Vector3 r(0.0f, m90 + mChara->mRot.y, 0.0f);
                mChara->mRot = r;
            } else {
                mChara->mPos = m14;
            }
            float end = motion->mTracks[0].m08;
            if (end >= motion->fn_80097D7C(0)) {
                m7C = 1;
                fn_801AB6B8(m2C, 0, 0.0f);
            }
            break;
        }
        case 1:
            if (m08 == 0) {
                int id = -1;
                int index = fn_801AC14C();
                if (index >= 0) {
                    id = m44[index];
                }
                if (id >= 0) {
                    m08 = 1;
                    motion->fn_80097C08(0, id, 0, 0, 1, 0.0f, 0.1f);
                    if ((m80 & 8) == 8) {
                        motion->mTracks[0].m0C = 1.5f;
                    }
                    m6C = m58[index];
                }
                if (m94 != 0) {
                    m0C = 1;
                }
            } else {
                atn::Vector3 d = m14 - m20;
                float speed = m6C / PSVECMag(&d);
                float length = motion->fn_80097D7C(0) - 1.0f;
                if (length > 0.0f) {
                    float step = motion->mTracks[0].m0C * lbl_8087EFA8->m3A4 / length;
                    if (m70 != 0) {
                        m74 += speed * step;
                    } else {
                        m74 -= speed * step;
                    }
                } else {
                    m74 = 1.0f;
                }
                m74 = (m74 > 0.0f ? m74 : 0.0f) < 1.0f ? (m74 > 0.0f ? m74 : 0.0f) : 1.0f;
                mChara->mPos = LerpCliff_unk(m14, m20, m74);
                float frame = motion->mTracks[0].m08;
                if (frame >= motion->fn_80097D7C(0)) {
                    fn_801AB6B8(m30, 0, 0.0f);
                } else {
                    float speed2 = motion->mTracks[0].m0C;
                    float frame2 = motion->mTracks[0].m08;
                    if (frame2 + speed2 >= motion->fn_80097D7C(0)) {
                        int id = -1;
                        float next = 0.0f;
                        if (m74 >= 1.0f) {
                            fn_801AB6B8(m30, 0, next);
                        } else {
                            if (m78 != 0) {
                                id = m44[m10];
                                next = m58[m10];
                            } else {
                                int index = fn_801AC14C();
                                if (index >= 0) {
                                    id = m44[index];
                                    next = m58[index];
                                }
                            }
                            if (id >= 0) {
                                motion->fn_80097C08(0, id, 0, 1, 1, 0.0f, 0.1f);
                                m6C = next;
                            } else {
                                m08 = 0;
                                motion->fn_80097C08(0, m44[0], 1, 0, 1, 0.0f, 0.1f);
                                motion->mTracks[0].m0C = 1.0f;
                            }
                        }
                    }
                }
            }
            break;
        case 2:
            if (motion->mTracks[0].m08 <= 30.0f) {
                float scale = motion->mTracks[0].m0C;
                float time = lbl_8087EFA8->m3A4;
                float speed = scale * time / motion->fn_80097D7C(0);
                atn::Vector3 pos = mChara->mPos;
                atn::Vector3 dir(0.0f, 0.0f, 1.0f);
                Mtx mtx;
                PSMTXRotRad(mtx, 'y', mChara->mRot.y);
                PSMTXMultVec(mtx, &dir, &dir);
                pos += dir * 4.0f * speed;
                mChara->mPos = pos;
            }
            float frame = motion->mTracks[0].m08;
            if (frame >= motion->fn_80097D7C(0)) {
                m0C = 1;
            }
            break;
        }
    }
    return m0C != 0;
}

// NONMATCHING: 98.5% - the original loads m94 before saving r31 (prologue scheduling).
boost::function<void()> ControlCliffhang::vf10() {
    if (m94 != 0) {
        return boost::bind(&CharaTask::vf8C, (CharaTask*)mChara, lbl_807C7030);
    }
    return boost::bind(&CharaTask::fn_801696E0, (CharaTask*)mChara);
}

// NONMATCHING: 99.9% - registers r3/r4 swapped in the copy of m14/rot (state 1).
void ControlCliffhang::fn_801AB6B8(int route, int first, float angle) {
    Motion_unk* motion = &mChara->mMotion;
    if (fn_801AB9E4(route) == 0) {
        if ((m80 & 4) == 4) {
            m7C = 2;
            motion->fn_80097C08(0, m98 != 0 ? 0x206 : 0x59, 0, 0, 1, 0.0f, 0.1f);
            motion->mTracks[0].m0C = 1.0f;
            m84 = mChara->mPos;
            if (lbl_8087F498 != 0 && (mChara->mType == 0 || rand() % 2 == 0)) {
                fn_803EA77C(lbl_8087F498, mChara, 0xF, 0, 1.0f, 10.0f);
            }
        } else {
            m0C = 1;
        }
        return;
    }
    atn::Vector3 rot = mChara->mRot;
    if (m40 != 0) {
        rot.y = m34.y;
    } else if (first != 0) {
        rot.y = angle;
    }
    switch (m7C) {
    case 0: {
        motion->fn_80097C08(0, m98 != 0 ? 0x204 : 0x1F8, 0, 1, 1, 0.0f, 0.1f);
        m84 = mChara->mPos;
        float turn = atn::WrapAngle_unk(rot.y - mChara->mRot.y);
        m90 = turn / (motion->fn_80097D7C(0) - 2.0f);
        if (m98 != 0 && lbl_8087F498 != 0 && (mChara->mType == 0 || rand() % 2 == 0)) {
            fn_803EA77C(lbl_8087F498, mChara, 0xC, 0, 1.0f, 10.0f);
        }
        break;
    }
    case 1:
        mChara->mPos = m14;
        mChara->mRot = rot;
        if (m78 != 0) {
            motion->fn_80097C08(0, m44[m10], 0, 1, 1, 0.0f, 0.1f);
        } else {
            motion->fn_80097C08(0, m44[0], 1, 0, 1, 0.0f, 0.1f);
        }
        break;
    }
    motion->m34C = 1;
    motion->mTracks[0].m0C = 1.0f;
    m08 = m78;
}

// NONMATCHING: 96.4% - stack slots of the vectors (the original keeps the declaration order
// differently) and the order of the subtractions of the slope test.
int ControlCliffhang::fn_801AB9E4(int route) {
    atn::Vector3 normal;
    CliffTable_unk* table = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
    if (table == 0) {
        return 0;
    }
    CliffLink_unk* link = fn_803C18DC(table, 5, route);
    if (link == 0) {
        return 0;
    }
    CliffPoint_unk* a = &table->mPoints[link->mA];
    CliffPoint_unk* b = &table->mPoints[link->mB];
    m14 = a->mPos;
    m20 = b->mPos;
    m2C = route;
    m30 = b->m00;
    m74 = 0.0f;
    m78 = 0;
    m40 = 1;
    atn::Vector3 up(0.0f, 1.0f, 0.0f);
    if (b->mPos.y > a->mPos.y) {
        atn::Vector3 d = (b->mPos - a->mPos).GetNormalized_unk();
        if (PSVECDotProduct(&up, &d) > 0.70710677f) {
            m40 = 0;
        }
    } else {
        atn::Vector3 d = (a->mPos - b->mPos).GetNormalized_unk();
        if (PSVECDotProduct(&up, &d) > 0.70710677f) {
            m40 = 0;
        }
    }
    if (m40 != 0) {
        atn::Vector3 dir = b->mPos - a->mPos;
        if (!((float)__fabs(PSVECMag(&dir)) < 1.1920929e-7f)) {
            PSVECNormalize(&dir, &dir);
        }
        atn::Vector3 side;
        atn::Vector3 axis(0.0f, 1.0f, 0.0f);
        PSVECCrossProduct(&dir, &axis, &side);
        if ((float)__fabs(fn_805F9920(&side)) < 1.1920929e-7f) {
            atn::Vector3 back(0.0f, 0.0f, -1.0f);
            atn::Vector3 cross;
            PSVECCrossProduct(&dir, &back, &cross);
            side = cross;
        }
        if (!((float)__fabs(fn_805F9920(&side)) < 1.1920929e-7f)) {
            PSVECNormalize(&side, &side);
        }
        atn::Vector3 to = a->mPos + side * 5.0f;
        if (fn_8004ECC0(lbl_8087EE98, 0, &a->mPos, &to, 0x80000000, 0, 0) != 0) {
            normal = side;
        } else {
            normal = atn::Vector3(-side.x, -side.y, -side.z);
        }
        atn::Vector3 c;
        PSVECCrossProduct(&dir, &normal, &c);
        if (c.y < 0.0f) {
            m10 = 3;
        } else {
            m10 = 4;
        }
    } else if (b->mPos.y > a->mPos.y) {
        m10 = 1;
    } else {
        m10 = 2;
    }
    m34 = normal.ToRotation_unk();
    switch (a->mType) {
    case 0:
        if (m98 != 0) {
            m44[0] = 0x205;
            m58[0] = 0.0f;
            m44[1] = 0x203;
            m58[1] = 9.219f;
            m44[2] = 0x202;
            m58[2] = 9.219f;
            m44[3] = 0x1F5;
            m58[3] = 5.0f;
            m44[4] = 0x1F6;
            m58[4] = 5.0f;
        } else {
            m44[0] = 0x1F9;
            m58[0] = 0.0f;
            m44[1] = 0x1FA;
            m58[1] = 6.0f;
            m44[2] = 0x1FA;
            m58[2] = 6.0f;
            m44[3] = 0x1F5;
            m58[3] = 5.0f;
            m44[4] = 0x1F6;
            m58[4] = 5.0f;
        }
        break;
    case 1:
        m44[0] = 0x1F9;
        m58[0] = 0.0f;
        m44[1] = 0x58;
        m58[1] = 12.0f;
        m44[2] = 0x58;
        m58[2] = 12.0f;
        m44[3] = 0x1F5;
        m58[3] = 5.0f;
        m44[4] = 0x1F6;
        m58[4] = 5.0f;
        break;
    case 2:
        m44[0] = 0x1FE;
        m58[0] = 0.0f;
        m44[1] = 0x1FA;
        m58[1] = 6.0f;
        m44[2] = 0x1FA;
        m58[2] = 6.0f;
        m44[3] = 0x1FC;
        m58[3] = 5.0f;
        m44[4] = 0x1FD;
        m58[4] = 5.0f;
        break;
    case 3:
        m44[m10] = 0x57;
        m58[m10] = 10.0f;
        m70 = 1;
        m6C = m58[m10];
        m78 = 1;
        break;
    }
    return 1;
}

int ControlCliffhang::fn_801AC14C() {
    int up = fn_800A55D4(lbl_8087EF70, 0, 0x1A);
    int down = fn_800A55D4(lbl_8087EF70, 0, 0x19);
    int left = fn_800A55D4(lbl_8087EF70, 0, 0x17);
    int right = fn_800A55D4(lbl_8087EF70, 0, 0x18);
    int ret = -1;
    if (m10 == 1) {
        if (up != 0 && m74 < 1.0f) {
            ret = 1;
            m70 = ret;
        } else if (down != 0 && (m80 & 0x10) == 0x10) {
            m94 = 1;
        }
    } else if (m10 == 2) {
        if (up != 0 && m74 > 0.0f) {
        } else if (down != 0 && m74 < 1.0f) {
            m70 = 1;
            ret = 2;
        }
    } else if (m10 == 3) {
        if (left != 0 && m74 < 1.0f) {
            m70 = 1;
            ret = 3;
        } else if (right != 0 && m74 > 0.0f) {
        }
    } else if (m10 == 4) {
        if (left != 0 && m74 > 0.0f) {
        } else if (right != 0 && m74 < 1.0f) {
            m70 = 1;
            ret = 4;
        }
    }
    return ret;
}

atn::Vector3 ControlCliffhang::vf14() {
    Motion_unk* motion = &mChara->mMotion;
    int id = motion->mTracks[0].m00;
    if (id == 0x59 || id == 0x206) {
        float t = (1.0f < (0.0f > (motion->mTracks[0].m08 - 40.0f) / 20.0f ? 0.0f : (motion->mTracks[0].m08 - 40.0f) / 20.0f))
                      ? 1.0f
                      : (0.0f > (motion->mTracks[0].m08 - 40.0f) / 20.0f ? 0.0f : (motion->mTracks[0].m08 - 40.0f) / 20.0f);
        return LerpCliff_unk(m84, mChara->mPos, t);
    }
    return mChara->mPos;
}

} // namespace chr
