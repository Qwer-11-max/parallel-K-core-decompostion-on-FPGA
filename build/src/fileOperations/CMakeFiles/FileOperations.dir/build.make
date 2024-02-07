# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = D:\env\Cmake2.28.1\bin\cmake.exe

# The command to remove a file.
RM = D:\env\Cmake2.28.1\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\CodeRepository\parallel-K-core-decompostion-on-FPGA

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\CodeRepository\parallel-K-core-decompostion-on-FPGA\build

# Include any dependencies generated for this target.
include src/fileOperations/CMakeFiles/FileOperations.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/fileOperations/CMakeFiles/FileOperations.dir/compiler_depend.make

# Include the progress variables for this target.
include src/fileOperations/CMakeFiles/FileOperations.dir/progress.make

# Include the compile flags for this target's objects.
include src/fileOperations/CMakeFiles/FileOperations.dir/flags.make

src/fileOperations/CMakeFiles/FileOperations.dir/fileOperations.cpp.obj: src/fileOperations/CMakeFiles/FileOperations.dir/flags.make
src/fileOperations/CMakeFiles/FileOperations.dir/fileOperations.cpp.obj: src/fileOperations/CMakeFiles/FileOperations.dir/includes_CXX.rsp
src/fileOperations/CMakeFiles/FileOperations.dir/fileOperations.cpp.obj: D:/CodeRepository/parallel-K-core-decompostion-on-FPGA/src/fileOperations/fileOperations.cpp
src/fileOperations/CMakeFiles/FileOperations.dir/fileOperations.cpp.obj: src/fileOperations/CMakeFiles/FileOperations.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\CodeRepository\parallel-K-core-decompostion-on-FPGA\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/fileOperations/CMakeFiles/FileOperations.dir/fileOperations.cpp.obj"
	cd /d D:\CodeRepository\parallel-K-core-decompostion-on-FPGA\build\src\fileOperations && D:\env\MinGW64_win32\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/fileOperations/CMakeFiles/FileOperations.dir/fileOperations.cpp.obj -MF CMakeFiles\FileOperations.dir\fileOperations.cpp.obj.d -o CMakeFiles\FileOperations.dir\fileOperations.cpp.obj -c D:\CodeRepository\parallel-K-core-decompostion-on-FPGA\src\fileOperations\fileOperations.cpp

src/fileOperations/CMakeFiles/FileOperations.dir/fileOperations.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/FileOperations.dir/fileOperations.cpp.i"
	cd /d D:\CodeRepository\parallel-K-core-decompostion-on-FPGA\build\src\fileOperations && D:\env\MinGW64_win32\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\CodeRepository\parallel-K-core-decompostion-on-FPGA\src\fileOperations\fileOperations.cpp > CMakeFiles\FileOperations.dir\fileOperations.cpp.i

src/fileOperations/CMakeFiles/FileOperations.dir/fileOperations.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/FileOperations.dir/fileOperations.cpp.s"
	cd /d D:\CodeRepository\parallel-K-core-decompostion-on-FPGA\build\src\fileOperations && D:\env\MinGW64_win32\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\CodeRepository\parallel-K-core-decompostion-on-FPGA\src\fileOperations\fileOperations.cpp -o CMakeFiles\FileOperations.dir\fileOperations.cpp.s

# Object files for target FileOperations
FileOperations_OBJECTS = \
"CMakeFiles/FileOperations.dir/fileOperations.cpp.obj"

# External object files for target FileOperations
FileOperations_EXTERNAL_OBJECTS =

src/fileOperations/libFileOperations.a: src/fileOperations/CMakeFiles/FileOperations.dir/fileOperations.cpp.obj
src/fileOperations/libFileOperations.a: src/fileOperations/CMakeFiles/FileOperations.dir/build.make
src/fileOperations/libFileOperations.a: src/fileOperations/CMakeFiles/FileOperations.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\CodeRepository\parallel-K-core-decompostion-on-FPGA\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libFileOperations.a"
	cd /d D:\CodeRepository\parallel-K-core-decompostion-on-FPGA\build\src\fileOperations && $(CMAKE_COMMAND) -P CMakeFiles\FileOperations.dir\cmake_clean_target.cmake
	cd /d D:\CodeRepository\parallel-K-core-decompostion-on-FPGA\build\src\fileOperations && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\FileOperations.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/fileOperations/CMakeFiles/FileOperations.dir/build: src/fileOperations/libFileOperations.a
.PHONY : src/fileOperations/CMakeFiles/FileOperations.dir/build

src/fileOperations/CMakeFiles/FileOperations.dir/clean:
	cd /d D:\CodeRepository\parallel-K-core-decompostion-on-FPGA\build\src\fileOperations && $(CMAKE_COMMAND) -P CMakeFiles\FileOperations.dir\cmake_clean.cmake
.PHONY : src/fileOperations/CMakeFiles/FileOperations.dir/clean

src/fileOperations/CMakeFiles/FileOperations.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\CodeRepository\parallel-K-core-decompostion-on-FPGA D:\CodeRepository\parallel-K-core-decompostion-on-FPGA\src\fileOperations D:\CodeRepository\parallel-K-core-decompostion-on-FPGA\build D:\CodeRepository\parallel-K-core-decompostion-on-FPGA\build\src\fileOperations D:\CodeRepository\parallel-K-core-decompostion-on-FPGA\build\src\fileOperations\CMakeFiles\FileOperations.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/fileOperations/CMakeFiles/FileOperations.dir/depend
