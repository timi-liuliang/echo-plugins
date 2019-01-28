/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_FormatImpl.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_FormatImpl__
#define __UT_FormatImpl__

#if !defined(__UT_Format__)
#error UT_FormatImpl.h should only be included by UT_Format.h
#endif

#include "UT_StackBuffer.h"

#include <SYS/SYS_FormatNumber.h>
#include <SYS/SYS_ParseNumber.h>
#include <SYS/SYS_String.h>

namespace UT { namespace Format {

template <typename T, unsigned N>
inline size_t
ArgValue::customArrayWrapper(
	char *buffer, size_t buffer_size, const void *value)
{
    const char* beg_str = "[";
    const char* end_str = "]";
    Writer writer(buffer, buffer_size);
    Formatter<> f;
    const T* ptr = static_cast<const T*>(value);
    size_t nb_needed = writer(beg_str, beg_str + 1);
    for (unsigned i = 0; i < N; ++i)
    {
	if (i < N - 1)
	    nb_needed += f.format(writer, "{}, ", {ptr[i]});
	else
	    nb_needed += f.format(writer, "{}",   {ptr[i]});
    }
    nb_needed += writer(end_str, end_str + 1);
    return nb_needed;
}

namespace Detail
{
    // To avoid having to include UT_Unicode.h. Rare enough that we don't
    // really care about a single function call.
    UT_API size_t formatCodePoint(
	    char *buffer, size_t buffer_size, utf32 cp);

    // Returns true if the FILE pointer represents a buffered stream.
    UT_API bool isBufferedFile(FILE *f);
}

template<typename W>
size_t
Formatter<W>::formatAdjustAndFill(
	W &writer,
	const char *str,
	size_t str_size,
	const FormatSpec &spec)
{
    str_size = std::min(str_size, spec.str_width);

    // Do we need to fill or adjust?
    if (str_size >= spec.field_width)
	return writer(str, str + str_size);

    size_t buf_size = spec.field_width;
    UT_StackBuffer<char> buf(buf_size);
    char *ptr = buf.array();

    switch (spec.align)
    {
    case FormatSpec::Left:
	std::copy(str, str + str_size, ptr);
	std::fill(ptr + str_size, ptr + buf_size, spec.fill);
	break;
    case FormatSpec::Center:
    {
	size_t left_size = (buf_size - str_size) / 2;
	std::fill(ptr, ptr + left_size, spec.fill);
	std::copy(str, str + str_size, ptr + left_size);
	std::fill(ptr + left_size + str_size, ptr + buf_size, spec.fill);
	break;
    }
    case FormatSpec::Number:
	if (str_size && (*str == '-' || *str == '+' || *str == ' '))
	{
	    *ptr++ = *str++;
	    buf_size--;
	    str_size--;
	}
	// no break
    case FormatSpec::Right:
    {
	size_t shift = buf_size - str_size;
	std::fill(ptr, ptr + shift, spec.fill);
	std::copy(str, str + str_size, ptr + shift);
	break;
    }
    }

    return writer(buf.array(), buf.array() + buf.size());
}

template<typename W>
size_t
Formatter<W>::formatCodePoint(
	W &writer,
	const FormatSpec &spec,
	const ArgValue &arg)
{
    int64 v = getIntValueFromArg(arg);

    // Clamp so we don't wrap values around for int64, etc.
    if (v < 0)
	v = 0;
    else if (v > std::numeric_limits<int>::max())
	v = std::numeric_limits<int>::max();

    size_t nb_needed = Detail::formatCodePoint(nullptr, 0, utf32(v));
    if (!nb_needed)
	return 0;

    UT_StackBuffer<char> buf(nb_needed);

    Detail::formatCodePoint(buf.array(), nb_needed, utf32(v));

    return formatAdjustAndFill(writer, buf.array(), nb_needed, spec);
}

template<typename W>
size_t
Formatter<W>::formatPercentage(
	W &writer,
	const FormatSpec &spec,
	const ArgValue &arg)
{
    SYS_FormatFlags flags = SYS_FormatFlags::None;

    if (spec.sign == FormatSpec::Plus || spec.sign == FormatSpec::Space)
	flags = flags | SYS_FormatFlags::AddPlus;
    
    if (spec.digit_grouping)
	flags = flags | SYS_FormatFlags::DigitGrouping;

    fpreal64 perc = getFloatValueFromArg(arg) * 100.0;

    size_t nb_needed = SYSformatFloat(
	    nullptr, 0, perc, spec.precision == -1 ? 2 : spec.precision,
	    SYS_FormatNotation::Fixed, flags);

    UT_StackBuffer<char> buf(nb_needed + 1);
    SYSformatFloat(buf.array(), nb_needed, perc,
	    spec.precision == -1 ? 2 : spec.precision,
	    SYS_FormatNotation::Fixed, flags);
    buf.array()[nb_needed] = '%';

    return formatAdjustAndFill(writer, buf.array(), nb_needed + 1, spec);
}


template<typename W>
size_t 
Formatter<W>::formatInteger(
	W &writer,
	const FormatSpec &spec,
	const ArgValue &arg)
{
    // Figure out the size we need.
    SYS_FormatFlags flags = SYS_FormatFlags::None;
    int base = 10;
    int min_digits = 0;
    
    switch(spec.type)
    {
    case 'B':
	flags = flags | SYS_FormatFlags::UpperCase;
	// no break
    case 'b':
	base = 2;
	break;
    case 'c':
	return formatCodePoint(writer, spec, arg);
    case 'd':
    case 'n':
	base = 10;
	break;
    case 'o':
	base = 8;
	break;
    case 'X':
	flags = flags | SYS_FormatFlags::UpperCase;
	// no break
    case 'x':
	base = 16;
	break;
    }

    if (spec.alt_form)
	flags = flags | SYS_FormatFlags::AddCPrefix;

    if (spec.sign == FormatSpec::Plus || spec.sign == FormatSpec::Space)
	flags = flags | SYS_FormatFlags::AddPlus;

    if (spec.digit_grouping)
	flags = flags | SYS_FormatFlags::DigitGrouping;
    
    size_t nb_needed = 0;

#define SIZE_CASE(TYPE, VAL) \
    case ArgValue::Type::TYPE: \
	nb_needed = SYSformatInteger(nullptr, 0, arg.VAL, base, min_digits, flags); \
	break;
    
    switch(arg.type())
    {
    SIZE_CASE(Int8, myI8)
    SIZE_CASE(UInt8, myU8)
    SIZE_CASE(Int16, myI16)
    SIZE_CASE(UInt16, myU16)
    SIZE_CASE(Int32, myI32)
    SIZE_CASE(UInt32, myU32)
    SIZE_CASE(Int64, myI64)
    SIZE_CASE(UInt64, myU64)
    default:
	UT_ASSERT_MSG(false, "Not an integer type.");
	break;
    }
#undef SIZE_CASE
    
    UT_StackBuffer<char> buf(nb_needed);
    
#define FMT_CASE(TYPE, VAL) \
    case ArgValue::Type::TYPE: \
	SYSformatInteger(buf, nb_needed, arg.VAL, base, min_digits, flags); \
	break;
    
    switch(arg.type())
    {
    FMT_CASE(Int8, myI8)
    FMT_CASE(UInt8, myU8)
    FMT_CASE(Int16, myI16)
    FMT_CASE(UInt16, myU16)
    FMT_CASE(Int32, myI32)
    FMT_CASE(UInt32, myU32)
    FMT_CASE(Int64, myI64)
    FMT_CASE(UInt64, myU64)
    default:
	break;
    }
#undef FMT_CASE

    if (nb_needed && spec.sign == FormatSpec::Space && buf.array()[0] == '+')
	buf.array()[0] = ' ';

    return formatAdjustAndFill(writer, buf.array(), nb_needed, spec);
}

template<typename W>
size_t 
Formatter<W>::formatFloat(
	W &writer,
	const FormatSpec &spec,
	const ArgValue &arg)
{
    SYS_FormatFlags flags = SYS_FormatFlags::None;
    SYS_FormatNotation notation = SYS_FormatNotation::Shortest;
    
    switch(spec.type)
    {
    case 'E':
	flags = flags | SYS_FormatFlags::UpperCase;
	// no break
    case 'e':
	notation = SYS_FormatNotation::Scientific;
	break;
    case 'F':
	flags = flags | SYS_FormatFlags::UpperCase;
	// no break
    case 'f':
	notation = SYS_FormatNotation::Fixed;
	break;
    case 'G':
	flags = flags | SYS_FormatFlags::UpperCase;
	// no break
    case 'g':
	notation = SYS_FormatNotation::Shortest;
	break;
    case 'X':
	flags = flags | SYS_FormatFlags::UpperCase;
	// no break
    case 'x':
	notation = SYS_FormatNotation::Hexadecimal;
	break;
    case '%':
	return formatPercentage(writer, spec, arg);
    }

    if (spec.sign == FormatSpec::Plus || spec.sign == FormatSpec::Space)
	flags = flags | SYS_FormatFlags::AddPlus;

    if (spec.digit_grouping)
	flags = flags | SYS_FormatFlags::DigitGrouping;
    
    size_t nb_needed = 0;

#define SIZE_CASE(TYPE, VAL, PREC) \
    case ArgValue::Type::TYPE: \
	nb_needed = SYSformatFloat(nullptr, 0, arg.VAL, \
		spec.precision == -1 ? PREC : spec.precision, notation, flags); \
	break;
    switch(arg.type())
    {
    SIZE_CASE(Float16, myF16, 4)
    SIZE_CASE(Float32, myF32, 6)
    SIZE_CASE(Float64, myF64, 8)
    default:
	UT_ASSERT_MSG(false, "Not a float type.");
	break;
    }
#undef SIZE_CASE

    UT_StackBuffer<char> buf(nb_needed);
    
#define SIZE_CASE(TYPE, VAL, PREC) \
    case ArgValue::Type::TYPE: \
	SYSformatFloat(buf, nb_needed, arg.VAL, \
		spec.precision == -1 ? PREC : spec.precision, notation, flags); \
	break;
    switch(arg.type())
    {
    SIZE_CASE(Float16, myF16, 4)
    SIZE_CASE(Float32, myF32, 6)
    SIZE_CASE(Float64, myF64, 8)
    default:
	break;
    }
#undef SIZE_CASE
    
    if (nb_needed && spec.sign == FormatSpec::Space && buf.array()[0] == '+')
	buf.array()[0] = ' ';

    return formatAdjustAndFill(writer, buf.array(), nb_needed, spec);
}

template<typename W>
size_t 
Formatter<W>::formatString(
	W &writer,
	const FormatSpec &spec,
	const ArgValue &arg)
{
    static const char null[] = "(null)";
    
    if (arg.type() == ArgValue::Type::SizedString)
    {
	if (arg.mySizedStr.myStr)
	    return formatAdjustAndFill(writer, arg.mySizedStr.myStr, arg.mySizedStr.mySize, spec);
    }
    else if (arg.type() == ArgValue::Type::ZeroString)
    {
	if (arg.myStr)
	    return formatAdjustAndFill(writer, arg.myStr, ::strlen(arg.myStr), spec);
    }
    else if (arg.type() == ArgValue::Type::CharString)
    {
	return formatAdjustAndFill(writer, &arg.myChar, 1, spec);
    }
    else
    {
	UT_ASSERT_MSG(false, "Not a string type");
    }

    return formatAdjustAndFill(writer, null, sizeof(null) - 1, spec);
}

template<typename W>
size_t 
Formatter<W>::formatPointer(
	W &writer,
	const FormatSpec &spec,
	const ArgValue &arg)
{
    UT_ASSERT_MSG(arg.type() == ArgValue::Type::Pointer, "Not a pointer type");
    
    uintptr_t val = reinterpret_cast<uintptr_t>(arg.myPtr);
    int min_digits = sizeof(intptr_t) * 2;
    SYS_FormatFlags flags = SYS_FormatFlags::AddCPrefix;
    
    size_t nb_needed = SYSformatInteger(nullptr, 0, val, 16, min_digits, flags);
    
    UT_StackBuffer<char> buf(nb_needed);
    
    SYSformatInteger(buf.array(), nb_needed, val, 16, min_digits, flags);
    
    return formatAdjustAndFill(writer, buf.array(), nb_needed, spec);
}

template<typename W>
size_t 
Formatter<W>::formatCustom(
	W &writer,
	const FormatSpec &spec,
	const ArgValue &arg)
{
    size_t nb_needed = arg.formatCustom(
	    nullptr, std::numeric_limits<size_t>::max());

    // Don't ask for more than the string width.
    nb_needed = std::min(nb_needed, spec.str_width);
    
    UT_StackBuffer<char> buf(nb_needed);

    arg.formatCustom(buf.array(), nb_needed);

    return formatAdjustAndFill(writer, buf.array(), nb_needed, spec);
}

template<typename W>
size_t 
Formatter<W>::formatArg(
	W &writer,
	const FormatSpec &spec,
	const ArgValue &arg)
{
    if (arg.isInteger())
	return formatInteger(writer, spec, arg);
    else if (arg.isFloat())
	return formatFloat(writer, spec, arg);
    else if (arg.isString())
	return formatString(writer, spec, arg);
    else if (arg.isPointer())
	return formatPointer(writer, spec, arg);
    else if (arg.isCustom())
	return formatCustom(writer, spec, arg);
    else
    {
	UT_ASSERT_MSG(false, "Unknown argument type.");
	return 0;
    }
}

template<typename W>
int64
Formatter<W>::getIntValueFromArg(const ArgValue &arg)
{
    int64 v = 0;
    switch(arg.type())
    {
    case ArgValue::Type::Int8: 		v = arg.myI8; break;
    case ArgValue::Type::UInt8: 	v = arg.myU8; break;
    case ArgValue::Type::Int16: 	v = arg.myI16; break;
    case ArgValue::Type::UInt16: 	v = arg.myU16; break;
    case ArgValue::Type::Int32: 	v = arg.myI32; break;
    case ArgValue::Type::UInt32: 	v = arg.myU32; break;
    case ArgValue::Type::Int64: 	v = arg.myI64; break;
    case ArgValue::Type::UInt64: 	v = int64(arg.myU64); break;
    case ArgValue::Type::Float16: 	v = int64(arg.myF16); break;
    case ArgValue::Type::Float32: 	v = int64(arg.myF32); break;
    case ArgValue::Type::Float64: 	v = int64(arg.myF64); break;
    default:
	// Don't care.
	break;
    }

    return v;
}

template<typename W>
fpreal64
Formatter<W>::getFloatValueFromArg(const ArgValue &arg)
{
    fpreal64 v = 0;
    switch(arg.type())
    {
    case ArgValue::Type::Int8: 		v = fpreal64(arg.myI8); break;
    case ArgValue::Type::UInt8: 	v = fpreal64(arg.myU8); break;
    case ArgValue::Type::Int16: 	v = fpreal64(arg.myI16); break;
    case ArgValue::Type::UInt16: 	v = fpreal64(arg.myU16); break;
    case ArgValue::Type::Int32: 	v = fpreal64(arg.myI32); break;
    case ArgValue::Type::UInt32: 	v = fpreal64(arg.myU32); break;
    case ArgValue::Type::Int64: 	v = fpreal64(arg.myI64); break;
    case ArgValue::Type::UInt64: 	v = fpreal64(arg.myU64); break;
    case ArgValue::Type::Float16: 	v = fpreal64(arg.myF16); break;
    case ArgValue::Type::Float32: 	v = fpreal64(arg.myF32); break;
    case ArgValue::Type::Float64: 	v = arg.myF64; break;
    default:
	// Don't care.
	break;
    }

    return v;
}


template<typename W>
const ArgValue &
Formatter<W>::getFormatArg(
	const char *&ptr,
	const ArgValue args[],
	size_t nb_args)
{
    static ArgValue empty_arg;

    size_t arg_index;

    if (SYSisdigit(*ptr))
    {
	if (myNextArgIndex > 0)
	{
	    if (myReportErrors)
		UT_ASSERT_MSG(false, "Can't switch from auto to manual indexing.");
	    return empty_arg;
	}

	const char *end = nullptr;
	uint64 num;
	SYSparseInteger(ptr, end, num, 10);
	arg_index = num;
	ptr = end;
	myNextArgIndex = -1;
    }
    else if (*ptr == '}' || *ptr == ':')
    {
	if (myNextArgIndex < 0)
	{
	    if (myReportErrors)
		UT_ASSERT_MSG(false, "Can't switch from manual to auto indexing.");
	    return empty_arg;
	}

	arg_index = myNextArgIndex++;
    }
    else
    {
	if (myReportErrors)
	    UT_ASSERT_MSG(SYSisdigit(*ptr) || *ptr == '}' || *ptr == ':',
		    "Invalid format string.");
	return empty_arg;
    }

    if (arg_index >= nb_args)
    {
	if (myReportErrors)
	    UT_ASSERT_MSG(false, "Argument index out of bounds.");
	return empty_arg;
    }

    return args[arg_index];
}


template<typename W>
const ArgValue &
Formatter<W>::getPrintfArg(
	const char *&ptr,
	const ArgValue args[],
	size_t nb_args)
{
    static ArgValue empty_arg;

    size_t arg_index;

    if (*ptr == '(')
    {
	if (!SYSisdigit(ptr[1]))
	{
	    if (myReportErrors)
		UT_ASSERT_MSG(SYSisdigit(ptr[1]), "Invalid format string.");
	    return empty_arg;
	}

	if (myNextArgIndex > 0)
	{
	    if (myReportErrors)
		UT_ASSERT_MSG(false, "Can't switch from auto to manual indexing.");
	    return empty_arg;
	}

	const char *end = nullptr;
	uint64 num;
	SYSparseInteger(ptr + 1, end, num, 10);

	if (*end != ')')
	{
	    if (myReportErrors)
		UT_ASSERT_MSG(*end == ')', "Invalid format string.");
	    return empty_arg;
	}

	arg_index = num;
	ptr = end + 1;
	myNextArgIndex = -1;
    }
    else
    {
	if (myNextArgIndex < 0)
	{
	    if (myReportErrors)
		UT_ASSERT_MSG(false, "Can't switch from manual to auto indexing.");
	    return empty_arg;
	}

	arg_index = myNextArgIndex++;
    }

    if (arg_index >= nb_args)
    {
	if (myReportErrors)
	    UT_ASSERT_MSG(false, "Argument index out of bounds.");
	return empty_arg;
    }

    return args[arg_index];
}


template<typename W>
bool
Formatter<W>::parseFormatSpec(
	const char *&ptr,
	const ArgValue &arg,
	const ArgValue args[],
	size_t nb_args,
	FormatSpec &spec)
{
    auto isAlignmentChar = [](char c) -> bool
	    { return c == '<' || c == '^' || c == '>' || c == '='; };

    // Numeric values are right-aligned by default.
    if (arg.isNumeric())
	spec.align = FormatSpec::Right;
    
    // Digit grouping can be globally overridden.
    spec.digit_grouping = myDigitGroupings;

    char c = *ptr++;

    if (c == '}')
	return true;

    if (c != ':')
    {
	if (myReportErrors)
	    UT_ASSERT_MSG(c != ':', "Invalid format character");
	return false;
    }

    // Parse fill+alignment
    c = *ptr;

    char align_char = '\0';
    char fill_char = '\0';

    // Check first if the next character is an alignment char, in that case
    // the first one is the fill char.
    if (c && isAlignmentChar(ptr[1]))
    {
	if (c == '{')
	{
	    if (myReportErrors)
		UT_ASSERT_MSG(c != '\0', "'{' is not a valid fill char.");
	    return false;
	}
	fill_char = c;
	align_char = ptr[1];
	ptr += 2;
    }
    else if (isAlignmentChar(c))
    {
	align_char = c;
	ptr++;
    }

    switch(align_char)
    {
    case '<':	spec.align = FormatSpec::Left; break;
    case '^':	spec.align = FormatSpec::Center; break;
    case '>':	spec.align = FormatSpec::Right; break;
    case '=':
	// If argument is not numeric, just leave it at the default.
	if (arg.isNumeric())
	    spec.align = FormatSpec::Number;
	break;
    }

    if (fill_char)
	spec.fill = fill_char;

    if (c == '}')
	return true;

    // Check sign.
    switch (*ptr)
    {
    case '+': spec.sign = FormatSpec::Plus; ptr++; break;
    case '-': spec.sign = FormatSpec::Minus; ptr++; break;
    case ' ': spec.sign = FormatSpec::Space; ptr++; break;
    }

    if (*ptr == '#')
    {
	spec.alt_form = true;
	ptr++;
    }

    if (*ptr == ',')
    {
	spec.digit_grouping = true;
	ptr++;
    }

    if (*ptr == '0')
    {
	spec.align = FormatSpec::Number;
	spec.fill = '0';
	ptr++;
    }

    if (SYSisdigit(*ptr))
    {
	const char *end = nullptr;
	SYSparseInteger(ptr, end, spec.field_width, 10);
	ptr = end;
    }
    else if (*ptr == '{')
    {
	ptr++;

	const ArgValue &warg = getFormatArg(ptr, args, nb_args);
	if (warg.isEmpty())
	    return false;

	if (*ptr++ != '}')
	{
	    if (myReportErrors)
		UT_ASSERT_MSG(c != '\0', "Missing closing '}' for width argument.");
	    return false;
	}

	spec.field_width = (int)std::max(int64(0), getIntValueFromArg(warg));
    }

    if (*ptr == '.')
    {
	ptr++;

	int64 precision;

	if (SYSisdigit(*ptr))
	{
	    const char *end = nullptr;
	    SYSparseInteger(ptr, end, precision, 10);
	    ptr = end;

	    if (arg.isFloat())
		spec.precision = precision;
	    else
		spec.str_width = precision;
	}
	else if (*ptr == '{')
	{
	    ptr++;

	    const ArgValue &parg = getFormatArg(ptr, args, nb_args);
	    if (parg.isEmpty())
		return false;

	    if (*ptr++ != '}')
	    {
		if (myReportErrors)
		    UT_ASSERT_MSG(c != '\0', "Missing closing '}' for precision argument.");
		return false;
	    }

	    precision = (int)std::max(int64(0), getIntValueFromArg(parg));

	    if (arg.isFloat())
		spec.precision = precision;
	    else
		spec.str_width = precision;
	}
    }

    if (*ptr && *ptr!= '}')
	spec.type = *ptr++;

    if (*ptr++ != '}')
    {
	if (myReportErrors)
	    UT_ASSERT_MSG(c != '\0', "Missing closing '}' in format string.");
	return false;
    }

    return true;
}


template<typename W>
bool
Formatter<W>::parsePrintfSpec(
	const char *&ptr,
	const ArgValue &arg,
	const ArgValue args[],
	size_t nb_args,
	FormatSpec &spec)
{
    // getPrintfArg should've gotten us past any positional argument indicator.
    spec.align = FormatSpec::Right;
    spec.digit_grouping = myDigitGroupings;

    while(*ptr)
    {
	if (*ptr == '-')
	{
	    spec.align = FormatSpec::Left;
	    ptr++;
	}
	else if (*ptr == '+' || *ptr == ' ')
	{
	    spec.sign = *ptr == '+' ? FormatSpec::Plus : FormatSpec::Space;
	    ptr++;
	}
	else if (*ptr == '#')
	{
	    spec.alt_form = true;
	    ptr++;
	}
	else if (*ptr == '\'')
	{
	    spec.digit_grouping = true;
	    ptr++;
	}
	else if (*ptr == '0')
	{
	    if (arg.isNumeric())
		spec.fill = '0';
	    ptr++;
	}
	else
	    break;
    }

    // Parse the width.
    if (SYSisdigit(*ptr))
    {
	const char *end = nullptr;
	SYSparseInteger(ptr, end, spec.field_width, 10);
	ptr = end;
    }
    else if (*ptr == '(')
    {
	const ArgValue &warg = getPrintfArg(ptr, args, nb_args);
	if (warg.isEmpty())
	    return false;

	spec.field_width = (int)std::max(int64(0), getIntValueFromArg(warg));
    }

    // Parse precision, if any.
    if (*ptr == '.')
    {
	ptr++;

	int64 precision;

	if (SYSisdigit(*ptr))
	{
	    const char *end = nullptr;
	    SYSparseInteger(ptr, end, precision, 10);
	    ptr = end;

	    if (arg.isFloat())
		spec.precision = precision;
	    else
		spec.str_width = precision;
	}
	else if (*ptr == '(')
	{
	    const ArgValue &parg = getPrintfArg(ptr, args, nb_args);
	    if (parg.isEmpty())
		return false;

	    precision = (int)std::max(int64(0), getIntValueFromArg(parg));

	    if (arg.isFloat())
		spec.precision = precision;
	    else
		spec.str_width = precision;
	}
    }

    // Formatting codes. Skip over the size specifiers.
    if (*ptr == 'j' || *ptr == 'z' || *ptr == 't' || *ptr == 'L' ||
	*ptr == 'w' /* MS extension */ )
	ptr++;
    else if (*ptr == 'l' || *ptr == 'h')
    {
	ptr++;
	if (ptr[-1] == ptr[0])
	    ++ptr;
    }
    else if (*ptr == 'I')
    {
	// MS extension
	ptr++;
	if (ptr[0] == '3' && ptr[1] == '2')
	    ptr += 2;
	else if (ptr[0] == '6' && ptr[1] == '4')
	    ptr += 2;
    }

    // Specifier
    if (*ptr == 'd' || *ptr == 'i')
    {
	spec.type = 'd';
	ptr++;
    }
    else if (::strchr("bceEfFgGopsuxX", *ptr) != nullptr)
    {
	spec.type = *ptr++;
    }
    else if (*ptr == 'a' || *ptr == 'A')
    {
	spec.type = *ptr == 'a' ? 'x' : 'X';
	ptr++;
    }
    else
    {
	if (myReportErrors)
	    UT_ASSERT_MSG(::strchr("aAbcdeEfFgGiopsuxX", *ptr) != nullptr,
		    "Unknown specifier character.");
	return false;
    }

    return true;
}

template<typename W>
size_t
Formatter<W>::format(W &writer, const char * format,
		     std::initializer_list<ArgValue> args)
{
    return this->format(writer, format, args.begin(), args.size());
}


template<typename W>
size_t 
Formatter<W>::format(W &writer, const char * format,
		     const ArgValue args[], size_t nb_args)
{
    if (!format || !*format)
	return 0;
    
    WriteTracker wt(writer);
    
    myNextArgIndex = 0;
    
    const char *ptr = format;
    
    while (*ptr)
    {
	char c = *ptr++;
	
	// Iterate until we have a curly brace.
	if (c != '{' && c != '}')
	    continue;
	
	// If it's either curly brace repeated, just write out up until the
	// first brace, skip over the second and continue.
	if (*ptr == c)
	{
	    if (!wt.write(format, ptr))
		return wt.written();
	    
	    format = ++ptr;
	    continue;
	}
	
	// Single close curly brace. That's an error.
	if (c == '}')
	{
	    if (myReportErrors)
		UT_ASSERT_MSG(false, "Unmatched '}' in format string.");
	    return wt.written();
	}
	
	// We have a formatting code. Write what we have so far.
	if (!wt.write(format, ptr - 1))
	    return wt.written();
	
	const ArgValue &arg = getFormatArg(ptr, args, nb_args);
	if (arg.isEmpty())
	    return wt.written();

	FormatSpec spec;
	if (!parseFormatSpec(ptr, arg, args, nb_args, spec))
	    return wt.written();
	
	wt.increment(formatArg(writer, spec, arg));
	
	format = ptr;
    }
    
    wt.write(format, ptr);
    
    return wt.written();
}

template<typename W>
size_t
Formatter<W>::printf(W &writer, const char * format,
		     std::initializer_list<ArgValue> args)
{
    return this->printf(writer, format, args.begin(), args.size());
}

template<typename W>
size_t
Formatter<W>::printf(W &writer, const char * format,
		     const ArgValue args[], size_t nb_args)
{
    if (!format || !*format)
	return 0;
    
    WriteTracker wt(writer);

    myNextArgIndex = 0;

    const char *ptr = format;

    while (*ptr)
    {
	char c = *ptr++;

	// Iterate until we have a %
	if (c != '%')
	    continue;

	// If it's a % repeated, then print it out and continue.
	if (*ptr == c)
	{
	    if (!wt.write(format, ptr))
		return wt.written();

	    format = ++ptr;
	    continue;
	}

	// We have a formatting code. Write what we have so far.
	if (!wt.write(format, ptr - 1))
	    return wt.written();

	const ArgValue &arg = getPrintfArg(ptr, args, nb_args);
	if (arg.isEmpty())
	    return wt.written();

	FormatSpec spec;
	if (!parsePrintfSpec(ptr, arg, args, nb_args, spec))
	    return wt.written();

	wt.increment(formatArg(writer, spec, arg));

	format = ptr;
    }

    wt.write(format, ptr);

    return wt.written();
}


} }

template<typename... Args>
size_t UTformat(FILE *file, const char *format, const Args& ...args)
{
    if (!file)
	return 0;
    
    using namespace UT::Format;

    FileWriter w(file);

    if (Detail::isBufferedFile(file))
    {
	Formatter<FileWriter> f;
	return f.format(w, format, {args...});
    }
    else
    {
	Writer bw;
	Formatter<Writer> f;

	size_t nb_size = f.format(bw, format, {args...});

	UT_StackBuffer<char> s(nb_size);
	bw.setBuffer(s.array(), nb_size);
	f.format(bw, format, {args...});

	return w(s.begin(), s.end());
    }
}

template<typename... Args>
size_t UTformat(const char *format, const Args& ...args)
{
    return UTformat(stdout, format, args...);
}


template<typename... Args>
size_t UTformat(std::ostream &os, const char *format, const Args& ...args)
{
    using namespace UT::Format;
    StreamWriter w(os);

    UT::Format::Formatter<StreamWriter> f;
    return f.format(w, format, {args...});
}

template<typename... Args>
size_t UTformat(char *buffer, size_t buffer_size, const char *format,
		const Args& ...args)
{
    using namespace UT::Format;
    if (!buffer)
	buffer_size = std::numeric_limits<size_t>::max();
    
    Writer w(buffer, buffer_size);

    UT::Format::Formatter<Writer> f;
    return f.format(w, format, {args...});
}

template<typename... Args>
size_t UTprintf(FILE *file, const char *format, const Args& ...args)
{
    if (!file)
	return 0;

    using namespace UT::Format;

    FileWriter w(file);

    if (Detail::isBufferedFile(file))
    {
	Formatter<FileWriter> f;
	return f.printf(w, format, {args...});
    }
    else
    {
	Writer bw;
	Formatter<Writer> f;

	size_t nb_size = f.printf(bw, format, {args...});

	UT_StackBuffer<char> s(nb_size);
	bw.setBuffer(s.array(), nb_size);
	f.printf(bw, format, {args...});

	return w(s.begin(), s.end());
    }
}

template<typename... Args>
size_t UTprintf(const char *format, const Args& ...args)
{
    return UTprintf(stdout, format, args...);
}


template<typename... Args>
size_t UTprintf(std::ostream &os, const char *format, const Args& ...args)
{
    using namespace UT::Format;
    StreamWriter w(os);

    UT::Format::Formatter<StreamWriter> f;
    return f.printf(w, format, {args...});
}

template<typename... Args>
size_t UTprintf(char *buffer, size_t buffer_size, const char *format,
		const Args& ...args)
{
    using namespace UT::Format;
    if (!buffer)
	buffer_size = std::numeric_limits<size_t>::max();

    Writer w(buffer, buffer_size);

    UT::Format::Formatter<Writer> f;
    return f.printf(w, format, {args...});
}


#endif // __UT_FormatImpl__
