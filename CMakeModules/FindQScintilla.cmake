# - Try to find the QScintilla2 includes and library
# which defines
#
# QSCINTILLA_FOUND - system has QScintilla2
# QSCINTILLA_DEBUG_FOUND - system has QScintilla2 built in debug mode
# QSCINTILLA_INCLUDE_DIR - where to find qextscintilla.h
# QSCINTILLA_LIBRARIES - the libraries to link against to use QScintilla
# QSCINTILLA_LIBRARIES_DEBUG - the libraries to link against to use QScintilla in debug mode
# QSCINTILLA_LIBRARY - where to find the QScintilla library (not for general use)
# QSCINTILLA_LIBRARY_DEBUG - where to find the QScintilla library built in debug mode (not for general use)

# copyright (c) 2007 Thomas Moenicke thomas.moenicke@kdemail.net
# modified on 2013 by Benjamin Bourdin benjamin.bourdin40@gmail.com
#
# Redistribution and use is allowed according to the terms of the FreeBSD license.

SET(QSCINTILLA_FOUND FALSE)
SET(QSCINTILLA_DEBUG_FOUND FALSE)

FIND_PATH(QSCINTILLA_INCLUDE_DIR qsciglobal.h
"$ENV{QTDIR}/include/Qsci" /usr/include /usr/include/Qsci /usr/include/qt5/Qsci
)

SET(QSCINTILLA_NAMES ${QSCINTILLA_NAMES} qscintilla2 libqscintilla2)
FIND_LIBRARY(QSCINTILLA_LIBRARY
	NAMES ${QSCINTILLA_NAMES}
	PATHS "$ENV{QTDIR}/lib/" /usr/lib/x86_64-linux-gnu/
)
SET(QSCINTILLA_NAMES_DEBUG ${QSCINTILLA_NAMES_DEBUG} qscintilla2_d libqscintilla2_d)
FIND_LIBRARY(QSCINTILLA_LIBRARY_DEBUG
	NAMES ${QSCINTILLA_NAMES_DEBUG}
	PATHS "$ENV{QTDIR}/lib/" /usr/lib/x86_64-linux-gnu/
)

# release librairy
IF (QSCINTILLA_LIBRARY AND QSCINTILLA_INCLUDE_DIR)
	SET(QSCINTILLA_LIBRARIES ${QSCINTILLA_LIBRARY})
	SET(QSCINTILLA_FOUND TRUE)
	IF (CYGWIN)
		IF(BUILD_SHARED_LIBS)
		# No need to define QSCINTILLA_USE_DLL here, because it's default for Cygwin.
		ELSE(BUILD_SHARED_LIBS)
		SET (QSCINTILLA_DEFINITIONS -DQSCINTILLA_STATIC)
		ENDIF(BUILD_SHARED_LIBS)
	ENDIF (CYGWIN)
ENDIF (QSCINTILLA_LIBRARY AND QSCINTILLA_INCLUDE_DIR)
# debug librairy
IF (QSCINTILLA_LIBRARY_DEBUG AND QSCINTILLA_INCLUDE_DIR)
	SET(QSCINTILLA_LIBRARIES_DEBUG ${QSCINTILLA_LIBRARY_DEBUG})
	SET(QSCINTILLA_DEBUG_FOUND TRUE)
	IF (CYGWIN)
		IF(BUILD_SHARED_LIBS)
		# No need to define QSCINTILLA_USE_DLL here, because it's default for Cygwin.
		ELSE(BUILD_SHARED_LIBS)
		SET (QSCINTILLA_DEFINITIONS -DQSCINTILLA_STATIC)
		ENDIF(BUILD_SHARED_LIBS)
	ENDIF (CYGWIN)
ENDIF (QSCINTILLA_LIBRARY_DEBUG AND QSCINTILLA_INCLUDE_DIR)

IF (QSCINTILLA_FOUND)
  IF (NOT QScintilla_FIND_QUIETLY)
    MESSAGE(STATUS "Found QScintilla2: ${QSCINTILLA_LIBRARY}")
  ENDIF (NOT QScintilla_FIND_QUIETLY)
ELSE (QSCINTILLA_FOUND)
  IF (QScintilla_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find QScintilla library")
  ENDIF (QScintilla_FIND_REQUIRED)
ENDIF (QSCINTILLA_FOUND)

MARK_AS_ADVANCED(QSCINTILLA_INCLUDE_DIR QSCINTILLA_LIBRARY QSCINTILLA_LIBRARY_DEBUG)