// RSA public key encryption

#ifndef __UT_RSA__
#define __UT_RSA__

#include "UT_API.h"
#include "UT_VeryLong.h"

class UT_API UT_RSA_Public
{
public:
    UT_VeryLong	myM;		// My modulus
    UT_VeryLong	myE;		// My public exponent

    // Requires 0 <= plain < m
    UT_VeryLong	encrypt( const UT_VeryLong &plain );
    void	encrypt(unsigned char *dest, const unsigned char *src,
			unsigned &len);

    void	setE(const unsigned *e, int ewords);
    void	setM(const unsigned *m, int mwords);
    void	setE(UT_VeryLong &e)	{ myE = e; }
    void	setM(UT_VeryLong &m)	{ myM = m; }
};

class UT_API UT_RSA_Private : public UT_RSA_Public
{
public:
    UT_VeryLong	myD;		// My private key
    UT_VeryLong	decrypt(const UT_VeryLong &cipher);
    void	decrypt(unsigned char *dest, const unsigned char *src,
			unsigned &len);

    void	setPassword(const char *passwd);
    // r1 and r2 should be null terminated random strings
    // each of length around 35 characters (for a 500 bit modulus)
    void	create(const char *r1, const char *r2, const char *passwd);
    void	setD(const unsigned *d, int dwords);
    void	setD(UT_VeryLong &d)	{ myD = d; }

    // Print code containing modulus and public/private keys
    void	printCode();
};

#endif

