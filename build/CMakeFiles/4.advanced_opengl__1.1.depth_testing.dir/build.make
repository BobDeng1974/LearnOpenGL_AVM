# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/snow2t/Workspace/LearnOpenGL_AVM

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/snow2t/Workspace/LearnOpenGL_AVM/build

# Include any dependencies generated for this target.
include CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/flags.make

CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.o: CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/flags.make
CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.o: ../src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/snow2t/Workspace/LearnOpenGL_AVM/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.o -c /home/snow2t/Workspace/LearnOpenGL_AVM/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp

CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/snow2t/Workspace/LearnOpenGL_AVM/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp > CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.i

CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/snow2t/Workspace/LearnOpenGL_AVM/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp -o CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.s

CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.o.requires:
.PHONY : CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.o.requires

CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.o.provides: CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.o.requires
	$(MAKE) -f CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/build.make CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.o.provides.build
.PHONY : CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.o.provides

CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.o.provides.build: CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.o

# Object files for target 4.advanced_opengl__1.1.depth_testing
4_advanced_opengl__1_1_depth_testing_OBJECTS = \
"CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.o"

# External object files for target 4.advanced_opengl__1.1.depth_testing
4_advanced_opengl__1_1_depth_testing_EXTERNAL_OBJECTS =

bin/4.advanced_opengl/4.advanced_opengl__1.1.depth_testing: CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.o
bin/4.advanced_opengl/4.advanced_opengl__1.1.depth_testing: CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/build.make
bin/4.advanced_opengl/4.advanced_opengl__1.1.depth_testing: /usr/local/lib/libglfw3.a
bin/4.advanced_opengl/4.advanced_opengl__1.1.depth_testing: /usr/lib/libassimp.so
bin/4.advanced_opengl/4.advanced_opengl__1.1.depth_testing: libSTB_IMAGE.a
bin/4.advanced_opengl/4.advanced_opengl__1.1.depth_testing: libGLAD.a
bin/4.advanced_opengl/4.advanced_opengl__1.1.depth_testing: CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable bin/4.advanced_opengl/4.advanced_opengl__1.1.depth_testing"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/build: bin/4.advanced_opengl/4.advanced_opengl__1.1.depth_testing
.PHONY : CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/build

CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/requires: CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp.o.requires
.PHONY : CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/requires

CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/cmake_clean.cmake
.PHONY : CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/clean

CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/depend:
	cd /home/snow2t/Workspace/LearnOpenGL_AVM/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/snow2t/Workspace/LearnOpenGL_AVM /home/snow2t/Workspace/LearnOpenGL_AVM /home/snow2t/Workspace/LearnOpenGL_AVM/build /home/snow2t/Workspace/LearnOpenGL_AVM/build /home/snow2t/Workspace/LearnOpenGL_AVM/build/CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/4.advanced_opengl__1.1.depth_testing.dir/depend
