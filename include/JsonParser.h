#ifndef INCLUDE_JSON_PARSER_H
#define INCLUDE_JSON_PARSER_H

#include "JsonLexer.h"
#include "JsonNode.h"

class JsonParser {
    public:
        JsonParser(char* json, uint32_t size);

        void parseJson(JsonObject* object);

        ~JsonParser() {

        }

    private:
        JsonLexer lexer;
};

#endif