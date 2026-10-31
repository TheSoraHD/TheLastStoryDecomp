#include "revolution/nand.h"
#include "revolution/fs.h"
#include "revolution/os.h"
#include "revolution/gx.h"
#include "revolution/sc.h"
#include "private/iosrestypes.h"

// inferred: new file of the 2010 SDK (not in the 2007 source). Messages taken from main.dol,
// array names invented from the error each one describes.

extern u16 OSSetFontEncode(u16);

static void (*NANDErrorFunc)(s32) = NULL;

static const char* const __NANDErrorMessageMaxBlocksDefault[] = {
    "\n"
    "\n"
    "\n"
    "Wii本体保存メモリの空き容量が異常です。\n"
    "くわしくはWii本体の取扱説明書をお読み\n"
    "ください。",

    "\n"
    "\n"
    "\n"
    "There is no more available space in\n"
    "Wii system memory. Refer to the Wii\n"
    "Operations Manual for details.",

    "\n"
    "\n"
    "\n"
    "Der Speicher der Wii-Konsole ist belegt.\n"
    "Bitte lies die Wii-Bedienungsanleitung,\n"
    "um weitere Informationen zu erhalten.",

    "\n"
    "\n"
    "\n"
    "Il n'y a pas assez d'espace libre\n"
    "dans la m\xE9""moire de la console Wii.\n"
    "Veuillez vous r\xE9""f\xE9""rer au Mode d'emploi\n"
    "de la Wii pour plus de d\xE9""tails.",

    "\n"
    "\n"
    "\n"
    "No queda espacio libre en la memoria\n"
    "de la consola Wii. Consulta el manual\n"
    "de operaciones de la consola Wii para\n"
    "obtener m\xE1""s informaci\xF3""n.",

    "\n"
    "\n"
    "\n"
    "Non c'\xE8"" pi\xF9"" spazio libero nella memoria\n"
    "della console Wii. Per maggiori\n"
    "informazioni, consulta il manuale di\n"
    "istruzioni della console Wii.",

    "\n"
    "\n"
    "\n"
    "Er is geen vrije ruimte meer in het\n"
    "interne geheugen van het Wii-systeem.\n"
    "Lees de handleiding voor meer informatie."
};

static const char* const __NANDErrorMessageMaxBlocksEurope[] = {
    "\n"
    "\n"
    "\n"
    "Wii本体保存メモリの空き容量が異常です。\n"
    "くわしくはWii本体の取扱説明書をお読み\n"
    "ください。",

    "\n"
    "\n"
    "\n"
    "There is no more available space in\n"
    "the Wii System Memory. Please refer to\n"
    "the Wii Operations Manual for details.",

    "\n"
    "\n"
    "\n"
    "Der Speicher der Wii-Konsole ist belegt.\n"
    "Bitte lies die Wii-Bedienungsanleitung,\n"
    "um weitere Informationen zu erhalten.",

    "\n"
    "\n"
    "\n"
    "Il n'y a pas assez d'espace libre dans\n"
    "la m\xE9""moire de la console Wii. Veuillez\n"
    "vous r\xE9""f\xE9""rer au mode d'emploi Wii pour\n"
    "plus de d\xE9""tails.",

    "\n"
    "\n"
    "\n"
    "No queda espacio libre en la memoria de\n"
    "la consola Wii. Consulta el manual de\n"
    "instrucciones de la consola Wii para\n"
    "obtener m\xE1""s informaci\xF3""n.",

    "\n"
    "\n"
    "\n"
    "Non c'\xE8"" pi\xF9"" spazio libero nella memoria\n"
    "della console Wii. Per maggiori\n"
    "informazioni, consulta il manuale di\n"
    "istruzioni della console Wii.",

    "\n"
    "\n"
    "\n"
    "Er is geen vrije ruimte meer in het\n"
    "interne geheugen van het Wii-systeem.\n"
    "Lees de handleiding voor meer informatie."
};

static const char* const __NANDErrorMessageMaxBlocksChinaKorea[] = {
    "\n"
    "\n"
    "エラーコード４０５。\n"
    "\n"
    "Wii本体保存メモリの空き容量が異常です。\n"
    "くわしくはWii本体の取扱説明書をお読み\n"
    "ください。",

    "\n"
    "\n"
    "Error #405,\n"
    "\n"
    "There is no more available space in\n"
    "Wii system memory. Refer to the Wii\n"
    "Operations Manual for details."
};

static const char* const __NANDErrorMessageMaxFilesDefault[] = {
    "\n"
    "\n"
    "\n"
    "Wii本体保存メモリの空きファイル数が異常です。\n"
    "くわしくはWii本体の取扱説明書をお読み\n"
    "ください。",

    "\n"
    "\n"
    "\n"
    "There is no more available space in\n"
    "Wii system memory. Refer to the Wii\n"
    "Operations Manual for details.",

    "\n"
    "\n"
    "\n"
    "Der Speicher der Wii-Konsole ist belegt.\n"
    "Bitte lies die Wii-Bedienungsanleitung,\n"
    "um weitere Informationen zu erhalten.",

    "\n"
    "\n"
    "\n"
    "Il n'y a pas assez d'espace libre dans\n"
    "la m\xE9""moire de la console Wii. Veuillez\n"
    "vous r\xE9""f\xE9""rer au Mode d'emploi de la Wii\n"
    "pour plus de d\xE9""tails.",

    "\n"
    "\n"
    "\n"
    "No queda espacio libre en la memoria de\n"
    "la consola Wii. Consulta el manual de\n"
    "operaciones de la consola Wii para\n"
    "obtener m\xE1""s informaci\xF3""n.",

    "\n"
    "\n"
    "\n"
    "Impossibile salvare altri dati nella\n"
    "memoria della console Wii. Per maggiori\n"
    "informazioni, consulta il manuale di\n"
    "istruzioni della console Wii.",

    "\n"
    "\n"
    "\n"
    "Er is geen ruimte meer beschikbaar\n"
    "in het interne geheugen van het\n"
    "Wii-systeem. Lees de handleiding voor\n"
    "meer informatie."
};

static const char* const __NANDErrorMessageMaxFilesEurope[] = {
    "\n"
    "\n"
    "\n"
    "Wii本体保存メモリの空きファイル数が異常です。\n"
    "くわしくはWii本体の取扱説明書をお読み\n"
    "ください。",

    "\n"
    "\n"
    "\n"
    "There is no more available space in\n"
    "the Wii System Memory. Please refer to\n"
    "the Wii Operations Manual for details.",

    "\n"
    "\n"
    "\n"
    "Der Speicher der Wii-Konsole ist belegt.\n"
    "Bitte lies die Wii-Bedienungsanleitung,\n"
    "um weitere Informationen zu erhalten.",

    "\n"
    "\n"
    "\n"
    "Il n'y a pas assez d'espace libre dans\n"
    "la m\xE9""moire de la console Wii. Veuillez\n"
    "vous r\xE9""f\xE9""rer au mode d'emploi Wii pour\n"
    "plus de d\xE9""tails.",

    "\n"
    "\n"
    "\n"
    "No queda espacio libre en la memoria de\n"
    "la consola Wii. Consulta el manual de\n"
    "instrucciones de la consola Wii para\n"
    "obtener m\xE1""s informaci\xF3""n.",

    "\n"
    "\n"
    "\n"
    "Impossibile salvare altri dati nella\n"
    "memoria della console Wii. Per maggiori\n"
    "informazioni, consulta il manuale di\n"
    "istruzioni della console Wii.",

    "\n"
    "\n"
    "\n"
    "Er is geen ruimte meer beschikbaar\n"
    "in het interne geheugen van het\n"
    "Wii-systeem. Lees de handleiding voor\n"
    "meer informatie."
};

static const char* const __NANDErrorMessageMaxFilesChinaKorea[] = {
    "\n"
    "\n"
    "エラーコード４０６。\n"
    "\n"
    "Wii本体保存メモリの空きファイル数が異常です。\n"
    "くわしくはWii本体の取扱説明書をお読み\n"
    "ください。",

    "\n"
    "\n"
    "Error #406,\n"
    "\n"
    "There is no more available space in\n"
    "Wii system memory. Refer to the Wii\n"
    "Operations Manual for details."
};

static const char* const __NANDErrorMessageCorruptDefault[] = {
    "\n"
    "\n"
    "\n"
    "Wii本体保存メモリが壊れました。\n"
    "くわしくはWii本体の取扱説明書をお読み\n"
    "ください。",

    "\n"
    "\n"
    "\n"
    "The Wii system memory has been damaged.\n"
    "Refer to the Wii Operations Manual for\n"
    "details.",

    "\n"
    "\n"
    "\n"
    "Der Speicher der Wii-Konsole\n"
    "ist besch\xE4""digt. Bitte lies die\n"
    "Wii-Bedienungsanleitung, um weitere\n"
    "Informationen zu erhalten.",

    "\n"
    "\n"
    "\n"
    "La m\xE9""moire de la console Wii a \xE9""t\xE9"" \n"
    "endommag\xE9""e. Veuillez vous r\xE9""f\xE9""rer au\n"
    "Mode d'emploi de la Wii pour plus de\n"
    "d\xE9""tails.",

    "\n"
    "\n"
    "\n"
    "La memoria de la consola Wii\n"
    "est\xE1"" da\xF1""ada. Consulta el manual de\n"
    "operaciones de la consola Wii para\n"
    "obtener m\xE1""s informaci\xF3""n.",

    "\n"
    "\n"
    "\n"
    "La memoria della console Wii e\n"
    "danneggiata. Per maggiori informazioni,\n"
    "consulta il manuale di istruzioni della\n"
    "console Wii.",

    "\n"
    "\n"
    "\n"
    "Het interne geheugen van het\n"
    "Wii-systeem is beschadigd. Lees de\n"
    "Wii-handleiding voor meer informatie."
};

static const char* const __NANDErrorMessageCorruptEurope[] = {
    "\n"
    "\n"
    "\n"
    "Wii本体保存メモリが壊れました。\n"
    "くわしくはWii本体の取扱説明書をお読み\n"
    "ください。",

    "\n"
    "\n"
    "\n"
    "The Wii System Memory has been damaged.\n"
    "Please refer to the Wii Operations Manual\n"
    "for details.",

    "\n"
    "\n"
    "\n"
    "Der Speicher der Wii-Konsole\n"
    "ist beschadigt. Bitte lies die\n"
    "Wii-Bedienungsanleitung, um weitere\n"
    "Informationen zu erhalten.",

    "\n"
    "\n"
    "\n"
    "La m\xE9""moire de la console Wii est\n"
    "endommag\xE9""e. Veuillez vous r\xE9""f\xE9""rer au\n"
    "mode d'emploi Wii pour plus de d\xE9""tails.\n"
    "",

    "\n"
    "\n"
    "\n"
    "La memoria de la consola Wii est\xE1"" da\xF1""ada.\n"
    "Consulta el manual de instrucciones de la\n"
    "consola Wii para obtener m\xE1""s informaci\xF3""n.",

    "\n"
    "\n"
    "\n"
    "La memoria della console Wii e\n"
    "danneggiata. Per maggiori informazioni,\n"
    "consulta il manuale di istruzioni della\n"
    "console Wii.",

    "\n"
    "\n"
    "\n"
    "Het interne geheugen van het\n"
    "Wii-systeem is beschadigd. Lees de\n"
    "Wii-handleiding voor meer informatie."
};

static const char* const __NANDErrorMessageCorruptChinaKorea[] = {
    "\n"
    "\n"
    "エラーコード４０８。\n"
    "\n"
    "Wii本体保存メモリが壊れました。\n"
    "くわしくはWii本体の取扱説明書をお読み\n"
    "ください。",

    "\n"
    "\n"
    "Error #408,\n"
    "\n"
    "The Wii system memory has been damaged.\n"
    "Refer to the Wii Operations Manual for\n"
    "details."
};

static const char* const __NANDErrorMessageBusyDefault[] = {
    "\n"
    "\n"
    "\n"
    "Wii本体保存メモリの書き込み/読み出しが\n"
    "できませんでした。\n"
    "くわしくはWii本体の取扱説明書をお読み\n"
    "ください。",

    "\n"
    "\n"
    "\n"
    "Could not access Wii system memory.\n"
    "Refer to the Wii Operations Manual for\n"
    "details.",

    "\n"
    "\n"
    "\n"
    "Auf den Speicher der Wii-Konsole konnte\n"
    "nicht zugegriffen werden. Bitte lies die\n"
    "Wii-Bedienungsanleitung, um weitere\n"
    "Informationen zu erhalten.",

    "\n"
    "\n"
    "\n"
    "Impossible d'acc\xE9""der \xE0"" la m\xE9""moire de\n"
    "la console Wii. Veuillez vous r\xE9""f\xE9""rer\n"
    "au Mode d'emploi de la Wii pour plus\n"
    "de d\xE9""tails.",

    "\n"
    "\n"
    "\n"
    "No se ha podido acceder a la memoria de\n"
    "la consola Wii. Consulta el manual de\n"
    "operaciones de la consola Wii para\n"
    "obtener m\xE1""s informaci\xF3""n.",

    "\n"
    "\n"
    "\n"
    "Impossibile accedere alla memoria della\n"
    "console Wii. Per maggiori informazioni,\n"
    "consulta il manuale di istruzioni della\n"
    "console Wii.",

    "\n"
    "\n"
    "\n"
    "Het interne geheugen van het Wii-systeem\n"
    "kan niet worden gelezen of beschreven.\n"
    "Lees de Wii-handleiding voor meer\n"
    "informatie."
};

static const char* const __NANDErrorMessageBusyEurope[] = {
    "\n"
    "\n"
    "\n"
    "Wii本体保存メモリの書き込み/読み出しが\n"
    "できませんでした。\n"
    "くわしくはWii本体の取扱説明書をお読み\n"
    "ください。",

    "\n"
    "\n"
    "\n"
    "Could not access the Wii System Memory.Please refer to the Wii Operations Manual\n"
    "for details.",

    "\n"
    "\n"
    "\n"
    "Auf den Speicher der Wii-Konsole konnte\n"
    "nicht zugegriffen werden. Bitte lies die\n"
    "Wii-Bedienungsanleitung, um weitere\n"
    "Informationen zu erhalten.",

    "\n"
    "\n"
    "\n"
    "Impossible d'acc\xE9""der \xE0"" la m\xE9""moire de la\n"
    "console Wii. Veuillez vous r\xE9""f\xE9""rer au\n"
    "mode d'emploi Wii pour plus de d\xE9""tails.",

    "\n"
    "\n"
    "\n"
    "No se ha podido acceder a la memoria de\n"
    "la consola Wii. Consulta el manual de\n"
    "instrucciones de la consola Wii para\n"
    "obtener m\xE1""s informaci\xF3""n.",

    "\n"
    "\n"
    "\n"
    "Impossibile accedere alla memoria della\n"
    "console Wii. Per maggiori informazioni,\n"
    "consulta il manuale di istruzioni della\n"
    "console Wii.",

    "\n"
    "\n"
    "\n"
    "Het interne geheugen van het Wii-systeem\n"
    "kan niet worden gelezen of beschreven.\n"
    "Lees de Wii-handleiding voor meer\n"
    "informatie."
};

static const char* const __NANDErrorMessageBusyChinaKorea[] = {
    "\n"
    "\n"
    "エラーコード４１１。\n"
    "\n"
    "Wii本体保存メモリの書き込み/読み出しが\n"
    "できませんでした。\n"
    "くわしくはWii本体の取扱説明書をお読み\n"
    "ください。",

    "\n"
    "\n"
    "Error #411,\n"
    "\n"
    "Could not access Wii system memory.\n"
    "Refer to the Wii Operations Manual for\n"
    "details."
};

static const char* const __NANDErrorMessageUnknownDefault[] = {
    "\n"
    "\n"
    "\n"
    "Wii本体保存メモリの書き込み/読み出し中に\n"
    "エラーが発生しました。\n"
    "くわしくはWii本体の取扱説明書をお読み\n"
    "ください。",

    "\n"
    "\n"
    "\n"
    "An error occurred while accessing Wii\n"
    "system memory. Refer to the Wii\n"
    "Operations Manual for details.",

    "\n"
    "\n"
    "\n"
    "Beim Zugriff auf den Speicher der\n"
    "Wii-Konsole ist ein Fehler aufgetreten.\n"
    "Bitte lies die Wii-Bedienungsanleitung,\n"
    "um weitere Informationen zu erhalten.",

    "\n"
    "\n"
    "\n"
    "Une erreur est survenue pendant le\n"
    "processus de lecture ou d'\xE9""criture\n"
    "dans la m\xE9""moire de la console Wii.\n"
    "Veuillez vous r\xE9""f\xE9""rer au Mode d'emploi\n"
    "de la Wii pour plus de d\xE9""tails.",

    "\n"
    "\n"
    "\n"
    "Se ha producido un error al intentar\n"
    "acceder a la memoria de la consola Wii.\n"
    "Consulta el manual de operaciones\n"
    "de la consola Wii para obtener m\xE1""s\n"
    "informaci\xF3""n.",

    "\n"
    "\n"
    "\n"
    "Si \xE8"" verificato un errore durante la\n"
    "lettura o la modifica dei dati\n"
    "all'interno della memoria della\n"
    "console Wii. Per maggiori informazioni,\n"
    "consulta il manuale di istruzioni della\n"
    "console Wii.",

    "\n"
    "\n"
    "\n"
    "Er is een fout opgetreden tijdens het\n"
    "lezen of beschrijven van het interne\n"
    "geheugen van het Wii-systeem. Lees de\n"
    "Wii-handleiding voor meer informatie."
};

static const char* const __NANDErrorMessageUnknownEurope[] = {
    "\n"
    "\n"
    "\n"
    "Wii本体保存メモリの書き込み/読み出し中に\n"
    "エラーが発生しました。\n"
    "くわしくはWii本体の取扱説明書をお読み\n"
    "ください。",

    "\n"
    "\n"
    "\n"
    "An error occurred during the process of\n"
    "reading from or writing to the Wii System\n"
    "Memory. Please refer to the Wii Operations\n"
    "Manual for details.",

    "\n"
    "\n"
    "\n"
    "Beim Zugriff auf den Speicher der\n"
    "Wii-Konsole ist ein Fehler aufgetreten.\n"
    "Bitte lies die Wii-Bedienungsanleitung,\n"
    "um weitere Informationen zu erhalten.",

    "\n"
    "\n"
    "\n"
    "Une erreur est survenue avec la m\xE9""moire\n"
    "de la console Wii pendant le processus\n"
    "de lecture ou d'\xE9""criture. Veuillez vous\n"
    "r\xE9""f\xE9""rer au mode d'emploi Wii pour plus\n"
    "de d\xE8""tails.",

    "\n"
    "\n"
    "\n"
    "Se ha producido un error durante la\n"
    "lectura o escritura de la memoria de\n"
    "la consola Wii. Consulta el manual de\n"
    "instrucciones de la consola Wii para\n"
    "obtener m\xE1""s informaci\xF3""n.",

    "\n"
    "\n"
    "\n"
    "Si \xE8"" verificato un errore durante la\n"
    "lettura o la modifica dei dati\n"
    "all'interno della memoria della\n"
    "console Wii. Per maggiori informazioni,\n"
    "consulta il manuale di istruzioni della\n"
    "console Wii.",

    "\n"
    "\n"
    "\n"
    "Er is een fout opgetreden tijdens het\n"
    "lezen of beschrijven van het interne\n"
    "geheugen van het Wii-systeem. Lees de\n"
    "Wii-handleiding voor meer informatie."
};

static const char* const __NANDErrorMessageUnknownChinaKorea[] = {
    "\n"
    "\n"
    "エラーコード４１２。\n"
    "\n"
    "Wii本体保存メモリの書き込み/読み出し中に\n"
    "エラーが発生しました。\n"
    "くわしくはWii本体の取扱説明書をお読み\n"
    "ください。",

    "\n"
    "\n"
    "Error #412,\n"
    "\n"
    "An error occurred while accessing Wii\n"
    "system memory. Refer to the Wii\n"
    "Operations Manual for details."
};

typedef struct NANDErrorMessageEntry_unk {
    s32 code;
    s32 region;
    const char* const* messages;
} NANDErrorMessageEntry_unk;

static const char* const* nandGetErrorMessages(s32 code, s32 region) {
    u8 i = 0;
    const NANDErrorMessageEntry_unk table[21] = {
        {ISFS_ERROR_MAXBLOCKS, 0, __NANDErrorMessageMaxBlocksDefault},
        {ISFS_ERROR_MAXFILES, 0, __NANDErrorMessageMaxFilesDefault},
        {ISFS_ERROR_CORRUPT, 0, __NANDErrorMessageCorruptDefault},
        {ISFS_ERROR_BUSY, 0, __NANDErrorMessageBusyDefault},
        {IOS_ERROR_FAIL_ALLOC, 0, __NANDErrorMessageBusyDefault},
        {ISFS_ERROR_UNKNOWN, 0, __NANDErrorMessageUnknownDefault},
        {IOS_ERROR_UNKNOWN, 0, __NANDErrorMessageUnknownDefault},
        {ISFS_ERROR_MAXBLOCKS, 1, __NANDErrorMessageMaxBlocksEurope},
        {ISFS_ERROR_MAXFILES, 1, __NANDErrorMessageMaxFilesEurope},
        {ISFS_ERROR_CORRUPT, 1, __NANDErrorMessageCorruptEurope},
        {ISFS_ERROR_BUSY, 1, __NANDErrorMessageBusyEurope},
        {IOS_ERROR_FAIL_ALLOC, 1, __NANDErrorMessageBusyEurope},
        {ISFS_ERROR_UNKNOWN, 1, __NANDErrorMessageUnknownEurope},
        {IOS_ERROR_UNKNOWN, 1, __NANDErrorMessageUnknownEurope},
        {ISFS_ERROR_MAXBLOCKS, 2, __NANDErrorMessageMaxBlocksChinaKorea},
        {ISFS_ERROR_MAXFILES, 2, __NANDErrorMessageMaxFilesChinaKorea},
        {ISFS_ERROR_CORRUPT, 2, __NANDErrorMessageCorruptChinaKorea},
        {ISFS_ERROR_BUSY, 2, __NANDErrorMessageBusyChinaKorea},
        {IOS_ERROR_FAIL_ALLOC, 2, __NANDErrorMessageBusyChinaKorea},
        {ISFS_ERROR_UNKNOWN, 2, __NANDErrorMessageUnknownChinaKorea},
        {IOS_ERROR_UNKNOWN, 2, __NANDErrorMessageUnknownChinaKorea},
    };

    while (i < 21) {
        if (table[i].region == region) {
            if (table[i].code == code) {
                return table[i].messages;
            }

            i++;
        }
        else {
            i += (u8)7; // next region (7 entries each)
        }
    }

    return __NANDErrorMessageUnknownDefault;
}

static void __NANDShowErrorMessage(s32 code) {
    const char* message;
    const char* const* messageList;
    GXColor bg = { 0, 0, 0, 0 };
    GXColor fg = { 255, 255, 255, 0 };

    if (SCGetLanguage() == 0) {
        OSSetFontEncode(1);
    }
    else {
        OSSetFontEncode(0);
    }

    switch (SCGetProductGameRegion()) {
        default:
            messageList = nandGetErrorMessages(code, 0);
            break;
        case 2:
            messageList = nandGetErrorMessages(code, 1);
            break;
        case 4:
        case 5:
            messageList = nandGetErrorMessages(code, 2);
            break;
    }

    if (SCGetLanguage() > 6) {
        message = messageList[1];
    }
    else {
        message = messageList[SCGetLanguage()];
    }

    OSFatal(fg, bg, message);
}

BOOL NANDSetAutoErrorMessaging(BOOL enable) {
    BOOL enabled, prev;
    enabled = OSDisableInterrupts();
    prev = NANDErrorFunc ? TRUE : FALSE;
    NANDErrorFunc = enable ? __NANDShowErrorMessage : NULL;
    OSRestoreInterrupts(enabled);
    return prev;
}

void __NANDPrintErrorMessage(s32 code) {
    if (NANDErrorFunc) {
        NANDErrorFunc(code);
    }
}
