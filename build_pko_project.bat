
@echo off
SETLOCAL
:: Configuration
set PKO_VERSION=v0.0.1
set OGRE_BRANCH_NAME=master
set GENERATOR="Visual Studio 16 2019"
set PLATFORM=x64
set BUILD_TYPE="Release"
set PKO_INSTALL="%~dp0INSTALL_PKO_Next_%OGRE_BRANCH_NAME%_%PKO_VERSION%_%PLATFORM%"

set CMAKE_BIN_x86="C:\Program Files (x86)\CMake\bin\cmake.exe"
set CMAKE_BIN_x64="C:\Program Files\CMake\bin\cmake.exe"
IF EXIST %CMAKE_BIN_x64% (
	echo CMake 64-bit detected
	set CMAKE_BIN=%CMAKE_BIN_x64%
) ELSE (
	IF EXIST %CMAKE_BIN_x86% (
		echo CMake 32-bit detected
		set CMAKE_BIN=%CMAKE_BIN_x86%
	) ELSE (
		echo Cannot detect either %CMAKE_BIN_x86% or
		echo %CMAKE_BIN_x64% make sure CMake is installed
		EXIT /B 1
	)
)
echo Using CMake at %CMAKE_BIN%


cd PKO_Project

IF NOT EXIST Dependencies (
    mkdir Dependencies
    cd Dependencies
    IF NOT EXIST Ogre (

        mklink /D Ogre ..\..\Ogre\ogre-next
        IF ERRORLEVEL 1 (
            echo Failed to create Dependency directory symlink. Run the script as Administrator.
            EXIT /B 1
        )
    )
    cd ..
	
)


IF NOT EXIST build (
	mkdir build
)

cd build
echo --- Running CMake configure ---

%CMAKE_BIN% -D CMAKE_INSTALL_PREFIX=%PKO_INSTALL% -D CMAKE_BUILD_TYPE=%BUILD_TYPE% -G %GENERATOR% -A %PLATFORM% ..

%CMAKE_BIN% --build . --config %BUILD_TYPE%
%CMAKE_BIN% --build . --target install --config %BUILD_TYPE%


echo Done!

ENDLOCAL
