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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/rabbitmq-c-0.8.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/rabbitmq-c-0.8.0/librabbitmq

# Include any dependencies generated for this target.
include tests/CMakeFiles/test_status_enum.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/test_status_enum.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/test_status_enum.dir/flags.make

tests/CMakeFiles/test_status_enum.dir/test_status_enum.c.o: tests/CMakeFiles/test_status_enum.dir/flags.make
tests/CMakeFiles/test_status_enum.dir/test_status_enum.c.o: ../tests/test_status_enum.c
	$(CMAKE_COMMAND) -E cmake_progress_report /root/rabbitmq-c-0.8.0/librabbitmq/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object tests/CMakeFiles/test_status_enum.dir/test_status_enum.c.o"
	cd /root/rabbitmq-c-0.8.0/librabbitmq/tests && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/test_status_enum.dir/test_status_enum.c.o   -c /root/rabbitmq-c-0.8.0/tests/test_status_enum.c

tests/CMakeFiles/test_status_enum.dir/test_status_enum.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_status_enum.dir/test_status_enum.c.i"
	cd /root/rabbitmq-c-0.8.0/librabbitmq/tests && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /root/rabbitmq-c-0.8.0/tests/test_status_enum.c > CMakeFiles/test_status_enum.dir/test_status_enum.c.i

tests/CMakeFiles/test_status_enum.dir/test_status_enum.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_status_enum.dir/test_status_enum.c.s"
	cd /root/rabbitmq-c-0.8.0/librabbitmq/tests && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /root/rabbitmq-c-0.8.0/tests/test_status_enum.c -o CMakeFiles/test_status_enum.dir/test_status_enum.c.s

tests/CMakeFiles/test_status_enum.dir/test_status_enum.c.o.requires:
.PHONY : tests/CMakeFiles/test_status_enum.dir/test_status_enum.c.o.requires

tests/CMakeFiles/test_status_enum.dir/test_status_enum.c.o.provides: tests/CMakeFiles/test_status_enum.dir/test_status_enum.c.o.requires
	$(MAKE) -f tests/CMakeFiles/test_status_enum.dir/build.make tests/CMakeFiles/test_status_enum.dir/test_status_enum.c.o.provides.build
.PHONY : tests/CMakeFiles/test_status_enum.dir/test_status_enum.c.o.provides

tests/CMakeFiles/test_status_enum.dir/test_status_enum.c.o.provides.build: tests/CMakeFiles/test_status_enum.dir/test_status_enum.c.o

# Object files for target test_status_enum
test_status_enum_OBJECTS = \
"CMakeFiles/test_status_enum.dir/test_status_enum.c.o"

# External object files for target test_status_enum
test_status_enum_EXTERNAL_OBJECTS =

tests/test_status_enum: tests/CMakeFiles/test_status_enum.dir/test_status_enum.c.o
tests/test_status_enum: tests/CMakeFiles/test_status_enum.dir/build.make
tests/test_status_enum: librabbitmq/librabbitmq.a
tests/test_status_enum: /usr/lib64/libssl.so
tests/test_status_enum: /usr/lib64/libcrypto.so
tests/test_status_enum: tests/CMakeFiles/test_status_enum.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable test_status_enum"
	cd /root/rabbitmq-c-0.8.0/librabbitmq/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_status_enum.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/test_status_enum.dir/build: tests/test_status_enum
.PHONY : tests/CMakeFiles/test_status_enum.dir/build

tests/CMakeFiles/test_status_enum.dir/requires: tests/CMakeFiles/test_status_enum.dir/test_status_enum.c.o.requires
.PHONY : tests/CMakeFiles/test_status_enum.dir/requires

tests/CMakeFiles/test_status_enum.dir/clean:
	cd /root/rabbitmq-c-0.8.0/librabbitmq/tests && $(CMAKE_COMMAND) -P CMakeFiles/test_status_enum.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test_status_enum.dir/clean

tests/CMakeFiles/test_status_enum.dir/depend:
	cd /root/rabbitmq-c-0.8.0/librabbitmq && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/rabbitmq-c-0.8.0 /root/rabbitmq-c-0.8.0/tests /root/rabbitmq-c-0.8.0/librabbitmq /root/rabbitmq-c-0.8.0/librabbitmq/tests /root/rabbitmq-c-0.8.0/librabbitmq/tests/CMakeFiles/test_status_enum.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/test_status_enum.dir/depend

