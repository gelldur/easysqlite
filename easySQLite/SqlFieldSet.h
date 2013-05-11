//
// Copyright (C) 2010 Piotr Zagawa
//
// Released under BSD License
//

#pragma once

#include <map>
#include <vector>
#include "SqlField.h"


namespace sql
{

class FieldSet
{
private:
	std::vector<Field> _vec;
	std::map<string, Field*> _map;

private:
	void copy(const std::vector<Field>& definition);

public:
	FieldSet(Field* definition);
	FieldSet(std::vector<Field>& definition);
	FieldSet(const FieldSet& source);

public:
	string toString();
	int count();
	Field* getByIndex(int index);
	Field* getByName(string name);

public:
	string definitionHash();
	string getDefinition();
	static FieldSet* createFromDefinition(string value);

};


//sql eof
};
