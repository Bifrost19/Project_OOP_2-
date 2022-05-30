#ifndef DATATYPE_H
#define DATATYPE_H
#include <cstring>
#include <iostream>

class DataType
{
private:

public:
	virtual int getInt() const;

	virtual double getDouble() const;

	virtual char* getString() const;

	virtual DataType* clone() const { return new (std::nothrow) DataType(*this); }

	virtual ~DataType() = default;
};
#endif

