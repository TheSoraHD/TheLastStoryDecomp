#include "game/FriendArganan.h"
#include "game/HitInfo_unk.h"
#include "math/Vec.h"

FriendArganan::FriendArganan(atn::Task* parent, int type, void* data) : PlayerTask(parent, type, data) {}

FriendArganan::~FriendArganan() {}

// inferred: Arganan takes no damage nor knockback (the parameter is the hit message, as in
// CharaTask::vf3C)
void FriendArganan::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    msg->m10.x *= 0.0f;
    msg->m10.y *= 0.0f;
    msg->m10.z *= 0.0f;
    hit->m90.mFlags = 0;
    hit->m68 = 0;
}
