// inferred: vector and rotation helpers of the TLS (2010) KPAD Motion Plus code. A separate object in the
// original: it has its own .sdata2 pool (0.5f, 0.0f and 1.0f again after KPADMpls_unk's) and its own .data
// (the identity rotation). Name invented; written from the asm.
#include <revolution/kpad/KPADPriv.h>

#include <internal/fdlibm.h>

// inferred: identity attitude (also used by KPADMpls_unk.c)
KPADMPDir fn_unk_identity_dir = { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };

// inferred: rounds to the nearest integer
s32 fn_8065AED0(f32 x)
{
    if (x < 0.0f)
        return (s32)(x - 0.5f);
    return (s32)(0.5f + x);
}

// inferred: normalizes a vector in place and returns its previous length
f32 fn_8065AF20(Vec* v)
{
    f32 len = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);

    if (len != 0.0f)
    {
        f32 inv = 1.0f / len;
        v->x *= inv;
        v->y *= inv;
        v->z *= inv;
    }
    return len;
}

// inferred: makes the three axes of an attitude orthonormal again (until the sum of the cross product
// lengths reaches `limit`)
// NONMATCHING: 90% - same operations; the original keeps fewer FPRs (no f27) and stores Z.y before Z.x
void fn_8065AFB0(KPADMPDir* dir, f32 limit)
{
    Vec c[3];
    f32 len;
    f32 inv;
    f32 sum;

    do
    {
        len = sqrt(dir->X.x * dir->X.x + dir->X.y * dir->X.y + dir->X.z * dir->X.z);
        inv = 1.0f / len;
        dir->X.x *= inv;
        dir->X.y *= inv;
        dir->X.z *= inv;
        len = sqrt(dir->Y.x * dir->Y.x + dir->Y.y * dir->Y.y + dir->Y.z * dir->Y.z);
        inv = 1.0f / len;
        dir->Y.x *= inv;
        dir->Y.y *= inv;
        dir->Y.z *= inv;
        len = sqrt(dir->Z.x * dir->Z.x + dir->Z.y * dir->Z.y + dir->Z.z * dir->Z.z);
        inv = 1.0f / len;
        dir->Z.x *= inv;
        dir->Z.y *= inv;
        dir->Z.z *= inv;

        c[0].x = dir->Y.y * dir->Z.z - dir->Y.z * dir->Z.y;
        c[0].y = dir->Y.z * dir->Z.x - dir->Y.x * dir->Z.z;
        c[0].z = dir->Y.x * dir->Z.y - dir->Y.y * dir->Z.x;
        c[1].x = dir->Z.y * dir->X.z - dir->Z.z * dir->X.y;
        c[1].y = dir->Z.z * dir->X.x - dir->Z.x * dir->X.z;
        c[1].z = dir->Z.x * dir->X.y - dir->Z.y * dir->X.x;
        c[2].x = dir->X.y * dir->Y.z - dir->X.z * dir->Y.y;
        c[2].y = dir->X.z * dir->Y.x - dir->X.x * dir->Y.z;
        c[2].z = dir->X.x * dir->Y.y - dir->X.y * dir->Y.x;

        len = sqrt(c[0].x * c[0].x + c[0].y * c[0].y + c[0].z * c[0].z);
        inv = 1.0f / len;
        dir->X.x = 0.5f * (dir->X.x + inv * c[0].x);
        dir->X.y = 0.5f * (dir->X.y + inv * c[0].y);
        dir->X.z = 0.5f * (dir->X.z + inv * c[0].z);
        sum = len;
        len = sqrt(c[1].x * c[1].x + c[1].y * c[1].y + c[1].z * c[1].z);
        inv = 1.0f / len;
        dir->Y.x = 0.5f * (dir->Y.x + inv * c[1].x);
        dir->Y.y = 0.5f * (dir->Y.y + inv * c[1].y);
        dir->Y.z = 0.5f * (dir->Y.z + inv * c[1].z);
        sum += len;
        len = sqrt(c[2].x * c[2].x + c[2].y * c[2].y + c[2].z * c[2].z);
        inv = 1.0f / len;
        dir->Z.x = 0.5f * (dir->Z.x + inv * c[2].x);
        dir->Z.y = 0.5f * (dir->Z.y + inv * c[2].y);
        dir->Z.z = 0.5f * (dir->Z.z + inv * c[2].z);
        sum += len;
    } while (sum < limit);
}

// inferred: rotation that takes the direction `a` to `b` (both normalized), as the rows a, n, a x n mapped
// to b, n, b x n (n = normalized a x b)
// NONMATCHING: 53% - the products of a x b are reused for the a*b terms (const a, b), but the original keeps
// one FPR less and does not schedule the first loads into the prologue
void fn_8065B320(KPADMPDir* out, const Vec* a, const Vec* b)
{
    Vec n;
    Vec an;
    Vec bn;
    f32 len;
    f32 inv;

    n.x = a->y * b->z - a->z * b->y;
    n.y = a->z * b->x - a->x * b->z;
    n.z = a->x * b->y - a->y * b->x;
    len = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
    if (len == 0.0f)
    {
        *out = fn_unk_identity_dir;
        return;
    }
    inv = 1.0f / len;
    n.x *= inv;
    n.y *= inv;
    n.z *= inv;
    an.x = a->y * n.z - a->z * n.y;
    an.y = a->z * n.x - a->x * n.z;
    an.z = a->x * n.y - a->y * n.x;
    bn.x = b->y * n.z - b->z * n.y;
    bn.y = b->z * n.x - b->x * n.z;
    bn.z = b->x * n.y - b->y * n.x;
    out->X.x = n.x * n.x + bn.x * an.x + b->x * a->x;
    out->X.y = n.y * n.x + bn.y * an.x + b->y * a->x;
    out->X.z = n.z * n.x + bn.z * an.x + b->z * a->x;
    out->Y.x = n.x * n.y + bn.x * an.y + b->x * a->y;
    out->Y.y = n.y * n.y + bn.y * an.y + b->y * a->y;
    out->Y.z = n.z * n.y + bn.z * an.y + b->z * a->y;
    out->Z.x = n.x * n.z + bn.x * an.z + b->x * a->z;
    out->Z.y = n.y * n.z + bn.y * an.z + b->y * a->z;
    out->Z.z = n.z * n.z + bn.z * an.z + b->z * a->z;
}

// inferred: out = b * a (rows of b combined with the axes of a)
void fn_8065B630(KPADMPDir* a, KPADMPDir* b, KPADMPDir* out)
{
    out->X.x = b->X.x * a->X.x + b->X.y * a->Y.x + b->X.z * a->Z.x;
    out->X.y = b->X.x * a->X.y + b->X.y * a->Y.y + b->X.z * a->Z.y;
    out->X.z = b->X.x * a->X.z + b->X.y * a->Y.z + b->X.z * a->Z.z;
    out->Y.x = b->Y.x * a->X.x + b->Y.y * a->Y.x + b->Y.z * a->Z.x;
    out->Y.y = b->Y.x * a->X.y + b->Y.y * a->Y.y + b->Y.z * a->Z.y;
    out->Y.z = b->Y.x * a->X.z + b->Y.y * a->Y.z + b->Y.z * a->Z.z;
    out->Z.x = b->Z.x * a->X.x + b->Z.y * a->Y.x + b->Z.z * a->Z.x;
    out->Z.y = b->Z.x * a->X.y + b->Z.y * a->Y.y + b->Z.z * a->Z.y;
    out->Z.z = b->Z.x * a->X.z + b->Z.y * a->Y.z + b->Z.z * a->Z.z;
}

// inferred: interpolates two attitudes (t from a to b) and makes the result orthonormal
void fn_8065B7F0(KPADMPDir* a, KPADMPDir* b, KPADMPDir* out, f32 t)
{
    out->X.x = a->X.x + t * (b->X.x - a->X.x);
    out->X.y = a->X.y + t * (b->X.y - a->X.y);
    out->X.z = a->X.z + t * (b->X.z - a->X.z);
    out->Y.x = a->Y.x + t * (b->Y.x - a->Y.x);
    out->Y.y = a->Y.y + t * (b->Y.y - a->Y.y);
    out->Y.z = a->Y.z + t * (b->Y.z - a->Y.z);
    out->Z.x = a->Z.x + t * (b->Z.x - a->Z.x);
    out->Z.y = a->Z.y + t * (b->Z.y - a->Z.y);
    out->Z.z = a->Z.z + t * (b->Z.z - a->Z.z);
    fn_8065AFB0(out, 2.999f);
}
