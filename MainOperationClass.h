#ifndef _MAINOPERATIONCLASS
#define _MAINOPERATIONCLASS
#include <iostream>
#include <fstream>
#include "DataType.h"
#include "Int.h"
#include "Double.h"
#include "String.h"
#include "Table.h"

using namespace std;

class MainOperationClass //Singleton class
{
private:
	//Default constructor
	MainOperationClass() {}

	//Deallocation functions
	void deallocateStaticVars();

	//Allocation functions
	DataType*** allocateBufferTable(unsigned int rowCount, unsigned int colCount);

	//Functions to find the length of a number
	int findNumIntLength(int num);

	int findNumDoubleLength(double num);

	//Convert from string to num
	int convertFromStringToInt(const char* string);

	double convertFromStringToDouble(const char* string);

	//Convert from num to string
	char* convertFromIntToString(int num);

	char* convertFromDoubleToString(double num);

	//Remove functions
	char* removeQuotationMarks(const char* string);

	void removeWhiteSpaces(char initialCell[]);

	void removeEqualSign(char formula[]);

	//Check validity functions
	bool isCharNum(char ch);

	bool isDoubleNumValid(const char* stringNum);

	bool isIntNumValid(const char* stringNum);

	//Calculation of Formulas functions
	DataType* getCellValue(const char* cell);

	bool isCharSign(char ch);

	bool isThereSignInArray(const char* operations, unsigned int length, char sign, int& index);

	void removeOperationFromArray(char* operations, unsigned int length, unsigned int index);

	void refactorNumsArray(char nums[][15], unsigned length, unsigned int index, double result);

	double checkOperandsValidity(char num[], bool& isThereError);

	char* calculateFormula(const char* formula, bool& isThereError);

	void evaluateFormulas(int** formulaIndexArray, unsigned int formulaCount, DataType*** table);

	//Help functions
	bool isThereOnlySpaces(char array[]);

	char** separateCells(char array[], unsigned int colCount, unsigned int& cellCountInCells);

	void checkSizeOfTable(unsigned int row, unsigned int col);

	unsigned int getLengthOfCell(DataType* cell);

	void placeSpace(unsigned int quantity);

	void placeSpaceInFile(ofstream& fileW, unsigned int quantity);

	char* getFileName(char* filePath);

	char* separateFileNameFromCommand(char* string, const char* command, unsigned int& strLength);

	bool isThereCommandInString(char* string, const char* command);

	void verifyDataForEditFunc();

	//Write and read to file
	void readTableFromFile(fstream& fileR);

	void readFromFile(char* command, bool& isOpenSuccessful);

	void writeToFile(ofstream& fileW);

	//Main functionalities
	void editTable(unsigned int row, unsigned int col, char cellInfo[]);

	void printTable();

	void saveTable();

	void saveAsTable(char* command);

	void closeTable();

	void openTable(char* command);

	void help();

public:

	//Copy constructor & initialization operator
	MainOperationClass(const MainOperationClass& mOC) = delete;

	MainOperationClass& operator= (const MainOperationClass& mOC) = delete;

	//Move constructor & move initialization operator
	MainOperationClass(MainOperationClass&& mOC) = delete;

	MainOperationClass& operator= (MainOperationClass&& mOC) = delete;

	// Get instance function
	static MainOperationClass& getInstance()
	{
		static MainOperationClass object;
		return object;
	}

	//Execute command function
	void executeCommand(char command[]);
};
#endif

