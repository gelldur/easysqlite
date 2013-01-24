#include "SqlField.h"


namespace sql
{

Field::Field(field_use use)
{
	this->_name.clear();
	this->_use = use;
	this->_type = type_undefined;
	this->_index = -1;
	this->_flags = flag_none;

	if (use == FIELD_KEY)
	{
		this->_name = "_ID";
		this->_type = type_int;
		this->_flags = flag_primary_key;
	}
}

Field::Field(string name, field_type type, int flags)
{
	this->_name = name;
	this->_use = FIELD_DEFAULT;
	this->_type = type;
	this->_index = -1;
	this->_flags = flags;
}

Field::Field(const Field& value)
{
	_name = value._name;
	_use = value._use;
	_type = value._type;
	_index = value._index;
	_flags = value._flags;
}

bool Field::isKeyIdField()
{
	return (_use == FIELD_KEY);
}

bool Field::isEndingField()
{
	return (_use == DEFINITION_END);
}

field_type Field::getType()
{
	return _type;
}

int Field::getIndex()
{
	return _index;
}

string Field::getName()
{
	return _name;
}

string Field::getTypeStr()
{
	switch (_type)
	{
	case type_int: return "INTEGER";
	case type_text: return "TEXT";
	case type_float: return "REAL";
	case type_bool: return "INTEGER";
	case type_time: return "INTEGER";
    default:
        break;
	}
	return "";
}

bool Field::isPrimaryKey()
{
	return ((_flags & flag_primary_key) == flag_primary_key);
}

bool Field::isNotNull()
{
	return ((_flags & flag_not_null) == flag_not_null);
}

string Field::getDefinition()
{
	string value = _name + " " + getTypeStr();

	if (isPrimaryKey())
		value += " PRIMARY KEY";

	if (isNotNull())
		value += " NOT NULL";

	return trim(value);
}

Field* Field::createFromDefinition(string value)
{
	std::vector<string> vec;

	listToVector(value, vec, " ");

	const int count = (int)vec.size();

	string _name;

	field_use _use = FIELD_DEFAULT;
	field_type _type = type_undefined;

	int _flags = flag_none;

	//parse name
	if (count > 0)
		_name = vec[0];

	//parse type
	if (count > 1)
	{
		std::string& type = vec[1];

		if (type.compare("INTEGER") == 0)
			_type = type_int;

		if (type.compare("TEXT") == 0)
			_type = type_text;

		if (type.compare("REAL") == 0)
			_type = type_float;
	}

	//parse optional flags
	if (count > 2)
	{
		std::string flags = vec[2];

		if (count > 3)
			flags += " " + vec[3];

		if (flags.find("PRIMARY KEY") != std::string::npos)
		{
			_use = FIELD_KEY;
			_flags = flag_primary_key;
		}

		if (flags.find("NOT NULL") != std::string::npos)
		{
			_flags |= flag_not_null;
		}
	}

	Field* field = NULL;

	if (!_name.empty())
	{
		if (_type != type_undefined)
		{
			if (_use == FIELD_DEFAULT)
			{
				field = new Field(_name, _type, _flags);
			} else {
				field = new Field(_use);
			}
		}
	}

	return field;
}


//sql eof
};
