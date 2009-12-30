######################################################################################
# Build function for liblightstone_test
######################################################################################

SET(SRCS lightstone_test/lightstone_test.c)
SET(LIBS ${liblightstone_LIBRARY} ${LIBLIGHTSTONE_DEP_LIBS})


BUILDSYS_BUILD_EXE(
  NAME lightstone_test
  SOURCES "${SRCS}" 
  CXX_FLAGS FALSE
  LINK_LIBS "${LIBS}"
  LINK_FLAGS FALSE 
  DEPENDS ${liblightstone_LIBRARY}_DEPEND
  SHOULD_INSTALL TRUE
  )
