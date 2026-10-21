// inferred file name (splits-auto-C.md, group 2): window of the system error messages (disc, Wii Remote,
// Nunchuk) shown over the game, with a direct GX text fallback while its layouts are not loaded.
// No vtable or RTTI; class name ErrorManager from the boost::bind type names (game/main.cpp). Instance at lbl_8087F420 (see game/GameSystem_unk.cpp).

#include "atn/D2A_Task.h"
#include "atn/FadeCommon.h"
#include "atn/GXDecl.h"
#include "atn/Memory.h"
#include "atn/SequenceControl.h"
#include "atn/SoundRef.h"
#include <boost/scoped_ptr.hpp>

extern "C" unsigned long strlen(const char* s);
extern "C" int sprintf(char* buf, const char* format, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" int fn_8006BA30();                       // inferred: language (0 Japanese)
// inferred: text manager (lbl_8087EEC8)
struct SysTextMgr_unk;
extern "C" SysTextMgr_unk* lbl_8087EEC8;
extern "C" int fn_8006F2F0(SysTextMgr_unk* mgr, wchar_t* dst, const char* src, unsigned int size); // inferred: SJIS to wide
extern "C" void DVDSetAutoFatalMessaging(int enable);
extern "C" int DVDGetDriveStatus();
extern "C" atn::Task* lbl_8087F530; // inferred: root task
extern "C" void fn_801FED24(void* layout, unsigned int id, int index, float value);
extern "C" void fn_801FECE0(void* layout, unsigned int id, float value);
extern "C" void fn_801FEE08(void* layout, unsigned int id, const wchar_t* text);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
// inferred: view of HomeButtonMenu (game/HomeButtonMenu.cpp), only the members used here
class HomeButtonMenu {
public:
    unsigned char m00[0x84];
    int m84; // inferred: state (1 open)

    static HomeButtonMenu* sInstance;
};
// inferred: input manager (lbl_8087F018)
struct SysInput_unk {
    int GetExt_unk() { return m5C; } // inferred: name invented
    unsigned char m0000[0x5C];
    unsigned char m5C; // inferred: extension type of the first Wii Remote
    unsigned char m005D[0x40C0 - 0x5D];
    int m40C0; // inferred: the first Wii Remote is connected
};
extern "C" SysInput_unk* lbl_8087F018;
extern "C" int fn_805FF120(); // inferred: the error is cleared
// inferred: arena (lbl_8087F610)
struct SysArena_unk;
extern "C" SysArena_unk* lbl_8087F610;
extern "C" void fn_804FB530(SysArena_unk* arena);
struct SysArenaState_unk {
    unsigned char m00[0xC8];
    int mC8;
};
extern "C" SysArenaState_unk* lbl_8087F628;
// inferred: sound, bgm, movie and field pauses
extern "C" void* lbl_8087EFE8;
extern "C" void fn_800CF7DC(void* sound, int pause, int frames, int arg);
extern "C" void* lbl_8087EE90;
extern "C" void fn_8004A2CC(void* bgm, int pause, int frames, int arg);
struct SysMovie_unk {
    unsigned char m00[0x08];
    int m08; // inferred: state (4 playing, 5 paused)
};
extern "C" SysMovie_unk* lbl_8087F010;
struct SysField_unk;
extern "C" SysField_unk* lbl_8087F430;
extern "C" void fn_8037177C(SysField_unk* field);
extern "C" void* lbl_8087EF68; // inferred: save manager
extern "C" int fn_800A4228(void* obj, int* out); // inferred: nonzero while saving
struct SysSave_unk {
    unsigned char m000[0x174];
    int m174;
};
// inferred: debug drawing and graphics device
extern "C" void* lbl_8087EEB0;
extern "C" void fn_80061824(void* draw, float x, float y, float z, float w, float h, float f6, float f7, float f8,
                            const wchar_t* text, unsigned int color, int a, int b, int c, int d,
                            unsigned int color2); // inferred: text
struct SysGraphics_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
    unsigned char m44[0x48 - 0x44];
    unsigned int mClearColor; // 0x48
};
extern "C" SysGraphics_unk* lbl_8087EEE0;
extern "C" void fn_8007192C(SysGraphics_unk* gfx); // inferred: begins the frame
extern "C" void fn_80075DEC(SysGraphics_unk* gfx);
extern "C" void fn_80071AFC(SysGraphics_unk* gfx);
extern "C" void fn_80071D60(SysGraphics_unk* gfx, int arg);
// GX and OS
typedef float SysMtx[3][4];
typedef float SysMtx44[4][4];
extern "C" unsigned short OSGetFontEncode();
extern "C" int OSInitFont(void* font);
extern "C" const char* OSGetFontTexture(const char* string, void** image, int* x, int* y, int* width);
extern "C" void C_MTXOrtho(SysMtx44 m, float t, float b, float l, float r, float n, float f);
extern "C" void GXSetProjection(SysMtx44 m, int type);
extern "C" void PSMTXIdentity(SysMtx m);
extern "C" void PSMTXScale(SysMtx m, float x, float y, float z);
extern "C" void GXLoadPosMtxImm(SysMtx m, int id);
extern "C" void GXSetCurrentMtx(int id);
extern "C" void GXClearVtxDesc();
extern "C" void GXSetVtxDesc(int attr, int type);
extern "C" void GXSetVtxAttrFmt(int fmt, int attr, int cnt, int type, int frac);
extern "C" void GXLoadTexObj(GXTexObj* obj, int id);
extern "C" void GXBegin(int type, int fmt, unsigned short n);
// inferred: font header of the OS (OSFontHeader)
struct SysFont_unk {
    unsigned char m00[0x10];
    unsigned short m10; // inferred: cell width
    unsigned short m12; // inferred: cell height
    unsigned char m14[0x18 - 0x14];
    unsigned short m18; // inferred: sheet format
    unsigned char m1A[0x1E - 0x1A];
    unsigned short m1E; // inferred: sheet width
    unsigned short m20; // inferred: sheet height
};
// inferred: write gather pipe of the GX FIFO (as in atn/LensFlare.cpp)
union SysWGPipe_unk {
    unsigned char u8;
    short s16;
    unsigned short u16;
};
#define GXWGFifo (*(volatile SysWGPipe_unk*)0xCC008000)
// inferred: GX vertex helpers of the SDK (inline)
inline void GXPosition3s16_unk(short x, short y, short z) {
    GXWGFifo.s16 = x;
    GXWGFifo.s16 = y;
    GXWGFifo.s16 = z;
}
inline void GXTexCoord2s16_unk(short s, short t) {
    GXWGFifo.s16 = s;
    GXWGFifo.s16 = t;
}
// inferred: messages per language (6) and error (31), four lines each (generated from the data of the DOL;
// the literals are in .rodata in this order, the empty one in .sdata2)
static const char* const sMessages[6][31][4] = {
    {
        {"", "", "", ""},
        {"", "", "", ""},
        {"\x83\x89\x83X\x83g\x83X\x83g\x81[\x83\x8A\x81[\x82\xCC\x83""f\x83""B\x83X\x83N\x82\xF0", "\x83Z\x83""b\x83g\x82\xB5\x82\xC4\x82\xAD\x82\xBE\x82\xB3\x82\xA2\x81""B", "", ""},
        {"\x83\x89\x83X\x83g\x83X\x83g\x81[\x83\x8A\x81[\x82\xCC\x83""f\x83""B\x83X\x83N\x82\xF0", "\x83Z\x83""b\x83g\x82\xB5\x82\xC4\x82\xAD\x82\xBE\x82\xB3\x82\xA2\x81""B", "", ""},
        {"\x83""f\x83""B\x83X\x83N\x82\xF0\x93\xC7\x82\xDF\x82\xDC\x82\xB9\x82\xF1\x82\xC5\x82\xB5\x82\xBD\x81""B", "\x82\xAD\x82\xED\x82\xB5\x82\xAD\x82\xCDWii\x96{\x91\xCC\x82\xCC\x8E\xE6\x88\xB5\x90\xE0\x96\xBE\x8F\x91\x82\xF0", "\x82\xA8\x93\xC7\x82\xDD\x82\xAD\x82\xBE\x82\xB3\x82\xA2\x81""B", ""},
        {"Wii\x83\x8A\x83\x82\x83R\x83\x93\x82\xCC\x92\xCA\x90M\x82\xAA\x90\xD8\x82\xEA\x82\xDC\x82\xB5\x82\xBD\x81""B", "", "", ""},
        {"Wii\x83\x8A\x83\x82\x83R\x83\x93\x82\xC9\x83k\x83\x93\x83`\x83\x83\x83N\x82\xDC\x82\xBD\x82\xCD", "\x83N\x83\x89\x83V\x83""b\x83N\x83R\x83\x93\x83g\x83\x8D\x81[\x83\x89\x82\xF0", "\x90\xDA\x91\xB1\x82\xB5\x82\xC4\x82\xAD\x82\xBE\x82\xB3\x82\xA2\x81""B", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Wii\x96{\x91\xCC\x95\xDB\x91\xB6\x83\x81\x83\x82\x83\x8A\x82\xCC\x8F\x91\x82\xAB\x8D\x9E\x82\xDD/\x93\xC7\x82\xDD\x8Fo\x82\xB5\x82\xAA\x82\xC5\x82\xAB\x82\xDC\x82\xB9\x82\xF1\x82\xC5\x82\xB5\x82\xBD\x81""B", "\x82\xAD\x82\xED\x82\xB5\x82\xAD\x82\xCDWii\x96{\x91\xCC\x82\xCC\x8E\xE6\x88\xB5\x90\xE0\x96\xBE\x8F\x91\x82\xF0\x82\xA8\x93\xC7\x82\xDD\x82\xAD\x82\xBE\x82\xB3\x82\xA2\x81""B", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"\x83""f\x81[\x83^\x82\xAA\x89\xF3\x82\xEA\x82\xC4\x82\xA2\x82\xE9\x82\xBD\x82\xDF\x81""A\x83Z\x81[\x83u\x83""f\x81[\x83^\x82\xF0\x8Eg\x97p\x82\xC5\x82\xAB\x82\xDC\x82\xB9\x82\xF1\x81""B", "\x83Z\x81[\x83u\x83""f\x81[\x83^\x82\xF0\x8D\xEC\x82\xE8\x92\xBC\x82\xB5\x82\xDC\x82\xB7\x82\xA9\x81H", "$Ba\x82\xCD\x82\xA2", "$Bmb\x82\xA2\x82\xA2\x82\xA6"},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"\x83""f\x81[\x83^\x82\xAA\x89\xF3\x82\xEA\x82\xC4\x82\xA2\x82\xE9\x82\xBD\x82\xDF\x81""A\x83Z\x81[\x83u\x83""f\x81[\x83^\x82\xF0\x8Eg\x97p\x82\xC5\x82\xAB\x82\xDC\x82\xB9\x82\xF1\x81""B", "\x83Z\x81[\x83u\x83""f\x81[\x83^\x82\xF0\x8D\xEC\x82\xE8\x92\xBC\x82\xB5\x82\xDC\x82\xB7\x82\xA9\x81H", "$Ba\x82\xCD\x82\xA2", "$Bmb\x82\xA2\x82\xA2\x82\xA6"},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Wii\x96{\x91\xCC\x95\xDB\x91\xB6\x83\x81\x83\x82\x83\x8A\x82\xCC\x8B\xF3\x82\xAB\x97""e\x97\xCA\x82\xAA\x95s\x91\xAB\x82\xB5\x82\xC4\x82\xA2\x82\xDC\x82\xB7\x81""B", "Wii\x83\x81\x83j\x83\x85\x81[\x82\xCC\x83""f\x81[\x83^\x8A\xC7\x97\x9D\x89\xE6\x96\xCA\x82\xC5\x83""f\x81[\x83^\x82\xF0SD\x83J\x81[\x83h\x82\xC9\x88\xDA\x93\xAE\x82\xB7\x82\xE9\x82\xA9", "\x83""f\x81[\x83^\x82\xF0\x8F\xC1\x8B\x8E\x82\xB5\x82\xC4""11\x83u\x83\x8D\x83""b\x83N\x82\xCC\x8B\xF3\x82\xAB\x83u\x83\x8D\x83""b\x83N\x82\xF0\x97p\x88\xD3\x82\xB5\x82\xC4\x82\xAD\x82\xBE\x82\xB3\x82\xA2\x81""B", ""},
        {"Wii\x96{\x91\xCC\x95\xDB\x91\xB6\x83\x81\x83\x82\x83\x8A\x82\xCC\x8B\xF3\x82\xAB\x83t\x83@\x83""C\x83\x8B\x90\x94\x82\xAA\x95s\x91\xAB\x82\xB5\x82\xC4\x82\xA2\x82\xDC\x82\xB7\x81""B", "Wii\x83\x81\x83j\x83\x85\x81[\x82\xCC\x83""f\x81[\x83^\x8A\xC7\x97\x9D\x89\xE6\x96\xCA\x82\xC5\x83""f\x81[\x83^\x82\xF0SD\x83J\x81[\x83h\x82\xC9\x88\xDA\x93\xAE\x82\xB7\x82\xE9\x82\xA9", "\x83""f\x81[\x83^\x82\xF0\x8F\xC1\x8B\x8E\x82\xB5\x82\xC4\x82\xAD\x82\xBE\x82\xB3\x82\xA2\x81""B", ""},
        {"Wii\x96{\x91\xCC\x95\xDB\x91\xB6\x83\x81\x83\x82\x83\x8A\x82\xCC\x8B\xF3\x82\xAB\x97""e\x97\xCA\x82\xAA\x95s\x91\xAB\x82\xB5\x82\xC4\x82\xA2\x82\xDC\x82\xB7\x81""B", "Wii\x83\x81\x83j\x83\x85\x81[\x82\xCC\x83""f\x81[\x83^\x8A\xC7\x97\x9D\x89\xE6\x96\xCA\x82\xC5\x83""f\x81[\x83^\x82\xF0SD\x83J\x81[\x83h\x82\xC9\x88\xDA\x93\xAE\x82\xB7\x82\xE9\x82\xA9", "\x83""f\x81[\x83^\x82\xF0\x8F\xC1\x8B\x8E\x82\xB5\x82\xC4""11\x83u\x83\x8D\x83""b\x83N\x82\xCC\x8B\xF3\x82\xAB\x83u\x83\x8D\x83""b\x83N\x82\xF0\x97p\x88\xD3\x82\xB5\x82\xC4\x82\xAD\x82\xBE\x82\xB3\x82\xA2\x81""B", ""},
        {"Wii\x96{\x91\xCC\x95\xDB\x91\xB6\x83\x81\x83\x82\x83\x8A\x82\xCC\x8B\xF3\x82\xAB\x83t\x83@\x83""C\x83\x8B\x90\x94\x82\xAA\x95s\x91\xAB\x82\xB5\x82\xC4\x82\xA2\x82\xDC\x82\xB7\x81""B", "Wii\x83\x81\x83j\x83\x85\x81[\x82\xCC\x83""f\x81[\x83^\x8A\xC7\x97\x9D\x89\xE6\x96\xCA\x82\xC5\x83""f\x81[\x83^\x82\xF0SD\x83J\x81[\x83h\x82\xC9\x88\xDA\x93\xAE\x82\xB7\x82\xE9\x82\xA9", "\x83""f\x81[\x83^\x82\xF0\x8F\xC1\x8B\x8E\x82\xB5\x82\xC4\x82\xAD\x82\xBE\x82\xB3\x82\xA2\x81""B", ""},
        {"", "", "", ""},
        {"Wii\x83\x81\x83j\x83\x85\x81[\x82\xCC\x83""f\x81[\x83^\x8A\xC7\x97\x9D\x89\xE6\x96\xCA\x82\xF0\x8AJ\x82\xAB\x82\xDC\x82\xB7\x82\xA9\x81H", "", "$Ba\x82\xCD\x82\xA2", "$Bmb\x82\xA2\x82\xA2\x82\xA6"},
    },
    {
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "Please insert The Last Story disc.", "", ""},
        {"", "Please insert The Last Story disc.", "", ""},
        {"The disc could not be read.", "Refer to the Wii Operations", "Manual for details.", ""},
        {"", "Communications with the Wii Remote have been interrupted.", "", ""},
        {"Connect a Nunchuk or", "Classic Controller to", "the Wii Remote.", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Could not access the Wii System Memory.", "Refer to the Wii Operations Manual for details.", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"The save data cannot be used because the data is corrupted.", "Recreate save data\x3F", "$BaYes", "$BmbNo"},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"The save data cannot be used because the data is corrupted.", "Recreate save data\x3F", "$BaYes", "$BmbNo"},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"There is not enough available space in the Wii System Memory.", "Create 11 blocks of free space either by moving data to an SD Card", "or deleting data in the Wii console's Data Management screen.", ""},
        {"There is not enough available space in the Wii System Memory.", "Either move data to an SD Card or delete data in", "the Wii console's Data Management screen.", ""},
        {"There is not enough available space in the Wii System Memory.", "Create 11 blocks of free space either by moving data to an SD Card", "or deleting data in the Wii console's Data Management screen.", ""},
        {"There is not enough available space in the Wii System Memory.", "Either move data to an SD Card or delete data in", "the Wii console's Data Management screen.", ""},
        {"", "", "", ""},
        {"Return to the Wii Menu Data Management screen\x3F", "", "$BaYes", "$BmbNo"},
    },
    {
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "Bitte schiebe die The Last Story-Disc ein.", "", ""},
        {"", "Bitte schiebe die The Last Story-Disc ein.", "", ""},
        {"Diese Disc kann nicht gelesen werden.", "Bitte lies die Wii-Bedienungsanleitung,", "um weitere Informationen zu erhalten.", ""},
        {"", "Die Verbindung zur Wii-Fernbedienung wurde unterbrochen.", "", ""},
        {"Bitte ein Nunchuk oder einen Classic Controller", "an die Wii-Fernbedienung anschlie\xDF""en.", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Auf den Speicher der Wii-Konsole konnte nicht zugegriffen", "werden. Bitte lies die Wii-Bedienungsanleitung, um weitere", "Informationen zu erhalten.", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Die Datei ist fehlerhaft und kann daher nicht verwendet werden.", "Speicherdaten wiederherstellen\x3F", "$BaJa", "$BmbNein"},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Die Datei ist fehlerhaft und kann daher nicht verwendet werden.", "Speicherdaten wiederherstellen\x3F", "$BaJa", "$BmbNein"},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Nicht gen\xFCgend freier Platz im Speicher der Wii-Konsole vorhanden.", "Bitte rufe den Datenverwaltungsbildschirm der Wii-Konsole auf, um", "Daten auf eine SD Card zu verschieben oder zu l\xF6schen", "(erforderliche Bl\xF6""cke: 11)."},
        {"Nicht gen\xFCgend freier Platz im Speicher der Wii-Konsole vorhanden.", "Bitte rufe den Datenverwaltungsbildschirm der Wii-Konsole auf, um", "Daten auf eine SD Card zu verschieben oder zu l\xF6schen.", ""},
        {"Nicht gen\xFCgend freier Platz im Speicher der Wii-Konsole vorhanden.", "Bitte rufe den Datenverwaltungsbildschirm der Wii-Konsole auf, um", "Daten auf eine SD Card zu verschieben oder zu l\xF6schen", "(erforderliche Bl\xF6""cke: 11)."},
        {"Nicht gen\xFCgend freier Platz im Speicher der Wii-Konsole vorhanden.", "Bitte rufe den Datenverwaltungsbildschirm der Wii-Konsole auf, um", "Daten auf eine SD Card zu verschieben oder zu l\xF6schen.", ""},
        {"", "", "", ""},
        {"Zum Datenverwaltungsbildschirm\x3F", "", "$BaJa", "$BmbNein"},
    },
    {
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "Veuillez ins\xE9rer le disque The Last Story.", "", ""},
        {"", "Veuillez ins\xE9rer le disque The Last Story.", "", ""},
        {"Impossible de lire le disque.", "Veuillez vous r\xE9""f\xE9rer au mode", "d'emploi Wii pour plus de d\xE9tails.", ""},
        {"", "La connexion avec la t\xE9l\xE9""commande Wii a \xE9t\xE9 interrompue.", "", ""},
        {"Veuillez connecter un Nunchuk ou ", "une manette classique \xE0 la t\xE9l\xE9""commande Wii.", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Impossible d'acc\xE9""der \xE0 la m\xE9moire de la console Wii. ", "Veuillez vous r\xE9""f\xE9rer au mode d'emploi Wii pour plus de d\xE9tails.", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Ces donn\xE9""es de sauvegarde ne peuvent pas \xEAtre utilis\xE9""es car ", "le fichier est corrompu. Cr\xE9""er de nouvelles donn\xE9""es de sauvegarde \x3F", "$BaOui", "$BmbNon"},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Ces donn\xE9""es de sauvegarde ne peuvent pas \xEAtre utilis\xE9""es car ", "le fichier est corrompu. Cr\xE9""er de nouvelles donn\xE9""es de sauvegarde \x3F", "$BaOui", "$BmbNon"},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Il n'y a pas assez d'espace libre dans la m\xE9moire de la console Wii. ", "Lib\xE9rez 11 blocs en transf\xE9rant des donn\xE9""es vers une carte SD ou ", "en les effa\xE7""ant depuis l'\xE9""cran de gestion des donn\xE9""es de la console Wii.", ""},
        {"Il n'y a pas assez d'espace libre dans la m\xE9moire de la console Wii.", "Transf\xE9rez des donn\xE9""es sur une carte SD ou effacez des donn\xE9""es ", "inutiles depuis l'\xE9""cran de gestion des donn\xE9""es de la console Wii.", ""},
        {"Il n'y a pas assez d'espace libre dans la m\xE9moire de la console Wii. ", "Lib\xE9rez 11 blocs en transf\xE9rant des donn\xE9""es vers une carte SD ou ", "en les effa\xE7""ant depuis l'\xE9""cran de gestion des donn\xE9""es de la console Wii.", ""},
        {"Il n'y a pas assez d'espace libre dans la m\xE9moire de la console Wii.", "Transf\xE9rez des donn\xE9""es sur une carte SD ou effacez des donn\xE9""es ", "inutiles depuis l'\xE9""cran de gestion des donn\xE9""es de la console Wii.", ""},
        {"", "", "", ""},
        {"Retourner \xE0 l'\xE9""cran de gestion des donn\xE9""es du menu Wii \x3F", "", "$BaOui", "$BmbNon"},
    },
    {
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "Inserta el disco de The Last Story.", "", ""},
        {"", "Inserta el disco de The Last Story.", "", ""},
        {"No se puede leer el disco. Consulta el", "manual de instrucciones de la consola Wii", "para obtener m\xE1s informaci\xF3n.", ""},
        {"", "Se ha interrumpido la conexi\xF3n con el mando de Wii.", "", ""},
        {"", "Conecta un Nunchuk o un mando cl\xE1sico al mando de Wii.", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"No se ha podido acceder a la memoria de la consola Wii.", "Consulta el manual de instrucciones de la consola Wii", "para obtener m\xE1s informaci\xF3n.", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Los datos est\xE1n da\xF1""ados y no se pueden usar.", "\xBFQuieres crear datos de guardado nuevos\x3F", "$BaS\xED", "$BmbNo"},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Los datos est\xE1n da\xF1""ados y no se pueden usar.", "\xBFQuieres crear datos de guardado nuevos\x3F", "$BaS\xED", "$BmbNo"},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"No hay suficiente espacio libre en la memoria de la consola Wii.", "Bloques libres necesarios: 11. Transfiere datos a una tarjeta SD o borra", "los que no necesites en la pantalla de gesti\xF3n de datos de la consola Wii.", ""},
        {"No hay suficiente espacio libre en la memoria de la consola Wii.", "Transfiere datos a una tarjeta SD o borra los que no necesites", "en la pantalla de gesti\xF3n de datos de la consola Wii.", ""},
        {"No hay suficiente espacio libre en la memoria de la consola Wii.", "Bloques libres necesarios: 11. Transfiere datos a una tarjeta SD o borra", "los que no necesites en la pantalla de gesti\xF3n de datos de la consola Wii.", ""},
        {"No hay suficiente espacio libre en la memoria de la consola Wii.", "Transfiere datos a una tarjeta SD o borra los que no necesites", "en la pantalla de gesti\xF3n de datos de la consola Wii.", ""},
        {"", "", "", ""},
        {"\xBFIr a la pantalla de gesti\xF3n de datos del men\xFA de Wii\x3F", "", "$BaS\xED", "$BmbNo"},
    },
    {
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "Inserisci il disco di The Last Story.", "", ""},
        {"", "Inserisci il disco di The Last Story.", "", ""},
        {"Impossibile leggere il disco. Per", "maggiori informazioni, consulta il", "manuale di istruzioni della console Wii.", ""},
        {"", "Il collegamento con il telecomando Wii si \xE8 interrotto.", "", ""},
        {"Collega un Nunchuk o un controller", "tradizionale al telecomando Wii.", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Impossibile accedere alla memoria della console Wii.", "Per maggiori informazioni, consulta il manuale", "di istruzioni della console Wii.", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Il file di salvataggio non pu\xF2 essere usato perch\xE9 i dati", "sono danneggiati. Ricreare i dati di salvataggio\x3F", "$BaS\xEC", "$BmbNo"},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Il file di salvataggio non pu\xF2 essere usato perch\xE9 i dati", "sono danneggiati. Ricreare i dati di salvataggio\x3F", "$BaS\xEC", "$BmbNo"},
        {"", "", "", ""},
        {"", "", "", ""},
        {"", "", "", ""},
        {"Non c'\xE8 abbastanza spazio libero nella memoria della console Wii.", "Blocchi liberi necessari: 11. Per creare spazio, vai allo schermo", "Gestione dati della console Wii e cancella alcuni dati o trasferiscili", "in una scheda SD."},
        {"Non c'\xE8 abbastanza spazio libero nella memoria della console Wii.", "Per creare spazio, vai allo schermo Gestione dati della console Wii", "e cancella alcuni dati o trasferiscili in una scheda SD.", ""},
        {"Non c'\xE8 abbastanza spazio libero nella memoria della console Wii.", "Blocchi liberi necessari: 11. Per creare spazio, vai allo schermo", "Gestione dati della console Wii e cancella alcuni dati o trasferiscili", "in una scheda SD."},
        {"Non c'\xE8 abbastanza spazio libero nella memoria della console Wii.", "Per creare spazio, vai allo schermo Gestione dati della console Wii", "e cancella alcuni dati o trasferiscili in una scheda SD.", ""},
        {"", "", "", ""},
        {"Vuoi andare allo schermo Gestione dati del menu Wii\x3F", "", "$BaS\xEC", "$BmbNo"},
    },
};

// inferred: error window (0x4C bytes, instance lbl_8087F420); names invented
// Real class name ErrorManager (boost::function type names in .rodata, see game/main.cpp).
class ErrorManager {
public:
    ErrorManager();
    void fn_80363DBC();      // inferred: per-frame update
    void fn_803646C4();      // inferred: draws the message without the layouts
    void fn_80364BE8();      // inferred: loads the OS font and sets up GX
    void fn_80364DB4(int x, int y, int s, int t); // inferred: draws a font cell
    void fn_80364E7C(int message); // inferred: shows a message with choices
    void fn_8036503C();      // inferred: moves the cursor of the choices
    void fn_80365320();      // inferred: closes the choices
    // inferred: the message is drawn directly (as GameFlow_unk::IsBusy_unk in game/GameSystem_unk.cpp)
    int IsBusy_unk() {
        int busy = 0;
        if (m18 != 0 || m1C != 0 || mReady == 0) {
            busy = 1;
        }
        return busy;
    }

    int mError;    // 0x00
    int mPrev;     // 0x04
    int mDelay;    // 0x08
    int m0C;       // inferred: check the Wii Remote
    int m10;       // inferred: frames before the Wii Remote check
    unsigned int mPaused; // 0x14: inferred: 1 sequence, 2 sound, 4 bgm, 8 movie
    int m18;
    int m1C;
    int m20;
    atn::D2A_Task* mWindow;  // 0x24
    atn::D2A_Task* mChoice;  // 0x28: inferred: window with four lines
    atn::D2A_Task* mChoice2; // 0x2C: inferred: window with two lines
    atn::D2A_Task* mCursor;  // 0x30
    int mReady;    // 0x34
    int m38;       // inferred: closing the window
    int m3C;       // inferred: closing the choices
    int mSelect;   // 0x40
    atn::FadeColor* mFade; // 0x44
    boost::scoped_ptr<unsigned char> mFont; // 0x48
};
extern "C" ErrorManager* lbl_8087F420;

// inferred: setters of the layout panes (as in atn/D2A_Task.cpp; names invented)
inline void SetText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
inline void SetParam_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
inline void SetRate_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}

// inferred: panes of the message lines
static const char* sLinePanes[] = {"select00_text", "select01_text", "select02_text", "string0007(1)"};
static const char* sChoicePanes[] = {"select00_text", "select01_text"};

// inferred: converts a message to wide characters, returns the length with the terminator
extern "C" int fn_80363C28(wchar_t* dst, const char* src, unsigned int size) {
    if (fn_8006BA30() == 0) {
        return fn_8006F2F0(lbl_8087EEC8, dst, src, size);
    }
    if (dst == 0 || src == 0) {
        return 0;
    }
    unsigned int i = 0;
    unsigned int n = strlen(src);
    for (unsigned int k = 0; k < n; k++) {
        dst[i++] = *(const unsigned char*)src;
        if (i >= size - 1) {
            break;
        }
        src++;
    }
    dst[i] = 0;
    return i + 1;
}

// inferred: creates the instance
extern "C" void fn_80363CE4() {
    if (lbl_8087F420 == 0) {
        lbl_8087F420 = new (1, "", "", 0) ErrorManager();
    }
}

ErrorManager::ErrorManager()
    : mError(0), mPrev(0), mDelay(0), m0C(0), m10(0), mPaused(0), m18(0), m1C(0), m20(0), mWindow(0), mChoice(0),
      mChoice2(0), mCursor(0), mReady(0), m38(0), m3C(0), mSelect(0), mFade(0) {
    DVDSetAutoFatalMessaging(1);
}


// NONMATCHING: 99.47% - the original reloads lbl_8087F018 for m40C0 (here the copy in a register is
// used) and mError when it is kept; register order of the constants 3/0 and of the fade counters
void ErrorManager::fn_80363DBC() {
    if (m20 != 0) {
        return;
    }
    if (mWindow == 0) {
        if (m1C == 0) {
            mWindow = fn_801F3FF8(lbl_8087F530, "/data/d2anime/camp_info4.d2a", 0);
            mWindow->fn_800D246C(1);
            mWindow->mBlend = 19;
            mChoice = fn_801F3FF8(lbl_8087F530, "/data/d2anime/camp_info4.d2a", 0);
            mChoice->fn_800D246C(1);
            mChoice2 = fn_801F3FF8(lbl_8087F530, "/data/d2anime/camp_info7.d2a", 0);
            mChoice2->fn_800D246C(1);
            mCursor = fn_801F3FF8(lbl_8087F530, "/data/d2anime/camp_cursor.d2a", 0);
            mCursor->fn_800D246C(1);
        }
    } else if (mReady == 0 && (mWindow->mFlags & 2) == 2 && (mChoice->mFlags & 2) == 2 &&
               (mChoice2->mFlags & 2) == 2) {
        mWindow->fn_800D246C(0);
        mWindow->mFlags |= 4;
        mWindow->mRepeat = 0;
        SetParam_unk(&mWindow->mData.m10, "pos0001(1)", 4, -15000.0f);
        mChoice->fn_800D246C(0);
        mChoice->mFlags |= 4;
        mChoice->mRepeat = 0;
        mChoice2->fn_800D246C(0);
        mChoice2->mFlags |= 4;
        mChoice2->mRepeat = 0;
        mCursor->fn_800D246C(0);
        mCursor->mFlags |= 4;
        mCursor->mRepeat = 1;
        mReady = 1;
    }
    if (mFade == 0 && lbl_8087F530 != 0) {
        mFade = atn::FadeColor::Create(lbl_8087F530, 1, 0, 0xFF000000);
        mFade->mZ = -10000.0f;
    }
    if (mError == 0 && mPrev != 0) {
        mPrev = 0;
    }
    int error = 0;
    if (!IsBusy_unk()) {
        SysInput_unk* input;
        if (m0C != 0 && (input = lbl_8087F018) != 0 &&
            (HomeButtonMenu::sInstance == 0 || HomeButtonMenu::sInstance->m84 != 1)) {
            if (lbl_8087F018->m40C0 == 0) {
                error = 5;
            }
            int ext = input->GetExt_unk();
            if (mError != 5 && error == 0 && m10 <= 0 && ext != 1 && ext != 2) {
                error = 6;
            }
        }
        if (m10 > 0) {
            m10--;
        }
    }
    switch (DVDGetDriveStatus()) {
    case -1:
        error = 1;
        break;
    case 4:
        error = 2;
        break;
    case 6:
        error = 3;
        break;
    case 11:
        error = 4;
        break;
    }
    if (error == 0) {
        if ((unsigned int)(mError - 1) <= 3 && fn_805FF120() == 0) {
            error = mError;
        }
        if ((unsigned int)(mError - 5) <= 1 && HomeButtonMenu::sInstance != 0 &&
            HomeButtonMenu::sInstance->m84 == 1) {
            error = mError;
        }
    }
    if (error == 0) {
        if (mDelay > 0) {
            mDelay--;
        } else if (mError != 0) {
            if (mError == 5) {
                m10 = 15;
            }
            mPrev = mError;
            mError = error;
            if (mReady != 0 && !mWindow->IsFlag_unk(4)) {
                mWindow->mSpeed = -1.0f;
                m38 = 1;
            }
            if (mFade != 0 && mFade->mActive != 0) {
                mFade->mWait = 0;
                mFade->mStep = -1;
                mFade->mFrame = mFade->mWait + mFade->mDuration + mFade->mDelay;
                mFade->mActive = 1;
            }
            mFont.reset();
        }
    } else if (mError != error) {
        mError = error;
        mDelay = 3;
        if (!IsBusy_unk()) {
            m38 = 0;
            for (int i = 0; i < 4; i++) {
                wchar_t text[0x100];
                fn_80363C28(text, sMessages[fn_8006BA30()][mError][i], 0x100);
                SetText_unk(&mWindow->mData.m10, sLinePanes[i], text);
            }
            mWindow->mSpeed = 1.0f;
            mWindow->mFlags &= ~4;
        } else {
            if (mFade != 0) {
                mFade->mWait = 30000;
                mFade->mStep = 1;
                mFade->mFrame = 0;
                mFade->mActive = 1;
            }
            fn_80364BE8();
        }
        if ((unsigned int)(mError - 1) <= 3 && lbl_8087F610 != 0) {
            fn_804FB530(lbl_8087F610);
        }
    }
    if (m38 != 0 && !mWindow->IsFlag_unk(4) && mWindow->mFrame <= 0.0f) {
        mWindow->mFlags |= 4;
        m38 = 0;
    }
    if (m3C != 0) {
        if (!mChoice->IsFlag_unk(4) && mChoice->mFrame <= 0.0f) {
            mChoice->mFlags |= 4;
            m3C = 0;
        }
        if (!mChoice2->IsFlag_unk(4) && mChoice2->mFrame <= 0.0f) {
            mChoice2->mFlags |= 4;
            m3C = 0;
        }
    }
    atn::FadeColor* fade = mFade;
    if (fade != 0 && fade->mActive != 0 && (float)fade->mStep > 0.0f && fade->mFrame >= fade->mDuration) {
        fade->mStep = 0;
    }
    if (mError == 0) {
        if (mPaused != 0) {
            if (atn::SequenceControl::sInstance != 0 && (mPaused & 1) == 1) {
                atn::SequenceControl::sInstance->fn_800D246C(0);
            }
            if (lbl_8087EFE8 != 0 && (mPaused & 2) == 2) {
                fn_800CF7DC(lbl_8087EFE8, 0, 8, 9);
            }
            if (lbl_8087EE90 != 0 && (mPaused & 4) == 4) {
                fn_8004A2CC(lbl_8087EE90, 0, 8, 0);
            }
            if (lbl_8087F010 != 0 && lbl_8087F010->m08 == 5 && (mPaused & 8) == 8) {
                lbl_8087F010->m08 = 4;
            }
            if (lbl_8087F430 != 0) {
                fn_8037177C(lbl_8087F430);
            }
            mPaused = 0;
        }
    } else {
        int pause = 1;
        if (lbl_8087F610 != 0 && lbl_8087F628->mC8 >= 8) {
            pause = 0;
        }
        void* save = lbl_8087EF68;
        if (save != 0) {
            int work;
            int saving = 1;
            if (fn_800A4228(save, &work) == 0 && ((SysSave_unk*)save)->m174 <= 0) {
                saving = 0;
            }
            if (saving) {
                pause = 0;
            }
        }
        if (pause && mPaused == 0) {
            if (atn::SequenceControl::sInstance != 0 && (atn::SequenceControl::sInstance->mFlags & 1) != 1) {
                atn::SequenceControl::sInstance->fn_800D246C(1);
                mPaused |= 1;
            }
            if (lbl_8087EFE8 != 0) {
                fn_800CF7DC(lbl_8087EFE8, 1, 8, 0);
                mPaused |= 2;
            }
            if (lbl_8087EE90 != 0) {
                fn_8004A2CC(lbl_8087EE90, 1, 8, 0);
                mPaused |= 4;
            }
            if (lbl_8087F010 != 0 && lbl_8087F010->m08 == 4) {
                lbl_8087F010->m08 = 5;
                mPaused |= 8;
            }
        }
    }
}

// inferred: colour as 0xRRGGBBAA from four components in [0, 1] (name invented)
inline unsigned int ToU8_unk(float f) {
    return f >= 1.0f ? 255 : (f <= 0.0f ? 0 : (unsigned int)(255.0f * f + 0.5f));
}

// NONMATCHING: 98.04% - stack slots of the colour and of the pointer to member test, registers of the
// font loop (the scale constant and the double of the int conversion are swapped)
void ErrorManager::fn_803646C4() {
    if (!IsBusy_unk()) {
        if (mError == 0) {
            return;
        }
        if (mReady != 0 && mWindow != 0 && !mWindow->IsFlag_unk(4)) {
            return;
        }
        float size = 20.0f;
        float step = 1.1f * size;
        for (unsigned int i = 0; i < 4; i++) {
            wchar_t text[0x100];
            fn_80363C28(text, sMessages[fn_8006BA30()][mError][i], 0x100);
            fn_80061824(lbl_8087EEB0, 80.0f, step * i + 80.0f, -100000.0f, size, size, 0.0f, 0.0f, 0.0f, text, -1, 1,
                        1, 0, 0, 0xFF000000);
        }
    } else {
        if (mError == 0) {
            return;
        }
        if (mFont) {
            SysGraphics_unk* gfx = lbl_8087EEE0;
            float rgba[4];
            rgba[0] = 0.0f;
            rgba[1] = 0.0f;
            rgba[2] = 0.0f;
            rgba[3] = 1.0f;
            unsigned int r = ToU8_unk(rgba[0]);
            unsigned int g = ToU8_unk(rgba[1]);
            unsigned int b = ToU8_unk(rgba[2]);
            unsigned int a = ToU8_unk(rgba[3]);
            gfx->mClearColor = (a << 24) | (r << 16) | (g << 8) | b;
            fn_8007192C(gfx);
            fn_80075DEC(gfx);
            float one = 1.0f;
            int y = 50;
            for (unsigned int i = 0; i < 4; i++) {
                char text[0x100];
                sprintf(text, "%s\0", sMessages[fn_8006BA30()][mError][i]);
                int x = 0;
                const char* p = text;
                while (*p != 0) {
                    int width;
                    int cy;
                    int cx;
                    void* image;
                    p = OSGetFontTexture(p, &image, &cx, &cy, &width);
                    SysFont_unk* font = (SysFont_unk*)mFont.get();
                    GXTexObj obj;
                    GXInitTexObj(&obj, image, font->m1E, font->m20, font->m18, 0, 0, 0);
                    GXInitTexObjLOD(&obj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
                    GXLoadTexObj(&obj, 0);
                    SysMtx mtx;
                    PSMTXScale(mtx, one / ((SysFont_unk*)mFont.get())->m1E, one / ((SysFont_unk*)mFont.get())->m20, one);
                    GXLoadTexMtxImm(mtx, 30, 1);
                    GXSetNumTexGens(1);
                    GXSetTexCoordGen2(0, 1, 4, 30, 0, 125);
                    fn_80364DB4(x + 50, y, cx, cy);
                    x += width;
                }
                y += 30;
            }
            fn_80071AFC(gfx);
            fn_80071D60(gfx, 1);
        } else if (mReady == 0 || mWindow->IsFlag_unk(4)) {
            fn_80364BE8();
        }
    }
}

void ErrorManager::fn_80364BE8() {
    if ((unsigned short)OSGetFontEncode() == 1) {
        mFont.reset(new (1, "", "", 0) unsigned char[0x120F00]);
    } else {
        mFont.reset(new (1, "", "", 0) unsigned char[0x20120]);
    }
    OSInitFont(mFont.get());
    SysGraphics_unk* gfx = lbl_8087EEE0;
    SysMtx44 proj;
    C_MTXOrtho(proj, 0.0f, gfx->mHeight, 0.0f, gfx->mWidth, 0.0f, -100.0f);
    GXSetProjection(proj, 1);
    SysMtx mtx;
    PSMTXIdentity(mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    GXSetZMode(1, 7, 1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOp(0, 3);
    GXSetTevOrder(0, 0, 0, 255);
    GXSetBlendMode(1, 1, 1, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 3, 0);
    GXSetVtxAttrFmt(0, 13, 1, 3, 0);
}

void ErrorManager::fn_80364DB4(int x, int y, int s, int t) {
    short x0 = x;
    short x1 = x0 + ((SysFont_unk*)mFont.get())->m10;
    short y0 = y;
    short y1 = y0 + ((SysFont_unk*)mFont.get())->m12;
    short s0 = s;
    short s1 = s + ((SysFont_unk*)mFont.get())->m10;
    short t0 = t;
    short t1 = t + ((SysFont_unk*)mFont.get())->m12;
    GXBegin(0x80, 0, 4);
    GXPosition3s16_unk(x0, y0, 0);
    GXTexCoord2s16_unk(s0, t0);
    GXPosition3s16_unk(x1, y0, 0);
    GXTexCoord2s16_unk(s1, t0);
    GXPosition3s16_unk(x1, y1, 0);
    GXTexCoord2s16_unk(s1, t1);
    GXPosition3s16_unk(x0, y1, 0);
    GXTexCoord2s16_unk(s0, t1);
}

void ErrorManager::fn_80364E7C(int message) {
    if (mReady == 0) {
        return;
    }
    if (message == 21 || message == 12 || message == 30) {
        for (int i = 0; i < 2; i++) {
            wchar_t text[0x100];
            fn_80363C28(text, sMessages[fn_8006BA30()][message][i], 0x100);
            SetText_unk(&mChoice2->mData.m10, sChoicePanes[i], text);
        }
        mChoice2->mSpeed = 1.0f;
        mChoice2->mFrame = 0.0f;
        mChoice2->mFlags &= ~4;
        mCursor->mSpeed = 1.0f;
        mCursor->mFrame = 0.0f;
        mCursor->mFlags &= ~4;
        mSelect = 1;
        fn_8036503C();
    } else {
        for (int i = 0; i < 4; i++) {
            wchar_t text[0x100];
            fn_80363C28(text, sMessages[fn_8006BA30()][message][i], 0x100);
            SetText_unk(&mChoice->mData.m10, sLinePanes[i], text);
        }
        mChoice->mSpeed = 1.0f;
        mChoice->mFrame = 0.0f;
        mChoice->mFlags &= ~4;
        mSelect = 0;
    }
}

void ErrorManager::fn_8036503C() {
    if (mReady == 0) {
        return;
    }
    atn::D2A_Task* choice = mChoice2;
    if (choice->IsFlag_unk(4)) {
        return;
    }
    atn::D2A_DrawRect rect;
    if (mSelect == 0) {
        rect = fn_801F4E8C(choice, fn_800DC6B4("cursor_point0"));
    } else {
        rect = fn_801F4E8C(choice, fn_800DC6B4("cursor_point1"));
    }
    SetParam_unk(&mCursor->mData.m10, "cursor_pos", 0, rect.x);
    SetParam_unk(&mCursor->mData.m10, "cursor_pos", 1, rect.y);
    SetParam_unk(&mCursor->mData.m10, "cursor_pos", 4, rect.z);
    SetRate_unk(&mCursor->mData.m10, "wRate", rect.w);
    SetRate_unk(&mCursor->mData.m10, "hRate", rect.h);
    void* pad = lbl_8087EF70;
    if (pad == 0) {
        return;
    }
    if (fn_800A555C(pad, 0, 0) != 0 || fn_800A555C(pad, 0, 26) != 0) {
        if (--mSelect < 0) {
            mSelect += 2;
        }
        fn_800C31F4("SE_UICOM_003", 0, -1, 1.0f);
    } else if (fn_800A555C(pad, 0, 1) != 0 || fn_800A555C(pad, 0, 25) != 0) {
        if ((unsigned int)++mSelect >= 2) {
            mSelect -= 2;
        }
        fn_800C31F4("SE_UICOM_003", 0, -1, 1.0f);
    }
}

void ErrorManager::fn_80365320() {
    if (mReady == 0) {
        return;
    }
    if (!mChoice->IsFlag_unk(4)) {
        mChoice->mSpeed = -1.0f;
    }
    if (!mChoice2->IsFlag_unk(4)) {
        mChoice2->mSpeed = -1.0f;
    }
    if (!mCursor->IsFlag_unk(4)) {
        mCursor->mFlags |= 4;
    }
    m3C = 1;
}
