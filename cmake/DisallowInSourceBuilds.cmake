# Disallow in-source builds

if(__disallow_in_source_builds)
	return()
endif()
set(disallow_in_source_builds YES)

function(disallow_in_source_builds)

  # disallow in-source builds
  if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    message(FATAL_ERROR "ERROR: In-source builds are not allowed!")
  endif()

endfunction()

disallow_in_source_builds()
