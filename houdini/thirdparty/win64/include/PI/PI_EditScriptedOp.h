/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PI_EditScriptedOp.h ( OP Library, C++)
 *
 * COMMENTS:	Provides a data structure for holding all the information
 * 		required to edit the various facets of a PI_ScriptOperator.
 * 		Also contains code to write a .ds file given just a pointer
 * 		to a OP_ScriptOperator.
 */

#ifndef __PI_EditScriptedOp__
#define __PI_EditScriptedOp__

#include "PI_API.h"
#include <UT/UT_String.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_SymbolTable.h>
#include <iosfwd>
#include "PI_EditScriptedParms.h"

class OP_ScriptOperator;
class OP_SpecificData;

class PI_API PI_EditScriptedOpHandle
{
public:
    PI_EditScriptedOpHandle(const char *type);
    PI_EditScriptedOpHandle(const PI_EditScriptedOpHandle &src);
    ~PI_EditScriptedOpHandle();
	
    int			 save(std::ostream &os);
    void		 addPIName(const char *name);
    int			 bindChannel(const char *piname, const char *chname);
    bool		 hasBinding() const;

    UT_String		 myName;
    UT_String		 myType;

    // Only used by type handles...
    UT_String		 myDefault;

    // Only used by persistant handles...
    UT_String		 myOrigName;
    
    // Our binding array...
    UT_StringArray	 myPINames;
    UT_StringArray	 myChannelNames;

    static const UT_StringArray &getValidTypes();
};

class PI_API PI_EditScriptedOp
{
public:
    PI_EditScriptedOp(OP_ScriptOperator *op, OP_Node *node);
    PI_EditScriptedOp(const PI_EditScriptedOp &source);
    virtual ~PI_EditScriptedOp();

    // Save the entire operator definition to it's current OTL.
    int				 save(OP_SpecificData *opspecific = 0);
    // Save the dialog script to a stream.
    int				 save(std::ostream &os,
				      OP_SpecificData *opspecific = 0);

    // Access our parm information.
    PI_EditScriptedParms	&getParms();
    const PI_EditScriptedParms	&getParms() const;

    // Get handle binding information.
    int				 getNHandles() const;
    PI_EditScriptedOpHandle	*getHandle(int i);

    // Add, remove, and reorder handles.
    void			 addHandle(PI_EditScriptedOpHandle *handle);
    bool			 moveHandles(int first, int last, int offset);
    bool			 removeHandles(int first, int last);

    // Set the basic information about the operator type.
    void			 setLabel(const char *newlabel)
				 { myLabel.harden(newlabel); }
    void			 setName(const char *newname)
				 { myName.harden(newname); }
    void			 setScript(const char *newscript)
				 { myScript.harden(newscript); }
    void			 setInputName(int idx, const char *name);
    void			 setInputLabel(int idx, const char *label);
    void			 setInputIsReference(int idx, bool isref);
    void			 setOutputLabel(int idx, const char *label);
    void			 setUnitLength(fpreal l) { myUnitLength = l; }
    void			 setUnitMass(fpreal l) { myUnitMass = l; }

    // Get some basic information about the operator type.
    int				 getShopFlag() const	{ return myShopFlag; }
    int				 getVopFlag() const	{ return myVopFlag; }
    int				 getObjFlag() const	{ return myObjFlag; }
    const char			*getInputName(int idx) const;
    const char			*getInputLabel(int idx) const;
    bool			 getInputIsReference(int idx) const;
    bool			 allowsInputNameEditing() const
					    { return myAllowsInputNameEditing; }
    const char			*getOutputLabel(int idx) const;
    fpreal			 getUnitLength() const	{ return myUnitLength; }
    fpreal			 getUnitMass() const	{ return myUnitMass; }

    const OP_ScriptOperator	*getOp() const  { return myOp; }

    // Handle our warning string.
    void			 clearWarning()		{ myWarning = ""; }
    const UT_String		&getWarning() const	{ return myWarning; }

    // Save a block of data into the stream in a format suitable for a
    // dialog script file. This means that the string is written as a
    // series of quoted strings which when concatenated together recreate
    // the original string.
    static void			 saveBlock(std::ostream &os, const char *block,
					   const char *indent = "\t");
   
private:
    int				 saveUnboundSelectors(std::ostream &os);
    int				 saveReservedSelectors(std::ostream &os);
    void			 saveHelp(std::ostream &os);

    OP_ScriptOperator				*myOp;
    PI_EditScriptedParms			 myParms;
    UT_ValArray<PI_EditScriptedOpHandle *>	 myHandles;

    // These guys are copies of the Op's value.  Note this is what
    // is used for saving, NOT the op's value, so adjust them
    // to save properly.  (You may want to change the Op value too
    // so everyone is synched nicely.)
    UT_String					 myName;
    UT_String					 myScript;
    UT_String					 myLabel;
    UT_String					 myHelp;
    int						 myShopFlag;
    int						 myVopFlag;
    int						 myObjFlag;
    UT_String					 myWarning;
    UT_StringArray				 myInputNames;
    UT_StringArray				 myInputLabels;
    UT_Array<bool>				 myInputReferences;
    UT_StringArray				 myOutputLabels;
    bool					 myAllowsInputNameEditing;
    fpreal					 myUnitLength;
    fpreal					 myUnitMass;
};

#endif

