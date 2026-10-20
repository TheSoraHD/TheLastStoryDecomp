#include "atn/ParseTree.h"
#include "atn/Memory.h"
#include "std/new.h"

extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int strcmp(const char* a, const char* b);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: offset of the next free node in ParseTree::m08
static int sNodeOffset;
// inferred: set while the first leaf node has not been placed in ParseTree::m0C
static int sFirstLeaf;
// inferred: value of the variable that compiles to (float*)-1
static float sArgument;

static inline bool IsIntFunc(const char* s) {
    return strncmp(s, "INT", 3) == 0;
}

static inline bool IsFunc(const char* s) {
    if (strncmp(s, "MAX", 3) == 0) {
        return true;
    }
    if (strncmp(s, "MIN", 3) == 0) {
        return true;
    }
    if (strncmp(s, "POW", 3) == 0) {
        return true;
    }
    if (strncmp(s, "MOD", 3) == 0) {
        return true;
    }
    return false;
}

static inline bool IsOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

static inline bool IsNameChar(char c) {
    return ('A' <= c && c <= 'z') || ('0' <= c && c <= '9') || c == '.' || c == '_';
}

// inferred: reads one token of expr into out, returns the rest of expr
extern "C" const char* fn_800A5B40(char* out, const char* expr) {
    *out = *expr++;
    if (IsIntFunc(out)) {
        return expr;
    }
    if (IsFunc(out)) {
        return expr;
    }
    if (IsOperator(*out)) {
        return expr;
    }
    if (*out == ',') {
        return expr;
    }
    if (*out == '(') {
        return expr;
    }
    if (*out == ')') {
        return expr;
    }
    out++;
    while (*expr != 0) {
        if (!IsNameChar(*expr)) {
            break;
        }
        *out++ = *expr++;
    }
    return expr;
}

namespace atn {

ParseTree::ParseTree() : m04(0), m08(0), m14(0) {
    memset(m0C, 0, sizeof(m0C));
}

ParseTree::~ParseTree() {
    Clear_unk();
}

int ParseTree::fn_800A5E00(const char* expr) {
    Clear_unk();
    int nodes = 0;
    if (fn_800A5F18(expr, &nodes, &m14)) {
        if (m14 > 0) {
            int size = m14 * sizeof(ValueNode) + m14 * sizeof(OperatorNode);
            m08 = new (8, "", "", 0) unsigned char[size];
            memset(m08, 0, size);
        }
        sNodeOffset = 0;
        sFirstLeaf = 1;
        m04 = fn_800A6198(expr, -1);
    }
    if (m04 == 0) {
        return 0;
    }
    return 1;
}

int ParseTree::fn_800A5F18(const char* str, int* nodes, int* ops) {
    char buf[0x40];
    const char* expr = str;
    int operand = 0;
    int depth = 0;
    int args = 0;
    int count = 0;
    while (*expr != 0) {
        memset(buf, 0, sizeof(buf));
        expr = fn_800A5B40(buf, expr);
        if (IsIntFunc(buf)) {
            count++;
        } else if (IsFunc(buf)) {
            count++;
            args++;
        } else if (IsOperator(buf[0])) {
            operand = 0;
            count++;
        } else if (IsNameChar(buf[0])) {
            operand = 1;
        } else if (strcmp(buf, ",") == 0) {
            args--;
        } else if (strcmp(buf, "(") == 0) {
            operand = 0;
            depth++;
        } else if (strcmp(buf, ")") == 0) {
            operand = 0;
            depth--;
        } else {
            return 0;
        }
    }
    if (operand == 0 && strcmp(buf, ")") != 0) {
        return 0;
    }
    if (depth != 0) {
        return 0;
    }
    if (args != 0) {
        return 0;
    }
    *nodes = count + 1;
    *ops = count;
    return 1;
}

static inline int IntFuncIndex(const char* s) {
    return strncmp(s, "INT", 3) ? -1 : 0;
}

static inline int FuncIndex(const char* s) {
    if (strncmp(s, "MAX", 3) == 0) {
        return 0;
    }
    if (strncmp(s, "MIN", 3) == 0) {
        return 1;
    }
    if (strncmp(s, "POW", 3) == 0) {
        return 2;
    }
    return strncmp(s, "MOD", 3) != 0 ? -1 : 3;
}

// inferred: copies the text between the parentheses at expr, returns the text after them
static inline const char* ReadParen(char* out, const char* expr) {
    int depth = 1;
    const char* p = expr + 1;
    while (*p != 0) {
        if (*p == '(') {
            depth++;
        } else if (*p == ')') {
            if (--depth == 0) {
                return p + 1;
            }
        }
        *out++ = *p++;
    }
    return 0;
}

BaseNode* ParseTree::fn_800A6198(const char* str, int func) {
    char buf[0x40];
    const char* expr = str;
    BaseNode* root = 0;
    OperatorNode* cur = 0;
    while (*expr != 0) {
        memset(buf, 0, sizeof(buf));
        if (IsIntFunc(expr)) {
            expr = fn_800A5B40(buf, expr);
            int idx = IntFuncIndex(buf);
            OperatorNode* node = new (m08 + sNodeOffset) OperatorNode();
            sNodeOffset += sizeof(OperatorNode);
            switch (idx) {
            case 0:
                node->m04 = 'i';
                break;
            }
            if (*expr == '(') {
                expr = ReadParen(buf, expr);
                node->m08 = fn_800A6198(buf, idx);
                node->m0C = vf0C("0");
                if (cur != 0 && cur->m0C == 0) {
                    cur->m0C = node;
                } else if (root == 0) {
                    root = node;
                }
            }
        } else if (IsFunc(expr)) {
            expr = fn_800A5B40(buf, expr);
            int idx = FuncIndex(buf);
            if (*expr == '(') {
                expr = ReadParen(buf, expr);
                BaseNode* node = fn_800A6198(buf, idx);
                if (cur != 0 && cur->m0C == 0) {
                    cur->m0C = node;
                } else if (root == 0) {
                    root = node;
                }
            }
        } else if (IsNameChar(*expr)) {
            expr = fn_800A5B40(buf, expr);
            BaseNode* node = vf0C(buf);
            if (cur != 0 && cur->m0C == 0) {
                cur->m0C = node;
            } else if (root == 0) {
                root = node;
            }
        } else if (*expr == '(') {
            expr = ReadParen(buf, expr);
            BaseNode* node = fn_800A6198(buf, -1);
            if (cur != 0 && cur->m0C == 0) {
                cur->m0C = node;
            } else if (root == 0) {
                root = node;
            }
        } else if (*expr == ',' && func >= 0) {
            expr = fn_800A5B40(buf, expr);
            cur = new (m08 + sNodeOffset) OperatorNode();
            sNodeOffset += sizeof(OperatorNode);
            switch (func) {
            case 0:
                cur->m04 = 'x';
                break;
            case 1:
                cur->m04 = 'n';
                break;
            case 2:
                cur->m04 = 'p';
                break;
            case 3:
                cur->m04 = 'm';
                break;
            }
            cur->m08 = root;
            root = cur;
        } else if (IsOperator(*expr)) {
            OperatorNode* node = new (m08 + sNodeOffset) OperatorNode();
            sNodeOffset += sizeof(OperatorNode);
            node->m04 = *expr++;
            if (cur == 0) {
                node->m08 = root;
                root = node;
                cur = node;
            } else if (node->m04 == '*' || node->m04 == '/') {
                if (cur->m04 == '*' || cur->m04 == '/') {
                    node->m08 = cur->m08;
                    cur->m08 = node;
                    cur = node;
                } else {
                    node->m08 = cur->m0C;
                    cur->m0C = node;
                    cur = node;
                }
            } else {
                node->m08 = root;
                root = node;
                cur = node;
            }
        } else {
            expr++;
        }
    }
    return root;
}

BaseNode* ParseTree::vf0C(const char* name) {
    ValueNode* node = fn_800A686C();
    node->m04 = fn_800DC288(name);
    return node;
}

ValueNode* ParseTree::fn_800A686C() {
    if (sFirstLeaf) {
        ValueNode* node = new (m0C) ValueNode();
        sFirstLeaf = 0;
        return node;
    }
    ValueNode* node = new (m08 + sNodeOffset) ValueNode();
    sNodeOffset += sizeof(ValueNode);
    return node;
}

VariableNode* ParseTree::fn_800A68D4() {
    if (sFirstLeaf) {
        VariableNode* node = new (m0C) VariableNode();
        sFirstLeaf = 0;
        return node;
    }
    VariableNode* node = new (m08 + sNodeOffset) VariableNode();
    sNodeOffset += sizeof(VariableNode);
    return node;
}

} // namespace atn

// inferred: writes the byte code of a node
extern "C" void fn_800A693C(unsigned char* code, atn::BaseNode* node, int* pos) {
    if (node == 0) {
        return;
    }
    int kind = node->vf10();
    if (kind == 1) {
        code[(*pos)++] = 0;
        memcpy(code + *pos, &((atn::ValueNode*)node)->m04, 4);
        *pos += 4;
    } else if (kind == 2) {
        code[(*pos)++] = 1;
        float* var = ((atn::VariableNode*)node)->m04;
        if (var == (float*)-1) {
            *(float**)(code + *pos) = &sArgument;
        } else {
            *(float**)(code + *pos) = var;
        }
        *pos += 4;
    } else if (kind == 3) {
        atn::OperatorNode* op = (atn::OperatorNode*)node;
        if (op->m08 == 0) {
            code[(*pos)++] = 0;
            float zero = 0.0f;
            memcpy(code + *pos, &zero, 4);
            *pos += 4;
        } else {
            fn_800A693C(code, op->m08, pos);
        }
        fn_800A693C(code, op->m0C, pos);
        switch (op->m04) {
        case '+':
            code[(*pos)++] = 2;
            break;
        case '-':
            code[(*pos)++] = 3;
            break;
        case '*':
            code[(*pos)++] = 4;
            break;
        case '/':
            code[(*pos)++] = 5;
            break;
        case 'm':
            code[(*pos)++] = 9;
            break;
        case 'x':
            code[(*pos)++] = 6;
            break;
        case 'n':
            code[(*pos)++] = 7;
            break;
        case 'p':
            code[(*pos)++] = 8;
            break;
        case 'i':
            code[(*pos)++] = 0x10;
            break;
        }
    }
}

extern "C" float fn_800A6BDC(const unsigned char* code);

// inferred: evaluates byte code with the given argument
extern "C" float fn_800A6BD4(const unsigned char* code, float arg) {
    sArgument = arg;
    return fn_800A6BDC(code);
}

// inferred: evaluates byte code
extern "C" float fn_800A6BDC(const unsigned char* code) {
    float stack[0x20];
    int sp = 0;
    stack[0] = 0.0f;
    while (*code != 0xFF) {
        switch (*code) {
        case 0: {
            float v = 0.0f;
            memcpy(&v, code + 1, 4);
            stack[sp++] = v;
            code += 5;
            break;
        }
        case 1: {
            float* var = *(float**)(code + 1);
            if (var != 0) {
                stack[sp++] = *var;
            } else {
                stack[sp++] = 0.0f;
            }
            code += 5;
            break;
        }
        case 2:
            stack[sp - 2] = stack[sp - 2] + stack[sp - 1];
            sp--;
            code++;
            break;
        case 3:
            stack[sp - 2] = stack[sp - 2] - stack[sp - 1];
            sp--;
            code++;
            break;
        case 4:
            stack[sp - 2] = stack[sp - 2] * stack[sp - 1];
            sp--;
            code++;
            break;
        case 5:
            stack[sp - 2] = stack[sp - 2] / stack[sp - 1];
            sp--;
            code++;
            break;
        case 9:
            stack[sp - 2] = (int)stack[sp - 2] % (int)stack[sp - 1];
            sp--;
            code++;
            break;
        case 6:
            code++;
            stack[sp - 2] = stack[sp - 2] > stack[sp - 1] ? stack[sp - 2] : stack[sp - 1];
            sp--;
            break;
        case 7:
            code++;
            stack[sp - 2] = stack[sp - 2] < stack[sp - 1] ? stack[sp - 2] : stack[sp - 1];
            sp--;
            break;
        case 8: {
            code++;
            float b = stack[sp - 1];
            stack[sp - 2] = pow(stack[sp - 2], b);
            sp--;
            break;
        }
        case 0x10:
            code++;
            stack[sp - 2] = floor(stack[sp - 2]);
            sp--;
            break;
        }
    }
    return stack[0];
}

// inferred: byte code with its size, and the table of its variables
struct ParseCodeRef_unk {
    int mSize;
    unsigned char* mCode;
};
struct ParseVars_unk {
    float** mVars;
};

// NONMATCHING: 92.8% - the original computes idx - 1 before scaling (subi; slwi; lwzx) instead
// of folding the -1 into the load offset
// inferred: resolves the variable references of byte code (1-based indices into vars)
extern "C" void fn_800A6E8C(ParseCodeRef_unk* ref, ParseVars_unk* vars) {
    if (ref->mSize == 0) {
        return;
    }
    unsigned char* p = ref->mCode;
    while (p < ref->mCode + ref->mSize) {
        while (*p != 0xFF) {
            switch (*p) {
            case 0:
                p += 5;
                break;
            case 1: {
                unsigned char* q = p + 1;
                unsigned long idx;
                memcpy(&idx, q, 4);
                if (idx != 0) {
                    if (idx == 0xFFFFFFFF) {
                        *(float**)q = &sArgument;
                    } else {
                        *(float**)q = vars->mVars[idx - 1];
                    }
                }
                p += 5;
                break;
            }
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 0x10:
                p++;
                break;
            }
        }
        p++;
    }
}

namespace atn {

ParseCode_unk::ParseCode_unk() : m00(0) {}

ParseCode_unk::~ParseCode_unk() {
    if (m00 != 0) {
        delete[] m00;
        m00 = 0;
    }
}

int ParseCode_unk::fn_800A6FF0(ParseTree* tree) {
    if (tree == 0 || tree->m04 == 0) {
        return 0;
    }
    if (m00 != 0) {
        delete[] m00;
        m00 = 0;
    }
    int size = tree->m14 + (tree->m14 + 1) * 5 + 1;
    if (size % 2 == 1) {
        size++;
    }
    unsigned char* code = m00 = new (8, "", "", 0) unsigned char[size];
    int pos = 0;
    fn_800A693C(code, tree->m04, &pos);
    code[pos++] = 0xFF;
    if (pos % 2 == 1) {
        pos++;
    }
    return 1;
}

} // namespace atn
