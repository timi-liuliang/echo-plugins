/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_MergeInto_h__
#define __HUSD_MergeInto_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include <UT/UT_StringArray.h>
#include <UT/UT_UniquePtr.h>

class HUSD_API HUSD_MergeInto
{
public:
			 HUSD_MergeInto();
			~HUSD_MergeInto();

    // Setting this flag to true indicates that the destination prim for each
    // merge operation should be created with any previously undefined parent
    // prims as Xforms.
    void		 setCreateParentPrimsAsXforms(bool createparentxforms)
			 { myCreateParentXforms = createparentxforms; }
    bool		 createParentPrimsAsXforms() const
			 { return myCreateParentXforms; }
    // Specify the primitive kind value to be set on the inserted primitives.
    void		 setPrimKind(const UT_StringHolder &kind)
			 { myPrimKind = kind; }
    const UT_StringHolder &primKind() const
			 { return myPrimKind; }
    // Specify the primitive kind value to be set on any automatically created
    // parents of the inserted primitives.
    void		 setParentPrimKind(const UT_StringHolder &kind)
			 { myParentPrimKind = kind; }
    const UT_StringHolder &parentPrimKind() const
			 { return myParentPrimKind; }
    // Specify that this operation should guarantee that each destination
    // location is unique, and doesn't overwrite any existing primitive on
    // the stage.
    void		 setMakeUniqueDestPaths(bool make_unique)
			 { myMakeUniqueDestPaths = make_unique; }
    bool		 makeUniqueDestPaths() const
			 { return myMakeUniqueDestPaths; }

    bool		 addHandle(const HUSD_DataHandle &src,
				const UT_StringHolder &dest_path,
				const UT_StringHolder &source_node_path);
    bool		 execute(HUSD_AutoLayerLock &lock) const;

private:
    class husd_MergeIntoPrivate;

    UT_UniquePtr<husd_MergeIntoPrivate>	 myPrivate;
    UT_StringArray			 myDestPaths;
    UT_StringArray			 mySourceNodePaths;
    UT_StringHolder			 myPrimKind;
    UT_StringHolder			 myParentPrimKind;
    bool				 myCreateParentXforms;
    bool				 myMakeUniqueDestPaths;
};

#endif

