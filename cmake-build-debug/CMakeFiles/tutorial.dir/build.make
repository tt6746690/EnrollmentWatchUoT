# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/markwang/github/EnrollmentWatchUofT

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/markwang/github/EnrollmentWatchUofT/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/tutorial.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tutorial.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tutorial.dir/flags.make

CMakeFiles/tutorial.dir/src/tut.cpp.o: CMakeFiles/tutorial.dir/flags.make
CMakeFiles/tutorial.dir/src/tut.cpp.o: ../src/tut.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/markwang/github/EnrollmentWatchUofT/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tutorial.dir/src/tut.cpp.o"
	/usr/local/opt/ccache/libexec/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tutorial.dir/src/tut.cpp.o -c /Users/markwang/github/EnrollmentWatchUofT/src/tut.cpp

CMakeFiles/tutorial.dir/src/tut.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tutorial.dir/src/tut.cpp.i"
	/usr/local/opt/ccache/libexec/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/markwang/github/EnrollmentWatchUofT/src/tut.cpp > CMakeFiles/tutorial.dir/src/tut.cpp.i

CMakeFiles/tutorial.dir/src/tut.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tutorial.dir/src/tut.cpp.s"
	/usr/local/opt/ccache/libexec/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/markwang/github/EnrollmentWatchUofT/src/tut.cpp -o CMakeFiles/tutorial.dir/src/tut.cpp.s

CMakeFiles/tutorial.dir/src/tut.cpp.o.requires:

.PHONY : CMakeFiles/tutorial.dir/src/tut.cpp.o.requires

CMakeFiles/tutorial.dir/src/tut.cpp.o.provides: CMakeFiles/tutorial.dir/src/tut.cpp.o.requires
	$(MAKE) -f CMakeFiles/tutorial.dir/build.make CMakeFiles/tutorial.dir/src/tut.cpp.o.provides.build
.PHONY : CMakeFiles/tutorial.dir/src/tut.cpp.o.provides

CMakeFiles/tutorial.dir/src/tut.cpp.o.provides.build: CMakeFiles/tutorial.dir/src/tut.cpp.o


# Object files for target tutorial
tutorial_OBJECTS = \
"CMakeFiles/tutorial.dir/src/tut.cpp.o"

# External object files for target tutorial
tutorial_EXTERNAL_OBJECTS =

../bin/tutorial: CMakeFiles/tutorial.dir/src/tut.cpp.o
../bin/tutorial: CMakeFiles/tutorial.dir/build.make
../bin/tutorial: CMakeFiles/tutorial.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/markwang/github/EnrollmentWatchUofT/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/tutorial"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tutorial.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tutorial.dir/build: ../bin/tutorial

.PHONY : CMakeFiles/tutorial.dir/build

CMakeFiles/tutorial.dir/requires: CMakeFiles/tutorial.dir/src/tut.cpp.o.requires

.PHONY : CMakeFiles/tutorial.dir/requires

CMakeFiles/tutorial.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tutorial.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tutorial.dir/clean

CMakeFiles/tutorial.dir/depend:
	cd /Users/markwang/github/EnrollmentWatchUofT/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/markwang/github/EnrollmentWatchUofT /Users/markwang/github/EnrollmentWatchUofT /Users/markwang/github/EnrollmentWatchUofT/cmake-build-debug /Users/markwang/github/EnrollmentWatchUofT/cmake-build-debug /Users/markwang/github/EnrollmentWatchUofT/cmake-build-debug/CMakeFiles/tutorial.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tutorial.dir/depend

