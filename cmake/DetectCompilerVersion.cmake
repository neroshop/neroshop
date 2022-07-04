# Detect C, C++, Fortran compiler major, minor, and patch version

set(COMPILERS C CXX)

foreach(comp  ${COMPILERS})
  string(REGEX MATCH "([0-9]*)\\.([0-9]*)\\.([0-9]*)"
         major ${CMAKE_${comp}_COMPILER_VERSION})
  set(CMAKE_${comp}_COMPILER_MAJOR ${CMAKE_MATCH_1})
  set(CMAKE_${comp}_COMPILER_MINOR ${CMAKE_MATCH_2})
  set(CMAKE_${comp}_COMPILER_PATCH ${CMAKE_MATCH_3})
endforeach()
