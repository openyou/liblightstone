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
OPTION_LIBRARY_BUILD_SHARED(ON)

#Project specific options
OPTION(BUILD_EXAMPLES "Build liblightstone examples" ON)

######################################################################################
# Project specific package finding
######################################################################################

IF(NOT WIN32)
  FIND_PACKAGE(libusb-1.0 REQUIRED)
  INCLUDE_DIRECTORIES(${LIBUSB_1_INCLUDE_DIRS})
  SET(LIBLIGHTSTONE_DEP_LIBS ${LIBUSB_1_LIBRARIES})
ELSE()
  SET(WDK_PATH CACHE PATH "Path to WDK Installation")
  INCLUDE_DIRECTORIES(${WDK_PATH}/inc/api ${WDK_PATH}/inc/crt ${WDK_PATH}/inc)
  LINK_DIRECTORIES(${WDK_PATH}/lib/wxp/i386)
  SET(LIBLIGHTSTONE_DEP_LIBS "hid.lib" "setupapi.lib")
ENDIF()

######################################################################################
# Project specific globals
######################################################################################

#library definitions

SET(LIBLIGHTSTONE_MAJOR_VERSION 1)
SET(LIBLIGHTSTONE_MINOR_VERSION 5)
SET(LIBLIGHTSTONE_BUILD_VERSION 0)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

SET(LIBLIGHTSTONE_VERSION ${LIBLIGHTSTONE_MAJOR_VERSION}.${LIBLIGHTSTONE_MINOR_VERSION}.${LIBLIGHTSTONE_BUILD_VERSION})

#library name definitions

CREATE_LIBRARY_LINK_NAME(lightstone)

INCLUDE_DIRECTORIES(${LIBLIGHTSTONE_INCLUDE_DIR})
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

#If we build libusb staticly on apple, we need the proper frameworks
IF(BUILD_STATIC AND APPLE)
  LIST(APPEND LIBLIGHTSTONE_DEP_LIBS "-framework IOKit" "-framework CoreFoundation")
ENDIF(BUILD_STATIC AND APPLE)

ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(examples)
