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
include src/shapes/CMakeFiles/hair.dir/depend.make

# Include the progress variables for this target.
include src/shapes/CMakeFiles/hair.dir/progress.make

# Include the compile flags for this target's objects.
include src/shapes/CMakeFiles/hair.dir/flags.make

src/shapes/CMakeFiles/hair.dir/hair.cpp.o: src/shapes/CMakeFiles/hair.dir/flags.make
src/shapes/CMakeFiles/hair.dir/hair.cpp.o: /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/shapes/hair.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/shapes/CMakeFiles/hair.dir/hair.cpp.o"
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/shapes && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/hair.dir/hair.cpp.o -c /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/shapes/hair.cpp

src/shapes/CMakeFiles/hair.dir/hair.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hair.dir/hair.cpp.i"
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/shapes && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/shapes/hair.cpp > CMakeFiles/hair.dir/hair.cpp.i

src/shapes/CMakeFiles/hair.dir/hair.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hair.dir/hair.cpp.s"
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/shapes && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/shapes/hair.cpp -o CMakeFiles/hair.dir/hair.cpp.s

src/shapes/CMakeFiles/hair.dir/hair.cpp.o.requires:
.PHONY : src/shapes/CMakeFiles/hair.dir/hair.cpp.o.requires

src/shapes/CMakeFiles/hair.dir/hair.cpp.o.provides: src/shapes/CMakeFiles/hair.dir/hair.cpp.o.requires
	$(MAKE) -f src/shapes/CMakeFiles/hair.dir/build.make src/shapes/CMakeFiles/hair.dir/hair.cpp.o.provides.build
.PHONY : src/shapes/CMakeFiles/hair.dir/hair.cpp.o.provides

src/shapes/CMakeFiles/hair.dir/hair.cpp.o.provides.build: src/shapes/CMakeFiles/hair.dir/hair.cpp.o

# Object files for target hair
hair_OBJECTS = \
"CMakeFiles/hair.dir/hair.cpp.o"

# External object files for target hair
hair_EXTERNAL_OBJECTS =

binaries/plugins/hair.so: src/shapes/CMakeFiles/hair.dir/hair.cpp.o
binaries/plugins/hair.so: src/shapes/CMakeFiles/hair.dir/build.make
binaries/plugins/hair.so: binaries/libmitsuba-core.so
binaries/plugins/hair.so: binaries/libmitsuba-render.so
binaries/plugins/hair.so: binaries/libmitsuba-core.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libz.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libpng.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libz.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libpng.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libjpeg.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libHalf.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libIex.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libImath.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libIlmThread.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libIlmImf.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libboost_system.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libboost_thread.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libpthread.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libIlmImf.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libboost_system.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libboost_thread.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libpthread.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libfftw3.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libfftw3_threads.so
binaries/plugins/hair.so: /usr/lib/x86_64-linux-gnu/libxerces-c.so
binaries/plugins/hair.so: src/shapes/CMakeFiles/hair.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module ../../binaries/plugins/hair.so"
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/shapes && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hair.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/shapes/CMakeFiles/hair.dir/build: binaries/plugins/hair.so
.PHONY : src/shapes/CMakeFiles/hair.dir/build

src/shapes/CMakeFiles/hair.dir/requires: src/shapes/CMakeFiles/hair.dir/hair.cpp.o.requires
.PHONY : src/shapes/CMakeFiles/hair.dir/requires

src/shapes/CMakeFiles/hair.dir/clean:
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/shapes && $(CMAKE_COMMAND) -P CMakeFiles/hair.dir/cmake_clean.cmake
.PHONY : src/shapes/CMakeFiles/hair.dir/clean

src/shapes/CMakeFiles/hair.dir/depend:
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mathieu/Documents/Fac/TP/TGESI/mitsuba /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/shapes /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/shapes /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/shapes/CMakeFiles/hair.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/shapes/CMakeFiles/hair.dir/depend

