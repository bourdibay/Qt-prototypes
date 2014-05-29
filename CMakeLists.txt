cmake_minimum_required(VERSION 2.8)

# Eliminate a warning when building in Windows that relates
# to static linking of Qt executables to qtmain.lib.
# This policy was introduced in CMake version 2.8.11.
# CMake version 2.8.11.2 warns when the policy is not set
# and uses OLD behavior.
if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()

Project (QtProto)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMakeModules/ )

# need to find Qt5 stuff...
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{QTDIR}/lib/cmake )
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{QTDIR}/lib/cmake )

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")


set(BUILD_DIRECTORY "${CMAKE_SOURCE_DIR}/Builds")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_definitions(-std=c++11 -stdlib=libc++)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	add_definitions(-std=gnu++11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
endif()


IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
macro(ADD_FRAMEWORK fwname appname)
    find_library(FRAMEWORK_${fwname}
    NAMES ${fwname}
    PATHS ${CMAKE_OSX_SYSROOT}/System/Library /Library/Frameworks ~/Library/Frameworks /System/Library
    PATH_SUFFIXES Frameworks
    NO_DEFAULT_PATH)
    if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
        MESSAGE(ERROR ": Framework ${fwname} not found")
    else()
        TARGET_LINK_LIBRARIES(${appname} "${FRAMEWORK_${fwname}}/${fwname}")
	include_directories(${FRAMEWORK_${fwname}})
        MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
    endif()
endmacro(ADD_FRAMEWORK)
ENDIF ()

set(EXECUTABLE_OUTPUT_PATH "${BUILD_DIRECTORY}/bin")
add_subdirectory( ListProto )
add_subdirectory( TreeProto )
add_subdirectory( TextEditor )
#add_subdirectory( Scintilla )
add_subdirectory( SideAreaManagement )
#add_subdirectory( SideAreaManagement_2 )
add_subdirectory( ModelSharing )
