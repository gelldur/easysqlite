#include "../UnitTest++/src/UnitTest++.h"
#include "../easySQLite/SqlFieldSet.h"


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

TEST(FieldSet)
{
	FieldSet fields(definition());

	CHECK_EQUAL(7, fields.count());

	if (fields.count() == 7)
	{
		for (int index = 0; index < 7; index++)
			CHECK_EQUAL(index, fields.getByIndex(index)->getIndex());
	}

	CHECK(fields.getByName("name"));
	CHECK(fields.getByName("valueInt"));
	CHECK(fields.getByName("valueDbl"));
	CHECK(fields.getByName("valueTxt"));
	CHECK(fields.getByName("valueBol"));
	CHECK(fields.getByName("valueTme"));

	CHECK_EQUAL("name", (fields.getByName("name") ? fields.getByName("name")->getName() : "*"));
	CHECK_EQUAL("valueInt", (fields.getByName("valueInt") ? fields.getByName("valueInt")->getName() : "*"));
	CHECK_EQUAL("valueDbl", (fields.getByName("valueDbl") ? fields.getByName("valueDbl")->getName() : "*"));
	CHECK_EQUAL("valueTxt", (fields.getByName("valueTxt") ? fields.getByName("valueTxt")->getName() : "*"));
	CHECK_EQUAL("valueBol", (fields.getByName("valueBol") ? fields.getByName("valueBol")->getName() : "*"));
	CHECK_EQUAL("valueTme", (fields.getByName("valueTme") ? fields.getByName("valueTme")->getName() : "*"));
}

TEST(FieldSetCreation)
{
	FieldSet fields(definition());

	sql::string strdef;
	strdef += "_ID INTEGER PRIMARY KEY, ";
	strdef += "name TEXT NOT NULL, ";
	strdef += "valueInt INTEGER, ";
	strdef += "valueDbl REAL, ";
	strdef += "valueTxt TEXT, ";
	strdef += "valueBol INTEGER NOT NULL, ";
	strdef += "valueTme INTEGER";

	CHECK_EQUAL(strdef, fields.getDefinition());	
	CHECK_EQUAL("63f1ae61aad81ca9e98eb7c9c643c55197aab28d", fields.definitionHash());	

	FieldSet fields_copy(fields);

	CHECK_EQUAL(fields_copy.definitionHash(), fields.definitionHash());	
}

TEST(FieldSetCreationFromString)
{
	sql::string strdef;
	strdef += "_ID INTEGER PRIMARY KEY, ";
	strdef += "name TEXT NOT NULL, ";
	strdef += "valueInt INTEGER, ";
	strdef += "valueDbl REAL, ";
	strdef += "valueTxt TEXT, ";
	strdef += "valueBol INTEGER NOT NULL, ";
	strdef += "valueTme INTEGER";

	FieldSet* fields = FieldSet::createFromDefinition(strdef);

	CHECK(fields != NULL);

	if (fields)
	{
		CHECK_EQUAL(strdef, fields->getDefinition());
		CHECK_EQUAL("63f1ae61aad81ca9e98eb7c9c643c55197aab28d", fields->definitionHash());
		delete fields;
	}
}

TEST(FieldSetFieldsOrder)
{
	FieldSet fields(definition());

	CHECK_EQUAL(7, fields.count());

	//check if fields by name and by index are the same
	CHECK_EQUAL(fields.getByName("_ID")->getName(), fields.getByIndex(0)->getName());
	CHECK_EQUAL(fields.getByName("name")->getName(), fields.getByIndex(1)->getName());
	CHECK_EQUAL(fields.getByName("valueInt")->getName(), fields.getByIndex(2)->getName());
	CHECK_EQUAL(fields.getByName("valueDbl")->getName(), fields.getByIndex(3)->getName());
	CHECK_EQUAL(fields.getByName("valueTxt")->getName(), fields.getByIndex(4)->getName());
	CHECK_EQUAL(fields.getByName("valueBol")->getName(), fields.getByIndex(5)->getName());
	CHECK_EQUAL(fields.getByName("valueTme")->getName(), fields.getByIndex(6)->getName());
}

//eofn
};
