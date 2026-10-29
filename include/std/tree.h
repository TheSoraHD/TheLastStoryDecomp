#ifndef STD_TREE_H
#define STD_TREE_H
// Partial reconstruction of the MSL C++ red-black tree (<tree>), the base of std::map/std::set.
// Real names from LastWorld_tools.rso: std::__tree_node_base<std::allocator<void*>> and
// std::__red_black_tree<__tree_node_base<...>*>::balance_insert (0x8003E120, weak copy in
// AI/AI_archer_attack01, called from game/TownTask, atn/AudioDevice, atn/ptr_string_base_char_0).
// Member names other than balance_insert are inferred.
#include <memory>

namespace std {

template <class Alloc>
struct __tree_node_base {
    __tree_node_base* left_;
    __tree_node_base* right_;
    unsigned long parent_; // inferred: parent pointer, bit 0 = red

    __tree_node_base* parent() const { return (__tree_node_base*)(parent_ & ~1); }
    bool is_red() const { return (parent_ & 1) == 1; }
    void set_red() { parent_ |= 1; }
    void set_black() { parent_ &= ~1; }
    void set_parent(__tree_node_base* p) { parent_ = (unsigned long)p | (parent_ & 1); }
};

typedef __tree_node_base<allocator<void*> > __tree_node_base_void; // inferred: shorthand

template <class NodePtr>
struct __red_black_tree {
    static void rotate_left(NodePtr x, NodePtr& root);
    static void rotate_right(NodePtr x, NodePtr& root);
    // rebalance after an insert (defined in AI/AI_archer_attack01.cpp, where the weak copy is)
    static void balance_insert(NodePtr x, NodePtr root);
};

template <class NodePtr>
inline void __red_black_tree<NodePtr>::rotate_left(NodePtr x, NodePtr& root) {
    NodePtr y = x->right_;
    if (root == x) {
        root = y;
    }
    x->right_ = y->left_;
    if (y->left_ != 0) {
        y->left_->set_parent(x);
    }
    y->set_parent(x->parent());
    if (x == x->parent()->left_) {
        x->parent()->left_ = y;
    } else {
        x->parent()->right_ = y;
    }
    y->left_ = x;
    x->set_parent(y);
}

template <class NodePtr>
inline void __red_black_tree<NodePtr>::rotate_right(NodePtr x, NodePtr& root) {
    NodePtr y = x->left_;
    if (root == x) {
        root = y;
    }
    x->left_ = y->right_;
    if (y->right_ != 0) {
        y->right_->set_parent(x);
    }
    y->set_parent(x->parent());
    if (x == x->parent()->left_) {
        x->parent()->left_ = y;
    } else {
        x->parent()->right_ = y;
    }
    y->right_ = x;
    x->set_parent(y);
}

typedef __red_black_tree<__tree_node_base_void*> __red_black_tree_void; // inferred: shorthand

} // namespace std

#endif
