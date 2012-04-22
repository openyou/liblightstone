######################################################################################
# CMake directives
######################################################################################

#Require 2.6 or higher.
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

INCLUDE( ${CMAKE_SOURCE_DIR}/BuildSysCMakeLib.cmake )

#setting link directory policy

IF(COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

######################################################################################
# Project declaration and options
######################################################################################

#Project declaration

PROJECT(LIBLIGHTSTONE)
INITIALIZE_BUILD()

#Common Options

OPTION_LIBRARY_BUILD_STATIC(ON)
OPTION_LIBRARY_BUILD_SHARED(OFF)

#Project specific options
OPTION(BUILD_EXAMPLES "Build liblightstone examples" ON)

######################################################################################
# Project specific package finding
######################################################################################

FIND_PACKAGE(HIDAPI)
FIND_PACKAGE(argtable)

######################################################################################
# Project specific globals
######################################################################################

#library definitions

SET(LIBLIGHTSTONE_MAJOR_VERSION 2)
SET(LIBLIGHTSTONE_MINOR_VERSION 0)
SET(LIBLIGHTSTONE_BUILD_VERSION 0)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${HIDAPI_INCLUDE_DIR})

SET(LIBLIGHTSTONE_VERSION ${LIBLIGHTSTONE_MAJOR_VERSION}.${LIBLIGHTSTONE_MINOR_VERSION}.${LIBLIGHTSTONE_BUILD_VERSION})

#library name definitions

CREATE_LIBRARY_LINK_NAME(lightstone)

INCLUDE_DIRECTORIES(${LIBLIGHTSTONE_INCLUDE_DIR})
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

LIST(APPEND LIBLIGHTSTONE_DEP_LIBS ${HIDAPI_LIBRARIES})

# #If we build libusb staticly on apple, we need the proper frameworks
IF(BUILD_STATIC AND APPLE)
  LIST(APPEND LIBLIGHTSTONE_DEP_LIBS "-framework IOKit" "-framework CoreFoundation")
ENDIF(BUILD_STATIC AND APPLE)

IF(UNIX AND NOT APPLE)
	SET(CMAKE_INSTALL_RPATH "/usr/lib:/home/qdot/usr/lib:${CMAKE_BINARY_DIR}/lib")
	SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
	LIST(APPEND LIBLIGHTSTONE_DEP_LIBS "udev")
ENDIF()

ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(examples)
