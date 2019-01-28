/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __POPNET_Node_h__
#define __POPNET_Node_h__

#include "POPNET_API.h"
#include <OP/OP_Network.h>
#include <POP/POP_Node.h>
#undef FLOAT_PARM
#undef INT_PARM
#undef STR_PARM

#include <POP/POP_Cache.h>
#include <PRM/PRM_Include.h>
#include <PRM/PRM_Name.h>
#include <UT/UT_String.h>

#define PN_SWITCHER 		"switcher"
#define PN_STARTTIME		"timestart"
#define PN_PREROLL		"timepreroll"
#define PN_INITIALSTATE		"initialstate"
#define PN_RANDOMSEED		"seed"
#define PN_OVERSAMPLE		"oversample"
#define PN_MAXPARTICLES		"maxparticles"
#define PN_REMOVEUNUSED		"rmunused"
#define PN_CACHEFRAMES		"caching"
#define PN_PATH1		"path1"
#define PN_PATH2		"path2"
#define PN_PATH3		"path3"
#define PN_PATH4		"path4"
#define PN_XFORMPATH		"xformpath"

class UT_IStream;
class GU_Detail;

class POPNET_API POPNET_Node : public OP_Network
{
public:
    virtual const char*	getChildType (void) const;
    virtual const char*	getOpType (void) const;

    virtual OP_OpTypeId  getChildTypeID() const;
    virtual OP_OpTypeId  getOpTypeID() const;

    static OP_Node*	myConstructor (OP_Network* net, const char* name,
				       OP_Operator* entry);

    static OP_TemplatePair	myTemplatePair;
    static PRM_Template		myTemplateList[];
    static PRM_Template		myObsoleteList[];
    static void			buildOperatorTable (OP_OperatorTable &table);
    static OP_Operator		*getManagementOperator();
    static const char	*theChildTableName;

    virtual void		resolveObsoleteParms(
						PRM_ParmList *obsolete_parms);

    // This method is static as SOP_Pop and CHOP_Particle want to reuse
    // the same backwards compatibility resolution mechanics.
    static void			applyBackwardsPathCompatibility(
						    PRM_ParmList *parms, 
						    PRM_ParmList *obsolete);

    // This method cooks if necessary, and returns a possibly cached
    // POP_ContextData for the given time.  The caller is expected to pass 
    // in the POP_ContextData it got from the previous call to this
    // method (0 if this is the first call).  After calling this method,
    // the caller should no longer use that old context data.
    // Using this "exchange" mechanism, POPNET_Node can keep track of which 
    // POP_ContextData are still being referenced by POP viewers/spreadsheets,
    // and ensure that they are not deleted prematurely.
    POP_ContextData 	*getCookedContextData(fpreal t, 
					      POP_ContextData *oldContextData);

    // This method is used to release context data's in which the
    // viewer/spreadsheet has lost interest.  After calling this method
    // the caller should no longer use that context data.
    static void		 releaseContextData(POP_ContextData *contextData);

    void	 needToRecookGuides();
    void	 updateGuideGeometry();
    void	 handleGeoSelectionChanged();

    OP_Network	*getXformObj();

    void	 computeNeedReset();
    int		 needReset() { return myNeedReset; }
    int		 cacheObsolete() { return myCacheObsolete; }
    void	 reset();

    virtual void	opChanged(OP_EventType reason, void *data=0);

    static int		ourDoInfoFlag;

    virtual void	 getNodeSpecificInfoText(OP_Context &context,
					OP_NodeInfoParms &iparms);

    static void		 buildInstanceGuide(POP_ContextData *data);

    // POPNETs have no outputs
    virtual unsigned	 maxOutputs() const { return 0; }

    ///Absolute width
    virtual fpreal 	 getW() const;
    ///Absolute height
    virtual fpreal	 getH() const;
    
protected:
	     POPNET_Node (OP_Network* parent, const char* name, 
			  OP_Operator* op);
    virtual ~POPNET_Node (void);

    virtual OP_ERROR	 cookMe (OP_Context &context);
    virtual OP_ERROR	 bypassMe(OP_Context &context, int &copied_input);

    virtual OP_DataType	 getCookedDataType (void) const;
    virtual void*	 getCookedData (OP_Context &);
    virtual void	 deleteCookedData (void);
    virtual int		 saveCookedData (const char *, OP_Context &);
    virtual int		 saveCookedData (std::ostream &os, OP_Context &, 
               		                 int binary = 0);

    virtual const char*	 getFileExtension (int binary) const;

private:
    void		 buildPopTable();
    void		 initializeContextData(float frameInc,
					       POP_Node *cookPop,
					       POP_Node *displayPop);
    void		 changeCacheSize(int newsize);

    fpreal	TIMESTART(void)
		{ return evalFloat(PN_STARTTIME, 0, 0); }
    fpreal	TIMEPREROLL(void)
		{ return evalFloat(PN_PREROLL, 0, 0); }
    void	INITIALSTATE(UT_String &s)
		{ evalString(s, PN_INITIALSTATE, 0, 0); }
    int		RANDOMSEED(void)
		{ return evalInt(PN_RANDOMSEED, 0, 0); }
    int		OVERSAMPLE(void)
		{ return evalInt(PN_OVERSAMPLE, 0, 0); }
    int	 	MAXPARTICLES(void)
		{ return evalInt(PN_MAXPARTICLES, 0, 0); }
    int		RMUNUSED(void)
		{ return evalInt(PN_REMOVEUNUSED, 0, 0); }

    // We don't use a fixed index here in preparation for multiparms.
    int		CACHEFRAMES()
		{ return evalInt("caching", 0, 0); }

    int				 myNeedReset;
    int				 myDoReset;
    int				 myNeedCookGuide;
    int				 myNeedUpdateGuideGeometry;
    int				 myCacheObsolete;   // Need to clear cache
    float			 myPrevFPS;
    GU_Detail			*myGdp;
    POP_ContextData		*myLocalContextData;
    POP_ContextData		*myCachedContextData;
    unsigned long		 myNetChecksum;
    POP_Node			*myLastCookPop;
    POP_Cache			 myPopCache;

    // A member to keep track of the time of the last cook.  We cannot query
    // it from the single precision myLocalContextData->myTime as doing so
    // introduces significant cumulative error.
    //
    // Ideally, POP_ContextData::myTime should be stored with double precision.
    // Such a change is not appropriate as this time, however, as that member
    // is considered part of the input, not the state.  Given that, and the
    // fact that all uses of POP_ContextData::myTime in the POP library would
    // implicitly cast the usage to a single precision float, storing it with
    // double precision would be, at best, misleading.
    double			 myLastCookTime;

    static POP_ContextData*	 theEmptyContextData;
};

#endif
