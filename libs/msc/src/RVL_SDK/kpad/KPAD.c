// Source: mscharged-decomp src/RVL_SDK/kpad/R4QK01/KPAD.c (CC0), with the TLS (2010) version string.
// Only the functions whose size did not change in 2010 match (KPADInsideStatus/KPADStatus padded
// to the 2010 layout in libs/msc/libs/RVL_SDK/include/revolution/kpad); the rest is the 2009 code.
#include <revolution/kpad/KPADPriv.h>
#include <revolution/wbc/WBC.h>

#include <internal/fdlibm.h>

static const char* __KPADVersion = "<< RVL_SDK - KPAD \trelease build: Aug 23 2010 19:48:23 (0x4302_145) >>";

static Vec2 icenter_org = { 0.000f, 0.000f };
static f32 idist_org = 1.000f;
static Vec2 iaccXY_nrm_hori = { 0.000f, -1.000f };
static Vec2 isec_nrm_hori = { 1.000f, 0.000f };
f32 kp_obj_interval = 0.200f;

f32 kp_acc_horizon_pw = 0.050f;
f32 kp_ah_circle_radius = 0.070f;
f32 kp_ah_circle_pw = 0.060f;
u16 kp_ah_circle_ct = 100;
BOOL kp_stick_clamp_cross = FALSE;

f32 kp_err_outside_frame = 0.050f;
f32 kp_err_dist_min;
f32 kp_err_dist_max = 3.000f;
f32 kp_err_dist_speed = 0.040f;
f32 kp_err_first_inpr = 0.900f;
f32 kp_err_next_inpr = 0.900f;
f32 kp_err_acc_inpr = 0.900f;
f32 kp_err_up_inpr = 0.700f;
f32 kp_err_near_pos = 0.100f;

static f32 kp_dist_vv1;

static s32 kp_fs_fstick_min = 15;
static s32 kp_fs_fstick_max = 71;
static s32 kp_cl_stick_min = 60;
static s32 kp_cl_stick_max = 308;
static s32 kp_cl_trigger_min = 30;
static s32 kp_cl_trigger_max = 180;
static f32 kp_rm_acc_max = 3.4f;
static f32 kp_fs_acc_max = 2.1f;

static s32 kp_tr_trigger_min = 0;
static s32 kp_tr_trigger_max = 256;
static s32 kp_guitar_stick_min = 0;
static s32 kp_guitar_stick_max = 1024;
static u8 kp_initialized = 0;
KPADInsideStatus inside_kpads[4] = { 0 };
static Vec2 Vec2_0 = { 0.0f, 0.0f };
static Mtx kp_fs_rot = { 0 };
static f64 kp_wbc_tgc_weight = 0.0;
static f64 kp_wbc_filtered[4] = { 0.0, 0.0, 0.0, 0.0 }; // TLS (2010): before kp_wbc_average
static f64 kp_wbc_average[4] = { 0.0, 0.0, 0.0, 0.0 };
static u16 kp_wbc_count = 0;
static u8 kp_wbc_status = 0;
static s8 kp_wbc_enabled = 0; // TLS (2010): s8
static u8 kp_wbc_busy = 0;
static u8 kp_wbc_wait_count = 200; // TLS (2010): 200
static f32 kp_wbc_ave_count = 400.0f;
// inferred: TLS (2010) balance board state (.sbss 0x808801D2-0x808801D8), names invented
static u8 kp_wbc_unk_d2;
static s8 kp_wbc_state;
static u8 kp_wbc_unk_d5;
static s16 kp_wbc_unk_d6;
static s8 kp_wbc_err;
static f32 kp_wbc_unk_b0;
static f64 kp_wbc_unk_b8;
static f64 kp_wbc_unk_c0;
static u8 kp_wbc_unk_tbl[] = { 2, 2, 3, 5, 5, 5 };
static f32 kp_fs_revise_deg = 24.0f;

void KPADSetBtnRepeat(KPADChannel chan, f32 delay_sec, f32 pulse_sec)
{
    KPADInsideStatus* kp = &inside_kpads[chan];
    if (pulse_sec)
    {
        kp->btn_repeat_delay = (u16)(s32)(delay_sec * 200.0f + 0.5f);
        kp->btn_repeat_pulse = (u16)(s32)(pulse_sec * 200.0f + 0.5f);
    }
    else
    {
        kp->btn_repeat_delay = KPAD_BTN_NO_RPT_DELAY;
        kp->btn_repeat_pulse = 0;
    }

    kp->btn_repeat_time = 0;
    kp->btn_repeat_next = kp->btn_repeat_delay;
    kp->btn_cl_repeat_time = 0;
    kp->btn_cl_repeat_next = kp->btn_repeat_delay;
}

void KPADSetPosParam(KPADChannel chan, f32 play_radius, f32 sensitivity)
{
    inside_kpads[chan].pos_play_radius = play_radius;
    inside_kpads[chan].pos_sensitivity = sensitivity;
}

void KPADSetHoriParam(KPADChannel chan, f32 play_radius, f32 sensitivity)
{
    inside_kpads[chan].hori_play_radius = play_radius;
    inside_kpads[chan].hori_sensitivity = sensitivity;
}

void KPADSetDistParam(KPADChannel chan, f32 play_radius, f32 sensitivity)
{
    inside_kpads[chan].dist_play_radius = play_radius;
    inside_kpads[chan].dist_sensitivity = sensitivity;
}

void KPADSetAccParam(KPADChannel chan, f32 play_radius, f32 sensitivity)
{
    inside_kpads[chan].acc_play_radius = play_radius;
    inside_kpads[chan].acc_sensitivity = sensitivity;
}

static void calc_dpd2pos_scale(KPADInsideStatus* kp)
{
    f32 scale;
    f32 sx, sy;
    sx = 1.0f;
    sy = (f32)KPAD_DPD_RESO_WY / (f32)KPAD_DPD_RESO_WX;

    scale = sqrt(sx * sx + sy * sy);

    if (kp->center_org.x < 0.0f)
    {
        sx += kp->center_org.x;
    }
    else
    {
        sx -= kp->center_org.x;
    }

    if (kp->center_org.y < 0.0f)
    {
        sy += kp->center_org.y;
    }
    else
    {
        sy -= kp->center_org.y;
    }

    kp->dpd2pos_scale = scale / ((sx < sy) ? sx : sy);
}

// TLS (2010): takes the channel instead of the status pointer
void reset_kpad(KPADChannel chan)
{
    KPADInsideStatus* kp = &inside_kpads[chan];
    KPADObject* op;
    KPADStatus* sp = &kp->status;
    KPADEXStatus* ep = &kp->status.ex_status;

    kp->resetReq = FALSE;

    kp->kobj_frame_min.x = -1.0f + kp_err_outside_frame;
    kp->kobj_frame_max.x = 1.0f - kp_err_outside_frame;
    kp->kobj_frame_min.y = -((f32)KPAD_DPD_RESO_WY / KPAD_DPD_RESO_WX) + kp_err_outside_frame;
    kp->kobj_frame_max.y = ((f32)KPAD_DPD_RESO_WY / KPAD_DPD_RESO_WX) - kp_err_outside_frame;

    kp->err_dist_speed_1 = 1.0f / kp_err_dist_speed;
    kp->err_dist_speedM_1 = -1.0f / kp_err_dist_speed;
    kp->ah_circle_radius2 = kp_ah_circle_radius * kp_ah_circle_radius;

    kp->err_dist_min = kp_err_dist_min;
    kp->dist_vv1 = kp_dist_vv1;

    sp->hold = sp->trig = sp->release = 0;
    kp->btn_repeat_time = 0;
    kp->btn_repeat_next = kp->btn_repeat_delay;

    sp->dpd_valid_fg = 0;
    kp->dpd_valid2_ct = 0;

    sp->pos = sp->vec = Vec2_0;
    sp->speed = 0.0f;

    sp->horizon.x = kp->acc_horizon.x = kp->obj_horizon.x = 1.0f;
    sp->horizon.y = kp->acc_horizon.y = kp->obj_horizon.y = 0.0f;
    sp->hori_vec = Vec2_0;
    sp->hori_speed = 0.0f;

    sp->acc_vertical.x = 1.0f;
    sp->acc_vertical.y = 0.0f;

    sp->dist = kp->dist_org;
    sp->dist_vec = sp->dist_speed = 0.0f;

    kp->sec_dist = sp->dist;
    kp->sec_length = kp->trust_sec_length = kp->dist_vv1 / kp->sec_dist;
    kp->sec_nrm = kp->sec_nrm_hori;

    sp->acc.x = sp->acc.z = 0.0f;
    sp->acc.y = -1.0f;
    sp->acc_value = 1.0f;
    sp->acc_speed = 0.0f;
    kp->hard_acc = sp->acc;

    kp->ah_circle_pos = kp->acc_horizon;
    kp->ah_circle_ct = kp_ah_circle_ct;

    kp->valid_objs = 0;

    op = &kp->kobj_sample[WPAD_MAX_DPD_OBJECTS - 1];
    do
    {
        op->error_fg = -1;
    } while (--op >= kp->kobj_sample);

    op = &kp->kobj_regular[KPAD_USE_OBJECTS - 1];
    do
    {
        op->error_fg = -1;
    } while (--op >= kp->kobj_regular);

    kp->bufCount = 0;
    kp->exResetReq = TRUE;
}

void KPADSetSensorHeight(KPADChannel chan, f32 level)
{
    KPADInsideStatus* kp = &inside_kpads[chan];
    kp->center_org.x = 0.0f;
    kp->center_org.y = -level;
    calc_dpd2pos_scale(kp);
}

// TLS (2010): aiming-mode and sensor-height accessors of the later KPAD API
void KPADEnableAimingMode(KPADChannel chan)
{
    KPADInsideStatus* kp = &inside_kpads[chan];
    kp->aimReq = TRUE;
    kp->aimEnabled = TRUE;
}

u8 KPADIsEnableAimingMode(KPADChannel chan)
{
    return inside_kpads[chan].aimEnabled;
}

f32 KPADGetSensorHeight(KPADChannel chan)
{
    return inside_kpads[chan].center_org.y;
}

void calc_button_repeat(KPADInsideStatus* kp, u32 dev_type, u32 count)
{
    KPADStatus* sp = &kp->status;
    KPADEXStatus* ep = &kp->status.ex_status;

    if (sp->trig != 0 || sp->release != 0)
    {
        kp->btn_repeat_time = 0;
        kp->btn_repeat_next = kp->btn_repeat_delay;
        if (sp->trig && kp->btn_repeat_pulse)
        {
            sp->hold |= KPAD_BUTTON_RPT;
        }
    }
    else if (sp->hold != 0)
    {
        kp->btn_repeat_time += count;
        if (kp->btn_repeat_time >= KPAD_BTN_NO_RPT_DELAY)
        {
            kp->btn_repeat_time -= KPAD_BTN_NO_RPT_DELAY;
        }

        if (kp->btn_repeat_time >= kp->btn_repeat_next)
        {
            sp->hold |= KPAD_BUTTON_RPT;
            kp->btn_repeat_next += kp->btn_repeat_pulse;
            if (kp->btn_repeat_time >= KPAD_BTN_RPT_TIME_MAX)
            {
                kp->btn_repeat_time -= KPAD_BTN_RPT_TIME_MAX;
                kp->btn_repeat_next -= KPAD_BTN_RPT_TIME_MAX;
            }
        }
    }

    // TLS (2010): devices 16-19 and 7 also report classic-style buttons
    if (dev_type == WPAD_DEV_TRAIN || dev_type == WPAD_DEV_GUITAR || dev_type == WPAD_DEV_DRUM || dev_type == 19
        || dev_type == WPAD_DEV_CLASSIC || dev_type == 7)
    {
        if (ep->cl.trig != 0 || ep->cl.release != 0)
        {
            kp->btn_cl_repeat_time = 0;
            kp->btn_cl_repeat_next = kp->btn_repeat_delay;
            if (ep->cl.trig && kp->btn_repeat_pulse)
            {
                ep->cl.hold |= KPAD_BUTTON_RPT;
            }
        }
        else if (ep->cl.hold != 0)
        {
            kp->btn_cl_repeat_time += count;
            if (kp->btn_cl_repeat_time >= KPAD_BTN_NO_RPT_DELAY)
            {
                kp->btn_cl_repeat_time -= KPAD_BTN_NO_RPT_DELAY;
            }

            if (kp->btn_cl_repeat_time >= kp->btn_cl_repeat_next)
            {
                ep->cl.hold |= KPAD_BUTTON_RPT;
                kp->btn_cl_repeat_next += kp->btn_repeat_pulse;
                if (kp->btn_cl_repeat_time >= KPAD_BTN_RPT_TIME_MAX)
                {
                    kp->btn_cl_repeat_time -= KPAD_BTN_RPT_TIME_MAX;
                    kp->btn_cl_repeat_next -= KPAD_BTN_RPT_TIME_MAX;
                }
            }
        }
    }
}

void read_kpad_button(KPADInsideStatus* kp, u32 dev_type, u32 count, u32 core, u32 fs, u32 cl)
{
    KPADStatus* sp = &kp->status;
    KPADEXStatus* ep = &kp->status.ex_status;
    u32 old_fg, change_fg;
    old_fg = sp->hold & KPAD_BUTTON_MASK;
    sp->hold = core & (KPAD_BUTTON_MASK & ~(WPAD_BUTTON_FS_Z | WPAD_BUTTON_FS_C));
    if (dev_type == WPAD_DEV_FREESTYLE || dev_type == 6) // TLS (2010): also device 6
    {
        sp->hold |= (kp->exResetReq ? 0 : fs) & (WPAD_BUTTON_FS_Z | WPAD_BUTTON_FS_C);
    }
    change_fg = sp->hold ^ old_fg;
    sp->trig = change_fg & sp->hold;
    sp->release = change_fg & old_fg;

    if (dev_type == WPAD_DEV_TRAIN || dev_type == WPAD_DEV_GUITAR || dev_type == WPAD_DEV_DRUM || dev_type == 19
        || dev_type == WPAD_DEV_CLASSIC || dev_type == 7)
    {
        cl = kp->exResetReq ? 0 : cl;
        old_fg = ep->cl.hold & KPAD_BUTTON_MASK;
        ep->cl.hold = cl & KPAD_BUTTON_MASK;
        change_fg = ep->cl.hold ^ old_fg;
        ep->cl.trig = change_fg & ep->cl.hold;
        ep->cl.release = change_fg & old_fg;
    }

    calc_button_repeat(kp, dev_type, count);
}

void calc_acc(KPADInsideStatus* kp, f32* acc, f32 acc2)
{
    f32 f1, f2;

    f2 = acc2 - *acc;

    if (kp->acc_play_mode == KPAD_PLAY_MODE_LOOSE)
    {
        if (f2 < 0.0f)
        {
            f1 = -f2;
        }
        else
        {
            f1 = f2;
        }

        if (f1 >= kp->acc_play_radius)
        {
            f1 = 1.0f;
        }
        else
        {
            f1 /= kp->acc_play_radius;
            f1 *= f1;
            f1 *= f1;
        }
        f1 *= kp->acc_sensitivity;
        *acc += f1 * f2;
    }
    else
    {
        if (f2 < -kp->acc_play_radius)
        {
            *acc += (f2 + kp->acc_play_radius) * kp->acc_sensitivity;
        }
        else if (f2 > kp->acc_play_radius)
        {
            *acc += (f2 - kp->acc_play_radius) * kp->acc_sensitivity;
        }
    }
}

void calc_acc_horizon(KPADInsideStatus* kp)
{
    f32 f1, vx, vy, ax, ay;
    f1 = sqrt(kp->hard_acc.x * kp->hard_acc.x + kp->hard_acc.y * kp->hard_acc.y);
    if (f1 == 0.0f || f1 >= 2.0f)
    {
        return;
    }
    ax = kp->hard_acc.x / f1;
    ay = kp->hard_acc.y / f1;

    if (f1 > 1.0f)
    {
        f1 = 2.0f - f1;
    }
    f1 *= f1 * kp_acc_horizon_pw;

    vx = kp->accXY_nrm_hori.x * ax + kp->accXY_nrm_hori.y * ay;
    vy = kp->accXY_nrm_hori.y * ax - kp->accXY_nrm_hori.x * ay;
    ax = (vx - kp->acc_horizon.x) * f1 + kp->acc_horizon.x;
    ay = (vy - kp->acc_horizon.y) * f1 + kp->acc_horizon.y;

    f1 = sqrt(ax * ax + ay * ay);
    if (f1 == 0.0f)
    {
        return;
    }
    kp->acc_horizon.x = ax / f1;
    kp->acc_horizon.y = ay / f1;

    kp->ah_circle_pos.x += (kp->acc_horizon.x - kp->ah_circle_pos.x) * kp_ah_circle_pw;
    kp->ah_circle_pos.y += (kp->acc_horizon.y - kp->ah_circle_pos.y) * kp_ah_circle_pw;

    vx = kp->acc_horizon.x - kp->ah_circle_pos.x;
    vy = kp->acc_horizon.y - kp->ah_circle_pos.y;
    if (vx * vx + vy * vy <= kp->ah_circle_radius2)
    {
        if (kp->ah_circle_ct)
            --kp->ah_circle_ct;
    }
    else
    {
        kp->ah_circle_ct = kp_ah_circle_ct;
    }
}

static void clamp_trigger(f32* trigger, s32 tr, s32 min, s32 max)
{
    if (tr <= min)
    {
        *trigger = 0.0f;
    }
    else if (tr >= max)
    {
        *trigger = 1.0f;
    }
    else
    {
        *trigger = (f32)(tr - min) / (f32)(max - min);
    }
}

void calc_acc_vertical(KPADInsideStatus* kp)
{
    KPADStatus* sp = &kp->status;
    f32 f1, f2, ax, ay;

    ax = sqrt(f2 = kp->hard_acc.x * kp->hard_acc.x + kp->hard_acc.y * kp->hard_acc.y);
    ay = -kp->hard_acc.z;
    f1 = sqrt(f2 + ay * ay);
    if (f1 == 0.0f || f1 >= 2.0f)
    {
        return;
    }
    f2 = ax / f1;
    ay /= f1;

    if (f1 > 1.0f)
    {
        f1 = 2.0f - f1;
    }
    f1 *= f1 * kp_acc_horizon_pw;

    ax = (f2 - sp->acc_vertical.x) * f1 + sp->acc_vertical.x;
    ay = (ay - sp->acc_vertical.y) * f1 + sp->acc_vertical.y;

    f1 = sqrt(ax * ax + ay * ay);
    if (f1 == 0.0f)
    {
        return;
    }
    sp->acc_vertical.x = ax / f1;
    sp->acc_vertical.y = ay / f1;
}

static f32 clamp_acc(f32 acc, f32 clamp)
{
    if (acc < 0.0f)
    {
        if (acc < -clamp)
            return -clamp;
    }
    else
    {
        if (acc > clamp)
            return clamp;
    }
    return acc;
}

void read_kpad_acc(KPADInsideStatus* kp, KPADUnifiedWpadStatus* uwp)
{
    KPADStatus* sp = &kp->status;
    Vec fsrc;
    Vec vec;

    switch (uwp->fmt)
    {
    case WPAD_FMT_CORE_BTN_ACC:
    case WPAD_FMT_CORE_BTN_ACC_DPD:
    case WPAD_FMT_FS_BTN_ACC:
    case WPAD_FMT_FS_BTN_ACC_DPD:
    case WPAD_FMT_CLASSIC_BTN_ACC:
    case WPAD_FMT_CLASSIC_BTN_ACC_DPD:
    case WPAD_FMT_GUITAR:
    case 15: // TLS (2010): later SDK formats
    case 16:
    case 17:
    case 19:
    case 20:
        break;

    default:
        return;
    }

    kp->hard_acc.x = clamp_acc((f32)(s32)-uwp->u.core.accX * kp->acc_scale_x, kp_rm_acc_max);
    kp->hard_acc.y = clamp_acc((f32)(s32)-uwp->u.core.accZ * kp->acc_scale_z, kp_rm_acc_max);
    kp->hard_acc.z = clamp_acc((f32)(s32)uwp->u.core.accY * kp->acc_scale_y, kp_rm_acc_max);

    vec = sp->acc;

    calc_acc(kp, &sp->acc.x, kp->hard_acc.x);
    calc_acc(kp, &sp->acc.y, kp->hard_acc.y);
    calc_acc(kp, &sp->acc.z, kp->hard_acc.z);
    sp->acc_value = sqrt(sp->acc.x * sp->acc.x + sp->acc.y * sp->acc.y + sp->acc.z * sp->acc.z);

    vec.x -= sp->acc.x;
    vec.y -= sp->acc.y;
    vec.z -= sp->acc.z;
    sp->acc_speed = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    calc_acc_horizon(kp);
    calc_acc_vertical(kp);
    // TLS (2010): device 6 with format 16 also carries the extension accelerometer
    if ((uwp->u.fs.err == WPAD_ERR_OK && uwp->u.fs.dev == WPAD_DEV_FREESTYLE && (uwp->fmt == WPAD_FMT_FS_BTN_ACC || uwp->fmt == WPAD_FMT_FS_BTN_ACC_DPD)) ||
        (uwp->u.fs.dev == 6 && uwp->fmt == 16 && (uwp->u.unk_tls[0x36] & 0x40)))
    {
        if (uwp->u.fs.dev == WPAD_DEV_FREESTYLE)
        {
            fsrc.x = clamp_acc((f32)(s32)-uwp->u.fs.fsAccX * kp->fs_acc_scale_x, kp_fs_acc_max);
            fsrc.y = clamp_acc((f32)(s32)-uwp->u.fs.fsAccZ * kp->fs_acc_scale_z, kp_fs_acc_max);
            fsrc.z = clamp_acc((f32)(s32)uwp->u.fs.fsAccY * kp->fs_acc_scale_y, kp_fs_acc_max);
        }
        else
        {
            fsrc.x = clamp_acc((f32)(s32)-uwp->u.fs.fsAccX * kp->fs_acc_scale_x, kp_fs_acc_max);
            fsrc.y = clamp_acc((f32)(s32)-uwp->u.fs.fsAccZ * kp->fs_acc_scale_z, kp_fs_acc_max);
            fsrc.z = clamp_acc((f32)(s32)uwp->u.fs.fsAccY * kp->fs_acc_scale_y, kp_fs_acc_max);
        }

        if (kp->fsAccRevise)
        {
            PSMTXMultVec(kp_fs_rot, &fsrc, &fsrc);
        }

        vec = sp->ex_status.fs.acc;

        calc_acc(kp, &sp->ex_status.fs.acc.x, fsrc.x);
        calc_acc(kp, &sp->ex_status.fs.acc.y, fsrc.y);
        calc_acc(kp, &sp->ex_status.fs.acc.z, fsrc.z);
        sp->ex_status.fs.acc_value = sqrt(sp->ex_status.fs.acc.x * sp->ex_status.fs.acc.x + sp->ex_status.fs.acc.y * sp->ex_status.fs.acc.y + sp->ex_status.fs.acc.z * sp->ex_status.fs.acc.z);

        vec.x -= sp->ex_status.fs.acc.x;
        vec.y -= sp->ex_status.fs.acc.y;
        vec.z -= sp->ex_status.fs.acc.z;
        sp->ex_status.fs.acc_speed = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }
}

static f32 calc_horizon(KPADInsideStatus* kp, Vec2* p1, Vec2* p2, Vec2* hori)
{
    f32 f1, f2, vx, vy;

    vx = p2->x - p1->x;
    vy = p2->y - p1->y;
    f2 = sqrt(vx * vx + vy * vy);
    f1 = 1.0f / f2;
    vx *= f1;
    vy *= f1;

    hori->x = kp->sec_nrm_hori.x * vx + kp->sec_nrm_hori.y * vy;
    hori->y = kp->sec_nrm_hori.y * vx - kp->sec_nrm_hori.x * vy;

    return kp->dist_vv1 * f1;
}

s8 select_2obj_first(KPADInsideStatus* kp)
{
    KPADObject *op1, *op2, *rp1, *rp2;
    Vec2 hori;
    f32 f1, max = kp_err_first_inpr;

    op1 = kp->kobj_sample;
    do
    {
        if (op1->error_fg != 0)
        {
            continue;
        }

        op2 = op1 + 1;
        do
        {
            if (op2->error_fg != 0)
            {
                continue;
            }

            f1 = calc_horizon(kp, &op1->center, &op2->center, &hori);

            if (f1 <= kp->err_dist_min || f1 >= kp_err_dist_max)
            {
                continue;
            }

            f1 = kp->acc_horizon.x * hori.x + kp->acc_horizon.y * hori.y;
            if (f1 < 0.0f)
            {
                if (-f1 > max)
                {
                    max = -f1;
                    rp1 = op2;
                    rp2 = op1;
                }
            }
            else
            {
                if (f1 > max)
                {
                    max = f1;
                    rp1 = op1;
                    rp2 = op2;
                }
            }

        } while (++op2 <= &kp->kobj_sample[WPAD_MAX_DPD_OBJECTS - 1]);
    } while (++op1 < &kp->kobj_sample[WPAD_MAX_DPD_OBJECTS - 1]);

    if (max == kp_err_first_inpr)
    {
        return 0;
    }

    kp->kobj_regular[0] = *rp1;
    kp->kobj_regular[1] = *rp2;

    return 2;
}

s8 select_2obj_continue(KPADInsideStatus* kp)
{
    KPADObject *op1, *op2, *rp1, *rp2;
    Vec2 nrm;
    s32 rev_fg;
    f32 f1, f2, f3, vx, vy, min = 2.0f;
    op1 = kp->kobj_sample;
    do
    {
        if (op1->error_fg != 0)
        {
            continue;
        }

        op2 = op1 + 1;
        do
        {
            if (op2->error_fg != 0)
            {
                continue;
            }
            vx = op2->center.x - op1->center.x;
            vy = op2->center.y - op1->center.y;
            f3 = sqrt(vx * vx + vy * vy);
            f1 = 1.0f / f3;
            nrm.x = vx * f1;
            nrm.y = vy * f1;

            f1 *= kp->dist_vv1;
            if (f1 <= kp->err_dist_min || f1 >= kp_err_dist_max)
            {
                continue;
            }

            f1 -= kp->sec_dist;
            if (f1 < 0.0f)
            {
                f1 *= kp->err_dist_speedM_1;
            }
            else
            {
                f1 *= kp->err_dist_speed_1;
            }
            if (f1 >= 1.0f)
            {
                continue;
            }

            f2 = kp->sec_nrm.x * nrm.x + kp->sec_nrm.y * nrm.y;
            if (f2 < 0.0f)
            {
                f2 = -f2;
                rev_fg = 1;
            }
            else
            {
                rev_fg = 0;
            }
            if (f2 <= kp_err_next_inpr)
            {
                continue;
            }
            f2 = (1.0f - f2) / (1.0f - kp_err_next_inpr);
            f1 += f2;
            if (f1 < min)
            {
                min = f1;
                if (rev_fg)
                {
                    rp1 = op2;
                    rp2 = op1;
                }
                else
                {
                    rp1 = op1;
                    rp2 = op2;
                }
            }

        } while (++op2 <= &kp->kobj_sample[WPAD_MAX_DPD_OBJECTS - 1]);
    } while (++op1 < &kp->kobj_sample[WPAD_MAX_DPD_OBJECTS - 1]);

    if (min == 2.0f)
    {
        return 0;
    }

    kp->kobj_regular[0] = *rp1;
    kp->kobj_regular[1] = *rp2;

    return 2;
}

s8 select_1obj_first(KPADInsideStatus* kp)
{
    KPADObject* op1;
    f32 vx, vy;
    Vec2 p1, p2;

    vx = kp->sec_nrm_hori.x * kp->acc_horizon.x + kp->sec_nrm_hori.y * kp->acc_horizon.y;
    vy = kp->sec_nrm_hori.y * kp->acc_horizon.x - kp->sec_nrm_hori.x * kp->acc_horizon.y;

    vx *= kp->trust_sec_length;
    vy *= kp->trust_sec_length;

    op1 = kp->kobj_sample;
    do
    {
        if (op1->error_fg != 0)
        {
            continue;
        }

        p1.x = op1->center.x - vx;
        p1.y = op1->center.y - vy;
        p2.x = op1->center.x + vx;
        p2.y = op1->center.y + vy;

        if (p1.x <= kp->kobj_frame_min.x || p1.x >= kp->kobj_frame_max.x || p1.y <= kp->kobj_frame_min.y || p1.y >= kp->kobj_frame_max.y)
        {
            if (p2.x > kp->kobj_frame_min.x && p2.x < kp->kobj_frame_max.x && p2.y > kp->kobj_frame_min.y && p2.y < kp->kobj_frame_max.y)
            {
                kp->kobj_regular[1] = *op1;
                kp->kobj_regular[0].center = p1;
                kp->kobj_regular[0].error_fg = 0;
                kp->kobj_regular[0].state_fg = -1;

                return -1;
            }
        }
        else
        {
            if (p2.x <= kp->kobj_frame_min.x || p2.x >= kp->kobj_frame_max.x || p2.y <= kp->kobj_frame_min.y || p2.y >= kp->kobj_frame_max.y)
            {
                kp->kobj_regular[0] = *op1;

                kp->kobj_regular[1].center = p2;
                kp->kobj_regular[1].error_fg = 0;
                kp->kobj_regular[1].state_fg = -1;

                return -1;
            }
        }

    } while (++op1 < &kp->kobj_sample[WPAD_MAX_DPD_OBJECTS]);

    return 0;
}

s8 select_1obj_continue(KPADInsideStatus* kp)
{
    KPADObject *op1, *op2, *rp1, *rp2;
    f32 f1, vx, vy;
    f32 min = kp_err_near_pos * kp_err_near_pos;

    op1 = kp->kobj_regular;
    do
    {
        if (op1->error_fg != 0)
        {
            continue;
        }
        if (op1->state_fg != 0)
        {
            continue;
        }

        op2 = kp->kobj_sample;
        do
        {
            if (op2->error_fg != 0)
            {
                continue;
            }

            vx = op1->center.x - op2->center.x;
            vy = op1->center.y - op2->center.y;
            f1 = vx * vx + vy * vy;
            if (f1 < min)
            {
                min = f1;
                rp1 = op1;
                rp2 = op2;
            }
        } while (++op2 < &kp->kobj_sample[WPAD_MAX_DPD_OBJECTS]);
    } while (++op1 < &kp->kobj_regular[KPAD_USE_OBJECTS]);

    if (min == kp_err_near_pos * kp_err_near_pos)
        return 0;

    *rp1 = *rp2;
    kp->sec_nrm.x = kp->sec_nrm_hori.x * kp->acc_horizon.x + kp->sec_nrm_hori.y * kp->acc_horizon.y;
    kp->sec_nrm.y = kp->sec_nrm_hori.y * kp->acc_horizon.x - kp->sec_nrm_hori.x * kp->acc_horizon.y;

    vx = kp->sec_length * kp->sec_nrm.x;
    vy = kp->sec_length * kp->sec_nrm.y;
    if (rp1 == &kp->kobj_regular[0])
    {
        kp->kobj_regular[1].center.x = rp1->center.x + vx;
        kp->kobj_regular[1].center.y = rp1->center.y + vy;
        kp->kobj_regular[1].error_fg = 0;
        kp->kobj_regular[1].state_fg = -1;
    }
    else
    {
        kp->kobj_regular[0].center.x = rp1->center.x - vx;
        kp->kobj_regular[0].center.y = rp1->center.y - vy;
        kp->kobj_regular[0].error_fg = 0;
        kp->kobj_regular[0].state_fg = -1;
    }

    if (kp->status.dpd_valid_fg < 0)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

void calc_dpd_variable(KPADInsideStatus* kp, s8 valid_fg_next)
{
    KPADStatus* sp = &kp->status;
    f32 f1, f2, dist;
    Vec2 pos, vec;

    if (valid_fg_next == 0)
    {
        sp->dpd_valid_fg = 0;
        return;
    }

    pos.x = kp->sec_nrm_hori.x * kp->sec_nrm.x + kp->sec_nrm_hori.y * kp->sec_nrm.y;
    pos.y = kp->sec_nrm_hori.y * kp->sec_nrm.x - kp->sec_nrm_hori.x * kp->sec_nrm.y;

    if (sp->dpd_valid_fg == 0)
    {
        sp->horizon = pos;
        sp->hori_vec = Vec2_0;
        sp->hori_speed = 0.0f;
    }
    else
    {
        vec.x = pos.x - sp->horizon.x;
        vec.y = pos.y - sp->horizon.y;
        f1 = sqrt(vec.x * vec.x + vec.y * vec.y);

        if (kp->hori_play_mode == KPAD_PLAY_MODE_LOOSE)
        {
            f32 gain;
            if (f1 >= kp->hori_play_radius)
            {
                gain = 1.0f;
            }
            else
            {
                gain = f1 / kp->hori_play_radius;
                gain *= gain;
                gain *= gain;
            }
            gain *= kp->hori_sensitivity;
            vec.x = gain * vec.x + sp->horizon.x;
            vec.y = gain * vec.y + sp->horizon.y;
            f1 = sqrt(vec.x * vec.x + vec.y * vec.y);
            vec.x /= f1;
            vec.y /= f1;

            sp->hori_vec.x = vec.x - sp->horizon.x;
            sp->hori_vec.y = vec.y - sp->horizon.y;
            sp->hori_speed = sqrt(sp->hori_vec.x * sp->hori_vec.x + sp->hori_vec.y * sp->hori_vec.y);

            sp->horizon = vec;
        }
        else
        {
            if (f1 > kp->hori_play_radius)
            {
                f1 = (f1 - kp->hori_play_radius) / f1 * kp->hori_sensitivity;
                vec.x = vec.x * f1 + sp->horizon.x;
                vec.y = vec.y * f1 + sp->horizon.y;
                f1 = sqrt(vec.x * vec.x + vec.y * vec.y);
                vec.x /= f1;
                vec.y /= f1;

                sp->hori_vec.x = vec.x - sp->horizon.x;
                sp->hori_vec.y = vec.y - sp->horizon.y;
                sp->hori_speed = sqrt(sp->hori_vec.x * sp->hori_vec.x + sp->hori_vec.y * sp->hori_vec.y);

                sp->horizon = vec;
            }
            else
            {
                sp->hori_vec = Vec2_0;
                sp->hori_speed = 0.0f;
            }
        }
    }

    dist = kp->dist_vv1 / kp->sec_length;

    if (sp->dpd_valid_fg == 0)
    {
        sp->dist = dist;
        sp->dist_vec = 0.0f;
        sp->dist_speed = 0.0f;
    }
    else
    {
        f2 = dist - sp->dist;
        if (f2 < 0.0f)
        {
            f1 = -f2;
        }
        else
        {
            f1 = f2;
        }

        if (kp->dist_play_mode == KPAD_PLAY_MODE_LOOSE)
        {
            if (f1 >= kp->dist_play_radius)
            {
                f1 = 1.0f;
            }
            else
            {
                f1 /= kp->dist_play_radius;
                f1 *= f1;
                f1 *= f1;
            }
            f1 *= kp->dist_sensitivity;

            sp->dist_vec = f1 * f2;
            if (sp->dist_vec < 0.0f)
            {
                sp->dist_speed = -sp->dist_vec;
            }
            else
            {
                sp->dist_speed = sp->dist_vec;
            }

            sp->dist += sp->dist_vec;
        }
        else
        {
            if (f1 > kp->dist_play_radius)
            {
                f1 = (f1 - kp->dist_play_radius) / f1 * kp->dist_sensitivity;
                sp->dist_vec = f1 * f2;
                if (sp->dist_vec < 0.0f)
                {
                    sp->dist_speed = -sp->dist_vec;
                }
                else
                {
                    sp->dist_speed = sp->dist_vec;
                }

                sp->dist += sp->dist_vec;
            }
            else
            {
                sp->dist_vec = 0.0f;
                sp->dist_speed = 0.0f;
            }
        }
    }

    pos.x = (kp->kobj_regular[0].center.x + kp->kobj_regular[1].center.x) * 0.5f;
    pos.y = (kp->kobj_regular[0].center.y + kp->kobj_regular[1].center.y) * 0.5f;

    f1 = kp->sec_nrm.x * kp->sec_nrm_hori.x + kp->sec_nrm.y * kp->sec_nrm_hori.y;
    f2 = -kp->sec_nrm.y * kp->sec_nrm_hori.x + kp->sec_nrm.x * kp->sec_nrm_hori.y;
    vec.x = f1 * pos.x - f2 * pos.y;
    vec.y = f2 * pos.x + f1 * pos.y;

    vec.x = (kp->center_org.x - vec.x) * kp->dpd2pos_scale;
    vec.y = (kp->center_org.y - vec.y) * kp->dpd2pos_scale;

    pos.x = -kp->accXY_nrm_hori.y * vec.x + kp->accXY_nrm_hori.x * vec.y;
    pos.y = -kp->accXY_nrm_hori.x * vec.x - kp->accXY_nrm_hori.y * vec.y;
    kp->unk_tls_0x5D8 = pos; // TLS (2010)
    if (sp->dpd_valid_fg == 0)
    {
        sp->pos = pos;
        sp->vec = Vec2_0;
        sp->speed = 0.0f;
    }
    else
    {
        vec.x = pos.x - sp->pos.x;
        vec.y = pos.y - sp->pos.y;
        f1 = sqrt(vec.x * vec.x + vec.y * vec.y);

        if (kp->pos_play_mode == KPAD_PLAY_MODE_LOOSE)
        {
            f32 gain;
            if (f1 >= kp->pos_play_radius)
            {
                gain = 1.0f;
            }
            else
            {
                gain = f1 / kp->pos_play_radius;
                gain *= gain;
                gain *= gain;
            }
            gain *= kp->pos_sensitivity;

            sp->vec.x = gain * vec.x;
            sp->vec.y = gain * vec.y;
            sp->speed = sqrt(sp->vec.x * sp->vec.x + sp->vec.y * sp->vec.y);

            sp->pos.x += sp->vec.x;
            sp->pos.y += sp->vec.y;
        }
        else
        {
            if (f1 > kp->pos_play_radius)
            {
                f1 = (f1 - kp->pos_play_radius) / f1 * kp->pos_sensitivity;
                sp->vec.x = f1 * vec.x;
                sp->vec.y = f1 * vec.y;
                sp->speed = sqrt(sp->vec.x * sp->vec.x + sp->vec.y * sp->vec.y);

                sp->pos.x += sp->vec.x;
                sp->pos.y += sp->vec.y;
            }
            else
            {
                sp->vec = Vec2_0;
                sp->speed = 0.0f;
            }
        }
    }

    sp->dpd_valid_fg = valid_fg_next;
}

static void calc_obj_horizon(KPADInsideStatus* kp)
{
    f32 f1, f2, vx, vy;

    vx = kp->kobj_regular[1].center.x - kp->kobj_regular[0].center.x;
    vy = kp->kobj_regular[1].center.y - kp->kobj_regular[0].center.y;
    f1 = sqrt(vx * vx + vy * vy);
    kp->sec_length = f1;

    f2 = 1.0f / f1;
    kp->sec_dist = kp->dist_vv1 * f2;

    vx *= f2;
    vy *= f2;
    kp->sec_nrm.x = vx;
    kp->sec_nrm.y = vy;

    kp->obj_horizon.x = kp->sec_nrm_hori.x * vx + kp->sec_nrm_hori.y * vy;
    kp->obj_horizon.y = kp->sec_nrm_hori.y * vx - kp->sec_nrm_hori.x * vy;
}

static void get_kobj(KPADInsideStatus* kp, DPDObject* wobj_p)
{
    const f32 dpd_scale = 2.0f / (f32)KPAD_DPD_RESO_WX;
    const f32 dpd_cx = (f32)(KPAD_DPD_RESO_WX - 1) / (f32)KPAD_DPD_RESO_WX;
    const f32 dpd_cy = (f32)(KPAD_DPD_RESO_WY - 1) / (f32)KPAD_DPD_RESO_WX;

    KPADObject* kobj_p;

    kobj_p = &kp->kobj_sample[WPAD_MAX_DPD_OBJECTS - 1];
    do
    {
        if (wobj_p->size)
        {
            kobj_p->center.x = (f32)(s32)wobj_p->x * dpd_scale - dpd_cx;
            kobj_p->center.y = (f32)(s32)wobj_p->y * dpd_scale - dpd_cy;

            kobj_p->error_fg = 0;
            kobj_p->state_fg = 0;
        }
        else
        {
            kobj_p->error_fg = -1;
        }

        --wobj_p;
    } while (--kobj_p >= kp->kobj_sample);
}

static void check_kobj_outside_frame(KPADInsideStatus* kp, KPADObject* kobj_t)
{
    KPADObject* kobj_p = &kobj_t[WPAD_MAX_DPD_OBJECTS - 1];

    do
    {
        if (kobj_p->error_fg < 0)
        {
            continue;
        }

        if (kobj_p->center.x <= kp->kobj_frame_min.x || kobj_p->center.x >= kp->kobj_frame_max.x || kobj_p->center.y <= kp->kobj_frame_min.y || kobj_p->center.y >= kp->kobj_frame_max.y)
        {
            kobj_p->error_fg |= 1;
        }
    } while (--kobj_p >= kobj_t);
}

static void check_kobj_same_position(KPADObject* kobj_t)
{
    KPADObject *op1, *op2;

    op1 = kobj_t;
    do
    {
        if (op1->error_fg != 0)
        {
            continue;
        }

        op2 = op1 + 1;
        do
        {
            if (op2->error_fg != 0)
            {
                continue;
            }

            if (op1->center.x == op2->center.x && op1->center.y == op2->center.y)
            {
                op2->error_fg |= 2;
            }
        } while (++op2 <= &kobj_t[WPAD_MAX_DPD_OBJECTS - 1]);
    } while (++op1 < &kobj_t[WPAD_MAX_DPD_OBJECTS - 1]);
}

void read_kpad_dpd(KPADInsideStatus* kp, KPADUnifiedWpadStatus* uwp)
{
    KPADStatus* sp = &kp->status;
    KPADObject* op1;
    s8 valid_fg_next;

    // TLS (2010): formats 15, 17 and 16 (later SDK devices) also carry DPD data when it is enabled
    if (uwp->fmt == WPAD_FMT_CORE_BTN_ACC_DPD || uwp->fmt == WPAD_FMT_FS_BTN_ACC_DPD || uwp->fmt == WPAD_FMT_CLASSIC_BTN_ACC_DPD || (uwp->fmt == WPAD_FMT_GUITAR && kp->dpdStatus)
        || (uwp->fmt == 15 && kp->dpdStatus) || (uwp->fmt == 17 && kp->dpdStatus) || (uwp->fmt == 16 && kp->dpdStatus))
    {
        get_kobj(kp, &uwp->u.core.obj[WPAD_MAX_DPD_OBJECTS - 1]);
    }
    else
    {
        op1 = &kp->kobj_sample[WPAD_MAX_DPD_OBJECTS - 1];
        do
        {
            op1->error_fg = -1;
        } while (--op1 >= kp->kobj_sample);
    }

    check_kobj_outside_frame(kp, kp->kobj_sample);
    check_kobj_same_position(kp->kobj_sample);
    kp->valid_objs = 0;
    op1 = &kp->kobj_sample[WPAD_MAX_DPD_OBJECTS - 1];
    do
    {
        if (op1->error_fg == 0)
        {
            ++kp->valid_objs;
        }
    } while (--op1 >= kp->kobj_sample);

    if (sp->acc_vertical.x <= kp_err_up_inpr)
    {
        goto LABEL_select_NG;
    }

    if (sp->dpd_valid_fg == 2 || sp->dpd_valid_fg == -2)
    {
        if (kp->valid_objs >= 2)
        {
            valid_fg_next = select_2obj_continue(kp);
            if (valid_fg_next)
            {
                goto LABEL_select_OK;
            }
        }
        if (kp->valid_objs >= 1)
        {
            valid_fg_next = select_1obj_continue(kp);
            if (valid_fg_next)
            {
                goto LABEL_select_OK;
            }
        }
    }
    else if (sp->dpd_valid_fg == 1 || sp->dpd_valid_fg == -1)
    {
        if (kp->valid_objs >= 2)
        {
            valid_fg_next = select_2obj_first(kp);
            if (valid_fg_next)
            {
                goto LABEL_select_OK;
            }
        }
        if (kp->valid_objs >= 1)
        {
            valid_fg_next = select_1obj_continue(kp);
            if (valid_fg_next)
            {
                goto LABEL_select_OK;
            }
        }
    }
    else
    {
        if (kp->valid_objs >= 2)
        {
            valid_fg_next = select_2obj_first(kp);

            if (valid_fg_next)
            {
                goto LABEL_select_OK;
            }
        }
        if (kp->valid_objs == 1)
        {
            valid_fg_next = select_1obj_first(kp);
            if (valid_fg_next)
                goto LABEL_select_OK;
        }
    }

LABEL_select_NG:
    valid_fg_next = 0;

LABEL_select_OK:
    if (valid_fg_next)
    {
        calc_obj_horizon(kp);
        if (kp->ah_circle_ct == 0)
        {
            if (kp->obj_horizon.x * kp->acc_horizon.x + kp->obj_horizon.y * kp->acc_horizon.y <= kp_err_acc_inpr)
            {
                valid_fg_next = 0;

                kp->kobj_regular[0].error_fg = kp->kobj_regular[1].error_fg = 1;
            }
        }

        if (sp->dpd_valid_fg == 2 && valid_fg_next == 2)
        {
            if (kp->dpd_valid2_ct == 200)
            {
                kp->trust_sec_length = kp->sec_length;
            }
            else
            {
                ++kp->dpd_valid2_ct;
            }
        }
        else
        {
            kp->dpd_valid2_ct = 0;
        }
    }
    else
    {
        kp->dpd_valid2_ct = 0;
    }

    calc_dpd_variable(kp, valid_fg_next);
}

void clamp_stick_circle(Vec2* stick, s32 sx, s32 sy, s32 min, s32 max)
{
    f32 length;
    f32 fx = (f32)sx;
    f32 fy = (f32)sy;
    f32 fmin = (f32)min;
    f32 fmax = (f32)max;

    length = sqrt(fx * fx + fy * fy);

    if (length <= fmin)
    {
        stick->x = stick->y = 0.0f;
    }
    else if (length >= fmax)
    {
        stick->x = fx / length;
        stick->y = fy / length;
    }
    else
    {
        length = (length - fmin) / (fmax - fmin) / length;
        stick->x = fx * length;
        stick->y = fy * length;
    }
}

void clamp_stick_cross(Vec2* stick, s32 sx, s32 sy, s32 min, s32 max)
{
    f32 length;

    if (sx < 0)
    {
        clamp_trigger(&stick->x, -sx, min, max);
        stick->x = -stick->x;
    }
    else
    {
        clamp_trigger(&stick->x, sx, min, max);
    }
    if (sy < 0)
    {
        clamp_trigger(&stick->y, -sy, min, max);
        stick->y = -stick->y;
    }
    else
    {
        clamp_trigger(&stick->y, sy, min, max);
    }

    length = stick->x * stick->x + stick->y * stick->y;
    if (length > 1.0f)
    {
        length = sqrt(length);
        stick->x /= length;
        stick->y /= length;
    }
}

void read_kpad_ext(KPADInsideStatus* kp, KPADUnifiedWpadStatus* uwp)
{
    KPADEXStatus* ep = &kp->status.ex_status;
    void (*clampStickFuncp)(Vec2* stick, s32 sx, s32 sy, s32 min, s32 max);

    clampStickFuncp = kp_stick_clamp_cross ? clamp_stick_cross : clamp_stick_circle;

    if (uwp->u.fs.dev == WPAD_DEV_FREESTYLE && (uwp->fmt == WPAD_FMT_FS_BTN || uwp->fmt == WPAD_FMT_FS_BTN_ACC || uwp->fmt == WPAD_FMT_FS_BTN_ACC_DPD))
    {
        if (kp->exResetReq)
        {
            kp->exResetReq = FALSE;

            ep->fs.stick = Vec2_0;
            ep->fs.acc.x = ep->fs.acc.z = 0.0f;
            ep->fs.acc.y = -1.0f;
            ep->fs.acc_value = 1.0f;
            ep->fs.acc_speed = 0.0f;
        }

        clampStickFuncp(&ep->fs.stick, uwp->u.fs.fsStickX, uwp->u.fs.fsStickY, kp_fs_fstick_min, kp_fs_fstick_max);
    }
    else if (uwp->u.cl.dev == WPAD_DEV_CLASSIC && (uwp->fmt == WPAD_FMT_CLASSIC_BTN || uwp->fmt == WPAD_FMT_CLASSIC_BTN_ACC || uwp->fmt == WPAD_FMT_CLASSIC_BTN_ACC_DPD))
    {
        if (kp->exResetReq)
        {
            kp->exResetReq = FALSE;

            ep->cl.lstick = Vec2_0;
            ep->cl.rstick = Vec2_0;
            ep->cl.ltrigger = ep->cl.rtrigger = 0.0f;
            ep->cl.hold = ep->cl.trig = ep->cl.release = 0x00000000;
            kp->btn_cl_repeat_time = 0;
            kp->btn_cl_repeat_next = kp->btn_repeat_delay;
        }

        clampStickFuncp(&ep->cl.lstick, uwp->u.cl.clLStickX, uwp->u.cl.clLStickY, kp_cl_stick_min, kp_cl_stick_max);
        clampStickFuncp(&ep->cl.rstick, uwp->u.cl.clRStickX, uwp->u.cl.clRStickY, kp_cl_stick_min, kp_cl_stick_max);
        clamp_trigger(&ep->cl.ltrigger, uwp->u.cl.clTriggerL, kp_cl_trigger_min, kp_cl_trigger_max);
        clamp_trigger(&ep->cl.rtrigger, uwp->u.cl.clTriggerR, kp_cl_trigger_min, kp_cl_trigger_max);
    }
    else if ((uwp->u.cl.dev == WPAD_DEV_GUITAR && uwp->fmt == WPAD_FMT_GUITAR) || (uwp->u.cl.dev == 18 && uwp->fmt == 15)) // TLS (2010): also device 18
    {
        if (kp->exResetReq)
        {
            kp->exResetReq = FALSE;

            ep->cl.lstick = Vec2_0;
            ep->cl.rstick = Vec2_0;
            ep->cl.ltrigger = ep->cl.rtrigger = 0.0f;
            ep->cl.hold = ep->cl.trig = ep->cl.release = 0x00000000;
            kp->btn_cl_repeat_time = 0;
            kp->btn_cl_repeat_next = kp->btn_repeat_delay;
        }

        clampStickFuncp(&ep->cl.lstick, uwp->u.cl.clLStickX, uwp->u.cl.clLStickY, kp_cl_stick_min, kp_cl_stick_max);
        clamp_trigger(&ep->cl.rstick.x, uwp->u.cl.clRStickX, kp_guitar_stick_min, kp_guitar_stick_max);
        clamp_trigger(&ep->cl.rstick.y, uwp->u.cl.clRStickY, kp_guitar_stick_min, kp_guitar_stick_max);
        clamp_trigger(&ep->cl.ltrigger, uwp->u.cl.clTriggerL, kp_tr_trigger_min, kp_tr_trigger_max);
        clamp_trigger(&ep->cl.rtrigger, uwp->u.cl.clTriggerR, kp_tr_trigger_min, kp_tr_trigger_max);
    }
    else if (uwp->u.tr.dev == WPAD_DEV_TRAIN && uwp->fmt == WPAD_FMT_TRAIN)
    {
        if (kp->exResetReq)
        {
            kp->exResetReq = FALSE;

            ep->cl.lstick = Vec2_0;
            ep->cl.rstick = Vec2_0;
            ep->cl.ltrigger = ep->cl.rtrigger = 0.0f;
            ep->cl.hold = ep->cl.trig = ep->cl.release = 0x00000000;
            kp->btn_cl_repeat_time = 0;
            kp->btn_cl_repeat_next = kp->btn_repeat_delay;
        }

        ep->cl.lstick.x = ep->cl.lstick.y = 0.0f;
        ep->cl.rstick.x = ep->cl.rstick.y = 0.0f;
        clamp_trigger(&ep->cl.ltrigger, uwp->u.tr.brake, kp_tr_trigger_min, kp_tr_trigger_max);
        clamp_trigger(&ep->cl.rtrigger, uwp->u.tr.mascon, kp_tr_trigger_min, kp_tr_trigger_max);
    }
    else if (uwp->u.cl.dev == 19 && uwp->fmt == 17) // TLS (2010): device 19 (sticks read as four axes)
    {
        if (kp->exResetReq)
        {
            kp->exResetReq = FALSE;

            ep->cl.lstick = Vec2_0;
            ep->cl.rstick = Vec2_0;
            ep->cl.ltrigger = ep->cl.rtrigger = 0.0f;
            ep->cl.hold = ep->cl.trig = ep->cl.release = 0x00000000;
            kp->btn_cl_repeat_time = 0;
            kp->btn_cl_repeat_next = kp->btn_repeat_delay;
        }

        clamp_trigger(&ep->cl.lstick.x, uwp->u.cl.clLStickX, kp_guitar_stick_min, kp_guitar_stick_max);
        clamp_trigger(&ep->cl.lstick.y, uwp->u.cl.clLStickY, kp_guitar_stick_min, kp_guitar_stick_max);
        clamp_trigger(&ep->cl.rstick.x, uwp->u.cl.clRStickX, kp_guitar_stick_min, kp_guitar_stick_max);
        clamp_trigger(&ep->cl.rstick.y, uwp->u.cl.clRStickY, kp_guitar_stick_min, kp_guitar_stick_max);
        clamp_trigger(&ep->cl.ltrigger, uwp->u.cl.clTriggerL, kp_tr_trigger_min, kp_tr_trigger_max);
        clamp_trigger(&ep->cl.rtrigger, uwp->u.cl.clTriggerR, kp_tr_trigger_min, kp_tr_trigger_max);
    }
    else if (uwp->u.bl.dev == WPAD_DEV_BALANCE_CHECKER && uwp->fmt == WPAD_FMT_BALANCE_CHECKER)
    {
        // TLS (2010): new weight tracking
        f64 total;
        f64 diff;
        int i;
        ep->bl.weight_err = WBCRead(&uwp->u.bl, ep->bl.weight, 4);
        if (WBCGetBatteryLevel(uwp->u.bl.battery) == 0)
        {
            ep->bl.weight_err = -1;
        }
        else if (kp_wbc_enabled < 0)
        {
            ep->bl.weight_err = -6;
        }
        else if (kp_wbc_state == 0)
        {
            ep->bl.weight_err = -7;
        }
        else if (kp_wbc_state <= 3)
        {
            if (ep->bl.weight_err == 1)
            {
                kp_wbc_state = -1;
                kp_wbc_unk_d5 = 0;
            }
            if (kp_wbc_state < 0)
            {
                ep->bl.weight_err = -4;
            }
            else
            {
                ep->bl.weight_err = -8;
            }
        }
        else if (ep->bl.weight_err == -2)
        {
            ep->bl.weight_err = -2;
        }
        else if (ep->bl.weight_err >= 0)
        {
            total = ep->bl.weight[0] + ep->bl.weight[1] + ep->bl.weight[2] + ep->bl.weight[3];
            for (i = 0; i < 4; ++i)
            {
                kp_wbc_filtered[i] = (kp_wbc_ave_count * kp_wbc_filtered[i] + ep->bl.weight[i]) / (kp_wbc_ave_count + 1.0f);
                ep->bl.weight_ave[i] = kp_wbc_filtered[i];
            }
            if (total > 150.0)
            {
                ep->bl.weight_err = -5;
            }
            else if (kp_wbc_unk_d5)
            {
                kp_wbc_unk_b0 = 0.2f * kp_wbc_unk_tbl[(u8)(s32)(total / 34.0)];
                kp_wbc_unk_d6++;
                if (kp_wbc_unk_d6 > 2000)
                {
                    kp_wbc_err = kp_wbc_unk_d5 == 1 ? -3 : -1;
                    kp_wbc_unk_d5 = 0;
                }
                else if (total < 2.0)
                {
                    kp_wbc_unk_d5 = 1;
                }
                if (kp_wbc_unk_d5 == 1)
                {
                    kp_wbc_err = 1;
                    if (total >= 2.0)
                    {
                        kp_wbc_unk_d5 = 2;
                        kp_wbc_count = 0;
                        kp_wbc_unk_c0 = kp_wbc_unk_b8 = total;
                    }
                }
                else if (kp_wbc_unk_d5 == 2)
                {
                    kp_wbc_err = 1;
                    kp_wbc_unk_b8 = total - kp_wbc_unk_c0;
                    if (kp_wbc_unk_b8 < 0.0)
                    {
                        kp_wbc_unk_b8 *= -1.0;
                    }
                    if (kp_wbc_unk_b8 < kp_wbc_unk_b0)
                    {
                        kp_wbc_count++;
                    }
                    else
                    {
                        kp_wbc_count = 0;
                        kp_wbc_unk_c0 = kp_wbc_unk_c0 + 0.5 * (total - kp_wbc_unk_c0);
                    }
                    if (kp_wbc_count > 100)
                    {
                        kp_wbc_unk_d5 = 3;
                        kp_wbc_count = 1;
                        for (i = 0; i < 4; ++i)
                        {
                            kp_wbc_average[i] = ep->bl.weight[i];
                        }
                        kp_wbc_unk_c0 = kp_wbc_unk_b8 = total;
                    }
                }
                else if (kp_wbc_unk_d5 == 3)
                {
                    kp_wbc_err = -4;
                    diff = total - (kp_wbc_average[0] + kp_wbc_average[1] + kp_wbc_average[2] + kp_wbc_average[3]);
                    if (diff < 0.0)
                    {
                        diff *= -1.0;
                    }
                    if (diff <= kp_wbc_unk_b0)
                    {
                        ++kp_wbc_count;
                        for (i = 0; i < 4; ++i)
                        {
                            kp_wbc_average[i] = ((kp_wbc_count - 1) * kp_wbc_average[i] + ep->bl.weight[i]) / kp_wbc_count;
                        }
                        total = ep->bl.weight[0] + ep->bl.weight[1] + ep->bl.weight[2] + ep->bl.weight[3];
                        if (kp_wbc_unk_c0 > total)
                        {
                            kp_wbc_unk_c0 = total;
                        }
                        if (kp_wbc_unk_b8 < total)
                        {
                            kp_wbc_unk_b8 = total;
                        }
                        if (kp_wbc_count == kp_wbc_ave_count)
                        {
                            kp_wbc_err = 0;
                            WBCGetTGCWeight(kp_wbc_average[0] + kp_wbc_average[1] + kp_wbc_average[2] + kp_wbc_average[3], &kp_wbc_tgc_weight, &uwp->u.bl);
                            kp_wbc_unk_d5 = 0;
                        }
                    }
                }
            }
        }
        ep->bl.tgc_weight = kp_wbc_tgc_weight;
        ep->bl.tgc_weight_err = kp_wbc_err;
    }
    else if ((uwp->u.cl.dev == 5 || uwp->u.cl.dev == 6 || uwp->u.cl.dev == 7 || uwp->u.cl.dev == 250) && uwp->fmt == 16)
    {
        // TLS (2010): Motion Plus with an extension in pass-through mode
        if (kp->exResetReq)
        {
            kp->exResetReq = FALSE;
            if (uwp->u.cl.dev == 6 && (uwp->u.unk_tls[0x36] & 0x40))
            {
                ep->fs.stick = Vec2_0;
                ep->fs.acc.x = ep->fs.acc.z = 0.0f;
                ep->fs.acc.y = -1.0f;
                ep->fs.acc_value = 1.0f;
                ep->fs.acc_speed = 0.0f;
            }
            if (uwp->u.cl.dev == 7 && (uwp->u.unk_tls[0x36] & 0x40))
            {
                ep->cl.lstick = Vec2_0;
                ep->cl.rstick = Vec2_0;
                ep->cl.ltrigger = ep->cl.rtrigger = 0.0f;
                ep->cl.hold = ep->cl.trig = ep->cl.release = 0x00000000;
                kp->btn_cl_repeat_time = 0;
                kp->btn_cl_repeat_next = kp->btn_repeat_delay;
            }
        }
        if ((uwp->u.unk_tls[0x36] & 0x01) && (uwp->u.unk_tls[0x36] & 0x40))
        {
            if (uwp->u.cl.dev == 6)
            {
                clampStickFuncp(&ep->fs.stick, uwp->u.fs.fsStickX, uwp->u.fs.fsStickY, kp_fs_fstick_min, kp_fs_fstick_max);
            }
            else if (uwp->u.cl.dev == 7)
            {
                clampStickFuncp(&ep->cl.lstick, uwp->u.cl.clLStickX, uwp->u.cl.clLStickY, kp_cl_stick_min, kp_cl_stick_max);
                clampStickFuncp(&ep->cl.rstick, uwp->u.cl.clRStickX, uwp->u.cl.clRStickY, kp_cl_stick_min, kp_cl_stick_max);
                clamp_trigger(&ep->cl.ltrigger, uwp->u.cl.clTriggerL, kp_cl_trigger_min, kp_cl_trigger_max);
                clamp_trigger(&ep->cl.rtrigger, uwp->u.cl.clTriggerR, kp_cl_trigger_min, kp_cl_trigger_max);
            }
        }
    }
}

// TLS (2010): Motion Plus result callback (registered from KPADiRead); fills the read buffers backwards
// NONMATCHING: 61% - the original copies the 0x3C bytes in 8-byte pairs (lwz 4/lwz 0), MWCC here copies word by word
static void fn_80657230(KPADChannel chan, const KPADMPStatus* mp)
{
    KPADInsideStatus* kp = &inside_kpads[chan];
    if (kp->mplsBuf)
    {
        kp->mplsBuf--;
        kp->mplsBuf->mpls = *mp;
    }
}

static void KPADiSamplingCallback(s32 chan);
static void KPADiConnectCallback(WPADChannel chan, WPADResult result);
static void KPADiControlDpdCallback(WPADChannel chan, WPADResult result);
static void fn_80658300(WPADChannel chan, WPADResult result);

s32 KPADRead(KPADChannel chan, KPADStatus samplingBufs[], u32 length)
{
    return KPADiRead(chan, samplingBufs, length, NULL, FALSE);
}

// TLS (2010): WPAD Motion Plus control (key/src/revolution/WPAD.h)
u8 WPADiGetMplsStatus(WPADChannel chan);
WPADResult WPADiControlMpls(WPADChannel chan, u8 command, WPADCallback callback);

// TLS (2010): Motion Plus part of KPAD (KPADMpls_unk.c)
BOOL fn_80658F30(void);
void fn_8065AA50(KPADChannel chan, KPADUnifiedWpadStatus* uniRingBuf, KPADUnifiedWpadStatus* exRingBuf, u32 exBufLen);
void fn_8065AEB0(void (*callback)(KPADChannel chan, const KPADMPStatus* mp));
void fn_8065AA80(KPADChannel chan, KPADMPStatus* mp, s32 idx, s32 count, KPADStatus* sp, f64 interval);

// TLS (2010): classic-style buttons of the later devices (6 and 7 when their extension is active)
#define KPAD_DEV_EX_ACTIVE(uwp) (((uwp)->u.unk_tls[0x36] & 0x40) && ((uwp)->u.unk_tls[0x36] & 0x01))

s32 KPADiRead(KPADChannel chan, KPADStatus samplingBufs[], u32 length, s32* result, BOOL preserve)
{
    KPADInsideStatus* kp = &inside_kpads[chan];
    KPADTmpStatus* tp;
    KPADTmpStatus* first;
    KPADUnifiedWpadStatus* uwp;
    KPADStatus saved;
    KPADStatus tmp;
    s32 error = 0;
    s32 idx;
    s32 return_ct = 0;
    s32 copy_ct;
    BOOL enabled;
    u32 core, fs, cl, dev;
    u32 tick;
    f64 sec;
    f64 interval;
    if (!kp_initialized)
    {
        error = -5;
        goto exit;
    }
    if (WPADGetStatus() != WPAD_LIB_STATUS_3)
    {
        error = -3;
        goto exit;
    }
    enabled = OSDisableInterrupts();
    if (kp->readLocked)
    {
        OSRestoreInterrupts(enabled);
        error = -4;
        goto exit;
    }
    kp->readLocked = TRUE;
    if (WPADProbe(chan, NULL) == WPAD_ERR_NO_CONTROLLER)
    {
        reset_kpad(chan);
        if (kp->dpd_ctrl_callback && kp->dpdPreCallbackDone && !kp->dpdPostCallbackDone)
        {
            KPADiControlDpdCallback(chan, WPAD_ERR_OK);
        }
        // TLS (2010)
        if (kp->mplsCallback && kp->unk_0x67b && !kp->unk_0x67c)
        {
            fn_80658300(chan, WPAD_ERR_OK);
        }
        kp->readLocked = FALSE;
        OSRestoreInterrupts(enabled);
        error = -2;
        goto exit;
    }
    OSRestoreInterrupts(enabled);
    if (kp->resetReq)
    {
        kp->status.wpad_err = WPAD_ERR_INVALID;
        reset_kpad(chan);
    }
    saved = *samplingBufs;
    if (kp->bufCount && samplingBufs && length)
    {
        enabled = OSDisableInterrupts();
        copy_ct = kp->bufCount;
        if (copy_ct > length)
        {
            copy_ct = length;
        }
        kp->bufCount = 0;
        // TLS (2010): sampling interval over the last three reads
        tick = kp->tickHist[0];
        kp->tickHist[0] = kp->tickHist[1];
        kp->tickHist[1] = kp->tickHist[2];
        kp->tickHist[2] = kp->prevTick;
        sec = (s64)((kp->prevTick + 0x100000000LL - tick) & 0xFFFFFFFFLL) / (f64)OS_TIME_SPEED;
        if (sec > 0.3)
        {
            sec = 0.3;
        }
        {
            u32 c0 = kp->cntHist[0];
            u32 c1 = kp->cntHist[1];
            tick = copy_ct + c0;
            kp->cntHist[0] = c1;
            kp->cntHist[1] = copy_ct;
            interval = sec / (c1 + tick);
        }
        return_ct = copy_ct;
        tp = (KPADTmpStatus*)samplingBufs + copy_ct;
        idx = kp->bufIdx - copy_ct;
        if (idx < 0)
        {
            idx += KPAD_MAX_SAMPLES + kp->exBufLen;
        }
        do
        {
            uwp = idx >= KPAD_MAX_SAMPLES ? &kp->exRingBuf[idx - KPAD_MAX_SAMPLES] : &kp->uniRingBuf[idx];
            --tp;
            tp->w = *uwp;
            ++idx;
            if (idx >= KPAD_MAX_SAMPLES + kp->exBufLen)
            {
                idx = 0;
            }
        } while (--copy_ct > 0);
        OSRestoreInterrupts(enabled);
        copy_ct = return_ct;
        tp = (KPADTmpStatus*)samplingBufs + copy_ct;
        first = tp;
        if (kp->readMode == 1)
        {
            do
            {
                --tp;
                uwp = &tp->w;
                if (kp->status.dev_type != uwp->u.core.dev)
                {
                    kp->status.dev_type = uwp->u.core.dev;
                    kp->exResetReq = TRUE;
                }
                kp->status.wpad_err = uwp->u.core.err;
                kp->status.data_format = uwp->fmt;
                core = fs = cl = KPAD_BUTTON_MASK;
                dev = kp->status.dev_type;
                switch (uwp->u.core.err)
                {
                case WPAD_ERR_OK:
                    if (dev == WPAD_DEV_FREESTYLE)
                    {
                        fs = uwp->u.fs.button;
                        cl = 0;
                    }
                    else if (dev == WPAD_DEV_CLASSIC || (dev >= WPAD_DEV_TRAIN && dev <= 19))
                    {
                        fs = 0;
                        cl = uwp->u.cl.clButton;
                    }
                    else if (dev == 6 && KPAD_DEV_EX_ACTIVE(uwp))
                    {
                        fs = uwp->u.fs.button;
                        cl = 0;
                    }
                    else if (dev == 7 && KPAD_DEV_EX_ACTIVE(uwp))
                    {
                        fs = 0;
                        cl = uwp->u.cl.clButton;
                    }
                    else
                    {
                        fs = cl = 0;
                    }
                case WPAD_ERR_CORRUPTED:
                case WPAD_ERR_BUSY:
                    core = uwp->u.core.button & 0x9F1F;
                    break;
                default:
                    break;
                }
                if (core == KPAD_BUTTON_MASK)
                {
                    core = kp->status.hold & 0x9F1F;
                }
                if (fs == KPAD_BUTTON_MASK)
                {
                    fs = kp->status.hold;
                }
                if (cl == KPAD_BUTTON_MASK)
                {
                    cl = kp->status.ex_status.cl.hold;
                }
                read_kpad_button(kp, dev, 1, core, fs, cl);
                switch (uwp->u.core.err)
                {
                case WPAD_ERR_OK:
                    read_kpad_ext(kp, uwp);
                case WPAD_ERR_CORRUPTED:
                    read_kpad_acc(kp, uwp);
                    read_kpad_dpd(kp, uwp);
                    break;
                default:
                    if (!preserve)
                    {
                        kp->status.dpd_valid_fg = FALSE;
                    }
                    break;
                }
                tp->k = kp->status;
            } while (--copy_ct > 0);
        }
        else
        {
            core = fs = cl = KPAD_BUTTON_MASK;
            do
            {
                --tp;
                uwp = &tp->w;
                if (kp->status.dev_type != uwp->u.core.dev)
                {
                    kp->status.dev_type = uwp->u.core.dev;
                    kp->exResetReq = TRUE;
                }
                dev = uwp->u.core.dev;
                switch (uwp->u.core.err)
                {
                case WPAD_ERR_OK:
                    if (dev == WPAD_DEV_FREESTYLE)
                    {
                        fs = uwp->u.fs.button;
                        cl = 0;
                    }
                    else if (dev == WPAD_DEV_CLASSIC || (dev >= WPAD_DEV_TRAIN && dev <= 19))
                    {
                        fs = 0;
                        cl = uwp->u.cl.clButton;
                    }
                    else if (dev == 6 && KPAD_DEV_EX_ACTIVE(uwp))
                    {
                        fs = uwp->u.fs.button;
                        cl = 0;
                    }
                    else if (dev == 7 && KPAD_DEV_EX_ACTIVE(uwp))
                    {
                        fs = 0;
                        cl = uwp->u.cl.clButton;
                    }
                    else
                    {
                        fs = cl = 0;
                    }
                case WPAD_ERR_CORRUPTED:
                case WPAD_ERR_BUSY:
                    core = uwp->u.core.button & 0x9F1F;
                    break;
                default:
                    break;
                }
            } while (--copy_ct > 0);
            if (core == KPAD_BUTTON_MASK)
            {
                core = kp->status.hold & 0x9F1F;
            }
            if (fs == KPAD_BUTTON_MASK)
            {
                fs = kp->status.hold;
            }
            if (cl == KPAD_BUTTON_MASK)
            {
                cl = kp->status.ex_status.cl.hold;
            }
            read_kpad_button(kp, dev, return_ct, core, fs, cl);
            copy_ct = return_ct;
            tp = first;
            do
            {
                --tp;
                uwp = &tp->w;
                kp->status.wpad_err = uwp->u.core.err;
                kp->status.data_format = uwp->fmt;
                switch (uwp->u.core.err)
                {
                case WPAD_ERR_OK:
                    read_kpad_ext(kp, uwp);
                case WPAD_ERR_CORRUPTED:
                    read_kpad_acc(kp, uwp);
                    read_kpad_dpd(kp, uwp);
                    break;
                default:
                    if (!preserve)
                    {
                        kp->status.dpd_valid_fg = FALSE;
                    }
                    break;
                }
                tp->k = kp->status;
            } while (--copy_ct > 0);
        }
        // TLS (2010): Motion Plus
        if (fn_80658F30())
        {
            kp->mplsBuf = &first->k;
            tmp.acc = kp->hard_acc;
            tmp.pos = kp->unk_tls_0x5D8;
            tmp.horizon = kp->obj_horizon;
            tmp.dpd_valid_fg = kp->status.dpd_valid_fg;
            fn_8065AA50(chan, kp->uniRingBuf, kp->exRingBuf, kp->exBufLen);
            fn_8065AEB0(fn_80657230);
            fn_8065AA80(chan, &kp->status.mpls, idx, return_ct, &tmp, interval);
            *samplingBufs = kp->status;
        }
    }
    kp->readLocked = FALSE;
exit:
    if (!return_ct)
    {
        if (!error)
        {
            if (preserve)
            {
                *samplingBufs = saved;
            }
            error = -1;
        }
        else if (error == -2 && preserve)
        {
            samplingBufs->dev_type = WPAD_DEV_NOT_FOUND;
            samplingBufs->data_format = WPAD_FMT_CORE_BTN;
            samplingBufs->wpad_err = WPAD_ERR_NO_CONTROLLER;
        }
    }
    if (result)
    {
        *result = error;
    }
    return return_ct;
}

void KPADSetObjInterval(f32 interval)
{
    BOOL enabled = OSDisableInterrupts();
    kp_obj_interval = interval;
    kp_err_dist_min = interval / 0.383864f;
    kp_dist_vv1 = interval / 0.383864f;
    OSRestoreInterrupts(enabled);
}

void KPADInit(void)
{
    KPADInitEx(NULL, 0);
}

void KPADInitEx(KPADUnifiedWpadStatus* buffer, u32 length)
{
    s32 i;
    KPADInsideStatus* kp;
    u32 idx;
    BOOL enabled;
    if (kp_initialized)
    {
        return;
    }
    WPADInit();
    enabled = OSDisableInterrupts(); // TLS (2010)
    memset(inside_kpads, 0, sizeof(inside_kpads));
    kp_err_dist_max = 1.0f + (f32)WPADGetDpdSensitivity();
    i = 0;
    do
    {
        kp = &inside_kpads[i];
        kp->appConnectCallback = WPADSetConnectCallback(i, KPADiConnectCallback);
        kp->appSamplingCallback = WPADSetSamplingCallback(i, KPADiSamplingCallback);
        // TLS (2010)
        kp->dpdStatus = FALSE;
        kp->dpdEnabled = TRUE;
        kp->dpdCmd = 0;
        kp->dpdPreCallbackDone = FALSE;
        kp->dpdPostCallbackDone = TRUE;
        kp->unk_0x679 = 0;
        kp->unk_0x676 = 0;
        kp->unk_0x67a = 0;
        kp->unk_0x67d = 3;
        kp->unk_0x677 = kp->unk_0x678 = WPADiGetMplsStatus(i);
        kp->unk_0x67b = 0;
        kp->unk_0x67c = 1;
        kp->status.dev_type = WPAD_DEV_NOT_FOUND;
        kp->status.data_format = WPAD_FMT_CORE_BTN;
        kp->dist_org = idist_org;
        kp->accXY_nrm_hori = iaccXY_nrm_hori;
        kp->sec_nrm_hori = isec_nrm_hori;
        kp->center_org = icenter_org;
        calc_dpd2pos_scale(kp);
        kp->pos_play_radius = kp->hori_play_radius = kp->dist_play_radius = kp->acc_play_radius = 0.0f;
        kp->pos_sensitivity = kp->hori_sensitivity = kp->dist_sensitivity = kp->acc_sensitivity = 1.0f;
        kp->pos_play_mode = kp->hori_play_mode = kp->dist_play_mode = kp->acc_play_mode = KPAD_PLAY_MODE_LOOSE;
        KPADSetBtnRepeat(i, 0.0f, 0.0f);
        kp->readMode = 0;
        kp->aimReq = TRUE;
        kp->aimEnabled = TRUE;
        kp->accScaleReset = TRUE;
        kp->fsAccRevise = FALSE;
        if (length && buffer)
        {
            kp->exBufLen = length / 4;
            kp->exRingBuf = buffer + i * kp->exBufLen;
        }
        else
        {
            kp->exBufLen = 0;
            kp->exRingBuf = NULL;
        }
        for (idx = 0; idx < KPAD_MAX_SAMPLES; ++idx)
        {
            kp->uniRingBuf[idx].u.core.err = WPAD_ERR_NO_CONTROLLER;
        }
        for (idx = 0; idx < kp->exBufLen; ++idx)
        {
            kp->exRingBuf[idx].u.core.err = WPAD_ERR_NO_CONTROLLER;
        }
    } while (++i < WPAD_MAX_CONTROLLERS);
    // TLS (2010): balance board state, once for all channels
    kp_wbc_enabled = kp_wbc_busy = FALSE;
    kp_wbc_count = 0;
    for (i = 0; i < 4; ++i)
    {
        kp_wbc_average[i] = 0.0;
    }
    kp_wbc_err = -2;
    kp_wbc_unk_d6 = 0;
    kp_wbc_unk_d5 = 1;
    kp_wbc_state = 0;
    WPADSetCallbackByKPAD(TRUE);
    KPADReset();
    kp_fs_rot[0][0] = 1.0f;
    kp_fs_rot[0][1] = 0.0f;
    kp_fs_rot[0][2] = 0.0f;
    kp_fs_rot[0][3] = 0.0f;
    kp_fs_rot[1][0] = 0.0f;
    kp_fs_rot[1][1] = (f32)cos(MTXDegToRad(kp_fs_revise_deg));
    kp_fs_rot[1][2] = (f32)-sin(MTXDegToRad(kp_fs_revise_deg));
    kp_fs_rot[1][3] = 0.0f;
    kp_fs_rot[2][0] = 0.0f;
    kp_fs_rot[2][1] = (f32)sin(MTXDegToRad(kp_fs_revise_deg));
    kp_fs_rot[2][2] = (f32)cos(MTXDegToRad(kp_fs_revise_deg));
    kp_fs_rot[2][3] = 0.0f;
    kp_initialized = TRUE;
    OSRestoreInterrupts(enabled);
    OSRegisterVersion(__KPADVersion);
}

void KPADReset(void)
{
    s32 chan;
    KPADSetObjInterval(kp_obj_interval);
    chan = WPAD_MAX_CONTROLLERS - 1;
    do
    {
        if (WPADGetStatus() == WPAD_LIB_STATUS_3)
        {
            WPADControlMotor(chan, 0);
        }
        inside_kpads[chan].resetReq = TRUE;
    } while (--chan >= 0);
}

static void KPADiConnectCallback(WPADChannel chan, WPADResult result)
{
    KPADInsideStatus* kp = &inside_kpads[chan];
    u32 idx;
    if (result == WPAD_ERR_OK)
    {
        kp->lastDeviceType = WPAD_DEV_CORE;
        kp->accScaleReset = TRUE;
        WPADSetCallbackByKPAD(FALSE);
        WPADSetSamplingCallback(chan, KPADiSamplingCallback);
        WPADSetCallbackByKPAD(TRUE);
        kp->dpdStatus = FALSE;
        kp->dpdCmd = 0;
        kp->dpdIssued = FALSE; // TLS (2010)
        if (chan == WPAD_CHAN3)
        {
            // TLS (2010)
            kp_wbc_enabled = kp_wbc_busy = FALSE;
            if ((s32)kp_wbc_state != 4)
            {
                kp_wbc_state = 0;
            }
            kp_wbc_unk_d5 = 0;
            kp_wbc_err = -2;
            kp->status.ex_status.bl.weight_err = -2;
        }
        // TLS (2010): Motion Plus state
        kp->unk_0x67a = 0;
        kp->unk_0x67d = 1;
        kp->unk_0x678 = kp->unk_0x679;
        kp->unk_0x677 = 0;
        kp->unk_0x67b = 0;
        kp->unk_0x67c = 1;
    }
    else
    {
        for (idx = 0; idx < KPAD_MAX_SAMPLES; ++idx)
        {
            kp->uniRingBuf[idx].u.core.err = WPAD_ERR_NO_CONTROLLER;
        }
        for (idx = 0; idx < kp->exBufLen; ++idx)
        {
            kp->exRingBuf[idx].u.core.err = WPAD_ERR_NO_CONTROLLER;
        }
        // TLS (2010)
        if (kp->dpd_ctrl_callback && !kp->dpdPostCallbackDone)
        {
            kp->dpdPostCallbackDone = TRUE;
            kp->dpd_ctrl_callback(chan, 1);
            kp->dpdPreCallbackDone = FALSE;
        }
        kp->dpdStatus = WPADIsDpdEnabled(chan);
        kp->dpdIssued = FALSE;
        if (!kp->unk_0x67c && kp->mplsCallback)
        {
            kp->mplsCallback(chan, 1);
        }
    }
    if (kp->appConnectCallback)
    {
        kp->appConnectCallback(chan, result);
    }
}

WPADConnectCallback* KPADSetConnectCallback(KPADChannel chan, WPADConnectCallback* callback)
{
    WPADConnectCallback* old;
    BOOL enabled = OSDisableInterrupts();
    old = inside_kpads[chan].appConnectCallback;
    inside_kpads[chan].appConnectCallback = callback;
    OSRestoreInterrupts(enabled);
    return old;
}

static void KPADiControlWbcCallback(WPADChannel chan, WPADResult result)
{
    kp_wbc_enabled = result == WPAD_ERR_OK;
    kp_wbc_unk_d2 = FALSE; // TLS (2010)
    kp_wbc_busy = FALSE;
}

// TLS (2010)
static void KPADiUpdateTempWbcCallback(WPADChannel chan, WPADResult result)
{
    if (result == WPAD_ERR_OK)
    {
        kp_wbc_state = 2;
        kp_wbc_status = 100;
        kp_wbc_count = 0;
    }
    kp_wbc_busy = FALSE;
}

void KPADDisableDPD(KPADChannel chan)
{
    inside_kpads[chan].dpdEnabled = FALSE;
}

void KPADEnableDPD(KPADChannel chan)
{
    inside_kpads[chan].dpdEnabled = TRUE;
}

static void KPADiControlDpdCallback(WPADChannel chan, WPADResult result)
{
    KPADInsideStatus* kp = &inside_kpads[chan];
    if (result == WPAD_ERR_OK)
    {
        if (kp->dpd_ctrl_callback && !kp->dpdPostCallbackDone)
        {
            kp->dpdPostCallbackDone = TRUE;
            kp->dpd_ctrl_callback(chan, 1);
            kp->dpdPreCallbackDone = FALSE;
        }
    }
    kp->dpdStatus = WPADIsDpdEnabled(chan);
    kp->dpdIssued = FALSE;
}

// TLS (2010): Motion Plus mode change result
static void fn_80658300(WPADChannel chan, WPADResult result)
{
    KPADInsideStatus* kp = &inside_kpads[chan];
    s32 err;
    kp->unk_0x67a = 0;
    kp->unk_0x67d = 1;
    if (result == WPAD_ERR_OK)
    {
        kp->unk_0x677 = kp->unk_0x676;
        return;
    }
    if ((result == -4 || result == -3) && !kp->unk_0x67c && kp->unk_0x676 && kp->unk_0x676 == kp->unk_0x678)
    {
        switch (kp->unk_0x676)
        {
        case 5:
            err = -2;
            break;
        case 7:
            err = -3;
            break;
        default:
            err = -1;
            break;
        }
        kp->unk_0x67c = 0;
        if (kp->mplsCallback)
        {
            kp->mplsCallback(chan, err);
        }
        kp->unk_0x67b = 1;
    }
    kp->unk_0x677 = 0;
}

static void KPADiSamplingCallback(s32 chan)
{
    KPADInsideStatus* kp = &inside_kpads[chan];
    KPADUnifiedWpadStatus* uwp;
    u32 idx;
    u32 type;
    u32 curDpd;
    u8 mpls;
    s32 err;
    WPADAccGravityUnit unit = { 1, 1, 1 };
    // TLS (2010): 11 device classes
    static struct
    {
        u8 dpd;
        u8 fmt;
    } table[] = {
        { 0, WPAD_FMT_CORE_BTN_ACC }, { 3, WPAD_FMT_CORE_BTN_ACC_DPD }, { 0, WPAD_FMT_FS_BTN_ACC }, { 1, WPAD_FMT_FS_BTN_ACC_DPD }, { 0, WPAD_FMT_CLASSIC_BTN_ACC }, { 1, WPAD_FMT_CLASSIC_BTN_ACC_DPD }, { 0, WPAD_FMT_GUITAR }, { 1, WPAD_FMT_GUITAR }, { 0, 15 }, { 1, 15 }, { 0, WPAD_FMT_BALANCE_CHECKER }, { 0, WPAD_FMT_BALANCE_CHECKER }, { 0, WPAD_FMT_TRAIN }, { 0, WPAD_FMT_TRAIN }, { 0, 17 }, { 1, 17 }, { 0, 16 }, { 1, 16 }, { 0, 19 }, { 0, 19 }, { 0, 20 }, { 0, 20 }
    };
    if (WPADProbe(chan, (WPADDeviceType*)&type) == WPAD_ERR_NO_CONTROLLER)
    {
        goto finish;
    }
    idx = kp->bufIdx;
    if (idx >= KPAD_MAX_SAMPLES + kp->exBufLen)
    {
        idx = 0;
    }
    if (idx >= KPAD_MAX_SAMPLES)
    {
        uwp = &kp->exRingBuf[idx - KPAD_MAX_SAMPLES];
    }
    else
    {
        uwp = &kp->uniRingBuf[idx];
    }
    WPADRead(chan, (WPADStatus*)&uwp->u);
    uwp->fmt = (u8)WPADGetDataFormat(chan);
    kp->bufIdx = (u8)(idx + 1);
    if (kp->bufCount < KPAD_MAX_SAMPLES + kp->exBufLen)
    {
        ++kp->bufCount;
    }
    // TLS (2010)
    kp->prevTick = kp->curTick;
    kp->curTick = OSGetTick();
    if (type != kp->lastDeviceType && type == WPAD_DEV_FREESTYLE)
    {
        kp->accScaleReset = TRUE;
    }
    if (kp->accScaleReset)
    {
        WPADGetAccGravityUnit(chan, WPAD_ACC_GRAVITY_UNIT_CORE, &unit);
        if (unit.x * unit.y * unit.z)
        {
            kp->acc_scale_x = 1.0f / unit.x;
            kp->acc_scale_y = 1.0f / unit.y;
            kp->acc_scale_z = 1.0f / unit.z;
            kp->accScaleReset = FALSE; // TLS (2010): only once the core unit is known
        }
        else
        {
            kp->acc_scale_x = 1.0f / 100;
            kp->acc_scale_y = 1.0f / 100;
            kp->acc_scale_z = 1.0f / 100;
        }
        WPADGetAccGravityUnit(chan, WPAD_ACC_GRAVITY_UNIT_FS, &unit);
        if (unit.x * unit.y * unit.z)
        {
            kp->fs_acc_scale_x = 1.0f / unit.x;
            kp->fs_acc_scale_y = 1.0f / unit.y;
            kp->fs_acc_scale_z = 1.0f / unit.z;
        }
        else
        {
            kp->fs_acc_scale_x = 1.0f / 200;
            kp->fs_acc_scale_y = 1.0f / 200;
            kp->fs_acc_scale_z = 1.0f / 200;
        }
    }
    if (kp->aimReq)
    {
        f32 level;
        if (kp->aimEnabled)
        {
            level = WPADGetSensorBarPosition() == WPAD_SENSOR_BAR_TOP ? 0.2f : -0.2f;
        }
        else
        {
            level = 0.0f;
        }
        KPADSetSensorHeight(chan, level);
        kp->aimReq = FALSE;
    }
    switch (type)
    {
    case WPAD_DEV_CORE:
    case 251:
    case 252:
    case WPAD_DEV_UNKNOWN:
        idx = 0;
        break;
    case WPAD_DEV_FREESTYLE:
        idx = 2;
        break;
    case WPAD_DEV_CLASSIC:
        idx = 4;
        break;
    case WPAD_DEV_GUITAR:
        idx = 6;
        break;
    case 18:
        idx = 8;
        break;
    case WPAD_DEV_BALANCE_CHECKER:
        idx = 10;
        break;
    case WPAD_DEV_TRAIN:
        idx = 12;
        break;
    case 19:
        idx = 14;
        break;
    case 5:
    case 6:
    case 7:
    case 250:
        idx = 16;
        break;
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
        idx = 18;
        break;
    case 29:
        idx = 20;
        break;
    default:
        goto finish;
    }
    if (kp->dpdEnabled)
    {
        ++idx;
    }
    curDpd = WPADIsDpdEnabled(chan) ? kp->dpdCmd : 0;
    if (curDpd != table[idx].dpd)
    {
        if (kp->dpd_ctrl_callback && !kp->dpdPreCallbackDone)
        {
            kp->dpdPreCallbackDone = TRUE;
            kp->dpd_ctrl_callback(chan, 0);
            kp->dpdPostCallbackDone = FALSE;
        }
        if (!kp->dpdIssued)
        {
            kp->dpdIssued = TRUE;
            if (WPADControlDpd(chan, table[idx].dpd, KPADiControlDpdCallback) == WPAD_ERR_OK)
            {
                kp->dpdCmd = table[idx].dpd;
            }
        }
    }
    else if (uwp->fmt != table[idx].fmt)
    {
        WPADSetDataFormat(chan, table[idx].fmt);
    }
    // TLS (2010): balance board calibration state machine
    if (type == WPAD_DEV_BALANCE_CHECKER && !kp_wbc_busy)
    {
        if (kp_wbc_enabled > 2)
        {
            if (kp_wbc_state == 1)
            {
                if (WPADControlBLC(chan, 0, KPADiUpdateTempWbcCallback) == WPAD_ERR_OK)
                {
                    kp_wbc_busy = TRUE;
                }
            }
            else if (kp_wbc_state == 2)
            {
                if (kp_wbc_status == 0)
                {
                    if (uwp->u.bl.temp == 127 || uwp->u.bl.temp == -128)
                    {
                        kp_wbc_state = 1;
                    }
                    else
                    {
                        kp_wbc_state = 3;
                        kp_wbc_count = 0;
                    }
                }
                else
                {
                    kp_wbc_status--;
                }
            }
            else if (kp_wbc_state == 3)
            {
                if (uwp->u.bl.err == WPAD_ERR_OK)
                {
                    int i;
                    ++kp_wbc_count;
                    for (i = 0; i < 4; ++i)
                    {
                        kp_wbc_average[i] = ((kp_wbc_count - 1) * kp_wbc_average[i] + uwp->u.bl.press[i]) / kp_wbc_count;
                    }
                    if (kp_wbc_count == kp_wbc_ave_count)
                    {
                        kp_wbc_state = 4;
                        kp_wbc_count = 0;
                        WBCRead(&uwp->u.bl, kp_wbc_filtered, 4);
                        WBCSetZEROPoint(kp_wbc_average, 4);
                    }
                }
            }
        }
        else if (kp_wbc_enabled == 0)
        {
            if (WPADControlBLC(chan, 0xAA, KPADiControlWbcCallback) == WPAD_ERR_OK)
            {
                kp_wbc_busy = TRUE;
            }
        }
        else if (kp_wbc_enabled == 1)
        {
            if (kp_wbc_unk_d2 < 3)
            {
                if (WBCSetupCalibration() == 1)
                {
                    kp_wbc_enabled = 2;
                    kp_wbc_status = 0;
                }
                else
                {
                    kp_wbc_unk_d2++;
                }
            }
            else
            {
                kp_wbc_enabled = -1;
            }
        }
        else if (kp_wbc_enabled == 2)
        {
            if (WBCGetCalibrationStatus())
            {
                kp_wbc_enabled = 3;
                kp_wbc_unk_d2 = 0;
            }
            else if (++kp_wbc_status > kp_wbc_wait_count)
            {
                kp_wbc_enabled = 1;
                kp_wbc_unk_d2++;
            }
        }
    }
    // TLS (2010): Motion Plus mode
    mpls = WPADiGetMplsStatus(chan);
    if (kp->unk_0x67a)
    {
        goto finish;
    }
    if (kp->unk_0x67b && mpls == kp->unk_0x677 && mpls == kp->unk_0x678)
    {
        if (kp->unk_0x678 == kp->unk_0x679)
        {
            kp->unk_0x67b = 0;
            if (kp->mplsCallback)
            {
                kp->mplsCallback(chan, 1);
            }
            kp->unk_0x67c = 1;
        }
        else
        {
            kp->unk_0x678 = kp->unk_0x679;
        }
    }
    else
    {
        if (kp->unk_0x67d)
        {
            kp->unk_0x67d--;
            goto finish;
        }
        kp->unk_0x676 = kp->unk_0x678;
        if (mpls && mpls != kp->unk_0x676)
        {
            kp->unk_0x676 = 0;
        }
        if (mpls != kp->unk_0x676)
        {
            if (!kp->unk_0x67b)
            {
                kp->unk_0x677 = 0;
            }
            kp->unk_0x67a = 1;
            if (!kp->unk_0x67b)
            {
                kp->unk_0x67b = 1;
                if (kp->mplsCallback)
                {
                    if (mpls || !kp->unk_0x678 || ((kp->lastDeviceType < 5 || kp->lastDeviceType > 7) && kp->lastDeviceType != 250))
                    {
                        err = 0;
                    }
                    else if (kp->unk_0x678 == 5)
                    {
                        err = -2;
                    }
                    else if (kp->unk_0x678 == 7)
                    {
                        err = -3;
                    }
                    else
                    {
                        err = -1;
                    }
                    kp->mplsCallback(chan, err);
                    if (err < 0 && !kp->unk_0x679)
                    {
                        kp->unk_0x678 = kp->unk_0x679;
                        kp->unk_0x676 = kp->unk_0x679;
                    }
                }
                kp->unk_0x67c = 0;
            }
            WPADiControlMpls(chan, kp->unk_0x676, fn_80658300);
        }
        else
        {
            kp->unk_0x678 = kp->unk_0x679;
        }
    }
    if (!mpls && !kp->unk_0x679)
    {
        kp->unk_0x678 = kp->unk_0x679;
    }
finish:
    kp->lastDeviceType = type;
    if (kp->appSamplingCallback)
    {
        kp->appSamplingCallback(chan);
    }
}
