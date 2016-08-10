#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <sstream>

using std::string;

enum CalcyType {
    CALCY_BOOL, CALCY_ID, CALCY_INT,
    CALCY_DOUBLE, CALCY_CHAR, CALCY_STRING,
    CALCY_NOTHING
};

class Type {
public:
    virtual string toString() = 0;
    CalcyType type;
};

class TypeBool : public Type {
public:
    explicit TypeBool(bool b) : value(b) { type = CALCY_BOOL; }
    string toString() {return value ? "#t" : "#f";}
    bool value;
};

class TypeInt : public Type {
public:
    explicit TypeInt(int v) : value(v) { type = CALCY_INT; }
    string toString() {return std::to_string(value);}
    int value;
};

class TypeId : public Type {
public:
    explicit TypeId(string i) : value(i) { type = CALCY_ID; }
    string toString() {return value;}
    string value;
};

class TypeDouble : public Type {
public:
    explicit TypeDouble(double d) : value(d) { type = CALCY_DOUBLE; }
    string toString() {return std::to_string(value);}
    double value;
};

class TypeChar : public Type {
public:
    explicit TypeChar(char c) : value(c) { type = CALCY_CHAR; }
    string toString() {
        std::stringstream ss;
        string s;
        ss << value;
        ss >> s;
        return s;
    }
    char value;
};

class TypeString : public Type {
public:
    explicit TypeString(string s) : value(s) { type = CALCY_STRING; }
    string toString() {return value;}
    string value;
};

class TypeNothing : public Type {
public:
    explicit TypeNothing() {type = CALCY_NOTHING; }
    string toString() {return "Nothing";}
};

#endif//TYPE_H
