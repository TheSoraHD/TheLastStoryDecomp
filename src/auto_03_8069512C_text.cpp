extern "C" {

typedef struct Unk {
    unsigned char field_0x00[0x0C];
    int field_0x0C;
} Unk;

int fn_80691F24(int arg0, int arg1);

int fn_8069512C(Unk* self) {
    return fn_80691F24(self->field_0x0C, 1);
}

}