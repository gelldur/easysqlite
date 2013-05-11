#include "../UnitTest++/src/UnitTest++.h"
#include "../easySQLite/SqlValue.h"


namespace Tests
{

using namespace sql;


TEST(ValueStore)
{
	Value v_udf;
	Value v_int("123", type_int);
	Value v_txt("text", type_text);
	Value v_dbl("0.5678", type_float);
	Value v_bol("1", type_bool);
	Value v_tme("0", type_time);

	CHECK(v_udf.isNull());
	CHECK_EQUAL(123, v_int.asInteger());
	CHECK_EQUAL("text", v_txt.asString());
	CHECK_EQUAL(0.5678, v_dbl.asDouble());
	CHECK_EQUAL(true, v_bol.asBool());
	CHECK(v_tme.asTime() == sql::time(0));
}

TEST(ValueCompare)
{
	Value v("test", type_text);

	Value v1(v);

	Value v2 = v;

	CHECK(v1.equals(v));
	CHECK(v2.equals(v));
}

TEST(ValueConvert)
{
	Value v("678", type_int);

	CHECK_EQUAL(false, v.asBool());
	CHECK_EQUAL(678, v.asDouble()); 
	CHECK_EQUAL(678, v.asInteger());
	CHECK_EQUAL("678", v.asString());
}


//eofn
};
