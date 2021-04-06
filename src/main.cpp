#include <iostream>
#include <sstream>
#include "JsonNode.h"
#include "JsonParser.h"

int main() {
    char json[] = "{\"elements\":[21.2,\"String1!\",20,\"true\",\"false\",\"null\"], \"e\":{\"h\":\"String!\"}}";
    JsonParser parser(json, sizeof(json)/sizeof(json[0]));

    JsonObject obj;
    parser.parseJson(&obj);

    std::stringstream output;
    obj.createJsonString(output);

    std::cout << output.str();

    return 0;
}