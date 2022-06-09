#include <iostream>
#include <fstream>
#include "DataType.h"
#include "Int.h"
#include "Double.h"
#include "String.h"
#include "Table.h"

using namespace std;

Table* Table::tableInstance = nullptr;

//Allocation functions

DataType*** allocateBufferTable(unsigned int rowCount, unsigned int colCount)
{
	DataType*** table = new (nothrow) DataType** [rowCount];
	if (!table) throw "Memory problem!";

	for (int i = 0; i < rowCount; i++)
	{
		table[i] = new (std::nothrow) DataType* [colCount];
		if (!table[i]) throw "Memory problem!";
	}

	return table;
}

//Functions to find the length of a number
int findNumIntLength(int num)
{
	int length = 0;

	if (num < 0)
	{
		length++;
		num = abs(num);
	}
	else if (num == 0) length++;

	while (num > 0)
	{
		num /= 10;
		length++;
	}

	return length;
}

int findNumDoubleLength(double num)
{
	int length = 0;
	bool isPassedTroughLoop = false;

	if (num < 0)
	{
		num = abs(num);
		length++;
	}

	double numHelper = (int)num;

	for (int i = 1; i <= 5; i++)
	{
		numHelper += (int)(num * pow(10, i)) % 10 * pow(10, -i);
		if (abs(num - numHelper) < pow(10, -(i + 1)))
		{
			length += i + 1 + findNumIntLength((int)num);
			isPassedTroughLoop = true;
			break;
		}
	}

	if (!isPassedTroughLoop)
	{
		length += 5 + findNumIntLength((int)num);
	}

	return length;
}

//Convert from string to num
int convertFromStringToInt(const char* string)
{
	int num = 0;
	int start;
	if (string[0] == '-' || string[0] == '+')
		start = 1;
	else start = 0;

	for (int i = start; i < strlen(string); i++)
	{
		num += (int)(string[i] - '0') * pow(10, strlen(string) - i - 1);
	}
	return ((string[0] == '-')? num * -1: num);
}

double convertFromStringToDouble(const char* string)
{
	double num = 0;
	int start;

	if (string[0] == '-' || string[0] == '+')
		start = 1;
	else start = 0;

	bool isCommaPassed = false;

	int numsBeforeCommaCounter = 0;

	for (int i = 0; i < strlen(string); i++)
	{
		if (string[i] == '.') break;
		numsBeforeCommaCounter++;
	}

	for (int i = start; i < strlen(string); i++)
	{
		if (string[i] == '.')
		{
			isCommaPassed = true;
			i++;
		}

		if(!isCommaPassed)  num += (int)(string[i] - '0') * pow(10, numsBeforeCommaCounter - i - 1);
		else num += (int)(string[i] - '0') * pow(10, (numsBeforeCommaCounter + 1) - i - 1);
	}

	return ((string[0] == '-') ? num * -1 : num);
}

//Convert from num to string
char* convertFromIntToString(int num)
{

	char str[50];
	int bufferNum = num;
	bool isNegative = false;

	if (num < 0)
	{
		num = abs(num);
		bufferNum = abs(bufferNum);
		isNegative = true;
	}

	for (int i = 0; i < findNumIntLength(bufferNum); i++)
	{
		str[i] = (char)((num % 10) + '0');
		num /= 10;
	}

	if (isNegative)
	{
		str[findNumIntLength(bufferNum)] = '-';
		str[findNumIntLength(bufferNum) + 1] = '\0';
	}
	else
		str[findNumIntLength(bufferNum)] = '\0';

	_strrev(str);

	char* strNum = new (nothrow) char[strlen(str) + 1];
	if (!strNum) throw "Memory problem!";

	strcpy_s(strNum, strlen(str) + 1, str);

	return strNum;
}

char* convertFromDoubleToString(double num)
{
	int wholeNumPart = (int)num;
	char str[50] = "";

	if (num < 0)
	{
		str[0] = '-';
		num = abs(num);
		wholeNumPart = abs(wholeNumPart);

		for (int i = 1; i < 1 + strlen(convertFromIntToString(wholeNumPart)); i++)
		{
			str[i] = convertFromIntToString(wholeNumPart)[i - 1];
		}
		str[1 + strlen(convertFromIntToString(wholeNumPart))] = '\0';
	}
	else
	{
		strcat_s(str, strlen(convertFromIntToString(wholeNumPart)) + 1, convertFromIntToString(wholeNumPart));
	}

	strcat_s(str, ".");

	double decimalPart = (num - wholeNumPart) * pow(10, findNumDoubleLength(num - wholeNumPart) - 2);

	char decimalPartString[20] = "";
	strcpy_s(decimalPartString, strlen(convertFromIntToString(decimalPart)) + 1,convertFromIntToString(decimalPart));

	int strLength = strlen(str);
	for (int i = strLength; i < strLength + strlen(decimalPartString); i++)
	{
		str[i] = decimalPartString[i - strLength];
	}
	str[strLength + strlen(decimalPartString)] = '\0';

	char* strDoubleNum = new (nothrow) char[strlen(str) + 1];
	if (!strDoubleNum) throw "Memory problem!";

	strcpy_s(strDoubleNum, strlen(str) + 1, str);

	return strDoubleNum;
}

//Removing functions

char* removeQuotationMarks(const char* string)
{
	char* newString = new (nothrow) char[strlen(string) - 1];
	if (!newString) throw "Memory problem!";

	bool isThereSlashBefore = false;
	int indexHelper = 0;

	for (int i = 0; i < strlen(string); i++)
	{
		if (string[i] == '\\' && i != strlen(string) - 1)
		{
			if (string[i + 1] == '"')
			{
				isThereSlashBefore = true;
				newString[i - 1 - indexHelper] = '"';
				indexHelper++;
				i++;
			}
			else if (string[i + 1] == '\\')
			{
				isThereSlashBefore = true;
				newString[i - 1 - indexHelper] = '\\';
				indexHelper++;
				i++;
			}
		}
		if (i != 0 && i != strlen(string) - 1 && isThereSlashBefore == false)
			newString[i - 1 - indexHelper] = string[i];

		isThereSlashBefore = false;
	}
	newString[strlen(string) - 2 - indexHelper] = '\0';
	return newString;
}

void removeWhiteSpaces(char initialCell[])
{
	char bufferCell[50];
	int removedSpacesCount = 0;
	bool isThereAnyOtherSymbolsPassed = false;

	for (int i = 0; i < strlen(initialCell); i++)
	{
		if (initialCell[i] == ' ' && !isThereAnyOtherSymbolsPassed)
		{
			removedSpacesCount++;
		}
		else
		{
			bufferCell[i - removedSpacesCount] = initialCell[i];
			isThereAnyOtherSymbolsPassed = true;
		}

	}
	bufferCell[strlen(initialCell) - removedSpacesCount] = '\0';

	_strrev(bufferCell);
	
	removedSpacesCount = 0;
	isThereAnyOtherSymbolsPassed = false;
	strcpy_s(initialCell, 2, "");

	for (int i = 0; i < strlen(bufferCell); i++)
	{
		if (bufferCell[i] == ' ' && !isThereAnyOtherSymbolsPassed)
		{
			removedSpacesCount++;
		}
		else
		{
			initialCell[i - removedSpacesCount] = bufferCell[i];
			isThereAnyOtherSymbolsPassed = true;
		}

	}
	initialCell[strlen(bufferCell) - removedSpacesCount] = '\0';
	_strrev(initialCell);
}

//Check validity functions

bool isCharNum(char ch)
{
	switch (ch)
	{
	case '1':
		return true;
	case '2':
		return true;
	case '3':
		return true;
	case '4':
		return true;
	case '5':
		return true;
	case '6':
		return true;
	case '7':
		return true;
	case '8':
		return true;
	case '9':
		return true;
	case '0':
		return true;
	default:
		return false;
	}
}

bool isDoubleNumValid(const char* stringNum)
{
	if (strlen(stringNum) == 0) return false;

	if (!isCharNum(stringNum[0]) && stringNum[0] != '0' && stringNum[0] != '-' && stringNum[0] != '+') return false;

	if (((stringNum[0] == '-' || stringNum[0] == '+') && strlen(stringNum) >= 1 && stringNum[1] == '.') || stringNum[strlen(stringNum) - 1] == '.'
		|| (stringNum[0] == 0 && strlen(stringNum) >= 1 && stringNum[1] == '.'))
	{
		return false;
	}

	unsigned int dotCounter = 0;

	for (int i = 1; i < strlen(stringNum); i++)
	{
		if (stringNum[i] != '.' && !isCharNum(stringNum[i]) && stringNum[i] != '0')
			return false;
		if (stringNum[i] == '.') dotCounter++;
	}

	if (dotCounter != 1) return false;

	return true;
}

bool isIntNumValid(const char* stringNum)
{
	if (strlen(stringNum) == 0) return false;

	if (!isCharNum(stringNum[0]) && stringNum[0] != '-' && stringNum[0] != '+') return false;

	for (int i = 1; i < strlen(stringNum); i++)
	{
		if (!isCharNum(stringNum[i])) return false;
	}

	return true;
}

//Calculation of Formulas functions

DataType* getCellValue(const char* cell)
{
	unsigned int row;
	unsigned int col;
	bool isThereCInCellCoord = false;

	for (int i = 0; i < strlen(cell); i++)
	{
		if (cell[i] == 'C')
		{
			isThereCInCellCoord = true;
			break;
		}
	}

	if (cell[0] != 'R' || !isThereCInCellCoord)
	{
		throw "Invalid cell coordinates!";
	}

	bool isCPassed = false;
	char num[15];
	unsigned int helperIndex;

	for (int i = 1; i < strlen(cell); i++)
	{
		if (cell[i] == 'C')
		{
			isCPassed = true;
			num[i - 1] = '\0';
			row = convertFromStringToInt(num);
			strcpy_s(num, "");
			i++;
			helperIndex = i;
		}
		if (!isCPassed)
		{
			num[i - 1] = cell[i];
		}
		else if (isCPassed)
		{
			num[i - helperIndex] = cell[i];
		}
	}

	num[strlen(cell) - helperIndex] = '\0';
	col = convertFromStringToInt(num);

	if (row <= 0 || col <= 0 || row > Table::tableInstance->getRowCount() || col > Table::tableInstance->getColCount()
		|| (typeid(*(Table::tableInstance->getTable()[row - 1][col - 1])) == typeid(String)
			&& !isDoubleNumValid(Table::tableInstance->getTable()[row - 1][col - 1]->getString())
			&& !isIntNumValid(Table::tableInstance->getTable()[row - 1][col - 1]->getString())))
		return new Int(0);

	return Table::tableInstance->getTable()[row - 1][col - 1]->clone();
}

bool isCharSign(char ch)
{
	switch (ch)
	{
	case '+':
		return true;
	case '-':
		return true;
	case '*':
		return true;
	case '/':
		return true;
	case '^':
		return true;
	default:
		return false;
	}
}

bool isThereSignInArray(const char* operations, unsigned int length, char sign, int& index)
{
	for (int i = 0; i < length; i++)
	{
		if (operations[i] == sign)
		{
			index = i;
			return true;
		}
	}

	return false;
}

void removeOperationFromArray(char* operations, unsigned int length, unsigned int index)
{
	for (int i = 0; i < length; i++)
	{
		if (i >= index && i != length - 1)
			operations[i] = operations[i + 1];
	}
}

void refactorNumsArray(char nums[][15], unsigned length, unsigned int index, double result)
{
	bool isDouble = abs(result) - abs((int)result);


	for (int i = 0; i < length; i++)
	{
		if (i == index)
			strcpy_s(nums[i], (isDouble) ? strlen(convertFromDoubleToString(result)) + 1
				: strlen(convertFromIntToString(result)) + 1,
				(isDouble) ? convertFromDoubleToString(result)
				: convertFromIntToString(result));
		else if (i > index && i != length - 1)
			strcpy_s(nums[i], strlen(nums[i + 1]) + 1, nums[i + 1]);
	}
}

double checkOperandsValidity(char num[], bool& isThereError)
{
	DataType* cellInfo = nullptr;

	if (!isDoubleNumValid(num) && !isIntNumValid(num))
	{
		try
		{
			cellInfo = getCellValue(num)->clone();
		}
		catch (...)
		{
			cout << "Invalid format of formula!" << endl;
			isThereError = true;
			return 0;
		}

		if (typeid(*cellInfo) == typeid(Int) && cellInfo != nullptr)
			return cellInfo->getInt();
		else if (typeid(*cellInfo) == typeid(Double) && cellInfo != nullptr)
			return cellInfo->getDouble();
		else if (typeid(*cellInfo) == typeid(String) && cellInfo != nullptr)
			return convertFromStringToDouble(cellInfo->getString());		
	}

	else if (isDoubleNumValid(num))
		return convertFromStringToDouble(num);
	else if (isIntNumValid(num))
		return convertFromStringToInt(num);
}

char* calculateFormula(const char* formula, bool& isThereError)
{
	const unsigned int FIXEDSIZE = 15;
	char nums[FIXEDSIZE][FIXEDSIZE];
	char operations[FIXEDSIZE];

	unsigned int literalCounter = 0;
	unsigned int operationCounter = 0;

	unsigned int helpCounter = 0;

	bool isThereSignPassed = true;

	//Separate literals or cells from operations
	for (int i = 0; i < strlen(formula); i++)
	{
		while ((!isCharSign(formula[i]) || i != 0 && isThereSignPassed && formula[i] == '-'
			|| i == 0 && formula[0] == '-' && isThereSignPassed) && i != strlen(formula))
		{
			if (formula[i] != ' ') isThereSignPassed = false;

			nums[literalCounter][helpCounter++] = formula[i];
			i++;
		}
		nums[literalCounter][helpCounter++] = '\0';
		removeWhiteSpaces(nums[literalCounter]);
		literalCounter++;

		if (i != strlen(formula))
		{
			operations[operationCounter] = formula[i];
			operationCounter++;
			helpCounter = 0;
			isThereSignPassed = true;
		}

	}

	//Calculate the expression
	while (operationCounter > 0)
	{
		int index = 0;
		double result = 0;

		if (isThereSignInArray(operations, operationCounter, '^', index))
		{
			double firstNum = checkOperandsValidity(nums[index], isThereError);
			double secondNum = checkOperandsValidity(nums[index + 1], isThereError);

			if (isThereError) return nullptr;
			result = pow(firstNum, secondNum);
		}
		else if (isThereSignInArray(operations, operationCounter, '*', index))
		{
			double firstNum = checkOperandsValidity(nums[index], isThereError);
			double secondNum = checkOperandsValidity(nums[index + 1], isThereError);

			if (isThereError) return nullptr;
			result = firstNum * secondNum;
		}
		else if (isThereSignInArray(operations, operationCounter, '/', index))
		{
			double firstNum = checkOperandsValidity(nums[index], isThereError);
			double secondNum = checkOperandsValidity(nums[index + 1], isThereError);

			if (isThereError) return nullptr;
			if (secondNum == 0)
			{
				isThereError = true;
				return nullptr;
			}
			result = firstNum / secondNum;

		}
		else if (isThereSignInArray(operations, operationCounter, '-', index))
		{
			double firstNum = checkOperandsValidity(nums[index], isThereError);
			double secondNum = checkOperandsValidity(nums[index + 1], isThereError);

			if (isThereError) return nullptr;
			result = firstNum - secondNum;

		}
		else if (isThereSignInArray(operations, operationCounter, '+', index))
		{
			double firstNum = checkOperandsValidity(nums[index], isThereError);
			double secondNum = checkOperandsValidity(nums[index + 1], isThereError);

			if (isThereError) return nullptr;
			result = firstNum + secondNum;
		}

		removeOperationFromArray(operations, operationCounter, index);
		operationCounter--;
		refactorNumsArray(nums, literalCounter, index, result);
		literalCounter--;
	}

	return nums[0];

}

void evaluateFormulas(int** formulaIndexArray, unsigned int formulaCount, DataType*** table)
{
	bool isThereError = false;

	for (int i = 0; i < formulaCount; i++)
	{
		bool isThereError = false;
		char formula[50];
		char* formulaP = calculateFormula(table[formulaIndexArray[i][0]][formulaIndexArray[i][1]]->getString(), isThereError);

		if(formulaP)
		strcpy_s(formula, strlen(formulaP) + 1, formulaP);
		
		if (isThereError)
		{
			table[formulaIndexArray[i][0]][formulaIndexArray[i][1]] = new (nothrow) String("ERROR");
		}
		else
		{
			table[formulaIndexArray[i][0]][formulaIndexArray[i][1]] = new (nothrow) String(formula);
			if (!table[formulaIndexArray[i][0]][formulaIndexArray[i][1]]) throw "Memory problem!";
		}
	}
}

void removeEqualSign(char formula[])
{
	char bufferArray[50];

	for (int i = 1; i < strlen(formula); i++)
	{
		bufferArray[i - 1] = formula[i];
	}
	bufferArray[strlen(formula) - 1] = '\0';

	strcpy_s(formula, strlen(bufferArray) + 1, bufferArray);
}

//Help functions

bool isThereOnlySpaces(char array[])
{
	for (int i = 0; i < strlen(array); i++)
	{
		if (array[i] != ' ') return false;
	}
	return true;
}

unsigned int countComma(char array[], unsigned int colCount)
{
	unsigned int counter = 0;

	for (int i = 0; i < strlen(array); i++)
	{
		if (array[i] == ',' && i != strlen(array) - 1) counter++;
	}
	return ((counter != 0) ? counter + 1 : colCount);
}

char** separateCells(char array[], unsigned int colCount)
{
	char** cells = new (nothrow) char* [colCount];
	if (!cells) throw "Memory problem!";

	for (int j = 0; j < colCount; j++)
	{
		cells[j] = new (nothrow) char[15];
		if (!cells[j]) throw "Memory problem!";
	}

	unsigned int iterator = 0;
	unsigned int innerIterator = 0;
	char cell[50];

	for (int k = 0; k < strlen(array); k++)
	{
		if ((array[k] == ',' && k != strlen(array) - 1))
		{
			if (strcmp(cell, "") && !isThereOnlySpaces(cell))
			{
				cell[k - innerIterator] = '\0';
				strcpy_s(cells[iterator++], strlen(cell) + 1, cell);
				innerIterator += strlen(cell) + 1;
			}
			else
			{
				cell[k - innerIterator + 1] = '\0';
				strcpy_s(cells[iterator++], 1, "");
				innerIterator++;
			}

			strcpy_s(cell, "");

		}
		else
		{
			cell[k - innerIterator] = array[k];
		}
		if (k == strlen(array) - 1)
		{
			if (strcmp(cell, "") && !isThereOnlySpaces(cell))
			{

				if(cell[k - innerIterator] == ',')
				cell[k - innerIterator] = '\0';
				else
				cell[k - innerIterator + 1] = '\0';

				strcpy_s(cells[iterator++], strlen(cell) + 1, cell);
				innerIterator += strlen(cell) + 1;
			}
			else
			{
				if (cell[k - innerIterator] == ',')
					cell[k - innerIterator] = '\0';
				else
					cell[k - innerIterator + 1] = '\0';

				strcpy_s(cells[iterator++], 1, "");
				innerIterator++;
			}

			strcpy_s(cell, "");
		}
	}

	return cells;

}

//Printing and reading

void printValue(DataType* dataType)
{
	if (typeid(*dataType) == typeid(Int))
		cout << dataType->getInt();
	else if (typeid(*dataType) == typeid(Double))
		cout << dataType->getDouble();
	else if (typeid(*dataType) == typeid(String))
		cout << dataType->getString();
}

void readTableFromFile(fstream& fileR)
{
	unsigned int rowCount;
	unsigned int colCount;

	fileR >> rowCount;
	fileR >> colCount;
	
	DataType*** table = allocateBufferTable(rowCount, colCount);

	// Allocate array to store the coordinates of the formulas,
	// so they can be evaluated after the initialization of the whole table
	int** formulaIndexArray = new (nothrow) int*[rowCount];
	if (!formulaIndexArray) throw "Memory problem!";

	unsigned int formulaCount = 0;

	for (int i = 0; i < rowCount; i++)
	{
		formulaIndexArray[i] = new (nothrow) int[2];
		if (!formulaIndexArray[i]) throw "Memory problem!";
	}

	for (int i = 0; i < rowCount; i++)
	{
		//Read the end line after the size of the table
		if (i == 0)
		{
			char ch;
			fileR.get(ch);
		}

		//Read line by line the file
		char initialLine[100];
		fileR.getline(initialLine, 100);

		//Count commas on 1 line
		unsigned int cellCount = countComma(initialLine, colCount);

		//Separate the cells in array
		char** cells = separateCells(initialLine, colCount);

		//Fill the empty cells
		for (int k = cellCount; k < colCount; k++)
		{
			strcpy_s(cells[k], 1, "");
			cellCount++;
		}

		//Convert to one of the three allowed variable types, each cell can hold
		for (int j = 0; j < colCount; j++)
		{
			removeWhiteSpaces(cells[j]);

			if (isDoubleNumValid(cells[j]))
			{
				table[i][j] = new (nothrow) Double(convertFromStringToDouble(cells[j]));
				if (!table[i][j]) throw "Memory problem!";
			}
			else if (isIntNumValid(cells[j]))
			{
				table[i][j] = new (nothrow) Int(convertFromStringToInt(cells[j]));
				if (!table[i][j]) throw "Memory problem!";
			}
			else if (cells[j][0] == '"' && cells[j][strlen(cells[j]) - 1] == '"')
			{
				table[i][j] = new (nothrow) String(removeQuotationMarks(cells[j]));
				if (!table[i][j]) throw "Memory problem!";
			}
			else if (cells[j][0] == '=')
			{
				char formulaWithoutEqualSign[50];
				strcpy_s(formulaWithoutEqualSign, cells[j]);

				removeEqualSign(formulaWithoutEqualSign);

				table[i][j] = new (nothrow) String(formulaWithoutEqualSign);
				if (!table[i][j]) throw "Memory problem!";

				formulaIndexArray[formulaCount][0] = i;
				formulaIndexArray[formulaCount][1] = j;
				formulaCount++;
			}
			else
			{
				if(!strcmp(cells[j], "")) table[i][j] = new (nothrow) String("#");
				else
				{
					table[i][j] = new (nothrow) String("#");
					cout << "Invalid format of info on row " << i << " and col " << j << endl;
				}

			}
		}

	}

	//Initialize the table
	Table::tableInstance = new (nothrow) Table(table, rowCount, colCount);
	if (!Table::tableInstance) throw "Memory problem!";

	//Evaluate the formulas
	evaluateFormulas(formulaIndexArray, formulaCount, table);

	//Tests
	for (int i = 0; i < rowCount; i++)
	{

		for (int j = 0; j < colCount; j++)
		{
			printValue(table[i][j]);
			cout << " ";
		}
		cout << endl;
	}

}

void readFromFile()
{
	char fileName[50];
	cout << "Enter the name of the file you want to read from: ";
	cin.getline(fileName, 50);

	strcat_s(fileName, ".txt");

	fstream fileR(fileName, ios::in);
	if (!fileR.is_open()) throw "A file with such name can't be opened!";

	readTableFromFile(fileR);
}

int main()
{
	readFromFile();

	return 0;
}

