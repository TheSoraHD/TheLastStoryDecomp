#ifndef ATN_IRENDER_H
#define ATN_IRENDER_H


namespace atn {

// inferred: render system passed to the draw virtuals (defined in atn/RenderView_unk.h)
struct RenderView_unk;

// inferred: renderer interface (RTTI __RTTI__Q23atn7IRender; base of atn::ModelBase and Event::DialogHead).
// Other files still keep local copies (atn/LensFlare.h, atn/PostFx.h, atn/VideoTHP.cpp, ...). The first
// virtual is named GetRenderType_unk instead of vf0C: Event::DialogHead overrides it apart from
// atn::Task::vf0C, which has the same signature (the base symbol 0x80046218 keeps the name
// vf0C__Q23atn7IRenderFv; no file that includes this header uses it).
class IRender {
public:
    virtual ~IRender() {}
    virtual int GetRenderType_unk();                          // 0x80046218
    virtual void vf10(RenderView_unk* view) = 0;              // inferred: draw
    virtual int vf14(void* arg1, void* arg2, void* frustum); // 0x80046200
    virtual void vf18(RenderView_unk* view);                  // 0x80046214: inferred: second draw pass
    virtual void vf1C();                                      // 0x800461FC
};

} // namespace atn

#endif
