#include "Table.h"

Table::Table(std::string name) : name(name) {
    db_read();
}
Table::Table(std::string name, const std::vector<std::string>& columns) {
    // the technology isnt there for this yet :)
    if (columns.size() > constants::MAX_BLOCK_ROWS) {
        throw std::runtime_error("Error: Max number of fields exceeded");
    }
    this->name = name;
    this->columns = columns;
    init_file();
    db_read();
}

bool Table::db_read() {
    std::fstream file_stream;
    bin_io::open_fileRW(file_stream, get_filename());
    // if file is not empty
    if (file_stream.peek() != std::ifstream::traits_type::eof()) {
        Record index_block;
        index_block.read(file_stream, 0);
        columns = index_block.to_vector();
    }
    // if the file is empty, probably means we're creating a new db
    // so add the fields
    open = !file_stream.fail();
    file_stream.close();
    return open;
}

void Table::insert_into(const std::vector<std::string>& fields) {
    if(!bin_io::file_exists(get_filename()))
        throw std::runtime_error("Error: cannot find specified table");
    std::fstream f;
    bin_io::open_fileW(f, get_filename());
    if (fields.size() != columns.size()) {
        throw std::runtime_error("Error: mismatched column number");
    }
    Record r;
    r.create_from_vector(fields);
    r.append_to_file(f);
    f.close();
}

void Table::select(const std::vector<std::string>& fields,
                   const std::vector<std::string>& where) {
    std::fstream file_stream;
    bin_io::open_fileRW(file_stream, get_filename());
    // this might need to be replaced with something else later
    if (fields[0] == "*") {
        Table temp("temp", columns);

        Record r;
        for (int i = 1; r.read(file_stream, i) > 0; ++i) {
            r.read(file_stream, i);
            temp.insert_into(r.to_vector());
        }
    }
    file_stream.close();
}

std::ostream& operator<<(std::ostream& outs, Table& table) {
    const int SPACING = 20;
    std::fstream file_stream;
    bin_io::open_fileRW(file_stream, table.get_filename());
    for (auto e : table.columns) {
        outs << std::setw(SPACING) << std::left;
        outs << e;
    }
    outs << '\n';
    outs << std::string(80, '-');
    outs << '\n';

    Record r;
    for (int i = 1; r.read(file_stream, i) > 0; ++i) {
        r.read(file_stream, i);
        auto row = r.to_vector();
        for (int j = 0; j < row.size(); ++j) {
            outs << std::setw(SPACING) << std::left;
            outs << row[j];
        }
        outs << '\n';
    }
    file_stream.close();
    return outs;
}

void Table::init_file() {
    remove(get_filename());
    // write fields to a block
    std::fstream f;
    bin_io::open_fileW(f, get_filename());
    Record r;
    r.create_from_vector(columns);
    // write block to beginning of the file
    r.write(f, 0);
    f.close();
}

const char* Table::get_filename() { return (name + ".db").c_str(); }