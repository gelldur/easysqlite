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

TEST(Database)
{
	sql::Database db;

	try
	{
		db.open("UnitTests.db");

		Table tb(db.getHandle(), "test", definition());

		if (tb.exists())
			tb.remove();

		tb.create();

		tb.open();

		CHECK_EQUAL(0, tb.recordCount());

		Record r(tb.fields());
		setValues(r);

		//test transaction commit
		tb.truncate();
		CHECK_EQUAL(0, tb.totalRecordCount());

		if (db.transactionBegin())
		{
			tb.addRecord(&r);
			tb.addRecord(&r);
			tb.addRecord(&r);

			db.transactionCommit();
			CHECK_EQUAL(3, tb.totalRecordCount());

		} else {
			CHECK_EQUAL("transaction begin fail", "");
		}

		//test transaction rollback
		tb.truncate();
		CHECK_EQUAL(0, tb.totalRecordCount());

		if (db.transactionBegin())
		{
			tb.addRecord(&r);
			tb.addRecord(&r);
			tb.addRecord(&r);

			db.transactionRollback();
			CHECK_EQUAL(0, tb.totalRecordCount());

		} else {
			CHECK_EQUAL("transaction begin fail", "");
		}

	} catch (Exception e) {
		CHECK_EQUAL("*", e.msg());
	}
}


//eofn
};
