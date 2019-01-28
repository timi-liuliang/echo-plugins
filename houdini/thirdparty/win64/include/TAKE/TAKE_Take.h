/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TAKE_Take.h ( TAK Library, C++)
 *
 * COMMENTS:	Defines a take
 */

#ifndef __TAKE_Take__
#define __TAKE_Take__

#include "TAKE_API.h"

#include "TAKE_DataId.h"

#include <UT/UT_Map.h>
#include <UT/UT_Array.h>
#include <iosfwd>

class TAKE_Data;
class TAKE_Manager;
class TAKE_StringSaver;
class UT_IStream;
class UT_StringArray;

class TAKE_API TAKE_Take 
{
public:
     TAKE_Take(TAKE_Take *parent, const char *name);
    ~TAKE_Take();

    // Parent/child management
    void	 destroyAllKids();
    void	 destroyKid(TAKE_Take *kid);

    // Insert a layer between us and our children
    void	 insertChildTake(TAKE_Take *kid);
    bool	 isAncestor(const TAKE_Take *take) const;

    // Add a child layer to us.  This layer will inherit all unbound data
    // from us.
    void	 addChildTake(TAKE_Take *kid)
		 {
		     kid->myParent = this;
		     myKids.append(kid);
		     mySortDirty = 1;
		 }

    void	 applyTake(int setactive, UT_Array<TAKE_Data *>&data);
    void	 applySpecificTake(bool setactive, const TAKE_DataId &id);
    void	 markActive();
    void	 clearData();

    int64	 getMemoryUsage(bool inclusive) const;

    // Name of the take
    void	 setName(const char *name);
    const char	*getName() const	{ return myName; }

    int		 save(TAKE_StringSaver &strings, std::ostream &os, int binary) const;
    int		 load(UT_StringArray &strings, TAKE_Manager *mgr, UT_IStream &is);

    // Data associated with the take layer
    TAKE_Data	*getData(const TAKE_DataId &id) const;
    void	 addData(TAKE_Data *data);
    void	 destroyData(const TAKE_DataId &id);
    void	 replaceData(TAKE_Data *new_data);

    // Returns an unsorted list of data
    void	 getDataList(UT_Array<TAKE_Data *> &list) const;
    void	 getNodeDataList(UT_Array<TAKE_Data *>&list, int opid) const;
    int		 getDeadDataCount() const	{ return myDeadDataCount; }


    const TAKE_Take	*getParent() const		{ return myParent; }
    TAKE_Take		*getParent()			{ return myParent; }
    void		 setParent(TAKE_Take *p)	{ myParent = p; }
    bool		 reparent(TAKE_Take *newparent);

    int			 entries() const	{ return myKids.entries(); }
    TAKE_Take		*getChild(int i)
			 {
			     if (mySortDirty) sortKids();
			     return myKids(i);
			 }
    int			 getChildren(UT_Array<TAKE_Take *>&layers);

    // semi-private.  This needs to be public to be accessed by static
    // functions, but should be considered private.
    void		 deReferenceMasterData(TAKE_Data *data);

    void		 cleanDeadData();

private:
    void			 sortKids();
    
    typedef UT_Map<TAKE_DataId, TAKE_Data *, TAKE_DataId::Hasher> DataMap;

    UT_Array<TAKE_Take *> myKids;
    TAKE_Take	*myParent;
    UT_StringHolder myName;
    
    DataMap	myData;
    int		myDeadDataCount;
    bool	mySortDirty:1;
};

#endif
