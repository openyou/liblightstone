######################################################################################
# Build function for main library
######################################################################################

IF(NOT WIN32)
  SET(SRCS lightstone_libusb1.c nputil_libusb1.c)

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
  SET(SRCS lightstone_win32.c)

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