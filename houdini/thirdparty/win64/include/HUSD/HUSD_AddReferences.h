/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_AddReferences_h__
#define __HUSD_AddReferences_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include "HUSD_LayerOffset.h"
#include <GU/GU_DetailHandle.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringMap.h>

class HUSD_API HUSD_AddReferences
{
public:
			 HUSD_AddReferences(HUSD_AutoWriteLock &lock);
			~HUSD_AddReferences();

    // Setting this flag to true indicates that when doing a Copy Layer
    // merge, the destination prim for each copy operation should be created
    // with any previously undefined parent prims as Xforms.
    void		 setCreateParentPrimsAsXforms(bool createparentxforms)
			 { myCreateParentXforms = createparentxforms; }
    bool		 createParentPrimsAsXforms() const
			 { return myCreateParentXforms; }
    // Specify the primitive kind value to be set on the primitive if we
    // have to create it.
    void		 setPrimKind(const UT_StringHolder &kind)
			 { myPrimKind = kind; }
    const UT_StringHolder &primKind() const
			 { return myPrimKind; }
    // Specify the primitive kind value to be set on the primitive if we
    // have to create it.
    void		 setRefType(const UT_StringHolder &reftype)
			 { myRefType = reftype; }
    const UT_StringHolder &refType() const
			 { return myRefType; }
    // Specify the primitive kind value to be set on the primitive if we
    // have to create it.
    void		 setRefEditOp(const UT_StringHolder &refeditop)
			 { myRefEditOp = refeditop; }
    const UT_StringHolder &refEditOp() const
			 { return myRefEditOp; }

    bool		 addReference(const UT_StringRef &primpath,
				const UT_StringRef &reffilepath,
				const UT_StringRef &refprimpath,
				const HUSD_LayerOffset &offset =
				    HUSD_LayerOffset(),
				const UT_StringMap<UT_StringHolder> &refargs =
				    UT_StringMap<UT_StringHolder>(),
				const GU_DetailHandle &gdh =
				    GU_DetailHandle()) const;

private:
    HUSD_AutoWriteLock	&myWriteLock;
    UT_StringHolder	 myPrimKind;
    UT_StringHolder	 myRefType;
    UT_StringHolder	 myRefEditOp;
    bool		 myCreateParentXforms;
};

#endif

