/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RAY_ProceduralFactory.h (RAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __RAY_ProceduralFactory__
#define __RAY_ProceduralFactory__

#include "RAY_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_Lock.h>

class VPRM_Stack;
class RAY_Procedural;
class RAY_ProceduralArg;

/// The procedural factory is used to store the definitions of all the
/// procedurals in mantra.  The factory is passed to the registerProcedural()
/// method on plugins.
class RAY_API RAY_ProceduralFactory
{
public:
     RAY_ProceduralFactory();
    ~RAY_ProceduralFactory();

    /// Class to create a procedural
    class RAY_API ProcDefinition
    {
    public:
	ProcDefinition(const UT_StringHolder &name);
	virtual ~ProcDefinition();

	/// Name of the procedural
	const UT_StringHolder	&name() const { return myName; }

	/// Query which DSO file was used to define this procedural
	const UT_StringHolder	&dsoname() const { return myDSOName; }

	/// Set the DSO file.  This is only required for old-school procedural
	/// definitions.
	void	setDSOName(const UT_StringHolder &dso) { myDSOName = dso; }

	/// Create a procedural, and pass ownership of the instance to mantra
	virtual RAY_Procedural			*create() const = 0;
	/// Provide a const reference to the arguments for the procedural
	virtual const RAY_ProceduralArg	*arguments() const = 0;

	/// @{
	/// @private
	/// Access to internals
	void		 processArguments();
	VPRM_Stack	&stack() const { return *myStack; }
	UT_Lock		&lock() const { return myLock; }
	/// @}

    private:
	mutable UT_Lock	 myLock;
	UT_StringHolder	 myName;
	UT_StringHolder	 myDSOName;
	VPRM_Stack	*myStack;
    };

    /// Initialize procedurals
    void	initialize();

    /// Add a new procedural to the factory.  The factory takes ownership of
    /// the definition.  Returns whether the procedural was stored.
    ///
    /// Setting HOUDINI_DSO_ERROR to 2 or greater will cause the procedurals to
    /// be printed out as they get added to mantra.
    bool	insert(ProcDefinition *def, bool replace_existing=true);

    /// Look up a procedural by name
    const ProcDefinition	*find(const UT_StringRef &name) const;

    /// Extract a list of procedural definitions
    void	getList(UT_Array<const ProcDefinition *> &list) const;

    /// Return number of procedurals
    size_t	size() const	{ return myProcs.size(); }
private:
    UT_StringMap<ProcDefinition *>	myProcs;
};

#endif
