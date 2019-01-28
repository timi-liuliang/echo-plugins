/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CVEX_Transform.h (CVEX Library, C++)
 *
 * COMMENTS:
 */

#ifndef __CVEX_Transform__
#define __CVEX_Transform__

#include "CVEX_API.h"
#include <UT/UT_Matrix4.h>
#include <UT/UT_Array.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_OpCaller.h>
#include <VEX/VEX_PodTypes.h>

class VEX_TransformContext;
class UT_OpCaller;
class UT_StringHolder;
class cvex_TransformData;

/// Each VEX shader can have a transform bound to it.  This transform is used
/// by the ptransform(), ntransform(), etc. functions to look up "world" or
/// "object" space.
///
/// Variables in the shader are defined to be in "current" space.  The CVEX
/// context allows you to specify various transforms from the current space to
/// another space.  The other spaces are defined by the built-in VEX spaces:
///	- "space:world": Transform current space to "world" space
///	- "space:camera": Transform current to "camera" space
///	- "space:object": Transform current to "object" space
///	- "space:light":  Transform current to "light" space
///
/// It's also possible to specify a "lookup" helper class which can be used to
/// look up user defined spaces.
class CVEX_API CVEX_Transform
{
public:
     CVEX_Transform();
    ~CVEX_Transform();

    /// Resets the transform so it can be re-used by different
    /// contexts.
    void clear();

    /// Class that passes information about context of transform lookup
    /// The @c cwd is the current node id (i.e. very likely the node associated
    /// with the OpCaller.  The @c worldId is the node id associated with the
    /// current transform.  That is, the object containing the node.  If the
    /// @c worldId isn't defined, the @c cwd will be used.
    class CVEX_API LookupContext
    {
    public:
	 LookupContext();
	~LookupContext();

	void 		 clear();

	UT_OpCaller	*opCaller() const	{ return myOpCaller; }
	fpreal		 time() const		{ return myTime; }
	/// Specifies the node for relative path searching
	int		 cwdId() const	
			 {
			     if (myCwdId < 0 && myOpCaller)
				 return myOpCaller->getOpId();
			     return myCwdId;
			 }
	/// Specifies the node that defines the transform space
	int		 worldId() const
			 {
			     if (myWorldId < 0)
			     {
				 // We want to give precedence to the cwd
				 // rather than the caller.
				 return cwdId();
			     }
			     return myWorldId;
			 }

	void		setOpCaller(UT_OpCaller *c)	{ myOpCaller = c; }
	void		setTime(fpreal t)		{ myTime = t; }
	void		setCwdId(int id)		{ myCwdId = id; }
	void		setWorldId(int id)		{ myWorldId = id; }

	void		setInputXform(int input, const UT_Matrix4D &xform);
	void		getInputXform(int input, UT_Matrix4D &xform) const;
	void		setInputXformId(int input, int nodeid);
	int 		getInputXformId(int input) const;

	/// The user data is set by the Lookup class (if set).  This allows the
	/// lookup to keep per-instantiation data.
	void		*getUserData() const	{ return myUserData; }

    private:
	void		*myUserData;
	UT_OpCaller	*myOpCaller;
	fpreal		 myTime;
	int		 myCwdId;
	int		 myWorldId;
	UT_Array<UT_Matrix4D>	myInputXform;
	UT_IntArray	 myInputXformId;
    };

    /// Class to look-up user defined spaces
    class CVEX_API Lookup
    {
    public:
	Lookup() {}
	virtual ~Lookup();

	/// Allocate user data for the lookup context
	virtual void	*allocateUserData() const;
	/// Delete the user data that was allocated for the lookup context
	virtual void	 freeUserData(void *) const;
	/// Reset the user data so it can be re-used later
	virtual void	 clearUserData(void *) const;

	/// Look up a matrix which will transform the "current" space into the
	/// named space.
	virtual bool	lookup(const char *name, UT_Matrix4D &m,
				const LookupContext &context) = 0;

	/// Look up a matrix which transforms the named space into "current"
	/// space.  By default, this will call @c lookup() and compute the
	/// inverse.  If you have the inverse available, you may want to make
	/// this more efficient by using it.
	virtual bool	lookupInverse(const char *name, UT_Matrix4D &m,
				const LookupContext &context);

	/// Lookup NDC space given a name
	virtual bool	lookupToNDC(const char *name,
				UT_Matrix4D &m,
				const LookupContext &context,
				bool &ortho) const;
	virtual bool	lookupFromNDC(const char *name,
				UT_Matrix4D &m,
				const LookupContext &context,
				bool &ortho) const;

	/// Path functions
	virtual bool	abspath(UT_StringHolder &result,
				const UT_StringHolder &relpath,
				const LookupContext &context) const;
	virtual int	idFromPath(const UT_StringHolder &relpath,
				const LookupContext &context) const;
	virtual bool	pathFromId(UT_StringHolder &result, int id,
				const LookupContext &context) const;
    };

    /// Set the lookup object for arbitrary spaces.
    static void	setLookup(const UT_SharedPtr<Lookup> &lookup);

    /// @{
    /// Get information about the context
    UT_OpCaller	*opCaller() const	{ return myContext.opCaller(); }
    fpreal	 time() const		{ return myContext.time(); }
    int		 cwdId() const		{ return myContext.cwdId(); }
    int		 worldId() const	{ return myContext.worldId(); }
    void	 getInputXform(int input, UT_Matrix4D &xform) const { myContext.getInputXform(input, xform); }
    int 	 getInputXformId(int input) const { return myContext.getInputXformId(input); }
    /// @}

    /// @{
    /// Set information about the context
    void	setOpCaller(UT_OpCaller *c)	{ myContext.setOpCaller(c); }
    void	setTime(fpreal t)		{ myContext.setTime(t); }
    void	setCwdId(int id)		{ myContext.setCwdId(id); }
    void	setWorldId(int id)		{ myContext.setWorldId(id); }
    void	setInputXform(int id, const UT_Matrix4D &xform) { myContext.setInputXform(id, xform); }
    void	setInputXformId(int id, int nodeid) { myContext.setInputXformId(id, nodeid); }
    /// @}

private:
    // Note:  The LookupContext must remain in scope during the lifetime of
    // this object.
    VEX_TransformContext	*context() const;

    cvex_TransformData	*myData;
    LookupContext	 myContext;

    friend class	CVEX_Context;
};

#endif
