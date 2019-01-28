/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_Decimal128.h (SYS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SYS_DECIMAL128_H_INCLUDED__
#define __SYS_DECIMAL128_H_INCLUDED__

#include "SYS_API.h"
#ifndef __SYS_Types__
#error This file must be included from SYS_Types.h
#endif

#include <iosfwd>


/// Decimal128 floating point class (IEEE 754-2008)
class SYS_API SYS_Decimal128
{
public:
    /// Default constructor, leaves value uninitialized
			SYS_Decimal128();

    explicit		SYS_Decimal128(int32 v);
    explicit		SYS_Decimal128(uint32 v);
    explicit		SYS_Decimal128(fpreal32 v);
    explicit		SYS_Decimal128(fpreal64 v);
    explicit		SYS_Decimal128(const char *str);

    /// Conversion to other type
    /// @{
    operator		int32() const;
    operator		uint32() const;
    operator		fpreal32() const;
    operator		fpreal64() const;
    /// @}

    /// Round to nearest integer, 0.6 rounds up
    int32		roundedInt() const;

    /// Unary minus
    SYS_Decimal128	operator-() const;

    /// Assignment
    /// @{
    SYS_Decimal128&	operator=(int32 v);
    SYS_Decimal128&	operator=(uint32 v);
    SYS_Decimal128&	operator=(fpreal32 v);
    SYS_Decimal128&	operator=(fpreal64 v);
    /// @}

    /// Arithmetic Assignment
    /// @{
    SYS_Decimal128&	operator+=(const SYS_Decimal128 &v);
    SYS_Decimal128&	operator-=(const SYS_Decimal128 &v);
    SYS_Decimal128&	operator*=(const SYS_Decimal128 &v);
    SYS_Decimal128&	operator/=(const SYS_Decimal128 &v);
    /// @}

    /// Set to zero
    void		zero();

    /// Classification
    /// @{
    bool		isCanonical() const;
    bool		isFinite() const;
    bool		isInfinite() const;
    bool		isInteger() const;
    bool		isNan() const;
    bool		isNegative() const;
    bool		isNormal() const;
    bool		isPositive() const;
    bool		isZero() const;
    /// @}

    /// Determine number of significant digits
    uint		numDigits() const;

    /// Comparison Operators
    /// @{
    bool		operator==(const SYS_Decimal128 &v) const;
    bool		operator!=(const SYS_Decimal128 &v) const
			    { return !(*this == v); }
    bool		operator<(const SYS_Decimal128 &v) const;
    bool		operator<=(const SYS_Decimal128 &v) const;
    bool		operator>(const SYS_Decimal128 &v) const;
    bool		operator>=(const SYS_Decimal128 &v) const;
    /// @}

    /// Maximum string length+1
    static const int	NUMBUF = 43;

    /// Conversion to string, buffer must contain at least NUMBUF bytes
    void		toString(char *str) const;

    /// Conversion from string
    void		fromString(const char *str);

private:
    uint8		myValue[16];
};

/// Binary arithmetic operators
/// @{
SYS_API SYS_Decimal128	operator+(const SYS_Decimal128 &x,
				  const SYS_Decimal128 &y);
SYS_API SYS_Decimal128	operator-(const SYS_Decimal128 &x,
				  const SYS_Decimal128 &y);
SYS_API SYS_Decimal128	operator*(const SYS_Decimal128 &x,
				  const SYS_Decimal128 &y);
SYS_API SYS_Decimal128	operator/(const SYS_Decimal128 &x,
				  const SYS_Decimal128 &y);
/// @}

/// Stream I/O
/// @{
SYS_API std::ostream&	operator<<(std::ostream &os, const SYS_Decimal128 &v);
SYS_API std::istream&	operator>>(std::istream &is, SYS_Decimal128 &v);
/// @}

/// String formatting
/// @{
SYS_API size_t format(char *buffer, size_t buffer_size, const SYS_Decimal128 &v);
/// @}

#endif // __SYS_DECIMAL128_H_INCLUDED__
