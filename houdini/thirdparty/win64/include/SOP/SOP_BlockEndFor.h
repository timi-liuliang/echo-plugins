/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_BlockEndFor.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_BlockEndFor_h__
#define __SOP_BlockEndFor_h__

#include "SOP_API.h"
#include "SOP_BlockEnd.h"

class UT_InfoTree;

class sop_PieceExtractor;

class SOP_API SOP_BlockEndFor : public SOP_BlockEnd
{
public:

    class	ForStateIterator
    {
    public:
	ForStateIterator(SOP_BlockEndFor *forsop, SOP_Node *error_sink, fpreal t, GU_DetailHandle iterationgeo);
	ForStateIterator(const ForStateIterator &iter)
	{
	    myPieceExtractor = 0;
	    *this = iter;
	}
	~ForStateIterator();

	ForStateIterator &operator=(const ForStateIterator &iter);

	GU_DetailHandle		getPieceGeo();
	GU_DetailHandle		getFeedbackGeo();
	GU_DetailHandle		getMetadataGeo();
	GU_DetailHandle		getIterationGeo() { return myIterationGeo; }
	exint 			getCurrentCookPass() const { return myCurrentCookPass; }
	// Expected iterations is the number we expect to actually perform
	exint 			getExpectedIterations() const { return myExpectedIterations; }
	// Full Pass iterations is the number prior to singlepass, this
	// can ensure switching to single pass gives same behaviour for
	// stuff that uses numiteration
	exint 			getFullPassIterations() const { return myFullPassIterations; }
	exint 			getStartPass() const { return myStartPass; }
	

	void			setPass(exint pass, GU_DetailHandle feedback);

	bool			hasPieceGeo() const
	{ return !myIterationGeo.isNull() && myPieceExtractor; }

	/// Allows one to reduce the reference count when all inputs
	/// have extracted the feedback for inplace operations.
	void			clearFeedbackGeo() 
	{ 
	    if (!myFeedbackGeo.isNull())
		myFeedbackGeo.removePreserveRequest();
	    myFeedbackGeo.clear(); 
	}

	/// Erases the piece extractor as it holds hard references
	/// to geometry
	void			clearPieceExtractor();

    protected:
	/// myFeedbackGeo is the last fed-back version of the geometry
	/// going through the system.
	GU_DetailHandle	 myFeedbackGeo;

	/// myIterationGeo is the second input, it is kept here so
	/// we can extract the pieces just-in-time and unlock the
	/// second input.
	GU_DetailHandle	 myIterationGeo;

	/// The most recently extracted piece of the myIterationGeo.  We
	/// cache it in case there are multiple next sops grabbing it, or
	/// for when we are done and the user is manually cooking the chain.
	GU_DetailHandle	 myPieceGeo;

	/// Likewise, the cached metadata.
	GU_DetailHandle	 myMetadataGeo;

	/// Functor object to extract a subset of the iteration geo.
	sop_PieceExtractor	 *myPieceExtractor;

	/// Used to dynamically recompute piece geo and provide
	/// info.
	exint		 myCurrentCookPass;
	exint		 myStartPass;
	exint		 myExpectedIterations;
	exint		 myFullPassIterations;

	bool		 myUseTemplate;
	fpreal		 myStartValue, myIncrement;

	bool		 myUseAttrib;
	UT_StringHolder	 myAttrib;
	GA_AttributeOwner myClass;
    };

	     SOP_BlockEndFor(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_BlockEndFor();

    virtual bool		 updateParmsFlags();

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static PRM_Template		 myTemplateList[];

    virtual void		 getNodeSpecificInfoText(OP_Context &context,
					OP_NodeInfoParms &iparms);

    // Fill in tree with this node's details
    virtual void		 fillInfoTreeNodeSpecific(UT_InfoTree &tree, 
					const OP_NodeInfoTreeParms &parms);

    // Special methods used to get the feedback geometry from Next
    // SOPs.
    virtual bool		 hasFeedbackGeo() const;
    virtual GU_DetailHandle	 getFeedbackGeo() const;

    virtual bool		 hasPieceGeo() const;
    virtual GU_DetailHandle	 getPieceGeo();

    // Force the for sop to cook its iteration input, if possible.
    // (will respect the debug setting)
    // Afterwards hasPieceGeo() is likely valid.  You will get recursion
    // issues if you have a Next in the second input, but you are
    // doing something wrong there regardless.
    virtual bool		 cookPieceGeo(OP_Context &context, SOP_Node *caller);

    virtual GU_DetailHandle	 cookMetadata(OP_Context &context, SOP_Node *caller);

    // Registers all each sops that want to be told if the feedback
    // geo changes.
    virtual void		 registerEachSOP(SOP_Node *sop);
    virtual void		 unregisterEachSOP(SOP_Node *sop);

    exint 			 getCurrentCookPass() const { if (myForState) return myForState->getCurrentCookPass(); return -1; }
    exint			 getExpectedIterations() const { if (myForState) return myForState->getExpectedIterations();  return 0; }

    // Resets our current cook pass to the default values before
    // the SOP cooked the first time.
    static int			 resetCookPassStatic(void *op, int idx, fpreal t,
					const PRM_Template *);
    void			 resetCookPass(fpreal t);

    /// Finds the template sop, either what is wired to second
    /// input or to the linked Begin.
    SOP_Node			*getTemplateSOP(fpreal t) const;
    /// Finds the feedback sop, what is wired to the primary input
    SOP_Node			*getFeedbackSOP(fpreal t) const;

    static sop_PieceExtractor *buildPieceExtractor(SOP_BlockEndFor *node,
						   SOP_Node *error_sink,
						   fpreal t,
						   GU_DetailHandle iterationgeo,
						   exint &niterations);

    // Should all inputs be treated as enclosed?
    virtual bool		 encloseInputs() const { return false; }

protected:
    virtual OP_ERROR	 cookMySop(OP_Context &context);

    virtual const char	*inputLabel(unsigned idx) const;
    virtual void	 getDescriptiveName(UT_String &name) const;

    virtual void	 dirtyAllEachSOPs();


    /// Rebuilds our myPieceExtractor from our relevant parameters &
    /// from the current myIterationGeo.  Returns number of items.
    exint		 rebuildPieceExtractor(OP_Context &context);

public:
    int		 	 ITERATIONMETHOD(fpreal t) const
			 { return evalInt("itermethod", 0, t); }
    int		 	 FORMETHOD(fpreal t) const
			 { return evalInt("method", 0, t); }
    exint	 	 ITERATIONS(fpreal t) const
			 { return evalInt("iterations", 0, t); }
    void		 ATTRIB(UT_String &str, fpreal t) const
			 { evalString(str, "attrib", 0, t); }
    bool		 USEMAXITER(fpreal t) const
			 { return evalInt("usemaxiter", 0, t); }
    bool		 USEATTRIB(fpreal t) const
			 { return evalInt("useattrib", 0, t); }
    int			 MAXITER(fpreal t) const
			 { return evalInt("maxiter", 0, t); }
    void		 PRIMARYNEXT(UT_String &str, fpreal t) const
			 { evalString(str, "blockpath", 0, t); }
    void		 TEMPLATEPATH(UT_String &str, fpreal t) const
			 { evalString(str, "templatepath", 0, t); }
    bool		 DOSINGLEPASS(fpreal t) const
			 { return evalInt("dosinglepass", 0, t); }
    int			 SINGLEPASS(fpreal t) const
			 { return evalInt("singlepass", 0, t); }
    bool		 STOPCONDITION(fpreal t) const
			 { return evalInt("stopcondition", 0, t); }
    bool		 MULTITHREAD(fpreal t) const
			 { return evalInt("multithread", 0, t); }

    virtual void	 PRIMARYPATH(UT_String &str, fpreal t) const
			 { str = ""; }

protected:
    UT_Set<int>		 myNextSOPList;
    ForStateIterator	*myForState;

private:
};

#endif

