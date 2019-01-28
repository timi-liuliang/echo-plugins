/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_PrimGroup_h__
#define __HOM_PrimGroup_h__

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_EnumModules.h"
#include "HOM_Prim.h"
#include "HOM_PtrOrNull.h"
#include <vector>
class HOM_Geometry;
class GA_AttributeOptions;

SWIGOUT(%rename(PrimGroup) HOM_PrimGroup;)

class HOM_API HOM_PrimGroup
{
public:
    HOM_PrimGroup()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_PrimGroup(const HOM_PrimGroup &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_PrimGroup()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_PrimGroup> prim_group) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_PrimGroup> prim_group) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject geometry;)
    virtual HOM_Geometry *geometry() = 0;

    virtual std::string name() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Prim> > prims() = 0;

    virtual bool contains(HOM_Prim &prim) = 0;

    virtual bool isOrdered() = 0;

    virtual void add(HOM_Prim &prim) = 0;

    virtual void add(const std::vector<HOM_Prim *> &prims) = 0;

    virtual void add(HOM_PrimGroup &prim_group) = 0;

    virtual void remove(HOM_Prim &prim) = 0;

    virtual void remove(const std::vector<HOM_Prim *> &prims) = 0;

    virtual void remove(HOM_PrimGroup &prim_group) = 0;

    virtual void clear() = 0;

    virtual void destroy() = 0;

#ifndef SWIG
    // Helper class necessary for getRawOptions[RO\RW]() calls to manage locks
    // that must be held for the duration of the reference to the UT_Options
    // object.
    class HOM_API OptionAutoLocks
    {
    public:
	class HOM_API ImplBase
	{
	    public:
		virtual ~ImplBase() {}
	    protected:
		ImplBase() {}
	};

	 OptionAutoLocks() : myImpl(0) {}
	~OptionAutoLocks() { delete myImpl; }

	 void		setImpl(ImplBase *impl) { myImpl = impl; }
    private:
	ImplBase	*myImpl;
    };
#endif

    // These methods return the UT_Options object associated with the attribute
    // and are not wrapped by swig.  The lock argument is present to ensure the
    // caller holds the locks necessary to work with a reference to an object
    // owned by the attribute.
    SWIGOUT(%ignore getRawOptionsRO;)
    virtual const GA_AttributeOptions &getRawOptionsRO(OptionAutoLocks &locks) = 0;
    SWIGOUT(%ignore getRawOptionsRW;)
    virtual GA_AttributeOptions &getRawOptionsRW(OptionAutoLocks &locks) = 0;

    // Get the data type of a particular option.
    // TODO: We could implement here, but we need HOMFoptionTypeToEnumValue().
    virtual HOM_EnumValue &optionType(const char *option_name) = 0;

    // Set an option value.  The type_hint is used when the value type mapping
    // to a UT_OptionType is ambiguous.
    virtual void setOption(const char *name, HOM_UTOptionAny value,
			   HOM_EnumValue &type_hint =HOM_fieldType::NoSuchField) = 0;

    // Remove an option from the attribute's UT_Options object.
    virtual void removeOption(const char *name) = 0;

#ifdef SWIG
%extend
{
    InterpreterObject options()
    {
	HOM_PrimGroup::OptionAutoLocks	 locks;
	const GA_AttributeOptions &attriboptions = self->getRawOptionsRO(locks);
	UT_Options	options;
	attriboptions.buildFullOptions(options);
	return HOMoptionsToInterpreterObject(options);
    }

    InterpreterObject option(const char *option_name)
    {
	HOM_PrimGroup::OptionAutoLocks	 locks;
	const GA_AttributeOptions &attriboptions = self->getRawOptionsRO(locks);
	UT_Options	options;
	attriboptions.buildFullOptions(options);
	const UT_OptionEntry *entry = options.getOptionEntry(option_name);
	return entry ? HOMoptionEntryToInterpreterObject(*entry)
		     : SWIG_Py_Void();
    }
}
#endif
};

#endif
