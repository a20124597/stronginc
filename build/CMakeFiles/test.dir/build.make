# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/ubuntu/StrongSim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/StrongSim/build

# Include any dependencies generated for this target.
include CMakeFiles/test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test.dir/flags.make

CMakeFiles/test.dir/cpp/app/example/test.cc.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/cpp/app/example/test.cc.o: ../cpp/app/example/test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/StrongSim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test.dir/cpp/app/example/test.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test.dir/cpp/app/example/test.cc.o -c /home/ubuntu/StrongSim/cpp/app/example/test.cc

CMakeFiles/test.dir/cpp/app/example/test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/cpp/app/example/test.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/StrongSim/cpp/app/example/test.cc > CMakeFiles/test.dir/cpp/app/example/test.cc.i

CMakeFiles/test.dir/cpp/app/example/test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/cpp/app/example/test.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/StrongSim/cpp/app/example/test.cc -o CMakeFiles/test.dir/cpp/app/example/test.cc.s

CMakeFiles/test.dir/cpp/app/example/test.cc.o.requires:

.PHONY : CMakeFiles/test.dir/cpp/app/example/test.cc.o.requires

CMakeFiles/test.dir/cpp/app/example/test.cc.o.provides: CMakeFiles/test.dir/cpp/app/example/test.cc.o.requires
	$(MAKE) -f CMakeFiles/test.dir/build.make CMakeFiles/test.dir/cpp/app/example/test.cc.o.provides.build
.PHONY : CMakeFiles/test.dir/cpp/app/example/test.cc.o.provides

CMakeFiles/test.dir/cpp/app/example/test.cc.o.provides.build: CMakeFiles/test.dir/cpp/app/example/test.cc.o


# Object files for target test
test_OBJECTS = \
"CMakeFiles/test.dir/cpp/app/example/test.cc.o"

# External object files for target test
test_EXTERNAL_OBJECTS =

test: CMakeFiles/test.dir/cpp/app/example/test.cc.o
test: CMakeFiles/test.dir/build.make
test: libgraph_api.a
test: /usr/local/lib/libmpi.so
test: /usr/lib/x86_64-linux-gnu/libboost_system.so
test: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
test: /usr/lib/x86_64-linux-gnu/libboost_program_options.so
test: /usr/local/lib/libglog.so
test: /usr/local/lib/libgflags.a
test: CMakeFiles/test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/StrongSim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test.dir/build: test

.PHONY : CMakeFiles/test.dir/build

CMakeFiles/test.dir/requires: CMakeFiles/test.dir/cpp/app/example/test.cc.o.requires

.PHONY : CMakeFiles/test.dir/requires

CMakeFiles/test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test.dir/clean

CMakeFiles/test.dir/depend:
	cd /home/ubuntu/StrongSim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/StrongSim /home/ubuntu/StrongSim /home/ubuntu/StrongSim/build /home/ubuntu/StrongSim/build /home/ubuntu/StrongSim/build/CMakeFiles/test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test.dir/depend

