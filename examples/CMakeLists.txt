######################################################################################
# Build function for liblightstone_test
######################################################################################

SET(SRCS lightstone_test/lightstone_test.c)

BUILDSYS_BUILD_EXE(
  NAME lightstone_test
  SOURCES "${SRCS}" 
  CXX_FLAGS "-DUSE_LIBHID"
  LINK_LIBS ${liblightstone_LIBRARY} ${LIBLIGHTSTONE_DEP_LIBS}
  LINK_FLAGS FALSE 
  DEPENDS ${liblightstone_LIBRARY}_DEPEND
  SHOULD_INSTALL TRUE
  )
