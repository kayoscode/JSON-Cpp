#ifndef INCLUDE_JSON_LEXER_H
#define INCLUDE_JSON_LEXER_H

#include <string>
#include <iostream>

#define NUMBER_CODE 50
#define FLT_CODE 51
#define STR_CODE 52
#define OBRC_CODE 0
#define CBRC_CODE 1
#define OBRK_CODE 2
#define CBRK_CODE 3
#define COLON_CODE 4
#define COMMA_CODE 5

class JsonLexer {
    public:
        JsonLexer(char* jsonContents, uint32_t size) 
            :json(jsonContents),
            size(size),
            index(0)
        {
        }

        ~JsonLexer() {

        }

        enum class TokenType {
            Reserve,
            Number,
            Float,
            String,
            Undefined
        };

        struct Token {
            char* begin = NULL, *end = NULL;
            TokenType type;
            int code;

            void print() {
                char tmp = *end;
                *end = 0;
                std::cout << begin << "\n";
                *end = tmp;
            }
        };

        bool getNextToken(JsonLexer::Token& nextToken);

    private:
        char* json;
        uint32_t index;
        uint32_t size;
};

#endif