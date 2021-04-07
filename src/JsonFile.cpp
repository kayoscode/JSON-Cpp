#include "JsonFile.h"
#include "JsonParser.h"

#include <iostream>

char* loadFile(const std::string& name, uint32_t& size) {
    char* ret;
    std::ifstream inputFile(name, std::ios::in | std::ios::binary | std::ios::ate);
    size = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    ret = new char[size + 1];
    inputFile.read(ret, size);
    ret[size] = 0;

    inputFile.close();

    return ret;
}

JsonFile::JsonFile(const std::string& name) :
    values(nullptr)
{
    uint32_t size = 0;
    char* fileContents = loadFile(name, size);
    loadJson(fileContents, size);
    delete[] fileContents;
}

void JsonFile::save(const std::string& name, bool beautiful) {
    //write contents to outputStream
    std::ofstream outputFile(name, std::ios::out);

    //if the values are null, there was an error loading the file
    if(values != nullptr) {
        values->createJsonString(outputFile, beautiful);
    }

    outputFile.close();
}

void JsonFile::loadJson(char* fileContents, uint32_t size) {
    values = new JsonValue();
    JsonParser parser(fileContents, size);
    parser.parseJson(values);
}