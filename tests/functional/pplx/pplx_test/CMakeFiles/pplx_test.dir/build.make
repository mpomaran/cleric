# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files (x86)\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files (x86)\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\workspace\Repos\arcticpanther\Cleric

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\workspace\Repos\arcticpanther\Cleric

# Include any dependencies generated for this target.
include tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\depend.make

# Include the progress variables for this target.
include tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\progress.make

# Include the compile flags for this target's objects.
include tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\flags.make

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_op_test.cpp.obj: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\flags.make
tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_op_test.cpp.obj: tests\functional\pplx\pplx_test\pplx_op_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\workspace\Repos\arcticpanther\Cleric\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/functional/pplx/pplx_test/CMakeFiles/pplx_test.dir/pplx_op_test.cpp.obj"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\pplx_test.dir\pplx_op_test.cpp.obj /FdCMakeFiles\pplx_test.dir\ /FS -c C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test\pplx_op_test.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_op_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pplx_test.dir/pplx_op_test.cpp.i"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  > CMakeFiles\pplx_test.dir\pplx_op_test.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test\pplx_op_test.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_op_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pplx_test.dir/pplx_op_test.cpp.s"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\pplx_test.dir\pplx_op_test.cpp.s /c C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test\pplx_op_test.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_op_test.cpp.obj.requires:

.PHONY : tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_op_test.cpp.obj.requires

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_op_test.cpp.obj.provides: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_op_test.cpp.obj.requires
	$(MAKE) -f tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\build.make /nologo -$(MAKEFLAGS) tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_op_test.cpp.obj.provides.build
.PHONY : tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_op_test.cpp.obj.provides

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_op_test.cpp.obj.provides.build: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_op_test.cpp.obj


tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_task_options.cpp.obj: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\flags.make
tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_task_options.cpp.obj: tests\functional\pplx\pplx_test\pplx_task_options.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\workspace\Repos\arcticpanther\Cleric\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tests/functional/pplx/pplx_test/CMakeFiles/pplx_test.dir/pplx_task_options.cpp.obj"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\pplx_test.dir\pplx_task_options.cpp.obj /FdCMakeFiles\pplx_test.dir\ /FS -c C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test\pplx_task_options.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_task_options.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pplx_test.dir/pplx_task_options.cpp.i"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  > CMakeFiles\pplx_test.dir\pplx_task_options.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test\pplx_task_options.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_task_options.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pplx_test.dir/pplx_task_options.cpp.s"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\pplx_test.dir\pplx_task_options.cpp.s /c C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test\pplx_task_options.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_task_options.cpp.obj.requires:

.PHONY : tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_task_options.cpp.obj.requires

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_task_options.cpp.obj.provides: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_task_options.cpp.obj.requires
	$(MAKE) -f tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\build.make /nologo -$(MAKEFLAGS) tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_task_options.cpp.obj.provides.build
.PHONY : tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_task_options.cpp.obj.provides

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_task_options.cpp.obj.provides.build: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_task_options.cpp.obj


tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplxtask_tests.cpp.obj: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\flags.make
tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplxtask_tests.cpp.obj: tests\functional\pplx\pplx_test\pplxtask_tests.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\workspace\Repos\arcticpanther\Cleric\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object tests/functional/pplx/pplx_test/CMakeFiles/pplx_test.dir/pplxtask_tests.cpp.obj"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\pplx_test.dir\pplxtask_tests.cpp.obj /FdCMakeFiles\pplx_test.dir\ /FS -c C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test\pplxtask_tests.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplxtask_tests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pplx_test.dir/pplxtask_tests.cpp.i"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  > CMakeFiles\pplx_test.dir\pplxtask_tests.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test\pplxtask_tests.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplxtask_tests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pplx_test.dir/pplxtask_tests.cpp.s"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\pplx_test.dir\pplxtask_tests.cpp.s /c C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test\pplxtask_tests.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplxtask_tests.cpp.obj.requires:

.PHONY : tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplxtask_tests.cpp.obj.requires

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplxtask_tests.cpp.obj.provides: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplxtask_tests.cpp.obj.requires
	$(MAKE) -f tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\build.make /nologo -$(MAKEFLAGS) tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplxtask_tests.cpp.obj.provides.build
.PHONY : tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplxtask_tests.cpp.obj.provides

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplxtask_tests.cpp.obj.provides.build: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplxtask_tests.cpp.obj


tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\stdafx.cpp.obj: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\flags.make
tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\stdafx.cpp.obj: tests\functional\pplx\pplx_test\stdafx.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\workspace\Repos\arcticpanther\Cleric\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object tests/functional/pplx/pplx_test/CMakeFiles/pplx_test.dir/stdafx.cpp.obj"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\pplx_test.dir\stdafx.cpp.obj /FdCMakeFiles\pplx_test.dir\ /FS -c C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test\stdafx.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\stdafx.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pplx_test.dir/stdafx.cpp.i"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  > CMakeFiles\pplx_test.dir\stdafx.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test\stdafx.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\stdafx.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pplx_test.dir/stdafx.cpp.s"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\pplx_test.dir\stdafx.cpp.s /c C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test\stdafx.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\stdafx.cpp.obj.requires:

.PHONY : tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\stdafx.cpp.obj.requires

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\stdafx.cpp.obj.provides: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\stdafx.cpp.obj.requires
	$(MAKE) -f tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\build.make /nologo -$(MAKEFLAGS) tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\stdafx.cpp.obj.provides.build
.PHONY : tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\stdafx.cpp.obj.provides

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\stdafx.cpp.obj.provides.build: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\stdafx.cpp.obj


# Object files for target pplx_test
pplx_test_OBJECTS = \
"CMakeFiles\pplx_test.dir\pplx_op_test.cpp.obj" \
"CMakeFiles\pplx_test.dir\pplx_task_options.cpp.obj" \
"CMakeFiles\pplx_test.dir\pplxtask_tests.cpp.obj" \
"CMakeFiles\pplx_test.dir\stdafx.cpp.obj"

# External object files for target pplx_test
pplx_test_EXTERNAL_OBJECTS =

Binaries\pplx_test.dll: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_op_test.cpp.obj
Binaries\pplx_test.dll: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_task_options.cpp.obj
Binaries\pplx_test.dll: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplxtask_tests.cpp.obj
Binaries\pplx_test.dll: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\stdafx.cpp.obj
Binaries\pplx_test.dll: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\build.make
Binaries\pplx_test.dll: Binaries\common_utilities.lib
Binaries\pplx_test.dll: Binaries\unittestpp.lib
Binaries\pplx_test.dll: Binaries\cpprest_2_10.lib
Binaries\pplx_test.dll: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\workspace\Repos\arcticpanther\Cleric\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX shared library ..\..\..\..\Binaries\pplx_test.dll"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test
	"C:\Program Files (x86)\CMake\bin\cmake.exe" -E vs_link_dll --intdir=CMakeFiles\pplx_test.dir --manifests  -- C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\pplx_test.dir\objects1.rsp @<<
 /out:..\..\..\..\Binaries\pplx_test.dll /implib:..\..\..\..\Binaries\pplx_test.lib /pdb:C:\workspace\Repos\arcticpanther\Cleric\Binaries\pplx_test.pdb /dll /version:0.0  /machine:X86 /debug /INCREMENTAL  -LIBPATH:C:\workspace\Repos\arcticpanther\Cleric\thirdparty\boost_1_66_0\lib  -LIBPATH:C:\workspace\Repos\arcticpanther\Cleric\thirdparty\openssl-1.0.2n\lib ..\..\..\..\Binaries\common_utilities.lib ..\..\..\..\Binaries\unittestpp.lib ..\..\..\..\Binaries\cpprest_2_10.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib  
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

# Rule to build all files generated by this target.
tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\build: Binaries\pplx_test.dll

.PHONY : tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\build

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\requires: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_op_test.cpp.obj.requires
tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\requires: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplx_task_options.cpp.obj.requires
tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\requires: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\pplxtask_tests.cpp.obj.requires
tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\requires: tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\stdafx.cpp.obj.requires

.PHONY : tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\requires

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\clean:
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test
	$(CMAKE_COMMAND) -P CMakeFiles\pplx_test.dir\cmake_clean.cmake
	cd C:\workspace\Repos\arcticpanther\Cleric
.PHONY : tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\clean

tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\workspace\Repos\arcticpanther\Cleric C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test C:\workspace\Repos\arcticpanther\Cleric C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test C:\workspace\Repos\arcticpanther\Cleric\tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : tests\functional\pplx\pplx_test\CMakeFiles\pplx_test.dir\depend

