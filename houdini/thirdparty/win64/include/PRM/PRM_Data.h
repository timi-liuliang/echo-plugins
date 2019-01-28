/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Data.h (Parameter Library)
 *
 * COMMENTS:
 *		PRM_Data contains the variable part of a Data
 *		parameter.
 *
 */

#ifndef __PRM_Data__
#define __PRM_Data__

#include "PRM_API.h"
#include "PRM_Default.h"
#include "PRM_Instance.h"
#include "PRM_KeySetType.h"
#include "PRM_Lock.h"
#include "PRM_Parm.h"
#include <CH/CH_ExprLanguage.h>
#include <CH/CH_Types.h>
#include <UT/UT_Array.h>
#include <SYS/SYS_Types.h>

#include <iostream>


class PRM_ParmList;
class PRM_Template;
class CH_Channel;
class CH_Collection;
class CH_Manager;
class UT_IStream;
class UT_JSONValueMap;
class UT_String;
class UT_StringWrap;
class UT_TokenString;


class PRM_API PRM_Data : public PRM_Instance
{
public:
			  PRM_Data(PRM_Template *thetemplateptr,
				     PRM_Parm *parm);
			  PRM_Data(PRM_Data *thesrcptr,
				     PRM_Parm *parm);
    virtual		 ~PRM_Data();
    virtual void	  adopt(PRM_Instance &thesrc);
    virtual PRM_Instance *duplicateSelf(PRM_Parm *parm);

    virtual void	  setValueToDefault(fpreal time, int index,
					    PRM_AddKeyType add_key);
    virtual void	  revertToDefault(int index);
    virtual void	  revertToDefaults(bool clear_channels = true);
    virtual void	  restoreFactoryDefault(int index);
    virtual void	  overwriteDefault(fpreal time, int index);
    virtual void	  overwriteDefaults(fpreal time);
    virtual int		  isDefault(int index, enum PRM_DefaultType deftype,
				    bool basiccheck = true) const;
    virtual void          save(std::ostream &os, int binary, bool compiled) const;
    virtual bool          load(UT_IStream &is);

    virtual void	  saveUndoData(PRM_UndoDataHandle &data) const;
    virtual bool	  loadUndoData(const PRM_UndoDataHandle &data);

    virtual void	  saveCommand(std::ostream &os, int values,
				      int index=-1) const;
    virtual int		  loadCommand(int argc, const char *const argv[],
               		              int index, bool values_only, 
               		              PRM_AddKeyType add_key,
               		              PRM_KeySetType set_type);

    virtual void	  getValue(PRM_ParmList *thelist, fpreal thetime,
				   int32 &theintref, int theindex,
				   int thread) const;
    virtual void	  getValue(PRM_ParmList *thelist, fpreal thetime,
				   int64 &theintref, int theindex,
				   int thread) const;
    virtual void	  getValue(PRM_ParmList *thelist, fpreal thetime,
				   fpreal &thefloatref, int theindex,
				   int thread) const;
    virtual void	  getValue(PRM_ParmList *thelist, fpreal thetime,
				   UT_String &thestrref, int theindex,
				   bool expand, int thread) const;
    virtual void	  getValue(PRM_ParmList *thelist, fpreal thetime,
				   PRM_DataItemHandle &thedataref, int theindex,
				   int thread) const;
    virtual void	  getValue(PRM_ParmList *thelist, fpreal time,
				   UT_JSONValueMap &themap, int index,
				   bool expand, int thread) const;

    // set value of a parameter
    //   setkey - determines how the value is set at keyframes. Whether the
    //            preference is used or always set as pending.
    virtual void	  setValue(fpreal time,
				   const char *value,
				   CH_StringMeaning meaning,
				   bool kill_expr = false,
				   int index = 0,
				   PRM_AddKeyType add_key = PRM_AK_MARK_PENDING,
				   PRM_KeySetType setkey = PRM_USE_PREFERENCES);

    virtual void	  setValue(fpreal time,
				   fpreal value,
				   bool kill_expr = false,
				   int index = 0,
				   PRM_AddKeyType add_key = PRM_AK_MARK_PENDING,
				   PRM_KeySetType setkey = PRM_USE_PREFERENCES);

    virtual void	  setValue(fpreal time,
				   int32 value,
				   bool kill_expr = false,
				   int index = 0,
				   PRM_AddKeyType add_key = PRM_AK_MARK_PENDING,
				   PRM_KeySetType setkey = PRM_USE_PREFERENCES);

    virtual void	  setValue(fpreal time,
				   int64 value,
				   bool kill_expr = false,
				   int index = 0,
				   PRM_AddKeyType add_key = PRM_AK_MARK_PENDING,
				   PRM_KeySetType setkey = PRM_USE_PREFERENCES);

    virtual void	  setValue(fpreal time,
				   const PRM_DataItemHandle &value,
				   bool kill_expr = false,
				   const int index = 0,
				   PRM_AddKeyType add_key = PRM_AK_MARK_PENDING,
				   PRM_KeySetType setkey = PRM_USE_PREFERENCES);

    virtual void	  getExpressionStringForUI(fpreal thetime,
						   UT_String &thestrref,
						   int theindex,
						   int thread) const;
    virtual void	  getExpression(fpreal thetime, UT_String &expr,
					int theindex, bool allow_value,
					int thread) const;
    virtual void	  setExpression(fpreal time,
					const char *expr,
					CH_ExprLanguage language,
					int index, bool evaluate = true,
					bool rmchannel = false);
    virtual void	  setDefaultChannelValue(CH_Channel *channelptr,
						 int theindex = 0);
    virtual bool	  getMagicString(UT_TokenString &magic,
					 PRM_ParmList *list, int size,
					 fpreal t, bool add_if_default /*=true*/,
					 int thread);
    //
    //  adds a keyframe if a change is pending
    //
    virtual void          hardenChanges(fpreal thetime,
					int theindex,
					bool theforceflag,
					const char *patt = 0,
					CH_ChannelRefList *list = 0);

    virtual int64	  getMemoryUsage(bool inclusive) const;

    bool setData(const PRM_DataItemHandle &b, fpreal time, int index);
    PRM_DataItemHandle getData(fpreal time, int index) const;

private:
    void		  revertToDefaultWithoutRemovingChannel(
			    fpreal time, int index);

    /// @{
    /// Helper methods for returning key-value dictionary (JSON map) values.
    void		  expandJSONMap(
			    PRM_ParmList *thelist, fpreal time, int index, 
			    int thread, const UT_JSONValueMap &src_map,
			    UT_JSONValueMap &expanded_map)
			    const;
    bool		  expandJSONString(
			    CH_Manager *cman, CH_Collection *collection,
			    const PRM_ParmList *thelist, fpreal time, 
			    int index, int thread, const char *str, 
			    UT_String &expanded_str, 
			    bool &is_time_dependent) const;
    void		  copyJSONMap(
			    const UT_JSONValueMap &src_map, 
			    UT_JSONValueMap &dest_map) const;

    mutable PRM_SpinLock  myLock;
    UT_Array<PRM_DataItemHandle> myData;
};

#endif
