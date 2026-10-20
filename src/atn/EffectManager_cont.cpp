// Tail of the real EffectManager.cpp (0x80239798-0x80243758 with the std:: templates of
// std/char_traits_char_8023C080): the this-adjusting thunks of atn::EffectManager for its IRender
// base at offset 0x48 (@72@vf10__Q23atn13EffectManagerFP10Camera_unk and
// @72@__dt__Q23atn13EffectManagerFv, emitted by MWCC after the last function of the file).
// The unit cannot be fused with atn/EffectManager (the std unit in between is not ours), so the
// thunks are written here as plain tail calls with their current symbol names.

struct Camera_unk;

namespace atn {
// inferred: only the two targets are needed here (see src/atn/EffectManager.cpp)
class EffectManager {
public:
    void vf10(Camera_unk* camera);
};
} // namespace atn

extern "C" void __dt__Q23atn13EffectManagerFv(void* self, int del);

extern "C" void fn_8024372C(char* self, Camera_unk* camera) {
    ((atn::EffectManager*)(self - 0x48))->vf10(camera);
}

extern "C" void fn_80243734(char* self, int del) {
    __dt__Q23atn13EffectManagerFv(self - 0x48, del);
}
