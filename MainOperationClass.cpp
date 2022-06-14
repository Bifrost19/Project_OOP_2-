#include "MainOperationClass.h"

//Deallocation functions
void MainOperationClass::deallocateStaticVars()
{

	delete Table::tableInstance;
	Table::tableInstance = nullptr;

	delete[] Table::currentFileName;
	Table::currentFileName = nullptr;
}

//Allocation functions
DataType*** MainOperationClass::allocateBufferTable(unsigned int rowCount, unsigned int colCount)
{
	DataType*** table = new (nothrow) DataType * *[rowCount];
	if (!table) throw "Memory problem!";

	for (int i = 0; i < rowCount; i++)
	{
		table[i] = new (std::nothrow) DataType * [colCount];
		if (!table[i]) throw "Memory problem!";
	}

	return table;
}

//Functions to find the length of a number
int MainOperationClass::findNumIntLength(int num)
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

int MainOperationClass::findNumDoubleLength(double num)
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
int MainOperationClass::convertFromStringToInt(const char* string)
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
	return ((string[0] == '-') ? num * -1 : num);
}

double MainOperationClass::convertFromStringToDouble(const char* string)
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

		if (!isCommaPassed)  num += (int)(string[i] - '0') * pow(10, numsBeforeCommaCounter - i - 1);
		else num += (int)(string[i] - '0') * pow(10, (numsBeforeCommaCounter + 1) - i - 1);
	}

	return ((string[0] == '-') ? num * -1 : num);
}

//Convert from num to string
char* MainOperationClass::convertFromIntToString(int num)
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

char* MainOperationClass::convertFromDoubleToString(double num)
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
	strcpy_s(decimalPartString, strlen(convertFromIntToString(decimalPart)) + 1, convertFromIntToString(decimalPart));

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

//Remove functions
char* MainOperationClass::removeQuotationMarks(const char* string)
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

void MainOperationClass::removeWhiteSpaces(char initialCell[])
{
	char bufferCell[100];
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

char* MainOperationClass::removeEqualSign(const char* formula)
{
	char bufferArray[50];

	for (int i = 1; i < strlen(formula); i++)
	{
		bufferArray[i - 1] = formula[i];
	}
	bufferArray[strlen(formula) - 1] = '\0';

	char* bufferArrayP = new (nothrow) char[strlen(bufferArray) + 1];
	if (!bufferArrayP) throw "Memory problem!";

	strcpy_s(bufferArrayP, strlen(bufferArray) + 1, bufferArray);
	return bufferArrayP;
}

//Check validity functions
bool MainOperationClass::isCharNum(char ch)
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

bool MainOperationClass::isDoubleNumValid(const char* stringNum)
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

bool MainOperationClass::isIntNumValid(const char* stringNum)
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
DataType* MainOperationClass::getCellValue(const char* cell, unsigned int rowF, unsigned int colF)
{
	unsigned int row;
	unsigned int col;
	bool isThereCInCellCoord = false;

	for (int i = 0; i < strlen(cell); i++)
	{
		if (cell[i] == 'C' || cell[i] == 'c')
		{
			isThereCInCellCoord = true;
			break;
		}
	}

	if ((cell[0] != 'R' && cell[0] != 'r') || !isThereCInCellCoord)
	{
		throw "Invalid cell coordinates!";
	}

	bool isCPassed = false;
	char num[15];
	unsigned int helperIndex;

	for (int i = 1; i < strlen(cell); i++)
	{
		if (cell[i] == 'C' || cell[i] == 'c')
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

	//Prevent from infinite recursion
	if (row - 1 == rowF && col - 1 == colF)
		return nullptr;

	if (row <= 0 || col <= 0 || row > Table::tableInstance->getRowCount() || col > Table::tableInstance->getColCount()
		|| (typeid(*(Table::tableInstance->getTable()[row - 1][col - 1])) == typeid(String)
			&& !isDoubleNumValid(Table::tableInstance->getTable()[row - 1][col - 1]->getString())
			&& !isIntNumValid(Table::tableInstance->getTable()[row - 1][col - 1]->getString())))
		return new Int(0);
	else if (typeid(*(Table::tableInstance->getTable()[row - 1][col - 1])) == typeid(Formula))
		return evaluateFormulas(Table::tableInstance->getTable()[row - 1][col - 1], rowF, colF);

	return Table::tableInstance->getTable()[row - 1][col - 1]->clone();
}

bool MainOperationClass::isCharSign(char ch)
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

bool MainOperationClass::isThereSignInArray(const char* operations, unsigned int length, char sign, int& index)
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

void MainOperationClass::removeOperationFromArray(char* operations, unsigned int length, unsigned int index)
{
	for (int i = 0; i < length; i++)
	{
		if (i >= index && i != length - 1)
			operations[i] = operations[i + 1];
	}
}

void MainOperationClass::refactorNumsArray(char nums[][15], unsigned length, unsigned int index, double result)
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

double MainOperationClass::checkOperandsValidity(char num[], bool& isThereError, unsigned int rowF, unsigned int colF)
{
	DataType* cellInfo = nullptr;

	if (!isDoubleNumValid(num) && !isIntNumValid(num))
	{
		try
		{
			cellInfo = getCellValue(num, rowF, colF);
			if(cellInfo)
			cellInfo = getCellValue(num, rowF, colF)->clone();
		}
		catch (...)
		{
			cout << "Invalid format of formula!" << endl;
			isThereError = true;
			return 0;
		}

		if (cellInfo == nullptr)
		{
			isThereError = true;
			return 0;
		}
		else if (typeid(*cellInfo) == typeid(Int) && cellInfo != nullptr)
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

char* MainOperationClass::calculateFormula(const char* formula, bool& isThereError, unsigned int rowF, unsigned int colF)
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
			double firstNum = checkOperandsValidity(nums[index], isThereError, rowF, colF);
			double secondNum = checkOperandsValidity(nums[index + 1], isThereError, rowF, colF);

			if (isThereError) return nullptr;
			result = pow(firstNum, secondNum);
		}
		else if (isThereSignInArray(operations, operationCounter, '*', index))
		{
			double firstNum = checkOperandsValidity(nums[index], isThereError, rowF, colF);
			double secondNum = checkOperandsValidity(nums[index + 1], isThereError, rowF, colF);

			if (isThereError) return nullptr;
			result = firstNum * secondNum;
		}
		else if (isThereSignInArray(operations, operationCounter, '/', index))
		{
			double firstNum = checkOperandsValidity(nums[index], isThereError, rowF, colF);
			double secondNum = checkOperandsValidity(nums[index + 1], isThereError, rowF, colF);

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
			double firstNum = checkOperandsValidity(nums[index], isThereError, rowF, colF);
			double secondNum = checkOperandsValidity(nums[index + 1], isThereError, rowF, colF);

			if (isThereError) return nullptr;
			result = firstNum - secondNum;

		}
		else if (isThereSignInArray(operations, operationCounter, '+', index))
		{
			double firstNum = checkOperandsValidity(nums[index], isThereError, rowF, colF);
			double secondNum = checkOperandsValidity(nums[index + 1], isThereError, rowF, colF);

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

DataType* MainOperationClass::evaluateFormulas(DataType* formula, unsigned int rowF, unsigned int colF)
{
	bool isThereError = false;
	char formulaArray[50];

	char* formulaP = calculateFormula(removeEqualSign(formula->getFormula()), isThereError, rowF, colF);

	if (formulaP)
		strcpy_s(formulaArray, strlen(formulaP) + 1, formulaP);

	if (isThereError)
	{
		return new (nothrow) String("ERROR");
	}
	else
	{
		if (isDoubleNumValid(formulaArray))
		{
			return new (nothrow) Double(convertFromStringToDouble(formulaArray));
		}
		else if (isIntNumValid(formulaArray))
		{
			return new (nothrow) Int(convertFromStringToInt(formulaArray));
		}
	}
}

//Help functions
bool MainOperationClass::isThereOnlySpaces(char array[])
{
	for (int i = 0; i < strlen(array); i++)
	{
		if (array[i] != ' ') return false;
	}
	return true;
}

char** MainOperationClass::separateCells(char array[], unsigned int colCount, unsigned int& cellCountInCells)
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

				if (cell[k - innerIterator] == ',')
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

	cellCountInCells = iterator;
	return cells;

}

void MainOperationClass::checkSizeOfTable(unsigned int row, unsigned int col)
{
	if (row > Table::tableInstance->getRowCount() && col > Table::tableInstance->getColCount())
		Table::tableInstance->resizeTable(row, col);
	else if (row > Table::tableInstance->getRowCount())
		Table::tableInstance->resizeTable(row, Table::tableInstance->getColCount());
	else if (col > Table::tableInstance->getColCount())
		Table::tableInstance->resizeTable(Table::tableInstance->getRowCount(), col);
}

unsigned int MainOperationClass::getLengthOfCell(DataType* cell, unsigned int rowF, unsigned int colF)
{
	//Find the length of the evaluation of the formula
	if (typeid(*cell) == typeid(Int))
		return findNumIntLength(cell->getInt());
	else if (typeid(*cell) == typeid(Double))
		return findNumDoubleLength(cell->getDouble());
	else if (typeid(*cell) == typeid(String))
		return strlen(cell->getString());
	else if (typeid(*cell) == typeid(Formula))
	{
		DataType* formula = evaluateFormulas(cell, rowF, colF);
		return getLengthOfCell(formula, rowF, colF);
	}
}

unsigned int MainOperationClass::getLengthOfCellFormula(DataType* cell)
{
	//Find the length of the formula
	if (typeid(*cell) == typeid(Int))
		return findNumIntLength(cell->getInt());
	else if (typeid(*cell) == typeid(Double))
		return findNumDoubleLength(cell->getDouble());
	else if (typeid(*cell) == typeid(String))
		return strlen(cell->getString());
	else if (typeid(*cell) == typeid(Formula))
		return strlen(cell->getFormula());
}

void MainOperationClass::placeSpace(unsigned int quantity)
{
	for (int i = 0; i < quantity; i++)
	{
		cout << " ";
	}
}

void MainOperationClass::placeSpaceInFile(ofstream& fileW, unsigned int quantity)
{
	for (int i = 0; i < quantity; i++)
	{
		fileW << " ";
	}
}

char* MainOperationClass::getFileName(char* filePath)
{
	_strrev(filePath);
	char fileName[50];
	unsigned int fileNameLength = 0;

	for (int i = 0; i < strlen(filePath); i++)
	{
		if (filePath[i] == '\\')
		{
			fileName[i] = '\0';
			break;
		}
		fileName[i] = filePath[i];
		fileNameLength++;
	}
	_strrev(fileName);
	fileName[fileNameLength] = '\0';

	char* fileNameP = new (nothrow) char[fileNameLength + 1];
	if (!fileNameP) throw "Memory problem!";
	strcpy_s(fileNameP, fileNameLength + 1, fileName);

	_strrev(filePath);

	return fileNameP;
}

char* MainOperationClass::separateFileNameFromCommand(char* string, const char* command, unsigned int& strLength)
{
	char fileName[100];
	unsigned int commandLength = strlen(command);

	for (int i = commandLength; i < strlen(string); i++)
	{
		fileName[i - commandLength] = string[i];
		if(i == strlen(string) - 1) fileName[i - commandLength + 1] = '\0';
	}

	removeWhiteSpaces(fileName);

	strLength = strlen(fileName);

	char* fileNameP = new (nothrow) char[strLength + 1];
	if (!fileNameP) throw "Memory problem!";
	strcpy_s(fileNameP, strLength + 1, fileName);

	return fileNameP;
}

bool MainOperationClass::isThereCommandInString(char* string, const char* command)
{
	if (strlen(string) <= strlen(command)) return false;

	for (int i = 0; i < strlen(command); i++)
	{
		if (string[i] != command[i]) return false;
	}

	return true;
}

void MainOperationClass::verifyDataForEditFunc()
{
	if (!Table::isTableOpened)
	{
		cout << "There isn't any opened table to edit!" << endl;
		return;
	}

	cout << "...../Editing mode/......" << endl;

	int row;
	int col;

	//Row
	do
	{
		cout << "Enter the row: ";
		cin >> row;
	} while (row <= 0);

	//Col
	do
	{
		cout << "Enter the column: ";
		cin >> col;
	} while (col <= 0);

	//Data
	char cellInfo[50];

	cin.ignore();
	cin.getline(cellInfo, 50);

	editTable(row, col, cellInfo);
}

//Write and read to file
void MainOperationClass::readTableFromFile(fstream& fileR)
{
	unsigned int rowCount;
	unsigned int colCount;

	fileR >> rowCount;
	fileR >> colCount;

	DataType*** table = allocateBufferTable(rowCount, colCount);

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

		unsigned int cellCountInCells = 0;

		//Separate the cells in array
		char** cells = separateCells(initialLine, colCount, cellCountInCells);

		//Fill the empty cells
		for (int k = cellCountInCells; k < colCount; k++)
		{
			strcpy_s(cells[k], 1, "");
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

				table[i][j] = new (nothrow) Formula(cells[j]);
				if (!table[i][j]) throw "Memory problem!";
			}
			else
			{
				if (!strcmp(cells[j], "")) table[i][j] = new (nothrow) String("");
				else
				{
					cout << "Error: row  " << i + 1 << ", col " << j + 1 << ", " << cells[j] << " is unknown data type." << endl;
					throw "Error: Invalid format";
				}

			}
		}
	}

	//Initialize the table
	Table::tableInstance = new (nothrow) Table(table, rowCount, colCount);
	if (!Table::tableInstance) throw "Memory problem!";
}

void MainOperationClass::readFromFile(char* command, bool& isOpenSuccessful)
{
	fstream fileR;

	char fileName[100];
	unsigned int strLength = 0;
	strcpy_s(fileName, strLength + 1, separateFileNameFromCommand(command, "open", strLength));

	fileR.open(fileName, ios::in);
	if (!fileR.is_open())
	{
		cout << "A file with such name can't be opened!" << endl;
		isOpenSuccessful = false;
		return;
	}
	else
	{
		Table::currentFileName = new (nothrow) char[strlen(fileName) + 1];
		if (!Table::currentFileName) throw "Memory problem!";

		strcpy_s(Table::currentFileName, strlen(fileName) + 1, fileName);
	}

	readTableFromFile(fileR);
	Table::isTableOpened = true;
	fileR.close();
}

void MainOperationClass::writeToFile(ofstream& fileW)
{
	fileW << Table::tableInstance->getRowCount() << " "
		<< Table::tableInstance->getColCount() << endl;

	//Store the length of the longest element in each column
	int* elementLengthArray = new (nothrow) int[Table::tableInstance->getColCount()];
	if (!elementLengthArray) throw "Memory problem!";

	for (int i = 0; i < Table::tableInstance->getColCount(); i++)
	{
		int maxLength = 0;

		for (int j = 0; j < Table::tableInstance->getRowCount(); j++)
		{
			if (maxLength < ((typeid(*Table::tableInstance->getTable()[j][i]) == typeid(String) && strcmp(Table::tableInstance->getTable()[j][i]->getString(), "")) ? getLengthOfCellFormula(Table::tableInstance->getTable()[j][i]) + 2 : getLengthOfCellFormula(Table::tableInstance->getTable()[j][i])))
				maxLength = ((typeid(*Table::tableInstance->getTable()[j][i]) == typeid(String) && strcmp(Table::tableInstance->getTable()[j][i]->getString(), "")) ? getLengthOfCellFormula(Table::tableInstance->getTable()[j][i]) + 2 : getLengthOfCellFormula(Table::tableInstance->getTable()[j][i]));
		}

		elementLengthArray[i] = maxLength;
	}

	//Print the table
	for (int i = 0; i < Table::tableInstance->getRowCount(); i++)
	{
		for (int j = 0; j < Table::tableInstance->getColCount(); j++)
		{
			if (elementLengthArray[j] == 0)
				fileW << " ";

			else if ((typeid(*Table::tableInstance->getTable()[i][j]) == typeid(String) && strcmp(Table::tableInstance->getTable()[i][j]->getString(), "")))
				placeSpaceInFile(fileW, elementLengthArray[j] - getLengthOfCellFormula(Table::tableInstance->getTable()[i][j]) - 2);
			else
				placeSpaceInFile(fileW, elementLengthArray[j] - getLengthOfCellFormula(Table::tableInstance->getTable()[i][j]));

			Table::tableInstance->printValueInFile(Table::tableInstance->getTable()[i][j], fileW);
			fileW << ",";
		}
		fileW << endl;
	}
	delete[] elementLengthArray;
}

//Execute command function
void MainOperationClass::executeCommand(char command[])
{
	if (isThereCommandInString(command, "open"))
		openTable(command);
	else if (!strcmp(command, "close"))
		closeTable();
	else if (!strcmp(command, "help"))
		help();
	else if (!strcmp(command, "edit"))
		verifyDataForEditFunc();
	else if (!strcmp(command, "print"))
		printTable();
	else if (!strcmp(command, "save"))
		saveTable();
	else if (isThereCommandInString(command, "saveas"))
		saveAsTable(command);
	else if (!strcmp(command, "clear"))
		system("cls");
	else if (!strcmp(command, "exit"))
	{
		if (Table::isThereUnsavedChanges)
			cout << "You have an opened file with unsaved changes, please select close or save first." << endl;

		else if(!Table::isPassedThroughCloseFunc)
			deallocateStaticVars();

		Table::isPassedThroughCloseFunc = false;
		return;
	}

	else
		cout << "Invalid command!" << endl;
}

//Main functionalities
void MainOperationClass::editTable(unsigned int row, unsigned int col, char cellInfo[])
{

	DataType* cellData = nullptr;

	removeWhiteSpaces(cellInfo);

	if (isDoubleNumValid(cellInfo))
	{
		checkSizeOfTable(row, col);
		delete[] Table::tableInstance->getTable()[row - 1][col - 1];
		Table::tableInstance->getTable()[row - 1][col - 1] = new (std::nothrow) Double(convertFromStringToDouble(cellInfo));
		if (!Table::tableInstance->getTable()[row - 1][col - 1]) throw "Memory problem!";
	}
	else if (isIntNumValid(cellInfo))
	{
		checkSizeOfTable(row, col);
		delete[] Table::tableInstance->getTable()[row - 1][col - 1];
		Table::tableInstance->getTable()[row - 1][col - 1] = new (std::nothrow) Int(convertFromStringToInt(cellInfo));
		if (!Table::tableInstance->getTable()[row - 1][col - 1]) throw "Memory problem!";
	}
	else if (cellInfo[0] == '"' && cellInfo[strlen(cellInfo) - 1] == '"')
	{
		checkSizeOfTable(row, col);
		delete[] Table::tableInstance->getTable()[row - 1][col - 1];
		Table::tableInstance->getTable()[row - 1][col - 1] = new (std::nothrow) String(removeQuotationMarks(cellInfo));
		if (!Table::tableInstance->getTable()[row - 1][col - 1]) throw "Memory problem!";
	}
	else if (cellInfo[0] == '=')
	{
		checkSizeOfTable(row, col);

		delete[] Table::tableInstance->getTable()[row - 1][col - 1];
		Table::tableInstance->getTable()[row - 1][col - 1] = new (std::nothrow) Formula(cellInfo);
		if (!Table::tableInstance->getTable()[row - 1][col - 1]) throw "Memory problem!";
	}
	else
	{
		cout << "Error: Invalid data" << endl;
		return;
	}
	Table::isThereUnsavedChanges = true;
	cout << "Changes were made successfully!" << endl;
}

void MainOperationClass::printTable()
{
	if (!Table::isTableOpened)
	{
		cout << "There isn't any opened table to print!" << endl;
		return;
	}

	cout << endl;

	//Store the length of the longest element in each column
	int* elementLengthArray = new (nothrow) int[Table::tableInstance->getColCount()];
	if (!elementLengthArray) throw "Memory problem!";

	for (int i = 0; i < Table::tableInstance->getColCount(); i++)
	{
		int maxLength = 0;

		for (int j = 0; j < Table::tableInstance->getRowCount(); j++)
		{
			if (maxLength < getLengthOfCell(Table::tableInstance->getTable()[j][i], j, i))
				maxLength = getLengthOfCell(Table::tableInstance->getTable()[j][i], j, i);
		}

		elementLengthArray[i] = maxLength;
	}

	//Print the table
	for (int i = 0; i < Table::tableInstance->getRowCount(); i++)
	{
		for (int j = 0; j < Table::tableInstance->getColCount(); j++)
		{
			if (elementLengthArray[j] == 0)
				cout << " ";
			else
				placeSpace(elementLengthArray[j] - getLengthOfCell(Table::tableInstance->getTable()[i][j], i, j));

			if (typeid(*Table::tableInstance->getTable()[i][j]) != typeid(Formula))
				Table::tableInstance->printValue(Table::tableInstance->getTable()[i][j]);
			else
			{
				DataType* formula = evaluateFormulas(Table::tableInstance->getTable()[i][j], i, j);
				Table::tableInstance->printValue(formula);
			}

			cout << "|";
		}
		cout << endl;
	}
	delete[] elementLengthArray;
}

void MainOperationClass::saveTable()
{
	if (!Table::isTableOpened)
	{
		cout << "There isn't any opened table to save!" << endl;
		return;
	}

	ofstream fileW(Table::currentFileName);

	if (!fileW.is_open()) throw "Problem with opening the file!";

	writeToFile(fileW);

	fileW.close();

	Table::isThereUnsavedChanges = false;
	cout << "Successfully saved " << getFileName(Table::currentFileName) << endl;
}

void MainOperationClass::saveAsTable(char* command)
{
	if (!Table::isTableOpened)
	{
		cout << "There isn't any opened table to save!" << endl;
		return;
	}

	char fileName[100];
	unsigned int strLength = 0;
	strcpy_s(fileName, strLength + 1, separateFileNameFromCommand(command, "saveas", strLength));

	ofstream fileW(fileName);

	if (!fileW.is_open()) throw "Problem with opening the file!";

	writeToFile(fileW);

	fileW.close();

	Table::isThereUnsavedChanges = false;
	cout << "Successfully saved " << getFileName(fileName) << endl;
}

void MainOperationClass::closeTable()
{
	if (!Table::isTableOpened)
	{
		cout << "There isn't any opened table to close!" << endl;
		return;
	}

	cout << "Successfully closed " << getFileName(Table::currentFileName) << endl;
	deallocateStaticVars();
	Table::isPassedThroughCloseFunc = true;
	Table::isThereUnsavedChanges = false;
	Table::isTableOpened = false;
}

void MainOperationClass::openTable(char* command)
{
	bool isOpenSuccessful = true;

	if (Table::isTableOpened)
	{
		cout << "There is opened table already!" << endl;
		return;
	}

	readFromFile(command, isOpenSuccessful);
	if (isOpenSuccessful) cout << "Successfully opened " << getFileName(Table::currentFileName) << endl;
}

void MainOperationClass::help()
{
	cout << "open <file>        opens <file>" << endl
		<< "close              closes currently opened file" << endl
		<< "save               saves the currently open file" << endl
		<< "saveas <file>      saves the currently open file in <file>" << endl
		<< "edit               makes changes to the table" << endl
		<< "print              prints the table on the screen" << endl
		<< "clear              clears the console prompt" << endl
		<< "help               prints this information" << endl
		<< "exit               exists the program" << endl;
}