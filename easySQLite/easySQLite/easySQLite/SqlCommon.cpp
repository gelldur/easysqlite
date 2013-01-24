#include "SqlCommon.h"
#include <sstream>
#include <CommonCrypto/CommonCrypto.h>

namespace sql
{
time::time()
{
	if (::time(&_value) == -1)
		_value = -1;
}

time::time(const time& value)
{
	_value = value._value;
}

time::time(integer value)
{
	_value = value;
}

time& time::operator=(const time& value)
{
	if (this != &value)
	{
		this->_value = value._value;
	}
	return *this;
}

bool time::operator==(const time& value)
{
	if (this == &value)
		return true;

	if (value._value == value._value)
		return true;

	return false;
}

time time::now()
{
	time t;
	return t;
}

double time::diff(time& value)
{
	return difftime(this->_value, value._value);
}

integer time::asInteger()
{
	return _value;
}

time_t time::get()
{
	return _value;
}

string time::format(const char* format)
{
	string s;
	tm localtime;
	char buffer[128];
    
#ifdef WIN32
    if (localtime_s(&localtime, &_value) == 0)
		if (strftime(buffer, 128, format, &localtime) > 0)
			s = buffer;
#else
    localtime_r(&_value, &localtime);
    if (strftime(buffer, 128, format, &localtime) > 0)
        s = buffer;
#endif

	return s;
}

string time::asString()
{
	return format("%d-%m-%Y %H:%M, %a");
}

string time::asTimeString()
{
	return format("%H:%M");
}

string time::asDateString()
{
	return format("%d-%m-%Y");
}

void time::addValue(integer value)
{
	_value += value;
}

void time::addMinutes(integer count)
{
	_value += (60 * count);
}

void time::addHours(integer count)
{
	_value += (3600 * count);
}

void time::addDays(integer count)
{
	_value += ((3600 * 24) * count);
}


string intToStr(int value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

string intToStr(integer value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

string quoteStr(string value)
{
	string s;

	for (string::iterator it = value.begin(); it != value.end(); it++)
	{
		char c = *it;
		s += c;
		if (c == '\'')
			s += c;
	}

	return s;
}

//CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

string binToHex(const char* buffer, int size)
{
	std::string s;

	char digit[4];

	unsigned char* p = (unsigned char*)buffer;

	for (int index = 0; index < size; index++)
	{
		sprintf(digit, "%02x", *p++);
		s += digit;
	}

	return s;
}

#pragma warning(default : 4996)

string generateSHA(const std::string value)
{
#ifdef WIN32
	CSHA1 sha;
    
	sha.Update((UINT_8*)value.c_str(), value.length());
    
	sha.Final();
    
	UINT_8 digest[20];
	if (sha.GetHash(digest))
	{
		const int size = sizeof(digest) / sizeof(UINT_8);
		return binToHex((char*)digest, size);
	}

	return "";
#else
    const char *str = value.c_str();
    unsigned char r[CC_SHA1_DIGEST_LENGTH];
    CC_SHA1(str, static_cast<CC_LONG>(value.length()), r);
    
    std::stringstream ss;
    ss << r;
    return ss.str();
#endif
}

string& trimleft(string& s)
{
	string::iterator it;

	for( it = s.begin(); it != s.end(); it++ )
		if( !isspace( *it ) )
			break;

	s.erase( s.begin(), it );
	return s;
}

string& trimright(string& s)
{
	string::difference_type dt;
	string::reverse_iterator it;

	for( it = s.rbegin(); it != s.rend(); it++ )
		if( !isspace( *it ) )
			break;

	dt = s.rend() - it;

	s.erase( s.begin() + dt, s.end() );
	return s;
}

string& trim(string& s)
{
	trimleft( s );
	trimright( s );
	return s;
}

string trim(const string& s)
{
	string t = s;
	return trim(t);
}

//CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

void listToVector(string s, std::vector<string>& vector, const char* sep)
{
	vector.clear();

	char* buffer = new char[s.length() + 1];

	memcpy(buffer, s.c_str(), s.length());
	buffer[s.length()] = 0;

	char* token = strtok(buffer, sep);

	while (token)
	{
		string sToken = token;
		vector.push_back(trim(sToken));
		token = strtok(NULL, sep);
	}

	delete [] buffer;
}

#pragma warning(default : 4996)


//sql eof
};
