#include "SQL.h"

SQL::SQL() {
    // create file if it dont exist
    std::ofstream fout;
    fout.open("tables.txt", std::ios::app);
    fout.close();

    std::ifstream fin;
    fin.open("tables.txt");
    std::string tablename;
    while (fin >> tablename) {
        tables[tablename] = new Table(tablename);
    }
    // std::cout << tables << '\n';
    std::cout << "Found and indexed tables: " << '\n';
    for (auto it = tables.begin(); it != nullptr; ++it) {
        std::cout << it.key() << '\n';
    }
    fin.close();
}

void SQL::interactive() {
    std::string command;
    while (true) {
        std::cout << "sql> ";
        getline(std::cin, command);
        execute_string(command);
    }
}

bool SQL::execute_string(std::string command, bool verbose) {
    try {
        auto parse_tree = parser.parse(command);
        #ifdef DEBUG
            std::cout << parse_tree << '\n';
        #endif
        // select
        if (parse_tree["command"][0] == "select") {
            tables[parse_tree["table_name"][0]]->select(parse_tree["fields"],
                                                        parse_tree["where"]);
            Table temp("temp");
            std::cout << temp << '\n';
        }
        // create
        if (parse_tree["command"][0] == "create" ||
            parse_tree["command"][0] == "make") {
            tables[parse_tree["table_name"][0]] =
                new Table(parse_tree["table_name"][0], parse_tree["fields"]);
            std::ofstream fout;
            fout.open("tables.txt", std::ios::app);
            fout << '\n' << parse_tree["table_name"][0];
            fout.close();
            if (verbose) {
                std::cout << *tables[parse_tree["table_name"][0]] << '\n';
            }
        }
        // insert
        if (parse_tree["command"][0] == "insert") {
            tables[parse_tree["table_name"][0]]->insert_into(
                parse_tree["fields"]);
            if (verbose) {
                std::cout << *tables[parse_tree["table_name"][0]] << '\n';
            }
        }
        return true;
    } catch (std::runtime_error e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}

void SQL::execute_file(std::string filename) {
    std::ifstream fin;
    fin.open(filename);
    std::string cmd;
    for (int i = 1; std::getline(fin, cmd);) {
        if (cmd.size() > 0 && (cmd.substr(0, 2) == "//" || cmd.at(0) == '#')) {
            std::cout << cmd << '\n';
        }
        else if (cmd.size() == 0 || string_util::iswhitespace(cmd)) {
            // do nothing, just skip it
        }
        else {
            std::cout<< i++ << ": " << cmd << '\n';
            execute_string(cmd, true);
        }
    }
    fin.close();
}