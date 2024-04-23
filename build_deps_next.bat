
@echo off
SETLOCAL
:: Configuration
set OGRE_BRANCH_NAME=master
set GENERATOR="Visual Studio 16 2019"
set PLATFORM=x64
set BUILD_TYPE="Release"

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

@REM set OGRE_DEPS_INSTALL_LIB="%~dp0Ogre\ogre-next-deps\build\ogredeps\lib\%BUILD_TYPE%"
@REM set OGRE_DEPS_INSTALL_INCLUDE="%~dp0Ogre\ogre-next-deps\build\ogredeps\include"
@REM :: Set paths
@REM set PATH=%OGRE_DEPS_INSTALL_LIB%;%OGRE_DEPS_INSTALL_INCLUDE%;%PATH%


IF NOT EXIST Ogre (
	mkdir Ogre
)
cd Ogre

IF NOT EXIST ogre-next-deps (
	mkdir ogre-next-deps
	echo --- Cloning ogre-next-deps ---
	git clone --recurse-submodules --shallow-submodules https://github.com/apiyap/ogre-next-deps
) ELSE (
	echo --- ogre-next-deps repo detected. Cloning skipped ---
)
cd ogre-next-deps

IF EXIST build (
    rd build /S /Q
)

mkdir build

cd build
echo --- Building ogre-next-deps ---
%CMAKE_BIN% -G %GENERATOR% -A %PLATFORM% ..
%CMAKE_BIN% --build . --config Debug
%CMAKE_BIN% --build . --target install --config Debug
%CMAKE_BIN% --build . --config Release
%CMAKE_BIN% --build . --target install --config Release

cd ../../

echo Done!

ENDLOCAL
