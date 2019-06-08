#include "SQL.h"

SQL::SQL() {
    print_welcome();
    get_tables();
}
SQL::SQL(const SQL&) {
    print_welcome();
    get_tables();
}

SQL::~SQL() {
    for (auto it = tables.begin(); it != nullptr; ++it) {
        delete *it;
    }
}

SQL& SQL::operator=(const SQL& other) {
    if (this == &other)
        return *this;
    for (auto it = tables.begin(); it != nullptr; ++it) {
        delete *it;
    }
    get_tables();
    // print_table_list();
    return *this;
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
            if (!tables.contains(table_name)) {
                Table invalid_table(table_name);
            }
            else {
                tables[table_name]->select(fields, parse_tree["where"]);
                Table temp("temp");
                std::cout << temp << '\n';
            }
        }
        // create
        if (command == "CREATE" || command == "MAKE") {
            if (tables.contains(table_name)) {
                delete tables[table_name];
                tables[table_name] = nullptr;
            }
            if (!tables.contains(table_name)) {
                std::ofstream fout;
                fout.open(TABLES_FILE, std::ios::app);
                fout << '\n' << table_name;
                fout.close();
            }
            tables[table_name] = new Table(table_name, fields);
            if (verbose) {
                std::cout << *tables[table_name] << '\n';
            }
        }
        // insert
        if (command == "INSERT") {
            if (!tables.contains(table_name)) {
                Table invalid_table(table_name);
            }
            else {
                tables[table_name]->insert_into(fields);
                if (verbose) {
                    std::cout << *tables[table_name] << '\n';
                }
            }
        }
        // drop
        if (command == "DROP") {
            if (tables.contains(table_name)) {
                remove(tables[table_name]->get_filename().c_str());
                delete tables[table_name];
                tables[table_name] = nullptr;
                tables.erase(table_name);
                std::ofstream fout;
                fout.open(TABLES_FILE);
                for (auto it = tables.begin(); it != nullptr; ++it) {
                    fout << it.key() << '\n';
                }
                fout.close();
            }
            else {
                Table invalid_table(table_name);
            }
        }
        if (command == "TABLES") {
            print_table_list();
        }
        return true;
    } catch (std::runtime_error& e) {
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

void SQL::get_tables() {
    // create file if it dont exist
    std::ofstream fout;
    fout.open(TABLES_FILE, std::ios::app);
    fout.close();

    std::ifstream fin;
    fin.open(TABLES_FILE);
    std::string tablename;
    while (fin >> tablename) {
        tables[tablename] = new Table(tablename);
    }
    fin.close();
}
void SQL::print_table_list() {
    int num = 1;
    for (auto it = tables.begin(); it != nullptr; ++it, ++num) {
        std::cout << std::setw(20) << std::left << it.key();
        if (num > 0 && num % 4 == 0) {
            std::cout << '\n';
        }
    }
    std::cout << '\n';
}
void SQL::print_welcome() {
    std::cout << "JasonSQL version 1.0.0" << '\n';
    std::cout << "\"tables\" for table list" << '\n';
}