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
// NONMATCHING: 51% - operation order of the nine sums not found
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
    out->X.x = a->x * b->x + n.x * n.x + an.x * bn.x;
    out->X.y = a->x * b->y + n.x * n.y + an.x * bn.y;
    out->X.z = a->x * b->z + n.x * n.z + an.x * bn.z;
    out->Y.x = a->y * b->x + n.y * n.x + an.y * bn.x;
    out->Y.y = a->y * b->y + n.y * n.y + an.y * bn.y;
    out->Y.z = a->y * b->z + n.y * n.z + an.y * bn.z;
    out->Z.x = a->z * b->x + n.z * n.x + an.z * bn.x;
    out->Z.y = a->z * b->y + n.z * n.y + an.z * bn.y;
    out->Z.z = a->z * b->z + n.z * n.z + an.z * bn.z;
}

// inferred: out = b * a (rows of b combined with the axes of a)
// NONMATCHING: 25% - the original loads after every store (no alias assumption); MWCC -O4,p hoists all
// the loads here (also with Vec members, flat floats or Wii/1.1-1.7)
void fn_8065B630(const KPADMPDir* a, const KPADMPDir* b, KPADMPDir* out)
{
    const f32(*ma)[3] = (const f32(*)[3])a;
    const f32(*mb)[3] = (const f32(*)[3])b;
    f32(*mo)[3] = (f32(*)[3])out;
    int i;
    int j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            mo[i][j] = mb[i][0] * ma[0][j] + mb[i][1] * ma[1][j] + mb[i][2] * ma[2][j];
        }
    }
}

// inferred: interpolates two attitudes (t from a to b) and makes the result orthonormal
// NONMATCHING: 26% - same as fn_8065B630: the original does not move the loads above the stores
void fn_8065B7F0(const KPADMPDir* a, const KPADMPDir* b, KPADMPDir* out, f32 t)
{
    const f32* pa = (const f32*)a;
    const f32* pb = (const f32*)b;
    f32* po = (f32*)out;
    int i;

    for (i = 0; i < 9; i++)
    {
        po[i] = pa[i] + t * (pb[i] - pa[i]);
    }
    fn_8065AFB0(out, 2.999f);
}
