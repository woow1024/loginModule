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
CMAKE_SOURCE_DIR = /opt/rabbitmq-c-0.8.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /opt/rabbitmq-c-0.8.0

# Include any dependencies generated for this target.
include tests/CMakeFiles/test_hostcheck.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/test_hostcheck.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/test_hostcheck.dir/flags.make

tests/CMakeFiles/test_hostcheck.dir/test_hostcheck.c.o: tests/CMakeFiles/test_hostcheck.dir/flags.make
tests/CMakeFiles/test_hostcheck.dir/test_hostcheck.c.o: tests/test_hostcheck.c
	$(CMAKE_COMMAND) -E cmake_progress_report /opt/rabbitmq-c-0.8.0/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object tests/CMakeFiles/test_hostcheck.dir/test_hostcheck.c.o"
	cd /opt/rabbitmq-c-0.8.0/tests && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/test_hostcheck.dir/test_hostcheck.c.o   -c /opt/rabbitmq-c-0.8.0/tests/test_hostcheck.c

tests/CMakeFiles/test_hostcheck.dir/test_hostcheck.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_hostcheck.dir/test_hostcheck.c.i"
	cd /opt/rabbitmq-c-0.8.0/tests && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /opt/rabbitmq-c-0.8.0/tests/test_hostcheck.c > CMakeFiles/test_hostcheck.dir/test_hostcheck.c.i

tests/CMakeFiles/test_hostcheck.dir/test_hostcheck.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_hostcheck.dir/test_hostcheck.c.s"
	cd /opt/rabbitmq-c-0.8.0/tests && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /opt/rabbitmq-c-0.8.0/tests/test_hostcheck.c -o CMakeFiles/test_hostcheck.dir/test_hostcheck.c.s

tests/CMakeFiles/test_hostcheck.dir/test_hostcheck.c.o.requires:
.PHONY : tests/CMakeFiles/test_hostcheck.dir/test_hostcheck.c.o.requires

tests/CMakeFiles/test_hostcheck.dir/test_hostcheck.c.o.provides: tests/CMakeFiles/test_hostcheck.dir/test_hostcheck.c.o.requires
	$(MAKE) -f tests/CMakeFiles/test_hostcheck.dir/build.make tests/CMakeFiles/test_hostcheck.dir/test_hostcheck.c.o.provides.build
.PHONY : tests/CMakeFiles/test_hostcheck.dir/test_hostcheck.c.o.provides

tests/CMakeFiles/test_hostcheck.dir/test_hostcheck.c.o.provides.build: tests/CMakeFiles/test_hostcheck.dir/test_hostcheck.c.o

tests/CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.o: tests/CMakeFiles/test_hostcheck.dir/flags.make
tests/CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.o: librabbitmq/amqp_hostcheck.c
	$(CMAKE_COMMAND) -E cmake_progress_report /opt/rabbitmq-c-0.8.0/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object tests/CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.o"
	cd /opt/rabbitmq-c-0.8.0/tests && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.o   -c /opt/rabbitmq-c-0.8.0/librabbitmq/amqp_hostcheck.c

tests/CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.i"
	cd /opt/rabbitmq-c-0.8.0/tests && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /opt/rabbitmq-c-0.8.0/librabbitmq/amqp_hostcheck.c > CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.i

tests/CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.s"
	cd /opt/rabbitmq-c-0.8.0/tests && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /opt/rabbitmq-c-0.8.0/librabbitmq/amqp_hostcheck.c -o CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.s

tests/CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.o.requires:
.PHONY : tests/CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.o.requires

tests/CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.o.provides: tests/CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.o.requires
	$(MAKE) -f tests/CMakeFiles/test_hostcheck.dir/build.make tests/CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.o.provides.build
.PHONY : tests/CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.o.provides

tests/CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.o.provides.build: tests/CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.o

# Object files for target test_hostcheck
test_hostcheck_OBJECTS = \
"CMakeFiles/test_hostcheck.dir/test_hostcheck.c.o" \
"CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.o"

# External object files for target test_hostcheck
test_hostcheck_EXTERNAL_OBJECTS =

tests/test_hostcheck: tests/CMakeFiles/test_hostcheck.dir/test_hostcheck.c.o
tests/test_hostcheck: tests/CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.o
tests/test_hostcheck: tests/CMakeFiles/test_hostcheck.dir/build.make
tests/test_hostcheck: tests/CMakeFiles/test_hostcheck.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable test_hostcheck"
	cd /opt/rabbitmq-c-0.8.0/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_hostcheck.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/test_hostcheck.dir/build: tests/test_hostcheck
.PHONY : tests/CMakeFiles/test_hostcheck.dir/build

tests/CMakeFiles/test_hostcheck.dir/requires: tests/CMakeFiles/test_hostcheck.dir/test_hostcheck.c.o.requires
tests/CMakeFiles/test_hostcheck.dir/requires: tests/CMakeFiles/test_hostcheck.dir/__/librabbitmq/amqp_hostcheck.c.o.requires
.PHONY : tests/CMakeFiles/test_hostcheck.dir/requires

tests/CMakeFiles/test_hostcheck.dir/clean:
	cd /opt/rabbitmq-c-0.8.0/tests && $(CMAKE_COMMAND) -P CMakeFiles/test_hostcheck.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test_hostcheck.dir/clean

tests/CMakeFiles/test_hostcheck.dir/depend:
	cd /opt/rabbitmq-c-0.8.0 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /opt/rabbitmq-c-0.8.0 /opt/rabbitmq-c-0.8.0/tests /opt/rabbitmq-c-0.8.0 /opt/rabbitmq-c-0.8.0/tests /opt/rabbitmq-c-0.8.0/tests/CMakeFiles/test_hostcheck.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/test_hostcheck.dir/depend
