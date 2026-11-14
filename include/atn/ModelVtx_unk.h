#ifndef ATN_MODELVTX_UNK_H
#define ATN_MODELVTX_UNK_H

// inferred: vertex data of the model file, read by atn::LoadModel::fn_80474F0C (atn/LH_Model) and
// drawn by atn/ModelDraw_unk. Names invented.

// inferred: vertex array of a mesh: GX attribute, data and stride (12 bytes; GX_VA_NULL ends the list)
struct DrawArray_unk {
    int m00; // inferred: attribute (0xFF ends the list)
    void* m04;
    int m08;
};

// inferred: attribute of a vertex description of the file (seven words)
struct LoadVtxAttr_unk {
    int m00;
    int mKind;  // 0x04: 0 position, 1 normal, 2 texture coordinates, 3 color, 4 position matrix, 5 texture matrix
    int mIndex; // 0x08: color / texture coordinate / matrix number
    int mType;  // 0x0C: component type of the file; in the description of a mesh, 2 is u8 indices
    int m10;
    int mFrac;  // 0x14: fractional bits
    int m18;
};
// inferred: chunk 'sdsc': vertex description
struct LoadVtxDesc_unk {
    unsigned char m00[0x18];
    int mCount;              // 0x18
    LoadVtxAttr_unk* mAttrs; // 0x1C
};
// inferred: chunk 'strm': vertex stream
struct LoadVtxStream_unk {
    unsigned char m00[0x14];
    LoadVtxDesc_unk* mDesc; // 0x14
    unsigned char m18[0x1C - 0x18];
    void* mData; // 0x1C
};

#endif
