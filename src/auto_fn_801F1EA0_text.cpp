typedef float f32;

struct Vec4f {
    f32 x, y, z, w;
    Vec4f(f32 a, f32 b, f32 c, f32 d) : x(a), y(b), z(c), w(d) {}
    Vec4f() {}
};

// .sdata2 float constants
extern const f32 lbl_80882C44; // A
extern const f32 lbl_80882C48; // B
extern const f32 lbl_80882C4C; // C
extern const f32 lbl_80882C50; // D
extern const f32 lbl_80882C54; // E

#define A lbl_80882C44
#define B lbl_80882C48
#define C lbl_80882C4C
#define D lbl_80882C50
#define E lbl_80882C54

// .bss (size 0x220)
Vec4f lbl_807C7D48[34];

// .ctors entry -> fn_801F1EA0
static void fn_801F1EA0(void) {
    lbl_807C7D48[0]  = Vec4f(A, A, B, B);
    lbl_807C7D48[1]  = Vec4f(A, A, B, B);
    lbl_807C7D48[2]  = Vec4f(A, C, D, E);
    lbl_807C7D48[3]  = Vec4f(A, A, D, D);
    lbl_807C7D48[4]  = Vec4f(D, C, C, E);
    lbl_807C7D48[5]  = Vec4f(D, A, C, D);
    lbl_807C7D48[6]  = Vec4f(C, C, E, E);
    lbl_807C7D48[7]  = Vec4f(C, A, E, D);
    lbl_807C7D48[8]  = Vec4f(E, C, B, E);
    lbl_807C7D48[9]  = Vec4f(E, A, B, D);
    lbl_807C7D48[10] = Vec4f(A, E, D, B);
    lbl_807C7D48[11] = Vec4f(A, A, D, D);
    lbl_807C7D48[12] = Vec4f(D, E, C, B);
    lbl_807C7D48[13] = Vec4f(A, A, D, D);
    lbl_807C7D48[14] = Vec4f(C, E, E, B);
    lbl_807C7D48[15] = Vec4f(A, A, D, D);
    lbl_807C7D48[16] = Vec4f(E, E, B, B);
    lbl_807C7D48[17] = Vec4f(A, A, D, D);
    lbl_807C7D48[18] = Vec4f(A, A, D, D);
    lbl_807C7D48[19] = Vec4f(A, A, D, D);
    lbl_807C7D48[20] = Vec4f(D, A, C, D);
    lbl_807C7D48[21] = Vec4f(A, A, D, D);
    lbl_807C7D48[22] = Vec4f(C, A, E, D);
    lbl_807C7D48[23] = Vec4f(A, A, D, D);
    lbl_807C7D48[24] = Vec4f(E, A, B, D);
    lbl_807C7D48[25] = Vec4f(A, A, D, D);
    lbl_807C7D48[26] = Vec4f(A, D, D, C);
    lbl_807C7D48[27] = Vec4f(A, D, D, C);
    lbl_807C7D48[28] = Vec4f(D, D, C, C);
    lbl_807C7D48[29] = Vec4f(E, D, B, C);
    lbl_807C7D48[30] = Vec4f(C, D, E, C);
    lbl_807C7D48[31] = Vec4f(C, D, E, C);
    lbl_807C7D48[32] = Vec4f(E, D, B, C);
    lbl_807C7D48[33] = Vec4f(C, D, E, C);
}
