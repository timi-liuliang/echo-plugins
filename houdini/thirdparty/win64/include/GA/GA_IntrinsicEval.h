/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_IntrinsicEval.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_IntrinsicEval__
#define __GA_IntrinsicEval__

#include "GA_API.h"
#include "GA_Types.h"	// For GA_LocalIntrinsic

class GA_IntrinsicDef;
class GA_IntrinsicManager;

/// Class used to keep track of inheritance of intrinsic attribute evaluation
class GA_API GA_IntrinsicEval
{
public:
    /// Evaluating a global intrinsic attribute
    GA_IntrinsicEval(const GA_IntrinsicManager &manager,
			GA_LocalIntrinsic local_handle);

    /// Destructor
    ~GA_IntrinsicEval();

    /// Return the user id for the given set of intrinsic attributes.  This
    /// method is used during evaluation of intrinsic attributes.  The
    /// IntrinsicDef object should have been initialized during the definition
    /// of the user attributes.  Example code might be: @code
    /// static GA_IntrinsicDef  theIntrinsicDef;
    /// 
    /// int
    /// MyClass::localIntrinsicTupleSize(const GA_IntrinsicEval &eval)
    /// {
    ///     switch (eval.getUserId(theIntrinsicDef))
    ///     {
    ///         case 0:
    ///             return INTRINSIC_ATTRIBUTE_0_TUPLE_SIZE;
    ///         case 1:
    ///             return INTRINSIC_ATTRIBUTE_1_TUPLE_SIZE;
    ///         default:
    ///             break;
    ///     }
    ///     return MyBaseClass::localIntrinsicTupleSize(eval);
    /// }
    /// @endcode
    /// @see GA_IntrinsicManager
    int		getUserId(const GA_IntrinsicDef &def) const;

    /// @private
    /// Return the raw handle
    int		handle() const	{ return myHandle; }

private:
    const GA_IntrinsicManager	&myManager;
    int				 myHandle;
};

#endif
