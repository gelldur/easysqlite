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

static void setValues(Record& r)
{
	r.setString("name", "test_text1");
	r.setInteger("valueInt", 890);
	r.setDouble("valueDbl", 0.345);
	r.setString("valueTxt", "test_text2");
	r.setBool("valueBol", true);
	r.setTime("valueTme", 0);
}

static void cmpValues(Record& r)
{
	CHECK_EQUAL("test_text1", r.getValue("name")->asString());
	CHECK_EQUAL(890, r.getValue("valueInt")->asInteger());
	CHECK_EQUAL(0.345, r.getValue("valueDbl")->asDouble());
	CHECK_EQUAL("test_text2", r.getValue("valueTxt")->asString());
	CHECK_EQUAL(true, r.getValue("valueBol")->asBool());
	CHECK_EQUAL(0, r.getValue("valueTme")->asTime().asInteger());
}

TEST(RecordSetModify)
{
	sql::Database db;

	try
	{
		db.open("UnitTests.db");

		Table tb(db.getHandle(), "test", definition());

		if (tb.exists())
			tb.remove();

		tb.create();

		Record r(tb.fields());

		setValues(r);

		for (int index = 0; index < 10; index++)
			tb.addRecord(&r);

		tb.open();

		CHECK_EQUAL(10, tb.recordCount());

		for (int index = 0; index < tb.recordCount(); index++)
			if (Record* record = tb.getRecord(index))
				cmpValues(*record);

	} catch (Exception e) {
		CHECK_EQUAL("*", e.msg());
	}
}

TEST(RecordSetData)
{
	sql::Database db;

	try
	{
		db.open("UnitTests.db");

		Table tb(db.getHandle(), "test", definition());

		if (tb.exists())
			tb.remove();

		tb.create();

		Record r(tb.fields());

		setValues(r);

		tb.addRecord(&r);

		tb.open();

		CHECK_EQUAL(1, tb.recordCount());

		if (Record* record = tb.getTopRecord())
		{				
			cmpValues(*record);			
			CHECK_EQUAL("1|test_text1|890|0.345|test_text2|1|01-01-1970 01:00, Thu", record->toString());
		} else {
			CHECK_EQUAL("*", "record not found");
		}

	} catch (Exception e) {
		CHECK_EQUAL("*", e.msg());
	}
}


//eofn
};
