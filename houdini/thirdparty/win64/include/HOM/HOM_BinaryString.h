#ifndef __HOM_BinaryString_h__
#define __HOM_BinaryString_h__

#ifdef SWIG
class HOM_BinaryString;

%typemaps_std_string(HOM_BinaryString, char, SWIG_AsCharPtrAndSize, SWIG_FromBinaryCharPtrAndSize, %checkcode(STDSTRING));
%traits_ptypen(HOM_BinaryString);
#else
#include <string>

class HOM_BinaryString : public std::string
{
    public:
        HOM_BinaryString()
            : std::string() {}

        HOM_BinaryString(const value_type* __s, size_type __n,
                const allocator_type& __a = allocator_type())
            : std::string(__s, __n, __a) {}

	HOM_BinaryString(
		const value_type* __s,
		const allocator_type& __a = allocator_type())
            : std::string(__s, __a) {}

        HOM_BinaryString(
		size_type __n, value_type __c,
		const allocator_type& __a = allocator_type())
            : std::string(__n, __c, __a) {}

	HOM_BinaryString(const std::string &s)
	    : std::string(s) {}

        template<class _InputIterator>
        HOM_BinaryString(_InputIterator __beg, _InputIterator __end,
                const allocator_type& __a = allocator_type())
        : std::string(__beg, __end, __a) {}
};
#endif

#endif
