/*
 * Author: Jason Wong
 * Project:
 * Purpose:
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
    // this is the end of the session, remove the temp file
    remove("temp.db");

    cout << endl;
    system("pause");
    return 0;
}