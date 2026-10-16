extern "C" {

typedef struct Unk {
    int field_0x00;
    int field_0x04;
    int field_0x08;
    int field_0x0C;
    const char* table;
} Unk;

char fn_806929FC(Unk* self, char c) {
    return self->table[(unsigned char)c];
}

char* fn_80692A10(Unk* self, char* lo, const char* hi) {
    for (; lo < hi; ++lo) {
        *lo = self->table[(unsigned char)*lo];
    }
    return (char*)hi;
}

}