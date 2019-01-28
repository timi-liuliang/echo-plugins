/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RBD_SharedAffectorList.h ( RBD Library, C++)
 *
 * COMMENTS:
 *	Provides a holder for shared lists of affectors.  These
 *	store the proxy RBD objects for the affectors and any
 *	spatial partitioning data that is useful.
 */

#ifndef __RBD_SharedAffectorList__
#define __RBD_SharedAffectorList__

#include "RBD_API.h"
#include <UT/UT_ValArray.h>
#include <SIM/SIM_ColliderInfo.h>

class UT_TokenString;
class RBD_Object;
class RBD_ObjectTree;
class RBD_Solver;
class RBD_SharedAffectorList;

typedef UT_ValArray<RBD_SharedAffectorList *> RBD_SharedAffectorListArray;

//
// RBD Shared affector list definition
//
class RBD_API RBD_SharedAffectorList
{
public:
    RBD_SharedAffectorList();
    ~RBD_SharedAffectorList();

    // These functions are used when accessing the list.
    int			 entries() const
			 { return myAffectors.entries(); }
    RBD_Object		*operator()(int i) const
			 { return myAffectors(i); }
    const SIM_Collider	*getCollider(int i, SIM_Object *obj, const char *defaultlabel)
			 { return myColliderInfoArray(i).getCollider(obj, defaultlabel); }
    SIM_Collider::SIM_ImpactApplyType	 getImpactApplyType(int i)
			 { return myColliderInfoArray(i).getImpactApplyType(); }

    // Gets the overlapping objects from our trees.
    void		 findOverlapIdx(const RBD_Solver *solver,
					RBD_Object *obj,
					UT_IntArray &overlap);
    // Dirties the object tree in case some of the solving objects have moved.
    void		 setObjectTreeDirty();

    // These functions are used when building the list.
    RBD_Object			*&operator()(int i)
				 { return myAffectors(i); }
    SIM_ColliderInfoArray	&getColliderInfoArray()
				 { return myColliderInfoArray; }
    void			 resizeAffectors();

private:
    UT_ValArray<RBD_Object *>	 myAffectors;
    SIM_ColliderInfoArray	 myColliderInfoArray;
    RBD_ObjectTree		*myMutualObjectTree;
    RBD_ObjectTree		*myOtherObjectTree;
};

#endif

