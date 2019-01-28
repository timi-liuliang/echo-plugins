/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        UT_DeepString.h
 *
 * COMMENTS:
 *	A UT_DeepString is a UT_String that always stores a deep copy its
 *	data.  This way you can return strings by value and you can have string
 *	member variables without having to define your own copy constructor and
 *	assignment operator.
 *
 *	Note that a UT_DeepString is just a UT_String that always stores
 *	a deep copy.  It's just a convenience class that avoids you having
 *	to pass UT_String::ALWAYS_DEEP to the constructor of the UT_String.
 *
 *	You should use UT_String with the UT_String::ALWAYS_DEEP parameter
 *	instead of this class whenever possible.  However, in templated
 *	code, it might not always be possible.
 */

#ifndef __UT_DeepString_h__
#define __UT_DeepString_h__

#include "UT_API.h"
#include "UT_String.h"

class UT_API UT_DeepString : public UT_String
{
public:
    UT_DeepString(const char *str = 0)
    : UT_String(UT_String::ALWAYS_DEEP, str)
    {}

    UT_DeepString(const std::string &str)
        : UT_String(UT_String::ALWAYS_DEEP, str.c_str())
    {}

    UT_DeepString(const UT_String &str)
    : UT_String(UT_String::ALWAYS_DEEP, (const char *)str)
    {}

    // When we copy construct a UT_String from a UT_DeepString, the UT_String
    // will always make a deep copy.
    UT_DeepString(const UT_DeepString &str)
    : UT_String(str)
    {}

    ~UT_DeepString()
    {}

    UT_DeepString &operator=(const UT_DeepString &str)
    {
	UT_String::operator=(str);
	return *this;
    }

    UT_DeepString &operator=(const UT_String &str)
    {
	UT_String::operator=(str);
	return *this;
    }

    UT_DeepString &operator=(const char *str)
    {
	UT_String::operator=(str);
	return *this;
    }

    bool operator==(const char *str) const
    { return UT_String::operator==(str); }
    bool operator==(const UT_String &str) const
    { return UT_String::operator==(str); }

    bool operator!=(const char *str) const
    { return UT_String::operator!=(str); }
    bool operator!=(const UT_String &str) const
    { return UT_String::operator!=(str); }

    bool operator<(const char *str) const
    { return UT_String::operator<(str); }
    bool operator<(const UT_String &str) const
    { return UT_String::operator<(str); }

    bool operator<=(const char *str) const
    { return UT_String::operator<=(str); }
    bool operator<=(const UT_String &str) const
    { return UT_String::operator<=(str); }

    bool operator>(const char *str) const
    { return UT_String::operator>(str); }
    bool operator>(const UT_String &str) const
    { return UT_String::operator>(str); }

    bool operator>=(const char *str) const
    { return UT_String::operator>=(str); }
    bool operator>=(const UT_String &str) const
    { return UT_String::operator>=(str); }
};

inline size_t hash_value(const UT_DeepString &str)
{
    return (size_t)str.hash();
}


#endif
