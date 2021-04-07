#include <iostream>
#include <sstream>
#include "JsonNode.h"
#include "JsonParser.h"

int main() {
    char json[] = "[1, 2, 3, 4, 5]";
    JsonParser parser(json, sizeof(json)/sizeof(json[0]));

    JsonValue val;
    parser.parseJson(&val);

    std::stringstream output;
    val.createJsonString(output);

    std::cout << output.str();

    return 0;
}