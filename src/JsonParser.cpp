#include "JsonParser.h"

JsonLexer::Token* loadJsonValue(JsonLexer& lexer, JsonLexer::Token* token, JsonValue* value);
JsonLexer::Token* loadJsonObject(JsonLexer& lexer, JsonLexer::Token* token, JsonObject* obj);
JsonLexer::Token* loadJsonArray(JsonLexer& lexer, JsonLexer::Token* token, JsonArray* obj);
JsonLexer::Token* loadObjectValue(JsonLexer& lexer, JsonLexer::Token* token, JsonValue* value, std::string& name);

JsonParser::JsonParser(char* file, uint32_t size) 
    :lexer(file, size)
{}

JsonLexer::Token* loadJsonValue(JsonLexer& lexer, JsonLexer::Token* token, JsonValue* newValue) {
    if(token->type == JsonLexer::TokenType::Float) {
        //we know it's a valid float, so we can use the lexeme
        char tmp = *token->end;
        size_t index;
        double value;

        *token->end = 0;
        value = std::stof(token->begin, &index);
        *token->end = tmp;
        
        newValue->updateValue(value);

        return lexer.getNextToken();
    }
    else if(token->type == JsonLexer::TokenType::Number) {
        //we already know it's a valid number, so let's just use the lexeme
        char tmp = *token->end;
        int value;

        *token->end = 0;
        value = std::stoi(token->begin);
        *token->end = tmp;

        newValue->updateValue(value);

        return lexer.getNextToken();
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

        return lexer.getNextToken();
    }
    else if(token->code == OBRC_CODE) {
        //load json object
        JsonObject* newObject = new JsonObject();
        loadJsonObject(lexer, token, newObject);
        newValue->updateValue(newObject);

        return lexer.getNextToken();
    }
    else if(token->code == OBRK_CODE) {
        //load array object
        JsonArray* newArray = new JsonArray();
        token = loadJsonArray(lexer, token, newArray);
        newValue->updateValue(newArray);
        return token;
    }
    else {
        std::cout << "Valid L Value expected\n";
    }

    return nullptr;
}

JsonLexer::Token* loadObjectValue(JsonLexer& lexer, JsonLexer::Token* token, JsonValue* value, std::string& name) {
    //we already know the incoming token is a string
    name = std::string(token->begin, token->end);
    token = lexer.getNextToken();

    if(token != nullptr) {
        if(token->code == COLON_CODE) {
            token = lexer.getNextToken();

            if(token == nullptr) {
                std::cout << "Invalid token\n";
            }

            //load the value, for now, just print the lexeme
            token = loadJsonValue(lexer, token, value);
        }
    }
    else {
        std::cout << "Invalid token\n";
    }

    return token;
}

JsonLexer::Token* loadJsonArray(JsonLexer& lexer, JsonLexer::Token* token, JsonArray* obj) {
    token = lexer.getNextToken();

    while(token != nullptr) {
        if(token->code == COMMA_CODE) {
            token = lexer.getNextToken();

            if(token == nullptr) {
                std::cout << "Invalid token\n";
            }
        }

        if(token->code == CBRK_CODE) {
            return lexer.getNextToken();
        }
        else {
            //load the value of the array and add to the index
            JsonValue* newValue = new JsonValue();
            token = loadJsonValue(lexer, token, newValue);
            obj->addElement(newValue);
        }
    }

    return token;
}

JsonLexer::Token* loadJsonObject(JsonLexer& lexer, JsonLexer::Token* token, JsonObject* obj) {
    token = lexer.getNextToken();

    while(token != nullptr) {
        if(token->code == COMMA_CODE) {
            token = lexer.getNextToken();

            if(token == nullptr) {
                std::cout << "Invalid token\n";
            }
        }

        if(token->code == STR_CODE) {
            std::string name;
            JsonValue* newValue = new JsonValue();
            token = loadObjectValue(lexer, token, newValue, name);
            obj->addValue(name, newValue);
        }
        else if(token->code == CBRC_CODE) {
            return lexer.getNextToken();
        }
        else {
            std::cout << "Comma or string expected!\n";
            return lexer.getNextToken();
        }
    }

    return token;
}

void JsonParser::parseJson(JsonValue* value) {
    JsonLexer::Token* token = lexer.getNextToken();

    if(token != nullptr) {
        loadJsonValue(lexer, token, value);
    }
    else {
        //TODO: throw error
        std::cout << "Syntax error\n";
    }
}