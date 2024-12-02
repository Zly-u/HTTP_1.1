set(SUB_PROJECT_NAME "Client_Core")

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCES "*.cpp" "*.c" "*.hpp" "*.h")
add_library(${SUB_PROJECT_NAME} STATIC ${SOURCES})

# Allows for the source files from "src" folder to be accessible in anything that includes this library.
target_include_directories(${SUB_PROJECT_NAME} PUBLIC includes)
target_include_directories(${SUB_PROJECT_NAME} PRIVATE src)

################################################################

# Lists own file to easily refer within the project.
set(
    G_CLIENT_CORE_INCLUDE_DIRS

    ${CMAKE_CURRENT_SOURCE_DIR}

    ${CMAKE_CURRENT_SOURCE_DIR}/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/Client

    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Client

    ${CMAKE_CURRENT_SOURCE_DIR}/libs

    CACHE INTERNAL ""
)
foreach(dir IN LISTS G_CLIENT_CORE_INCLUDE_DIRS)
    target_include_directories(${SUB_PROJECT_NAME} PUBLIC ${dir})
endforeach()

################################################################

target_link_libraries(${SUB_PROJECT_NAME} PUBLIC "SharedLib")

################################################################

set_target_properties(
    ${SUB_PROJECT_NAME} PROPERTIES
    PREFIX ""
    OUTPUT_NAME "${SUB_PROJECT_NAME}"
)

install(
    TARGETS ${SUB_PROJECT_NAME}
    RUNTIME DESTINATION ${G_INSTALL_DIR}
    LIBRARY DESTINATION ${G_INSTALL_DIR}
    ARCHIVE DESTINATION ${G_INSTALL_DIR}
)