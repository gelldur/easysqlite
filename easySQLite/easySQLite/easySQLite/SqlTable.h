//
// Copyright (C) 2010 Piotr Zagawa
//
// Released under BSD License
//

#pragma once

#include "sqlite3.h"
#include "SqlCommon.h"
#include "SqlFieldSet.h"
#include "SqlRecordSet.h"


namespace sql
{

class Table
{
private:
	sqlite3* _db;
	string _tableName;
	RecordSet _recordset;

public:
	Table(sqlite3* db, string tableName, Field* definition);
	Table(sqlite3* db, string tableName, FieldSet* fields);

public:
	string name();
	string getDefinition();
	string toString();
	string errMsg();
	FieldSet* fields();
	sqlite3* getHandle();

public:
	bool create();
	bool exists();
	bool remove();
	bool truncate();

public:
	bool open();
	bool open(string whereCondition);
	bool open(string whereCondition, string sortBy);
	bool query(string queryStr);
	int totalRecordCount();

public:
	int recordCount();
	Record* getRecord(int record_index);
	Record* getTopRecord();
	Record* getRecordByKeyId(integer keyId);

public:
	bool addRecord(Record* record);
	bool updateRecord(Record* record);
	bool deleteRecords(string whereCondition);
	bool copyRecords(Table& source);
	bool backup(Table& source);

public:
	static Table* createFromDefinition(sqlite3* db, string tableName, string fieldsDefinition);

};


//sql eof
};
