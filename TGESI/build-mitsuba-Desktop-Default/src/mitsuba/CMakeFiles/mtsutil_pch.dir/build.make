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

# Utility rule file for mtsutil_pch.

# Include the progress variables for this target.
include src/mitsuba/CMakeFiles/mtsutil_pch.dir/progress.make

src/mitsuba/CMakeFiles/mtsutil_pch: src/mitsuba/mtsutil_pch/./mitsuba_precompiled.hpp.gch

src/mitsuba/mtsutil_pch/./mitsuba_precompiled.hpp.gch: /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/data/pch/mitsuba_precompiled.hpp
src/mitsuba/mtsutil_pch/./mitsuba_precompiled.hpp.gch: /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/data/pch/mitsuba_precompiled.hpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating mtsutil_pch/./mitsuba_precompiled.hpp.gch"
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/mitsuba && /usr/bin/cmake -E make_directory /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/mitsuba/mtsutil_pch/.
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/mitsuba && /usr/bin/c++  -fvisibility=hidden -pipe -march=nocona -ffast-math -Wall -Winvalid-pch -mfpmath=sse -fopenmp -O3 -DNDEBUG -DMTS_DEBUG -DSINGLE_PRECISION -DSPECTRUM_SAMPLES=3 -DMTS_SSE -DMTS_HAS_COHERENT_RT -DMTS_HAS_FFTW=1 -DMTS_HAS_LIBPNG=1 -DMTS_HAS_LIBJPEG=1 -DMTS_HAS_OPENEXR=1 -I/usr/include/eigen3 -I/usr/include/OpenEXR -I/home/mathieu/Documents/Fac/TP/TGESI/mitsuba/include -x c++-header -c -o /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/mitsuba/mtsutil_pch/./mitsuba_precompiled.hpp.gch /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/data/pch/mitsuba_precompiled.hpp

mtsutil_pch: src/mitsuba/CMakeFiles/mtsutil_pch
mtsutil_pch: src/mitsuba/mtsutil_pch/./mitsuba_precompiled.hpp.gch
mtsutil_pch: src/mitsuba/CMakeFiles/mtsutil_pch.dir/build.make
.PHONY : mtsutil_pch

# Rule to build all files generated by this target.
src/mitsuba/CMakeFiles/mtsutil_pch.dir/build: mtsutil_pch
.PHONY : src/mitsuba/CMakeFiles/mtsutil_pch.dir/build

src/mitsuba/CMakeFiles/mtsutil_pch.dir/clean:
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/mitsuba && $(CMAKE_COMMAND) -P CMakeFiles/mtsutil_pch.dir/cmake_clean.cmake
.PHONY : src/mitsuba/CMakeFiles/mtsutil_pch.dir/clean

src/mitsuba/CMakeFiles/mtsutil_pch.dir/depend:
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mathieu/Documents/Fac/TP/TGESI/mitsuba /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/mitsuba /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/mitsuba /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/mitsuba/CMakeFiles/mtsutil_pch.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/mitsuba/CMakeFiles/mtsutil_pch.dir/depend

