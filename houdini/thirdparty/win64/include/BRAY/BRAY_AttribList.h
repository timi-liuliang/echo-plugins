/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_AttribList.h (RAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __RAY_AttribList__
#define __RAY_AttribList__

#include "BRAY_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_NonCopyable.h>
#include <VEX/VEX_LateBinding.h>
#include <GA/GA_Types.h>

class UT_JSONWriter;

/// Each BRAY_Object can define a list of attributes that it can evaluate.
class BRAY_API BRAY_AttribList
    : UT_NonCopyable
{
public:
    // TODO: Almost identical to a VEX_LateBinding::LateParameter
    struct BRAY_API Attrib
    {
	Attrib() = delete;
	Attrib(const UT_StringHolder &name,
		int tuple_size,
		GA_AttributeOwner owner,
		GA_Storage store,
		bool is_array)
	    : myName(name)
	    , myTupleSize(tuple_size)
	    , myOwner(owner)
	    , myStorage(store)
	    , myVexBase(getVexBaseType(store))
	    , myIsArray(is_array)
	{
	}
	static VEX_BaseType	getVexBaseType(GA_Storage store);

	const UT_StringHolder	&name() const { return myName; }
	int			 tupleSize() const { return myTupleSize; }
	GA_AttributeOwner	 owner() const { return myOwner; }
	GA_Storage		 storage() const { return myStorage; }
	VEX_BaseType		 vexBaseType() const { return myVexBase; }
	bool			 isArray() const { return myIsArray; }
	bool			 isDisplayColor() const;

	void			 dump() const;
	void			 dump(UT_JSONWriter &w) const;

	UT_StringHolder		myName;
	int			myTupleSize;
	GA_AttributeOwner	myOwner;
	GA_Storage		myStorage;
	VEX_BaseType		myVexBase;
	bool			myIsArray;
    };

    BRAY_AttribList();
    BRAY_AttribList(const Attrib *list, int list_size, bool volume);
    ~BRAY_AttribList();

    /// Look up a list of attributes in a shared table.  If different
    /// BRAY_Objects share the same attribute list, then it's possible they will
    /// be shaded in batch.  Using the shared table can improve rendering
    /// performance.
    static const BRAY_AttribList	*findList(const Attrib *list,
						int list_size,
						bool for_volume);
    static const BRAY_AttribList	*findList(const BRAY_AttribList &src);

    void	setCapacity(int n);
    int		addAttribute(const UT_StringHolder &name,
			int tuple_size,
			GA_AttributeOwner owner,
			GA_Storage storage,
			bool isarray);

    int			size() const { return myList.size(); }
    bool		forVolume() const { return myForVolume; }
    GA_Storage		storage(int i) const { return myList[i].myStorage; }
    int			tupleSize(int i) const { return myList[i].myTupleSize; }
    GA_AttributeOwner	owner(int i) const { return myList[i].myOwner; }
    VEX_BaseType	vexBaseType(int i) const { return myList[i].myVexBase; }
    bool		isArray(int i) const { return myList[i].myIsArray; }
    const UT_StringHolder	&name(int i) const { return myList[i].myName; }
    const Attrib	&attrib(int i) const { return myList[i]; }
    const Attrib	&operator[](int i) const { return myList[i]; }
    int			 displayColor() const { return myDisplayColor; }

    const VEX_LateBindingPtr	&lateBinding() const { return myBinding; }

    void	dump() const;
    void	dump(UT_JSONWriter &w) const;

private:
    UT_Array<Attrib>	myList;
    VEX_LateBindingPtr	myBinding;
    int			myDisplayColor;
    bool		myForVolume;
};

#endif
