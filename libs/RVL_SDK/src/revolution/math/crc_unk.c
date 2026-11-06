// Source: mscharged-decomp (CC0), the CRC part of src/RVL_SDK/dwc/dwci_np_math.c (NitroSDK MATH CRC).
// inferred: file name; in The Last Story these four functions sit between NWC24 and SO, only DWC calls them.
#include <nitro/math/crc.h>

void MATHi_CRC8InitTable(MATHCRC8Table* table, u8 poly)
{
    u32 r, i, j;
    u8* t = table->table;

    for (i = 0; i < 256; i++)
    {
        r = i;
        for (j = 0; j < 8; j++)
        {
            if (r & 0x80)
            {
                r = (r << 1) ^ poly;
            }
            else
            {
                r <<= 1;
            }
        }
        t[i] = (u8)r;
    }
}

void MATHi_CRC32InitTableRev(MATHCRC32Table* table, u32 poly)
{
    u32 r, i, j;
    u32* t = table->table;

    for (i = 0; i < 256; i++)
    {
        r = i;
        for (j = 0; j < 8; j++)
        {
            if (r & 1)
            {
                r = (r >> 1) ^ poly;
            }
            else
            {
                r >>= 1;
            }
        }
        t[i] = r;
    }
}

void MATHi_CRC8Update(const MATHCRC8Table* table, MATHCRC8Context* context,
    const void* input, u32 length)
{
    u32 r, i;
    const u8* t = table->table;
    u8* data = (u8*)input;

    r = *context;
    for (i = 0; i < length; i++)
    {
        r = t[(r ^ *data) & 0xff];
        data++;
    }
    *context = (MATHCRC8Context)r;
}

void MATHi_CRC32UpdateRev(const MATHCRC32Table* table,
    MATHCRC32Context* context, const void* input,
    u32 length)
{
    u32 r, i;
    const u32* t = table->table;
    u8* data = (u8*)input;

    r = *context;
    for (i = 0; i < length; i++)
    {
        r = (r >> 8) ^ t[(r ^ *data) & 0xff];
        data++;
    }
    *context = r;
}

u8 MATH_CalcCRC8(const MATHCRC8Table* table, const void* data,
    u32 dataLength)
{
    MATHCRC8Context context = 0;
    MATHi_CRC8Update(table, &context, data, dataLength);
    return context;
}

u32 MATH_CalcCRC32(const MATHCRC32Table* table, const void* data,
    u32 dataLength)
{
    MATHCRC32Context context = (MATHCRC32Context)(~0);
    MATHi_CRC32UpdateRev(table, &context, data, dataLength);
    return ~context;
}
