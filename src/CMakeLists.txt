######################################################################################
# Build function for main library
######################################################################################

SET(SRCS lightstone_libhid.c)

BUILDSYS_BUILD_LIB(
  NAME lightstone
  SOURCES "${SRCS}" 
  CXX_FLAGS "-DUSE_LIBHID"
  LINK_LIBS ${LIBLIGHTSTONE_DEP_LIBS}
  LINK_FLAGS FALSE
  DEPENDS FALSE
  SHOULD_INSTALL TRUE
  VERSION "${LIBLIGHTSTONE_VERSION}"
  )
