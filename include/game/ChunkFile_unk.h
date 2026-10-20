#ifndef GAME_CHUNKFILE_UNK_H
#define GAME_CHUNKFILE_UNK_H

// inferred: reader of a binary file made of tagged chunks (functions 0x80442B50-0x80442F10, at the end
// of the game/HomingBullet split); the file starts with a 4-byte magic and stores self-relative
// offsets that the constructor turns into pointers, byte swapping the headers when the magic is
// reversed. Used by the collision data (ColliObjectData_unk "@HCB", ColliTreeData_unk "@HOC").
// All names invented.
struct ChunkFile_unk {
    // inferred: chunk descriptor (16 bytes)
    struct Chunk {
        int mType;            // 0x00
        unsigned int m04;
        void* mData;          // 0x08
        unsigned int mSize;   // 0x0C: in bytes
    };
    // inferred: table of chunks
    struct ChunkTable {
        unsigned int mCount;
        unsigned int m04;
        Chunk mChunks[1];     // 0x08
    };

    ChunkFile_unk(void* data, unsigned int magic); // 0x80442B50
    ~ChunkFile_unk();                              // 0x80442E64
    int IsSwapped() const;                         // 0x80442EA4
    unsigned int GetChunkCount() const;            // 0x80442EAC
    Chunk* FindChunk(int type, unsigned int index); // 0x80442ECC

    int mValid;          // 0x00
    int mSwapped;        // 0x04
    unsigned int mMagic; // 0x08
    void* mData;         // 0x0C
    ChunkTable* m10;     // 0x10
    void* m14;
    void* m18;
    void* m1C;
};

#endif
