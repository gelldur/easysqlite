#include "../UnitTest++/src/UnitTest++.h"
#include "../easySQLite/SqlRecordSet.h"
#include "../easySQLite/SqlDatabase.h"
#include "../easySQLite/SqlTable.h"


namespace Tests
{

using namespace sql;


static Field* definition()
{
	static Field def[] = 
	{
		Field(FIELD_KEY),
		Field("name", type_text, flag_not_null),
		Field("valueInt", type_int),
		Field("valueDbl", type_float),
		Field("valueTxt", type_text),
		Field("valueBol", type_bool, flag_not_null),
		Field("valueTme", type_time),
		Field(DEFINITION_END),
	};

	return &def[0];
}

static string strdef()
{
	sql::string strdef = "CREATE TABLE test (";
	strdef += "_ID INTEGER PRIMARY KEY, ";
	strdef += "name TEXT NOT NULL, ";
	strdef += "valueInt INTEGER, ";
	strdef += "valueDbl REAL, ";
	strdef += "valueTxt TEXT, ";
	strdef += "valueBol INTEGER NOT NULL, ";
	strdef += "valueTme INTEGER";
	strdef += ")";

	return strdef;
}

static void setValues(Record& r)
{
	r.setString("name", "test_text1");
	r.setInteger("valueInt", 890);
	r.setDouble("valueDbl", 0.345);
	r.setString("valueTxt", "test_text2");
	r.setBool("valueBol", true);
	r.setTime("valueTme", 0);
}

TEST(TableSetup)
{
	sql::Database db;

	try
	{
		db.open("UnitTests.db");

		Table tb(db.getHandle(), "test", definition());

		CHECK_EQUAL("test", tb.name());

		CHECK_EQUAL(strdef(), tb.getDefinition());

		if (tb.exists())
			tb.remove();

		tb.create();

		CHECK(tb.exists());

		Record r(tb.fields());

		setValues(r);

		//add 10 records
		for (int index = 0; index < 10; index++)
			tb.addRecord(&r);

		tb.open();

		CHECK_EQUAL(10, tb.recordCount());

		//check _ID of record with index = 5
		CHECK_EQUAL(6, tb.getRecord(5)->getKeyIdValue()->asInteger());

		//select 4 records
		tb.open("_ID >= 5 AND _ID <= 8");

		CHECK_EQUAL(4, tb.recordCount());

		//update data of record with _ID = 5
		if (Record* record = tb.getRecordByKeyId(5))
		{
			r.setString("name", "new_text5");
			r.setInteger("valueInt", 111);
			r.setDouble("valueDbl", 0.222);
			r.setString("valueTxt", "new_text8");
			r.setBool("valueBol", false);
			r.setTime("valueTme", 1);

			tb.updateRecord(record);
		} else {
			CHECK_EQUAL("*", "record not found");
		}

		//get updated record
		tb.open("_ID = 5");

		//check returned result
		CHECK_EQUAL(1, tb.recordCount());

		if (Record* record = tb.getRecord(0))
		{
			CHECK_EQUAL("new_text5", r.getValue("name")->asString());
			CHECK_EQUAL(111, r.getValue("valueInt")->asInteger());
			CHECK_EQUAL(0.222, r.getValue("valueDbl")->asDouble());
			CHECK_EQUAL("new_text8", r.getValue("valueTxt")->asString());
			CHECK_EQUAL(false, r.getValue("valueBol")->asBool());
			CHECK_EQUAL(1, r.getValue("valueTme")->asTime().asInteger());
		} else {
			CHECK_EQUAL("*", "record not found");
		}

		//delete 7 records
		tb.deleteRecords("_ID < 4");
		tb.open();

		CHECK_EQUAL(7, tb.recordCount());

		//remove all records
		tb.truncate();
		tb.open();

		CHECK_EQUAL(0, tb.recordCount());

	} catch (Exception e) {
		CHECK_EQUAL("*", e.msg());
	}
}

TEST(TableCreationFromString)
{
	sql::Database db;

	try
	{
		db.open("UnitTests.db");

		Table tb(db.getHandle(), "test", definition());

		sql::string strdef;
		strdef += "_ID INTEGER PRIMARY KEY, ";
		strdef += "name TEXT NOT NULL, ";
		strdef += "valueInt INTEGER, ";
		strdef += "valueDbl REAL, ";
		strdef += "valueTxt TEXT, ";
		strdef += "valueBol INTEGER NOT NULL, ";
		strdef += "valueTme INTEGER";

		if (Table* table = Table::createFromDefinition(db.getHandle(), "test1", strdef))
		{
			CHECK_EQUAL(table->fields()->definitionHash(), tb.fields()->definitionHash());

		} else {
			CHECK_EQUAL("*", "table not created");
		}

	} catch (Exception e) {
		CHECK_EQUAL("*", e.msg());
	}
}


//eofn
};
