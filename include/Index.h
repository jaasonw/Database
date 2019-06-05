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
    friend std::ostream& operator<<(std::ostream& outs, const Index& i) {
        for (auto map_it = i.index.begin(); map_it != nullptr; ++map_it) {
            outs << map_it.key() << '\n';
            for (auto mmap_it = map_it->begin(); mmap_it != nullptr; ++mmap_it) {
                outs << '\t';
                outs << mmap_it.key() << ": " << *mmap_it << '\n';
            }
        }
        return outs;
    }
    MultiMap::MultiMap<std::string, long>& operator[](std::string key) {
        return index[key];
    }
    bool contains(std::string field) { return index.contains(field); }
};