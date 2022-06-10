#ifndef INT_H
#define INT_H
#include "DataType.h"

class Int : public DataType
{
private:
	int value;
public:
	Int() : value(0) {}

	Int(int _value) : value(_value) {}

	int getInt() const override { return this->value; };

	Int* clone() const override { return new (std::nothrow) Int(*this); }

	void setValue(int _value) { this->value = _value; }

};
#endif

