#include "Table.h"

void Table::allocateDataTypeTable(unsigned int rowCount, unsigned int colCount)
{
	this->table = new (std::nothrow) DataType * *[rowCount];
	if (!this->table) throw "Memory problem!";

	for (int i = 0; i < rowCount; i++)
	{
		this->table[i] = new (std::nothrow) DataType * [colCount];
		if (!this->table[i]) throw "Memory problem!";

		for (int j = 0; j < colCount; j++)
		{
			this->table[i][j] = table[i][j]->clone();
			if (!this->table[i][j]) throw "Memory problem!";
		}
	}
}

Table::Table(DataType*** table, unsigned int rowCount, unsigned int colCount)
{
	this->rowCount = rowCount;
	this->colCount = colCount;

	allocateDataTypeTable(this->rowCount, this->colCount);
}

Table::Table(const Table& other)
{
	this->rowCount = other.rowCount;
	this->colCount = other.colCount;

	allocateDataTypeTable(this->rowCount, this->colCount);
}

Table& Table::operator= (const Table& other)
{
	if (this != &other)
	{
		this->rowCount = other.rowCount;
		this->colCount = other.colCount;

		allocateDataTypeTable(this->rowCount, this->colCount);
	}
	return *this;
}

Table::~Table()
{
	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < colCount; j++)
		{
			delete this->table[i][j];
		}

		delete[] this->table[i];
	}

	delete[] this->table;
}


