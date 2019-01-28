/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_SafeFloat.h (UT Library, C++)
 *
 * COMMENTS:    Functions for packing integers into "safe" float values,
 *              i.e. not denormal, infinite, or NaN; just normal values.
 *
 *              - Integers in the range
 *                abs(i) <= (2^24) = 16,777,216
 *                will be represented exactly by the floats of the same value.
 *              - Integers in the range
 *                16,777,216 < abs(i) < (2^24) + 104*(2^23) = 889,192,448
 *                will be represented by larger, non-infinite floats.
 *              - Integers in the range
 *                889,192,448 <= abs(i) < (2^24) + 230*(2^23) = 1,946,157,056
 *                will be represented by small, non-denormal floats.
 *              - Any other integers, abs(i) >= 1,946,157,056, will not be
 *                represented correctly.
 */

#ifndef __UT_SafeFloat__
#define __UT_SafeFloat__

#include <SYS/SYS_Types.h>
#include "UT_Assert.h"

static inline float
UTpackIntToSafeFloat(int input)
{
    SYS_FPRealUnionF output;
    // If -(2^24) <= input <= (2^24), just convert directly,
    // so that the float value will be the same as the int value.
    if (SYSabs(input) <= (1 << (output.MANTISSA_BITS+1)))
    {
        UT_ASSERT_P(int(float(input)) == input);
        return float(input);
    }

    // Else, things get more complicated.

    // Add in the sign bit.
    output.uval = 0;
    if (input < 0)
    {
        output.uval |= (1 << (output.MANTISSA_BITS+output.EXPONENT_BITS));
        input = -input;
    }

    // Subtract off 2^24.
    input -= (1 << (output.MANTISSA_BITS+1));

    // If the number is small enough, use large numbers first, so that
    // we'll have valid integers for the first while.
    const int zeroexp = (1 << (output.EXPONENT_BITS-1)) - 1; // 0 (excess 127) = 127
    const int infiniteexp = (1 << output.EXPONENT_BITS) - 1; // 255
    const int startexp = zeroexp + (output.MANTISSA_BITS+1); // 24 (excess 127) = 151
    const int explength = infiniteexp - startexp;            // 255-151 = 104
    const int length = explength*(1 << output.MANTISSA_BITS);// 104*(2^23)
    const int startencoded = (startexp << output.MANTISSA_BITS);

    if (input < length)
    {
        output.uval |= (startencoded + input);
        return output.fval;
    }

    // Subtract off length.
    input -= length;

    // Avoid denormal numbers by adding one to the exponent
    input += (1 << output.MANTISSA_BITS);

    // Check for overflowing back around to 1
    const int oneencoded = (zeroexp << output.MANTISSA_BITS);
    UT_ASSERT_P(input < oneencoded);
    // Clamp if went past maximum
    // (Even though this doesn't necessarily make sense, it may avoid a
    // crash if a user hits it, and we have an assert for it above.)
    if (input >= oneencoded)
        input = oneencoded - 1;

    output.uval |= input;
    return output.fval;
}

static inline int
UTunpackIntFromSafeFloat(float input)
{
    // Special case for zero, for simplicity
    if (input == 0)
        return 0;

    SYS_FPRealUnionF output;
    output.fval = input;

    bool negative = (output.uval >> (output.MANTISSA_BITS+output.EXPONENT_BITS));
    output.uval &= ~(1 << (output.MANTISSA_BITS+output.EXPONENT_BITS));

    const int zeroexp = (1 << (output.EXPONENT_BITS-1)) - 1; // 0 (excess 127) = 127
    const int infiniteexp = (1 << output.EXPONENT_BITS) - 1; // 255
    const int startexp = zeroexp + (output.MANTISSA_BITS+1); // 24 (excess 127) = 151
    const int explength = infiniteexp - startexp;            // 255-151 = 104
    const int length = explength*(1 << output.MANTISSA_BITS);// 104*(2^23)
    const int startencoded = (startexp << output.MANTISSA_BITS);
    const int oneencoded = (zeroexp << output.MANTISSA_BITS);

    // If we're in the int block, just convert to int
    if (output.uval >= oneencoded && output.uval <= startencoded)
    {
        UT_ASSERT_P(float(int(input)) == input);
        return int(input);
    }

    if (output.uval > startencoded)
    {
        output.uval -= startencoded;
        // Check for infinite/NaN numbers
        UT_ASSERT_P(output.uval < length);
        // Clamp to just below infinite if infinite or NaN
        if (output.uval >= length)
            output.uval = length-1;
        output.uval += (1 << (output.MANTISSA_BITS+1));
        return negative ? -output.ival : output.ival;
    }

    // Check for denormal numbers or overflowing back around to 1
    UT_ASSERT_P(output.uval >= (1 << output.MANTISSA_BITS) && output.uval < oneencoded);
    // Clamp if denormal or went past maximum
    // (Even though this doesn't necessarily make sense, it may avoid a
    // crash if a user hits it, and we have an assert for it above.)
    if (output.uval < (1 << output.MANTISSA_BITS))
        return 0;
    if (output.uval >= oneencoded)
        output.uval = oneencoded - 1;

    // Undo adjustments from UTpackIntFromSafeFloat in denormal case
    output.uval += length+((1 << (output.MANTISSA_BITS+1))-(1 << output.MANTISSA_BITS));

    if (negative)
        output.ival = -output.ival;

    return output.ival;
}

#endif
