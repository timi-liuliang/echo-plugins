/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP Library (C++)
 *
 */

#ifndef __VOP_Block_h__
#define __VOP_Block_h__

#include <UT/UT_Set.h>

#include "VOP_API.h"
#include "VOP_Node.h"

class VOP_API VOP_Block : public VOP_Node
{
public:
    static PRM_Template		 myTemplateListOpen[];
    static PRM_Template		 myTemplateListClose[];

    virtual void		 getCode(UT_String &codestr,
                		         const VOP_CodeGenContext &context);

    virtual const char		*inputLabel(unsigned idx) const;
    virtual const char		*outputLabel(unsigned idx) const;

    virtual const VOP_Block *castToBlock() const { return this; }
    virtual VOP_Block *castToBlock() { return this; }

    virtual unsigned		 getNumVisibleInputs() const;
    virtual unsigned		 getNumVisibleOutputs() const;
    virtual unsigned	 	 orderedInputs() const;

    virtual unsigned		 getBuiltInInputs() const = 0;

    /// Convenience method to get the paired block node
    VOP_Node			*getPairedNodeRaw() const;

    /// Only returns non-null if it is a valid pair.
    VOP_Block			*getPairedNode() const
    { UT_StringHolder tmp; return getPairedNode(tmp); }
    VOP_Block			*getPairedNode(UT_StringHolder &error) const;

    bool			 isOpenBlock() const
				 { return myIsOpenBlock; }
    bool			 isCloseBlock() const
				 { return !isOpenBlock(); }
    virtual bool		 isLoop() const = 0;
    virtual bool		 isConditional() const = 0;

    virtual void		 getOpenCode(UT_String &codestr,
					const VOP_CodeGenContext &context) = 0;
    virtual void		 getCloseCode(UT_String &codestr,
					const VOP_CodeGenContext &context) = 0;

    /// Creates a set of all the nodes inside of our open/close pair.
    /// Returns true if done with no errors.
    bool			 findEnclosedItems(
					OP_EnclosedItems &items,
					UT_StringHolder &error) const;
    bool			 findEnclosedItems(
					OP_EnclosedItems &items) const
    { UT_StringHolder tmp; return findEnclosedItems(items, tmp); }

    /// Given a set of enclosed nodes, likely from the findEnclosedItems,
    /// generates a set of all nodes that are inputs & are not themselves
    /// enclosed
    void			 findAllBlockInputNodes(
					OP_EnclosedItems &inputnodes,
					const OP_EnclosedItems &enclosed) const;

    // Returns true if the begin/end pairs are copacetic.
    bool			 validatePairing(UT_StringHolder &error) const;

    // We enforce that blocked pairs have the same color
    virtual bool	    	 setColor(const UT_Color &col);

protected:
    VOP_Block(OP_Network *parent, const char *name,
	      OP_Operator *entry, bool isopen);
    virtual ~VOP_Block();

    virtual bool		 generateErrorsSubclass();

    virtual UT_StringHolder	 builtinVARNAME(int idx) const = 0;
    virtual void		 getBuiltInTypeInfo(VOP_TypeInfo &type_info,
					int idx) = 0;

    // Called on the open node, asks if the given close node
    // is a valid pair.
    virtual bool		 validatePairingSubclass(
					const VOP_Block *close) const = 0;

    virtual void	 postOpChanged(OP_EventType reason, void *data);

    virtual void	 getInputNameSubclass(UT_String &in, int idx) const;
    virtual int		 getInputFromNameSubclass(const UT_String &in) const;
    virtual void	 getInputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						  int idx);

    virtual void	 getOutputNameSubclass(UT_String &out, int idx) const;
    virtual void	 getOutputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						   int idx);

    virtual int		 NUMPARMS() const;
    UT_StringHolder	 VARNAME(int idx) const;
    UT_StringHolder	 VARLABEL(int idx) const;

private:
    // Set by constructor, implied by the type, so does not
    // have to be saved.
    bool			myIsOpenBlock;

    // These are for our variables *after* the built ins.
    mutable OP_VERSION		myVarNameCacheParms;
    mutable UT_StringArray	myVarNameCache;
    mutable bool		myFindingEnclosed;
};

class VOP_API VOP_BlockWhile : public VOP_Block
{
public:
    static OP_Node		*myConstructorClose(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    virtual unsigned		 getBuiltInInputs() const { return 1; }
    virtual UT_StringHolder	 builtinVARNAME(int idx) const
    {
	UT_StringHolder		result;
	result.reference("condition");
	return result;
    }
    virtual void	 getBuiltInTypeInfo(VOP_TypeInfo &type_info, int idx)
    {
	type_info.setType(VOP_TYPE_INTEGER);
    }
    virtual bool	 validatePairingSubclass(const VOP_Block *close) const
    {
	return false;
    }
    virtual bool	 isLoop() const { return true; }
    virtual bool	 isConditional() const { return false; }

    virtual void	 getOpenCode(UT_String &codestr,
				const VOP_CodeGenContext &context);
    virtual void	 getCloseCode(UT_String &codestr,
				const VOP_CodeGenContext &context);

protected:
    VOP_BlockWhile(OP_Network *parent, const char *name,
		   OP_Operator *entry, bool isopen)
	: VOP_Block(parent, name, entry, isopen)
    { }
};

class VOP_API VOP_BlockNone : public VOP_Block
{
public:
    static OP_Node		*myConstructorOpen(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static OP_Node		*myConstructorClose(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    virtual unsigned		 getBuiltInInputs() const { return 0; }
    virtual UT_StringHolder	 builtinVARNAME(int idx) const
    {
	UT_StringHolder		result;
	return result;
    }
    virtual void	 getBuiltInTypeInfo(VOP_TypeInfo &type_info, int idx)
    {
    }
    virtual bool	 validatePairingSubclass(const VOP_Block *close) const
    {
	if (dynamic_cast<const VOP_BlockWhile *>(close) ||
	    dynamic_cast<const VOP_BlockNone *>(close))
	{
	    return true;
	}
	return false;
    }
    virtual bool	 isLoop() const { return false; }
    virtual bool	 isConditional() const { return false; }

    virtual void	 getOpenCode(UT_String &codestr,
				const VOP_CodeGenContext &context);
    virtual void	 getCloseCode(UT_String &codestr,
				const VOP_CodeGenContext &context);

protected:
    VOP_BlockNone(OP_Network *parent, const char *name,
		  OP_Operator *entry, bool isopen)
	: VOP_Block(parent, name, entry, isopen)
    { }
};

class VOP_API VOP_BlockIf : public VOP_Block
{
public:
    static OP_Node		*myConstructorOpen(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    virtual unsigned		 getBuiltInInputs() const { return 1; }
    virtual UT_StringHolder	 builtinVARNAME(int idx) const
    {
	UT_StringHolder		result;
	result.reference("condition");
	return result;
    }
    virtual void	 getBuiltInTypeInfo(VOP_TypeInfo &type_info, int idx)
    {
	type_info.setType(VOP_TYPE_INTEGER);
    }
    virtual bool	 validatePairingSubclass(const VOP_Block *close) const
    {
	if (dynamic_cast<const VOP_BlockWhile *>(close) ||
	    dynamic_cast<const VOP_BlockNone *>(close))
	{
	    return true;
	}
	return false;
    }
    virtual bool	 isLoop() const { return false; }
    virtual bool	 isConditional() const { return true; }

    virtual void	 getOpenCode(UT_String &codestr,
				const VOP_CodeGenContext &context);
    virtual void	 getCloseCode(UT_String &codestr,
				const VOP_CodeGenContext &context);

protected:
    VOP_BlockIf(OP_Network *parent, const char *name,
		OP_Operator *entry, bool isopen)
	: VOP_Block(parent, name, entry, isopen)
    { }
};

class VOP_API VOP_BlockBreakIf : public VOP_Block
{
public:
    static OP_Node		*myConstructorClose(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    virtual unsigned		 getBuiltInInputs() const { return 1; }
    virtual UT_StringHolder	 builtinVARNAME(int idx) const
    {
	UT_StringHolder		result;
	result.reference("condition");
	return result;
    }
    virtual void	 getBuiltInTypeInfo(VOP_TypeInfo &type_info, int idx)
    {
	type_info.setType(VOP_TYPE_INTEGER);
    }
    virtual bool	 validatePairingSubclass(const VOP_Block *close) const
    {
	return false;
    }
    virtual bool	 isLoop() const { return false; }
    virtual bool	 isConditional() const { return true; }

    virtual void	 getOpenCode(UT_String &codestr,
				const VOP_CodeGenContext &context);
    virtual void	 getCloseCode(UT_String &codestr,
				const VOP_CodeGenContext &context);

protected:
    VOP_BlockBreakIf(OP_Network *parent, const char *name,
		     OP_Operator *entry, bool isopen)
	: VOP_Block(parent, name, entry, isopen)
    { }
};


class VOP_API VOP_BlockFor : public VOP_Block
{
public:
    static OP_Node		*myConstructorOpen(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    virtual unsigned		 getBuiltInInputs() const { return 2; }
    virtual UT_StringHolder	 builtinVARNAME(int idx) const
    {
	UT_StringHolder		result;
	if (idx == 1)
	    result.reference("index");
	else
	    result.reference("length");
	return result;
    }
    virtual void	 getBuiltInTypeInfo(VOP_TypeInfo &type_info, int idx)
    {
	type_info.setType(VOP_TYPE_INTEGER);
    }
    virtual bool	 validatePairingSubclass(const VOP_Block *close) const
    {
	if (dynamic_cast<const VOP_BlockBreakIf *>(close) ||
	    dynamic_cast<const VOP_BlockNone *>(close))
	{
	    return true;
	}
	return false;
    }
    virtual bool	 isLoop() const { return true; }
    virtual bool	 isConditional() const { return false; }

    virtual void	 getOpenCode(UT_String &codestr,
				const VOP_CodeGenContext &context);
    virtual void	 getCloseCode(UT_String &codestr,
				const VOP_CodeGenContext &context);

protected:
    VOP_BlockFor(OP_Network *parent, const char *name,
		 OP_Operator *entry, bool isopen)
	: VOP_Block(parent, name, entry, isopen)
    { }
};

class VOP_API VOP_BlockForEach : public VOP_Block
{
public:
    static OP_Node		*myConstructorOpen(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    virtual unsigned		 getBuiltInInputs() const { return 3; }
    virtual UT_StringHolder	 builtinVARNAME(int idx) const
    {
	UT_StringHolder		result;
	switch (idx)
	{
	    case 0:		// Array.
		result.reference("array");
		break;
	    case 1:		// Element.
		result.reference("element");
		break;
	    case 2:		// Index.
		result.reference("index");
		break;
	}
	return result;
    }
    virtual void	 getBuiltInTypeInfo(VOP_TypeInfo &type_info, int idx);
    virtual bool	 validatePairingSubclass(const VOP_Block *close) const
    {
	if (dynamic_cast<const VOP_BlockBreakIf *>(close) ||
	    dynamic_cast<const VOP_BlockNone *>(close))
	{
	    return true;
	}
	return false;
    }
    virtual bool	 isLoop() const { return true; }
    virtual bool	 isConditional() const { return false; }

    virtual void	 getOpenCode(UT_String &codestr,
				const VOP_CodeGenContext &context);
    virtual void	 getCloseCode(UT_String &codestr,
				const VOP_CodeGenContext &context);

protected:
    VOP_BlockForEach(OP_Network *parent, const char *name,
		     OP_Operator *entry, bool isopen)
	: VOP_Block(parent, name, entry, isopen)
    { }
};


#endif
