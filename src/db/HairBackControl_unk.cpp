#include "atn/Csv.h"
#include "atn/LH_Binary.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" char* strstr(const char* s, const char* sub);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: hash of a name
extern "C" std::string fn_8006B174(const std::string& s); // inferred: normalised copy of a name
// inferred: loads a resource synchronously (path relative to the data root)
// fn_80473FCC is atn::LoadHandle::fn_80473FCC (atn/LoadHandle.h)

// inferred: attributes of a map (fn_8021771C, db/MapIdTable_unk); mB4 has one bit per group of
// motions available in the map
struct MapAttr_unk {
    unsigned char m00[0xB4];
    unsigned int mFlags;
};
extern "C" MapAttr_unk* fn_8021771C(int map, int a, int b);

// inferred: motion names: ids 0-569 (sNames) and 570- (sNames2), see fn_802180A8
static const char* const sNames[570] = {
    "CH_WTG00",
    "CH_WTM00",
    "CH_WTN00",
    "CH_WTS00",
    "CH_WTT00",
    "CH_WTS00_02",
    "CH_WTN00_02",
    "CH_WTA00A",
    "CH_WTA00B",
    "CH_WTA02",
    "CH_WTN02",
    "CH_WTA02_01",
    "CH_WTN02_01",
    "CH_WTA02_02",
    "CH_WTN02_02",
    "CH_WTA02_04",
    "CH_WTA02_05",
    "CH_WTA08",
    "CH_WKG00",
    "CH_WKM00",
    "CH_WKN00",
    "CH_WKS00",
    "CH_WKT00",
    "CH_WKN00_02",
    "CH_RSG00",
    "CH_RSM00",
    "CH_RSN00",
    "CH_RSS00",
    "CH_RST00",
    "CH_RSN00_02",
    "CH_RNG00",
    "CH_RNM00",
    "CH_RNN00",
    "CH_RNS00",
    "CH_RNT00",
    "CH_RNN01",
    "CH_RNN00_02",
    "CH_CRG00",
    "CH_CRG01",
    "CH_CRM00",
    "CH_CRN00",
    "CH_CRS00",
    "CH_CRT00",
    "CH_APA00A",
    "CH_APA00B",
    "CH_APA01",
    "CH_DDA00A",
    "CH_DDA00A01",
    "CH_DDA00A02",
    "CH_DDA00B",
    "CH_DDA00B02",
    "CH_DMA00",
    "CH_DMA00_01",
    "CH_DMA01",
    "CH_DMA01_02",
    "CH_DMA02",
    "CH_DMA03A",
    "CH_DMA03A01",
    "CH_DMA03A02",
    "CH_DMA03B",
    "CH_DMA03B02",
    "CH_DMA04A",
    "CH_DMA04B",
    "CH_DMA04C",
    "CH_DMA04D",
    "CH_GMA04",
    "CH_GMA05",
    "CH_GMA06",
    "CH_GMA07",
    "CH_ASA00",
    "CH_ASA08",
    "CH_ASA13",
    "CH_AMA08",
    "CH_IMA00",
    "CH_IMA01",
    "CH_IMA02L",
    "CH_IMA02R",
    "CH_IMA02W",
    "CH_IMA03",
    "CH_IMA05",
    "CH_IMA05_02",
    "CH_IMA08",
    "CH_IMA08_01",
    "CH_IMA08_02",
    "CH_IMA11",
    "CH_IMN10_00",
    "CH_IMN10_01",
    "CH_IMN10_02",
    "CH_IMN10_03",
    "CH_IMN10_04",
    "CH_IMN10_05",
    "CH_IMA12_00",
    "CH_IMN13",
    "CH_MSN005",
    "CH_MSN000",
    "CH_UDN01_02",
    "CH_IOA00",
    "CH_IOA01",
    "CH_IOS03",
    "CH_ITA01",
    "CH_ITA04",
    "CH_ITN03",
    "CH_SKM00A",
    "CH_SKM00B",
    "CH_SKM00C",
    "CH_SKM00D",
    "CH_SKM01",
    "CH_SKM00_02",
    "CH_THC00",
    "CH_THC01",
    "CH_THC02",
    "CH_THC03",
    "CH_THC04",
    "CH_PSN_RAND00",
    "CH_PSN_RAND01",
    "CH_PSN_RAND02",
    "CH_PSN_RAND03",
    "CH_PSN_MEET00B",
    "CH_PSN_MEET00D",
    "CH_PSN_MEET00E",
    "CH_PSN_MEET01A",
    "CH_PSN_MEET01B",
    "CH_PSN_MEET01C",
    "CH_PSN_MEET01D",
    "CH_PSN_MEET01E",
    "CH_PSN_MEET02B",
    "CH_PSN_MEET03A",
    "CH_PSN_MEET03B",
    "CH_PSN_MEET03C",
    "CH_PSN_MEET04A",
    "CH_PSN_MEET04B",
    "CH_PSN_MEET04C",
    "CH_PMN_12A_00",
    "CH_PMN_12B_00",
    "CH_PMN_12C_00",
    "CH_PMN_12A_01",
    "CH_PMN_12B_01",
    "CH_PMN_12C_01",
    "CH_PSN00_08",
    "CH_ASN05_00",
    "CH_CHASE_00",
    "CH_CHASE_01",
    "CH_PMN_10_03",
    "CH_PSN_03F",
    "CH_PSN_03B",
    "CH_PSN_03L",
    "CH_PSN_03R",
    "CH_PMN_03_01L",
    "CH_PMN_03_01R",
    "CH_PMN_03_04",
    "CH_PCN_40_00A",
    "CH_PCN_40_00B",
    "CH_PCN_40_00C",
    "CH_PCN_40_01A",
    "CH_PCN_40_01B",
    "CH_PCN_40_01C",
    "CH_PCN_40_02",
    "CH_PCN_40_03",
    "CH_FACE01",
    "CH_FACE02",
    "CH_FACE03",
    "CH_FACE04",
    "CH_FACE05",
    "CH_FACE_DFN00",
    "CH_FACE_DFN00_01",
    "CH_FACE_DFN00_02",
    "CH_FACE_DFT00",
    "CH_FACE_DFT00_01",
    "CH_FACE_ANG00",
    "CH_FACE_ANG00_01",
    "CH_FACE_ANG01",
    "CH_FACE_ANG01_01",
    "CH_FACE_DMG00",
    "CH_FACE_DMG00_01",
    "CH_FACE_DMG01",
    "CH_FACE_DMG01_01",
    "CH_FACE_SAD00",
    "CH_FACE_SAD00_01",
    "CH_FACE_SAD01",
    "CH_FACE_SAD01_01",
    "CH_FACE_SML00",
    "CH_FACE_SML00_01",
    "CH_FACE_SML01",
    "CH_FACE_SML01_01",
    "CH_FACE_SML02",
    "CH_FACE_SML02_01",
    "CH_FACE_SML03",
    "CH_FACE_SML03_01",
    "CH_FACE_SUP00",
    "CH_FACE_SUP00_01",
    "CH_FACE_SUP01",
    "CH_FACE_SUP01_01",
    "CH_FACE_KIS00",
    "CH_FACE_KIS00_01",
    "CH_FACE_OMK00",
    "CH_FACE_OMK00_01",
    "CH_FACE_OMK01",
    "CH_FACE_OMK01_01",
    "CH_FACE_SPF00",
    "CH_FACE_SPF00_01",
    "CH_FACE_DFB00",
    "CH_FACE_DFB00_01",
    "CH_FACE_FER00",
    "CH_FACE_FER00_01",
    "CH_FACE_QUE00",
    "CH_FACE_QUE00_01",
    "CH_FACE_PER00",
    "CH_FACE_PER00_01",
    "CH_LIP_A",
    "CH_LIP_I",
    "CH_LIP_U",
    "CH_LIP_E",
    "CH_LIP_O",
    "CH_LIP_N",
    "CH_SWAP01",
    "CH_SWAP02",
    "CH_SWAP03",
    "CH_SWAP04",
    "CH_SWAP05",
    "CH_SWAP06",
    "CH_SWAP07",
    "CH_SWAP08",
    "CH_SWAP09",
    "CH_SWAP10",
    "CH_SWAP11",
    "CH_SWAP12",
    "CH_SWAP13",
    "CH_SWAP14",
    "CH_SWAP15",
    "CH_SWAP16",
    "CH_SWAP17",
    "CH_SWAP18",
    "CH_SWAP19",
    "CH_SWAP20",
    "CH_AI01",
    "CH_AI02",
    "CH_AI03",
    "CH_AI04",
    "CH_C_EVENT",
    "CH_EV01",
    "CH_EV02",
    "CH_EV03",
    "CH_EV04",
    "CH_EV05",
    "CH_EV06",
    "CH_EV07",
    "CH_EV08",
    "CH_EV09",
    "CH_EV10",
    "CH_EV11",
    "CH_EV12",
    "CH_EV13",
    "CH_EV14",
    "CH_EV15",
    "CH_EV16",
    "CH_EV17",
    "CH_EV18",
    "CH_EV19",
    "CH_EV20",
    "CH_EV21",
    "CH_EV22",
    "CH_EV23",
    "CH_EV24",
    "CH_EV25",
    "CH_EV26",
    "CH_EV27",
    "CH_EV28",
    "CH_EV29",
    "CH_EV30",
    "CH_EV31",
    "CH_EV32",
    "CH_EV33",
    "CH_EV34",
    "CH_EV35",
    "CH_EV36",
    "CH_EV37",
    "CH_EV38",
    "CH_EV39",
    "CH_EV40",
    "CH_EV41",
    "CH_EV42",
    "CH_EV43",
    "CH_EV44",
    "CH_EV45",
    "CH_EV46",
    "CH_EV47",
    "CH_EV48",
    "CH_EV49",
    "CH_EV50",
    "CH_EV51",
    "CH_EV52",
    "CH_EV53",
    "CH_EV54",
    "CH_EV55",
    "CH_EV56",
    "CH_EV57",
    "CH_EV58",
    "CH_EV59",
    "CH_EV60",
    "CH_EV61",
    "CH_EV62",
    "CH_EV63",
    "CH_EV64",
    "CH_EV65",
    "CH_EV66",
    "CH_EV67",
    "CH_EV68",
    "CH_EV69",
    "CH_EV70",
    "CH_EV71",
    "CH_EV72",
    "CH_EV73",
    "CH_EV74",
    "CH_EV75",
    "CH_EV76",
    "CH_EV77",
    "CH_EV78",
    "CH_EV79",
    "CH_EV80",
    "CH_SK01A",
    "CH_SK01B",
    "CH_SK01C",
    "CH_SK02A",
    "CH_SK02B",
    "CH_SK02C",
    "CH_SK03A",
    "CH_SK03B",
    "CH_SK03C",
    "CH_SK03D",
    "CH_SK03E",
    "CH_SK04A",
    "CH_SK04B",
    "CH_SK04C",
    "CH_SK05A",
    "CH_SK05B",
    "CH_SK05C",
    "CH_SKN_CALL00",
    "CH_WTG01",
    "CH_WTM01",
    "CH_WTN01",
    "CH_WTS01",
    "CH_WTT01",
    "CH_ATS00",
    "CH_ATS00_02",
    "CH_ATS00L",
    "CH_ATS00R",
    "CH_ATS00BL",
    "CH_ATS00BR",
    "CH_ATS01",
    "CH_ATS02",
    "CH_ATS03",
    "CH_ATS04",
    "CH_ATS06",
    "CH_ATS07",
    "CH_ATS08_00",
    "CH_ATS08_01",
    "CH_ATA07",
    "CH_ATD00",
    "CH_ATD01",
    "CH_ATD02",
    "CH_ATD03",
    "CH_ATD04",
    "CH_ATT00",
    "CH_IMS06A",
    "CH_IMS06B",
    "CH_IMS06C",
    "CH_ATS09_00",
    "CH_ATS09_01",
    "CH_ATS09_02",
    "CH_DDS09_00",
    "CH_DDS09_02",
    "CH_SKS01A",
    "CH_SKS01B",
    "CH_SKS11",
    "CH_SKS11_01",
    "CH_ASS18",
    "CH_ASA19",
    "CH_ASA19_01",
    "CH_ASA19_02",
    "CH_ASA19_03",
    "CH_ASA19_04",
    "CH_ATG00_C",
    "CH_ATG01_C",
    "CH_ATG00_HL",
    "CH_ATG00_HR",
    "CH_ATG00_HCL",
    "CH_ATG00_HCR",
    "CH_ATG00_HCUL",
    "CH_ATG00_HCUR",
    "CH_HCG00",
    "CH_HCG01",
    "CH_HCG02",
    "CH_HCG03",
    "CH_HCG04",
    "CH_HCG05",
    "CH_HCG06",
    "CH_HCG07",
    "CH_HCM00",
    "CH_HCM01",
    "CH_HCM02",
    "CH_HCM03",
    "CH_HCM04",
    "CH_HCM05",
    "CH_HCM06",
    "CH_HCM07",
    "CH_HCS00",
    "CH_HCS01",
    "CH_HCS02",
    "CH_HCS03",
    "CH_HCS04",
    "CH_HCS05",
    "CH_HCS06",
    "CH_HCS07",
    "CH_HCT00",
    "CH_HCT01",
    "CH_HCT02",
    "CH_HCT03",
    "CH_HCT04",
    "CH_HCT05",
    "CH_HCT06",
    "CH_HCT07",
    "CH_HSG00",
    "CH_HSG01",
    "CH_HSG02",
    "CH_HSG03",
    "CH_HSG04",
    "CH_HSG05",
    "CH_HSM00",
    "CH_HSM01",
    "CH_HSM02",
    "CH_HSM03",
    "CH_HSM04",
    "CH_HSM05",
    "CH_HSS00",
    "CH_HSS01",
    "CH_HSS02",
    "CH_HSS03",
    "CH_HSS04",
    "CH_HSS05",
    "CH_HSS08",
    "CH_HSS09",
    "CH_HSS10",
    "CH_HST00",
    "CH_HST01",
    "CH_HST02",
    "CH_HST03",
    "CH_HST04",
    "CH_HST05",
    "CH_GDS00",
    "CH_GDS01",
    "CH_GDS00_01",
    "CH_GDS01_01",
    "CH_GDT00",
    "CH_GDT01",
    "CH_GMS00",
    "CH_GMS01",
    "CH_GMS02",
    "CH_GMS03",
    "CH_GMS00_01",
    "CH_GMS01_01",
    "CH_GMS02_01",
    "CH_GMS03_01",
    "CH_GMT00",
    "CH_GMT01",
    "CH_GMT02",
    "CH_GMT03",
    "CH_SKA06",
    "CH_DMA05A",
    "CH_DMA05B",
    "CH_DMA05C",
    "CH_DMA05D",
    "CH_DMA06A02",
    "CH_DMA06B02",
    "CH_DMA06A03",
    "CH_DMA06B03",
    "CH_DMA06C03",
    "CH_DMA09",
    "CH_DMA12",
    "CH_DMA12_01",
    "CH_DMA04A_01",
    "CH_DMA04B_01",
    "CH_DMA09A",
    "CH_DMA09B",
    "CH_DMA09C",
    "CH_DMA09_03",
    "CH_DMT03B",
    "CH_DDT00A",
    "CH_DDT00B",
    "CH_MSN005_01",
    "CH_MSN000_01",
    "CH_AVA02",
    "CH_ITA00",
    "CH_ITA02",
    "CH_DMA16",
    "CH_GMN04",
    "CH_GMN05",
    "CH_RIDE_WTS_03",
    "CH_RIDE_RNN_00",
    "CH_RIDE_ASS_16",
    "CH_CARRY01",
    "CH_CARRY02",
    "CH_MCN002",
    "CH_MCN003",
    "CH_MCN004",
    "CH_MCN004_02",
    "CH_MCN005",
    "CH_MCN006",
    "CH_MCN007",
    "CH_MCN002_01",
    "CH_MCN003_01",
    "CH_MCN005_01",
    "CH_MCN006_01",
    "CH_IMA01_01",
    "CH_IMA03_01",
    "CH_MWN02",
    "CH_MWN03",
    "CH_MWN04",
    "CH_MWN05",
    "CH_MWN07",
    "CH_IMA15_00",
    "CH_IMA15_01",
    "CH_IMA15_02",
    "CH_IMS06B_01",
    "CH_IOS04",
    "CH_DMN00",
    "CH_IOS04_01",
    "CH_IOS04_02",
    "CH_ASN16_00",
    "CH_DMA10A",
    "CH_DMA10B",
    "CH_IOS04_08",
    "CH_IMS16_00",
    "CH_IMS16_01",
    "CH_IOS04_05",
    "CH_IOS04_09",
    "CH_DMA17",
    "CH_IOA05A",
    "CH_IOA05B",
    "CH_IOA06A",
    "CH_IOA06B",
    "CH_IOA07A",
    "CH_IOA07B",
    "CH_IOA07C",
    "CH_IOA07D",
    "CH_IOA08A",
    "CH_IOA08B",
    "CH_IOA08C",
    "CH_IOA08D",
    "CH_DMN02",
    "CH_IOA09A",
    "CH_IOA09B",
    "CH_IOA10A",
    "CH_IOA10B",
    "CH_IOA10C",
    "CH_IOA11A",
    "CH_IOA11B",
    "CH_IOA11C",
    "CH_IOA12_01",
    "CH_IOA12_02",
    "CH_ACS01_01",
    "CH_ACS01_02",
    "CH_ACS01A",
    "CH_ACS01B",
    "CH_ACS01C_01",
    "CH_ACS01C_02",
    "CH_ACS10",
    "CH_DMN01",
    "CH_AES02A",
    "CH_AES02B",
    "CH_AES02C",
};
static const char* const sNames2[40] = {
    "GM_ACT01",
    "GM_ACT02",
    "GM_ACT03",
    "GM_ACT04",
    "GM_ACT05",
    "GM_ACT06",
    "GM_ACT07",
    "GM_ACT08",
    "GM_ACT09",
    "GM_ACT10",
    "GM_EV01",
    "GM_EV02",
    "GM_EV03",
    "GM_EV04",
    "GM_EV05",
    "GM_EV06",
    "GM_EV07",
    "GM_EV08",
    "GM_EV09",
    "GM_EV10",
    "GM_EV11",
    "GM_EV12",
    "GM_EV13",
    "GM_EV14",
    "GM_EV15",
    "GM_EV16",
    "GM_EV17",
    "GM_EV18",
    "GM_EV19",
    "GM_EV20",
    "GM_EV21",
    "GM_EV22",
    "GM_EV23",
    "GM_EV24",
    "GM_EV25",
    "GM_EV26",
    "GM_EV27",
    "GM_EV28",
    "GM_EV29",
    "GM_EV30",
};

static unsigned int* sHashes;
static unsigned int* sHashes2;
static bool* sEnabled;
static unsigned int* sHairHashes; // inferred: hashes of /boot/hair_back_control.csv
static unsigned int sHairCount;

// inferred: hashes the motion names and loads /boot/hair_back_control.csv
extern "C" void fn_80217E18() {
    sHashes = new (11, "", "", 0) unsigned int[570];
    sEnabled = new (11, "", "", 0) bool[570];
    for (int i = 0; i < 570; i++) {
        sHashes[i] = fn_800DC6B4(sNames[i]);
        sEnabled[i] = true;
    }
    sHashes2 = new (11, "", "", 0) unsigned int[40];
    for (int i = 0; i < 40; i++) {
        sHashes2[i] = fn_800DC6B4(sNames2[i]);
    }
    atn::LH_Binary handle;
    handle.fn_80473FCC("/boot/hair_back_control.csv");
    atn::Csv csv((const char*)handle.fn_80470580(), handle.fn_8047059C());
    do {
        if (*csv.fn_8005B9CC() != '\0') {
            sHairCount++;
        }
    } while (csv.fn_8005BBF8() != 0);
    sHairHashes = new (11, "", "", 0) unsigned int[sHairCount];
    csv.Reset_unk();
    do {
        if (*csv.fn_8005B9CC() != '\0') {
            // inferred: the original always writes the first element
            sHairHashes[0] = fn_800DC6B4(csv.mField);
        }
    } while (csv.fn_8005BBF8() != 0);
}

// inferred: motion id from its name (-1 if unknown or not available in the current map)
extern "C" int fn_802180A8(const char* name) {
    unsigned int hash = fn_800DC6B4(name);
    for (int i = 0; i < 570; i++) {
        if (hash == sHashes[i]) {
            return sEnabled[i] ? i : -1;
        }
    }
    for (int i = 0; i < 40; i++) {
        if (hash == sHashes2[i]) {
            return i;
        }
    }
    return -1;
}

extern "C" const char* fn_80218218(int id) {
    if (id >= 0 && id <= 569) {
        return sNames[id];
    }
    return 0;
}

extern "C" const char* fn_8021823C(int id) {
    if (id >= 0 && id <= 39) {
        return sNames2[id];
    }
    return 0;
}

// inferred: address of a member at 0xD0 of an unknown object
struct Unk_80218260 {
    unsigned char m00[0xD0];
    unsigned char mD0;
};
extern "C" unsigned char* fn_80218260(Unk_80218260* p) {
    return &p->mD0;
}

static inline void Disable_unk(int first, int last) {
    for (int i = first; i <= last; i++) {
        sEnabled[i] = false;
    }
}

// inferred: enables the motions available in a map
extern "C" void fn_80218268(int map, int a, int b) {
    for (int i = 0; i < 570; i++) {
        sEnabled[i] = true;
    }
    MapAttr_unk* attr = fn_8021771C(map, a, b);
    if (attr != 0) {
        if ((attr->mFlags & 0x1) != 0x1) {
            Disable_unk(0x1F5, 0x1FF);
        }
        if ((attr->mFlags & 0x2) != 0x2) {
            Disable_unk(0x237, 0x239);
        }
        if ((attr->mFlags & 0x4) != 0x4) {
            Disable_unk(0x22F, 0x234);
        }
        if ((attr->mFlags & 0x8) != 0x8) {
            Disable_unk(0x235, 0x236);
        }
        if ((attr->mFlags & 0x10) != 0x10) {
            Disable_unk(0x202, 0x206);
        }
        if ((attr->mFlags & 0x20) != 0x20) {
            Disable_unk(0x207, 0x209);
        }
        if ((attr->mFlags & 0x40) != 0x40) {
            Disable_unk(0x218, 0x21B);
        }
        if ((attr->mFlags & 0x80) != 0x80) {
            Disable_unk(0x21C, 0x21F);
        }
        if ((attr->mFlags & 0x100) != 0x100) {
            Disable_unk(0x220, 0x221);
        }
        if ((attr->mFlags & 0x200) != 0x200) {
            Disable_unk(0x225, 0x226);
        }
        if ((attr->mFlags & 0x400) != 0x400) {
            Disable_unk(0x227, 0x229);
        }
        if ((attr->mFlags & 0x800) != 0x800) {
            Disable_unk(0x210, 0x212);
        }
        if ((attr->mFlags & 0x1000) != 0x1000) {
            Disable_unk(0x22A, 0x22E);
        }
        if ((attr->mFlags & 0x2000) != 0x2000) {
            Disable_unk(0x215, 0x217);
        }
    }
}

extern "C" bool* fn_802185F4() {
    return sEnabled;
}

// inferred: whether a motion uses the back hair control (listed in the CSV, or not an "_ev" motion)
extern "C" bool fn_802185FC(const char* name) {
    unsigned int hash = fn_800DC6B4(fn_8006B174(std::string(name)).c_str());
    for (unsigned int i = 0; i < sHairCount; i++) {
        if (hash == sHairHashes[i]) {
            return true;
        }
    }
    return strstr(name, "_ev") == 0;
}
