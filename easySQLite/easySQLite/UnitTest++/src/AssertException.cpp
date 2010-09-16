#include "AssertException.h"
#include <cstring>

namespace UnitTest {

//CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

AssertException::AssertException(char const* description, char const* filename, int lineNumber)
    : m_lineNumber(lineNumber)
{
	using namespace std;

    strcpy(m_description, description);
    strcpy(m_filename, filename);
}

#pragma warning(default : 4996)

AssertException::~AssertException() throw()
{
}

char const* AssertException::what() const throw()
{
    return m_description;
}

char const* AssertException::Filename() const
{
    return m_filename;
}

int AssertException::LineNumber() const
{
    return m_lineNumber;
}

}
