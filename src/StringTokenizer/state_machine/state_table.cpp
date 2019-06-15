#include "StringTokenizer/state_machine/state_table.h"
#include <iostream>

namespace state_machine {
    // Fill all cells of the array with -1
    void init_table(int _table[][MAX_COLUMNS]) {
        for (int i = 0; i < NUM_ROWS; i++) {
            for (int j = 0; j < MAX_COLUMNS; j++) {
                _table[i][j] = -1;
            }
        }
    }
    
    // Mark this state (row) with a 1 (success)
    void mark_success(int _table[][MAX_COLUMNS], int state) {
        _table[state][0] = 1;
    }
    
    // Mark this state (row) with a 0 (fail)
    void mark_fail(int _table[][MAX_COLUMNS], int state) {
        _table[state][0] = 0;
    }

    // true if state is a success state
    bool is_success(int _table[][MAX_COLUMNS], int state) {
        return _table[state][0] == 1;
    }

    // Mark a range of cells in the array.
    void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state) {
        for (int i = from; i <= to; i++) {
            _table[row][i] = state;
        }
    }

    // Mark columns represented by the string columns[] for this row
    void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state) {
        for (int i = 0; columns[i] != '\0'; i++) {
            _table[row][static_cast<int>(columns[i])] = state;
        }
    }

    // Mark this row and column
    void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state) {
        _table[row][column] = state;
    }

    // Mark all in row
    void mark_all(int row, int _table[][MAX_COLUMNS], int state) {
        for (int i = 1; i < MAX_COLUMNS; ++i) {
            _table[row][i] = state;
        }
    }

    void unmark_cell(int row, int _table[][MAX_COLUMNS], int column) {
        _table[row][column] = -1;
    }

    // This can realistically be used on a small table
    void print_table(int _table[][MAX_COLUMNS]) {
        for (int i = 0; i < NUM_ROWS; i++) {
            for (int j = 0; j < MAX_COLUMNS; j++) {
                std::cout << _table[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    // show string s and mark this position on the string:
    // hello world   pos: 7
    //       ^
    void show_string(const char* s, int _pos) {
        std::cout << s << std::endl;
        for (int i = 0; i < _pos; i++) {
            std::cout << " ";
        }
        std::cout << '^' << std::endl;
        std::cout << "pos: " << _pos;
    }

    // returns true if a valid token was found and moves the position to the
    // the end of the last valid token
    bool get_token(const char* str, size_t& pos, int table[][MAX_COLUMNS], int& state) {
        int _pos = pos;
        bool token_found = false;
        while (str[_pos] != '\0' && state != -1) {
            state = table[state][static_cast<int>(str[_pos])];
            if (is_success(table, state)) {
                pos = _pos;
                token_found = true;
            }
            ++_pos;
        }
        return token_found;
    }
}