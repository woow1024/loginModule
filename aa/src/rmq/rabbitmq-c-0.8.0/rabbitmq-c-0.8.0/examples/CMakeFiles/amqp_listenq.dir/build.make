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
include examples/CMakeFiles/amqp_listenq.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/amqp_listenq.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/amqp_listenq.dir/flags.make

examples/CMakeFiles/amqp_listenq.dir/amqp_listenq.c.o: examples/CMakeFiles/amqp_listenq.dir/flags.make
examples/CMakeFiles/amqp_listenq.dir/amqp_listenq.c.o: examples/amqp_listenq.c
	$(CMAKE_COMMAND) -E cmake_progress_report /opt/rabbitmq-c-0.8.0/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/CMakeFiles/amqp_listenq.dir/amqp_listenq.c.o"
	cd /opt/rabbitmq-c-0.8.0/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/amqp_listenq.dir/amqp_listenq.c.o   -c /opt/rabbitmq-c-0.8.0/examples/amqp_listenq.c

examples/CMakeFiles/amqp_listenq.dir/amqp_listenq.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/amqp_listenq.dir/amqp_listenq.c.i"
	cd /opt/rabbitmq-c-0.8.0/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /opt/rabbitmq-c-0.8.0/examples/amqp_listenq.c > CMakeFiles/amqp_listenq.dir/amqp_listenq.c.i

examples/CMakeFiles/amqp_listenq.dir/amqp_listenq.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/amqp_listenq.dir/amqp_listenq.c.s"
	cd /opt/rabbitmq-c-0.8.0/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /opt/rabbitmq-c-0.8.0/examples/amqp_listenq.c -o CMakeFiles/amqp_listenq.dir/amqp_listenq.c.s

examples/CMakeFiles/amqp_listenq.dir/amqp_listenq.c.o.requires:
.PHONY : examples/CMakeFiles/amqp_listenq.dir/amqp_listenq.c.o.requires

examples/CMakeFiles/amqp_listenq.dir/amqp_listenq.c.o.provides: examples/CMakeFiles/amqp_listenq.dir/amqp_listenq.c.o.requires
	$(MAKE) -f examples/CMakeFiles/amqp_listenq.dir/build.make examples/CMakeFiles/amqp_listenq.dir/amqp_listenq.c.o.provides.build
.PHONY : examples/CMakeFiles/amqp_listenq.dir/amqp_listenq.c.o.provides

examples/CMakeFiles/amqp_listenq.dir/amqp_listenq.c.o.provides.build: examples/CMakeFiles/amqp_listenq.dir/amqp_listenq.c.o

examples/CMakeFiles/amqp_listenq.dir/utils.c.o: examples/CMakeFiles/amqp_listenq.dir/flags.make
examples/CMakeFiles/amqp_listenq.dir/utils.c.o: examples/utils.c
	$(CMAKE_COMMAND) -E cmake_progress_report /opt/rabbitmq-c-0.8.0/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/CMakeFiles/amqp_listenq.dir/utils.c.o"
	cd /opt/rabbitmq-c-0.8.0/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/amqp_listenq.dir/utils.c.o   -c /opt/rabbitmq-c-0.8.0/examples/utils.c

examples/CMakeFiles/amqp_listenq.dir/utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/amqp_listenq.dir/utils.c.i"
	cd /opt/rabbitmq-c-0.8.0/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /opt/rabbitmq-c-0.8.0/examples/utils.c > CMakeFiles/amqp_listenq.dir/utils.c.i

examples/CMakeFiles/amqp_listenq.dir/utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/amqp_listenq.dir/utils.c.s"
	cd /opt/rabbitmq-c-0.8.0/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /opt/rabbitmq-c-0.8.0/examples/utils.c -o CMakeFiles/amqp_listenq.dir/utils.c.s

examples/CMakeFiles/amqp_listenq.dir/utils.c.o.requires:
.PHONY : examples/CMakeFiles/amqp_listenq.dir/utils.c.o.requires

examples/CMakeFiles/amqp_listenq.dir/utils.c.o.provides: examples/CMakeFiles/amqp_listenq.dir/utils.c.o.requires
	$(MAKE) -f examples/CMakeFiles/amqp_listenq.dir/build.make examples/CMakeFiles/amqp_listenq.dir/utils.c.o.provides.build
.PHONY : examples/CMakeFiles/amqp_listenq.dir/utils.c.o.provides

examples/CMakeFiles/amqp_listenq.dir/utils.c.o.provides.build: examples/CMakeFiles/amqp_listenq.dir/utils.c.o

examples/CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.o: examples/CMakeFiles/amqp_listenq.dir/flags.make
examples/CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.o: examples/unix/platform_utils.c
	$(CMAKE_COMMAND) -E cmake_progress_report /opt/rabbitmq-c-0.8.0/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.o"
	cd /opt/rabbitmq-c-0.8.0/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.o   -c /opt/rabbitmq-c-0.8.0/examples/unix/platform_utils.c

examples/CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.i"
	cd /opt/rabbitmq-c-0.8.0/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /opt/rabbitmq-c-0.8.0/examples/unix/platform_utils.c > CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.i

examples/CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.s"
	cd /opt/rabbitmq-c-0.8.0/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /opt/rabbitmq-c-0.8.0/examples/unix/platform_utils.c -o CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.s

examples/CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.o.requires:
.PHONY : examples/CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.o.requires

examples/CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.o.provides: examples/CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.o.requires
	$(MAKE) -f examples/CMakeFiles/amqp_listenq.dir/build.make examples/CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.o.provides.build
.PHONY : examples/CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.o.provides

examples/CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.o.provides.build: examples/CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.o

# Object files for target amqp_listenq
amqp_listenq_OBJECTS = \
"CMakeFiles/amqp_listenq.dir/amqp_listenq.c.o" \
"CMakeFiles/amqp_listenq.dir/utils.c.o" \
"CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.o"

# External object files for target amqp_listenq
amqp_listenq_EXTERNAL_OBJECTS =

examples/amqp_listenq: examples/CMakeFiles/amqp_listenq.dir/amqp_listenq.c.o
examples/amqp_listenq: examples/CMakeFiles/amqp_listenq.dir/utils.c.o
examples/amqp_listenq: examples/CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.o
examples/amqp_listenq: examples/CMakeFiles/amqp_listenq.dir/build.make
examples/amqp_listenq: librabbitmq/librabbitmq.so.4.2.0
examples/amqp_listenq: /usr/lib64/libssl.so
examples/amqp_listenq: /usr/lib64/libcrypto.so
examples/amqp_listenq: examples/CMakeFiles/amqp_listenq.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable amqp_listenq"
	cd /opt/rabbitmq-c-0.8.0/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/amqp_listenq.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/amqp_listenq.dir/build: examples/amqp_listenq
.PHONY : examples/CMakeFiles/amqp_listenq.dir/build

examples/CMakeFiles/amqp_listenq.dir/requires: examples/CMakeFiles/amqp_listenq.dir/amqp_listenq.c.o.requires
examples/CMakeFiles/amqp_listenq.dir/requires: examples/CMakeFiles/amqp_listenq.dir/utils.c.o.requires
examples/CMakeFiles/amqp_listenq.dir/requires: examples/CMakeFiles/amqp_listenq.dir/unix/platform_utils.c.o.requires
.PHONY : examples/CMakeFiles/amqp_listenq.dir/requires

examples/CMakeFiles/amqp_listenq.dir/clean:
	cd /opt/rabbitmq-c-0.8.0/examples && $(CMAKE_COMMAND) -P CMakeFiles/amqp_listenq.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/amqp_listenq.dir/clean

examples/CMakeFiles/amqp_listenq.dir/depend:
	cd /opt/rabbitmq-c-0.8.0 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /opt/rabbitmq-c-0.8.0 /opt/rabbitmq-c-0.8.0/examples /opt/rabbitmq-c-0.8.0 /opt/rabbitmq-c-0.8.0/examples /opt/rabbitmq-c-0.8.0/examples/CMakeFiles/amqp_listenq.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/amqp_listenq.dir/depend

