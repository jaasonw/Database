cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
project(database)

include_directories(include)
set (SOURCES
    "src/Index.cpp"
    "src/Parser/Parser.cpp"
    "src/Parser/SQLStateMachine.cpp"
    "src/rpn/shunting_yard.cpp"
    "src/SQL.cpp"
    "src/StringTokenizer"
    "src/StringTokenizer/state_machine"
    "src/StringTokenizer/state_machine/state_table.cpp"
    "src/StringTokenizer/STokenizer.cpp"
    "src/StringTokenizer/Token.cpp"
    "src/Table.cpp"
    "src/util/io/binary_util.cpp"
    "src/util/io/Record.cpp"
    "src/util/level.cpp"
    "src/util/string_util.cpp"
    "src/Parser/Parser.cpp"
    "src/Parser/SQLStateMachine.cpp"
    "src/rpn/shunting_yard.cpp"
    "src/StringTokenizer/state_machine"
    "src/StringTokenizer/state_machine/state_table.cpp"
    "src/StringTokenizer/STokenizer.cpp"
    "src/StringTokenizer/Token.cpp"
    "src/StringTokenizer/state_machine/state_table.cpp"
    "src/util/io/binary_util.cpp"
    "src/util/io/Record.cpp"
    "src/util/level.cpp"
    "src/util/string_util.cpp"
    "src/util/io/binary_util.cpp"
    "src/util/io/Record.cpp"
)

add_executable(database main.cpp ${SOURCES})