#include "SQL.h"

SQL::SQL() {
    get_tables();
    print_table_list();
}
SQL::SQL(const SQL&) {
    get_tables();
    print_table_list();
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
    print_table_list();
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
        // select
        if (parse_tree["command"][0] == "select") {
            std::string table_name = parse_tree["table_name"][0];
            if (!tables.contains(table_name)) {
                Table invalid_table(table_name);
            }
            tables[table_name]->select(parse_tree["fields"],
                                       parse_tree["where"]);
            Table temp("temp");
            std::cout << temp << '\n';
        }
        // create
        if (parse_tree["command"][0] == "create" ||
            parse_tree["command"][0] == "make") {
            std::string table_name = parse_tree["table_name"][0];
            if (tables.contains(table_name)) {
                delete tables[table_name];
                tables[table_name] = nullptr;
            }
            if (!tables.contains(table_name)) {
                std::ofstream fout;
                fout.open("tables.txt", std::ios::app);
                fout << '\n' << table_name;
                fout.close();
            }
            tables[table_name] = new Table(table_name, parse_tree["fields"]);
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

void SQL::get_tables() {
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
    fin.close();
}
void SQL::print_table_list() {
    std::cout << "Found and indexed tables: " << '\n';
    int num = 1;
    for (auto it = tables.begin(); it != nullptr; ++it, ++num) {
        std::cout << std::setw(20) << std::left << it.key();
        if (num > 0 && num % 4 == 0) {
            std::cout << '\n';
        }
    }
    std::cout << '\n';
}