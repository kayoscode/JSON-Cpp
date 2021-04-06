#include "JsonNode.h"
#include <iostream>

JsonValue::~JsonValue() {
    delete objectValue;
    delete arrayValue;
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

JsonValue::JsonValue(const std::wstring& str) 
    :stringValue(str)
{
    this->type = JsonValueType::String;
}

JsonValue::JsonValue(const std::string& str)
    :stringValue(str.begin(), str.end())
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

void JsonObject::createJsonString(std::ostream& output) {
    output << '{';

    for(std::map<std::string, JsonValue*>::iterator i = keys.begin(); i != keys.end(); ++i) {
        output << '"';
        output << i->first;
        output << '"';

        output << ':';
        i->second->createJsonString(output);

        if(std::next(i) != keys.end()) {
            output << ',';
        }
    }

    output << '}';
}

void JsonArray::createJsonString(std::ostream& output) {
    output << '[';

    for(int i = 0; i < values.size(); ++i) {
        values[i]->createJsonString(output);

        if(i < values.size() - 1) {
            output << ',';
        }
    }

    output << ']';
}

void JsonValue::createJsonString(std::ostream& output) {
    switch(type) {
        case JsonValueType::None:
            break;
        case JsonValueType::String:
            char c;
            output << '"';
            for(int i = 0; i < stringValue.size(); ++i) {
                output << (char)stringValue[i];
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
            objectValue->createJsonString(output);
            break;
        case JsonValueType::Array:
            arrayValue->createJsonString(output);
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

void JsonValue::updateValue(const std::wstring& str) {
    type = JsonValueType::String;
    stringValue = str;
}

void JsonValue::updateValue(const std::string& str) {
    type = JsonValueType::String;
    stringValue = std::wstring(str.begin(), str.end());
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