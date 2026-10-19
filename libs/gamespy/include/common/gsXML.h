// GameSpy SDK XML writer accessors (see libs/gamespy/license.txt).
#ifndef __GSXML_H__
#define __GSXML_H__

#include "common/gsCommon.h"

typedef void* GSXmlStreamWriter;

const char* gsXmlWriterGetData(GSXmlStreamWriter stream);
int gsXmlWriterGetDataLength(GSXmlStreamWriter stream);

#endif
