######################################################################################
# Build function for main library
######################################################################################

INCLUDE_DIRECTORIES(../libnputil/include/)
SET(SRCS lightstone.c ../libnputil/src/nputil_libusb1.c)

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
