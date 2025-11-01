#include "JsonFile.h"
#include "JsonParser.h"

//load the entire json file into memory at once
static char* loadFile(const std::string& name, uint32_t& size) {
    char* ret;
    std::ifstream inputFile(name, std::ios::in | std::ios::binary | std::ios::ate);

    if(!inputFile.is_open()) {
        return nullptr;
    }

    size = (uint32_t)inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    ret = new char[size + 1];
    inputFile.read(ret, size);
    ret[size] = 0;

    inputFile.close();

    return ret;
}

//load the json file and convert to json format
JsonFile::JsonFile(const std::string& name) :
    values(nullptr)
{
    uint32_t size = 0;
    char* fileContents = loadFile(name, size);

    if(fileContents == nullptr) {
        loadSuccessful = false;
    }
    else {
        loadSuccessful = true;
        loadSuccessful = loadJson(fileContents, size);
        delete[] fileContents;
    }
}

//save the json as a file
void JsonFile::save(const std::string& name, bool beautiful) {
    //write contents to outputStream
    std::ofstream outputFile(name, std::ios::out);

    //if the values are null, there was an error loading the file
    if(values != nullptr) {
        values->createJsonString(outputFile, beautiful);
    }

    outputFile.close();
}

bool JsonFile::loadJson(char* fileContents, uint32_t size) {
    values = new JsonValue();
    JsonParser parser(fileContents, size);
    return !parser.parseJson(values);
}