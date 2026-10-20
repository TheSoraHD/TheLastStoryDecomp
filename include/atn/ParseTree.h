#ifndef ATN_PARSETREE_H
#define ATN_PARSETREE_H

// Expression parser (atn/ParseTree.cpp, 0x800A5D14-0x800A7314). Only what db/GrowthParseGlobal
// uses is declared; member names are invented.

namespace atn {

// inferred: nodes allocated by ParseTree (vtables 0x80778B88, 0x80778B60, base atn::BaseNode)
class BaseNode {
public:
    virtual ~BaseNode();
};

class ValueNode : public BaseNode {
public:
    float m04;
};

class VariableNode : public BaseNode {
public:
    float* m04; // inferred: the variable being read
};

class ParseTree {
public:
    ParseTree();          // 0x800A5D14
    virtual ~ParseTree(); // 0x800A5D6C
    // inferred: creates the node of a name found in the expression (0x800A67D4)
    virtual BaseNode* vf0C(const char* name);

    void fn_800A5E00(const char* expr); // inferred: parses an expression
    ValueNode* fn_800A686C();           // inferred: allocates a ValueNode
    VariableNode* fn_800A68D4();        // inferred: allocates a VariableNode

    int m04;
    unsigned char* m08;
    unsigned char m0C[8];
    int m14;
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
