#include "game/GameVoice.h"
#include "game/CharaTask.h"
#include "atn/SoundCore.h"
#include "atn/SoundManager.h"
#include "atn/Memory.h"
#include "game/GameUI.h"
#include "game/GameChoiceWindow.h"
#include "game/GameScene.h"
#include "game/PlayerManager.h"
#include "game/FriendManager.h"
#include "game/GameConfig_unk.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" int rand();
extern "C" PlayerManager* lbl_8087F8A0; // inferred: the party
extern "C" GameConfig_unk* lbl_8087F0A8;

extern "C" atn::SoundEngine_unk* lbl_8087EE90;
extern "C" unsigned int fn_80049B74(atn::SoundEngine_unk* engine, const char* name); // inferred: sound id of a name
extern "C" const char* fn_80049CDC(atn::SoundEngine_unk* engine, unsigned int id);   // inferred: name of a sound id
extern "C" int fn_800CB4EC(atn::SoundRef* sound);  // inferred: the sound is being prepared
extern "C" bool fn_800CB58C(atn::SoundRef* sound); // inferred: the sound is playing
extern "C" int fn_800CB6F8(atn::SoundRef* sound);  // inferred: sound id
extern "C" int fn_8006AA20(const char* path);       // inferred: the file exists
extern "C" void fn_800CB504(atn::SoundRef* sound);  // inferred: starts the prepared sound
extern "C" void fn_800CB6E4(atn::SoundRef* sound, const void* pos); // inferred: sets the position
extern "C" void fn_800CB5B4(atn::SoundRef* sound, float volume, int frames); // inferred: sets the volume
extern "C" void fn_800CB654(atn::SoundRef* sound, float a, float b, float c); // inferred: sets the effect sends
extern "C" int fn_80049C3C(atn::SoundEngine_unk* engine, const char* name);  // inferred: the sound has no effects

extern "C" void* lbl_8087EEB8; // inferred: debug message output
// inferred: debug message list (atn/FileUtil_unk.cpp)
struct DebugMessages_unk {
    void fn_800697D8(const char* message);
};

// inferred: one line of the voice table of the characters (CharaTask::m7C; db/VoiceTable, see
// atn/ptr_string_base_char_0.cpp)
struct VoiceEntry_unk {
    const char* fn_8021FD7C(int kind) const; // inferred: name of the voice
    int fn_8021FFB0(int kind) const;         // inferred: message of the voice
};

// inferred: the field (lbl_8087F430, game/GameTask.cpp); only the members used here
struct GameVoiceField_unk {
    unsigned char m0000[0x10D8];
    void* m10D8;       // inferred: the scene (route tables, messages)
    unsigned char m10DC[0x54E4 - 0x10DC];
    int m54E4;         // inferred: state of the field (8: no voices)
    unsigned char m54E8[0x5538 - 0x54E8];
    int m5538;         // inferred: nonzero mutes the voices
};
extern "C" GameVoiceField_unk* lbl_8087F430;
extern "C" int fn_803761CC(GameVoiceField_unk* field); // inferred: the voices are enabled
// inferred: the battle (lbl_8087F540); only the pause flag
struct GameVoiceBattle_unk {
    unsigned char m00[0x70];
    int m70;
};
extern "C" GameVoiceBattle_unk* lbl_8087F540;

// inferred: sound id of a voice name (0 without a name; name invented)
static inline unsigned int VoiceId_unk(const char* name) {
    return name != 0 ? fn_80049B74(lbl_8087EE90, name) : 0;
}

// inferred: the message window of the HUD is free or has `time` left at most (name invented)
static inline bool IsMessageFree_unk(float time) {
    if (lbl_8087F490 != 0 && lbl_8087F490->m2640 != 0 && lbl_8087F490->m2640->mState != 0 &&
        lbl_8087F490->m2640->m60 < time) {
        return false;
    }
    return true;
}

// inferred: voices 0x24, 0x26 and 0x28 (name invented)
static inline bool IsCall_unk(int kind) {
    return kind == 0x24 || kind == 0x26 || kind == 0x28;
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

// inferred: the character stops playing `data` (name invented)
static inline void StopLip_unk(Chara1154_unk* lip, LipSyncData_unk* data) {
    if (lip->m00 == data) {
        lip->m00 = 0;
        lip->m04 = 0.0f;
    }
}

inline void GameVoiceChannel_unk::DetachLip_unk() {
    if (mChara != 0 && mLipSync.mCount != 0) {
        StopLip_unk(&mChara->m1154, &mLipSync);
    }
}

inline void GameVoiceChannel_unk::Stop_unk(int frames, int keepChara) {
    fn_800CB5C8(&mSound, frames, 0);
    if (keepChara == 0) {
        DetachLip_unk();
        mFile.fn_80473F88();
        mChara = 0;
    }
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

// inferred: the lip sync of the character has ended (name invented)
static inline bool LipEnded_unk(Chara1154_unk& lip) {
    return lip.m00 != 0 ? lip.m04 >= lip.m00->mCount : true;
}

// NONMATCHING: 99.8% - in the loops the character and its lip sync go to r3/r4 swapped (see Stop_unk)
void GameVoice::vf10() {
    for (GameVoiceDelayed_unk* it = mDelayed.begin(); it != mDelayed.end();) {
        if (it->mTimer >= it->mDelay) {
            if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 0 && lbl_8087F540 != 0 && lbl_8087F540->m70 == 0) {
                it->mChara->fn_80179F98(it->mKind);
                fn_803EA09C(it->mChara, it->mName, it->m18, it->m1C, 0, it->mVolume);
            }
            mDelayed.erase(it);
        } else {
            it->mTimer++;
            it++;
        }
    }
    for (GameVoiceRequest_unk* request = mRequests.begin(); request != mRequests.end();) {
        switch (request->mState) {
        case 1:
            if (request->mChannel != 0 &&
                (fn_800CB4EC(&request->mChannel->mSound) != 0 || request->mChannel->mFile.fn_80473F50() != 0)) {
                break;
            }
            if (request->mChannel != 0) {
                if (request->mChara != 0) {
                    for (unsigned int i = 0; i < 4; i++) {
                        GameVoiceChannel_unk* channel = &mChannels[i];
                        if (channel->mSound.mPtr != 0 && channel != request->mChannel && channel->mChara == request->mChara) {
                            channel->Stop_unk(5);
                        }
                    }
                }
                fn_800CB504(&request->mChannel->mSound);
                if (request->mChannel->mSound.mPtr != 0) {
                    ((atn::SoundCore*)request->mChannel->mSound.mPtr)->fn_800CA144(0);
                }
                if (request->mChara != 0 && request->mChannel->mFile.fn_80470580() != 0) {
                    request->mChannel->mLipSync.fn_8007A530((const char*)request->mChannel->mFile.fn_80470580(),
                                                           request->mChannel->mFile.fn_8047059C());
                    request->mChannel->mFile.fn_80473F88();
                    Chara1154_unk& lip = request->mChara->m1154;
                    lip.m00 = &request->mChannel->mLipSync;
                    lip.m04 = 0.0f;
                }
            }
            request->mState = 3;
            break;
        case 2:
            if (request->mChannel != 0) {
                request->mChannel->mSound.Reset();
            }
            request->mState = 0;
            break;
        case 3: {
            if (request->mChara != 0 && request->mChannel != 0) {
                fn_800CB6E4(&request->mChannel->mSound, &request->mChara->mPos);
            }
            bool done = false;
            if (request->mLength > 0) {
                if (request->mLength <= request->mTimer) {
                    done = true;
                }
            } else if (request->mChannel != 0) {
                if (!fn_800CB58C(&request->mChannel->mSound)) {
                    if (request->mChara == 0 || LipEnded_unk(request->mChara->m1154)) {
                        done = true;
                    }
                }
            } else {
                done = true;
            }
            if (done) {
                request->mChannel->Stop_unk(0);
                request->mState = 0;
            } else {
                request->mTimer++;
                if (request->mChannel != 0) {
                    request->mChannel->mTime = request->mTimer;
                }
            }
            break;
        }
        }
        if (request->mState == 0) {
            mRequests.erase(request);
        } else {
            request++;
        }
    }
}

// inferred: stops the prepared voice of the channel if it is not `name` (inlined in fn_803E907C and
// the other players, removed by the linker; name invented)
void GameVoiceChannel_unk::CheckPrepared_unk(const char* name) {
    if (mPlaying) {
        unsigned int id = VoiceId_unk(name);
        if (id != fn_800CB6F8(&mSound)) {
            if (lbl_8087EEB8 != 0) {
                char buf[0x100];
                atn::SoundEngine_unk* engine = lbl_8087EE90;
                const char* label = fn_80049CDC(engine, fn_800CB6F8(&mSound));
                sprintf(buf, "SoundWarning: cannot match [%d:%s] with prepared voice [%d:%s]", id, name,
                        fn_800CB6F8(&mSound), label);
                ((DebugMessages_unk*)lbl_8087EEB8)->fn_800697D8(buf);
            }
            Stop_unk(0);
        }
    }
}

// NONMATCHING: 99.0% - the voice id goes through r3 instead of r4 (VoiceId_unk); register of the id
atn::SoundRef GameVoice::fn_803E907C(const char* name, const atn::Vector3* pos, int offset, int steal, float volume) {
    atn::SoundRef ref;
    if (mRequests.size() >= 32) {
        return ref;
    }
    GameVoiceChannel_unk* channel = fn_803EB7E4(VoiceId_unk(name), steal);
    if (channel == 0) {
        return ref;
    }
    channel->CheckPrepared_unk(name);
    if (!channel->mPlaying) {
        channel->Stop_unk(0);
        atn::SoundCore* sound = NewVoice_unk(fn_80049B74(lbl_8087EE90, name));
        if (sound == 0) {
            sound = 0;
        } else {
            sound->mA8 = lbl_8087EFE8->fn_800CFDA0(2);
            sound->mAC = lbl_8087EFE8->fn_800CFDC8(8);
            sound->fn_800CA7D4(offset);
            sound->fn_800C97F8(name, pos);
        }
        channel->mSound = (atn::SoundObject*)sound;
        if (channel->mSound.mPtr == 0) {
            return ref;
        }
    }
    channel->mPlaying = 0;
    channel->mPrepared = 0;
    fn_800CB5B4(&channel->mSound, volume, 0);
    fn_800CB654(&channel->mSound, 1.0f, 1.0f, 1.0f);
    if (fn_80049C3C(lbl_8087EE90, name) != 0) {
        fn_800CB654(&channel->mSound, 0.0f, 0.0f, 0.0f);
    }
    channel->mPos = *pos;
    GameVoiceRequest_unk request;
    request.mState = 1;
    request.mChannel = channel;
    mRequests.push_back(request);
    return channel->mSound;
}

// NONMATCHING: 95% - branch shape of the battle check (bne + b), registers of the party loop
void GameVoice::fn_803EA77C(CharaTask* chara, int kind, int delay, float volume, float time) {
    if (lbl_8087F430 != 0 && fn_803761CC(lbl_8087F430) == 0) {
        return;
    }
    if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 8) {
        return;
    }
    if (lbl_8087F540 != 0) {
        if (lbl_8087F540->m70 != 0) {
            return;
        }
    }
    if (lbl_8087F430 != 0 && lbl_8087F430->m5538 != 0) {
        return;
    }
    if (!IsMessageFree_unk(time)) {
        return;
    }
    if (mDelayed.size() >= 16) {
        return;
    }
    if (!fn_803EBA54()) {
        return;
    }
    if (chara == 0 && FriendManager::sInstance != 0) {
        int n = lbl_8087F8A0->mCount - 1;
        if (n > 0) {
            PlayerTask* first = lbl_8087F8A0->FindByIndex_unk(rand() % n + 1);
            PlayerTask* player = first;
            do {
                if (player->fn_80179EBC(kind) && ((VoiceEntry_unk*)player->m7C)->fn_8021FD7C(kind) != 0) {
                    chara = player;
                    break;
                }
                player = player->mNext;
                if (player == 0) {
                    player = (PlayerTask*)FriendManager::sInstance->fn_8036554C();
                }
            } while (player != first);
        }
    }
    if (chara == 0) {
        return;
    }
    if (chara->fn_80179EBC(kind) == 0) {
        return;
    }
    const char* name = ((VoiceEntry_unk*)chara->m7C)->fn_8021FD7C(kind);
    if (name == 0) {
        return;
    }
    if (lbl_8087F0A8->mVoiceLimitLevel >= 1) {
        volume = 0.0f;
    }
    bool call = IsCall_unk(kind);
    bool other = kind != 0x24 && kind != 0x26 && kind != 0x28;
    mDelayed.push_back(GameVoiceDelayed_unk(chara, kind, name, volume, time, other, call, delay));
}

// inferred: plays the message of the voice `kind` of `chara` in the HUD
void GameVoice::fn_803EAA7C(CharaTask* chara, int kind, float time) {
    atn::SoundRef sound;
    if (lbl_8087F430 != 0 && lbl_8087F430->m5538 != 0) {
        return;
    }
    if (!IsMessageFree_unk(time)) {
        return;
    }
    if (chara == 0) {
        return;
    }
    VoiceEntry_unk* voices = (VoiceEntry_unk*)chara->m7C;
    if (voices == 0 || voices->fn_8021FFB0(kind) <= 0) {
        return;
    }
    void* scene;
    if (lbl_8087F430 != 0) {
        scene = lbl_8087F430->m10D8;
    } else {
        scene = 0;
    }
    void* msg = scene != 0 ? ((GameScene*)scene)->fn_803CC6B4(voices->fn_8021FFB0(kind)) : 0;
    if (msg == 0) {
        return;
    }
    lbl_8087F490->fn_803E3BE8(msg, 0, time);
    lbl_8087F490->m2658 = chara;
}

CharaTask* GameVoice::fn_803EAC20() {
    return lbl_8087F490 != 0 ? lbl_8087F490->m2658 : 0;
}

// NONMATCHING: 99.3% - lbl_8087F490 is not reloaded for the store; r3/r4 of Stop_unk
void GameVoice::fn_803EAC3C(CharaTask* chara) {
    if (lbl_8087F490 != 0 && (lbl_8087F490->m2658 == chara || chara == 0)) {
        lbl_8087F490->m2658 = 0;
    }
    for (GameVoiceRequest_unk* request = mRequests.begin(); request != mRequests.end();) {
        if (request->mChara == chara) {
            request->mChannel->Stop_unk(0);
            mRequests.erase(request);
        } else {
            request++;
        }
    }
    fn_803EAF60(chara);
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

// NONMATCHING: 98.8% - r3/r4 of Stop_unk in the loops
void GameVoice::fn_803EB4A8(int keepChara, int keepRequests) {
    for (int i = 0; i < 4; i++) {
        GameVoiceChannel_unk* channel = &mChannels[i];
        if (channel->mPlaying || channel->mPrepared) {
            channel->Stop_unk(0);
        }
    }
    mDelayed.clear();
    if (keepRequests == 0) {
        for (unsigned int i = 0; i < mRequests.size(); i++) {
            GameVoiceChannel_unk* channel = mRequests[i].mChannel;
            if (channel != 0) {
                channel->Stop_unk(10, keepChara);
            }
        }
        mRequests.clear();
    }
}

// NONMATCHING: 99.1% - registers of id, found and the loop pointer
GameVoiceChannel_unk* GameVoice::fn_803EB668(CharaTask* chara, const char* name, int steal) {
    unsigned int id = VoiceId_unk(name);
    GameVoiceChannel_unk* found = 0;
    if (id != 0) {
        GameVoiceChannel_unk* channel = mChannels;
        for (int i = 0; i < mNumChannels; channel++, i++) {
            if (id == fn_800CB6F8(&channel->mSound)) {
                found = channel;
                break;
            }
        }
    }
    if (found == 0 && chara != 0) {
        GameVoiceChannel_unk* channel = mChannels;
        for (int i = 0; i < mNumChannels; channel++, i++) {
            if (channel->mChara == chara) {
                channel->Stop_unk(5);
                found = channel;
                break;
            }
        }
    }
    if (found != 0) {
        return found;
    }
    return fn_803EB7E4(id, steal);
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
