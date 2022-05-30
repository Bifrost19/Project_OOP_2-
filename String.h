#ifndef STRING_H
#define STRING_H
#include "DataType.h"

class String : public DataType
{
private:
	char* str;

	void copyString(const char* _str);
public:
	String() : str(nullptr) {}

	String(const char* _str);

	String(const String& other);

	String& operator= (const String& other);

	String* clone() const override { return new (std::nothrow) String(*this); }

	char* getString() const override { return this->str; }

	void setStr(const char* _str);

	~String() { delete[] this->str; }
};
#endif

