#ifndef ATN_PARSETREE_H
#define ATN_PARSETREE_H

// Expression parser (atn/ParseTree.cpp, 0x800A5B40-0x800A73A4). Member names are invented.

extern "C" double pow(double x, double y);
extern "C" double floor(double x);
extern "C" void* memset(void* dst, int c, unsigned long n);

namespace atn {

// inferred: node of an expression tree (vtable slots: dtor, value, kind)
class BaseNode {
public:
    virtual ~BaseNode() {}
    virtual float vf0C() = 0; // inferred: evaluates the node
    virtual int vf10() = 0;   // inferred: kind (1 value, 2 variable, 3 operator)
};

class ValueNode : public BaseNode {
public:
    ValueNode() : m04(0.0f) {}
    virtual ~ValueNode() {}
    virtual float vf0C() { return m04; }
    virtual int vf10() { return 1; }

    float m04;
};

class VariableNode : public BaseNode {
public:
    VariableNode() : m04(0) {}
    virtual ~VariableNode() {}
    virtual float vf0C() {
        if (m04 != 0) {
            return *m04;
        }
        return 0.0f;
    }
    virtual int vf10() { return 2; }

    float* m04; // inferred: the variable being read; (float*)-1 = the global argument
};

// inferred: binary operator; m04 is '+', '-', '*', '/', or a function: 'x' MAX, 'n' MIN,
// 'p' POW, 'm' MOD, 'i' INT
class OperatorNode : public BaseNode {
public:
    OperatorNode() : m04(0), m08(0), m0C(0) {}
    virtual ~OperatorNode() {}
    virtual float vf0C() {
        float a = m08 != 0 ? m08->vf0C() : 0.0f;
        float b = m0C != 0 ? m0C->vf0C() : 0.0f;
        switch (m04) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        case 'm':
            return (int)a % (int)b;
        case 'x':
            return a > b ? a : b;
        case 'n':
            return a < b ? a : b;
        case 'p':
            return pow(a, b);
        case 'i':
            return floor(a);
        }
        return 0.0f;
    }
    virtual int vf10() { return 3; }

    char m04;
    BaseNode* m08;
    BaseNode* m0C;
};

class ParseTree {
public:
    ParseTree();          // 0x800A5D14
    virtual ~ParseTree(); // 0x800A5D6C
    // inferred: creates the node of a name found in the expression (0x800A67D4)
    virtual BaseNode* vf0C(const char* name);

    int fn_800A5E00(const char* expr); // inferred: parses an expression
    int fn_800A5F18(const char* expr, int* nodes, int* ops); // inferred: validates and counts
    BaseNode* fn_800A6198(const char* expr, int func);       // inferred: builds the tree
    ValueNode* fn_800A686C();           // inferred: allocates a ValueNode
    VariableNode* fn_800A68D4();        // inferred: allocates a VariableNode

    void Clear_unk() {
        if (m08 != 0) {
            delete[] m08;
            m08 = 0;
        }
        memset(m0C, 0, sizeof(m0C));
        m14 = 0;
        m04 = 0;
    }

    BaseNode* m04;          // inferred: root
    unsigned char* m08;     // inferred: node pool (0x18 bytes per operator)
    unsigned char m0C[8];   // inferred: storage of the first leaf node
    int m14;                // inferred: number of operators
};

// inferred: compiled form of a ParseTree (an array allocated with new[]). Name invented.
class ParseCode_unk {
public:
    ParseCode_unk();                     // 0x800A6F7C
    ~ParseCode_unk();                    // 0x800A6F88
    int fn_800A6FF0(ParseTree* tree);    // inferred: compiles the tree, 0 if it is empty

    unsigned char* m00;
};

} // namespace atn

#endif
