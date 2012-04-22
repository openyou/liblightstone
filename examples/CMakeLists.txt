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

IF(Argtable_FOUND)
  INCLUDE_DIRECTORIES(${Argtable_INCLUDES})
  SET(SRCS lightstone_osc/lightstone_osc.cpp)
  SET(LIBS ${liblightstone_LIBRARY} ${LIBLIGHTSTONE_DEP_LIBS} ${Argtable_LIBRARY})

  BUILDSYS_BUILD_EXE(
    NAME lightstone_osc
    SOURCES "${SRCS}" 
    CXX_FLAGS FALSE
    LINK_LIBS "${LIBS}"
    LINK_FLAGS FALSE 
    DEPENDS lightstone_DEPEND
    SHOULD_INSTALL TRUE
    )
ELSE()
  MESSAGE("Not building lightstone_osc - Argtable")
ENDIF()
