#include <iostream>
#include <sstream>
#include "JsonNode.h"
#include "JsonParser.h"

int main() {
    char json[] = "[[1, 2, 3, 4, 5], 12.123, {\"tag\":[123, [134,2,3]]}, {}]";
    JsonParser parser(json, sizeof(json)/sizeof(json[0]));

    JsonValue val;
    parser.parseJson(&val);

    std::stringstream output;
    val.createJsonString(output);

    std::cout << output.str() << "\n";

    std::cout << val.arrayValue->get(2)->objectValue->lookupNode("tag")->arrayValue->get(1)->arrayValue->get(0)->numberValue << "\n";

    return 0;
}