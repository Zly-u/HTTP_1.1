cmake_minimum_required(VERSION 3.29)
set(CMAKE_CXX_STANDARD 23)

set(
    G_MASTER_PROJECT_NAME "HTTP Shenanigans"
    CACHE INTERNAL ""
)

project(
    ${G_MASTER_PROJECT_NAME}
    VERSION 1.0
    DESCRIPTION "HTTP Server practice"
    LANGUAGES C CXX
)

################################################################

set(
    G_INSTALL_DIR
    ${CMAKE_SOURCE_DIR}/__BUILD__/
    CACHE INTERNAL ""
)

################################################################

if(WIN32)

endif()

################################################################

# Include projects for compilation
add_subdirectory(Shared)
add_subdirectory(Client)
add_subdirectory(Server)

################################################################

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_compile_options("-O3")
endif()