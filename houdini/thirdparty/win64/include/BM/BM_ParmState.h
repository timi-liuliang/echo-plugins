/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BM_ParmState.h ( Direct manipulation library, C++)
 *
 * COMMENTS:
 *	The base class of a BM state/handle that supports parameters.
 */

#ifndef __BM_ParmState_H__
#define __BM_ParmState_H__

#include "BM_API.h"
#include <UT/UT_String.h>
#include "BM_SimpleState.h"
#include <CH/CH_ExprLanguage.h>


class PRM_Template;
class PRM_ParmList;
class PRM_Parm;
class PRM_PresetInfo;
class PSI2_DialogPRM;
class PSI2_DialogPRMExported;
class BM_ParmStateUndo;


class BM_API BM_ParmState : public BM_SimpleState
{
    // Rather than putting the undo code in this file, it's in a friend class.
    // BM_ParmStateUndo needs access to parameter() method to save and
    // load its value.
    friend class BM_ParmStateUndo;
	
public:
    // Class constructor and destructor. 'name' is the name of the state.
    BM_ParmState(BM_SceneManager &app, const char *cursor,
		 const char		*vname = 0,
		 const char		*english = 0,
		 const char	*const *vnames = 0,
		 UI_EventMethod  const *vmethods = 0);
    virtual ~BM_ParmState();

    // Query the name of the state:
    const UT_String	&name(void) const	{ return myName; }
    const UT_String	&english(void) const	{ return myEnglishName; }

    // The name of this class:
    virtual const char	*className() const;

    // In case you need to know what the dialog is:
    virtual void	 prepareDialogForDisplay() const;
    virtual int		 hasDialog (void) const;
    virtual UI_Feel	*dialogFeel(void) const;
    virtual int		 getToolboxCount() const;
    virtual UI_Feel	*getToolbox(int index) const;

    // This gets the extra copy of the dialog feel.  We need this as handle's
    // dialogs are in two places, and our UI library can't mulitparent.
    // Note that this is only non-null if isHandle is 1.
    UI_Feel		*extraDialogFeel() const;

    // Initialize the app, wire the interface, tie all the UI values, etc:
    // This method should only be called by the BM_ResourceManager when the
    // state is created.
    virtual void	 initializeUI(PRM_Template *tmpl=0);


    virtual bool getStateParmNames(UT_StringArray &ret, const char* prefix=nullptr );
    virtual bool evalStateParm(const char *name, UT_StringHolder &ret);
    virtual bool setStateParm(const char *name, const UT_StringHolder &val);
    virtual bool pressStateButton(const char *name);

protected:
    // Returns a preset type name to be used when saving preset info and dialogs.
    virtual const char	*getPresetTypeName() const = 0; 
    
    // Do the AP_Interface thing (called by initializeUI()):
    virtual void	 initApplication(UI_Manager *, int c, const char **v);

    // Return various things needed for the interaction with the dialog:
    const PRM_Parm	*parameter(const char *name) const;
    PRM_Parm		*parameter(const char *name);
    PRM_ParmList	*parameters (void) const  { return myParmList;    }
    UI_Value		*parmValue  (void) const  { return myParmVal;     }

    // Figure out the parameter given its name or index in the parm list:
    const PRM_Parm	*parameter(int index) const;
    PRM_Parm		*parameter(int index);

    // Get or set the values of various types of PRM parameters:
    void		 setParm(const char *name, const UT_String &value,
				 CH_StringMeaning meaning,
				 fpreal t=0, int refresh=1);
    void		 setParm(const char *name, int   value, fpreal t=0,
				 int refresh=1);
    void		 setParm(const char *name, fpreal value, fpreal t=0,
				 int refresh=1);
    void		 setXYParm (const char *name,
				    fpreal x,fpreal y, 
				    fpreal t=0, int refresh=1);
    void		 setXYZParm(const char *name,
				    fpreal x, fpreal y, fpreal z,
				    fpreal t=0, int refresh=1);
    void		 setParm(int index, const UT_String &v,
				 CH_StringMeaning meaning,
				 fpreal t=0, int refresh=1);
    void		 setParm(int index, int value, fpreal t=0,
				 int refresh=1);
    void		 setParm(int index, fpreal value, fpreal t=0,
				 int refresh=1);
    void		 setXYParm (int index, fpreal x, fpreal y,fpreal t=0,
				    int refresh=1);
    void		 setXYZParm(int index, fpreal x, fpreal y, fpreal z,
					     	     fpreal t=0, int refresh=1);

    bool		 parmExists( const char *name ) const;
    bool		 parmExists( int index ) const;

    void		 getParm(const char *name, UT_String &value,
				    fpreal t=0) const;
    void		 getParm(const char *name, int   &value,
				    fpreal t=0) const;
    void		 getParm(const char *name, bool &value,
				    fpreal t=0) const
			 {
			     int v;
			     getParm(name, v, t);
			     value = (v != 0);
			 }
    void		 getParm(const char *name, fpreal &value,
				 fpreal t=0) const;
    void		 getXYParm (const char *name, fpreal &x, fpreal &y, 
				    fpreal t=0) const;
    void		 getXYParm (const char *name, int &x, int &y, 
				    fpreal t=0) const;
    void		 getXYZParm(const char *n,
				    fpreal &x, fpreal &y, fpreal &z, 
				    fpreal t=0) const;
    void		 getParm(int index, UT_String &value, fpreal t=0) const;
    void		 getParm(int index, int   &value, fpreal t=0) const;
    void		 getParm(int index, bool  &value, fpreal t=0) const
			 {
			     int v;
			     getParm(index, v, t);
			     value = (v != 0);
			 }
    void		 getParm(int index, fpreal &value, fpreal t=0) const;
    void		 getXYParm (int index, fpreal &x, fpreal &y,
				    fpreal t=0) const;
    void		 getXYParm (int index, int &x, int &y, fpreal t=0) const;
    void		 getXYZParm(int index,
				    fpreal &x, fpreal &y, fpreal &z, 
				    fpreal t=0) const;

    // Overwrite the defaults of the given parm:
    void		 overwriteParmDefault   (const char *name, fpreal t=0);
    void		 overwriteXYParmDefault (const char *name, fpreal t=0);
    void		 overwriteXYZParmDefault(const char *name, fpreal t=0);
    void		 overwriteParmDefault   (int index, fpreal t=0);
    void		 overwriteXYParmDefault (int index, fpreal t=0);
    void		 overwriteXYZParmDefault(int index, fpreal t=0);
    void		 overwriteParmDefaults();

    // Enable or disable parameters: Return 0 if nothing has changed.
    virtual int		 disableParms(void);
    int			 enableParm(const char *name, int onoff, int refresh=1);
    int			 enableParm(int index, int onoff, int refresh=1);

    // Show/hide parmeters. Returns true if changed.
    bool		 setVisibleState(int pi, bool state, bool refresh=true);
    bool		 setVisibleState(const char *pn, bool state,
					 bool refresh=true);

    // Refresh all the parms if the dialog exists, or just one parm:
    void		 refreshParms(void);
    void		 refreshParm (int index);

    // callback that reacts to changes in dialog values (after 
    // the parameter has been changed)
    virtual int		 handleParameterEvent(UI_Event *event);
	
    // save the parameter data for undo/redo if the mySaveParmForUndoFlag
    // is turned on or the force parameter is true. Returns true
    // if parm was saved for undo, or false otherwise (e.g., if undo
    // manager is not accepting for undo)
    bool		 saveParmForUndo( int index, bool force = false );

    // this method is invoked before a parameter is changed by a dialog.
    // This method was added, because the parameter could not be saved for
    // undo, before the parameter was changed. This method is invoked
    // before a parameter is changed by objects other than derived
    // from BM_ParmState
    virtual int		 handleSaveParmForUndo( int index );
    
    // is called after a parameter has changed the value due to 
    // the execution of an undo for that parameter
    virtual int		 handleParameterUndo( int parm_index );
    
    // access and modify the flag that determines whether parameter
    // gets saved for undo before it is modified
    int			 getSaveParmForUndoFlag() 
			    { return mySaveParmForUndoFlag; };
    void		 setSaveParmForUndoFlag( int onoff)
			    { mySaveParmForUndoFlag = (onoff != 0); };
    
private:
    // static method registered with the dialog and used by the dialog
    // to save parameter for und before the parameter is modified.
    // This method invokes virtual handleSaveParmForUndo(), so that
    // the derived classes can decide whether to save the parameter
    // for undo or not.
    static void		 saveParmForUndoCallback( void * state,
						  int parm_index )
			    {
				((BM_ParmState *)state)->handleSaveParmForUndo(
								parm_index );
			    }
    
    void		 handleParameterEventVoidRet(UI_Event *event)
					    { handleParameterEvent(event); }
    void		 handleToolboxParameterEvent(UI_Event *event);
    void		 parameterChanged(UI_Event *event);
    
    // data

    UT_String		 myName; 	// name 
    UT_String		 myEnglishName; // name in English

    PSI2_DialogPRMExported *myParmToolbox;
    PRM_ParmList	*myParmList;	// the parms inside the dialog
    UI_Value		*myParmVal;	// value to communicate with parms
    UI_Value		*myToolboxParmVal;
    PSI2_DialogPRM	*myParmDialog;	// the dialog that holds the parms
    PSI2_DialogPRM	*myExtraParmDialog; // an extra copy of that dialog.
    PRM_PresetInfo	*myPresetInfo;

    unsigned		 myOwnParmsFlag:1,  // own its parm list and dialog?
			 mySaveParmForUndoFlag:1; //should save parm for undo?
};

#endif
