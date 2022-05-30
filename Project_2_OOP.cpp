#include <iostream>
#include <fstream>
#include "DataType.h"
#include "Int.h"
#include "Double.h"
#include "String.h"
#include "Table.h"

using namespace std;

Table* Table::tableInstance = nullptr;

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
	if (num < 1)
	{
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

	if (num < 0)
	{
		str[0] = '-';
		num = abs(num);
		bufferNum = abs(bufferNum);
	}

	for (int i = 0; i < findNumIntLength(bufferNum); i++)
	{
		str[i] = (char)((num % 10) + '0');
		num /= 10;
	}

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

	double decimalPart = (num - (double)wholeNumPart) * pow(10, findNumDoubleLength(num - wholeNumPart) - 2);
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

	cout << initialCell;
}

void readTableFromFile(fstream& fileR)
{
	unsigned int rowCount;
	unsigned int colCount;

	fileR >> rowCount;
	fileR >> colCount;

	DataType*** table = allocateBufferTable(rowCount, colCount);

	char ch;

	for (int i = 0; i < rowCount; i++)
	{

		for (int j = 0; j < colCount; j++)
		{
			char initialCell[50];
			fileR.getline(initialCell, 50, ',');

			removeWhiteSpaces(initialCell);

			double comparePercision = pow(10, -(findNumDoubleLength(convertFromStringToDouble(initialCell)) - findNumIntLength((int)convertFromStringToDouble(initialCell))));
			double doubleDifference = abs(convertFromStringToDouble(convertFromDoubleToString(convertFromStringToDouble(initialCell))) - convertFromStringToDouble(initialCell));

			if (doubleDifference < comparePercision)
			{
				table[i][j] = new (nothrow) Double(convertFromStringToDouble(initialCell));
				if (!table[i][j]) throw "Memory problem!";
			}
			else if (!strcmp(convertFromIntToString(convertFromStringToInt(initialCell)), initialCell))
			{

			}
			else if ()
			{

			}
		}
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

void printValue(DataType* dataType)
{
	if (typeid(*dataType) == typeid(Int))
		cout << dataType->getInt();
	else if(typeid(*dataType) == typeid(Double))
		cout << dataType->getDouble();
	else if (typeid(*dataType) == typeid(String))
		cout << dataType->getString();
}

int main()
{
	//cout <<( abs(convertFromStringToDouble(convertFromDoubleToString(convertFromStringToDouble("9.94"))) - convertFromStringToDouble("9.94"))
	//	   < pow(10,-( findNumDoubleLength(convertFromStringToDouble("9.94")) - findNumIntLength((int)convertFromStringToDouble("9.94")))));
	

	//cout << abs(convertFromStringToDouble(convertFromDoubleToString(convertFromStringToDouble("9.94"))) - convertFromStringToDouble("9.94"));
	//cout << endl << pow(10, -(findNumDoubleLength(convertFromStringToDouble("9.94")) - findNumIntLength((int)convertFromStringToDouble("9.94"))));

	//char arr[] = "    soodd   ";
		//removeWhiteSpaces(arr);
	//cout << convertFromDoubleToString(-9.94);
	//cout << findNumDoubleLength(-0.01);
	//cout << strcmp(convertFromIntToString(convertFromStringToInt("456")), "456");
	//readFromFile();
	//cout << removeQuotationMarks("\"asd\"");
	//readFromFile();
	//Test
	//Int* i1 = new Int(666);
	//Int* i2 = new Int(420);
	//Double* d1 = new Double(3.33);
	//String* str1 = new String("Franjo");
	//DataType*** dt1 = new DataType**[1];
	//dt1[0] = new DataType * [3];
	//dt1[0][0] = i1;
	//dt1[0][1] = d1;
	//dt1[0][2] = str1;
	//printValue(dt1[0][0]);
	
	return 0;
}

