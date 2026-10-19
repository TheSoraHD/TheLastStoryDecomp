// GameSpy SDK gsXML.c: only the writer accessors survive in this game
// (see libs/gamespy/license.txt).
#include "common/gsXML.h"

typedef struct GSIXmlStreamWriter {
    char* mBuffer;   // 0x0
    int mLen;        // 0x4
    int mCapacity;   // 0x8
    gsi_bool mClosed; // 0xC
} GSIXmlStreamWriter;

const char* gsXmlWriterGetData(GSXmlStreamWriter stream) {
    GSIXmlStreamWriter* writer = (GSIXmlStreamWriter*)stream;
    return writer->mBuffer;
}

int gsXmlWriterGetDataLength(GSXmlStreamWriter stream) {
    GSIXmlStreamWriter* writer = (GSIXmlStreamWriter*)stream;
    return writer->mLen;
}
