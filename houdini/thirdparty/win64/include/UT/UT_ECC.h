/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ECC.h ( Elliptic Curve Cryptography UT Library, C++)
 *
 * COMMENTS:	Implementation of the NIST-163 ECC.
 */

#ifndef __UT_ECC__
#define __UT_ECC__

#include "UT_API.h"
#include "UT_VeryLong.h"
#include "UT_PolyField.h"

class UT_ECurve;
class UT_WorkBuffer;

class UT_API UT_ECCPrivateKey {
public:
    UT_ECCPrivateKey(const unsigned int *key, int nwords, const UT_ECurve *curve=0);
    UT_ECCPrivateKey(const UT_VeryLong &lval, const UT_ECurve *curve=0);

    //
    // Generate a random number given a seed
    //
    static void		genRandom(UT_VeryLong &rnd,
				const unsigned seed[], unsigned seedlen,
				const UT_ECurve *curve=0);

    //
    // Generate a random number given a bunch of random data.
    // The random data must be larger than that needed for this purpose,
    // false is returned if it fails.
    static bool		genRandomRaw(UT_VeryLong &rnd,
				const unsigned seed[], unsigned seedlen,
				const UT_ECurve *curve=0);

    //
    // Given a random number (within the field modulus) and a message
    // encode the message into 'e'.  If the encoding failed, then please choose
    // a different random number.
    //
    // WARNING:  The message must be fewer than 160 bits (or so).
    //
    void		genPubXY(UT_VeryLong &codea,
				 const UT_VeryLong &rnd) const ;
    int			encode(UT_VeryLong &codea, UT_VeryLong &codeb,
			       const UT_VeryLong &rnd,
			       const UT_VeryLong &message) const;
    int			encode(UT_VeryLong &codeb,
				const unsigned seed[], unsigned seedlen,
				const UT_VeryLong &message) const;
    UT_VeryLong		 myKey;
    const UT_ECurve	*myCurve;
};

class UT_API UT_ECCPublicKey {
public:
    UT_ECCPublicKey(const UT_ECCPrivateKey &pkey);
    UT_ECCPublicKey(const unsigned int *x, int nxwords,
		    const unsigned int *y, int nywords,
		    const UT_ECurve *curve = 0);
    UT_ECCPublicKey(const UT_VeryLong &x, const UT_VeryLong &y,
		    const UT_ECurve *curve = 0);

    static void		printCode(UT_WorkBuffer &buf, const UT_VeryLong &codea,
						const UT_VeryLong &codeb);
    static int		scanCode(const char *buffer, UT_VeryLong &codea,
						     UT_VeryLong &codeb);

    int			decode(const UT_VeryLong &message,
			       const UT_VeryLong &codea,
			       const UT_VeryLong &codeb) const;

    UT_PolyField		 myX, myY;
    const UT_ECurve		*myCurve;
};

#endif
