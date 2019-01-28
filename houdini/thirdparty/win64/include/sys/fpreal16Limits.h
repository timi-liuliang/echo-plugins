///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2002, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////


// Primary authors:
//     Florian Kainz <kainz@ilm.com>
//     Rod Bogart <rgb@ilm.com>


#ifndef __FPREAL16LIMITS_H_INCLUDED__
#define __FPREAL16LIMITS_H_INCLUDED__


//------------------------------------------------------------------------
//
//	C++ standard library-style numeric_limits for class fpreal16
//
//------------------------------------------------------------------------

#include "SYS_Types.h"
#include <limits>

namespace std {
	
template <>
class numeric_limits <fpreal16>
{
public:

    static const bool	is_specialized = true;

    static		fpreal16 min() noexcept {return H_REAL16_NRM_MIN;}
    static		fpreal16 max() noexcept {return H_REAL16_MAX;}

    static const int	digits = H_REAL16_MANT_DIG;
    static const int	digits10 = H_REAL16_DIG;
    static const bool	is_signed = true;
    static const bool	is_integer = false;
    static const bool	is_exact = false;
    static const int	radix = H_REAL16_RADIX;

    static fpreal16	epsilon() noexcept {return H_REAL16_EPSILON;}
    static fpreal16	round_error() noexcept {return H_REAL16_EPSILON / 2;}

    static const int	min_exponent = H_REAL16_MIN_EXP;
    static const int	min_exponent10 = H_REAL16_MIN_10_EXP;
    static const int	max_exponent = H_REAL16_MAX_EXP;
    static const int	max_exponent10 = H_REAL16_MAX_10_EXP;

    static const bool	has_infinity = true;
    static const bool	has_quiet_NaN = true;
    static const bool	has_signaling_NaN = true;
    static const float_denorm_style
			has_denorm = denorm_present;
    static const bool	has_denorm_loss = false;

    static fpreal16	infinity() noexcept {return fpreal16::posInf();}
    static fpreal16	quiet_NaN() noexcept {return fpreal16::qNan();}
    static fpreal16	signaling_NaN() noexcept {return fpreal16::sNan();}
    static fpreal16	denorm_min() noexcept {return H_REAL16_MIN;}

    static const bool	is_iec559 = false;
    static const bool	is_bounded = false;
    static const bool	is_modulo = false;

    static const bool	traps = true;
    static const bool	tinyness_before = false;
    static const float_round_style
			round_style = round_to_nearest;
};

} // namespace std

#endif // __FPREAL16LIMITS_H_INCLUDED__
