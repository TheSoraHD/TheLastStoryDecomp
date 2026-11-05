#ifndef GAME_MAPPARTSFACTORY_H
#define GAME_MAPPARTSFACTORY_H

class MapPartsBase;
namespace atn {
class Task;
class CsvTab;
}

// inferred: map part factory (game/MapPartsFactory.cpp, free functions; used by MapTask::CreateParts)
extern "C" int fn_80496A28(const char* name); // inferred: type of a part from its csv name (-1 if unknown)
extern "C" MapPartsBase* fn_80496B3C(atn::Task* parent, const char* name, atn::CsvTab* csv); // inferred: creates it

#endif
