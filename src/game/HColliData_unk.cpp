// inferred: collision data loaded from chunk files: ColliObjectData_unk (magic "@HCB", used by
// atn::LoadColliObject) and ColliTreeData_unk (magic "@HOC", used by atn::LoadColliTree). The
// constructors byte swap the chunks when the file has the other byte order and turn the
// self-relative offsets of the tree into pointers. Real file name unknown: the link order puts it
// between game/Gun and game/HomeButtonMenu. All names invented.
#include "game/ChunkFile_unk.h"

typedef unsigned int u32_unk;

static const char sObjectMagic[] = "@HCB";
static const char sTreeMagic[] = "@HOC";

// inferred: byte swap helpers
inline void SwapU32_unk(void* p) {
    __stwbrx(*(u32_unk*)p, p, 0);
}

inline void SwapF32_unk(void* p) {
    float f = *(float*)p;
    __stwbrx(*(u32_unk*)&f, p, 0);
}

// inferred: self-relative offset stored in place
inline void* Rel_unk(void* field) {
    return (char*)field + *(int*)field;
}

// ---------------------------------------------------------------------------------------------
// Collision object

// inferred: collision object data (chunk 0x208 is the object)
struct ColliObjectData_unk {
    ColliObjectData_unk(void* data);
    ~ColliObjectData_unk();
    void fn_8043F694(); // inferred: byte swaps the chunks
    void fn_8043F908(void* p); // inferred: byte swaps one chunk 0x207 entry

    ChunkFile_unk mFile;
    void* m20; // the collision object
};

ColliObjectData_unk::ColliObjectData_unk(void* data) : mFile(data, *(const u32_unk*)sObjectMagic) {
    m20 = 0;
    if (mFile.IsSwapped()) {
        fn_8043F694();
    }
    ChunkFile_unk::Chunk* c = mFile.FindChunk(0x208, 0);
    if (c != 0) {
        m20 = c->mData;
    }
}

ColliObjectData_unk::~ColliObjectData_unk() {}

// NONMATCHING: 38.4% - the original unrolls the byte swap loops (Wii/1.3 never unrolls a loop
// whose body uses __stwbrx) and inlines the recursion once
void ColliObjectData_unk::fn_8043F694() {
    ChunkFile_unk::Chunk* c = mFile.m10->mChunks;
    u32_unk count = mFile.GetChunkCount();
    for (u32_unk i = 0; i < count; i++, c++) {
        switch (c->mType) {
        case 0x205: {
            char* p = (char*)c->mData;
            for (u32_unk j = 0; j < c->mSize / 0x44; j++) {
                for (int k = 0; k < 3; k++) {
                    SwapF32_unk(p + k * 4);
                    SwapF32_unk(p + 0x0C + k * 4);
                    SwapF32_unk(p + 0x18 + k * 4);
                    SwapF32_unk(p + 0x24 + k * 4);
                    SwapF32_unk(p + 0x30 + k * 4);
                }
                SwapF32_unk(p + 0x3C);
                p += 0x44;
            }
            break;
        }
        case 0x206: {
            char* p = (char*)c->mData;
            SwapU32_unk(p);
            SwapU32_unk(p + 4);
            SwapF32_unk(p + 0x18);
            SwapF32_unk(p + 0x0C);
            SwapF32_unk(p + 0x10);
            SwapF32_unk(p + 0x14);
            break;
        }
        case 0x207:
            fn_8043F908(c->mData);
            break;
        case 0x208: {
            char* p = (char*)c->mData;
            SwapU32_unk(p);
            SwapU32_unk(p + 4);
            SwapU32_unk(p + 8);
            break;
        }
        case 0x209: {
            char* p = (char*)c->mData;
            for (u32_unk j = 0; j < c->mSize / 0x20; j++) {
                SwapU32_unk(p);
                SwapU32_unk(p + 0x04);
                SwapU32_unk(p + 0x08);
                SwapU32_unk(p + 0x0C);
                SwapU32_unk(p + 0x10);
                SwapU32_unk(p + 0x14);
                SwapU32_unk(p + 0x18);
                SwapU32_unk(p + 0x1C);
                p += 0x20;
            }
            break;
        }
        }
    }
}

// NONMATCHING: 72.7% - the original unrolls the byte swap loops (Wii/1.3 never unrolls a loop
// whose body uses __stwbrx) and inlines the recursion once
void ColliObjectData_unk::fn_8043F908(void* data) {
    char* p = (char*)data;
    SwapU32_unk(p);
    SwapU32_unk(p + 4);
    SwapU32_unk(p + 8);
    for (int k = 0; k < 3; k++) {
        SwapF32_unk(p + 0x10 + k * 4);
        SwapF32_unk(p + 0x2C + k * 4);
        SwapF32_unk(p + 0x50 + k * 4);
        SwapF32_unk(p + 0x5C + k * 4);
    }
    SwapF32_unk(p + 0x1C);
    SwapF32_unk(p + 0x20);
    SwapF32_unk(p + 0x24);
    SwapF32_unk(p + 0x28);
}

// ---------------------------------------------------------------------------------------------
// Collision tree

// inferred: node of the tree (eight children and a list of polygons)
struct ColliTreeNode_unk {
    u32_unk mCount;      // 0x00: number of entries of m3C
    float m04[3];        // inferred: box
    float m10[3];
    int mChildren[8];    // 0x1C: self-relative offsets, then pointers
    int m3C;             // 0x3C: self-relative offset of the entry list
};

// inferred: entry of chunk 0x203 (0x48 bytes; m00 is a self-relative offset, 1 = none)
struct ColliTreeEntry_unk {
    int m00;
    int m04;
    float m08[3];
    float m14[3];
    float m20[3];
    float m2C[3];
    float m38[3];
    float m44;
};

// inferred: header of the tree (chunk 0x200)
struct ColliTreeRoot_unk {
    int m00;
    int m04;
    float m08;
    float m0C;
    float m10;
    int mTop; // 0x14: self-relative offset of the top node
};

struct ColliTreeData_unk {
    ColliTreeData_unk(void* data);
    ~ColliTreeData_unk();
    void fn_8043FEE8(); // inferred: byte swaps the chunks
    void fn_80440184(ColliTreeNode_unk* node); // inferred: byte swaps a node
    void fn_80440280(void* p, u32_unk count);   // inferred: byte swaps the 0x48-byte entries of chunk 0x203
    void fn_80440360(ColliTreeNode_unk* node); // inferred: relocates a node and its children
    void fn_8044058C(int* list, u32_unk count); // inferred: relocates a list of offsets

    void SwapRoot_unk(ColliTreeRoot_unk* root) {
        SwapU32_unk(&root->m00);
        SwapU32_unk(&root->m04);
        SwapF32_unk(&root->m08);
        SwapF32_unk(&root->m0C);
        SwapF32_unk(&root->m10);
        if (m2C) {
            SwapU32_unk(&root->mTop);
        }
    }
    void SwapEntries_unk(void* data, u32_unk count) {
        char* p = (char*)data;
        for (u32_unk j = 0; j < count; j++) {
            SwapU32_unk(p);
            SwapU32_unk(p + 8);
            SwapU32_unk(p + 4);
            p += 0x20;
        }
    }
    ChunkFile_unk mFile;
    ColliTreeRoot_unk* m20; // the collision tree
    void* m24;              // inferred: chunk 0x203 (0x48-byte entries)
    u32_unk m28;            // inferred: number of entries of m24
    int m2C;                // inferred: the offsets are already in the native byte order
    int m30;                // inferred: the file is byte swapped
};

// NONMATCHING: 49.3% - the original unrolls the byte swap loops (Wii/1.3 never unrolls a loop
// whose body uses __stwbrx) and inlines the recursion once
ColliTreeData_unk::ColliTreeData_unk(void* data) : mFile(data, *(const u32_unk*)sTreeMagic) {
    m20 = 0;
    m24 = 0;
    m28 = 0;
    m2C = ((u32_unk*)mFile.mData)[3] & 4;
    m30 = mFile.IsSwapped();
    if (m2C == 0 && m30 != 0) {
        fn_8043FEE8();
    }
    ChunkFile_unk::Chunk* c = mFile.FindChunk(0x200, 0);
    if (c != 0) {
        m20 = (ColliTreeRoot_unk*)c->mData;
    }
    ColliTreeRoot_unk* root = m20;
    if (root != 0) {
        if (m30) {
            SwapRoot_unk(root);
        }
        if (root->mTop != 0) {
            ColliTreeNode_unk* top = (ColliTreeNode_unk*)Rel_unk(&root->mTop);
            root->mTop = (int)top;
            fn_80440360(top);
        }
    }
    c = mFile.FindChunk(0x203, 0);
    if (c != 0) {
        char* p = (char*)c->mData;
        u32_unk n = c->mSize / 0x48;
        m24 = p;
        m28 = n;
        if (p != 0) {
            if (m30) {
                fn_80440280(p, n);
            }
            for (u32_unk k = 0; k < n; k++) {
                if (*(u32_unk*)p != 1) {
                    *(int*)p = (int)Rel_unk(p);
                }
                p += 0x48;
            }
        }
    }
    c = mFile.FindChunk(0x204, 0);
    if (c != 0) {
        void* p = c->mData;
        u32_unk n = c->mSize >> 5;
        if (p != 0 && m30) {
            SwapEntries_unk(p, n);
        }
    }
}

ColliTreeData_unk::~ColliTreeData_unk() {}

// NONMATCHING: 60.2% - the original unrolls the byte swap loops (Wii/1.3 never unrolls a loop
// whose body uses __stwbrx) and inlines the recursion once
void ColliTreeData_unk::fn_8043FEE8() {
    ChunkFile_unk::Chunk* c = mFile.m10->mChunks;
    u32_unk count = mFile.GetChunkCount();
    for (u32_unk i = 0; i < count; i++, c++) {
        switch (c->mType) {
        case 0x201:
            fn_80440184((ColliTreeNode_unk*)c->mData);
            break;
        case 0x200:
            SwapRoot_unk((ColliTreeRoot_unk*)c->mData);
            break;
        case 0x203:
            fn_80440280(c->mData, c->mSize / 0x48);
            break;
        case 0x204:
            SwapEntries_unk(c->mData, c->mSize >> 5);
            break;
        }
    }
}

// NONMATCHING: 44.9% - the original unrolls the byte swap loops (Wii/1.3 never unrolls a loop
// whose body uses __stwbrx) and inlines the recursion once
void ColliTreeData_unk::fn_80440184(ColliTreeNode_unk* node) {
    SwapU32_unk(&node->mCount);
    for (int k = 0; k < 3; k++) {
        SwapF32_unk(&node->m04[k]);
        SwapF32_unk(&node->m10[k]);
    }
    if (m2C) {
        for (int k = 0; k < 8; k++) {
            SwapU32_unk(&node->mChildren[k]);
        }
        SwapU32_unk(&node->m3C);
    }
}

// NONMATCHING: 66.2% - the original unrolls the byte swap loops (Wii/1.3 never unrolls a loop
// whose body uses __stwbrx) and inlines the recursion once
void ColliTreeData_unk::fn_80440280(void* data, u32_unk count) {
    ColliTreeEntry_unk* e = (ColliTreeEntry_unk*)data;
    for (u32_unk i = 0; i < count; i++) {
        SwapU32_unk(&e->m00);
        SwapU32_unk(&e->m04);
        for (int k = 0; k < 3; k++) {
            SwapF32_unk(&e->m08[k]);
            SwapF32_unk(&e->m14[k]);
            SwapF32_unk(&e->m20[k]);
            SwapF32_unk(&e->m2C[k]);
            SwapF32_unk(&e->m38[k]);
        }
        SwapF32_unk(&e->m44);
        e++;
    }
}

// NONMATCHING: 56.3% - the original unrolls the byte swap loops (Wii/1.3 never unrolls a loop
// whose body uses __stwbrx) and inlines the recursion once
void ColliTreeData_unk::fn_80440360(ColliTreeNode_unk* node) {
    if (node == 0) {
        return;
    }
    if (m30) {
        fn_80440184(node);
    }
    for (int i = 0; i < 8; i++) {
        if (node->mChildren[i] != 0) {
            ColliTreeNode_unk* child = (ColliTreeNode_unk*)Rel_unk(&node->mChildren[i]);
            node->mChildren[i] = (int)child;
            fn_80440360(child);
        }
    }
    if (node->m3C != 0) {
        int* list = (int*)Rel_unk(&node->m3C);
        node->m3C = (int)list;
        fn_8044058C(list, node->mCount);
    }
}

void ColliTreeData_unk::fn_8044058C(int* list, u32_unk count) {
    if (list != 0) {
        for (u32_unk k = 0; k < count; k++) {
            if (m30) {
                SwapU32_unk(&list[k]);
            }
            list[k] = (int)Rel_unk(&list[k]);
        }
    }
}
