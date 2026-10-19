#ifndef ATN_LAYER3D_H
#define ATN_LAYER3D_H

// inferred: registers a debug toggle `name` bound to `flag` (class unknown)
extern "C" void fn_80087994(void* menu, const char* name, int* flag, int arg3, int arg4);

namespace atn {

// inferred: node of the list of render objects passed to IRenderLayer::vf14 (names invented)
struct RenderObj_unk {
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10(void* arg); // draw
    virtual void vf14();
    virtual void vf18(void* arg); // pre-render
};
struct RenderNode_unk {
    RenderObj_unk* mObj;   // 0x00
    float m04;             // 0x04: inferred: sort depth
    RenderNode_unk* mNext; // 0x08
};

// inferred: render layer interface. Field meaning unknown; values are those set by the
// inlined constructors (see LayerPost3D::LayerPost3D at 0x800B66A4).
class IRenderLayer {
public:
    IRenderLayer(const char* name, int arg0C = -1, int arg10 = -1) : m04(1), mName(name), m0C(arg0C), m10(arg10) {}
    virtual ~IRenderLayer() {}
    virtual void vf0C(void* menu) { fn_80087994(menu, mName, &m04, 0, 0); } // 0x800B320C (weak)
    virtual void vf10() = 0;
    virtual void vf14(void* arg) = 0;
    virtual void vf18() {} // 0x800B3208 (weak)
    virtual void vf1C() = 0;
    virtual void vf20() = 0;

    int m04;
    const char* mName; // 0x08
    int m0C;
    int m10;
};

// inferred from the vtable __vt__Q23atn7Layer2D (0x80779048)
class Layer2D : public IRenderLayer {
public:
    Layer2D(const char* name, int arg0C = -1, int arg10 = -1) : IRenderLayer(name, arg0C, arg10) {}
    virtual ~Layer2D() {}
    virtual void vf10() {} // 0x800B3228 (weak)
    virtual void vf14(void* arg); // 0x800B3464
    virtual void vf1C();          // 0x800B34C0
    virtual void vf20();          // 0x800B35F4
};

// inferred from the vtable __vt__Q23atn7Layer3D
class Layer3D : public IRenderLayer {
public:
    Layer3D(const char* name, int arg0C = -1, int arg10 = -1) : IRenderLayer(name, arg0C, arg10), m14(1), m18(0) {}
    virtual ~Layer3D() {}
    virtual void vf10() {} // 0x800B3870 (weak)
    virtual void vf14(void* arg);
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();

    int m14;
    unsigned int m18;            // inferred: number of deferred nodes in m1C
    RenderNode_unk* m1C[16];     // inferred: deferred nodes (drawn by vf18)
};

} // namespace atn

#endif
