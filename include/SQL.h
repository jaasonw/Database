#pragma once
#include "Parser/Parser.h"
#include "Table.h"
#include <iostream>
#include <string>

class SQL {
private:
    Parser parser;
    // um, add in the big 3 for this later, not that i'll ever need it
    Map::Map<std::string, Table*> tables;
public:
    SQL();
    void interactive();
    void execute_file(std::string filename);
    void execute_string(std::string str);
};