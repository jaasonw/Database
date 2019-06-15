# SQL Database

A miniature relational database system written from scratch in C++

## Dependencies

None, All data structures (except vector) are written from scratch and doesn't use any external libraries, so this should build on just about any platform and any compiler (tested on g++ and clang) supporting the C++11 standard while producing a fairly lightweight executable 

## Building

I've included a python build script that invokes the either the g++ or clang compiler. 

Use ```python build.py``` to run it

Or just download from releases

## Usage

Because this isn't a fully featured SQL Database, it only supports the following commands

| Command |
| :---- |
| `SELECT <fields> FROM <table> WHERE <conditions>` |
| `INSERT INTO <table> VALUES(val1, val2, ...)` |
| `CREATE TABLE <table>(col1, col2, ...)` |
| `DROP TABLE <table>` |


In addition to the SQL subset, a few auxilary commands are also supported

| Command | Action
| :---- | :----
| `tables` | Displays a list of the available tables
| `exec <file>` | Opens a executes a file containing SQL commands
| `exit` | Closes the session
