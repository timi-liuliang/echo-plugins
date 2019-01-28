/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AttributeRef.h (GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_AttributeRef__
#define __GA_AttributeRef__

#include "GA_API.h"
#include "GA_Attribute.h"
#include "GA_AIFStringTuple.h"
#include "GA_AIFSharedStringArray.h"
#include "GA_AIFNumericArray.h"
#include "GA_AIFTuple.h"
#include "GA_Types.h"

#include <SYS/SYS_Compiler.h>
#include <UT/UT_Array.h>

class GA_AIFCopyData;
class GA_AIFIndexPair;
class GA_AIFSharedStringTuple;
class GA_AIFInterp;
class GA_AIFMath;
class GA_Attribute;


/// @brief This class provides a way to manage a reference to an attribute
/// permitting Write-Only access.
class GA_API GA_WOAttributeRef
{
public:
    /// Default constructor.  If an attribute is passed in, the ref will be
    /// valid.  Otherwise, it will be invalid.
    GA_WOAttributeRef(GA_Attribute *attrib = 0);

    /// Define the * operator to dereference the attribute
    GA_Attribute	&operator*() const
			    { UT_ASSERT(myAttribute); return *myAttribute; }
    /// Define the -> operator to access the attribute
    GA_Attribute	*operator->() const
			    { return myAttribute; }

    /// Assignment operator
    GA_WOAttributeRef	&operator=(GA_Attribute *attrib);
    
    /// Comparison operator
    bool		operator==(const GA_WOAttributeRef &src) const
			    { 
				return 
				(myAttribute == src.myAttribute);
			    }
    /// Comparison operator
    bool		operator!=(const GA_WOAttributeRef &src) const
			    { 
				return 
				( myAttribute != src.myAttribute);
			    }

    /// Test to see if the attribute reference refers to a valid attribute
    bool		 isValid() const	
			    { 
				return myAttribute;
			    }

    /// Test to see if the attribute reference refers to a valid attribute
    bool		 isInvalid() const	
			    { 
				return !myAttribute;
			    }

    /// Method  to clear the handle (so that it is no longer valid)
    void		 clear();

    /// Set type information on the attribute
    bool		 setTypeInfo(GA_TypeInfo info);

    /// Determine the storage type of the underlying attribute.
    bool		 isFloat() const 
    { if (!getAIFTuple()) return false;
      return GAisFloatStorage(getAIFTuple()->getStorage(get())); }
    bool		 isInt() const 
    { if (!getAIFTuple()) return false;
      return GAisIntStorage(getAIFTuple()->getStorage(get())); }
    bool		 isString() const 
    { if (getAIFTuple()) return false;
      if (getAIFStringTuple()) return true;
      if (getAIFSharedStringArray()) return true;
      return false; }

    GA_StorageClass	 getStorageClass() const
    { if (getAIFTuple()) { return GAstorageClass(getAIFTuple()->getStorage(get())); }
      if (getAIFStringTuple()) return GA_STORECLASS_STRING;
      if (getAIFSharedStringArray()) return GA_STORECLASS_STRING;
      if (getAIFNumericArray()) { return GAstorageClass(getAIFNumericArray()->getStorage(get())); }
      return GA_STORECLASS_OTHER;
    }

    /// Tuple size of the attribute.  Non-tuple types are treated
    /// as size one.
    int			 getTupleSize() const
    { if (getAIFTuple()) { return getAIFTuple()->getTupleSize(get()); }
      if (getAIFStringTuple()) { return getAIFStringTuple()->getTupleSize(get()); }
      if (getAIFSharedStringArray()) { return getAIFSharedStringArray()->getTupleSize(get()); }
      if (getAIFNumericArray()) { return getAIFNumericArray()->getTupleSize(get()); }
      return 1;
    }
    void		 setTupleSize(int size) 
    { if (getAIFTuple()) { getAIFTuple()->setTupleSize(get(), size); }
      if (getAIFStringTuple()) { getAIFStringTuple()->setTupleSize(get(), size); }
      if (getAIFNumericArray()) { getAIFNumericArray()->setTupleSize(get(), size); }
    }

    inline GA_Attribute		*get() const { return myAttribute; }
    inline operator GA_Attribute *() const   { return myAttribute; }
    SYS_SAFE_BOOL inline operator bool() const  { return myAttribute != 0; }
    inline GA_Attribute		*getAttribute() const 
				 { 
				     return myAttribute;
				 }
    inline const GA_AIFCopyData	*getAIFCopyData() const 
				 { 
				     return myCopyData; 
				 }
    inline const GA_AIFMath	*getAIFMath() const
				 { 
				     return myMath;
				 }
    inline const GA_AIFInterp	*getAIFInterp() const 
				 { 
				     return myInterp;
				 }
    inline const GA_AIFTuple	*getAIFTuple() const
				 { 
				     return myTuple;
				 }
    inline const GA_AIFStringTuple*getAIFStringTuple() const
				 { 
				     return myStringTuple;
				 }
    inline const GA_AIFIndexPair*getAIFIndexPair() const 
				 { 
				     return myIndexPair;
				 }
    inline const GA_AIFNumericArray*getAIFNumericArray() const 
				 {
				     if (get())
					 return get()->getAIFNumericArray();
				     return 0;
				 }
    inline const GA_AIFSharedStringArray*getAIFSharedStringArray() const 
				 {
				     if (get())
					 return get()->getAIFSharedStringArray();
				     return 0;
				 }


private:
    GA_Attribute		*myAttribute;
    const GA_AIFMath		*myMath;
    const GA_AIFInterp		*myInterp;
    const GA_AIFCopyData	*myCopyData;
    const GA_AIFTuple		*myTuple;
    const GA_AIFStringTuple	*myStringTuple;
    const GA_AIFIndexPair	*myIndexPair;

    friend class GA_ROAttributeRef;
};


/// @brief This class provides a way to manage a reference to an attribute
/// permitting Read-Write access.
class GA_API GA_RWAttributeRef : public GA_WOAttributeRef
{
public:
    /// Default constructor.  If an attribute is passed in, the ref will be
    /// valid.  Otherwise, it will be invalid.
    GA_RWAttributeRef(GA_Attribute *attrib = 0);

private:
    /// No additional data beyond the base classes so that splicing to the
    /// base class in pass-by-value situations isn't an issue.
};


/// @brief This class provides a way to manage a reference to an attribute
/// permitting Read-Only access.
class GA_API GA_ROAttributeRef
{
public:
    /// Default constructor.  If an attribute is passed in, the ref will be
    /// valid.  Otherwise, it will be invalid.
    GA_ROAttributeRef(const GA_Attribute *attrib = 0);

    /// Conversion constructor (implicit)
    GA_ROAttributeRef(const GA_RWAttributeRef &src)
    {
	myAttribute = src.myAttribute;
	myMath = src.myMath;
	myCopyData = src.myCopyData;
	myInterp = src.myInterp;
	myTuple = src.myTuple;
	myStringTuple = src.myStringTuple;
	myIndexPair = src.myIndexPair;
    }

    /// Define the * operator to dereference the attribute
    const GA_Attribute	&operator*() const
			    { UT_ASSERT(myAttribute); return *myAttribute; }
    /// An -> operator to access the attribute
    const GA_Attribute	*operator->() const
			    { return myAttribute; }

    /// Assignment operators
    GA_ROAttributeRef	&operator=(const GA_Attribute *attrib);
    
    GA_ROAttributeRef	&operator=(const GA_RWAttributeRef &src)
    {
	myAttribute = src.myAttribute;
	myMath = src.myMath;
	myCopyData = src.myCopyData;
	myInterp = src.myInterp;
	myTuple = src.myTuple;
	myStringTuple = src.myStringTuple;
	myIndexPair = src.myIndexPair;

	return *this;
    }

    /// Comparison operator
    bool		operator==(const GA_ROAttributeRef &src) const
			    { return 
				(myAttribute == src.myAttribute);
			    }
    /// Comparison operator
    bool		operator!=(const GA_ROAttributeRef &src) const
			    { 
				return 
				(myAttribute != src.myAttribute);
			    }

    /// Test to see if the attribute reference refers to a valid attribute
    inline bool		 isValid() const	
			    { 
				return myAttribute;
			    }

    /// Test to see if the attribute reference refers to a valid attribute
    inline bool		 isInvalid() const
			    { 
				return !myAttribute;
			    }

    /// Method  to clear the handle (so that it is no longer valid)
    void		 clear();

    /// Get type information on the attribute, with invalid references
    /// returning GA_TYPE_VOID.
    GA_TypeInfo		 getTypeInfo() const;

    /// Determine the storage type of the underlying attribute.
    bool		 isFloat() const 
    { if (!getAIFTuple()) return false;
      return GAisFloatStorage(getAIFTuple()->getStorage(get())); }
    bool		 isInt() const 
    { if (!getAIFTuple()) return false;
      return GAisIntStorage(getAIFTuple()->getStorage(get())); }
    bool		 isString() const 
    { if (getAIFTuple()) return false;
      if (getAIFStringTuple()) return true;
      if (getAIFSharedStringArray()) return true;
      return false; }

    GA_StorageClass	 getStorageClass() const
    {
	if (getAIFTuple())
	    return GAstorageClass(getAIFTuple()->getStorage(get()));
	if (getAIFStringTuple())
	    return GA_STORECLASS_STRING;
	if (getAIFSharedStringArray())
	    return GA_STORECLASS_STRING;
	if (getAIFNumericArray())
	    return GAstorageClass(getAIFNumericArray()->getStorage(get()));
	return GA_STORECLASS_OTHER;
    }

    /// Tuple size of the attribute.  Non-tuple types are treated
    /// as size one.
    int			 getTupleSize() const
    {
	if (getAIFTuple())
	    return getAIFTuple()->getTupleSize(get());
	if (getAIFSharedStringArray())
	    return getAIFSharedStringArray()->getTupleSize(get());
	if (getAIFStringTuple())
	    return getAIFStringTuple()->getTupleSize(get());
	if (getAIFNumericArray()) 
	    return getAIFNumericArray()->getTupleSize(get());
	return 1;
    }

    inline const GA_Attribute	*get() const     { return myAttribute; }
    inline operator const GA_Attribute *() const { return myAttribute; }
    SYS_SAFE_BOOL inline operator bool() const  { return myAttribute != 0; }
    inline const GA_Attribute	*getAttribute() const 
				 { 
				     return myAttribute;
				 }
    inline const GA_AIFCopyData	*getAIFCopyData() const
				 { 
				     return myCopyData;
				 }
    inline const GA_AIFMath	*getAIFMath() const
				 { 
				     return myMath;
				 }
    inline const GA_AIFInterp	*getAIFInterp() const
				 { 
				     return myInterp;
				 }
    inline const GA_AIFTuple	*getAIFTuple() const
				 { 
				     return myTuple;
				 }
    inline const GA_AIFStringTuple*getAIFStringTuple() const 
				 {
				     return myStringTuple;
				 }
    inline const GA_AIFNumericArray*getAIFNumericArray() const 
				 {
				     if (get())
					 return get()->getAIFNumericArray();
				     return 0;
				 }
    inline const GA_AIFSharedStringArray*getAIFSharedStringArray() const 
				 {
				     if (get())
					 return get()->getAIFSharedStringArray();
				     return 0;
				 }

    /// Shared String Tuple Interface.  This allows you to get integer or
    /// string values out of the attribute.
    const GA_AIFSharedStringTuple *getAIFSharedStringTuple() const;
    inline const GA_AIFIndexPair*getAIFIndexPair() const
				 { 
				     return myIndexPair;
				 }

    /// Look up the AIFStringTuple and evaluate the string.  If there is no AIF
    /// support, this will return a NULL pointer
    const char		*getString(GA_Offset element_index,
				int tuple_index=0) const;

private:
    const GA_Attribute			*myAttribute;
    const GA_AIFMath			*myMath;
    const GA_AIFInterp			*myInterp;
    const GA_AIFCopyData		*myCopyData;
    const GA_AIFTuple			*myTuple;
    const GA_AIFStringTuple		*myStringTuple;
    const GA_AIFIndexPair		*myIndexPair;
};

#endif
