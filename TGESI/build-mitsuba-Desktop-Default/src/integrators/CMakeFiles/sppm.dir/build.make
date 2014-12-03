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
include src/integrators/CMakeFiles/sppm.dir/depend.make

# Include the progress variables for this target.
include src/integrators/CMakeFiles/sppm.dir/progress.make

# Include the compile flags for this target's objects.
include src/integrators/CMakeFiles/sppm.dir/flags.make

src/integrators/CMakeFiles/sppm.dir/photonmapper/sppm.cpp.o: src/integrators/CMakeFiles/sppm.dir/flags.make
src/integrators/CMakeFiles/sppm.dir/photonmapper/sppm.cpp.o: /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/integrators/photonmapper/sppm.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/integrators/CMakeFiles/sppm.dir/photonmapper/sppm.cpp.o"
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/integrators && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/sppm.dir/photonmapper/sppm.cpp.o -c /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/integrators/photonmapper/sppm.cpp

src/integrators/CMakeFiles/sppm.dir/photonmapper/sppm.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sppm.dir/photonmapper/sppm.cpp.i"
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/integrators && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/integrators/photonmapper/sppm.cpp > CMakeFiles/sppm.dir/photonmapper/sppm.cpp.i

src/integrators/CMakeFiles/sppm.dir/photonmapper/sppm.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sppm.dir/photonmapper/sppm.cpp.s"
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/integrators && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/integrators/photonmapper/sppm.cpp -o CMakeFiles/sppm.dir/photonmapper/sppm.cpp.s

src/integrators/CMakeFiles/sppm.dir/photonmapper/sppm.cpp.o.requires:
.PHONY : src/integrators/CMakeFiles/sppm.dir/photonmapper/sppm.cpp.o.requires

src/integrators/CMakeFiles/sppm.dir/photonmapper/sppm.cpp.o.provides: src/integrators/CMakeFiles/sppm.dir/photonmapper/sppm.cpp.o.requires
	$(MAKE) -f src/integrators/CMakeFiles/sppm.dir/build.make src/integrators/CMakeFiles/sppm.dir/photonmapper/sppm.cpp.o.provides.build
.PHONY : src/integrators/CMakeFiles/sppm.dir/photonmapper/sppm.cpp.o.provides

src/integrators/CMakeFiles/sppm.dir/photonmapper/sppm.cpp.o.provides.build: src/integrators/CMakeFiles/sppm.dir/photonmapper/sppm.cpp.o

# Object files for target sppm
sppm_OBJECTS = \
"CMakeFiles/sppm.dir/photonmapper/sppm.cpp.o"

# External object files for target sppm
sppm_EXTERNAL_OBJECTS =

binaries/plugins/sppm.so: src/integrators/CMakeFiles/sppm.dir/photonmapper/sppm.cpp.o
binaries/plugins/sppm.so: src/integrators/CMakeFiles/sppm.dir/build.make
binaries/plugins/sppm.so: binaries/libmitsuba-core.so
binaries/plugins/sppm.so: binaries/libmitsuba-render.so
binaries/plugins/sppm.so: binaries/libmitsuba-core.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libz.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libpng.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libz.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libpng.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libjpeg.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libHalf.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libIex.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libImath.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libIlmThread.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libIlmImf.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libboost_system.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libboost_thread.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libpthread.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libIlmImf.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libboost_system.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libboost_thread.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libpthread.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libfftw3.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libfftw3_threads.so
binaries/plugins/sppm.so: /usr/lib/x86_64-linux-gnu/libxerces-c.so
binaries/plugins/sppm.so: src/integrators/CMakeFiles/sppm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module ../../binaries/plugins/sppm.so"
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/integrators && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sppm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/integrators/CMakeFiles/sppm.dir/build: binaries/plugins/sppm.so
.PHONY : src/integrators/CMakeFiles/sppm.dir/build

src/integrators/CMakeFiles/sppm.dir/requires: src/integrators/CMakeFiles/sppm.dir/photonmapper/sppm.cpp.o.requires
.PHONY : src/integrators/CMakeFiles/sppm.dir/requires

src/integrators/CMakeFiles/sppm.dir/clean:
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/integrators && $(CMAKE_COMMAND) -P CMakeFiles/sppm.dir/cmake_clean.cmake
.PHONY : src/integrators/CMakeFiles/sppm.dir/clean

src/integrators/CMakeFiles/sppm.dir/depend:
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mathieu/Documents/Fac/TP/TGESI/mitsuba /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/integrators /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/integrators /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/integrators/CMakeFiles/sppm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/integrators/CMakeFiles/sppm.dir/depend

