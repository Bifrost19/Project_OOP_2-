#ifndef TABLE_H
#define TABLE_H
#include <fstream>
#include "DataType.h"
#include "Int.h"
#include "Double.h"
#include "String.h"

class Table
{
private:
	DataType*** table;
	unsigned int rowCount;
	unsigned int colCount;

	void allocateDataTypeTable(DataType*** table, unsigned int rowCount, unsigned int colCount);

	void deallocate();

public:
	static Table* tableInstance;
	static bool isTableOpened;
	static char* currentFileName;
	static bool isThereUnsavedChanges;

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

	DataType*** getTable() { return this->table; }

	//Methods
	void resizeTable(unsigned int rowCount, unsigned int colCount);

	void printValue(DataType* dataType);

	void printValueInFile(DataType* dataType, std::ofstream& fileW);
};
#endif

