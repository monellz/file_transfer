# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/monell/code/file_transfer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/monell/code/file_transfer/build

# Include any dependencies generated for this target.
include server/CMakeFiles/server.dir/depend.make

# Include the progress variables for this target.
include server/CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include server/CMakeFiles/server.dir/flags.make

server/CMakeFiles/server.dir/server.cc.o: server/CMakeFiles/server.dir/flags.make
server/CMakeFiles/server.dir/server.cc.o: ../server/server.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/monell/code/file_transfer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object server/CMakeFiles/server.dir/server.cc.o"
	cd /home/monell/code/file_transfer/build/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/server.cc.o -c /home/monell/code/file_transfer/server/server.cc

server/CMakeFiles/server.dir/server.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/server.cc.i"
	cd /home/monell/code/file_transfer/build/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/monell/code/file_transfer/server/server.cc > CMakeFiles/server.dir/server.cc.i

server/CMakeFiles/server.dir/server.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/server.cc.s"
	cd /home/monell/code/file_transfer/build/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/monell/code/file_transfer/server/server.cc -o CMakeFiles/server.dir/server.cc.s

server/CMakeFiles/server.dir/server.cc.o.requires:

.PHONY : server/CMakeFiles/server.dir/server.cc.o.requires

server/CMakeFiles/server.dir/server.cc.o.provides: server/CMakeFiles/server.dir/server.cc.o.requires
	$(MAKE) -f server/CMakeFiles/server.dir/build.make server/CMakeFiles/server.dir/server.cc.o.provides.build
.PHONY : server/CMakeFiles/server.dir/server.cc.o.provides

server/CMakeFiles/server.dir/server.cc.o.provides.build: server/CMakeFiles/server.dir/server.cc.o


# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/server.cc.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

server/server: server/CMakeFiles/server.dir/server.cc.o
server/server: server/CMakeFiles/server.dir/build.make
server/server: server/CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/monell/code/file_transfer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable server"
	cd /home/monell/code/file_transfer/build/server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
server/CMakeFiles/server.dir/build: server/server

.PHONY : server/CMakeFiles/server.dir/build

server/CMakeFiles/server.dir/requires: server/CMakeFiles/server.dir/server.cc.o.requires

.PHONY : server/CMakeFiles/server.dir/requires

server/CMakeFiles/server.dir/clean:
	cd /home/monell/code/file_transfer/build/server && $(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : server/CMakeFiles/server.dir/clean

server/CMakeFiles/server.dir/depend:
	cd /home/monell/code/file_transfer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/monell/code/file_transfer /home/monell/code/file_transfer/server /home/monell/code/file_transfer/build /home/monell/code/file_transfer/build/server /home/monell/code/file_transfer/build/server/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : server/CMakeFiles/server.dir/depend

