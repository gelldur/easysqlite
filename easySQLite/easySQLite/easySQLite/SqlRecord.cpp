#include "SqlRecord.h"


namespace sql
{

Record::Record(FieldSet* fields)
	: _fields(fields)
{
	initColumnCount(_fields->count());
}

Record::Record(Record* record)
	: _fields(record->_fields)
{
	initColumnCount(_fields->count());
	_values = record->_values;
}

Record::Record(const Record& record)
	: _fields(record._fields)
{
	initColumnCount(_fields->count());
	_values = record._values;
}

FieldSet* Record::fields()
{
	return _fields;
}

void Record::initColumnCount(int columns)
{
	_values.resize(columns);
}

void Record::initColumnValue(int column_index, char* value, field_type type)
{
	_values[column_index].setValue(value, type);
}

int Record::columnCount()
{
	return static_cast<int>(_values.size());
}

Value* Record::getValue(int column_index)
{
	if ((column_index >= 0) && (column_index < (int)_values.size()))
		return &_values.at(column_index);

	return NULL;
}

Value* Record::getValue(string fieldName)
{
	if (Field* field = _fields->getByName(fieldName))
		return getValue(field->getIndex());

	return NULL;
}

Value* Record::getKeyIdValue()
{
	for (int index = 0; index < _fields->count(); index++)
	{
		if (Field* field = _fields->getByIndex(index))
		{
			if (field->isKeyIdField())
				return getValue(field->getIndex());
		}
	}

	return NULL;
}

string Record::toString()
{
	string s;

	for (int column = 0; column < columnCount(); column++)
		if (Value* value = getValue(column))
		{
			s += value->toString();
			if (column < (columnCount() - 1))
				s += "|";
		}

	return s;
}

string Record::toSql()
{
	string s;

	for (int index = 0; index < _fields->count(); index++)
	{
		if (Field* field = _fields->getByIndex(index))
		{
			if (Value* value = getValue(field->getName()))
			{
				s += value->toSql(field->getType());

				if (index < (_fields->count() - 1))
					s += ", ";
			}
		}
	}

	return s;
}

void Record::setNull(int index)
{
	if (Value* v = getValue(index))
		v->setNull();
}

void Record::setString(int index, string value)
{
	if (Value* v = getValue(index))
		v->setString(value);
}

void Record::setInteger(int index, integer value)
{
	if (Value* v = getValue(index))
		v->setInteger(value);
}

void Record::setDouble(int index, double value)
{
	if (Value* v = getValue(index))
		v->setDouble(value);
}

void Record::setBool(int index, bool value)
{
	if (Value* v = getValue(index))
		v->setBool(value);
}

void Record::setTime(int index, time value)
{
	if (Value* v = getValue(index))
		v->setTime(value);
}

Field* Record::fieldByName(string fieldName)
{
	if (Field* field = _fields->getByName(fieldName))
	{
		return field;
	} else {
		THROW_EXCEPTION("Record::fieldByName: field '" + fieldName + "' not found")
		return NULL;
	}
}

void Record::setNull(string fieldName)
{
	if (Field* field = fieldByName(fieldName))
		setNull(field->getIndex());
}

void Record::setString(string fieldName, string value)
{
	if (Field* field = fieldByName(fieldName))
		setString(field->getIndex(), value);
}

void Record::setInteger(string fieldName, integer value)
{
	if (Field* field = fieldByName(fieldName))
		setInteger(field->getIndex(), value);
}

void Record::setDouble(string fieldName, double value)
{
	if (Field* field = fieldByName(fieldName))
		setDouble(field->getIndex(), value);
}

void Record::setBool(string fieldName, bool value)
{
	if (Field* field = fieldByName(fieldName))
		setBool(field->getIndex(), value);
}

void Record::setTime(string fieldName, time value)
{
	if (Field* field = fieldByName(fieldName))
		setTime(field->getIndex(), value);
}

string Record::toSqlInsert(string tableName)
{
	string s = "insert into " + tableName + " ";

	s += "(" + _fields->toString() + ")";

	s += " values ";

	s += "(" + toSql() + ")";

	return s;
}

string Record::toSqlUpdate(string tableName)
{
	string s = "update " + tableName + " set ";

	for (int index = 0; index < _fields->count(); index++)
	{
		if (Field* field = _fields->getByIndex(index))
		{
			if (field->isKeyIdField())
				continue;

			if (Value* value = getValue(field->getName()))
			{
				s += field->getName() + "=" + value->toSql(field->getType());

				if (index < (_fields->count() - 1))
					s += ", ";
			}
		}
	}

	if (Value* value = getKeyIdValue())
		s += " where _ID = " + value->toSql(type_int);

	return s;
}

bool Record::equalsColumnValue(Record* record, string fieldName)
{
	if (record)
	{
		if (Value* value1 = getValue(fieldName))
			if (Value* value2 = record->getValue(fieldName))
				return value1->equals(*value2);
	}
	return false;
}

bool Record::equalsValues(Record* record)
{
	if (record)
	{
		for (int index = 0; index < _fields->count(); index++)
		{
			if (Field* field = _fields->getByIndex(index))
			{
				if (field->isKeyIdField())
					continue;

				if (Value* value1 = getValue(field->getName()))
					if (Value* value2 = record->getValue(field->getName()))
						if (!value1->equals(*value2))
							return false;
			}
		}
		return true;
	}
	return false;
}


//sql eof
};
