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
include test/CMakeFiles/gl_overlay.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/gl_overlay.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/gl_overlay.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/gl_overlay.dir/flags.make

test/CMakeFiles/gl_overlay.dir/gl_overlay.cxx.o: test/CMakeFiles/gl_overlay.dir/flags.make
test/CMakeFiles/gl_overlay.dir/gl_overlay.cxx.o: ../test/gl_overlay.cxx
test/CMakeFiles/gl_overlay.dir/gl_overlay.cxx.o: test/CMakeFiles/gl_overlay.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/gl_overlay.dir/gl_overlay.cxx.o"
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/gl_overlay.dir/gl_overlay.cxx.o -MF CMakeFiles/gl_overlay.dir/gl_overlay.cxx.o.d -o CMakeFiles/gl_overlay.dir/gl_overlay.cxx.o -c /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/test/gl_overlay.cxx

test/CMakeFiles/gl_overlay.dir/gl_overlay.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gl_overlay.dir/gl_overlay.cxx.i"
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/test/gl_overlay.cxx > CMakeFiles/gl_overlay.dir/gl_overlay.cxx.i

test/CMakeFiles/gl_overlay.dir/gl_overlay.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gl_overlay.dir/gl_overlay.cxx.s"
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/test/gl_overlay.cxx -o CMakeFiles/gl_overlay.dir/gl_overlay.cxx.s

# Object files for target gl_overlay
gl_overlay_OBJECTS = \
"CMakeFiles/gl_overlay.dir/gl_overlay.cxx.o"

# External object files for target gl_overlay
gl_overlay_EXTERNAL_OBJECTS =

bin/test/gl_overlay: test/CMakeFiles/gl_overlay.dir/gl_overlay.cxx.o
bin/test/gl_overlay: test/CMakeFiles/gl_overlay.dir/build.make
bin/test/gl_overlay: lib/libfltk_gl.a
bin/test/gl_overlay: lib/libfltk.a
bin/test/gl_overlay: /usr/lib/x86_64-linux-gnu/libdl.a
bin/test/gl_overlay: /usr/lib/x86_64-linux-gnu/libSM.so
bin/test/gl_overlay: /usr/lib/x86_64-linux-gnu/libICE.so
bin/test/gl_overlay: /usr/lib/x86_64-linux-gnu/libX11.so
bin/test/gl_overlay: /usr/lib/x86_64-linux-gnu/libXext.so
bin/test/gl_overlay: /usr/lib/x86_64-linux-gnu/libXinerama.so
bin/test/gl_overlay: /usr/lib/x86_64-linux-gnu/libXfixes.so
bin/test/gl_overlay: /usr/lib/x86_64-linux-gnu/libXcursor.so
bin/test/gl_overlay: /usr/lib/x86_64-linux-gnu/libXrender.so
bin/test/gl_overlay: /usr/lib/x86_64-linux-gnu/libXft.so
bin/test/gl_overlay: /usr/lib/x86_64-linux-gnu/libfontconfig.so
bin/test/gl_overlay: test/CMakeFiles/gl_overlay.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test/gl_overlay"
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gl_overlay.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/gl_overlay.dir/build: bin/test/gl_overlay
.PHONY : test/CMakeFiles/gl_overlay.dir/build

test/CMakeFiles/gl_overlay.dir/clean:
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test && $(CMAKE_COMMAND) -P CMakeFiles/gl_overlay.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/gl_overlay.dir/clean

test/CMakeFiles/gl_overlay.dir/depend:
	cd /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/test /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test /home/thomas/Bureau/ProjetImage/ProjetImage_IMAGINE_BES_CARO_NOYE/Code/fltk/build/test/CMakeFiles/gl_overlay.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/gl_overlay.dir/depend

