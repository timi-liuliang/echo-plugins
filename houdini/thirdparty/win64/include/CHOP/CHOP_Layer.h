/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CHOP_Math.h ( Channel Operator Library, C++)
 *
 * COMMENTS:
 *
 * Mix CHOP channels as relative animation layers.
 */

#ifndef __CHOP_Layer__
#define __CHOP_Layer__

#include "CHOP_API.h"
#include "CHOP_Node.h"
#include <CL/CL_Align.h>
#include <UT/UT_Interrupt.h>
#include <SYS/SYS_Types.h>


class UT_String;

#define CHOP_LAYER_ACTIVE_LAYER_INDEX 2

class CHOP_API CHOP_Layer : public CHOP_Node
{
public:


    static OP_Node *myConstructor(OP_Network*, const char *, OP_Operator *);

    static OP_TemplatePair myTemplatePair;
    static OP_VariablePair myVariablePair;
    static PRM_Template myTemplateList[];
    static CH_LocalVariable myVariableList[];

    OP_ERROR cookMyChop(OP_Context &context);

    // Returns the active layer parm
    virtual bool findParmFromTrack(const OP_FollowChanRefsOptions& opt, const char *trackname, OP_NodeParmRefCycle &cycle, OP_Node *&node, PRM_Parm *&parm, int &vecidx); 

    virtual void opChanged(OP_EventType reason, void *data);

    virtual bool scopedChannelsDirty();
    virtual bool getActiveLayerContribution( const UT_String &track_name,  fpreal t, fpreal &value, fpreal &weight);

protected:

    CHOP_Layer(OP_Network *net, const char *name, OP_Operator *op);
    virtual ~CHOP_Layer();

private:

    enum LayerContrib
    {
	LayerContrib_ActiveValues,
	LayerContrib_ActiveWeights,
	LayerContrib_Full
    };

    OP_ERROR internalCookMyChop(OP_Context &context, LayerContrib mode);

    void combineChannels(const CL_Clip *source, CL_TrackListC *tracks, UT_Array<int> *trackWeightIds, CL_Clip *dest);
    void combineCHOPs( const OP_Context &context, CL_ClipListC &source);
    static int updateChannelNames(void *data, int index, fpreal t, const PRM_Template * );
    void cookWeights( const UT_Array<fpreal>& t);
    CL_Track* dupTrackInfo(CL_Clip *dest, const CL_Track *track, int data_too );


    int ACTIVE(void) const
	{ return evalInt("active", 0, 0); }

    int NLAYERS(void) const
	{ return evalInt("nlayers", 0, 0); }
    fpreal WEIGHT(int i, fpreal t) const
	{ return evalFloatInst("weight#", &i, 0, t); }
    bool MUTE(int i, fpreal t) const
	{ return evalIntInst("mute#", &i, 0, t)!=0; }
    bool SOLO(int i, fpreal t) const
	{ return evalIntInst("solo#", &i, 0, t)!=0; }

    int myLastActive;
    bool myInitClip;
    LayerContrib myMode;
    CL_Clip myWorkClip;
    UT_Array<fpreal> myRawWeights;
    UT_Array<fpreal> myWeights;

    // for interrupts..
    UT_Interrupt *myBoss;
    int		myStop;
};

#endif

