#include "Formula.h"

void Formula::copyFormula(const char* formula)
{
	this->formula = new (std::nothrow) char[strlen(formula) + 1];
	if (!this->formula) throw "Memory problem!";

	strcpy_s(this->formula, strlen(formula) + 1, formula);
}

Formula::Formula(const char* formula)
{
	copyFormula(formula);
}

Formula::Formula(const Formula& other)
{
	copyFormula(other.formula);
}

Formula& Formula::operator= (const Formula& other)
{
	if (this != &other)
		copyFormula(other.formula);
	return *this;
}

void Formula::setFormula(const char* formula)
{
	copyFormula(formula);
}