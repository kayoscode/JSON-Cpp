#ifndef INCLUDE_JSON_FILE_H
#define INCLUDE_JSON_FILE_H

#include "JsonNode.h"

#include <cstdint>
#include <string>
#include <ostream>
#include <fstream>

class JsonFile {
    public:
        JsonFile(const std::string& fileName);

        ~JsonFile() {
        }

        JsonValue* getHead() {
            return values;
        }

        void save(const std::string& newFile, bool beautiful);

        bool isLoadSuccessful() {
            return loadSuccessful;
        }

    private:
        bool loadJson(char* fileContents, uint32_t size);
        JsonValue* values;
        bool loadSuccessful;
};

#endif