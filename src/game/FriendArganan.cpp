#include "game/FriendArganan.h"
#include "game/HitInfo_unk.h"
#include "math/Vec.h"

FriendArganan::FriendArganan(atn::Task* parent, int type, void* data) : PlayerTask(parent, type, data) {}

FriendArganan::~FriendArganan() {}

// inferred: Arganan takes no damage nor knockback
void FriendArganan::vf3C(HitInfo_unk* hit) {
    hit->m10.x *= 0.0f;
    hit->m10.y *= 0.0f;
    hit->m10.z *= 0.0f;
    hit->m90 = 0;
    hit->m68 = 0;
}
