# Welcome to Json Cpp
### A simple JSON file parser for C++

## Usage in a project:

Option 1 (CMake):
1. Navigate to your project directory
2. git clone https://github.com/youngbrycecode/JSON-Cpp.git
3. Modify CMakeLists.txt: 
```
add_subdirectory(JSON-Cpp) 
target_link_libraries(${PROJ_NAME}
    JsonParser
)
target_include_directories("${PROJ_NAME}" PRIVATE ${PROJECT_SOURCE_DIR}/JSON-Cpp/include)
```
