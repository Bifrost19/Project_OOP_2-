#ifndef TABLE_H
#define TABLE_H
#include "DataType.h"

class Table
{
private:
	DataType*** table;
	unsigned int rowCount;
	unsigned int colCount;

	void allocateDataTypeTable(unsigned int rowCount, unsigned int colCount);

public:
	static Table* tableInstance;

	//Constructors
	Table() : table(0), rowCount(0), colCount(0) {}

	Table(DataType*** table, unsigned int rowCount, unsigned int colCount);

	Table(const Table& other);

	//Operators
	Table& operator= (const Table& other);

	//Destructor
	~Table();

	//Getters
	unsigned int getRowCount() const { return this->rowCount; }

	unsigned int getColCount() const { return this->colCount; }

	DataType*** getTable() const { return this->table; }
};
#endif

