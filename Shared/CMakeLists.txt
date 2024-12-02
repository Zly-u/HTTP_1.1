set(CMAKE_CXX_STANDARD 23)

################################################################

set(SUB_PROJECT_NAME "SharedLib")

file(GLOB_RECURSE SOURCES "*.cpp" "*.c" "*.hpp" "*.h")
add_library(${SUB_PROJECT_NAME} STATIC ${SOURCES})

# Allows for the source files from "includes" folder to be accessible in anything that includes this library.
target_include_directories(${SUB_PROJECT_NAME} PUBLIC libs)
target_include_directories(${SUB_PROJECT_NAME} PUBLIC includes)
target_include_directories(${SUB_PROJECT_NAME} PRIVATE src)

################################################################

# Lists own file to easily refer within the Project
# and by other Module/Project
set(
    G_PUBLIC_SHARED_INCLUDE_DIRS

    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/libs
    ${CMAKE_CURRENT_SOURCE_DIR}/includes

    CACHE INTERNAL ""
)

# Makes it so when we work inside of this Project
# we don't have to specify full paths to the includes
# that are inside of this project.
# But it will force anything that includes this Project
# to use Full paths for clarity.
set(
    G_PRIVATE_SHARED_INCLUDE_DIRS

    ${CMAKE_CURRENT_SOURCE_DIR}/libs/Shared
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/Shared/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/Shared/SDL/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/Shared/SDL_image/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/Shared/tinyfiledialogs

    ${CMAKE_CURRENT_SOURCE_DIR}/includes/Shared

    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Shared

    CACHE INTERNAL ""
)

foreach(dir IN LISTS G_PUBLIC_SHARED_INCLUDE_DIRS)
    target_include_directories(${SUB_PROJECT_NAME} PUBLIC ${dir})
endforeach()

foreach(dir IN LISTS G_PRIVATE_SHARED_INCLUDE_DIRS)
    target_include_directories(${SUB_PROJECT_NAME} PRIVATE ${dir})
endforeach()

################################################################

#target_compile_features(${SUB_PROJECT_NAME} PUBLIC cxx_std_20)

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

################################################################

#libs
target_link_directories(
    ${SUB_PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/Shared/SDL/lib/x64
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/Shared/SDL_image/lib/x64
)

target_link_libraries(
    ${SUB_PROJECT_NAME}
    LINK_PUBLIC SDL2.lib
    LINK_PUBLIC SDL2main.lib
    LINK_PUBLIC SDL2_image.lib
)

set(
    SDL2_DLLS
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/Shared/SDL/lib/x64/SDL2.dll
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/Shared/SDL_image/lib/x64/SDL2_image.dll
)

install(
    FILES ${SDL2_DLLS}
    DESTINATION ${G_INSTALL_DIR}
)