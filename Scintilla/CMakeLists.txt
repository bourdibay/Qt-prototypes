cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0043)
    cmake_policy(SET CMP0043 NEW)
endif()

project (ScintillaEditor)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMakeModules/ )
# need to find Qt5 stuff...
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{QTDIR}/lib/cmake )
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{QTDIR}/lib/cmake )
# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)
find_package(QScintilla REQUIRED)
# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})
# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})
# QSCINTILLA dependencies
include_directories(${QSCINTILLA_INCLUDE_DIR})
include_directories(${QSCINTILLA_INCLUDE_DIR}/../)


# only way to add headers in generated solution
file(GLOB headers "src/*/*.h")
file(GLOB sources "*.cpp" "src/*.cpp" "src/*/*.cpp")
INCLUDE_DIRECTORIES(
    ./src/
    ./src/TextEditor/
)
SET(HEADER_MOC
    ./src/test/testWindow.h
    ./src/test/TestScintilla.h
)
ADD_EXECUTABLE(
    ScintillaEditor
    ${sources}
    ${headers}
)

SET(LIB_DEBUG "")
IF (${QSCINTILLA_DEBUG_FOUND})
    SET(LIB_DEBUG ${LIB_DEBUG} ${QSCINTILLA_LIBRARIES_DEBUG})
ENDIF(${QSCINTILLA_DEBUG_FOUND})

IF ("${LIB_DEBUG}" STREQUAL "")
    TARGET_LINK_LIBRARIES(ScintillaEditor ${QSCINTILLA_LIBRARIES})
ELSE ("${LIB_DEBUG}" STREQUAL "")
    TARGET_LINK_LIBRARIES(ScintillaEditor optimized "${QSCINTILLA_LIBRARIES}" debug "${QSCINTILLA_LIBRARIES_DEBUG}")
ENDIF ("${LIB_DEBUG}" STREQUAL "")

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(BUILD_DIRECTORY "${CMAKE_SOURCE_DIR}/Builds")
set(EXECUTABLE_OUTPUT_PATH "${BUILD_DIRECTORY}/bin")

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

# Use the Widgets module from Qt 5.
qt5_use_modules(ScintillaEditor Widgets)
