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
include tests\common\TestRunner\CMakeFiles\test_runner.dir\depend.make

# Include the progress variables for this target.
include tests\common\TestRunner\CMakeFiles\test_runner.dir\progress.make

# Include the compile flags for this target's objects.
include tests\common\TestRunner\CMakeFiles\test_runner.dir\flags.make

tests\common\TestRunner\CMakeFiles\test_runner.dir\test_runner.cpp.obj: tests\common\TestRunner\CMakeFiles\test_runner.dir\flags.make
tests\common\TestRunner\CMakeFiles\test_runner.dir\test_runner.cpp.obj: tests\common\TestRunner\test_runner.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\workspace\Repos\arcticpanther\Cleric\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/common/TestRunner/CMakeFiles/test_runner.dir/test_runner.cpp.obj"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\test_runner.dir\test_runner.cpp.obj /FdCMakeFiles\test_runner.dir\ /FS -c C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner\test_runner.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\common\TestRunner\CMakeFiles\test_runner.dir\test_runner.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_runner.dir/test_runner.cpp.i"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  > CMakeFiles\test_runner.dir\test_runner.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner\test_runner.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\common\TestRunner\CMakeFiles\test_runner.dir\test_runner.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_runner.dir/test_runner.cpp.s"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\test_runner.dir\test_runner.cpp.s /c C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner\test_runner.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\common\TestRunner\CMakeFiles\test_runner.dir\test_runner.cpp.obj.requires:

.PHONY : tests\common\TestRunner\CMakeFiles\test_runner.dir\test_runner.cpp.obj.requires

tests\common\TestRunner\CMakeFiles\test_runner.dir\test_runner.cpp.obj.provides: tests\common\TestRunner\CMakeFiles\test_runner.dir\test_runner.cpp.obj.requires
	$(MAKE) -f tests\common\TestRunner\CMakeFiles\test_runner.dir\build.make /nologo -$(MAKEFLAGS) tests\common\TestRunner\CMakeFiles\test_runner.dir\test_runner.cpp.obj.provides.build
.PHONY : tests\common\TestRunner\CMakeFiles\test_runner.dir\test_runner.cpp.obj.provides

tests\common\TestRunner\CMakeFiles\test_runner.dir\test_runner.cpp.obj.provides.build: tests\common\TestRunner\CMakeFiles\test_runner.dir\test_runner.cpp.obj


tests\common\TestRunner\CMakeFiles\test_runner.dir\test_module_loader.cpp.obj: tests\common\TestRunner\CMakeFiles\test_runner.dir\flags.make
tests\common\TestRunner\CMakeFiles\test_runner.dir\test_module_loader.cpp.obj: tests\common\TestRunner\test_module_loader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\workspace\Repos\arcticpanther\Cleric\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tests/common/TestRunner/CMakeFiles/test_runner.dir/test_module_loader.cpp.obj"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\test_runner.dir\test_module_loader.cpp.obj /FdCMakeFiles\test_runner.dir\ /FS -c C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner\test_module_loader.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\common\TestRunner\CMakeFiles\test_runner.dir\test_module_loader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_runner.dir/test_module_loader.cpp.i"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  > CMakeFiles\test_runner.dir\test_module_loader.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner\test_module_loader.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\common\TestRunner\CMakeFiles\test_runner.dir\test_module_loader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_runner.dir/test_module_loader.cpp.s"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner
	C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\cl.exe  @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\test_runner.dir\test_module_loader.cpp.s /c C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner\test_module_loader.cpp
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

tests\common\TestRunner\CMakeFiles\test_runner.dir\test_module_loader.cpp.obj.requires:

.PHONY : tests\common\TestRunner\CMakeFiles\test_runner.dir\test_module_loader.cpp.obj.requires

tests\common\TestRunner\CMakeFiles\test_runner.dir\test_module_loader.cpp.obj.provides: tests\common\TestRunner\CMakeFiles\test_runner.dir\test_module_loader.cpp.obj.requires
	$(MAKE) -f tests\common\TestRunner\CMakeFiles\test_runner.dir\build.make /nologo -$(MAKEFLAGS) tests\common\TestRunner\CMakeFiles\test_runner.dir\test_module_loader.cpp.obj.provides.build
.PHONY : tests\common\TestRunner\CMakeFiles\test_runner.dir\test_module_loader.cpp.obj.provides

tests\common\TestRunner\CMakeFiles\test_runner.dir\test_module_loader.cpp.obj.provides.build: tests\common\TestRunner\CMakeFiles\test_runner.dir\test_module_loader.cpp.obj


# Object files for target test_runner
test_runner_OBJECTS = \
"CMakeFiles\test_runner.dir\test_runner.cpp.obj" \
"CMakeFiles\test_runner.dir\test_module_loader.cpp.obj"

# External object files for target test_runner
test_runner_EXTERNAL_OBJECTS =

Binaries\test_runner.exe: tests\common\TestRunner\CMakeFiles\test_runner.dir\test_runner.cpp.obj
Binaries\test_runner.exe: tests\common\TestRunner\CMakeFiles\test_runner.dir\test_module_loader.cpp.obj
Binaries\test_runner.exe: tests\common\TestRunner\CMakeFiles\test_runner.dir\build.make
Binaries\test_runner.exe: Binaries\unittestpp.lib
Binaries\test_runner.exe: Binaries\cpprest_2_10.lib
Binaries\test_runner.exe: tests\common\TestRunner\CMakeFiles\test_runner.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\workspace\Repos\arcticpanther\Cleric\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ..\..\..\Binaries\test_runner.exe"
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner
	"C:\Program Files (x86)\CMake\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\test_runner.dir --manifests  -- C:\PROGRA~2\MIB055~1\2017\COMMUN~1\VC\Tools\MSVC\1412~1.258\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\test_runner.dir\objects1.rsp @<<
 /out:..\..\..\Binaries\test_runner.exe /implib:..\..\..\Binaries\test_runner.lib /pdb:C:\workspace\Repos\arcticpanther\Cleric\Binaries\test_runner.pdb /version:0.0   /machine:X86 /debug /INCREMENTAL /subsystem:console  -LIBPATH:C:\workspace\Repos\arcticpanther\Cleric\thirdparty\boost_1_66_0\lib  -LIBPATH:C:\workspace\Repos\arcticpanther\Cleric\thirdparty\openssl-1.0.2n\lib  ..\..\..\Binaries\unittestpp.lib ..\..\..\Binaries\cpprest_2_10.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<
	cd C:\workspace\Repos\arcticpanther\Cleric

# Rule to build all files generated by this target.
tests\common\TestRunner\CMakeFiles\test_runner.dir\build: Binaries\test_runner.exe

.PHONY : tests\common\TestRunner\CMakeFiles\test_runner.dir\build

tests\common\TestRunner\CMakeFiles\test_runner.dir\requires: tests\common\TestRunner\CMakeFiles\test_runner.dir\test_runner.cpp.obj.requires
tests\common\TestRunner\CMakeFiles\test_runner.dir\requires: tests\common\TestRunner\CMakeFiles\test_runner.dir\test_module_loader.cpp.obj.requires

.PHONY : tests\common\TestRunner\CMakeFiles\test_runner.dir\requires

tests\common\TestRunner\CMakeFiles\test_runner.dir\clean:
	cd C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner
	$(CMAKE_COMMAND) -P CMakeFiles\test_runner.dir\cmake_clean.cmake
	cd C:\workspace\Repos\arcticpanther\Cleric
.PHONY : tests\common\TestRunner\CMakeFiles\test_runner.dir\clean

tests\common\TestRunner\CMakeFiles\test_runner.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\workspace\Repos\arcticpanther\Cleric C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner C:\workspace\Repos\arcticpanther\Cleric C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner C:\workspace\Repos\arcticpanther\Cleric\tests\common\TestRunner\CMakeFiles\test_runner.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : tests\common\TestRunner\CMakeFiles\test_runner.dir\depend

