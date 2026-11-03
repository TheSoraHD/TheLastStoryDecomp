#include "game/GameVoice.h"
#include "game/CharaTask.h"
#include "atn/SoundCore.h"
#include "atn/SoundManager.h"
#include "atn/Memory.h"

extern "C" int sprintf(char* s, const char* format, ...);

extern "C" atn::SoundEngine_unk* lbl_8087EE90;
extern "C" unsigned int fn_80049B74(atn::SoundEngine_unk* engine, const char* name); // inferred: sound id of a name
extern "C" const char* fn_80049CDC(atn::SoundEngine_unk* engine, unsigned int id);   // inferred: name of a sound id
extern "C" int fn_800CB4EC(atn::SoundRef* sound);  // inferred: the sound is being prepared
extern "C" bool fn_800CB58C(atn::SoundRef* sound); // inferred: the sound is playing
extern "C" int fn_800CB6F8(atn::SoundRef* sound);  // inferred: sound id
extern "C" int fn_8006AA20(const char* path);       // inferred: the file exists

extern "C" void* lbl_8087EEB8; // inferred: debug message output
// inferred: debug message list (atn/FileUtil_unk.cpp)
struct DebugMessages_unk {
    void fn_800697D8(const char* message);
};

// inferred: the HUD (lbl_8087F490, game/GameUI.cpp); only its sound object at 0x2658
struct GameVoiceUI_unk {
    unsigned char m0000[0x2658];
    void* m2658;
};
extern "C" GameVoiceUI_unk* lbl_8087F490;

// inferred: sound id of a voice name (0 without a name; name invented)
static inline unsigned int VoiceId_unk(const char* name) {
    unsigned int id = name != 0 ? fn_80049B74(lbl_8087EE90, name) : 0;
    return id;
}

// inferred: sound for a voice, without the duplicate check of the sound manager (name invented)
static inline atn::SoundCore* NewVoice_unk(unsigned int id) {
    lbl_8087EFE8->m34C8 = 0;
    atn::SoundCore* sound = lbl_8087EFE8->fn_800D0AB0(id);
    lbl_8087EFE8->m34C8 = 1;
    return sound;
}

GameVoice* GameVoice::fn_803E836C(atn::Task* parent) {
    if (parent != 0 && lbl_8087F498 == 0) {
        lbl_8087F498 = new ((MemoryTag)10, "", "", 0) GameVoice(parent);
    }
    return lbl_8087F498;
}

GameVoiceChannel_unk::GameVoiceChannel_unk() {
    Stop_unk(0);
}

inline void GameVoiceChannel_unk::DetachLip_unk() {
    if (mChara != 0 && mLipSync.mCount != 0) {
        Chara1154_unk& lip = mChara->m1154;
        if (lip.m00 == &mLipSync) {
            lip.m00 = 0;
            lip.m04 = 0.0f;
        }
    }
}

inline void GameVoiceChannel_unk::Stop_unk(int frames) {
    fn_800CB5C8(&mSound, frames, 0);
    DetachLip_unk();
    mFile.fn_80473F88();
    mChara = 0;
    mPos = lbl_807C7030;
    mTime = 0;
    mPlaying = 0;
    mPrepared = 0;
}

GameVoice::GameVoice(atn::Task* parent) : atn::Task(parent), mNumChannels(1) {}

GameVoice::~GameVoice() {
    for (int i = 0; i < 4; i++) {
        GameVoiceChannel_unk* channel = &mChannels[i];
        fn_800CB5C8(&channel->mSound, 0, 0);
        channel->mSound.Reset();
    }
    lbl_8087F498 = 0;
}

int GameVoice::vf0C() {
    return fn_800D3FA4() != 0;
}

void* fn_803EAC20() {
    return lbl_8087F490 != 0 ? lbl_8087F490->m2658 : 0;
}

void GameVoice::fn_803EAF60(CharaTask* chara) {
    for (GameVoiceDelayed_unk* it = mDelayed.begin(); it != mDelayed.end();) {
        if (it->mChara == chara || chara == 0) {
            mDelayed.erase(it);
        } else {
            it++;
        }
    }
}

// NONMATCHING: 98.26% - the id goes through r3 instead of r4; "/database/lip/%s.lip" needs the
// "SoundWarning: cannot match" string of an unwritten function before it in the pool
int GameVoice::fn_803EB038(const char* name, int offset) {
    GameVoiceChannel_unk* channel = fn_803EB7E4(VoiceId_unk(name), 0);
    if (channel == 0 || channel->mPlaying || channel->mPrepared) {
        return 0;
    }
    channel->Stop_unk(0);
    atn::SoundCore* sound = NewVoice_unk(fn_80049B74(lbl_8087EE90, name));
    if (sound == 0) {
        sound = 0;
    } else {
        sound->mA8 = lbl_8087EFE8->fn_800CFDA0(2);
        sound->mAC = lbl_8087EFE8->fn_800CFDC8(8);
        sound->fn_800CA7D4(offset);
        sound->fn_800C93D4(name);
    }
    channel->mSound = (atn::SoundObject*)sound;
    if (channel->mSound.mPtr != 0) {
        channel->mPlaying = 1;
    }
    char path[0x100];
    sprintf(path, "/database/lip/%s.lip", name);
    if (fn_8006AA20(path)) {
        channel->mFile.vf0C(path);
    }
    channel->mPrepared = 1;
    return 1;
}

int GameVoice::fn_803EB234() {
    GameVoiceChannel_unk* channel = mChannels;
    for (int i = 0; i < 4; i++, channel++) {
        if (fn_800CB4EC(&channel->mSound) != 0 || channel->mFile.fn_80473F50() != 0) {
            return 1;
        }
    }
    return 0;
}

int GameVoice::fn_803EB2A4(const char* name) {
    unsigned int id = VoiceId_unk(name);
    GameVoiceChannel_unk* channel = mChannels;
    for (int i = 0; i < 4; i++, channel++) {
        if (channel->mPlaying && fn_800CB6F8(&channel->mSound) == id) {
            return 1;
        }
    }
    return 0;
}

int GameVoice::fn_803EB338() {
    for (int i = 0; i < 4; i++) {
        if (mChannels[i].mPlaying) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 93.70% - registers of the character and of its lip sync swapped (r3/r4)
int GameVoice::fn_803EB394(const char* name) {
    unsigned int id = VoiceId_unk(name);
    for (int i = 0; i < 4; i++) {
        GameVoiceChannel_unk* channel = &mChannels[i];
        if (channel->mPlaying && fn_800CB6F8(&channel->mSound) == id) {
            channel->Stop_unk(0);
        }
    }
    return 0;
}

// inferred: stops the prepared voices that lost their sound (name invented)
void GameVoice::StopInvalid_unk() {
    for (int i = 0; i < 4; i++) {
        GameVoiceChannel_unk& channel = mChannels[i];
        if (channel.mPlaying && channel.mSound.mPtr == 0) {
            if (lbl_8087EEB8 != 0) {
                char buf[0x100];
                atn::SoundEngine_unk* engine = lbl_8087EE90;
                const char* label = fn_80049CDC(engine, fn_800CB6F8(&channel.mSound));
                sprintf(buf, "SoundWarning: stopped invalid prepared voice [%d:%s]", fn_800CB6F8(&channel.mSound), label);
                ((DebugMessages_unk*)lbl_8087EEB8)->fn_800697D8(buf);
            }
            channel.Stop_unk(0);
        }
    }
}

// NONMATCHING: 97.78% - registers of the loop and of the character swapped; string offset (see fn_803EB038)
GameVoiceChannel_unk* GameVoice::fn_803EB7E4(unsigned int id, int steal) {
    StopInvalid_unk();
    GameVoiceChannel_unk* found = 0;
    if (id != 0) {
        GameVoiceChannel_unk* channel = mChannels;
        for (int i = 0; i < mNumChannels; i++, channel++) {
            if (id == fn_800CB6F8(&channel->mSound)) {
                found = channel;
                break;
            }
        }
    }
    if (found == 0) {
        GameVoiceChannel_unk* channel = mChannels;
        for (int i = 0; i < mNumChannels; i++, channel++) {
            if (!fn_800CB58C(&channel->mSound) && channel->mChara == 0) {
                found = channel;
                break;
            }
        }
    }
    if (found == 0) {
        GameVoiceChannel_unk* channel = mChannels;
        for (int i = 0; i < mNumChannels; i++, channel++) {
            if (!fn_800CB58C(&channel->mSound)) {
                found = channel;
                break;
            }
        }
    }
    if (found == 0 && steal != 0) {
        int best = -1;
        GameVoiceChannel_unk* channel = mChannels;
        for (int i = 0; i < mNumChannels; i++, channel++) {
            if (channel->mTime > best) {
                best = channel->mTime;
                found = channel;
            }
        }
    }
    return found;
}

int GameVoice::fn_803EBA54() {
    GameVoiceChannel_unk* channel = mChannels;
    for (int i = 0; i < mNumChannels; channel++, i++) {
        if (!fn_800CB58C(&channel->mSound)) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 99.46% - registers of the character and of its lip sync swapped (r3/r4)
void GameVoice::fn_803EBAC8(int frames, int keepPrepared) {
    for (int i = 0; i < 4; i++) {
        GameVoiceChannel_unk* channel = &mChannels[i];
        if (keepPrepared == 0 || !channel->mPlaying) {
            channel->Stop_unk(frames);
        }
    }
    mRequests.clear();
    mDelayed.clear();
}

void GameVoice::fn_803EBBCC(CharaTask* chara) {
    GameVoiceRequest_unk* request = mRequests.begin();
    for (unsigned int i = 0; i < mRequests.size(); request++, i++) {
        if (request->mChara == chara) {
            request->mChara = 0;
        }
    }
}

int GameVoice::fn_803EBC04(const char* name) {
    if (lbl_8087EE90 != 0) {
        return fn_80049B74(lbl_8087EE90, name) != 0;
    }
    return 0;
}
