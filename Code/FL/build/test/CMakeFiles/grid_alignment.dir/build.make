# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build

# Include any dependencies generated for this target.
include test/CMakeFiles/grid_alignment.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/grid_alignment.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/grid_alignment.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/grid_alignment.dir/flags.make

test/CMakeFiles/grid_alignment.dir/grid_alignment.cxx.o: test/CMakeFiles/grid_alignment.dir/flags.make
test/CMakeFiles/grid_alignment.dir/grid_alignment.cxx.o: ../test/grid_alignment.cxx
test/CMakeFiles/grid_alignment.dir/grid_alignment.cxx.o: test/CMakeFiles/grid_alignment.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/grid_alignment.dir/grid_alignment.cxx.o"
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/grid_alignment.dir/grid_alignment.cxx.o -MF CMakeFiles/grid_alignment.dir/grid_alignment.cxx.o.d -o CMakeFiles/grid_alignment.dir/grid_alignment.cxx.o -c /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/test/grid_alignment.cxx

test/CMakeFiles/grid_alignment.dir/grid_alignment.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grid_alignment.dir/grid_alignment.cxx.i"
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/test/grid_alignment.cxx > CMakeFiles/grid_alignment.dir/grid_alignment.cxx.i

test/CMakeFiles/grid_alignment.dir/grid_alignment.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grid_alignment.dir/grid_alignment.cxx.s"
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/test/grid_alignment.cxx -o CMakeFiles/grid_alignment.dir/grid_alignment.cxx.s

# Object files for target grid_alignment
grid_alignment_OBJECTS = \
"CMakeFiles/grid_alignment.dir/grid_alignment.cxx.o"

# External object files for target grid_alignment
grid_alignment_EXTERNAL_OBJECTS =

bin/test/grid_alignment: test/CMakeFiles/grid_alignment.dir/grid_alignment.cxx.o
bin/test/grid_alignment: test/CMakeFiles/grid_alignment.dir/build.make
bin/test/grid_alignment: lib/libfltk.a
bin/test/grid_alignment: /usr/lib/x86_64-linux-gnu/libdl.a
bin/test/grid_alignment: /usr/lib/x86_64-linux-gnu/libSM.so
bin/test/grid_alignment: /usr/lib/x86_64-linux-gnu/libICE.so
bin/test/grid_alignment: /usr/lib/x86_64-linux-gnu/libX11.so
bin/test/grid_alignment: /usr/lib/x86_64-linux-gnu/libXext.so
bin/test/grid_alignment: /usr/lib/x86_64-linux-gnu/libXinerama.so
bin/test/grid_alignment: /usr/lib/x86_64-linux-gnu/libXfixes.so
bin/test/grid_alignment: /usr/lib/x86_64-linux-gnu/libXcursor.so
bin/test/grid_alignment: /usr/lib/x86_64-linux-gnu/libXrender.so
bin/test/grid_alignment: /usr/lib/x86_64-linux-gnu/libXft.so
bin/test/grid_alignment: /usr/lib/x86_64-linux-gnu/libfontconfig.so
bin/test/grid_alignment: test/CMakeFiles/grid_alignment.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test/grid_alignment"
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/grid_alignment.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/grid_alignment.dir/build: bin/test/grid_alignment
.PHONY : test/CMakeFiles/grid_alignment.dir/build

test/CMakeFiles/grid_alignment.dir/clean:
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test && $(CMAKE_COMMAND) -P CMakeFiles/grid_alignment.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/grid_alignment.dir/clean

test/CMakeFiles/grid_alignment.dir/depend:
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/test /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test/CMakeFiles/grid_alignment.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/grid_alignment.dir/depend

