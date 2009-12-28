######################################################################################
# Build function for liblightstone_test
######################################################################################

SET(SRCS liblightstone_test/liblightstone_test.c)

BUILDSYS_BUILD_EXE(
  NAME liblightstone_test
  SOURCES "${SRCS}" 
  CXX_FLAGS FALSE
  LINK_LIBS ${liblightstone_LIBRARY} ${LIBLIGHTSTONE_DEP_LIBS}
  LINK_FLAGS FALSE 
  DEPENDS ${liblightstone_LIBRARY}_DEPEND
  SHOULD_INSTALL TRUE
  )
