#ifndef DOUBLE_H
#define DOUBLE_H
#include "DataType.h"

class Double : public DataType
{
private:
	double value;
public:
	Double() : value(0.0) {}

	Double(double _value) : value(_value) {}

	double getDouble() const override { return this->value; }

	Double* clone() const override { return new (std::nothrow) Double(*this); }

	void setValue(double _value) { this->value = _value; }
};
#endif

