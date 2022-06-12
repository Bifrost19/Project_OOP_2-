#ifndef FORMULA
#define FORMULA
#include "DataType.h"

class Formula : public DataType
{
private:
	char* formula;

	void copyFormula(const char* formula);
public:
	Formula() : formula(nullptr) {}

	Formula(const char* formula);

	Formula(const Formula& other);

	Formula& operator= (const Formula& other);

	~Formula() { delete[] this->formula; }

	Formula* clone() const override { return new (std::nothrow) Formula(*this); }

	char* getFormula() const override { return this->formula; }

	void setFormula(const char* formula);
};
#endif

