# a simple build script suitable for compiling small projects
# written by jason wong

import os
import time

COMPILER = "g++"
EXECUTABLE = "a.exe"

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

cmd = f"{COMPILER} {flag_str} {include_str} -o {EXECUTABLE} main.cpp {sources_str} {link_str}"
print(cmd)
t1 = time.time()
os.system(cmd)
t2 = time.time()

print("Operation time Elapsed:", f'{t2 - t1:.2f}', "seconds")
