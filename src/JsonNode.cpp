#include "JsonNode.h"
#include <iostream>

JsonValue::~JsonValue() {
    delete objectValue;
    delete arrayValue;
}

JsonObject::~JsonObject() {
    for(std::map<std::string, JsonValue*>::iterator i = keys.begin(); i != keys.end(); ++i) {
	    delete i->second;
    }
}

JsonArray::~JsonArray() {
    for(int i = 0; i < values.size(); ++i) {
        delete values[i];
    }
}

JsonValue::JsonValue(JsonObject* object) 
    :objectValue(object)
{
    type = JsonValueType::Object;
}

JsonValue::JsonValue(JsonArray* arr) 
    :arrayValue(arr)
{
    this->type = JsonValueType::Array;
}

JsonValue::JsonValue(const std::string& str)
    :stringValue(str)
{
    type = JsonValueType::String;
}

JsonValue::JsonValue(int number) 
    :numberValue(number)
{
    this->type = JsonValueType::Number;
}

JsonValue::JsonValue(double number) 
    :floatValue(number)
{
    this->type = JsonValueType::Float;
}

JsonValue::JsonValue(bool booleanValue, bool isNull) {
    std::cout << booleanValue << " " << isNull << "\n";
    updateValue(booleanValue, isNull);
}

JsonValue::JsonValue() 
    :objectValue(nullptr),
    arrayValue(nullptr),
    stringValue(),
    numberValue(0),
    floatValue(0),
    booleanValue(false),
    isNull(false)
{
    type = JsonValueType::None;
}

JsonValue::JsonValue(const JsonValue& value) 
    :objectValue(value.objectValue),
    arrayValue(value.arrayValue),
    stringValue(value.stringValue),
    numberValue(value.numberValue),
    floatValue(value.floatValue),
    booleanValue(value.booleanValue),
    isNull(value.isNull),
    type(value.type)
{}

JsonValue& JsonValue::operator=(const JsonValue& value) {
    this->objectValue = value.objectValue;
    this->arrayValue = value.arrayValue;
    this->stringValue = value.stringValue;
    this->numberValue = value.numberValue;
    this->floatValue = value.floatValue;
    this->booleanValue = value.booleanValue;
    this->isNull = value.isNull;
    this->type = value.type;

    return *this;
}

void JsonObject::createJsonString(std::ostream& output, bool beautiful, int depth) {
    output << '{';

    if(beautiful) {
        output << '\n';

        for(int i = 0; i < depth; i++) {
            output << "  ";
        }
    }

    for(std::map<std::string, JsonValue*>::iterator i = keys.begin(); i != keys.end(); ++i) {
        output << '"';
        output << i->first;
        output << '"';

        output << ':';

        if(beautiful) {
            output << ' ';
        }

        i->second->createJsonString(output, beautiful, depth);

        if(std::next(i) != keys.end()) {
            output << ',';
        }
        else {
            depth--;
        }

        if(beautiful) {
            output << '\n';

            for(int i = 0; i < depth; i++) {
                output << "  ";
            }
        }
    }

    output << '}';
}

void JsonArray::createJsonString(std::ostream& output, bool beautiful, int depth) {
    output << '[';

    if(beautiful) {
        output << '\n';

        for(int i = 0; i < depth; i++) {
            output << "  ";
        }
    }

    for(int i = 0; i < values.size(); ++i) {
        values[i]->createJsonString(output, beautiful, depth);

        if(i < values.size() - 1) {
            output << ',';
        }
        else {
            depth--;
        }

        if(beautiful) {
            output << '\n';

            for(int i = 0; i < depth; i++) {
                output << "  ";
            }
        }
    }

    output << ']';
}

void JsonValue::createJsonString(std::ostream& output, bool beautiful, int depth) {
    switch(type) {
        case JsonValueType::None:
            break;
        case JsonValueType::String:
            output << '"';
            for(int i = 0; i < stringValue.size(); ++i) {
                if(stringValue[i] == '\\') {
                    output << "\\\\";
                }
                else if(stringValue[i] == '\b') {
                    output << "\\b";
                }
                else if(stringValue[i] == '\n') {
                    output << "\\n";
                }
                else if(stringValue[i] == '\"') {
                    output << "\\\"";
                }
                else if(stringValue[i] == '\t') {
                    output << "\\t";
                }
                else if(stringValue[i] == '\r') {
                    output << "\\r";
                }
                else {
                    output << (char)stringValue[i];
                }
            }
            output << '"';
            break;
        case JsonValueType::Number:
            output << numberValue;
            break;
        case JsonValueType::Float:
            output << floatValue;
            break;
        case JsonValueType::Object:
            objectValue->createJsonString(output, beautiful, depth + 1);
            break;
        case JsonValueType::Array:
            arrayValue->createJsonString(output, beautiful, depth + 1);
            break;
        case JsonValueType::Boolean:
            output << (booleanValue? "\"true\"" : "\"false\"");
            break;
        case JsonValueType::Null:
            output << "\"null\"";
            break;
    }
}

void JsonValue::updateValue(JsonObject* object) {
    type = JsonValueType::Object;
    objectValue = object;
}

void JsonValue::updateValue(JsonArray* arr) {
    type = JsonValueType::Array;
    arrayValue = arr;
}

void JsonValue::updateValue(const std::string& str) {
    type = JsonValueType::String;

    for(int i = 0; i < str.size(); ++i) {
        if(str[i] == '\\') {
            i++;

            if(i < str.size()) {
                if(str[i] == '\\') {
                    stringValue += '\\';
                }
                else if(str[i] == 'b') {
                    stringValue += '\b';
                }
                else if(str[i] == 'n') {
                    stringValue += '\n';
                }
                else if(str[i] == '"') {
                    stringValue += '\"';
                }
                else if(str[i] == 't') {
                    stringValue += '\t';
                }
                else if(str[i] == 'r') {
                    stringValue += '\r';
                }
            }
            else {
                stringValue += '\\';
            }
        }
        else {
            stringValue += str[i];
        }
    }
}

void JsonValue::updateValue(int integer) {
    type = JsonValueType::Number;
    numberValue = integer;
}

void JsonValue::updateValue(double flt) {
    type = JsonValueType::Float;
    floatValue = flt;
}

void JsonValue::updateValue(bool booleanValue, bool isNull = false) {
    if(isNull) {
        this->isNull = true;
        this->type = JsonValueType::Null;
    }
    else {
        this->booleanValue = booleanValue;
        this->type = JsonValueType::Boolean;
    }
}
