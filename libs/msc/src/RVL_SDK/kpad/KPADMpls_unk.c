// inferred: Motion Plus part of the TLS (2010) KPAD library, a separate object in the original (own
// .sdata2 pool after KPAD's, own .data and .sbss). Name invented; written from the asm.
#include <revolution/kpad/KPADPriv.h>

#include <internal/fdlibm.h>

// inferred: Motion Plus calibration of WPAD (key/src/revolution/WPAD.h)
typedef struct KPADMplsCalib_unk
{
    f32 pitchZero;  // 0x00
    f32 pitchScale; // 0x04
    f32 yawZero;    // 0x08
    f32 yawScale;   // 0x0C
    f32 rollZero;   // 0x10
    f32 rollScale;  // 0x14
    s32 degrees;    // 0x18
} KPADMplsCalib_unk;

u8 KPADIsEnableAimingMode(KPADChannel chan);
f32 KPADGetSensorHeight(KPADChannel chan);
extern KPADMPDir fn_unk_identity_dir;
void WPADiGetMplsCalibration(WPADChannel chan, KPADMplsCalib_unk* high, KPADMplsCalib_unk* low);
u8 WPADiGetMplsStatus(WPADChannel chan);
WPADResult WPADiControlMpls(WPADChannel chan, u8 command, WPADCallback callback);

// inferred: Motion Plus work area of one channel (0xDB0 bytes); names invented from their use
typedef struct KPADMplsChan_unk
{
    KPADMPStatus status;      // 0x000: copied to KPADStatus.mpls
    Vec last;                 // 0x03C: speed of the previous sample
    Vec speedDelta;           // 0x048
    f32 scale[3];             // 0x054: degrees per count of each axis
    KPADMPDir prevDir;        // 0x060
    Vec base[3];              // 0x084: inferred: reference axes for the direction revision
    f32 m0A8;                 // 0x0A8
    f32 interval;             // 0x0AC: seconds per sample
    f32 degInterval;          // 0x0B0: 360 * interval
    f32 m0B4;                 // 0x0B4
    u8 resetReq;              // 0x0B8
    u8 m0B9;                  // 0x0B9
    u8 m0BA;                  // 0x0BA
    u8 m0BB;                  // 0x0BB
    u8 mode;                  // 0x0BC: 0 Motion Plus, 1 with extension, 2 none
    u8 zeroPlayEnabled;       // 0x0BD
    u8 zeroDriftUpdate;       // 0x0BE
    u8 dirReviseEnabled;      // 0x0BF
    u8 accReviseEnabled;      // 0x0C0
    u8 dpdReviseEnabled;      // 0x0C1
    s8 ctrlResult;            // 0x0C2: 1 ok, -1 error
    u8 m0C3;                  // 0x0C3
    f32 m0C4;                 // 0x0C4
    f32 zeroPlay;             // 0x0C8
    f32 zeroDriftRange;       // 0x0CC
    s32 zeroDriftCount;       // 0x0D0
    f32 zeroDriftPower;       // 0x0D4
    f32 dirReviseRate;        // 0x0D8
    f32 accRevisePower;       // 0x0DC
    f32 accReviseRange;       // 0x0E0
    f32 m0E4;                 // 0x0E4
    f32 zeroPlayResult;       // 0x0E8
    f32 m0EC;                 // 0x0EC
    f32 dirReviseResult;      // 0x0F0
    f32 accReviseResult;      // 0x0F4
    f32 dpdReviseResult;      // 0x0F8
    f32 dpd2posScale;         // 0x0FC
    s32 sensorBarAuto;        // 0x100
    f32 sensorHeight;         // 0x104
    f64 countScale[2][3];     // 0x108: high and low speed
    KPADMplsCalib_unk calib[2]; // 0x138
    f32 zero[3];              // 0x170: zero drift of each axis (counts)
    s32 histIdx;              // 0x17C
    s32 hist[3][256];         // 0x180
    f64 acc[3];               // 0xD80
    KPADUnifiedWpadStatus* uniRingBuf; // 0xD98
    KPADUnifiedWpadStatus* exRingBuf;  // 0xD9C
    u32 exBufLen;             // 0xDA0
    f32 m0DA4[3];             // 0xDA4
} KPADMplsChan_unk;

// inferred: Motion Plus part of the 2010 WPAD status (in KPADUnifiedWpadStatus.u)
typedef struct KPADMplsRaw_unk
{
    u8 pad_0x0[0x36];
    u8 stat;  // 0x36: bits 2, 3, 1: low speed of pitch, yaw, roll; bit 6 extension; bit 0 extension data
    u8 pad_0x37;
    s16 pitch; // 0x38
    s16 yaw;   // 0x3A
    s16 roll;  // 0x3C
} KPADMplsRaw_unk;

// inferred: raw speeds when the Motion Plus has no own calibration (mode 2)
typedef struct KPADMplsRaw2_unk
{
    u8 pad_0x0[0x2A];
    s16 vy; // 0x2A
    s16 vz; // 0x2C
    s16 vx; // 0x2E
} KPADMplsRaw2_unk;

typedef struct KPADMplsWork_unk
{
    f32 decay;                                        // 0x000
    void (*callback)(KPADChannel chan, KPADMPStatus mp); // 0x004
    KPADMplsChan_unk ch[WPAD_MAX_CONTROLLERS];        // 0x008
} KPADMplsWork_unk;

static KPADMplsWork_unk* kp_mpls_work_unk;

// inferred: counts per revolution (360 degrees) and full scale of each axis, per mode
s32 kp_mpls_unk_tbl_a[3][3] = { { 1600, 1600, 1600 }, { 1600, 1600, 1600 }, { 1600, 1600, 1600 } };
s32 kp_mpls_unk_tbl_b[3][3] = { { 0x7FFFFF, 0x7FFFFF, 0x7FFFFF }, { 0x7FFFFF, 0x7FFFFF, 0x7FFFFF }, { 0x7FFFFF, 0x7FFFFF, 0x7FFFFF } };

// inferred: Motion Plus control result
void fn_80658DB0(KPADChannel chan, BOOL flag)
{
    KPADMplsChan_unk* c = &kp_mpls_work_unk->ch[chan];
    if (kp_mpls_work_unk == NULL)
        return;
    if (!flag)
        c->ctrlResult = 1;
    else
        c->ctrlResult = -1;
}

// inferred: clears the newest history entry (the zero-drift detection starts again)
static void reset_hist(KPADChannel chan)
{
    KPADMplsChan_unk* c = &kp_mpls_work_unk->ch[chan];
    u8 idx;
    if (kp_mpls_work_unk == NULL)
        return;
    idx = (u8)(c->histIdx + 0xFF);
    c->hist[0][idx] = c->hist[1][idx] = c->hist[2][idx] = 0x80000000;
    c->m0EC = 0.0f;
}

void fn_80658DF0(KPADChannel chan)
{
    KPADMplsChan_unk* c = &kp_mpls_work_unk->ch[chan];
    if (kp_mpls_work_unk == NULL)
        return;
    if (c->ctrlResult == 0)
    {
        reset_hist(chan);
        return;
    }
    if (c->ctrlResult < 0)
    {
        c->ctrlResult = 0;
        WPADiControlMpls(chan, 0x80, (WPADCallback)fn_80658DB0);
        return;
    }
    if (1.0f == c->m0EC)
    {
        if (c->m0C4 <= 0.01f)
        {
            c->resetReq = FALSE;
        }
        else
        {
            c->ctrlResult = 0;
            WPADiControlMpls(chan, 0x80, (WPADCallback)fn_80658DB0);
            c->m0C4 -= 0.01f;
            reset_hist(chan);
        }
    }
}

// inferred: requests a zero-drift reset (inlined)
static void set_reset_req(KPADChannel chan)
{
    KPADMplsChan_unk* c = &kp_mpls_work_unk->ch[chan];
    if (kp_mpls_work_unk == NULL)
        return;
    c->resetReq = TRUE;
    c->m0C4 = 0.03f;
}

BOOL fn_80658F30(void)
{
    return kp_mpls_work_unk != NULL;
}

// TLS (2010): functions written further down or in KPADMplsMath_unk.c
void fn_8065A8B0(KPADChannel chan);
void fn_8065AFB0(KPADMPDir* dir, f32 limit);
f32 fn_8065AF20(Vec* v);
void fn_8065B320(KPADMPDir* out, const Vec* a, const Vec* b);
void fn_8065B630(KPADMPDir* a, KPADMPDir* b, KPADMPDir* out);
void fn_8065B7F0(KPADMPDir* a, KPADMPDir* b, KPADMPDir* out, f32 t);

// inferred: same as calc_dpd2pos_scale of KPAD.c
static inline f32 calc_dpd2pos_scale(const Vec2* center)
{
    f32 scale;
    f32 sx, sy;
    sx = 1.0f;
    sy = 0.75f;

    scale = sqrt(sx * sx + sy * sy);

    if (center->x < 0.0f)
    {
        sx += center->x;
    }
    else
    {
        sx -= center->x;
    }

    if (center->y < 0.0f)
    {
        sy += center->y;
    }
    else
    {
        sy -= center->y;
    }

    return scale / ((sx < sy) ? sx : sy);
}

// inferred: dpd2pos scale with the sensor bar height of the channel
// NONMATCHING: 76% - the original does not fold center.x (0.0f kept in f31 across the sqrt call)
__declspec(noinline) void fn_80658F50(KPADChannel chan)
{
    Vec2 center;

    center.x = 0.0f;
    if (kp_mpls_work_unk == NULL)
        return;
    if (kp_mpls_work_unk->ch[chan].sensorBarAuto)
    {
        center.y = WPADGetSensorBarPosition() == WPAD_SENSOR_BAR_TOP ? -0.2f : 0.2f;
    }
    else
    {
        center.y = -kp_mpls_work_unk->ch[chan].sensorHeight;
    }
    kp_mpls_work_unk->ch[chan].dpd2posScale = calc_dpd2pos_scale(&center);
}

// inferred: pulls the axis `v` towards `base` when it moved away from it since `prev`; returns the correction
f32 fn_80659070(KPADMplsChan_unk* c, Vec* v, Vec* prev, Vec* base)
{
    Vec d;
    Vec e;
    f32 lenD;
    f32 dot;
    f32 len;
    f32 k;

    d.x = v->x - prev->x;
    d.y = v->y - prev->y;
    d.z = v->z - prev->z;
    lenD = sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
    e.x = v->x - base->x;
    e.y = v->y - base->y;
    e.z = v->z - base->z;
    if (e.x * d.x + e.y * d.y + e.z * d.z >= 0.0f)
    {
        return 0.0f;
    }
    dot = v->x * e.x + v->y * e.y + v->z * e.z;
    e.x -= dot * v->x;
    e.y -= dot * v->y;
    e.z -= dot * v->z;
    len = sqrt(e.x * e.x + e.y * e.y + e.z * e.z);
    if (len < 0.001f)
    {
        return 0.0f;
    }
    k = lenD * c->dirReviseRate / len;
    v->x -= k * e.x;
    v->y -= k * e.y;
    v->z -= k * e.z;
    return k;
}

// inferred: turns the attitude so that the measured acceleration points down (when its length is near 1 G);
// returns the angle moved
f32 fn_80659220(KPADChannel chan, KPADMPDir* dir, Vec* acc)
{
    KPADMplsChan_unk* c = &kp_mpls_work_unk->ch[chan];
    KPADMPDir rot;
    KPADMPDir tmp;
    Vec t;
    Vec u;
    Vec a;
    f32 len;
    f32 w;
    f32 inv;

    if (kp_mpls_work_unk == NULL)
        return 0.0f;
    len = acc->x * acc->x + acc->y * acc->y + acc->z * acc->z;
    if (0.0f == len)
        return 0.0f;
    len = sqrt(len);
    if (len < 1.0f)
    {
        if (len <= 1.0f - c->accReviseRange)
            return 0.0f;
        w = (1.0f / c->accReviseRange) * (len - (1.0f - c->accReviseRange));
    }
    else
    {
        if (len >= 1.0f + c->accReviseRange)
            return 0.0f;
        w = (-1.0f / c->accReviseRange) * (len - (1.0f + c->accReviseRange));
    }
    w *= w;
    w *= c->accRevisePower;
    inv = 1.0f / len;
    a.x = inv * acc->x;
    a.y = inv * acc->y;
    a.z = inv * acc->z;
    t.x = a.x * dir->X.x + a.y * dir->Y.x + a.z * dir->Z.x;
    t.y = a.x * dir->X.y + a.y * dir->Y.y + a.z * dir->Z.y;
    t.z = a.x * dir->X.z + a.y * dir->Y.z + a.z * dir->Z.z;
    u.x = t.x + -t.x * w;
    u.y = t.y + (-1.0f - t.y) * w;
    u.z = t.z + -t.z * w;
    if (fn_8065AF20(&u) == 0.0f)
        return 0.0f;
    fn_8065B320(&rot, &t, &u);
    fn_8065B630(&rot, dir, &tmp);
    *dir = tmp;
    fn_8065AFB0(dir, 2.999f);
    t.x -= u.x;
    t.y -= u.y;
    t.z -= u.z;
    return sqrt(t.x * t.x + t.y * t.y + t.z * t.z);
}

// inferred: turns the attitude towards the one given by the pointer (DPD) position and horizon; returns the
// largest change of an axis
// NONMATCHING: 96% - 0x10 bytes more of stack (MWCC keeps a temporary for the copies of the Z axis)
f32 fn_806594D0(KPADChannel chan, KPADMPDir* dir, KPADStatus* sp)
{
    KPADMplsChan_unk* c = &kp_mpls_work_unk->ch[chan];
    KPADMPDir m1;
    KPADMPDir m2;
    KPADMPDir rot;
    Vec w;
    Vec v;
    f32 k = 0.383864f;
    f32 len;
    f32 lx, ly, lz;

    if (kp_mpls_work_unk == NULL)
        return 0.0f;
    if (KPADIsEnableAimingMode(chan) != kp_mpls_work_unk->ch[chan].sensorBarAuto ||
        kp_mpls_work_unk->ch[chan].sensorHeight != KPADGetSensorHeight(chan))
    {
        kp_mpls_work_unk->ch[chan].sensorBarAuto = KPADIsEnableAimingMode(chan);
        kp_mpls_work_unk->ch[chan].sensorHeight = KPADGetSensorHeight(chan);
        fn_80658F50(chan);
    }
    k /= c->dpd2posScale;
    if (sp->dpd_valid_fg == 2)
    {
        m1.X.x = sp->horizon.x;
        m1.X.y = sp->horizon.y;
        m1.X.z = 0.0f;
        m1.Y.x = -sp->horizon.y;
        m1.Y.y = sp->horizon.x;
        m1.Y.z = 0.0f;
        m1.Z = fn_unk_identity_dir.Z;
        v = fn_unk_identity_dir.Z;
        w = fn_unk_identity_dir.Z;
        v.x = -k * sp->pos.x;
        fn_8065AF20(&v);
        fn_8065B320(&rot, &w, &v);
        fn_8065B630(&rot, &m1, &m2);
        v = m2.Z;
        w = m2.Z;
        len = sqrt(dir->Z.x * dir->Z.x + dir->Z.z * dir->Z.z);
        v.x *= len;
        v.z *= len;
        v.y = dir->Z.y;
        fn_8065B320(&rot, &w, &v);
        fn_8065B630(&rot, &m2, &m1);
        m2 = *dir;
        fn_8065B7F0(dir, &m1, dir, c->m0E4);
        m2.X.x -= dir->X.x;
        m2.X.y -= dir->X.y;
        m2.X.z -= dir->X.z;
        lx = m2.X.x = m2.X.x * m2.X.x + m2.X.y * m2.X.y + m2.X.z * m2.X.z;
        m2.Y.x -= dir->Y.x;
        m2.Y.y -= dir->Y.y;
        m2.Y.z -= dir->Y.z;
        ly = m2.Y.x = m2.Y.x * m2.Y.x + m2.Y.y * m2.Y.y + m2.Y.z * m2.Y.z;
        m2.Z.x -= dir->Z.x;
        m2.Z.y -= dir->Z.y;
        m2.Z.z -= dir->Z.z;
        lz = m2.Z.x = m2.Z.x * m2.Z.x + m2.Z.y * m2.Z.y + m2.Z.z * m2.Z.z;
        if (lx > ly)
        {
            if (lx > lz)
                return sqrt(lx);
            return sqrt(lz);
        }
        if (ly > lz)
            return sqrt(ly);
        return sqrt(lz);
    }
    return 0.0f;
}

// inferred: turns the attitude by the angular speed of this sample (small-angle rotations about X, Y and Z)
// NONMATCHING: 78% - same formulas; the original computes both X and Y rotations before updating Z and keeps
// the intermediate X/Y values in registers (statement order not found)
void fn_80659860(KPADMplsChan_unk* c)
{
    KPADMPDir d;
    f32 a;
    f32 t;
    f32 r;

    d = c->status.dir;
    a = c->m0A8 * c->status.mpls.x;
    t = c->degInterval * a;
    r = (1.0f + 0.0001f * (t * t)) * (c->m0B4 * a);
    c->status.dir.Y.x += r * d.Z.x;
    c->status.dir.Y.y += r * d.Z.y;
    c->status.dir.Y.z += r * d.Z.z;
    c->status.dir.Z.x -= r * d.Y.x;
    c->status.dir.Z.y -= r * d.Y.y;
    c->status.dir.Z.z -= r * d.Y.z;
    a = c->m0A8 * c->status.mpls.y;
    t = c->degInterval * a;
    r = (1.0f + 0.0001f * (t * t)) * (c->m0B4 * a);
    c->status.dir.Z.x += r * d.X.x;
    c->status.dir.Z.y += r * d.X.y;
    c->status.dir.Z.z += r * d.X.z;
    c->status.dir.X.x -= r * d.Z.x;
    c->status.dir.X.y -= r * d.Z.y;
    c->status.dir.X.z -= r * d.Z.z;
    a = c->m0A8 * c->status.mpls.z;
    t = c->degInterval * a;
    r = (1.0f + 0.0001f * (t * t)) * (c->m0B4 * a);
    c->status.dir.X.x += r * d.Y.x;
    c->status.dir.X.y += r * d.Y.y;
    c->status.dir.X.z += r * d.Y.z;
    c->status.dir.Y.x -= r * d.X.x;
    c->status.dir.Y.y -= r * d.X.y;
    c->status.dir.Y.z -= r * d.X.z;
    fn_8065AFB0(&c->status.dir, 2.999f);
}

// inferred: smoothed angular speed of the pitch axis (counts)
s32 fn_80659B20(KPADMplsChan_unk* c, KPADUnifiedWpadStatus* uwp)
{
    KPADMplsRaw_unk* raw = (KPADMplsRaw_unk*)&uwp->u;
    f32 zero;
    f64 scale;
    f64 d;
    f64 a;

    if (raw->stat & 4)
    {
        zero = c->calib[1].pitchZero;
        scale = c->countScale[1][0];
    }
    else
    {
        zero = c->calib[0].pitchZero;
        scale = c->countScale[0][0];
    }
    d = scale * ((f64)raw->pitch - zero) - c->acc[0];
    a = 50.0 * d;
    if (a < 0.0)
        a = -a;
    if (a < 0.1)
        a = 0.1;
    else if (a > 1.0)
        a = 1.0;
    d *= a;
    c->acc[0] += d;
    a = c->acc[0] * c->m0DA4[0];
    if (a < 0.0)
        return (s32)(a - 0.5);
    return (s32)(0.5 + a);
}

// inferred: smoothed angular speed of the yaw axis (counts)
s32 fn_80659C10(KPADMplsChan_unk* c, KPADUnifiedWpadStatus* uwp)
{
    KPADMplsRaw_unk* raw = (KPADMplsRaw_unk*)&uwp->u;
    f32 zero;
    f64 scale;
    f64 d;
    f64 a;

    if (raw->stat & 8)
    {
        zero = c->calib[1].yawZero;
        scale = c->countScale[1][1];
    }
    else
    {
        zero = c->calib[0].yawZero;
        scale = c->countScale[0][1];
    }
    d = scale * ((f64)raw->yaw - zero) - c->acc[1];
    a = 50.0 * d;
    if (a < 0.0)
        a = -a;
    if (a < 0.1)
        a = 0.1;
    else if (a > 1.0)
        a = 1.0;
    d *= a;
    c->acc[1] += d;
    a = c->acc[1] * c->m0DA4[1];
    if (a < 0.0)
        return (s32)(a - 0.5);
    return (s32)(0.5 + a);
}

// inferred: smoothed angular speed of the roll axis (counts)
s32 fn_80659D00(KPADMplsChan_unk* c, KPADUnifiedWpadStatus* uwp)
{
    KPADMplsRaw_unk* raw = (KPADMplsRaw_unk*)&uwp->u;
    f32 zero;
    f64 scale;
    f64 d;
    f64 a;

    if (raw->stat & 2)
    {
        zero = c->calib[1].rollZero;
        scale = c->countScale[1][2];
    }
    else
    {
        zero = c->calib[0].rollZero;
        scale = c->countScale[0][2];
    }
    d = scale * ((f64)raw->roll - zero) - c->acc[2];
    a = 50.0 * d;
    if (a < 0.0)
        a = -a;
    if (a < 0.1)
        a = 0.1;
    else if (a > 1.0)
        a = 1.0;
    d *= a;
    c->acc[2] += d;
    a = c->acc[2] * c->m0DA4[2];
    if (a < 0.0)
        return (s32)(a - 0.5);
    return (s32)(0.5 + a);
}

// inferred: integrates the Motion Plus samples of this read (zero-drift detection with a history of the
// last samples of each axis)
// NONMATCHING: 96.4% - (1) the original computes the end index of the history loop before the start index
// (subf before subi); (2) the status passed by value to the callback is copied member by member (Vec, Vec,
// KPADMPDir) in the original, as one block here (a member-wise local copy changes the register allocation)
void fn_80659DF0(KPADChannel chan, s32 idx, s32 count)
{
    s32 cntX;
    s32 cntY;
    s32 thX;
    s32 thY;
    s32 minX;
    s32 thZ;
    s32 rangeX;
    s32 rangeY;
    s32 rangeZ;
    s32 n;
    u8 update;
    s32 v;
    KPADMplsChan_unk* c;
    KPADUnifiedWpadStatus* uwp;
    s32 cntZ;
    s32 minY;
    s32 minZ;
    Vec old;
    s32 i;
    s32 d;
    s32 h;
    s32 lo, hi;
    s32 j, end;
    f32 range;
    f32 power;
    f32 r;
    f32 a;

    c = &kp_mpls_work_unk->ch[chan];
    if (count > 0 && kp_mpls_work_unk != NULL)
    {
    if (c->resetReq)
    {
        range = c->m0C4;
        update = TRUE;
        n = (s32)(1500.0f * range);
        power = 0.9f;
    }
    else
    {
        update = kp_mpls_work_unk->ch[chan].zeroDriftUpdate;
        range = kp_mpls_work_unk->ch[chan].zeroDriftRange;
        n = kp_mpls_work_unk->ch[chan].zeroDriftCount;
        power = kp_mpls_work_unk->ch[chan].zeroDriftPower;
    }
    thX = kp_mpls_unk_tbl_b[c->mode][0];
    thY = kp_mpls_unk_tbl_b[c->mode][1];
    thZ = kp_mpls_unk_tbl_b[c->mode][2];
    minX = -thX;
    minY = -thY;
    minZ = -thZ;
    rangeX = fn_8065AED0(range / c->scale[0]);
    rangeY = fn_8065AED0(range / c->scale[1]);
    rangeZ = fn_8065AED0(range / c->scale[2]);
    if (rangeX == 0)
        rangeX++;
    if (rangeY == 0)
        rangeY++;
    if (rangeZ == 0)
        rangeZ++;
    c->m0BB = 0;
    c->m0BA = 0;
    c->m0B9 = 0;
    c->zeroPlayResult = 1.0f;
    c->m0EC = 1.0f;
    do
    {
        i = idx - count - 1;
        if (i < 0)
            i += KPAD_MAX_SAMPLES + c->exBufLen;
        uwp = i >= KPAD_MAX_SAMPLES ? &c->exRingBuf[i - KPAD_MAX_SAMPLES] : &c->uniRingBuf[i];
        old = c->last;
        c->last = c->status.mpls;
        if (uwp->u.core.err != WPAD_ERR_OK)
        {
            c->speedDelta.x *= kp_mpls_work_unk->decay;
            c->status.mpls.x += c->speedDelta.x;
            c->speedDelta.y *= kp_mpls_work_unk->decay;
            c->status.mpls.y += c->speedDelta.y;
            c->speedDelta.z *= kp_mpls_work_unk->decay;
            c->status.mpls.z += c->speedDelta.z;
            c->zeroPlayResult = 0.0f;
            c->m0EC = 0.0f;
        }
        else
        {
        c->histIdx = (u8)(c->histIdx + 1);
        if (c->mode < 2)
            v = fn_80659B20(c, uwp);
        else
            v = ((KPADMplsRaw2_unk*)&uwp->u)->vx;
        c->hist[0][c->histIdx] = v;
        cntX = 1;
        d = v - fn_8065AED0(c->zero[0]);
        if (d <= minX || d >= thX)
        {
            c->speedDelta.x *= kp_mpls_work_unk->decay;
            c->status.mpls.x += c->speedDelta.x;
            c->m0B9++;
            c->zeroPlayResult = 0.0f;
        }
        else
        {
            c->status.mpls.x = v;
            lo = v - rangeX;
            hi = v + rangeX;
            end = (u8)(c->histIdx - n);
            j = (u8)(c->histIdx - 1);
            do
            {
                h = c->hist[0][j];
                if (h < lo || h > hi)
                    break;
                j = (u8)(j - 1);
                v += h;
                cntX++;
            } while (j != end);
            r = (f32)(cntX - 1) / (f32)(n - 1);
            r *= r;
            r *= r;
            r *= r;
            r *= r;
            r *= r;
            c->status.mpls.x += r * ((f32)v / (f32)cntX - c->status.mpls.x);
            if (update)
                c->zero[0] += r * power * (c->status.mpls.x - c->zero[0]);
            c->status.mpls.x = c->scale[0] * (c->status.mpls.x - c->zero[0]);
            if (c->zeroPlayEnabled)
            {
                if (c->status.mpls.x >= -c->zeroPlay && c->status.mpls.x <= c->zeroPlay)
                {
                    a = c->status.mpls.x;
                    if (a < 0.0)
                        a = -a;
                    a = 1.0f - a / c->zeroPlay;
                    if (a < c->zeroPlayResult)
                        c->zeroPlayResult = a;
                    c->status.mpls.x = 0.0f;
                }
                else
                {
                    c->zeroPlayResult = 0.0f;
                }
            }
            if (c->mode == 1)
                c->speedDelta.x = 0.5f * (c->status.mpls.x - old.x);
            else
                c->speedDelta.x = c->status.mpls.x - c->last.x;
        }
        if (c->mode < 2)
            v = fn_80659C10(c, uwp);
        else
            v = -((KPADMplsRaw2_unk*)&uwp->u)->vy;
        c->hist[1][c->histIdx] = v;
        cntY = 1;
        d = v - fn_8065AED0(c->zero[1]);
        if (d <= minY || d >= thY)
        {
            c->speedDelta.y *= kp_mpls_work_unk->decay;
            c->status.mpls.y += c->speedDelta.y;
            c->m0BA++;
            c->zeroPlayResult = 0.0f;
        }
        else
        {
            c->status.mpls.y = v;
            lo = v - rangeY;
            hi = v + rangeY;
            end = (u8)(c->histIdx - n);
            j = (u8)(c->histIdx - 1);
            do
            {
                h = c->hist[1][j];
                if (h < lo || h > hi)
                    break;
                j = (u8)(j - 1);
                v += h;
                cntY++;
            } while (j != end);
            r = (f32)(cntY - 1) / (f32)(n - 1);
            r *= r;
            r *= r;
            r *= r;
            r *= r;
            r *= r;
            c->status.mpls.y += r * ((f32)v / (f32)cntY - c->status.mpls.y);
            if (update)
                c->zero[1] += r * power * (c->status.mpls.y - c->zero[1]);
            c->status.mpls.y = c->scale[1] * (c->status.mpls.y - c->zero[1]);
            if (c->zeroPlayEnabled)
            {
                if (c->status.mpls.y >= -c->zeroPlay && c->status.mpls.y <= c->zeroPlay)
                {
                    a = c->status.mpls.y;
                    if (a < 0.0)
                        a = -a;
                    a = 1.0f - a / c->zeroPlay;
                    if (a < c->zeroPlayResult)
                        c->zeroPlayResult = a;
                    c->status.mpls.y = 0.0f;
                }
                else
                {
                    c->zeroPlayResult = 0.0f;
                }
            }
            if (c->mode == 1)
                c->speedDelta.y = 0.5f * (c->status.mpls.y - old.y);
            else
                c->speedDelta.y = c->status.mpls.y - c->last.y;
        }
        if (c->mode < 2)
            v = fn_80659D00(c, uwp);
        else
            v = ((KPADMplsRaw2_unk*)&uwp->u)->vz;
        c->hist[2][c->histIdx] = v;
        cntZ = 1;
        d = v - fn_8065AED0(c->zero[2]);
        if (d <= minZ || d >= thZ)
        {
            c->speedDelta.z *= kp_mpls_work_unk->decay;
            c->status.mpls.z += c->speedDelta.z;
            c->m0BB++;
            c->zeroPlayResult = 0.0f;
        }
        else
        {
            c->status.mpls.z = v;
            lo = v - rangeZ;
            hi = v + rangeZ;
            end = (u8)(c->histIdx - n);
            j = (u8)(c->histIdx - 1);
            do
            {
                h = c->hist[2][j];
                if (h < lo || h > hi)
                    break;
                j = (u8)(j - 1);
                v += h;
                cntZ++;
            } while (j != end);
            r = (f32)(cntZ - 1) / (f32)(n - 1);
            r *= r;
            r *= r;
            r *= r;
            r *= r;
            r *= r;
            c->status.mpls.z += r * ((f32)v / (f32)cntZ - c->status.mpls.z);
            if (update)
                c->zero[2] += r * power * (c->status.mpls.z - c->zero[2]);
            c->status.mpls.z = c->scale[2] * (c->status.mpls.z - c->zero[2]);
            if (c->zeroPlayEnabled)
            {
                if (c->status.mpls.z >= -c->zeroPlay && c->status.mpls.z <= c->zeroPlay)
                {
                    a = c->status.mpls.z;
                    if (a < 0.0)
                        a = -a;
                    a = 1.0f - a / c->zeroPlay;
                    if (a < c->zeroPlayResult)
                        c->zeroPlayResult = a;
                    c->status.mpls.z = 0.0f;
                }
                else
                {
                    c->zeroPlayResult = 0.0f;
                }
            }
            if (c->mode == 1)
                c->speedDelta.z = 0.5f * (c->status.mpls.z - old.z);
            else
                c->speedDelta.z = c->status.mpls.z - c->last.z;
        }
        if (cntX < cntY)
        {
            if (cntX < cntZ)
                cntZ = cntX;
        }
        else if (cntY < cntZ)
        {
            cntZ = cntY;
        }
        r = (f32)(cntZ - 1) / (f32)(n - 1);
        if (r < c->m0EC)
            c->m0EC = r;
        }
        c->status.angle.x += c->interval * c->status.mpls.x;
        c->status.angle.y += c->interval * c->status.mpls.y;
        c->status.angle.z += c->interval * c->status.mpls.z;
        fn_80659860(c);
        if (kp_mpls_work_unk->callback)
            kp_mpls_work_unk->callback(chan, c->status);
    } while (--count != 0);
    kp_mpls_work_unk->callback = NULL;
    }
}

// inferred: counts per degree of each axis from the WPAD calibration (high and low speed)
// NONMATCHING: 97% - the original keeps the 0.0f of the checks in f4 for the final m0EC = 0.0f
void fn_8065A8B0(KPADChannel chan)
{
    KPADMplsChan_unk* c = &kp_mpls_work_unk->ch[chan];
    f64 k;
    if (kp_mpls_work_unk == NULL)
        return;
    if (c->mode >= 2)
    {
        c->countScale[0][0] = 0.0;
        return;
    }
    WPADiGetMplsCalibration(chan, &c->calib[0], &c->calib[1]);
    if (0.0f == c->calib[0].pitchScale || 0.0f == c->calib[0].yawScale || 0.0f == c->calib[0].rollScale ||
        0.0f == c->calib[1].pitchScale || 0.0f == c->calib[1].yawScale || 0.0f == c->calib[1].rollScale)
    {
        c->countScale[0][0] = 0.0;
        return;
    }
    k = 5242.879375 * c->calib[0].degrees;
    c->countScale[0][0] = k / c->calib[0].pitchScale;
    c->countScale[0][1] = k / c->calib[0].yawScale;
    c->countScale[0][2] = k / c->calib[0].rollScale;
    k = 5242.879375 * c->calib[1].degrees;
    c->countScale[1][0] = k / c->calib[1].pitchScale;
    c->countScale[1][1] = k / c->calib[1].yawScale;
    c->countScale[1][2] = k / c->calib[1].rollScale;
    c->ctrlResult = 1;
    set_reset_req(chan);
    reset_hist(chan);
}

void fn_8065AA50(KPADChannel chan, KPADUnifiedWpadStatus* uniRingBuf, KPADUnifiedWpadStatus* exRingBuf, u32 exBufLen)
{
    KPADMplsChan_unk* c = &kp_mpls_work_unk->ch[chan];
    if (kp_mpls_work_unk == NULL)
        return;
    c->uniRingBuf = uniRingBuf;
    c->exRingBuf = exRingBuf;
    c->exBufLen = exBufLen;
}


static void set_mode(KPADChannel chan, s32 mode)
{
    KPADMplsChan_unk* c = &kp_mpls_work_unk->ch[chan];
    if (kp_mpls_work_unk == NULL)
        return;
    if (c->mode != mode)
    {
        c->countScale[0][0] = 0.0;
    }
    c->mode = mode;
    c->scale[0] = 1.0 / ((360.0 / kp_mpls_unk_tbl_a[c->mode][0]) * kp_mpls_unk_tbl_b[c->mode][0]);
    c->scale[1] = 1.0 / ((360.0 / kp_mpls_unk_tbl_a[c->mode][1]) * kp_mpls_unk_tbl_b[c->mode][1]);
    c->scale[2] = 1.0 / ((360.0 / kp_mpls_unk_tbl_a[c->mode][2]) * kp_mpls_unk_tbl_b[c->mode][2]);
}

static f32 revise_dir(KPADChannel chan)
{
    KPADMplsChan_unk* c = &kp_mpls_work_unk->ch[chan];
    f32 max;
    f32 r;
    if (kp_mpls_work_unk == NULL)
        return 0.0f;
    max = fn_80659070(c, &c->status.dir.X, &c->prevDir.X, &c->base[0]);
    r = fn_80659070(c, &c->status.dir.Y, &c->prevDir.Y, &c->base[1]);
    if (r > max)
        max = r;
    r = fn_80659070(c, &c->status.dir.Z, &c->prevDir.Z, &c->base[2]);
    if (r > max)
        max = r;
    fn_8065AFB0(&c->status.dir, 2.999f);
    return max;
}

// inferred: Motion Plus update of one read (called from KPADiRead)
// NONMATCHING: 99.8% - only the order of the parameter copies in the prologue (mr r28, r7 first)
void fn_8065AA80(KPADChannel chan, KPADMPStatus* mp, s32 idx, s32 count, KPADStatus* sp, f64 interval)
{
    KPADMplsChan_unk* c = &kp_mpls_work_unk->ch[chan];
    u32 type;
    s32 mode;
    u8 st;
    if (kp_mpls_work_unk == NULL)
        return;
    mode = 2;
    if (WPADProbe(chan, (WPADDeviceType*)&type) != WPAD_ERR_NO_CONTROLLER && ((type >= 5 && type <= 7) || type == 250))
    {
        st = WPADiGetMplsStatus(chan);
        if (st == 4)
        {
            mode = 0;
        }
        else if (st != 0)
        {
            mode = type != 5;
            c->mode = mode;
        }
    }
    set_mode(chan, mode);
    if (count > 0 && sp && mode != 2)
    {
        c->interval = interval;
        c->degInterval = 360.0 * interval;
        c->m0B4 = 6.283185307179586 * interval;
        if (c->countScale[0][0] == 0.0)
        {
            fn_8065A8B0(chan);
        }
        c->prevDir = c->status.dir;
        fn_80659DF0(chan, idx, count);
        if (c->resetReq)
        {
            fn_80658DF0(chan);
        }
        if (c->dirReviseEnabled)
        {
            c->dirReviseResult = revise_dir(chan);
        }
        else
        {
            c->dirReviseResult = 0.0f;
        }
        if (c->accReviseEnabled)
        {
            c->accReviseResult = fn_80659220(chan, &c->status.dir, &sp->acc);
        }
        else
        {
            c->accReviseResult = 0.0f;
        }
        if (c->dpdReviseEnabled)
        {
            c->dpdReviseResult = fn_806594D0(chan, &c->status.dir, sp);
        }
        else
        {
            c->dpdReviseResult = 0.0f;
        }
    }
    mp->mpls = c->status.mpls;
    mp->angle = c->status.angle;
    mp->dir = c->status.dir;
}

void fn_8065AEB0(void (*callback)(KPADChannel chan, KPADMPStatus mp))
{
    if (kp_mpls_work_unk == NULL)
        return;
    kp_mpls_work_unk->callback = callback;
}
