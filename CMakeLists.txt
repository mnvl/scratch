
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(SCRATCH)

SET(SCRATCH_ROOT ${SCRATCH_SOURCE_DIR})
ADD_DEFINITIONS(-DSCRATCH_ROOT="${SCRATCH_ROOT}")

SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

SET(SCRATCH_CMAKE_DIR "${SCRATCH_ROOT}/etc/cmake")
SET(SCRATCH_LUA_DIR "${SCRATCH_ROOT}/lua")

LIST(APPEND CMAKE_MODULE_PATH "${SCRATCH_CMAKE_DIR}")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

ENABLE_TESTING()

SET(SCRATCH_LIBRARIES)

INCLUDE(CollectSourceFiles)
INCLUDE(AddConfigurationDependentTest)

IF(WIN32)
  SET(CMAKE_FIND_LIBRARY_SUFFIXES .a .dll.a .lib .dll)

  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
  ADD_DEFINITIONS(-D_WIN32_WINNT=0x0501)
  ADD_DEFINITIONS(-DD3D_DEBUG_INFO)
  ADD_DEFINITIONS(-DLUA_BUILD_AS_DLL)
  ADD_DEFINITIONS(-DLUABIND_DYNAMIC_LINK)
  LIST(APPEND SCRATCH_LIBRARIES ws2_32 mswsock winmm)

  IF(NOT ("$ENV{PREFIX}" STREQUAL ""))
  	SET(PREFIX $ENV{PREFIX})
    MESSAGE(STATUS "PREFIX = " ${PREFIX})
  ELSE()
    SET(PREFIX "c:/MinGW/msys/1.0/local")
  ENDIF()

  SET(CMAKE_PREFIX_PATH ${PREFIX})
  FILE(GLOB Boost_INCLUDE_DIR "${PREFIX}/include/boost-*")
ENDIF()

IF(UNIX)
  FIND_LIBRARY(SCRATCH_LIBM m)
  LIST(APPEND SCRATCH_LIBRARIES ${SCRATCH_LIBM})
ENDIF()

INCLUDE_DIRECTORIES(${SCRATCH_ROOT}/src)
INCLUDE_DIRECTORIES(${SCRATCH_ROOT}/build/src)

FIND_PACKAGE(DirectX)
INCLUDE_DIRECTORIES(${DirectX_INCLUDE_DIR})
LIST(APPEND SCRATCH_LIBRARIES ${DirectX_LIBRARIES})

FIND_PACKAGE(Log4cplus REQUIRED)
INCLUDE_DIRECTORIES(${LOG4CPLUS_INCLUDE_DIR})
LIST(APPEND SCRATCH_LIBRARIES ${LOG4CPLUS_LIBRARIES})

SET(SDL_BUILDING_LIBRARY ON)
FIND_PACKAGE(SDL REQUIRED)
INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
LIST(APPEND SCRATCH_LIBRARIES ${SDL_LIBRARY})

FIND_PACKAGE(SDL REQUIRED)
INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
LIST(APPEND SCRATCH_LIBRARIES ${SDL_LIBRARY})

SET(ENV{SDLDIR} ${SDL_INCLUDE_DIR})
FIND_PACKAGE(SDL_image REQUIRED)
INCLUDE_DIRECTORIES(${SDLIMAGE_INCLUDE_DIR})
LIST(APPEND SCRATCH_LIBRARIES ${SDLIMAGE_LIBRARY})

FIND_PACKAGE(Freetype REQUIRED)
INCLUDE_DIRECTORIES(${FREETYPE_INCLUDE_DIR})
LIST(APPEND SCRATCH_LIBRARIES ${FREETYPE_LIBRARY})

FIND_PACKAGE(SDL_ttf REQUIRED)
INCLUDE_DIRECTORIES(${SDLTTF_INCLUDE_DIR})
LIST(APPEND SCRATCH_LIBRARIES ${SDLTTF_LIBRARY})

ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
SET(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE(Boost 1.48 REQUIRED COMPONENTS thread system date_time regex filesystem unit_test_framework serialization)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LIST(APPEND SCRATCH_LIBRARIES ${Boost_LIBRARIES})

FIND_PACKAGE(XercesC3 REQUIRED)
INCLUDE_DIRECTORIES(${XERCESC3_INCLUDE_DIR})
LIST(APPEND SCRATCH_LIBRARIES ${XERCESC3_LIBRARIES})

FIND_PACKAGE(Lua51 REQUIRED)
INCLUDE_DIRECTORIES(${LUA_INCLUDE_DIR})
LIST(APPEND SCRATCH_LIBRARIES ${LUA_LIBRARIES})

FIND_PACKAGE(LuaBind REQUIRED)
INCLUDE_DIRECTORIES(${LUABIND_INCLUDE_DIR})
LIST(APPEND SCRATCH_LIBRARIES ${LUABIND_LIBRARIES})

LIST(REVERSE SCRATCH_LIBRARIES)
LIST(APPEND SCRATCH_LIBRARIES ${SDLTTF_LIBRARY})
LIST(APPEND SCRATCH_LIBRARIES ${FREETYPE_LIBRARY})
MESSAGE(STATUS "SCRATCH_LIBRARIES = " ${SCRATCH_LIBRARIES})

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(lua)
