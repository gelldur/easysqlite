#include "SqlRecordSet.h"


namespace sql
{

RecordSet::RecordSet(sqlite3* db)
	: _fields(NULL)
{
	_db = db;
	_err_msg.clear();
	_result_query = SQLITE_ERROR;
	_records.clear();
}

RecordSet::RecordSet(sqlite3* db, FieldSet* fields)
	: _fields(*fields)
{
	_db = db;
	_err_msg.clear();
	_result_query = SQLITE_ERROR;
	_records.clear();
}

RecordSet::RecordSet(sqlite3* db, Field* definition)
	: _fields(definition)
{
	_db = db;
	_err_msg.clear();
	_result_query = SQLITE_ERROR;
	_records.clear();
}

RecordSet::~RecordSet(void)
{
	close();
}

string RecordSet::errMsg()
{
	return _err_msg;
}

void RecordSet::close()
{
	_err_msg.clear();
	_records.clear();
	_result_query = SQLITE_ERROR;
}

FieldSet* RecordSet::fields()
{
	return &_fields;
}

bool RecordSet::isResult()
{
	return (_result_query == SQLITE_OK);
}

int RecordSet::count()
{
	return static_cast<int>(_records.size());
}

int RecordSet::on_next_record(void* param, int column_count, char** values, char** columns)
{
	RecordSet* recordset = (RecordSet*)param;

	Record record(recordset->fields());

	record.initColumnCount(column_count);

  for (int index = 0; index < column_count; index++)
	{
		char* value = values[index];

		if (Field* field = recordset->_fields.getByIndex(index))
		{
			record.initColumnValue(index, value, field->getType());			
		}
  }

	recordset->_records.push_back(record);

  return DATASET_ITERATION_CONTINUE;
}

bool RecordSet::query(string sql)
{
	close();

	char* error = NULL;

	_result_query = sqlite3_exec(_db, sql.c_str(), on_next_record, this, &error);

	if (isResult())
	{
		return true;
	}

	if (error)
	{
		_err_msg = error;
		sqlite3_free(error);
	}

	THROW_EXCEPTION("RecordSet::query: " + errMsg())

	return false;
}

Record* RecordSet::getRecord(int record_index)
{
	if ((record_index >= 0) && (record_index < (int)_records.size()))
		return &_records.at(record_index);

	return NULL;
}

string RecordSet::toString()
{
	string s;

	for (int record_index = 0; record_index < count(); record_index++)
	{
		if (Record* record = getRecord(record_index))
		{
			s += intToStr(record_index + 1) + ". " + record->toString();
			s += "\r\n";
		}
	}

	return s;
}

Record* RecordSet::getTopRecord()
{
	if (isResult())
	{
		return getRecord(0);
	}
	return NULL;
}

Value* RecordSet::getTopRecordFirstValue()
{
	if (isResult())
	{
		if (Record* record = getRecord(0))
		{
			return record->getValue(0);
		}
	}
	return NULL;
}


//sql eof
};
