#include "JsonLexer.h"
#include <stdint.h>
#include <map>
#include <iostream>

static std::map<char, int> reserves {
    { '{', OBRC_CODE },
    { '}', CBRC_CODE },
    { '[', OBRK_CODE },
    { ']', CBRK_CODE },
    { ':', COLON_CODE },
    { ',', COMMA_CODE },
};

static bool isEOF(uint32_t index, uint32_t size) {
    return index >= size;
}

static bool isWhiteSpace(char ch) {
    return ch == ' ' || ch == '\n' || ch == '\r' || ch == 9;
}

static void skipWhiteSpace(char* json, uint32_t& index, uint32_t size) {
    char ch = json[index];

    while(isWhiteSpace(ch) && !isEOF(index, size)) {
        index++;
        ch = json[index];
    }
}

static bool isAlpha(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static bool isNum(char ch) {
    return ch >= '0' && ch <= '9';
}

static bool isAlphaNum(char ch) {
    return isAlpha(ch) || isNum(ch);
}

static bool isStrStart(char ch) {
    return ch == '"';
}

static char getIntegerNum(char* json, uint32_t& index, uint32_t size) {
    char ch = json[index];

    while(isNum(ch) && !isEOF(index, size)) {
        index++;
        ch = json[index];
    }

    return ch;
}

static void loadNumber(char* json, uint32_t& index, uint32_t size, JsonLexer::Token* newToken) {
    char ch;
    bool flt = false;
    newToken->begin = json + index;
    index++;

    ch = getIntegerNum(json, index, size);
    if(ch == '.') {
        index++;
        flt = true;

        ch = getIntegerNum(json, index, size);

        if(ch == 'e' || ch == 'E') {
            index++;
            ch = json[index];

            if(ch == '-' || ch == '+') {
                index++;
                ch = json[index];
            }
            
            if(!isNum(ch)) {
                std::cout << "ERROR: Exponential notation must have at least one digit";
            }
            else {
                ch = getIntegerNum(json, index, size);
            }
        }
    }

    if(!flt) {
        newToken->type = JsonLexer::TokenType::Number;
        newToken->code = NUMBER_CODE;
    }
    else {
        newToken->type = JsonLexer::TokenType::Float;
        newToken->code = FLT_CODE;
    }

    newToken->end = json + index;
}

static void loadString(char* json, uint32_t& index, uint32_t size, JsonLexer::Token* newToken) {
    char ch;
    char endingQuote = json[index];
    index++;
    newToken->begin = json + index;
    ch = json[index];

    while(ch != endingQuote && !isEOF(index, size)) {
        if(ch == '\\') {
            index++;
        }
        if(ch != '\n') {
            index++;
            ch = json[index];
        }
        else {
            break;
        }
    }

    if(ch != '\n') {
        index++;
        ch = json[index];
        newToken->type = JsonLexer::TokenType::String;
        newToken->end = json + index - 1;
        newToken->code = STR_CODE;
    }
    else {
        std::cout << "Error: Lines cannot be on multiple lines!\n";
    }
}

static bool loadReserve(char* json, uint32_t& index, uint32_t size, JsonLexer::Token* newToken) {
    char ch = json[index];
    newToken->begin = json + index;
    index++;
    
    std::map<char, int>::iterator res = reserves.find(ch);

    if(res != reserves.end()) {
        newToken->code = res->second;
        newToken->type = JsonLexer::TokenType::Reserve;
        newToken->end = json + index;
        return true;
    }

    return false;
}

bool JsonLexer::getNextToken(JsonLexer::Token& ret) {
    skipWhiteSpace(json, index, size);
    char ch = json[index];

    if(isNum(ch) || ch == '-') {
        //load number
        loadNumber(json, index, size, &ret);
    }
    else if(isStrStart(ch)) {
        //load string
        loadString(json, index, size, &ret);
    }
    else {
        //if it's a reserve, add it, otherwise there is an error :D
        if(!loadReserve(json, index, size, &ret)) {
            ret.begin = nullptr;
            ret.end = nullptr;
            return false;
        }
    }

    if(ret.begin == nullptr || ret.end == nullptr) {
        return false;
    }

    return true;
}