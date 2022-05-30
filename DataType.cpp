#include "DataType.h"

int DataType::getInt() const
{
	throw "Unknown data type!";
	return 0;
}

double DataType::getDouble() const
{
	throw "Unknown data type!";
	return 0.0;
}

char* DataType::getString() const
{
	throw "Unknown data type!";
	return nullptr;
}
