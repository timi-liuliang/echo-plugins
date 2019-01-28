/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_DataUtils_h__
#define __SIM_DataUtils_h__

#include "SIM_Data.h"
#include "SIM_DataFactory.h"
#include "SIM_Utils.h"
#include <UT/UT_StringArray.h>
#include <UT/UT_StringHolder.h>
#include <SYS/SYS_Compiler.h>

/// This macro provides a quick way to define a static function called
/// classname() which returns the provided class name as a string.
#define DECLARE_CLASSNAME(DataClass, SuperClass) \
	private: \
	    typedef SuperClass	 BaseClass; \
	    typedef DataClass	 ThisClass; \
	    static inline UT_StringHolder classname() \
				 { return SYS_CONCAT(#DataClass,_UTsh); } \
	    virtual const UT_String	&getDataTypeSubclass() const \
				 { \
				     static UT_String theType(classname()); \
				     return theType; \
				 } \
	protected: \
	    static void	    getDataTypeSuperclasses(UT_StringArray& classes) \
			    { \
				classes.append(SYS_CONCAT(#SuperClass,_UTsh)); \
				SuperClass::getDataTypeSuperclasses(classes); \
			    } \
	private: \
	/**/

/// This function implements a standard version of getDistanceToType().
/// Only if your subclass uses multiple inheritance will it be necessary to
/// write a customized version of this function. Be sure to add this
/// declaration to your class definition or the DECLARE_DATAFACTORY()
/// macro will not compile properly.
#define DECLARE_STANDARD_GETCASTTOTYPE() \
	protected: \
	    virtual void	*getCastToType(const char *totype) const \
				 { \
				     if( strcmp(classname(), totype) == 0 ) \
					 return (void *)this; \
				     return BaseClass::getCastToType(totype); \
				 } \
	/**/

/// This macro simplifies the calling and casting the return value from
/// This macro quickly defines all the static functions needed to declare
/// a SIM_DataFactory for a SIM_Data subclass.
#define DECLARE_DATAFACTORY(DataClass, SuperClass, Description, DopParms) \
	public: \
	    class DataClass##Factory : public SIM_DataFactory \
	    { \
		public: \
				 DataClass##Factory(SIM_Engine *engine) \
				    : SIM_DataFactory(\
					  SYS_CONCAT(#DataClass,_UTsh), \
					  SYS_CONCAT(Description,_UTsh), \
					  DataClass::getDataTypeSuperclasses, \
					  DataClass::constructor, \
					  DataClass::destructor, \
					  DataClass::DopParms, \
					  engine) \
				 { } \
		virtual		~DataClass##Factory() \
				 { } \
	    }; \
	    static void		 createDataFactory(SIM_Engine *engine) \
				 { \
				     SIM_DataFactory	*factory; \
				     factory = new DataClass##Factory( \
						   engine); \
				     SIM_DataFactoryCreator::addDataFactory \
						   (engine, factory); \
				 } \
	    friend class	 DataClass##Factory; \
	private: \
	    DECLARE_CLASSNAME(DataClass, SuperClass); \
	    static SIM_Data	*constructor(const SIM_DataFactory *factory) \
				 { return (BaseClass *)new \
					  DataClass(factory); } \
	    static void		 destructor(SIM_Data *data) \
				 { delete (DataClass *)data-> \
					  getPointerToType(#DataClass); } \
	/**/

/// Add this macro to the initialization function for your library. It uses
/// the declaration above to register your data type.
#define IMPLEMENT_DATAFACTORY(DataClass) \
	static SIM_DataFactoryCreator \
	       DataClass##Creator(DataClass::createDataFactory);

#endif

