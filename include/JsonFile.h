#ifndef INCLUDE_JSON_FILE_H
#define INCLUDE_JSON_FILE_H

#include <string>
#include <ostream>
#include <fstream>
#include "JsonNode.h"

class JsonFile {
    public:
        JsonFile(const std::string& fileName);

        ~JsonFile() {
        }

        JsonValue* getHead() {
            return values;
        }

        void save(const std::string& newFile, bool beautiful);

    private:
        void loadJson(char* fileContents, uint32_t size);
        JsonValue* values;
};

#endif