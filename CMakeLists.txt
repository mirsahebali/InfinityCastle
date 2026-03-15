cmake_minimum_required(VERSION 3.24...3.30)
project(InfinityCastle)

include(FetchContent)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 5.5)

FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
  carena
  GIT_REPOSITORY https://github.com/mirsahebali/carena.h.git
  GIT_TAG        origin/main
  GIT_SHALLOW    ON
)

FetchContent_MakeAvailable(carena)

# Our Project
add_executable(${PROJECT_NAME})
add_subdirectory(src)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>)

if ("${PLATFORM}" STREQUAL "Web")
    add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/../resources
    )
    #DEPENDS ${PROJECT_NAME}
else()
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    )
    #DEPENDS ${PROJECT_NAME}
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${carena_SOURCE_DIR})

if(RUN_TESTS)
  target_compile_definitions(${PROJECT_NAME} PRIVATE RUN_TESTS)
endif()

#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib m)

find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
    target_link_libraries(${PROJECT_NAME} ${MATH_LIBRARY})
endif()


# Web Configurations
if ("${PLATFORM}" STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    target_link_options(${PROJECT_NAME} PUBLIC -sUSE_GLFW=3 PUBLIC --preload-file resources)
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  set(
    CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -g -Wextra -Wall  -fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer"
  )
  target_link_options(${PROJECT_NAME}
    BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address
  )
endif()
