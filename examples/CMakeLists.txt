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
  DEPENDS lightstone_DEPEND
  SHOULD_INSTALL TRUE
  )

INCLUDE_DIRECTORIES("/home/qdot/usr/include/oscpack")
LINK_DIRECTORIES("/home/qdot/usr/lib")

SET(SRCS lightstone_osc/lightstone_osc.cpp)
SET(LIBS ${liblightstone_LIBRARY} ${LIBLIGHTSTONE_DEP_LIBS} "OSC")

BUILDSYS_BUILD_EXE(
  NAME lightstone_osc
  SOURCES "${SRCS}" 
  CXX_FLAGS FALSE
  LINK_LIBS "${LIBS}"
  LINK_FLAGS FALSE 
  DEPENDS lightstone_DEPEND
  SHOULD_INSTALL TRUE
  )
