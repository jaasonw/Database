#pragma once
#include "Map/Map.h"
#include "MultiMap/MultiMap.h"
#include <iostream>
#include <string>

// this class pretty much just exists for the sole purpose of printing the index
// and making the map typename look a bit nicer I guess
class Index {
private:
    Map::Map<std::string, MultiMap::MultiMap<std::string, long>> index;
public:
    friend std::ostream& operator<<(std::ostream& outs, const Index& i);
    MultiMap::MultiMap<std::string, long>& operator[](std::string key);
    bool contains(std::string field);
};