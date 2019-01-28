/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DOP_Auto.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __DOP_Auto_h__
#define __DOP_Auto_h__

#include "DOP_API.h"
#include <SIM/SIM_Options.h>
#include <SIM/SIM_RootDataId.h>
#include "DOP_Node.h"

class DOP_API DOP_Auto : public DOP_Node
{
public:
	     DOP_Auto(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~DOP_Auto();

    virtual const char		*inputLabel(unsigned idx) const;
    virtual int			 cookedInputIndex() const
				 { return myLastInput; }

    virtual OP_ERROR		 cookMe(OP_Context &context);
    virtual bool		 evalVariableValue(
					fpreal &value, int index, int thread);
    virtual bool		 evalVariableValue(
					UT_String &str,int index, int thread);

    virtual void		 getNodeSpecificInfoText(OP_Context &context,
					OP_NodeInfoParms &iparms);
    virtual void		 fillInfoTreeNodeSpecific(UT_InfoTree &tree,
					const OP_NodeInfoTreeParms &parms);

    static OP_Node		*myConstructor(OP_Network *net,
					const char *name,
					OP_Operator *entry);
    static PRM_Template		 theCommonTemplates[];
    static CH_LocalVariable	 theCommonVariables[];

    virtual void                 resolveObsoleteParms(
					PRM_ParmList *obsolete_parms);

protected:
    void		 setLastCookedInputIndex(int idx) { myLastInput = idx; }
    virtual void	 processObjectsSubclass(fpreal time,
					int foroutputidx,
					const SIM_ObjectArray &objects,
					DOP_Engine &engine);
    /// This function just makes sure we are active and that the object
    /// being processed is part of our group, and calls doApplyOutputData
    /// to do all the work of actually attaching the data.
    virtual void	 applyOutputDataSubclass(fpreal time,
					int outputidx,
					SIM_RootData &rootdata,
					const char *parentdataname,
					DOP_Engine &engine,
					UT_StringArray *datanames);
    virtual void	 preNetworkProcessingSubclass(
					DOP_Engine &engine, bool firstframe);
    virtual void	 getInputInfoSubclass(int inputidx,
					      DOP_InOutInfo &info) const;
    virtual void	 getOutputInfoSubclass(int outputidx,
					       DOP_InOutInfo &info) const;
    virtual PRM_Template*getCommonTemplates() const;
    virtual int		 getFirstSubdataInput() const;
    virtual bool	 getHasInputObjects() const;
    /// True if we are to have no grey inputs and only work with
    /// apply data.
    bool		 isPureApplyData() const;

    virtual bool	 ASSUMEDATACONST();
    virtual int		 SHAREDATA();
    virtual int		 UNIQUEDATANAME();
    virtual void	 DATANAME(UT_String &str, fpreal t);

    void		 GROUP(UT_String &str, fpreal t);

    const SIM_Data	*getShareData(const SIM_Engine &engine,
				const char *lcddatatype);
    void		 setShareData(const SIM_RootData &rootdata,
				const char *dataname);
    void		 attachInputSubData(SIM_RootData &rootdata,
				const char *dataname,
				DOP_Engine &engine,
				fpreal time);

    /// This function actually does all the work for applyOutputDataSubclass.
    /// This extra level of indirection allows efficient impementation of
    /// the processObjectsSubclass method. This function calls
    /// doApplyOutputDataSubclass().
    void		 doApplyOutputData(fpreal time,
				int outputidx,
				SIM_RootData &rootdata,
				const char *parentdataname,
				DOP_Engine &engine,
				UT_StringArray *datanames,
				bool active);

    /// Implements the doApplyOutputData functionality. Making this function
    /// virtual allows it to be overridden (by DOP_FileData, for example).
    virtual void	 doApplyOutputDataSubclass(fpreal time,
				int outputidx,
				SIM_RootData &rootdata,
				const char *parentdataname,
				DOP_Engine &engine,
				UT_StringArray *datanames,
				bool active);

    void		 removeData(SIM_RootData &rootdata,
				const UT_String &dataname);

    virtual void	 setAdditionalParametersOnDataSubclass(
				SIM_Data &data, SIM_RootData &root_data,
				const char *data_name, DOP_Engine &engine,
				int output_idx);

    virtual void	syncNodeVersion(const char *old_version,
					const char *cur_version,
					bool *node_deleted);

    virtual bool         updateParmsFlags();
    virtual void	 getDescriptiveParmName(UT_String &name) const;

private:
    int			 PARMOP(const char *parmname);

    bool		 getParmsToModify(UT_IntArray &parmstomodify,
				const SIM_Data *existingdata,
				bool existingdataiscompatibletype);
    CH_LocalVariable	*getVariableById(int index) const;
    void		 setParametersOnData(SIM_Data *data,
				DOP_Engine &engine,
				const UT_IntArray &parmstomodify,
				double time);
    void		 createData(SIM_RootData &rootdata,
				const UT_String &dataname,
				const char *datatype,
				const char *lcddatatype,
				DOP_Engine &engine,
				double time,
				int outputidx, 
				bool active);

    const SIM_DopDescription *getDopDescription() const;

    const SIM_Data	*myCurrentData;
    const SIM_Engine	*myCurrentEngine;
    SIM_Options		 myOptions;
    SIM_RootDataId	 myLastCreatedDataId;
    UT_String		 myLastCreatedDataName;
    int			 myLastInput;

protected:
    OP_VERSION		 myLastVersionParms;
    bool		 myTimeDep;
};

#endif
