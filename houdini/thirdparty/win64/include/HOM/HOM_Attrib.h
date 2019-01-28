/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Attrib_h__
#define __HOM_Attrib_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_EnumModules.h"
#include "HOM_IndexPairPropertyTable.h"
#include "HOM_IterableList.h"
#include "HOM_PtrOrNull.h"
#include <string>
class HOM_Geometry;
class GA_AttributeOptions;

SWIGOUT(%rename(Attrib) HOM_Attrib;)

class HOM_API HOM_Attrib
{
public:
    HOM_Attrib()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Attrib(const HOM_Attrib &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Attrib()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_Attrib> attrib) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_Attrib> attrib) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    virtual void destroy() = 0;

    SWIGOUT(%newobject geometry;)
    virtual HOM_Geometry *geometry() = 0;

    virtual std::string name() = 0;
    virtual HOM_EnumValue &dataType() = 0;
    virtual HOM_EnumValue &type() = 0;
    virtual bool isArrayType() = 0;
    virtual std::string qualifier() = 0;

    virtual int size() = 0;
    virtual void setSize(int size) = 0;
    virtual bool isTransformedAsVector() = 0;
    virtual bool isTransformedAsNormal() = 0;

    // This methods return hboost::any objects and are not wrapped directly
    // by swig.
    SWIGOUT(%ignore boostAnyAttribValueAt;)
    virtual hboost::any boostAnyDefaultValue() = 0;

    virtual std::vector<std::string> strings() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_IndexPairPropertyTable> > indexPairPropertyTables() = 0;

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
    InterpreterObject defaultValue()
    {
	return HOMboostAnyToInterpreterObject(self->boostAnyDefaultValue());
    }

    InterpreterObject options()
    {
	HOM_Attrib::OptionAutoLocks	 locks;
	const GA_AttributeOptions &attriboptions = self->getRawOptionsRO(locks);
	UT_Options	options;
	attriboptions.buildFullOptions(options);
	return HOMoptionsToInterpreterObject(options);
    }

    InterpreterObject option(const char *option_name)
    {
	HOM_Attrib::OptionAutoLocks	 locks;
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
