#include "String.h"
#include "String.h"

void String::copyString(const char* _str)
{
	this->str = new (std::nothrow) char[strlen(_str) + 1];
	if (!this->str) throw "Memory problem!";

	strcpy_s(this->str, strlen(_str) + 1, _str);
}

String::String(const char* _str)
{
	copyString(_str);
}

String::String(const String& other)
{
	copyString(other.str);
}

String& String::operator=(const String& other)
{
	if (this != &other)
		copyString(other.str);
	return *this;
}

void String::setStr(const char* _str)
{
	copyString(_str);
}
