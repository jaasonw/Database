#include "SQL.h"

SQL::SQL() {
    print_welcome();
    get_tables();
    if (tables.contains(Table::TEMP))
        execute_string("DROP TABLE " + std::string(Table::TEMP));
}

void SQL::interactive() {
    std::string command;
    while (true) {
        std::cout << "sql> ";
        getline(std::cin, command);
        execute_string(command);
    }
}

bool SQL::execute_string(const std::string& command, bool verbose) {
    try {
        auto parse_tree = parser.parse(command);
        #ifdef DEBUG
            std::cout << parse_tree << '\n';
        #endif

        std::string command;
        std::string table_name;
        std::vector<std::string> fields;
        if (parse_tree["command"].size() > 0)
            command = parse_tree["command"][0];
        if (parse_tree["table_name"].size() > 0)
            table_name = parse_tree["table_name"][0];
        if (parse_tree["fields"].size() > 0)
            fields = parse_tree["fields"];
        // select
        if (command == "SELECT") {
            if (!tables.contains(table_name) || table_name == Table::TEMP) {
                throw std::runtime_error(UNKNOWN_TABLE);
            } else {
                tables[Table::TEMP] =
                    tables[table_name].select(fields, parse_tree["where"]);
                tables[Table::TEMP].print_table() << '\n';
            }
        }
        // create
        else if (command == "CREATE" || command == "MAKE") {
            if (table_name == Table::TEMP) {
                throw std::runtime_error(ILLEGAL_NAME);
            }
            if (!tables.contains(table_name)) {
                std::ofstream fout;
                fout.open(TABLES_FILE, std::ios::app);
                fout << '\n' << table_name;
                fout.close();
            }
            tables[table_name] = Table(table_name, fields);
            if (verbose) {
                tables[table_name].print_table() << '\n';
            }
        }
        // insert
        else if (command == "INSERT") {
            if (!tables.contains(table_name)) {
                throw std::runtime_error(UNKNOWN_TABLE);
            }
            else {
                tables[table_name].insert_into(fields);
                if (verbose) {
                    tables[table_name].print_table() << '\n';
                }
            }
        }
        // drop
        else if (command == "DROP") {
            if (tables.contains(table_name)) {
                remove(tables[table_name].get_filename().c_str());
                tables.erase(table_name);
                std::ofstream fout;
                fout.open(TABLES_FILE);
                for (auto it = tables.begin(); it != nullptr; ++it) {
                    fout << it.key() << '\n';
                }
                fout.close();
            }
            else {
                throw std::runtime_error(UNKNOWN_TABLE);
            }
        }
        // print tables command
        else if (command == "TABLES") {
            print_table_list();
        }
        // exits the program
        else if (command == "EXIT") {
            if (tables.contains(Table::TEMP))
                execute_string("DROP TABLE " + std::string(Table::TEMP));
            exit(0);
        }
        // executes a given file
        else if (command == "EXEC") {
            std::string filename = "";
            for (size_t i = 0; i < fields.size(); ++i) {
                if (fields[i] != "\"")
                    filename += fields[i];
            }
            execute_file(filename);
        }
        return true;
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}

void SQL::execute_file(const std::string& filename) {
    std::ifstream fin;
    fin.open(filename);
    std::string cmd;
    for (int i = 0; std::getline(fin, cmd);) {
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

void SQL::get_tables() {
    // create file if it dont exist
    std::ofstream fout;
    fout.open(TABLES_FILE, std::ios::app);
    fout.close();

    std::ifstream fin;
    fin.open(TABLES_FILE);
    std::string tablename;
    while (fin >> tablename) {
        tables[tablename] = Table(tablename);
    }
    fin.close();
}
void SQL::print_table_list() {
    int num = 1;
    for (auto it = tables.begin(); it != nullptr; ++it, ++num) {
        if (it.key() != "temp")
        std::cout << std::setw(20) << std::left << it.key();
        if (num > 0 && num % 4 == 0) {
            std::cout << '\n';
        }
    }
    std::cout << '\n';
}
void SQL::print_welcome() {
    std::cout << "JasonSQL version 1.0.1" << '\n';
    std::cout << "\"tables\" for table list" << '\n';
}