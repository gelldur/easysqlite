#include "../UnitTest++/src/UnitTest++.h"
#include "../easySQLite/SqlField.h"


namespace Tests
{

using namespace sql;


TEST(FieldKeySetup)
{
	Field f(FIELD_KEY);

	CHECK_EQUAL("_ID INTEGER PRIMARY KEY", f.getDefinition());	
	CHECK_EQUAL("_ID", f.getName());
	CHECK_EQUAL(type_int, f.getType());
	CHECK_EQUAL("INTEGER", f.getTypeStr());
	CHECK(!f.isEndingField());
	CHECK(f.isKeyIdField());
	CHECK(f.isPrimaryKey());
}

TEST(FieldEndSetup)
{
	Field f(DEFINITION_END);

	CHECK_EQUAL("", f.getDefinition());	
	CHECK_EQUAL("", f.getName());
	CHECK_EQUAL(type_undefined, f.getType());
	CHECK_EQUAL("", f.getTypeStr());
	CHECK(f.isEndingField());
}

TEST(FieldsDefinition)
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

	CHECK_EQUAL("name TEXT NOT NULL", def[1].getDefinition());
	CHECK_EQUAL("valueInt INTEGER", def[2].getDefinition());
	CHECK_EQUAL("valueDbl REAL", def[3].getDefinition());
	CHECK_EQUAL("valueTxt TEXT", def[4].getDefinition());
	CHECK_EQUAL("valueBol INTEGER NOT NULL", def[5].getDefinition());
	CHECK_EQUAL("valueTme INTEGER", def[6].getDefinition());
}

TEST(FieldCreation)
{
	Field f(FIELD_KEY);

	CHECK_EQUAL("_ID INTEGER PRIMARY KEY", f.getDefinition());	

	Field f1(f);

	CHECK_EQUAL("_ID INTEGER PRIMARY KEY", f1.getDefinition());	
}

TEST(FieldCreationFromString)
{
	Field* f = Field::createFromDefinition("_ID INTEGER PRIMARY KEY");

	CHECK(f != NULL);

	if (f)
	{
		CHECK_EQUAL("_ID INTEGER PRIMARY KEY", f->getDefinition());	
		delete f;
	}
}


//eofn
};
