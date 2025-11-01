#ifndef INCLUDE_JSON_PARSER_H
#define INCLUDE_JSON_PARSER_H

#include "JsonLexer.h"
#include "JsonNode.h"

#include <cstdint>

class JsonParser {
    public:
        JsonParser(char* json, uint32_t size);

        bool parseJson(JsonValue* object);

        ~JsonParser() {

        }

    private:
        JsonLexer lexer;
};

#endif