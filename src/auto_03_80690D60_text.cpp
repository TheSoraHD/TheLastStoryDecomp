extern "C" {

typedef struct Unk {
    unsigned char field_0x00[0x08];
    int field_0x08;
    int field_0x0C;
} Unk;

int fn_80690D60(Unk* self) {
    return self->field_0x0C - self->field_0x08;
}

}