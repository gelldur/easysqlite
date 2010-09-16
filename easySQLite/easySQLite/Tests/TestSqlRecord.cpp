#include "../UnitTest++/src/UnitTest++.h"
#include "../easySQLite/SqlRecord.h"


namespace Tests
{

using namespace sql;


TEST(RecordSetup)
{
	Field def[] = 
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

	FieldSet fields(def);

	Record r(&fields);

	CHECK(r.fieldByName("name"));
	CHECK(r.fieldByName("valueInt"));
	CHECK(r.fieldByName("valueDbl"));
	CHECK(r.fieldByName("valueTxt"));
	CHECK(r.fieldByName("valueBol"));
	CHECK(r.fieldByName("valueTme"));

	CHECK_EQUAL(7, r.columnCount());

	if (r.columnCount() == 7)
	{
		//check if values by name and by index are the same
		CHECK_EQUAL(r.getValue("_ID"), r.getValue(0));
		CHECK_EQUAL(r.getValue("name"), r.getValue(1));
		CHECK_EQUAL(r.getValue("valueInt"), r.getValue(2));
		CHECK_EQUAL(r.getValue("valueDbl"), r.getValue(3));
		CHECK_EQUAL(r.getValue("valueTxt"), r.getValue(4));
		CHECK_EQUAL(r.getValue("valueBol"), r.getValue(5));
		CHECK_EQUAL(r.getValue("valueTme"), r.getValue(6));
	}
}

TEST(RecordQueries)
{
	Field def[] = 
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

	FieldSet fields(def);

	Record r(&fields);

	Record rnew(r);

	CHECK_EQUAL(rnew.toSql(), r.toSql());

	CHECK_EQUAL("insert into test (_ID, name, valueInt, valueDbl, valueTxt, valueBol, valueTme) values (null, null, null, null, null, null, null)",
		r.toSqlInsert("test"));

	CHECK_EQUAL("update test set name=null, valueInt=null, valueDbl=null, valueTxt=null, valueBol=null, valueTme=null where _ID = null",
		r.toSqlUpdate("test"));

	CHECK_EQUAL("null, null, null, null, null, null, null",
		r.toSql());

	CHECK_EQUAL("null|null|null|null|null|null|null",
		r.toString());
}

TEST(RecordSetValues)
{
	Field def[] = 
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

	FieldSet fields(def);

	Record r(&fields);

	r.setInteger("_ID", 123);
	r.setString("name", "test_text1");
	r.setInteger("valueInt", 890);
	r.setDouble("valueDbl", 0.345);
	r.setString("valueTxt", "test_text2");
	r.setBool("valueBol", true);
	r.setTime("valueTme", 0);

	CHECK_EQUAL(123, r.getKeyIdValue()->asInteger());

	CHECK_EQUAL("123|test_text1|890|0.34500000|test_text2|1|0", r.toString());

	CHECK_EQUAL("123, 'test_text1', 890, 0.34500000, 'test_text2', 1, 0", r.toSql());
}

TEST(RecordGetValues)
{
	Field def[] = 
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

	FieldSet fields(def);

	Record r(&fields);

	r.setInteger("_ID", 123);
	r.setString("name", "test_text1");
	r.setInteger("valueInt", 890);
	r.setDouble("valueDbl", 0.345);
	r.setString("valueTxt", "test_text2");
	r.setBool("valueBol", true);
	r.setTime("valueTme", 0);

	CHECK_EQUAL(123, r.getValue("_ID")->asInteger());
	CHECK_EQUAL("test_text1", r.getValue("name")->asString());
	CHECK_EQUAL(890, r.getValue("valueInt")->asInteger());
	CHECK_EQUAL(0.345, r.getValue("valueDbl")->asDouble());
	CHECK_EQUAL("test_text2", r.getValue("valueTxt")->asString());
	CHECK_EQUAL(true, r.getValue("valueBol")->asBool());
	CHECK_EQUAL(0, r.getValue("valueTme")->asTime().asInteger());
}


//eofn
};
