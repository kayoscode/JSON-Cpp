#ifndef INCLUDE_JSON_NODE_H
#define INCLUDE_JSON_NODE_H

#include <string>
#include <map>
#include <vector>
#include <ostream>

struct JsonValue;

/**
 * The various types for a JsonValue
 * */
enum class JsonValueType {
    Object,
    Array,
    String,
    Number,
    Float,
    Boolean,
    Null,
    None
};

class JsonObject {
    public:
        JsonObject() {

        }

        ~JsonObject();

        /**
         * Return whether the node exists or not
         * @param name name of the node
         * */
        inline bool nodeExists(const std::string& name) {
            return keys.find(name) != keys.end();
        }

        /**
         * Operator if you want to use it
         * */
        inline JsonValue* operator[](const std::string& name) {
            return lookupNode(name);
        }

        /**
         * Searches for the key,
         * returns null if not found
         * @param name name of the node
         * */
        inline JsonValue* lookupNode(const std::string& name) {
            std::map<std::string, JsonValue*>::iterator lookup = keys.find(name);

            if(lookup != keys.end()) {
                return lookup->second;
            }

            return nullptr;
        }

        /**
         * Adds a value to the object
         * @param name the name of the variable
         * @param value the value to add {takes ownership of the parameter}
         * @return true if successful, false otherwise
         * */
        inline bool addValue(const std::string& name, JsonValue* value) {
            if(keys.find(name) != keys.end()) {
                return false;
            }

            keys[name] = value;
            return true;
        }

        /**
         * Removes a key and value from the object
         * */
        inline bool removeValue(const std::string& name) {
            if(keys.find(name) != keys.end()) {
                return false;
            }

            keys.erase(name);
            return true;
        }

        /**
         * @return the number of key value pairs
         * */
        int size() {
            return (int)keys.size();
        }

        const std::map<std::string, JsonValue*>& getKeys() {
            return keys;
        }

        /**
         * Creates a json file in a string format
         * */
        void createJsonString(std::ostream& output, bool beautiful = false, int depth = 0);

    private:
        std::map<std::string, JsonValue*> keys;
};

class JsonArray {
    public:
        /**
         * Constructor
         * */
        JsonArray() {

        }

        /**
         * Destructor
         * */
        ~JsonArray();
        
        /**
         * return the length of the array
         * */
        inline int size() {
            return (int)values.size();
        }

        JsonValue* operator[](int index) {
            return get(index);
        }

        /**
         * return the value at an index
         * */
        JsonValue* get(int index) {
            return values[index];
        }

        /**
         * Adds a new value to the array
         * @param value new value to add {function takes ownership}
         * @return always returns true
         * */
        bool addElement(JsonValue* value) {
            values.push_back(value);
            return true;
        }

        /**
         * Removes an element from the array
         * @return true if a value was removed.
         * */
        inline bool removeElement(int index) {
            if(index >= values.size() && index >= 0) {
                return false;
            }

            values.erase(values.begin() + index);
            return true;
        }

        /**
         * Creates a file in a string format
         * */
        void createJsonString(std::ostream& output, bool beautiful = false, int depth = 0);

    private:
        std::vector<JsonValue*> values;
};

struct JsonValue {
    JsonValue();

    /**
     * copy constructor
     * */
    JsonValue(const JsonValue& value);

    /**
     * Initialize with JsonObject
     * Takes ownership of @param object
     * */
    JsonValue(JsonObject* object);

    /**
     * Initialize with json array
     * Takes ownership of @param object
     * */
    JsonValue(JsonArray* arr);

    /**
     * Initialize with string
     * */
    JsonValue(const std::string& str);

    /**
     * Initialize with number
     * */
    JsonValue(int number);

    /**
     * Initialize with float
     * */
    JsonValue(double number);

    /**
     * if @param isNull is set to false, set the boolean value to @param booleanValue
     * if @param isNull is set to true, @param booleanValue doesn't mean anything and instead the node is null
     * */
    JsonValue(bool booleanValue, bool isNull);
    ~JsonValue();

    /**
     * Creates a json file in a string format
     * */
    void createJsonString(std::ostream& output, bool beautiful = false, int depth = 0);

    /**
     * Equals operator
     * */
    JsonValue& operator=(const JsonValue& value);

    /**
     * update the values and potentially change the type
     * */
    void updateValue(JsonObject* object);
    void updateValue(JsonArray* arr);
    void updateValue(const std::string& str);
    void updateValue(int integer);
    void updateValue(double flt);
    void updateValue(bool booleanValue, bool isNull);

    /**
     * Stores each value type as separate members
     * */
    JsonObject* objectValue;
    JsonArray* arrayValue;
    std::string stringValue;
    int numberValue;
    double floatValue;
    bool booleanValue;
    bool isNull;

    JsonValueType type;
};

#endif
