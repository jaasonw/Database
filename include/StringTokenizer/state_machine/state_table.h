#pragma once
// Utilities for creating the a state table for the state machine

namespace state_machine {
    const int MAX_COLUMNS = 255;
    const int NUM_ROWS = 30;

    // Fill all cells of the array with -1
    void init_table(int _table[][MAX_COLUMNS]);

    // Mark this state (row) with a 1 (success)
    void mark_success(int _table[][MAX_COLUMNS], int state);

    // Mark this state (row) with a 0 (fail)
    void mark_fail(int _table[][MAX_COLUMNS], int state);

    // true if state is a success state
    bool is_success(int _table[][MAX_COLUMNS], int state);

    // Mark a range of cells in the array.
    void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state);

    // Mark columns represented by the string columns[] for this row
    void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state);

    // Mark this row and column
    void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state);

    // This can realistically be used on a small table
    void print_table(int _table[][MAX_COLUMNS]);

    // show string s and mark this position on the string:
    // hello world   pos: 7
    //       ^
    void show_string(const char* s, int _pos);

    // returns true if a valid token was found, sets the pos to the last valid
    // 
    bool get_token(const char* str, int& pos, int table[][MAX_COLUMNS], int& state);
}