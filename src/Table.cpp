#include "Table.h"

const char* Table::TEMP = "temp";

Table::Table() : name("") {}
Table::Table(const std::string& name) : name(name) {
    // this shouldnt call on a nonexistent table
    if (!bin_io::file_exists(get_filename().c_str()))
        throw std::runtime_error(CANNOT_FIND_TABLE);
    db_read();
    reindex();
}
Table::Table(const std::string& name, const std::vector<std::string>& columns) {
    // the technology isnt there for this yet :)
    if (columns.size() > constants::MAX_BLOCK_ROWS) {
        throw std::runtime_error(MAX_FIELD_EXCEEDED);
    }
    // check field lengths, because the technology isnt here for this yet
    for (size_t i = 0; i < columns.size(); ++i) {
        if (columns[i].size() >= constants::MAX_BLOCK_COLS)
            throw std::runtime_error(MAX_FIELD_SIZE_EXCEEDED + columns[i]);
    }
    this->name = name;
    this->columns = columns;
    init_file();
    db_read();
}

bool Table::db_read() {
    std::fstream file_stream;
    bin_io::open_fileRW(file_stream, get_filename().c_str());
    // if file is not empty
    if (file_stream.peek() != std::ifstream::traits_type::eof()) {
        Record index_block;
        index_block.read(file_stream, 0);
        columns = index_block.to_vector();
    }
    // map the columns to make selecting them easier later on
    for (size_t i = 0; i < columns.size(); ++i) {
        column_map[columns[i]] = i;
    }
    file_stream.close();
    return !file_stream.fail();
}

void Table::insert_into(const std::vector<std::string>& fields) {
    // check field lengths, because the technology isnt here for this yet
    for (size_t i = 0; i < fields.size(); ++i) {
        if (fields[i].size() >= constants::MAX_BLOCK_COLS)
            throw std::runtime_error(MAX_FIELD_SIZE_EXCEEDED + fields[i]);
    }
    if(!bin_io::file_exists(get_filename().c_str()))
        throw std::runtime_error(CANNOT_FIND_TABLE);
    std::fstream f;
    bin_io::open_fileW(f, get_filename().c_str());
    if (fields.size() != columns.size()) {
        throw std::runtime_error(MISMATCHED_COL_NUM);
    }
    Record r;
    r.create_from_vector(fields);
    long record_number = r.append_to_file(f);
    // update the index
    for (size_t i = 0; i < columns.size(); ++i) {
        index[columns[i]][r.buffer[i]] += record_number;
    }
    f.close();
}

Table Table::select(const std::vector<std::string>& fields,
                   const std::vector<std::string>& where) {
    if (!bin_io::file_exists(get_filename().c_str()))
        throw std::runtime_error(CANNOT_FIND_TABLE);
    std::fstream file_stream;
    bin_io::open_fileRW(file_stream, get_filename().c_str());

    // handle where condition
    std::vector<long> where_indices;
    Queue<std::string> where_queue = infix_to_postfix(where);
    where_indices = evaluate_where(where_queue);


    // this might need to be replaced with something else later
    if (fields[0] == "*") {
        Table temp(TEMP, columns);

        Record r;
        if (where_indices.size() > 0 || where.size() > 0) {
            for (size_t i = 0; i < where_indices.size(); ++i) {
                r.read(file_stream, where_indices[i]);
                temp.insert_into(r.to_vector());
            }
        }
        else {
            for (int i = 1; r.read(file_stream, i) > 0; ++i) {
                r.read(file_stream, i);
                temp.insert_into(r.to_vector());
            }
        }
        file_stream.close();
        return temp;
    }
    else {
        for (size_t i = 0; i < fields.size(); ++i) {
            if (!column_map.contains(fields[i])) {
                throw std::runtime_error(INVALID_NAME + fields[i]);
            }
        }
        Table temp(TEMP, fields);

        Record r;
        if (where_indices.size() > 0 || where.size() > 0) {
            for (size_t i = 0; i < where_indices.size(); ++i) {
                std::vector<std::string> temp_row;
                r.read(file_stream, where_indices[i]);
                for (size_t j = 0; j < fields.size(); ++j) {
                    temp_row.push_back(r.buffer[column_map[fields[j]]]);
                }
                temp.insert_into(temp_row);
            }
        }
        else {
            for (long i = 1; r.read(file_stream, i) > 0; ++i) {
                std::vector<std::string> temp_row;
                r.read(file_stream, i);
                for (size_t j = 0; j < fields.size(); ++j) {
                    temp_row.push_back(r.buffer[column_map[fields[j]]]);
                }
                temp.insert_into(temp_row);
            }
        }
        file_stream.close();
        return temp;
    }
    file_stream.close();
    // this shouldn't really ever get here, but return an empty table in case
    // it ever does
    return Table();
}

std::ostream& operator<<(std::ostream& outs, const Table& table) {
    outs << &table;
    return outs;
}

void Table::init_file() {
    remove(get_filename().c_str());
    // write fields to a block
    std::fstream f;
    bin_io::open_fileW(f, get_filename().c_str());
    Record r;
    r.create_from_vector(columns);
    // write block to beginning of the file
    r.write(f, 0);
    f.close();
}

std::string Table::get_filename() const { return (name + ".db"); }

void Table::reindex() {
    std::fstream file_stream;
    file_stream.open(get_filename());
    Record r;
    for (long i = 1; r.read(file_stream, i) > 0; ++i) {
        r.read(file_stream, i);
        auto row = r.to_vector();
        MultiMap::MultiMap<std::string, long> mmap;
        for (size_t j = 0; j < columns.size(); ++j) {
            index[columns[j]].insert(row[j], i);
        }
    }
    file_stream.close();
}

std::vector<long> Table::evaluate_where(Queue<std::string>& where) {
    Stack<std::vector<long>> results;
    while (!where.empty()) {
        if (string_util::uppercase(where.front()) != "AND" &&
            string_util::uppercase(where.front()) != "OR") {

            std::string arg1 = where.pop();
            std::string arg2 = where.pop();
            std::string op = where.pop();
            if (op == "=")
                results.push(get_equal(arg1, arg2));
            else if (op == "<")
                results.push(get_less(arg1, arg2));
            else if (op == "<=")
                results.push(get_less_equal(arg1, arg2));
            else if (op == ">")
                results.push(get_greater(arg1, arg2));
            else if (op == ">=")
                results.push(get_greater_equal(arg1, arg2));
        } else {
            std::string logic = where.pop();
            if (string_util::uppercase(logic) == "AND") {
                results.push(
                    vset::set_intersect(results.pop(), results.pop())
                );
            }
            else if (string_util::uppercase(logic) == "OR") {
                results.push(
                    vset::set_union(results.pop(), results.pop())
                );
            }
        }
    }
    // this should end with only 1 in the queue, if not....we have a problem
    assert(results.size() <= 1);
    if (!results.empty())
        return results.pop();
    else
        return std::vector<long>();
}

std::vector<long> Table::get_greater(const std::string& arg1,
                                     const std::string& arg2) {
    if (!index.contains(arg1))
        throw std::runtime_error(INVALID_NAME + arg1);
    std::vector<long> results;
    for (auto it = index[arg1].upper_bound(arg2); it != nullptr; ++it) {
        for (size_t i = 0; i < it->size(); ++i) {
            results.push_back(it->at(i));
        }
    }
    return results;
}
std::vector<long> Table::get_greater_equal(const std::string& arg1,
                                           const std::string& arg2) {
    if (!index.contains(arg1))
        throw std::runtime_error(INVALID_NAME + arg1);
    std::vector<long> results;
    for (auto it = index[arg1].lower_bound(arg2); it != nullptr; ++it) {
        for (size_t i = 0; i < it->size(); ++i) {
            results.push_back(it->at(i));
        }
    }
    return results;
}
std::vector<long> Table::get_less(const std::string& arg1,
                                  const std::string& arg2) {
    if (!index.contains(arg1))
        throw std::runtime_error(INVALID_NAME + arg1);
    std::vector<long> results;
    MultiMap::MultiMap<std::string, long> _index = index[arg1];
    for (auto it = _index.begin(); it != _index.lower_bound(arg2); ++it) {
        for (size_t i = 0; i < it->size(); ++i) {
            results.push_back(it->at(i));
        }
    }
    return results;
}
std::vector<long> Table::get_less_equal(const std::string& arg1,
                                        const std::string& arg2) {
    if (!index.contains(arg1))
        throw std::runtime_error(INVALID_NAME + arg1);
    std::vector<long> results;
    MultiMap::MultiMap<std::string, long> _index = index[arg1];
    for (auto it = _index.begin(); it != _index.upper_bound(arg2); ++it) {
        for (size_t i = 0; i < it->size(); ++i) {
            results.push_back(it->at(i));
        }
    }
    return results;
}
std::vector<long> Table::get_equal(const std::string& arg1,
                                   const std::string& arg2) {
    if (!index.contains(arg1))
        throw std::runtime_error(INVALID_NAME + arg1);
    std::vector<long> results;
    if (index[arg1].contains(arg2)) {
        for (size_t i = 0; i < index[arg1][arg2].size(); ++i) {
            results.push_back(index[arg1][arg2][i]);
        }
    }
    return results;
}

std::ostream& Table::print_table(std::ostream& outs) {
    const int NUM_WIDTH = 8;
    std::fstream file_stream;
    bin_io::open_fileRW(file_stream, get_filename().c_str());
    outs << std::setw(NUM_WIDTH) << std::left << "Num";
    for (auto e : columns) {
        outs << std::setw(constants::MAX_BLOCK_COLS) << std::left;
        outs << e;
    }
    outs << '\n';
    outs << std::string(columns.size() * constants::MAX_BLOCK_COLS, '-');
    outs << '\n';

    Record r;
    // sort by first column
    MultiMap::MultiMap<std::string, long> sorted = index[columns[0]];
    int num = 1;
    for (auto it = sorted.begin(); it != nullptr; ++it) {
        for (size_t i = 0; i < it->size(); ++i, ++num) {
            outs << std::setw(NUM_WIDTH) << std::left << num;
            r.read(file_stream, it->at(i));
            auto row = r.to_vector();
            for (size_t j = 0; j < row.size(); ++j) {
                outs << std::setw(constants::MAX_BLOCK_COLS) << std::left;
                outs << row[j];
            }
            outs << '\n';
        }
    }
    file_stream.close();
    return outs;
}