// atn/ShaderManager_unk.cpp: manager of the model shaders (file name invented). It loads the 76
// shader files, builds a shader (atn/Shader_unk.cpp) for each material description and one
// level-of-detail list per shader. It has its own string pool and 0.0f, so it is a file of its own.
#include "atn/Shader_unk.h"
#include "atn/LH_Model.h"
#include "atn/Memory.h"

extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void* fn_8008937C(void* parent, const char* name); // inferred: debug menu group

// inferred: debug settings (lbl_8087EFA8); m4C is the root of the debug menu
struct DebugRoot_unk {
    unsigned char m00[0x4C];
    void* m4C;
};
extern "C" DebugRoot_unk* lbl_8087EFA8;

// inferred: data of a loaded shader file (LoadModel::m5C)
struct ShaderFile_unk {
    unsigned char m00[0x30];
    int m30;                     // inferred: number of material descriptions
    atn::ShaderDesc_unk** m34;
};

struct ShaderMaterial_unk;
ShaderMaterial_unk* fn_800C4A90(ShaderTex_unk* shader);

static const char* sShaderFiles[] = {
    "color.shader",
    "color_LS.shader",
    "default.shader",
    "default_Alpha.shader",
    "default_Alpha_LM.shader",
    "default_fadelm_LM.shader",
    "default_layer_MT.shader",
    "default_layer_MT_Alpha.shader",
    "default_layer_MT_Alpha_LM.shader",
    "default_LM.shader",
    "default_MT.shader",
    "default_MT_Alpha.shader",
    "default_MT_LM.shader",
    "default_MT_LM_RFX.shader",
    "emboss.shader",
    "emboss_Alpha.shader",
    "emboss_AMB_LS.shader",
    "emboss_LM.shader",
    "emboss_LS.shader",
    "emboss_MT.shader",
    "emboss_MT_LM.shader",
    "emboss_RF.shader",
    "emboss_Spec.shader",
    "emboss_Spec_Alpha.shader",
    "emboss_Spec_AMB_LS.shader",
    "emboss_Spec_FB.shader",
    "emboss_Spec_LM.shader",
    "emboss_Spec_LM_FB.shader",
    "emboss_Spec_LS.shader",
    "emboss_Spec_MT.shader",
    "emboss_Spec_RF.shader",
    "emboss_Spec_RFX.shader",
    "emboss_Spec_RF_LS.shader",
    "error_missing_material.shader",
    "error_missing_shader.shader",
    "error_missing_texture.shader",
    "error_unmatched_material.shader",
    "glass.shader",
    "glass_emboss.shader",
    "haze.shader",
    "lightshaft.shader",
    "moss.shader",
    "outline.shader",
    "outline_LS.shader",
    "reflect.shader",
    "reflect_RF.shader",
    "reflect_RFX.shader",
    "reflect_Spec.shader",
    "skin.shader",
    "skin_Spec.shader",
    "slime.shader",
    "slime_Spec_RF.shader",
    "smoke.shader",
    "soft_hair.shader",
    "soft_hair_Spec.shader",
    "specular.shader",
    "specular_Alpha.shader",
    "specular_Alpha_LM.shader",
    "specular_AMB.shader",
    "specular_AMB_LS.shader",
    "specular_AMB_RF.shader",
    "specular_LM_RF.shader",
    "specular_MT.shader",
    "specular_MT_LM.shader",
    "specular_RF.shader",
    "specular_Spec.shader",
    "specular_Spec_Alpha.shader",
    "specular_Spec_AMB_LS.shader",
    "specular_Spec_LM.shader",
    "specular_Spec_LS.shader",
    "specular_Spec_RF_LS.shader",
    "water.shader",
    "water_LM_RFX.shader",
    "water_RFX.shader",
    "water_uv.shader",
    "water_uv_RF.shader",
};

static const char* sErrorShaders[] = {
    "error_missing_material",
    "error_missing_shader",
    "error_missing_texture",
    "error_unmatched_material",
};

extern "C" ShaderManager_unk* lbl_8087EFE0;

static ShaderMaterial_unk* sErrorMaterials[4]; // inferred: materials of the error shaders

// inferred: loads the shader files and builds their shaders (name invented; removed by the linker
// once inlined)
void ShaderManager_unk::LoadShaders_unk() {
    for (unsigned int i = 0; i < sizeof(sShaderFiles) / sizeof(sShaderFiles[0]); i++) {
        const char* path = sShaderFiles[i];
        atn::LH_Model* model = new (6, "", "", 0) atn::LH_Model;
        model->fn_80473FCC(path);
        mModels.push_back(model);
        ShaderFile_unk* file = (ShaderFile_unk*)model->fn_80475DF8();
        for (int j = 0; j < file->m30; j++) {
            mShaders.push_back(new (6, "", "", 0) ShaderTex_unk(model, file->m34[j]));
        }
    }
}

// inferred: one LOD list per shader (name invented; removed by the linker once inlined)
void ShaderManager_unk::CreateLODs_unk() {
    void* group = fn_8008937C(lbl_8087EFA8->m4C, "ShaderLODs");
    for (ShaderTex_unk** it = mShaders.begin(); it != mShaders.end(); it++) {
        ShaderLOD_unk* lod = new (6, "", "", 0) ShaderLOD_unk;
        lod->fn_800C4ECC(*it, 0.0f);
        mLODs.push_back(lod);
        lod->fn_800C4F70(group);
    }
}

ShaderManager_unk::ShaderManager_unk() {
    LoadShaders_unk();
    CreateLODs_unk();
}

// NONMATCHING: 95.5% - the inlined std::vector<T*>::push_back loads the capacity before the size (known
// mismatch of include/std/vector.h) and the shader description is loaded before the new.
void fn_800C5644() {
    lbl_8087EFE0 = new (6, "", "", 0) ShaderManager_unk;
}

ShaderLOD_unk* ShaderManager_unk::fn_800C5F5C(const char* name) {
    unsigned int hash = fn_800DC6B4(name);
    for (ShaderLOD_unk** it = mLODs.begin(); it != mLODs.end(); it++) {
        if (hash == fn_800DC6B4((*it)->mLODs.begin()->mShader->mDesc->m010)) {
            return *it;
        }
    }
    return 0;
}

ShaderLOD_unk* ShaderManager_unk::fn_800C5FF0(unsigned int hash) const {
    for (ShaderLOD_unk* const* it = mLODs.mData; it != mLODs.mData + mLODs.mSize; it++) {
        if (hash == fn_800DC6B4((*it)->mLODs.mData[0].mShader->mDesc->m010)) {
            return *it;
        }
    }
    return 0;
}

// NONMATCHING: 98.7% - the loop registers of the inlined FindShader_unk (index and iterator) are swapped.
// inferred: material of an error shader (code -1 to -4), created on first use
ShaderMaterial_unk* ShaderManager_unk::fn_800C607C(int code) {
    int i = -1 - code;
    if (sErrorMaterials[i] == 0) {
        sErrorMaterials[i] = fn_800C4A90(FindShader_unk(sErrorShaders[i]));
    }
    return sErrorMaterials[i];
}
