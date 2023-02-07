#ifndef VALUE_H
#define VALUE_H

enum ValueType {ZERO, ONE, UNKNOWN};
class Value
{
public:
    Value();
    Value(ValueType);
    Value(bool);
    Value& operator=(ValueType);
    Value& operator=(bool);
    Value& operator=(int);
    Value& operator!();
    Value& operator&=(Value);
    Value& operator|=(Value);

    bool operator==(ValueType);
    bool operator==(Value);
    bool operator==(bool);
    bool operator==(int);
    bool operator!=(ValueType);
    bool operator!=(Value);
    bool operator!=(bool);
    bool operator!=(int);

    operator bool();

    ValueType type();    // Value to ValueType
private:
    bool v[2];
};

#endif // VALUE_H
