/*
 * Author: Jason Wong
 * Project: Database
 * Purpose: Implement a small version of a SQL Relational Database
 * Notes:
 */
#include "SQL.h"
#include <iostream>

using namespace std;

int main(int argc, char const* argv[]) {
    SQL sql;
    if (argc > 1) {
        sql.execute_file(argv[1]);
    }
    sql.interactive();
    
    cout << endl;
    system("pause");
    return 0;
}