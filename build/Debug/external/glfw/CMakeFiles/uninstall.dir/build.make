# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\antis\Desktop\opengl_ex

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\antis\Desktop\opengl_ex\build\Debug

# Utility rule file for uninstall.

# Include the progress variables for this target.
include external/glfw/CMakeFiles/uninstall.dir/progress.make

external/glfw/CMakeFiles/uninstall:
	cd /d C:\Users\antis\Desktop\opengl_ex\build\Debug\external\glfw && "C:\Program Files\CMake\bin\cmake.exe" -P C:/Users/antis/Desktop/opengl_ex/build/Debug/external/glfw/cmake_uninstall.cmake

uninstall: external/glfw/CMakeFiles/uninstall
uninstall: external/glfw/CMakeFiles/uninstall.dir/build.make

.PHONY : uninstall

# Rule to build all files generated by this target.
external/glfw/CMakeFiles/uninstall.dir/build: uninstall

.PHONY : external/glfw/CMakeFiles/uninstall.dir/build

external/glfw/CMakeFiles/uninstall.dir/clean:
	cd /d C:\Users\antis\Desktop\opengl_ex\build\Debug\external\glfw && $(CMAKE_COMMAND) -P CMakeFiles\uninstall.dir\cmake_clean.cmake
.PHONY : external/glfw/CMakeFiles/uninstall.dir/clean

external/glfw/CMakeFiles/uninstall.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\antis\Desktop\opengl_ex C:\Users\antis\Desktop\opengl_ex\external\glfw C:\Users\antis\Desktop\opengl_ex\build\Debug C:\Users\antis\Desktop\opengl_ex\build\Debug\external\glfw C:\Users\antis\Desktop\opengl_ex\build\Debug\external\glfw\CMakeFiles\uninstall.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : external/glfw/CMakeFiles/uninstall.dir/depend
