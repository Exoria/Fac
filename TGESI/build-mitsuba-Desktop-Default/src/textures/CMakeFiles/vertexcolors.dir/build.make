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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mathieu/Documents/Fac/TP/TGESI/mitsuba

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default

# Include any dependencies generated for this target.
include src/textures/CMakeFiles/vertexcolors.dir/depend.make

# Include the progress variables for this target.
include src/textures/CMakeFiles/vertexcolors.dir/progress.make

# Include the compile flags for this target's objects.
include src/textures/CMakeFiles/vertexcolors.dir/flags.make

src/textures/CMakeFiles/vertexcolors.dir/vertexcolors.cpp.o: src/textures/CMakeFiles/vertexcolors.dir/flags.make
src/textures/CMakeFiles/vertexcolors.dir/vertexcolors.cpp.o: /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/textures/vertexcolors.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/textures/CMakeFiles/vertexcolors.dir/vertexcolors.cpp.o"
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/textures && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/vertexcolors.dir/vertexcolors.cpp.o -c /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/textures/vertexcolors.cpp

src/textures/CMakeFiles/vertexcolors.dir/vertexcolors.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vertexcolors.dir/vertexcolors.cpp.i"
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/textures && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/textures/vertexcolors.cpp > CMakeFiles/vertexcolors.dir/vertexcolors.cpp.i

src/textures/CMakeFiles/vertexcolors.dir/vertexcolors.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vertexcolors.dir/vertexcolors.cpp.s"
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/textures && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/textures/vertexcolors.cpp -o CMakeFiles/vertexcolors.dir/vertexcolors.cpp.s

src/textures/CMakeFiles/vertexcolors.dir/vertexcolors.cpp.o.requires:
.PHONY : src/textures/CMakeFiles/vertexcolors.dir/vertexcolors.cpp.o.requires

src/textures/CMakeFiles/vertexcolors.dir/vertexcolors.cpp.o.provides: src/textures/CMakeFiles/vertexcolors.dir/vertexcolors.cpp.o.requires
	$(MAKE) -f src/textures/CMakeFiles/vertexcolors.dir/build.make src/textures/CMakeFiles/vertexcolors.dir/vertexcolors.cpp.o.provides.build
.PHONY : src/textures/CMakeFiles/vertexcolors.dir/vertexcolors.cpp.o.provides

src/textures/CMakeFiles/vertexcolors.dir/vertexcolors.cpp.o.provides.build: src/textures/CMakeFiles/vertexcolors.dir/vertexcolors.cpp.o

# Object files for target vertexcolors
vertexcolors_OBJECTS = \
"CMakeFiles/vertexcolors.dir/vertexcolors.cpp.o"

# External object files for target vertexcolors
vertexcolors_EXTERNAL_OBJECTS =

binaries/plugins/vertexcolors.so: src/textures/CMakeFiles/vertexcolors.dir/vertexcolors.cpp.o
binaries/plugins/vertexcolors.so: src/textures/CMakeFiles/vertexcolors.dir/build.make
binaries/plugins/vertexcolors.so: binaries/libmitsuba-core.so
binaries/plugins/vertexcolors.so: binaries/libmitsuba-render.so
binaries/plugins/vertexcolors.so: binaries/libmitsuba-core.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libz.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libpng.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libz.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libpng.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libjpeg.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libHalf.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libIex.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libImath.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libIlmThread.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libIlmImf.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libboost_system.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libboost_thread.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libpthread.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libIlmImf.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libboost_system.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libboost_thread.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libpthread.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libfftw3.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libfftw3_threads.so
binaries/plugins/vertexcolors.so: /usr/lib/x86_64-linux-gnu/libxerces-c.so
binaries/plugins/vertexcolors.so: src/textures/CMakeFiles/vertexcolors.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module ../../binaries/plugins/vertexcolors.so"
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/textures && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vertexcolors.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/textures/CMakeFiles/vertexcolors.dir/build: binaries/plugins/vertexcolors.so
.PHONY : src/textures/CMakeFiles/vertexcolors.dir/build

src/textures/CMakeFiles/vertexcolors.dir/requires: src/textures/CMakeFiles/vertexcolors.dir/vertexcolors.cpp.o.requires
.PHONY : src/textures/CMakeFiles/vertexcolors.dir/requires

src/textures/CMakeFiles/vertexcolors.dir/clean:
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/textures && $(CMAKE_COMMAND) -P CMakeFiles/vertexcolors.dir/cmake_clean.cmake
.PHONY : src/textures/CMakeFiles/vertexcolors.dir/clean

src/textures/CMakeFiles/vertexcolors.dir/depend:
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mathieu/Documents/Fac/TP/TGESI/mitsuba /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/textures /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/textures /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/textures/CMakeFiles/vertexcolors.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/textures/CMakeFiles/vertexcolors.dir/depend

