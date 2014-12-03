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

# Utility rule file for halton_pch.

# Include the progress variables for this target.
include src/samplers/CMakeFiles/halton_pch.dir/progress.make

src/samplers/CMakeFiles/halton_pch: src/samplers/halton_pch/./mitsuba_precompiled.hpp.gch

src/samplers/halton_pch/./mitsuba_precompiled.hpp.gch: /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/data/pch/mitsuba_precompiled.hpp
src/samplers/halton_pch/./mitsuba_precompiled.hpp.gch: /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/data/pch/mitsuba_precompiled.hpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating halton_pch/./mitsuba_precompiled.hpp.gch"
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/samplers && /usr/bin/cmake -E make_directory /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/samplers/halton_pch/.
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/samplers && /usr/bin/c++  -fvisibility=hidden -pipe -march=nocona -ffast-math -Wall -Winvalid-pch -mfpmath=sse -fopenmp -O3 -DNDEBUG -DMTS_DEBUG -DSINGLE_PRECISION -DSPECTRUM_SAMPLES=3 -DMTS_SSE -DMTS_HAS_COHERENT_RT -DMTS_HAS_FFTW=1 -DMTS_HAS_LIBPNG=1 -DMTS_HAS_LIBJPEG=1 -DMTS_HAS_OPENEXR=1 -I/usr/include/eigen3 -I/usr/include/OpenEXR -I/home/mathieu/Documents/Fac/TP/TGESI/mitsuba/include -fPIC -x c++-header -c -o /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/samplers/halton_pch/./mitsuba_precompiled.hpp.gch /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/data/pch/mitsuba_precompiled.hpp

halton_pch: src/samplers/CMakeFiles/halton_pch
halton_pch: src/samplers/halton_pch/./mitsuba_precompiled.hpp.gch
halton_pch: src/samplers/CMakeFiles/halton_pch.dir/build.make
.PHONY : halton_pch

# Rule to build all files generated by this target.
src/samplers/CMakeFiles/halton_pch.dir/build: halton_pch
.PHONY : src/samplers/CMakeFiles/halton_pch.dir/build

src/samplers/CMakeFiles/halton_pch.dir/clean:
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/samplers && $(CMAKE_COMMAND) -P CMakeFiles/halton_pch.dir/cmake_clean.cmake
.PHONY : src/samplers/CMakeFiles/halton_pch.dir/clean

src/samplers/CMakeFiles/halton_pch.dir/depend:
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mathieu/Documents/Fac/TP/TGESI/mitsuba /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/src/samplers /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/samplers /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/src/samplers/CMakeFiles/halton_pch.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/samplers/CMakeFiles/halton_pch.dir/depend

