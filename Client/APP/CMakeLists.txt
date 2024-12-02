set(SUB_PROJECT_NAME "Client_App")
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCES "*.cpp" "*.c" "*.hpp" "*.h")
add_executable(${SUB_PROJECT_NAME} ${SOURCES})

target_include_directories(${SUB_PROJECT_NAME} PUBLIC libs)
target_include_directories(${SUB_PROJECT_NAME} PRIVATE src)

################################################################

set(
    G_CLIENT_APP_INCLUDE_DIRS

    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src

    CACHE INTERNAL ""
)

foreach(dir IN LISTS G_CLIENT_APP_INCLUDE_DIRS)
    target_include_directories(${SUB_PROJECT_NAME} PUBLIC ${dir})
endforeach()

################################################################

target_link_libraries(${SUB_PROJECT_NAME} PUBLIC "Client_Core")

################################################################

install(
    TARGETS ${SUB_PROJECT_NAME}
    RUNTIME DESTINATION ${G_INSTALL_DIR}
    LIBRARY DESTINATION ${G_INSTALL_DIR}
    ARCHIVE DESTINATION ${G_INSTALL_DIR}
)

# install resources
#install(
#    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/APP/res
#    DESTINATION ${G_INSTALL_DIR}
#)