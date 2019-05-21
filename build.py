# a simple build script suitable for compiling small projects
# written by jason wong

import os
import sys
import time

COMPILER = "g++"
EXECUTABLE = "a.exe"
MAIN = "main.cpp"

if (len(sys.argv) > 1):
    if sys.argv[1] == "map_interactive":
        EXECUTABLE = "map_interactive.exe"
        MAIN = "tests/map_interactive.cpp"

    elif sys.argv[1] == "btree_interactive":
        EXECUTABLE = "btree_interactive.exe"
        MAIN = "tests/btree_interactive.cpp"

    elif sys.argv[1] == "bplus_test":
        EXECUTABLE = "bplus_test.exe"
        MAIN = "tests/bplus_test.cpp"


# includes and sources should search recursively
INCLUDES = [
    "include",
]

SOURCES = [
    "src"
]
LINK = [

]
FLAGS = [
    "-std=c++11",
    "-Wall",
    "-Wextra",
    "-g"
]

# add clang specific flags here
if COMPILER == "clang++":
    pass

flag_str = ""
for f in FLAGS:
    flag_str += f + " "


include_str = ""

for top_level_directory in INCLUDES:
    include_str += f"-I\"{top_level_directory}/\" "
    for root, directories, filenames in os.walk(top_level_directory):
        for directory in directories:
            include_str += f"-I\"{os.path.join(root, directory)}/\" "

sources_str = ""

for top_level_directory in SOURCES:
    for root, directories, filenames in os.walk(top_level_directory):
        for filename in filenames:
            sources_str += f" \"{os.path.join(root,filename)}\" "

link_str = ""
for l in LINK:
    link_str += f"-l{l} "

try:
    os.remove(EXECUTABLE)
except:
    pass

cmd = f"{COMPILER} {flag_str} {include_str} -o {EXECUTABLE} {MAIN} {sources_str} {link_str}"
print(cmd)
t1 = time.time()
os.system(cmd)
t2 = time.time()

print("Operation time Elapsed:", f'{t2 - t1:.2f}', "seconds")
