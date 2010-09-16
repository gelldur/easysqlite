#include "SqlTable.h"


namespace sql
{

Table::Table(sqlite3* db, string tableName, Field* definition)
	: _db(db), _tableName(tableName), _recordset(db, definition)
{
}

Table::Table(sqlite3* db, string tableName, FieldSet* fields)
	: _db(db), _tableName(tableName), _recordset(db, fields)
{
}

string Table::name()
{
	return _tableName;
}

string Table::getDefinition()
{
	return "CREATE TABLE " + _tableName + " (" + fields()->getDefinition() + ")";
}

FieldSet* Table::fields()
{
	return _recordset.fields();
}

string Table::toString()
{
	string s;

	for (int index = 0; index < fields()->count(); index++)
	{
		if (Field* f = fields()->getByIndex(index))
		{
			s += intToStr(index + 1) + ". " + f->getDefinition();

			if (index < (fields()->count() - 1))
				s +="\r\n";
		}
	}

	return s;
}

string Table::errMsg()
{
	return _recordset.errMsg();
}

sqlite3* Table::getHandle()
{
	return _db;
}

bool Table::create()
{
	if (exists())
		return true;

	const string sqlDefinition = getDefinition();

	if (_recordset.query(sqlDefinition))
	{
		return true;
	}

	return false;
}

bool Table::query(string queryStr)
{
	if (_recordset.query(queryStr))
	{
		return true;
	}
	return false;
}

bool Table::open()
{
	const string queryStr = "select * from " + _tableName;

	if (_recordset.query(queryStr))
	{
		return true;
	}

	return false;
}

//example of whereCondition:
//"_ID > 40"
bool Table::open(string whereCondition)
{
	const string queryStr = "select * from " + _tableName + (whereCondition.empty() ? "" : " where " + whereCondition);

	if (_recordset.query(queryStr))
	{
		return true;
	}

	return false;
}

//example of sortBy:
//"_ID desc"
bool Table::open(string whereCondition, string sortBy)
{
	const string queryStr = "select * from " + _tableName
		+ (whereCondition.empty() ? "" : " where " + whereCondition)
		+ (sortBy.empty() ? "" : " order by " + sortBy);

	if (_recordset.query(queryStr))
	{
		return true;
	}

	return false;
}

bool Table::truncate()
{
	const string queryStr = "delete from " + _tableName;

	if (_recordset.query(queryStr))
	{
		return true;
	}

	return false;
}

bool Table::remove()
{
	const string queryStr = "drop table " + _tableName;

	if (_recordset.query(queryStr))
	{
		return true;
	}

	return false;
}

bool Table::deleteRecords(string whereCondition)
{	
	const string sql = "delete from " + _tableName + (whereCondition.empty() ? "" : " where " + whereCondition);

	RecordSet rs(_db, _recordset.fields());

	if (rs.query(sql))
	{
		return true;
	}

	return false;
}

int Table::recordCount()
{
	return _recordset.count();
}

int Table::totalRecordCount()
{
	const string queryStr = "select count(*) from " + _tableName;

	RecordSet rs(_db, _recordset.fields());

	if (rs.query(queryStr))
	{
		if (Value* value = rs.getTopRecordFirstValue())
		{
			return (int)value->asInteger();
		}
	}

	return -1;
}

bool Table::exists()
{
	const string queryStr = "select count(*) from sqlite_master where type = 'table' and name = '" + _tableName + "'";

	RecordSet rs(_db, _recordset.fields());

	if (rs.query(queryStr))
	{
		if (Value* value = rs.getTopRecordFirstValue())
		{
			return (value->asInteger() > 0);
		}
	}

	return false;
}

Record* Table::getRecord(int record_index)
{
	return _recordset.getRecord(record_index);
}

Record* Table::getTopRecord()
{
	return getRecord(0);
}

Record* Table::getRecordByKeyId(integer keyId)
{
	const string queryStr = "select * from " + _tableName + " where _ID = " + intToStr(keyId);

	if (_recordset.query(queryStr))
	{
		if (_recordset.count() > 0)
		{
			return _recordset.getRecord(0);
		}
	}

	return NULL;
}

bool Table::addRecord(Record* record)
{
	if (record)
	{
		const string sql = record->toSqlInsert(name());

		RecordSet rs(_db, _recordset.fields());

		if (rs.query(sql))
		{
			return true;
		}
	}
	return false;
}

bool Table::updateRecord(Record* record)
{
	if (record)
	{
		const string sql = record->toSqlUpdate(name());

		RecordSet rs(_db, _recordset.fields());

		if (rs.query(sql))
		{
			return true;
		}
	}
	return false;
}

bool Table::copyRecords(Table& source)
{
	for (int index = 0; index < source.recordCount(); index++)
	{
		if (Record* record = source.getRecord(index))
		{
			if (!addRecord(record))
				return false;
		}
	}
	return true;
}

Table* Table::createFromDefinition(sqlite3* db, string tableName, string fieldsDefinition)
{
	if (FieldSet* fields = FieldSet::createFromDefinition(fieldsDefinition))
	{
		Table* table = new Table(db, tableName, fields);

		delete fields;

		return table;
	}

	return NULL;
}

bool Table::backup(Table& source)
{
	bool bResult = false;

	if (exists())
		remove();

	if (create())
	{
		if (source.open())
		{
			if (copyRecords(source))
			{
				if (source.totalRecordCount() == totalRecordCount())
					bResult = true;
			}
		}
	}

	return bResult;
}


//sql eof
};
