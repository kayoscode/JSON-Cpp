cmake_minimum_required(VERSION 3.18)

SET(PROJ_NAME JsonParser)
project(${PROJ_NAME})

file(GLOB SRC 
    "src/*.cpp"
    "include/*.h"
)

add_executable(${PROJ_NAME}
    "${SRC}"
)

target_include_directories("${PROJ_NAME}" PRIVATE ${PROJECT_SOURCE_DIR}/./include)