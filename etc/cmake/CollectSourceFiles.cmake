# usage: COLLECT_SOURCE_FILES(SOURCES_VAR TEST_SOURCES_VAR)
#
# Collects all source files in current directory and divides them to two lists:
# with test_ prefix and without. Useful for storing tests an sources in same
# directory, e.g. you may have class Foo in foo.{h.cc} and test_foo.cc.
#
FUNCTION(COLLECT_SOURCE_FILES SOURCES_VAR TEST_SOURCES_VAR)
  FILE(GLOB_RECURSE SOURCES *.h *.cc)
  FILE(GLOB_RECURSE TEST_SOURCES test_*.h test_*.cc)

  LIST(REMOVE_ITEM SOURCES ${TEST_SOURCES} "")

  SET(${SOURCES_VAR} ${SOURCES} PARENT_SCOPE)
  SET(${TEST_SOURCES_VAR} ${TEST_SOURCES} PARENT_SCOPE)
ENDFUNCTION()
