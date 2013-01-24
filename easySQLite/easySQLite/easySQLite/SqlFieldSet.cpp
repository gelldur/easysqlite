#include "SqlFieldSet.h"
#include "SqlCommon.h"

namespace sql
{

FieldSet::FieldSet(Field* definition)
{
	//make fields map from array
	if (definition)
	{
		int index = 0;
		while (true)
		{
			Field& field = definition[index];

			if (field.isEndingField())
				break;

			field._index = index;

			_vec.push_back(field);			
			_map[field.getName()] = &field;

			index++;
		}
	}
}

FieldSet::FieldSet(std::vector<Field>& definition)
{
	copy(definition);
}

FieldSet::FieldSet(const FieldSet& source)
{
	copy(source._vec);
}

void FieldSet::copy(const std::vector<Field>& definition)
{
	_vec = definition;

	//make fields map from vector
	for (int index = 0; index < (int)_vec.size(); index++)
	{
		Field& field = _vec[index];
		_map[field.getName()] = &field;
	}
}

int FieldSet::count()
{
	return (int)_vec.size();
}

Field* FieldSet::getByIndex(int index)
{
	if ((index >= 0) && (index < count()))
		return &_vec[index];

	return NULL;
}

Field* FieldSet::getByName(string name)
{
	return _map[name];
}

string FieldSet::getDefinition()
{
	string s;

	for (int index = 0; index < count(); index++)
	{
		if (Field* f = getByIndex(index))
		{
			s += f->getDefinition();
			if (index < (count() - 1))
				s += ", ";
		}
	}

	return s;
}

string FieldSet::definitionHash()
{
	return generateSHA(getDefinition());
}

string FieldSet::toString()
{
	string s;

	for (int index = 0; index < count(); index++)
	{
		if (Field* f = getByIndex(index))
		{
			s += f->getName();
			if (index < (count() - 1))
				s += ", ";
		}
	}

	return s;
}

FieldSet* FieldSet::createFromDefinition(string value)
{
	std::vector<string> vec;
	listToVector(value, vec, ",");

	std::vector<Field> fields;

	for (int index = 0; index < (int)vec.size(); index++)
	{
		std::string definition = vec[index];

		if (Field* field = Field::createFromDefinition(definition))
		{
			Field f(*field);
			fields.push_back(f);
			delete field;
		} else {
			return NULL;
		}
	}

	return new FieldSet(fields);
}


//sql eof
};
