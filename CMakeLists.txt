cmake_minimum_required(VERSION 3.22 VERSION 0.1.0)

project(OpenglSfml)

set(HEADER_FILES src/pch/opengl_sfml_window_pch.hpp)


set(SOURCE_FILES 
    src/main.cpp 
    ${HEADER_FILES}
    )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(STANDARD_CXX_VERSION_FLAG "-std=c++11")
set(OPTIMIZE_FLAG "-O3")
set(WARNING_FLAGS "-Werror -Wpedantic -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter")

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY shader DESTINATION ${CMAKE_BINARY_DIR})
file(COPY lint_codebase.sh DESTINATION ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STANDARD_CXX_VERSION_FLAG} ${OPTIMIZE_FLAG} ${WARNING_FLAGS}")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGLEW -lGLU -lGL)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${HEADER_FILES})
