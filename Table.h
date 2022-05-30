#ifndef TABLE_H
#define TABLE_H
#include "DataType.h"

class Table
{
private:
	DataType*** table;
	unsigned int rowCount;
	unsigned int colCount;
	static Table* tableInstance;
public:
	Table() : table(0), rowCount(0), colCount(0) {}

	Table(DataType*** table, unsigned int rowCount, unsigned int colCount);

	//Copy constructor, operator = 

	~Table();
};
#endif

