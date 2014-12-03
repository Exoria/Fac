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

# Utility rule file for scene_schema.

# Include the progress variables for this target.
include data/schema/CMakeFiles/scene_schema.dir/progress.make

data/schema/CMakeFiles/scene_schema: binaries/./data/schema/scene.xsd
data/schema/CMakeFiles/scene_schema: binaries/./data/schema/upgrade_0.3.0.xsl
data/schema/CMakeFiles/scene_schema: binaries/./data/schema/upgrade_0.4.0.xsl
data/schema/CMakeFiles/scene_schema: binaries/./data/schema/upgrade_0.5.0.xsl

binaries/./data/schema/scene.xsd: /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/data/schema/scene.xsd
binaries/./data/schema/scene.xsd: /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/data/schema/upgrade_0.3.0.xsl
binaries/./data/schema/scene.xsd: /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/data/schema/upgrade_0.4.0.xsl
binaries/./data/schema/scene.xsd: /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/data/schema/upgrade_0.5.0.xsl
binaries/./data/schema/scene.xsd: /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/data/schema/scene.xsd
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Copying XML data: scene schema and upgrade XSL files"
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/data/schema && /usr/bin/cmake -E copy_if_different /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/data/schema/scene.xsd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/binaries/./data/schema/scene.xsd
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/data/schema && /usr/bin/cmake -E copy_if_different /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/data/schema/upgrade_0.3.0.xsl /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/binaries/./data/schema/upgrade_0.3.0.xsl
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/data/schema && /usr/bin/cmake -E copy_if_different /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/data/schema/upgrade_0.4.0.xsl /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/binaries/./data/schema/upgrade_0.4.0.xsl
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/data/schema && /usr/bin/cmake -E copy_if_different /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/data/schema/upgrade_0.5.0.xsl /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/binaries/./data/schema/upgrade_0.5.0.xsl

binaries/./data/schema/upgrade_0.3.0.xsl: binaries/./data/schema/scene.xsd

binaries/./data/schema/upgrade_0.4.0.xsl: binaries/./data/schema/scene.xsd

binaries/./data/schema/upgrade_0.5.0.xsl: binaries/./data/schema/scene.xsd

scene_schema: data/schema/CMakeFiles/scene_schema
scene_schema: binaries/./data/schema/scene.xsd
scene_schema: binaries/./data/schema/upgrade_0.3.0.xsl
scene_schema: binaries/./data/schema/upgrade_0.4.0.xsl
scene_schema: binaries/./data/schema/upgrade_0.5.0.xsl
scene_schema: data/schema/CMakeFiles/scene_schema.dir/build.make
.PHONY : scene_schema

# Rule to build all files generated by this target.
data/schema/CMakeFiles/scene_schema.dir/build: scene_schema
.PHONY : data/schema/CMakeFiles/scene_schema.dir/build

data/schema/CMakeFiles/scene_schema.dir/clean:
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/data/schema && $(CMAKE_COMMAND) -P CMakeFiles/scene_schema.dir/cmake_clean.cmake
.PHONY : data/schema/CMakeFiles/scene_schema.dir/clean

data/schema/CMakeFiles/scene_schema.dir/depend:
	cd /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mathieu/Documents/Fac/TP/TGESI/mitsuba /home/mathieu/Documents/Fac/TP/TGESI/mitsuba/data/schema /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/data/schema /home/mathieu/Documents/Fac/TP/TGESI/build-mitsuba-Desktop-Default/data/schema/CMakeFiles/scene_schema.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : data/schema/CMakeFiles/scene_schema.dir/depend

