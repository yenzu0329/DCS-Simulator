#include "value.h"

Value::Value()              {*this = UNKNOWN;}
Value::Value(ValueType vt)  {*this = vt;}
Value::Value(bool b)        {*this = b;}

Value& Value::operator=(ValueType vt)
{
    switch (vt)
    {
    case ZERO:      v[0] = v[1] = 0; break;
    case ONE:       v[0] = 1; v[1] = 0; break;
    case UNKNOWN:   v[0] = 0; v[1] = 1; break;
    }
    return *this;
}
Value& Value::operator=(bool vt)    {return operator=((ValueType)vt);}
Value& Value::operator=(int vt)     {return operator=((ValueType)vt);}

Value& Value::operator!()
{
    if(type() == ZERO)      *this = ONE;
    else if(type() == ONE)  *this = ZERO;
    else                    *this = UNKNOWN;
    return *this;
}

Value& Value::operator&=(Value value)
{
    if(value.type() == ZERO || type() == ZERO)
        *this = ZERO;
    else if (value.type() == ONE && type() == ONE)
        *this = ONE;
    else
        *this = UNKNOWN;
    return *this;
}

Value& Value::operator|=(Value value)
{
    if(value.type() == ZERO && type() == ZERO)
        *this = ZERO;
    else if (value.type() == ONE || type() == ONE)
        *this = ONE;
    else
        *this = UNKNOWN;
    return *this;
}

bool Value::operator==(Value value) {return value == type();}
bool Value::operator==(int value)   {return value == type();}
bool Value::operator==(ValueType vt)
{
    if(vt == UNKNOWN || type() == UNKNOWN)
        return false;
    return vt == type();
}
bool Value::operator==(bool b)
{
    if(b==1)    return type() == ONE;
    else        return type() == ZERO;
}

bool Value::operator!=(ValueType vt)
{
    if(vt == UNKNOWN || type() == UNKNOWN)
        return false;
    return vt != type();
}
bool Value::operator!=(Value value) {return value != type();}
bool Value::operator!=(int value)   {return value != type();}
bool Value::operator!=(bool b)      {return !operator==(b);}

ValueType Value::type()
{
    if(v[1] == 0)
        if(v[0] == 0)   return ZERO;
        else            return ONE;
    else                return UNKNOWN;
}

Value::operator bool()
{
    if(type() == ZERO)  return false;
    else                return true;
}
