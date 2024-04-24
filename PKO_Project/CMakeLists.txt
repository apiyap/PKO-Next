#This scripts will add all the cpp and h files under src and include folders, and
#assumes that your Ogre source code is in Dependencies/Ogre and that:
# In Windows you built Ogre into Dependencies/Ogre/build
# In Linux you built Release into Dependencies/Ogre/build/Release
# In Linux you built Debug into Dependencies/Ogre/build/Debug
#
# If your source code is not at "Dependencies/Ogre"; you can use "mklink /D" to create
# a symbolic link to where the source code is located on Windows.
# On Linux, you can use "ln -s"

#set( CMAKE_TOOLCHAIN_FILE CMake/iOS.cmake )

cmake_minimum_required( VERSION 3.5.1 )
project( PKO_Project )


include( CMake/Bootstrap.cmake )
include( CMake/Dependencies/OGRE.cmake )


if( APPLE )
	set( CMAKE_CXX_STANDARD 11 )
endif()

setupOgre( OGRE_SOURCE, OGRE_BINARIES, OGRE_LIBRARIES, FALSE, FALSE )

# Setup our application
set( EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin" )
if( MSVC )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	if( NOT PLATFORM_X64 )
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
	endif()
	add_definitions( -DUNICODE -D_UNICODE )
endif()

if( APPLE )
	macro( add_recursive dir retVal )
		file( GLOB_RECURSE ${retVal} ${dir}/*.h ${dir}/*.cpp ${dir}/*.c ${dir}/*.mm ${dir}/*.m )
	endmacro()
else()
	macro( add_recursive dir retVal )
		file( GLOB_RECURSE ${retVal} ${dir}/*.h ${dir}/*.cpp ${dir}/*.c )
	endmacro()
endif()

include_directories( "./include" )
include_directories( "./include/ImguiOgre" )
# Ogre doesn't need this include, but we do because of Rapidjson in UnitTesting.cpp
include_directories( "./Dependencies/Ogre/Dependencies/include" )

add_recursive( ./src SOURCES )
add_recursive( ./include HEADERS )

set(BUILD_COMMAND_OPTS --target install --config ${CMAKE_BUILD_TYPE})
set(BUILD_COMMAND_COMMON ${CMAKE_COMMAND}
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  -G ${CMAKE_GENERATOR}
  -DCMAKE_INSTALL_PREFIX="${CMAKE_CURRENT_SOURCE_DIR}/pkodeps"
  -DCMAKE_GENERATOR_PLATFORM=${CMAKE_GENERATOR_PLATFORM}
  -DCMAKE_GENERATOR_TOOLSET=${CMAKE_GENERATOR_TOOLSET}
  -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
  -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE # allow linking into a shared lib
  )

  if(MSVC OR MINGW OR SKBUILD) # other platforms dont need this

  if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/pkodeps")
  message(STATUS "Create directory ${CMAKE_CURRENT_SOURCE_DIR}/pkodeps")
  	file( MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pkodeps" )
  endif()
#   message(STATUS "Building Assimp")
#   file(DOWNLOAD
# 		  https://github.com/assimp/assimp/archive/refs/tags/v5.2.5.zip
# 	  ${PROJECT_BINARY_DIR}/v5.2.5.tar.gz)
#   execute_process(COMMAND ${CMAKE_COMMAND}
# 	  -E tar xf v5.2.5.tar.gz WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
#   execute_process(COMMAND ${BUILD_COMMAND_COMMON}
# 	  -DZLIB_ROOT=${OGREDEPS_PATH}
# 	  -DBUILD_SHARED_LIBS=OFF
# 	  -DASSIMP_BUILD_TESTS=OFF
# 	  -DASSIMP_NO_EXPORT=TRUE
# 	  -DASSIMP_BUILD_OGRE_IMPORTER=OFF
# 	  -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
# 	  ${PROJECT_BINARY_DIR}/assimp-5.2.5
# 	  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/assimp-5.2.5)
#   execute_process(COMMAND ${CMAKE_COMMAND}
# 	--build ${PROJECT_BINARY_DIR}/assimp-5.2.5 ${BUILD_COMMAND_OPTS})

# message(STATUS "Building Bullet")
# set(BULLET_DIR "${PROJECT_BINARY_DIR}/3.25.tar.gz" CACHE PATH "")
# # if(NOT EXISTS ${BULLET_DIR})
# message(STATUS "Downloading Bullet")
# Set(FETCHCONTENT_QUIET FALSE)
# file(DOWNLOAD
# 	https://github.com/bulletphysics/bullet3/archive/refs/tags/3.25.tar.gz
# 	${PROJECT_BINARY_DIR}/3.25.tar.gz
# 	SHOW_PROGRESS
# 	STATUS DOWNLOAD_RESULT)
# 	list(GET DOWNLOAD_RESULT 0 DOWNLOAD_RESULT_CODE)
# 	if(NOT DOWNLOAD_RESULT_CODE EQUAL 0)
# 		message(FATAL_ERROR "Failed downloading! Error: ${DOWNLOAD_RESULT}.")
# 	endif()

# message("Extracting file ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/bullet3.tar.gz...")	
# execute_process(COMMAND ${CMAKE_COMMAND}
# 	-E tar xf 3.25.tar.gz WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
# # endif()

# execute_process(COMMAND ${BUILD_COMMAND_COMMON}
# 	-DBUILD_SHARED_LIBS=OFF
# 	-DINSTALL_LIBS=ON
# 	-DCMAKE_POSITION_INDEPENDENT_CODE=ON
# 	-DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON
# 	-DBUILD_PYBULLET=OFF
# 	-DUSE_DOUBLE_PRECISION=OFF
# 	-DBUILD_CPU_DEMOS=OFF
# 	-DBUILD_BULLET2_DEMOS=OFF
# 	-DBUILD_EXTRAS=OFF
# 	-DBUILD_EGL=OFF
# 	-DBUILD_ENET=OFF
# 	-DBUILD_UNIT_TESTS=OFF
# 	-DCMAKE_RELWITHDEBINFO_POSTFIX= # fixes FindBullet on MSVC
# 	-DBUILD_CLSOCKET=OFF
# 	${PROJECT_BINARY_DIR}/bullet3-3.25
# 	WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/bullet3-3.25)
# execute_process(COMMAND ${CMAKE_COMMAND}
# --build ${PROJECT_BINARY_DIR}/bullet3-3.25 ${BUILD_COMMAND_OPTS})
# set(BULLET_ROOT ${OGREDEPS_PATH})

endif()

 
# Include necessary submodules
set(CMAKE_MODULE_PATH
"${PROJECT_SOURCE_DIR}/Dependencies/Ogre/CMake/Utils"
"${PROJECT_SOURCE_DIR}/Dependencies/Ogre/CMake/Packages"
)

find_package(freetype)
find_package(ZLIB)
find_package(ZZip)

message(STATUS "Building imgui")
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui-1.90.4-docking" CACHE PATH "")
if(NOT EXISTS ${IMGUI_DIR})
message(STATUS "Downloading imgui")
file(DOWNLOAD
	https://github.com/ocornut/imgui/archive/v1.90.4-docking.tar.gz
	"${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui.tar.gz")
execute_process(COMMAND ${CMAKE_COMMAND}
	-E tar xf imgui.tar.gz WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/")
endif()

include_directories( ${IMGUI_DIR} ${ZLIB_INCLUDE_DIRS} ${BZIP2_INCLUDE_DIR})
list(APPEND IMGUI_SOURCES
${IMGUI_DIR}/imgui.cpp
${IMGUI_DIR}/imgui_draw.cpp
${IMGUI_DIR}/imgui_tables.cpp
${IMGUI_DIR}/imgui_widgets.cpp
${IMGUI_DIR}/imgui_demo.cpp)
if(FREETYPE_FOUND)
message(STATUS "Found freetype lib")
message(STATUS "freetype include: ${FREETYPE_INCLUDE_DIRS}")
include_directories( ${FREETYPE_INCLUDE_DIRS})
set(IMGUI_ENABLE_FREETYPE TRUE)
list(APPEND IMGUI_SOURCES ${IMGUI_DIR}/misc/freetype/imgui_freetype.cpp)
else()
message(STATUS "NOT Found freetype lib")
endif()


set( MESHES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extra/meshes" )

if( APPLE )
	if( APPLE_IOS )
		file( GLOB_RECURSE RESOURCES ./src/*.storyboard )
	endif()
	set( RESOURCES ${RESOURCES}
		${MESHES_DIR} 
		${CMAKE_CURRENT_SOURCE_DIR}/bin/Data )

	if( APPLE_IOS )
		set_source_files_properties(
			${CMAKE_CURRENT_SOURCE_DIR}/src/OgreCommon/System/OSX/OSXUtils.mm
			PROPERTIES HEADER_FILE_ONLY TRUE
		)
	else()
		set_source_files_properties(
			${CMAKE_CURRENT_SOURCE_DIR}/src/OgreCommon/System/iOS/AppDelegate.mm
			${CMAKE_CURRENT_SOURCE_DIR}/src/OgreCommon/System/iOS/GameViewController.mm
			${CMAKE_CURRENT_SOURCE_DIR}/src/OgreCommon/System/iOS/iOSUtils.mm
			PROPERTIES HEADER_FILE_ONLY TRUE
		)
	endif()
endif()

if( APPLE )
	# Treat our *.c & *.cpp files as *.mm + Enable ARC on them
	set_source_files_properties( ${SOURCES} PROPERTIES
		COMPILE_FLAGS
			"-x objective-c++ -fobjc-arc"
	)
endif()

 
add_executable( ${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SOURCES} ${HEADERS} ${IMGUI_SOURCES} ${RESOURCES} )

target_link_libraries( ${PROJECT_NAME} ${OGRE_LIBRARIES}  ${FREETYPE_LIBRARIES} ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES})
 

if( APPLE )
	set_target_properties( ${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "NO" )
	set_target_properties( ${PROJECT_NAME} PROPERTIES RESOURCE "${RESOURCES}" )
	#set_target_properties( ${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_ICON_FILE SampleBrowser_OSX.icns)
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
		-framework CoreGraphics -framework QuartzCore -framework Metal  -framework StoreKit" )

	if( APPLE_IOS )
		# iPhone
		set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
			-framework UIKit -framework CoreText -framework CoreMotion" )

		# These must be global for all projects; or else they'll cause
		# trouble or increased compiling times
		set( CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "10.0" )
		set( CMAKE_XCODE_ATTRIBUTE_VALID_ARCHS "arm64" )

		set_target_properties( ${PROJECT_NAME} PROPERTIES
			MACOSX_BUNDLE_INFO_PLIST
				${CMAKE_CURRENT_SOURCE_DIR}/Scripts/Apple/iPhone/iOSApp.plist
			# XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS
			#	"${CMAKE_CURRENT_SOURCE_DIR}/Scripts/Apple/iPhone/iOSEntitlements.plist"
		)
	else()
		# macOS
		set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Foundation" )
		set_target_properties( ${PROJECT_NAME} PROPERTIES
			MACOSX_BUNDLE_INFO_PLIST
				${CMAKE_CURRENT_SOURCE_DIR}/Scripts/Apple/macOS/macOSApp.plist
			# XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS
			#	"${CMAKE_CURRENT_SOURCE_DIR}/Scripts/Apple/macOS/macOSEntitlements.plist"
		)
	endif()
endif()

install(
    TARGETS ${PROJECT_NAME}
      LIBRARY DESTINATION "${CMAKE_INSTALL_BINDIR}/${CMAKE_BUILD_TYPE}"
      ARCHIVE DESTINATION "${CMAKE_INSTALL_BINDIR}/${CMAKE_BUILD_TYPE}"
      RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/${CMAKE_BUILD_TYPE}"
      COMPONENT libraries)
	  
install( DIRECTORY 
	"${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}"
	"${EXECUTABLE_OUTPUT_PATH}/Data"
	"${OGRE_SOURCE}/Dependencies/bin/${CMAKE_BUILD_TYPE}"
	DESTINATION "${CMAKE_INSTALL_PREFIX}")

install( DIRECTORY 
	"${MESHES_DIR}"
	DESTINATION "${CMAKE_INSTALL_PREFIX}/Data")

install( FILES 
	"${CMAKE_CURRENT_SOURCE_DIR}/extra/resources2.cfg"
	DESTINATION "${CMAKE_INSTALL_PREFIX}/Data")	