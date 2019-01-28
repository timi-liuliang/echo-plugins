/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_ShapeDiff.h (C++)
 *
 * COMMENTS: See http://internal/wiki/index.php/Houdini17/Character/PoseSpaceDeformation for desc.
 *
 */

#ifndef __SOP_ShapeDiff_h__
#define __SOP_ShapeDiff_h__

#include "SOP_API.h"
#include "SOP_Node.h"

#include <GA/GA_Handle.h>
#include <GA/GA_PageHandle.h>

class SOP_DeformXforms;

class SOP_API SOP_ShapeDiff : public SOP_Node
{
public:
    template <typename T, template <typename> class HANDLE_T>
    struct HandlePairT
    {
	typedef T				Type;
	typedef typename HANDLE_T<T>::ROType	ROHandle;
	typedef typename HANDLE_T<T>::RWType	RWHandle;

	GA_Range				myRange;
	ROHandle				mySrcRestH;
	ROHandle				mySrcDeformH;
	RWHandle				myDstH;
    };
    template <typename T>
    struct HandlePair
    {
	typedef HandlePairT<T, GA_HandleT>	Type;
    };
    template <typename T>
    struct PageHandlePair
    {
	typedef HandlePairT<T, GA_PageHandleV>	Type;
    };

    typedef HandlePair<UT_Vector3>::Type	HandlePairV3;
    typedef HandlePair<UT_Quaternion>::Type	HandlePairQ;
    typedef PageHandlePair<UT_Vector3>::Type    PageHandlePairV3;
    typedef PageHandlePair<UT_Quaternion>::Type	PageHandlePairQ;

				SOP_ShapeDiff(OP_Network *net, const char *name, OP_Operator *entry);
    virtual			~SOP_ShapeDiff();

    static OP_Node*		myConstructor(OP_Network *net, const char *name, OP_Operator *entry);
    static PRM_Template*	buildTemplates();
    virtual const SOP_NodeVerb*	cookVerb() const override final;

protected:
    friend class SOP_ShapeDiffVerb;

    virtual const char*		inputLabel(unsigned idx) const override final;
    virtual bool		load(UT_IStream &is, const char *extension, const char *path = 0) override;

    virtual OP_ERROR		cookMySop(OP_Context &context) override final;
    virtual OP_ERROR		cookMyGuide1(OP_Context &context) override final;

    void			initDeformXforms(OP_Context &context);

    // Get attributes based on destination gdp
    // dqblendattrib is not a deform attrib, just used for blending
    void			getAttribs(
				    const GU_Detail *inputgeo,
				    const GU_Detail *deformgeo,
				    UT_Array<PageHandlePairV3> &normaloffsets,
				    UT_Array<PageHandlePairV3> &vectoroffsets,
				    UT_Array<PageHandlePairQ>  &quaternionoffsets);

    UT_Array<PageHandlePairV3>	myNormalOffsets;
    UT_Array<PageHandlePairV3>	myVectorOffsets;
    UT_Array<PageHandlePairQ>	myQuaternionOffsets;

private:
    //
    // UI Parameter methods
    SYS_FORCE_INLINE
    int DIFF_TYPE(fpreal t = 0.0f) const
	{ return evalInt("difftype", 0, t); }
    SYS_FORCE_INLINE
    void BONE_TRANSFORM_PATH(UT_String &str, fpreal t = 0.0f) const
	{ evalString(str, "bonetransformpath", 0, t); }
    SYS_FORCE_INLINE
    void SKEL_ROOT_PATH(UT_String &str, fpreal t = 0.0f) const
	{ evalString(str, "skelrootpath", 0, t); }
    SYS_FORCE_INLINE
    bool DONORMAL(fpreal t = 0.0f) const
	{ return evalInt("donormal", 0, t); }
    SYS_FORCE_INLINE
    bool DOATTRIBS(fpreal t = 0.0f) const
	{ return evalInt("dovattribs", 0, t); }
    SYS_FORCE_INLINE
    void ATTRIBS(UT_String &str, fpreal t = 0.0f) const
	{ evalString(str, "vattribs", 0, t); }
    SYS_FORCE_INLINE
    bool DOQATTRIBS(fpreal t = 0.0f) const
	{ return evalInt("doqattribs", 0, t); }
    SYS_FORCE_INLINE
    void QATTRIBS(UT_String &str, fpreal t = 0.0f) const
	{ evalString(str, "qattribs", 0, t); }

    void			getCaptureRegionRootPath(const GU_Detail *igdp, UT_String &root_path);

    SOP_DeformXforms*		myDeformXforms;
};

#endif // __SOP_ShapeDiff_h__
