//
// Copyright (C) 2010 Piotr Zagawa
//
// Released under BSD License
//

#pragma once

#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <iostream>

namespace sql
{
//field state enums
//--------------------------

enum field_use
{
	FIELD_DEFAULT,
	FIELD_KEY,
	DEFINITION_END,
};


enum field_type
{
	type_undefined,
	type_int,
	type_text,
	type_float,
	type_bool,
	type_time,
};

	
enum field_flags
{
	flag_none = 0,
	flag_not_null = 1,
	flag_primary_key = 2,
};


//common data types
//--------------------------

//string
typedef std::string string;


//integer
#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef __int64 integer;
#else
typedef long long int integer;
#endif


//time
class time
{
private:
	time_t _value;

private:
	string format(const char* format);

public:
	time();
	time(const time& value);
	time(integer value);
	time& operator=(const time& value);
	bool operator==(const time& value);
	static time now();

public:
	double diff(time& value);
	time_t get();
	void addValue(integer value);
	void addMinutes(integer count);
	void addHours(integer count);
	void addDays(integer count);

public:
	integer asInteger();
	string asString();
	string asTimeString();
	string asDateString();

};


//common exception class
class Exception
{
private:
	string _msg;

public:
	Exception(string msg)
		: _msg(msg)
	{
	};
	string msg()
	{
		return _msg;
	}
};


//comment this directive to disable exceptions
#define USE_EXCEPTIONS


#ifndef THROW_EXCEPTION
	#ifdef USE_EXCEPTIONS
		#define THROW_EXCEPTION(msg) throw Exception(msg);
	#else
		#define THROW_EXCEPTION(msg)
	#endif
#endif


//utils
//--------------------------
class log
{
public:
	log(std::string s)
	{
        std::cout << s << std::endl;
	}
};


string intToStr(int value);
string intToStr(integer value);

string quoteStr(string value);

string binToHex(const char* buffer, int size);

string generateSHA(const std::string value);

string& trimleft(string& s);
string& trimright(string& s);
string& trim(string& s);
string trim(const string& s);

void listToVector(string s, std::vector<string>& vector, const char* sep = ",");


//sql eof
};
