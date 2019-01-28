/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_DataFactory_h__
#define __SIM_DataFactory_h__

#include "SIM_API.h"
#include <UT/UT_String.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringHolder.h>

class UT_Guid;
class SIM_Data;
class SIM_DataFactory;
class SIM_DopDescription;
class SIM_Engine;
class SIM_DataFactoryCreator;

typedef void (*SIM_GetSuperclasses)(UT_StringArray &);
typedef SIM_Data *(*SIM_DataConstructor)(const SIM_DataFactory *);
typedef void (*SIM_DataDestructor)(SIM_Data *);

/// This class provides a means of creating and destroying SIM_Data objects.
/// Nothing needs to be known about the SIM_Data object except its data
/// type name. It is possible to derive a subclass to use instead of
/// relying on the default implementations used by the DECLARE_DATAFACTORY
/// and IMPLEMENT_DATAFACTORY macros. But generally these macros will be
/// sufficient.
class SIM_API SIM_DataFactory
{
public:
    /// Constructor which receives all the data for this factory.
    /// The constructor arguments contain all the information that may be
    /// required for the new data type without actually requiring an
    /// instance of that data type to be constructed.
			 SIM_DataFactory(
				 const UT_StringHolder &datatype,
				 const UT_StringHolder &description,
				 SIM_GetSuperclasses get_superclasses,
				 SIM_DataConstructor constructor,
				 SIM_DataDestructor destructor,
				 const SIM_DopDescription *dopdesc,
				 const SIM_Engine *engine);
    /// Destructor.
    virtual		~SIM_DataFactory();

    /// Return the SIM_Engine that owns this factory.
    const SIM_Engine			&getEngine() const;
    /// Get the name of the data type that is created by this factory.
    /// This is the unique identifier that is passed to a SIM_Object to
    /// create a new instance of this data type.
    const UT_StringHolder		&getDataType() const
					    { return myDataType; }
    /// Get array of all superclass names of our data type
    void				 getDataTypeSuperclasses(
						UT_StringArray &classes) const
					    { myGetSuperclasses(classes); }
    /// Gets a string that describes what this data type does.
    /// This descriptive string is what is presented to the user when
    /// given a menu of data type choices.
    const UT_StringHolder		&getDescription() const;
    /// Returns the name of the DSO or DLL file that defines the data type.
    /// For data types defined internally to Houdini the returned string
    /// will be empty.
    const UT_String			&getSource() const;

    /// Create a new instance of our particular subclass of SIM_Data.
    virtual SIM_Data			*newData(bool doinit) const;
    /// Calls newData() to create some new data, then changes the unique id.
    SIM_Data				*newData(const UT_Guid &uniqueid) const;
    /// Delete an instance of our particular subclass of SIM_Data.
    virtual void			 deleteData(SIM_Data *data) const;
    /// Return our SIM_DopDescription.
    virtual const SIM_DopDescription	*getDopDescription() const;

private:
    /// All our data is set up in the constructor and cannot be modified.
    const UT_StringHolder		 myDataType;
    const UT_StringHolder		 myDescription;
    const SIM_GetSuperclasses		 myGetSuperclasses;
    const SIM_DataConstructor		 myConstructor;
    const SIM_DataDestructor		 myDestructor;
    const SIM_DopDescription		*myDopDescription;
    const SIM_Engine			*myEngine;
    const SIM_DataFactoryCreator	*myCreator;

    friend class			 SIM_DataFactoryCreator;
};

/// This helper class lets each SIM_Engine create an instance of
/// each SIM_DataFactory available on the system.
class SIM_API SIM_DataFactoryCreator
{
public:
    /// Defines a function for creating a SIM_DataFactory.
    typedef void (*SIM_DataFactoryCreateFunc)(SIM_Engine *engine);

    /// Constructor sets the SIM_DataFactory creation function.
    /// It also registers this SIM_DataFactoryCreator instance with the
    /// global list in the SIM_Engine class.
		 SIM_DataFactoryCreator(SIM_DataFactoryCreateFunc createfunc);
    /// Destructor which removes this SIM_DataFactoryCreator from the
    /// global list owned by the SIM_Engine class.
		~SIM_DataFactoryCreator();

    /// Returns the name of the DSO or DLL file that defines the data type.
    /// For data types defined internally to Houdini the returned string
    /// will be empty.
    const UT_String	&getSource() const;
    /// Call the creation function to make a new SIM_DataFactory instance.
    void		 createDataFactory(SIM_Engine *engine);
    /// Call addDataFactory on the specified SIM_Engine.
    static void		 addDataFactory(SIM_Engine *engine,
					SIM_DataFactory *factory);

private:
    SIM_DataFactoryCreateFunc		 myCreateFunc;
    const UT_String			 mySource;

    static const SIM_DataFactoryCreator	*theCreatingFactory;
};

#endif

