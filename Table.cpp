#include "Table.h"

void Table::allocateDataTypeTable(DataType*** table, unsigned int rowCount, unsigned int colCount)
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

void Table::resizeTable(unsigned int rowCount, unsigned int colCount)
{
	DataType*** buffer = new (std::nothrow) DataType * *[this->rowCount];
	if (!buffer) throw "Memory problem!";

	for (int i = 0; i < this->rowCount; i++)
	{
		buffer[i] = new (std::nothrow) DataType * [this->colCount];
		if (!buffer[i]) throw "Memory problem!";

		for (int j = 0; j < this->colCount; j++)
		{
			buffer[i][j] = this->table[i][j]->clone();
			if (!buffer[i][j]) throw "Memory problem!";
		}
	}

	deallocate();

	this->table = new (std::nothrow) DataType * *[rowCount];
	if (!this->table) throw "Memory problem!";

	for (int i = 0; i < rowCount; i++)
	{
		this->table[i] = new (std::nothrow) DataType * [colCount];
		if (!this->table[i]) throw "Memory problem!";

		for (int j = 0; j < colCount; j++)
		{
			this->table[i][j] = new (std::nothrow) String("");
			if (!this->table[i][j]) throw "Memory problem!";
		}
	}

	for (int i = 0; i < this->rowCount; i++)
	{
		for (int j = 0; j < this->colCount; j++)
		{
			this->table[i][j] = buffer[i][j]->clone();
			if (!this->table[i][j]) throw "Memory problem!";
		}
	}

	this->rowCount = rowCount;
	this->colCount = colCount;

	delete[] buffer;
}

void Table::deallocate()
{
	for (int i = 0; i < this->rowCount; i++)
	{
		for (int j = 0; j < this->colCount; j++)
		{
			delete this->table[i][j];
		}

		delete[] this->table[i];
	}

	delete[] this->table;
}

Table::Table(DataType*** table, unsigned int rowCount, unsigned int colCount)
{
	this->rowCount = rowCount;
	this->colCount = colCount;

	allocateDataTypeTable(table, this->rowCount, this->colCount);
}

Table::Table(const Table& other)
{
	this->rowCount = other.rowCount;
	this->colCount = other.colCount;

	allocateDataTypeTable(other.table, this->rowCount, this->colCount);
}

Table& Table::operator= (const Table& other)
{
	if (this != &other)
	{
		this->rowCount = other.rowCount;
		this->colCount = other.colCount;

		allocateDataTypeTable(other.table, this->rowCount, this->colCount);
	}
	return *this;
}

Table::~Table()
{
	deallocate();
}

void Table::printValue(DataType* dataType)
{
	if (typeid(*dataType) == typeid(Int))
		std::cout << dataType->getInt();
	else if (typeid(*dataType) == typeid(Double))
		std::cout << dataType->getDouble();
	else if (typeid(*dataType) == typeid(String))
		std::cout << dataType->getString();
}

void Table::printValueInFile(DataType* dataType, std::ofstream& fileW)
{
	if (typeid(*dataType) == typeid(Int))
		fileW << dataType->getInt();
	else if (typeid(*dataType) == typeid(Double))
		fileW << dataType->getDouble();
	else if (typeid(*dataType) == typeid(String))
		if (strcmp(dataType->getString(), ""))
			fileW << "\"" << dataType->getString() << "\"";
}