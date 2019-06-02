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
    file_stream.close();
    return !file_stream.fail();
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
    else {
        std::vector<int> column_indices;
        // really bad search algorithm but lazy and should work for small sets
        for (size_t i = 0; i < fields.size(); i++) {
            bool found = false;
            for (int j = 0; j < columns.size(); j++) {
                if (columns[j] == fields[i]) {
                    column_indices.push_back(j);
                    found = true;
                    break;
                }
            }
            if (!found)
                throw std::runtime_error("Error: Invalid name: " + fields[i]);
        }
        Table temp("temp", fields);

        Record r;
        for (long i = 1; r.read(file_stream, i) > 0; ++i) {
            r.read(file_stream, i);
            std::vector<std::string> temp_row;

            for (size_t j = 0; j < column_indices.size(); ++j) {
                temp_row.push_back(r.buffer[column_indices[j]]);
            }
            temp.insert_into(temp_row);
        }

    }
    file_stream.close();
}

std::ostream& operator<<(std::ostream& outs, Table& table) {
    std::fstream file_stream;
    bin_io::open_fileRW(file_stream, table.get_filename());
    for (auto e : table.columns) {
        outs << std::setw(constants::MAX_BLOCK_COLS) << std::left;
        outs << e;
    }
    outs << '\n';
    outs << std::string(table.columns.size() * constants::MAX_BLOCK_COLS, '-');
    outs << '\n';

    Record r;
    for (long i = 1; r.read(file_stream, i) > 0; ++i) {
        r.read(file_stream, i);
        auto row = r.to_vector();
        for (size_t j = 0; j < row.size(); ++j) {
            outs << std::setw(constants::MAX_BLOCK_COLS) << std::left;
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