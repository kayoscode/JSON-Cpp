#include "JsonParser.h"
#include "JsonLexer.h"

static void loadJsonValue(JsonLexer& lexer, JsonLexer::Token* token, JsonValue* value, bool& error);
static void loadJsonObject(JsonLexer& lexer, JsonLexer::Token* token, JsonObject* obj, bool& error);
static void loadJsonArray(JsonLexer& lexer, JsonLexer::Token* token, JsonArray* obj, bool& error);
static void loadObjectValue(JsonLexer& lexer, JsonLexer::Token* token, JsonValue* value, std::string& name, bool& error);

JsonParser::JsonParser(char* file, uint32_t size) 
    :lexer(file, size)
{}

static void loadJsonValue(JsonLexer& lexer, JsonLexer::Token* token, JsonValue* newValue, bool& error) {
    if(token->type == JsonLexer::TokenType::Float) {
        //we know it's a valid float, so we can use the lexeme
        char tmp = *token->end;
        size_t index;
        double value;

        *token->end = 0;
        value = std::stof(token->begin, &index);
        *token->end = tmp;
        
        newValue->updateValue(value);

        lexer.getNextToken(*token);
        return;
    }
    else if(token->type == JsonLexer::TokenType::Number) {
        //we already know it's a valid number, so let's just use the lexeme
        char tmp = *token->end;
        int value;

        *token->end = 0;
        value = std::stoi(token->begin);
        *token->end = tmp;

        newValue->updateValue(value);

        lexer.getNextToken(*token);
        return;
    }
    else if(token->type == JsonLexer::TokenType::String) {
        std::string value(token->begin, token->end);

        if(value == "true") {
            newValue->updateValue(true, false);
        }
        else if(value == "false") {
            newValue->updateValue(false, false);
        }
        else if(value == "null") {
            newValue->updateValue(false, true);
        }
        else {
            newValue->updateValue(value);
        }

        lexer.getNextToken(*token);
        return;
    }
    else if(token->code == OBRC_CODE) {
        //load json object
        JsonObject* newObject = new JsonObject();
        loadJsonObject(lexer, token, newObject, error);
        newValue->updateValue(newObject);

        lexer.getNextToken(*token);
        return;
    }
    else if(token->code == OBRK_CODE) {
        //load array object
        JsonArray* newArray = new JsonArray();
        loadJsonArray(lexer, token, newArray, error);
        newValue->updateValue(newArray);
        return;
    }
    else {
        std::cout << "Valid L Value expected\n";
        error = true;
    }

    token->begin = nullptr;
    token->end = nullptr;
}

static void loadObjectValue(JsonLexer& lexer, JsonLexer::Token* token, JsonValue* value, std::string& name, bool& error) {
    //we already know the incoming token is a string
    name = std::string(token->begin, token->end);
    lexer.getNextToken(*token);

    if(token->begin != nullptr && token->end != nullptr) {
        if(token->code == COLON_CODE) {
            lexer.getNextToken(*token);

            if(token->begin == nullptr && token->end == nullptr) {
                std::cout << "Invalid token\n";
                error = true;
            }

            //load the value, for now, just print the lexeme
            loadJsonValue(lexer, token, value, error);
        }
    }
    else {
        std::cout << "Invalid token\n";
        error = true;
    }
}

static void loadJsonArray(JsonLexer& lexer, JsonLexer::Token* token, JsonArray* obj, bool& error) {
    lexer.getNextToken(*token);

    while(token->begin != nullptr && token->end != nullptr) {
        if(token->code == COMMA_CODE) {
            lexer.getNextToken(*token);

            if(token->begin == nullptr && token->end == nullptr) {
                std::cout << "Invalid token\n";
                error = true;
            }
        }

        if(token->code == CBRC_CODE || token->code == CBRK_CODE) {
            lexer.getNextToken(*token);
            return;
        }
        else {
            //load the value of the array and add to the index
            JsonValue* newValue = new JsonValue();
            loadJsonValue(lexer, token, newValue, error);
            obj->addElement(newValue);
        }
    }
}

static void loadJsonObject(JsonLexer& lexer, JsonLexer::Token* token, JsonObject* obj, bool& error) {
    lexer.getNextToken(*token);

    while(token->begin != nullptr && token->end != nullptr) {
        if(token->code == COMMA_CODE) {
            lexer.getNextToken(*token);

            if(token->begin == nullptr && token->end == nullptr) {
                std::cout << "Invalid token\n";
                error = true;
            }
        }

        if(token->code == STR_CODE) {
            std::string name;
            JsonValue* newValue = new JsonValue();
            loadObjectValue(lexer, token, newValue, name, error);

            //avoid memory leak
            if(!obj->addValue(name, newValue)) {
                delete newValue;
            }
        }
        else if(token->code == CBRC_CODE || token->code == CBRK_CODE) {
            lexer.getNextToken(*token);
            return;
        }
        else {
            std::cout << "Comma or string expected!\n";
            lexer.getNextToken(*token);
            error = true;
        }
    }
}

bool JsonParser::parseJson(JsonValue* value) {
    JsonLexer::Token token;
    bool hasAnother = lexer.getNextToken(token);

    bool parseError = false;

    if(hasAnother) {
        loadJsonValue(lexer, &token, value, parseError);
    }
    else {
        //TODO: throw error
        std::cout << "Syntax error\n";
        parseError = true;
    }

    return parseError;
}
