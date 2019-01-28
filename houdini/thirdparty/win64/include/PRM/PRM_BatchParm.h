/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_BatchParm.h ( PRM Library, C++)
 *
 * COMMENTS:	Batch evaluation of parameters.  This class is the definition
 *		of a parameter.
 */

#ifndef __PRM_BatchParm__
#define __PRM_BatchParm__

#include "PRM_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_String.h>

typedef enum {
    PRM_BATCH_INT,
    PRM_BATCH_REAL,
    PRM_BATCH_STRING,
    PRM_BATCH_RAWSTRING,
} PRM_BatchType;

#define PRM_BATCH_CACHE_SIZE	4

class PRM_API PRM_BatchParm {
public:
     PRM_BatchParm();
    ~PRM_BatchParm();

    bool		 init(const char *name, PRM_BatchType type,
				int vsize, void *userdata);

    const char		*getName() const	{ return myName; }
    PRM_BatchType	 getType() const	{ return myType; }
    int			 getVectorSize() const	{ return myVectorSize; }
    bool		 isEvaluated() const	{ return myEvaluated; }

    int			 getEvaluatingOp() const	{ return myOpId; }
    int			 getEvaluatingParmIndex() const	{ return myParmIndex; }

    void	 clearEvaluated()	{ myEvaluated = false; }
    void	 clearValueSet()	{ myValueSet = false; }

    // Determine whether we can set the value of the parm into the actual OP
    // node's parameter.
    bool	 isValueSet() const	{ return myValueSet && myEvaluated; }

    int		*getInt()			{ return myVector.myInt; }
    fpreal	*getReal()			{ return myVector.myReal; }
    UT_String	*getString()			{ return myVector.myString; }

    void	*getUserData()			{ return myUserData; }

    int		 getDefaultedInt(int def, int idx=0)
		    { return myEvaluated ? myVector.myInt[idx] : def; }
    fpreal	 getDefaultedReal(fpreal def, int idx=0)
		    { return myEvaluated ? myVector.myReal[idx] : def; }
    const char	*getDefaultedString(const char *def, int idx=0)
		    {
			if (myEvaluated)
			    def = (const char *)myVector.myString[idx];
			return def;
		    }

    void	 setValue(int32 *val)
		 {
		     myValueSet = true;
		     for (int i = 0; i < myVectorSize; i++)
			 myVector.myInt[i] = (int)val[i];
		 }
    void	 setValue(int64 *val)
		 {
		     myValueSet = true;
		     for (int i = 0; i < myVectorSize; i++)
			 myVector.myInt[i] = (int)val[i];
		 }
    void	 setValue(fpreal32 *val)
		 {
		     myValueSet = true;
		     for (int i = 0; i < myVectorSize; i++)
			 myVector.myReal[i] = (fpreal)val[i];
		 }
    void	 setValue(fpreal64 *val)
		 {
		     myValueSet = true;
		     for (int i = 0; i < myVectorSize; i++)
			 myVector.myReal[i] = (fpreal)val[i];
		 }
    void	 setValue(const char **val)
		 {
		     myValueSet = true;
		     for (int i = 0; i < myVectorSize; i++)
			 myVector.myString[i].harden(val[i]);
		 }

    void	 dump();

private:
    void	 freeVector();
    void	 setEvaluated(int opid, int pidx)
		 {
		     myEvaluated = true;
		     myOpId = opid;
		     myParmIndex = pidx;
		 }

    // Rather than allocating memory for small vectors, we just create a cache.
    union {
	fpreal		 myReal[PRM_BATCH_CACHE_SIZE];
	int		 myInt[PRM_BATCH_CACHE_SIZE];
    } myCache;
    union {
	fpreal		*myReal;
	int		*myInt;
	UT_String	*myString;
    } myVector;
    const char		*myName;
    void		*myUserData;
    PRM_BatchType	 myType;
    int			 myVectorSize;
    int			 myOpId, myParmIndex;
    bool		 myEvaluated;
    bool		 myValueSet;

    friend class	 PRM_BatchList;
};

#endif
