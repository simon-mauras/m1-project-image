PROJECT(Helloworld)
#Required in DGtal

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_BUILD_TYPE Release)
ADD_DEFINITIONS("-std=c++11")

### We need Boost Program Options library
SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
SET(Boost_FOUND FALSE)
FIND_PACKAGE(Boost 1.46.0 REQUIRED COMPONENTS program_options)
if (Boost_FOUND)
  message(STATUS "Boost and boost_program_options found.")
  include_directories(${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

FIND_PACKAGE(DGtal REQUIRED)
INCLUDE_DIRECTORIES(${DGTAL_INCLUDE_DIRS})
LINK_DIRECTORIES(${DGTAL_LIBRARY_DIRS})

SET(SRC_DIR src/)

SET(SRCs
  getRank
  naiveDistance
  imgRotate
  imgAddNoise
  imgScale
  shape_distance
  shape_indexing
  test)


FOREACH(FILE ${SRCs})
  add_executable(${FILE} ${SRC_DIR}${FILE})
  target_link_libraries (${FILE} ${DGTAL_LIBRARIES} ${Boost_LIBRAIRIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})
ENDFOREACH(FILE)
