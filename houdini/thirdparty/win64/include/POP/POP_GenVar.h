/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __POP_GenVar_h__
#define __POP_GenVar_h__

#include "POP_API.h"
#include "POP_Node.h"


class UT_String;


/// A subclass of POP_Node that handles many functions common to generators
/// including some common generator variables.  Many generator POPs will
/// derive from this subclass instead of POP_Node.

class POP_API POP_GenVar : public POP_Node
{
public:

    static CH_LocalVariable	myVariables[];
    static OP_VariablePair	myVariablePair;

    static OP_TemplatePair	myTemplatePair;

    enum PrimitiveOrigin
    {
	PRIMITIVE_EDGE = 0,
	PRIMITIVE_SURFACE = 1,
	PRIMITIVE_CENTER = 2
    };

protected:
	     POP_GenVar (OP_Network* net, const char* name, 
		         OP_Operator* entry);
    virtual ~POP_GenVar (void);

    // the birthFrom___ functions birth particles from certain types of 
    // source geometry. 
    //   Points     = birth from points
    //   Primitives = birth from primitives 
    //   MetaPrim   = birth from metaprimitives
    //   Detail     = birth from detail's volume
    //
    //     data			POP_ContextData being used
    //     doVar		update local variables for source point
    //     gdp			detail to retrieve geometry from
    //     group		point or primitive group to use (or NULL)
    //	   xform		transform to apply to birthed particle position
    //				(or NULL)
    //	   xformv		transform to apply to vector attributes
    //				(or NULL)
    //     map			mapping for inheriting attributes.
    //				The map should correspond to the inherit
    //				attribute data if it is set, otherwise, it
    //				should be built for the source gdp.
    //				(or NULL to do no inheriting).
    //     inherit	        attribute data to inherit general attributes
    //				from. if NULL, then general attributes are
    //				inherited from the source geo. 
    // 	   split		attributes from source particle when 
    // 				splitting (or NULL when not splitting).
    //     originIndex  	origin index value.
    //	   originUse	        how to set origin id. Bit mask
    //				    POP_ORIGIN_INDEX: add passed in index
    //				    POP_ORIGIN_GEO: add source geometry number
    //				    POP_ORIGIN_PARENT: add split origin
    //				source geo number:
    //			            point - pt num
    //			            primitives - primitive num
    //			            metaprim - 0
    //			            detail - 0    
    //     born			number of points to birth
    //     part			particle primitive to birth into
    //     birthGroup		group to add birthed points to (or NULL)
    //     lifetime		function that generates a particle lifetime
    //     probability		function that generates a random number
    //				representing the number of particles to birth
    //				per point/primitive.
    //     birthAttrib  	function to set attributes after particle 
    //				is birthed
    //     doRandom		if 1, birth from randomized points. if 0,
    //				birth from points in order
    //     whereFrom   		PRIMITIVE_EDGE, PRIMITIVE_SURFACE, 
    //				or PRIMITIVE_CENTER
    //     checkTrim		if 1, birth from trimmed area only. 
    //				if 0, birth from entire surface.
    //     threshold		for metaprim birthing, points are sampled
    //				and added. Birthin of a particle will occur
    //				when this threshold is passed.
    //     densityMin		for metaprim birthing, particle will not be
    //				birthed unless the density is at least this
    //     intersector		a GU_RayIntersect to perform ray intersections
    //			 	with the detail. It is the caller's 
    //				responsibility to make sure that the 
    //				intersector is initialized.
    //     useDensityProb	use metaball density as a probability
    //				distribution for particle births. Causes
    //				threshold parameter to be ignored.
    //     jitter		function to do jitter births
    //     maxAttempts		failure threshold to tolerate before giving up

    void        birthFromPoints (int doVar, POP_ContextData* data,
				 GU_Detail* gdp, GA_PointGroup* group,
				 const UT_Matrix4* xform,
				 const UT_Matrix3* xformv,
				 POP_AttribMap* map,
			 	 GEO_PointP split,
				 POP_IntFunc originIndex, int originUse,
                                 float* born, GEO_PrimParticle* part,
                                 GA_PointGroup* birthGroup,
				 POP_FloatFunc lifetime,
				 POP_FloatFunc probability,
                                 POP_BirthAttribFunc birthAttrib,
				 int doRandom,
				 POP_JitterFunc jitter = 0,
				 int maxAttempts=100);

    void	birthFromPointVelocity (int doVar, POP_ContextData *data,
				GU_Detail *gdp, GA_PointGroup *group,
				const UT_Matrix4 *xform,
				const UT_Matrix3 *xformv,
				POP_AttribMap *map,
				POP_IntFunc originIndex, int originUse,
				float *born, GEO_PrimParticle *part,
				GA_PointGroup *birthGroup,
				POP_FloatFunc lifetime,
				POP_BirthAttribFunc birthAttrib,
				UT_Vector3 birthVelocity,
				UT_Vector3 birthAcceleration,
				fpreal velocityScale,
				fpreal particleSpacing);

    void        birthFromPrimitives (int doVar, POP_ContextData* data,
				     GU_Detail* gdp, GA_PrimitiveGroup* group,
				     const UT_Matrix4* xform,
				     const UT_Matrix3* xformv,
                                     POP_AttribMap* map,
			 	     GEO_PointP split,
				     POP_IntFunc originIndex, int originUse,
                                     float* born, GEO_PrimParticle* part,
                                     GA_PointGroup* birthGroup,
				     POP_FloatFunc lifetime,
				     POP_FloatFunc probability,
                                     POP_BirthAttribFunc birthAttrib,
				     PrimitiveOrigin whereFrom, int checkTrim,
				     POP_JitterFunc jitter = 0,
				     int doRandom = 0, int maxAttempts = 100);

    void        birthFromMetaPrim  (int doVar, POP_ContextData* data,
                                    GU_Detail* gdp, GA_PrimitiveGroup* group,
				    const UT_Matrix4* xform,
				    const UT_Matrix3* xformv,
				    POP_AttribMap* map,
			 	    GEO_PointP split,
				    POP_IntFunc originIndex, int originUse,
                                    float* born, GEO_PrimParticle* part,
                                    GA_PointGroup* birthGroup,
				    POP_FloatFunc lifetime,
                                    POP_BirthAttribFunc birthAttrib,
				    float threshold, float densityMin,
				    int useDensityProb,
				    POP_JitterFunc jitter = 0,
				    int maxAttempts = 100);

    void        birthFromDetail    (int doVar, POP_ContextData* data,
				    GU_Detail* gdp,
				    const UT_Matrix4* xform,
				    const UT_Matrix3* xformv,
				    POP_AttribMap* map,
			 	    GEO_PointP split,
				    POP_IntFunc originIndex, int originUse,
                                    float* born, GEO_PrimParticle* part,
                                    GA_PointGroup* birthGroup,
				    POP_FloatFunc lifetime,
                                    POP_BirthAttribFunc birthAttrib,
				    GU_RayIntersect* intersector,
				    POP_JitterFunc jitter = 0,
				    int maxAttempts = 100);

    // the birthNonUniformlyFromRandomPrimitives function births particles 
    // from certain types of source geometry in a non-uniform random manner.
    //
    // All parameters common with the birthFrom__ functions have the same
    // meaning.  In addition, this method supports the following additional
    // parameters.
    // 
    //     distribAttrib - name of the primitive attribute which specifies
    //                     the probability distribution for each primitive.
    //                     The values are normalized and then used as the
    //                     probability that this is the random primitive
    //                     selected to birth from.
    //                     If attribute does not exist, a uniform distribution
    //                     is used by default.
    //                     NOTE: if the value of the attribute for a
    //                           primitive is <= 0, then particles will
    //                           not birth off of that primitive.
    //
    void        birthNonUniformlyFromRandomPrimitives (int doVar,
	                         POP_ContextData* data,
                                 GU_Detail* gdp, GA_PrimitiveGroup* group,
			         const UT_Matrix4* xform,
				 const UT_Matrix3* xformv,
                                 POP_AttribMap* map,
			         POP_IntFunc originIndex, int originUse,
			         float* born, GEO_PrimParticle* part,
                                 GA_PointGroup* birthGroup,
                                 POP_FloatFunc lifetime,
                                 POP_FloatFunc probability,
                                 POP_BirthAttribFunc birthAttrib,
			         PrimitiveOrigin whereFrom, int checkTrim,
				 POP_JitterFunc jitter,
				 const UT_String &distribAttrib,
				 int maxAttempts);


    // buildVectorTransform() builds a matrix that transforms vectors.
    // If xform is used to transform points, then xformv can be
    // applied to transform vectors.
    void		buildVectorTransform (UT_Matrix3& xformv,
                                              const UT_Matrix4& xform);

    virtual bool	evalVariableValue(fpreal &v, int index, int thread);
    virtual bool	evalVariableValue(UT_String &v, int i, int thr)
			{ return POP_Node::evalVariableValue(v, i, thr); }

    void		setupVars       (POP_ContextData* data, 
					 GU_Detail* gdp,
					 const GA_ElementGroup* source = 0);

    // Jitter births can change the detail being birthed from, consequently
    // obsoleting the underlying attribute data set up with the setupVars()
    // call.  The hasAttributeVars() and refreshAttributeVars() methods below
    // can be used to keep this data updated.
    bool		hasSourceAttributeVars() const;
    void		refreshSourceAttributeVars(GU_Detail *gdp);

    /// Local variables may require evaluating attributes that would not
    /// otherwise need to be evaluated for birthing purposes.  Call this
    /// method to register those attributes for evaluation with the map.
    void		registerSourceAttributesForEval(POP_AttribMap &map)
									const;

    virtual int		isVarDependent  (GU_Detail* gdp, POP_FParam param,
					 float t, int thread);
    virtual int		isVarDependent  (GU_Detail* gdp, POP_IParam param,
					 float t, int thread);
    virtual int		isVarDependent  (GU_Detail* gdp, POP_SParam param,
					 float t, int thread);
    virtual int		isVarDependent  (GU_Detail* gdp, EV_EXPRESSION* expr,
					 int thread);

    virtual void	allowVar (int val) { myAllowGeneratorVar = val; };
    int			isAllowVar() const { return myAllowGeneratorVar; }

    int			isVarTest() const { return myVarTest; }
    void		setVarDependent(int val) { myVarDependent = val; }

    float	getLastEmissionTime(const GEO_Detail *gdp) const;
    void	getLastEmissionAttrName(UT_String &name) const;
    void	setLastEmissionTime(GEO_Detail *gdp, float time) const;

private:
    class POP_API SourceAttribOffsets 
    {
    public:
	void			 refresh(const GU_Detail *gdp);
	void			 clear();
	bool			 areAnyValid() const;
	void			 registerForEval(POP_AttribMap &map) const;

	GA_ROAttributeRef	 myAlphaOffset;
	GA_ROAttributeRef	 myDiffuseOffset;
	GA_ROAttributeRef	 myDistanceOffset;
	GA_ROAttributeRef	 myDragOffset;
	GA_ROAttributeRef	 myMassOffset;
	GA_ROAttributeRef	 myNormalOffset;
	GA_ROAttributeRef	 myScaleOffset;
	GA_ROAttributeRef	 mySpringKOffset;
	GA_ROAttributeRef	 myTensionOffset;
	GA_ROAttributeRef	 myTextureOffset;
	GA_ROAttributeRef	 myUpOffset;
	GA_ROAttributeRef	 myVelocityOffset;
    };

protected:
    const UT_Vector4	*mySourcePos;	// source position
    const GA_GBElement	*mySourceAttElem;	// Source attributes
    GEO_Point mySourceAttTmpPt;
    GA_GBPrimWrap mySourceTmpPrim;
    const SourceAttribOffsets	*mySourceAttOffsets;
    const GA_GBElement		*mySourceElem;	// source element
    GEO_Point mySourceTmpPt;
    int				 mySourceIter;	// iteration number
    float			 mySourceU;	// source parametric u
    float			 mySourceV;	// source parametric v
    UT_Vector3			*mySourceNml;	// source normal

    /// A given candidate for mySourceAttElem can be from the source detail
    /// or from the map's evaluation detail.  This method returns the right
    /// attribute offsets to use for mySourceAttOffsets.
    ///
    /// This method will initialize myEvalOffsetCache prior to returning it
    /// when necessary.
    const SourceAttribOffsets	*getSourceOffsets(
					const POP_AttribMap *map,
					GEO_PointP source_att_elem);

private:
    void		 computeBBox();

    int			 myNumParticles;
    int			 myNumPoints;
    int			 myNumPrims;
    int			 myNumGroupEntries;
    GU_Detail		*myBBGdp;
    UT_Vector3		 myBBOrigin;
    UT_Vector3		 myBBScale;
    float		 myTimeInc;

    int			 myAllowGeneratorVar;
    int			 myVarDependent;
    int			 myVarTest;

    /// mySourceAttOffsets will either be NULL or will be pointing to one
    /// of the following two caches.  The first caches attribute info for
    /// the source detail.  The second caches the same for the evaluation
    /// detail in the local POP_AttribMap instance.
    SourceAttribOffsets	 mySourceOffsetCache;
    SourceAttribOffsets	 myEvalOffsetCache;
    bool		 myEvalOffsetCacheInit;
};

#endif
