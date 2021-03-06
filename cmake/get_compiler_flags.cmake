# Get compiler flags

macro(get_compiler_flags)

  # Convert CMAKE_BUILD_TYPE to upper case so even if the user gives it via the
  # command line, the tests below work as intended
  string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)

  # Forward cmake compiler flags to automake builds by setting our own variables
  # CFLAGS and CXXFLAGSS. Only these flags should then be used by the
  # individual external projects builds.
  if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CFLAGS "${CFLAGS} ${CMAKE_C_FLAGS}")
    set(CXXFLAGS "${CXXFLAGS} ${CMAKE_CXX_FLAGS}")
  endif()
  if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    set(CFLAGS "${CFLAGS} ${CMAKE_C_FLAGS_DEBUG}")
    set(CXXFLAGS "${CXXFLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
  endif()
  if(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    set(CFLAGS "${CFLAGS} ${CMAKE_C_FLAGS_RELEASE}")
    set(CXXFLAGS "${CXXFLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
  endif()
  if(CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
    set(CFLAGS "${CFLAGS} ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
    set(CXXFLAGS "${CXXFLAGS} ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
  endif()
  if(CMAKE_BUILD_TYPE STREQUAL "MINSIZEREL")
    set(CFLAGS "${CFLAGS} ${CMAKE_C_FLAGS_MINSIZEREL}")
    set(CXXFLAGS "${CXXFLAGS} ${CMAKE_CXX_FLAGS_MINSIZEREL}")
  endif()

  # Remove first empty character of compiler flag strings
  if (CFLAGS)
    STRING(REGEX REPLACE "^ " "" CFLAGS ${CFLAGS})
  endif()
  if (CXXFLAGS)
    STRING(REGEX REPLACE "^ " "" CXXFLAGS ${CXXFLAGS})
  endif()

  # Echo compiler flags string used
  if (CFLAGS)
    message(STATUS "C compiler flags: '${CFLAGS}'")
  endif()
  if (CXXFLAGS)
    message(STATUS "C++ compiler flags: '${CXXFLAGS}'")
  endif()

endmacro()

get_compiler_flags()
