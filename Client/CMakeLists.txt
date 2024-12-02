set(CMAKE_CXX_STANDARD 23)

################################################################

if(WIN32)

endif()

################################################################

add_subdirectory(APP)
add_subdirectory(CORE)

################################################################

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_compile_options("-O3")
endif()