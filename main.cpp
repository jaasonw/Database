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
    // Parser p("");
    SQL sql;
    // Table t("students");
    sql.interactive();
    // cout << t; 
    cout << endl;
    system("pause");
    return 0;
}