//
// Copyright (C) 2010 Piotr Zagawa
//
// Released under BSD License
//

#pragma once

#include "SqlCommon.h"


namespace sql
{

class Field
{
public:
	friend class FieldSet;

private:
	string _name;
	field_use _use;
	field_type _type;
	int _index;
	int _flags;

public:
	Field(field_use use);
	Field(string name, field_type type, int flags = flag_none);
	Field(const Field& value);

public:
	bool isKeyIdField();
	bool isEndingField();

public:
	int getIndex();
	string getName();
	string getTypeStr();
	field_type getType();
	bool isPrimaryKey();
	bool isNotNull();

public:
	string getDefinition();
	static Field* createFromDefinition(string value);

};


//sql eof
};
