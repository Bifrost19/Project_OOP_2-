#include "MainOperationClass.h"

using namespace std;

//Static vars
Table* Table::tableInstance = nullptr;
bool Table::isTableOpened = false;
bool Table::isThereUnsavedChanges = false;
char* Table::currentFileName = nullptr;

int main()
{
	char command[100];
	do
	{
		cout << endl << "Type a command (use 'help' for help)" << endl;
		cout << ">";
		cin.getline(command, 100);
		MainOperationClass::getInstance().executeCommand(command);
	} while (strcmp(command, "exit") || Table::isThereUnsavedChanges);

	cout << "Exiting program..." << endl;

	// Static vars are deallocated in execute command function
	return 0;
}