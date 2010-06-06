######################################################################################
# Build function for main library
######################################################################################

IF(NOT WIN32)
  INCLUDE_DIRECTORIES(../libnputil/include/ .)
  SET(SRCS lightstone_libusb1.c ../libnputil/src/nputil_libusb1.c)

  BUILDSYS_BUILD_LIB(
    NAME lightstone
    SOURCES "${SRCS}" 
    CXX_FLAGS FALSE
    LINK_LIBS ${LIBLIGHTSTONE_DEP_LIBS}
    LINK_FLAGS FALSE
    DEPENDS FALSE
    SHOULD_INSTALL TRUE
    VERSION "${LIBLIGHTSTONE_VERSION}"
    )
ELSE()
  INCLUDE_DIRECTORIES(../libnputil/include/ .)
  SET(SRCS lightstone_win32.c ../libnputil/src/nputil_win32hid.c)

  BUILDSYS_BUILD_LIB(
    NAME lightstone
    SOURCES "${SRCS}" 
    CXX_FLAGS "-DUSE_WIN32"
    LINK_LIBS ${LIBLIGHTSTONE_DEP_LIBS}
    LINK_FLAGS FALSE
    DEPENDS FALSE
    SHOULD_INSTALL TRUE
    VERSION "${LIBLIGHTSTONE_VERSION}"
    )
ENDIF()