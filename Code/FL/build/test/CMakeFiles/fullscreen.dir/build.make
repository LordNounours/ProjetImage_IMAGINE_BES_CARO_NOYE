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
include test/CMakeFiles/fullscreen.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/fullscreen.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/fullscreen.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/fullscreen.dir/flags.make

test/CMakeFiles/fullscreen.dir/fullscreen.cxx.o: test/CMakeFiles/fullscreen.dir/flags.make
test/CMakeFiles/fullscreen.dir/fullscreen.cxx.o: ../test/fullscreen.cxx
test/CMakeFiles/fullscreen.dir/fullscreen.cxx.o: test/CMakeFiles/fullscreen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/fullscreen.dir/fullscreen.cxx.o"
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/fullscreen.dir/fullscreen.cxx.o -MF CMakeFiles/fullscreen.dir/fullscreen.cxx.o.d -o CMakeFiles/fullscreen.dir/fullscreen.cxx.o -c /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/test/fullscreen.cxx

test/CMakeFiles/fullscreen.dir/fullscreen.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fullscreen.dir/fullscreen.cxx.i"
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/test/fullscreen.cxx > CMakeFiles/fullscreen.dir/fullscreen.cxx.i

test/CMakeFiles/fullscreen.dir/fullscreen.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fullscreen.dir/fullscreen.cxx.s"
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/test/fullscreen.cxx -o CMakeFiles/fullscreen.dir/fullscreen.cxx.s

# Object files for target fullscreen
fullscreen_OBJECTS = \
"CMakeFiles/fullscreen.dir/fullscreen.cxx.o"

# External object files for target fullscreen
fullscreen_EXTERNAL_OBJECTS =

bin/test/fullscreen: test/CMakeFiles/fullscreen.dir/fullscreen.cxx.o
bin/test/fullscreen: test/CMakeFiles/fullscreen.dir/build.make
bin/test/fullscreen: lib/libfltk_gl.a
bin/test/fullscreen: lib/libfltk.a
bin/test/fullscreen: /usr/lib/x86_64-linux-gnu/libdl.a
bin/test/fullscreen: /usr/lib/x86_64-linux-gnu/libSM.so
bin/test/fullscreen: /usr/lib/x86_64-linux-gnu/libICE.so
bin/test/fullscreen: /usr/lib/x86_64-linux-gnu/libX11.so
bin/test/fullscreen: /usr/lib/x86_64-linux-gnu/libXext.so
bin/test/fullscreen: /usr/lib/x86_64-linux-gnu/libXinerama.so
bin/test/fullscreen: /usr/lib/x86_64-linux-gnu/libXfixes.so
bin/test/fullscreen: /usr/lib/x86_64-linux-gnu/libXcursor.so
bin/test/fullscreen: /usr/lib/x86_64-linux-gnu/libXrender.so
bin/test/fullscreen: /usr/lib/x86_64-linux-gnu/libXft.so
bin/test/fullscreen: /usr/lib/x86_64-linux-gnu/libfontconfig.so
bin/test/fullscreen: test/CMakeFiles/fullscreen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test/fullscreen"
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fullscreen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/fullscreen.dir/build: bin/test/fullscreen
.PHONY : test/CMakeFiles/fullscreen.dir/build

test/CMakeFiles/fullscreen.dir/clean:
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test && $(CMAKE_COMMAND) -P CMakeFiles/fullscreen.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/fullscreen.dir/clean

test/CMakeFiles/fullscreen.dir/depend:
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/test /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test/CMakeFiles/fullscreen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/fullscreen.dir/depend
