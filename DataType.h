#ifndef DATATYPE_H
#define DATATYPE_H
#include <cstring>
#include <iostream>
#include <fstream>

class DataType
{
private:

public:
	//Getters of derived classes
	virtual int getInt() const;

	virtual double getDouble() const;

	virtual char* getString() const;

	virtual char* getFormula() const;

	virtual DataType* clone() const { return new (std::nothrow) DataType(*this); }

	virtual ~DataType() = default;
};
#endif

