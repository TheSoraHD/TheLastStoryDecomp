#ifndef ATN_LAYER3D_H
#define ATN_LAYER3D_H

namespace atn {

// inferred: render layer interface. Field meaning unknown; values are those set by the
// inlined constructors (see LayerPost3D::LayerPost3D at 0x800B66A4).
class IRenderLayer {
public:
    IRenderLayer(const char* name, int arg0C = -1, int arg10 = -1) : m04(1), mName(name), m0C(arg0C), m10(arg10) {}
    virtual ~IRenderLayer() {}
    virtual void vf0C(); // 0x800B320C
    virtual void vf10() = 0;
    virtual void vf14(void* arg) = 0;
    virtual void vf18(); // 0x800B3208
    virtual void vf1C() = 0;
    virtual void vf20() = 0;

    int m04;
    const char* mName; // 0x08
    int m0C;
    int m10;
};

// inferred from the vtable __vt__Q23atn7Layer3D
class Layer3D : public IRenderLayer {
public:
    Layer3D(const char* name, int arg0C = -1, int arg10 = -1) : IRenderLayer(name, arg0C, arg10), m14(1), m18(0) {}
    virtual ~Layer3D() {}
    virtual void vf10();
    virtual void vf14(void* arg);
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();

    int m14;
    int m18;
};

} // namespace atn

#endif
