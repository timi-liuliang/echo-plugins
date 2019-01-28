/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	XUSD_HydraInstancer.h (HUSD Library, C++)
 *
 * COMMENTS:	Basic instancer for creating instance transforms.
 *
 */

#ifndef XUSD_HydraInstancer_h
#define XUSD_HydraInstancer_h

#include "HUSD_API.h"
#include <UT/UT_Lock.h>
#include <GT/GT_Transform.h>
#include <GT/GT_TransformArray.h>

#include <pxr/pxr.h>
#include <pxr/imaging/hd/instancer.h>
#include <pxr/imaging/hd/vtBufferSource.h>
#include <pxr/base/tf/hashmap.h>
#include <pxr/base/tf/token.h>

PXR_NAMESPACE_OPEN_SCOPE

class HUSD_API XUSD_HydraInstancer : public HdInstancer
{
public:
    XUSD_HydraInstancer(HdSceneDelegate* del,
			SdfPath const& id,
			SdfPath const &parentInstancerId);
    virtual ~XUSD_HydraInstancer();

    VtMatrix4dArray	computeTransforms(const SdfPath &protoId,
					  bool recurse,
					  const GfMatrix4d *protoXform);

    // Checks the change tracker to determine whether instance primvars are
    // dirty, and if so pulls them. Since primvars can only be pulled once,
    // and are cached, this function is not re-entrant. However, this function
    // is called by ComputeInstanceTransforms, which is called (potentially)
    // by HdMantraMesh::Sync(), which is dispatched in parallel, so it needs
    // to be guarded by _instanceLock.
    //
    // Pulled primvars are cached in _primvarMap.
    void	syncPrimvars();

    // Map of the latest primvar data for this instancer, keyed by
    // primvar name. Primvar values are VtValue, an any-type; they are
    // interpreted at consumption time (here, in ComputeInstanceTransforms).
    TfHashMap<TfToken,
              HdVtBufferSource*,
              TfToken::HashFunctor> myPrimvarMap;

    mutable UT_Lock myLock;
};

class XUSD_HydraTransforms : public GT_TransformArray
{
public:
	     XUSD_HydraTransforms() : myDataId(-1) {}
    virtual ~XUSD_HydraTransforms() {}
    
    void	  setDataId(int64 id) { myDataId = id; }
    virtual int64 getDataId() const   { return myDataId; }
private:
    int64 myDataId;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
