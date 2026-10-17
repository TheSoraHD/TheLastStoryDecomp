#ifndef ATN_LAYER3D_H
#define ATN_LAYER3D_H

namespace atn {

// inferred: render layer interface. Field meaning unknown; values are those set by the
// inlined constructors (see LayerPost3D::LayerPost3D at 0x800B66A4).
class IRenderLayer {
public:
    IRenderLayer(const char* name)
        : m04(1), mName(name), m0C(-1), m10(-1), m14(1), m18(0) {}
    virtual ~IRenderLayer() {}
    virtual void vf0C() = 0;
    virtual void vf10() = 0;
    virtual void vf14() = 0;
    virtual void vf18() = 0;
    virtual void vf1C() = 0;
    virtual void vf20() = 0;

    int m04;
    const char* mName; // 0x08
    int m0C;
    int m10;
    int m14;
    int m18;
};

// inferred from the vtable at 0x807790? (__vt__Q23atn7Layer3D)
class Layer3D : public IRenderLayer {
public:
    Layer3D(const char* name) : IRenderLayer(name) {}
    virtual ~Layer3D() {}
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
};

} // namespace atn

#endif
