#include "MainOperationClass.h"
#include "doctest.h"

TEST_CASE("Open function test")
{
	// The command is in format open <file path>
	//Replace with existing file path before testing
	char command[] = "open C:\\Users\\Miro-GamingPC\\source\\repos\\Project_2_OOP\\Project_2_OOP\\table.txt";
	MainOperationClass::getInstance().executeCommand(command);
	CHECK(Table::isTableOpened == true);
}

TEST_CASE("Edit function test")
{

	SUBCASE("Edit function with int value")
	{
		char change[] = "1";
		MainOperationClass::getInstance().editTable(1, 1, change);
		//The coordinates of the cells start from (1, 1)
		CHECK(Table::tableInstance->getTable()[0][0]->getInt() == 1);

	}

	SUBCASE("Edit function with double value")
	{
		char change[] = "1.5";
		MainOperationClass::getInstance().editTable(2, 2, change);
		CHECK(abs(Table::tableInstance->getTable()[1][1]->getDouble() - 1.5) < 0.0001);
	}

	SUBCASE("Edit function with string value")
	{
		char change[] = "\"Procent\"";
		char changeWithoutQuatations[] = "Procent";
		//The strings have to be entered with quotaion marks, so another string with removed quotations is used for the check
		MainOperationClass::getInstance().editTable(3, 3, change);
		CHECK(!strcmp(Table::tableInstance->getTable()[2][2]->getString(), changeWithoutQuatations));
	}

	SUBCASE("Edit function with formula")
	{
		char change[] = "=R1C1 + R2C2";
		MainOperationClass::getInstance().editTable(4, 4, change);
		CHECK(!strcmp(Table::tableInstance->getTable()[3][3]->getFormula(), "=R1C1 + R2C2"));
	}

}

TEST_CASE("Print function test")
{
	// (1, 1) => 1 
	// (2, 2) => 1.5
	// (3, 3) => Procent
	// (4, 4) => = R1C1 + R2C2 => 1 + 1.5 => 2.5
	MainOperationClass::getInstance().printTable();
}

TEST_CASE("Save as function test")
{
	// The command is in format saveas <file path>
	//Replace with existing file path before testing
	char command[] = "saveas C:\\Users\\Miro-GamingPC\\source\\repos\\Project_2_OOP\\Project_2_OOP\\tableSave.txt";
	MainOperationClass::getInstance().executeCommand(command);
	REQUIRE_FALSE(Table::isThereUnsavedChanges);
}

TEST_CASE("Save function test")
{
	//Set this bool to true just to check if the save was passed successfully
	Table::isThereUnsavedChanges = true;
	MainOperationClass::getInstance().saveTable();
	REQUIRE_FALSE(Table::isThereUnsavedChanges);
}

TEST_CASE("Close function test")
{
	MainOperationClass::getInstance().closeTable();
	REQUIRE_FALSE(Table::isTableOpened);
}
