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
include CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/flags.make

CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.o: CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/flags.make
CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.o: ../src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/snow2t/Workspace/LearnOpenGL_AVM/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.o -c /home/snow2t/Workspace/LearnOpenGL_AVM/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp

CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/snow2t/Workspace/LearnOpenGL_AVM/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp > CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.i

CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/snow2t/Workspace/LearnOpenGL_AVM/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp -o CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.s

CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.o.requires:
.PHONY : CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.o.requires

CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.o.provides: CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.o.requires
	$(MAKE) -f CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/build.make CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.o.provides.build
.PHONY : CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.o.provides

CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.o.provides.build: CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.o

# Object files for target 5.advanced_lighting__1.advanced_lighting
5_advanced_lighting__1_advanced_lighting_OBJECTS = \
"CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.o"

# External object files for target 5.advanced_lighting__1.advanced_lighting
5_advanced_lighting__1_advanced_lighting_EXTERNAL_OBJECTS =

bin/5.advanced_lighting/5.advanced_lighting__1.advanced_lighting: CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.o
bin/5.advanced_lighting/5.advanced_lighting__1.advanced_lighting: CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/build.make
bin/5.advanced_lighting/5.advanced_lighting__1.advanced_lighting: /usr/local/lib/libglfw3.a
bin/5.advanced_lighting/5.advanced_lighting__1.advanced_lighting: /usr/lib/libassimp.so
bin/5.advanced_lighting/5.advanced_lighting__1.advanced_lighting: libSTB_IMAGE.a
bin/5.advanced_lighting/5.advanced_lighting__1.advanced_lighting: libGLAD.a
bin/5.advanced_lighting/5.advanced_lighting__1.advanced_lighting: CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable bin/5.advanced_lighting/5.advanced_lighting__1.advanced_lighting"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/build: bin/5.advanced_lighting/5.advanced_lighting__1.advanced_lighting
.PHONY : CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/build

CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/requires: CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp.o.requires
.PHONY : CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/requires

CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/cmake_clean.cmake
.PHONY : CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/clean

CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/depend:
	cd /home/snow2t/Workspace/LearnOpenGL_AVM/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/snow2t/Workspace/LearnOpenGL_AVM /home/snow2t/Workspace/LearnOpenGL_AVM /home/snow2t/Workspace/LearnOpenGL_AVM/build /home/snow2t/Workspace/LearnOpenGL_AVM/build /home/snow2t/Workspace/LearnOpenGL_AVM/build/CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/5.advanced_lighting__1.advanced_lighting.dir/depend
