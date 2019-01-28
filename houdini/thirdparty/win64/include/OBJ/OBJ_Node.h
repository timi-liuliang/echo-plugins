/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ library (C++)
 *
 * COMMENTS:	This defines a base object, here, children will inherit
 *		  my attributes...
 *
 *		My input (there's only one ever) is my "parent" in the
 *		  object heirarchy, thus, I can have as many children as
 *		  I want, but only one parent.
 *
 */

#ifndef __OBJ_Node_h__
#define __OBJ_Node_h__

#include "OBJ_API.h"
#include "OBJ_Error.h"
#include "OBJ_NodeFlags.h"
#include "OBJ_XformCache.h"

#include <GU/GU_DetailHandle.h>
#include <GEO/GEO_PackedTypes.h>

#include <OP/OP_DataMicroNode.h>
#include <OP/OP_Network.h>
#include <OP/OP_Node.h>
#include <OP/OP_Version.h>
#include <SOP/SOP_ObjectAppearanceFwd.h>

#include <UT/UT_Map.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_Playback.h>
#include <UT/UT_ValArray.h>

#include <stddef.h>


class CMD_Manager;
class GU_Detail;
class OBJ_Ambient;
class OBJ_Blend;
class OBJ_Bone;
class OBJ_Camera;
class OBJ_DopNet;
class OBJ_Fetch;
class OBJ_Fog;
class OBJ_Geometry;
class OBJ_Handle;
class OBJ_Light;
class OBJ_Null;
class OBJ_Rivet;
class OBJ_StereoCamera;
class OBJ_Sticky;
class OBJ_SubNet;
class OBJ_UndoPreTransform;
class OP_Options;
class PI_EditScriptedParms;
class PI_PropertyMap;
class SHOP_ReData;
class SOP_Node;
class UT_IStream;
class UT_Options;
class UT_String;
class UT_StringRef;


enum OBJ_OBJECT_TYPE 
{
    OBJ_WORLD	 = 0x00001,		// unused
    OBJ_GEOMETRY = 0x00002,		// has geometry and should render
    OBJ_CAMERA	 = 0x00004,		// has viewing parameters
    OBJ_LIGHT	 = 0x00008,		// has light specific parameters
    OBJ_RENDERER = 0x00010,		// obsolete
    OBJ_FOG	 = 0x00020,		// atmosphere
    OBJ_BONE	 = 0x00040,		// bone parameters
    OBJ_HANDLE	 = 0x00080,		// geometry handle
    OBJ_BLEND	 = 0x00100,		// blend object
    OBJ_FORCE	 = 0x00200,		// object describing a force
    OBJ_CAMSWITCH= 0x00400,		// switch between multiple cameras
    OBJ_SOUND    = 0x00800,		// sound object
    OBJ_MICROPHONE = 0x01000,		// microphone object
    OBJ_SUBNET 	 = 0x02000,		// subnet object
    OBJ_FETCH 	 = 0x04000,		// fetch object
    OBJ_NULL	 = 0x08000,		// null object
    OBJ_STICKY	 = 0x10000,		// sticky object
    OBJ_DOPNET	 = 0x20000,		// DOP network object
    OBJ_RIVET	 = 0x40000,		// rivet object
    OBJ_MUSCLE	 = 0x80000,

    // the above are the basic types used in the bitfield defining
    // an objects characteristics. Below are some common combinations.
    OBJ_STD_LIGHT	= OBJ_CAMERA|OBJ_LIGHT,	// ambient doesn't have CAMERA
    OBJ_STD_BONE	= OBJ_BONE | OBJ_GEOMETRY, 
    OBJ_STD_HANDLE	= OBJ_GEOMETRY | OBJ_HANDLE,
    OBJ_STD_BLEND	= OBJ_GEOMETRY | OBJ_BLEND,
    OBJ_STD_FETCH	= OBJ_GEOMETRY | OBJ_FETCH,
    OBJ_STD_STICKY	= OBJ_GEOMETRY | OBJ_NULL | OBJ_STICKY,
    OBJ_STD_RIVET	= OBJ_GEOMETRY | OBJ_NULL | OBJ_RIVET,
    OBJ_STD_NULL	= OBJ_GEOMETRY | OBJ_NULL,
    OBJ_STD_MUSCLE	= OBJ_GEOMETRY | OBJ_MUSCLE,
    OBJ_STD_CAMSWITCH	= OBJ_CAMERA | OBJ_CAMSWITCH,
    OBJ_ALL		= OBJ_WORLD|OBJ_GEOMETRY|OBJ_RENDERER|OBJ_CAMERA|
			  OBJ_LIGHT|OBJ_FOG|OBJ_BONE|OBJ_HANDLE|
			  OBJ_BLEND|OBJ_FORCE|OBJ_CAMSWITCH|OBJ_SOUND|
			  OBJ_MICROPHONE|OBJ_SUBNET|OBJ_FETCH|
			  OBJ_STICKY|OBJ_DOPNET|OBJ_RIVET
};

enum OBJ_MOTIONBLUR_TYPE 
{
    OBJ_MBLUR_NONE = 0,
    OBJ_MBLUR_XFORM = 1,
    OBJ_MBLUR_DEFORM = 2,
    OBJ_MBLUR_VELOCITY = 3
};

enum OBJ_SHADER_LOCATION
{
    OBJ_SHADER_NOWHERE,
    OBJ_SHADER_MATERIAL,	// Assign shader to material
    OBJ_SHADER_SPARE,		// Parameter on object
    OBJ_SHADER_PROPERTY,	// Parameter on property SHOP
};

enum OBJ_UIChangeType
{
    OBJ_UICHANGE_DISPLAY_ORIGIN_FLAG = OP_UICHANGE_OPTYPE_OBJ,
    OBJ_UICHANGE_PICKING
};

enum class OBJ_MatchTransform
{
    T,
    R,
    S,
    P,
    All
};

#define OBJ_MATERIAL_SPARE_TAG	"material_spare"

enum OBJ_BaseIndex
{
    // Transform
    I_XORDER,
    I_RORDER,
    I_T,
    I_R,
    I_S,
    I_P,
    I_PIVOTR,
    I_SCALE,
    I_PRETRANSFORM,
    I_KEEPPOS,
    I_CHILDCOMP,
    I_CONSTRAINTS_ON,
    I_CONSTRAINTS,

    I_LOOKAT,
    I_LOOKUP,
    I_LOOKUPOBJ,
    I_PATHOBJ,
    I_PATHROLL,
    I_PATHPOS,
    I_PATHUPARMTYPE,
    I_PATHORIENT,
    I_PATHUP,
    I_PATHBANK,

    // Render
    I_TDISPLAY,
    I_DISPLAY,

    // Misc
    I_USE_DCOLOR,
    I_DCOLOR,
    I_PICKING,
    I_PICKSCRIPT,
    I_CACHING,


    I_N_BASE_INDICES	 // should always be last in the list
};

enum OBJ_VariableId {
    OBJ_VAR_IPT,		// Instance point
    OBJ_MAX_VARIABLES
};

// We must maintain this order! If you change/add to OBJ_KeepPosType, then
// you must also fix it in OBJ_Command.C
enum OBJ_KeepPosType
{
    OBJ_KEEPPOS_NEVER = 0,
    OBJ_KEEPPOS_ALWAYS,
    OBJ_KEEPPOS_SOMETIMES,

    OBJ_KEEPPOS_NUM_FLAGS   // should always be last in the list
};

typedef enum {
    OBJ_PARMS_PLAIN,
    OBJ_PARMS_SCRIPT,
    OBJ_PARMS_SCRIPT_CLEAN
} OBJ_ParmsStyle;


#define OBJ_SELECTABLE_FLAG	'S'

#define FLOAT_OBJ_PARM(name, idx, vi, t)	\
		return evalFloat(name, &getIndirect()[idx], vi, t);
#define FLOAT_OBJ_PARMS(name, idx, v, t)	\
		evalFloats(name, &getIndirect()[idx], v, t);
#define INT_OBJ_PARM(name, idx, vi, t)	\
		return evalInt(name, &getIndirect()[idx], vi, t);
#define STR_OBJ_PARM(name, idx, vi, t)	\
		evalString(str, name, &getIndirect()[idx], vi, t);
#define SET_FLOAT(name, parm_index, vector_index, t, val, add_key) \
	    setFloat(name, getIndirect()[parm_index], vector_index, t, val, add_key);
#define SET_INT(name, parm_index, vector_index, t, val) \
	    setInt(name, getIndirect()[parm_index], vector_index, t, val);
#define SET_STRING(val, name, parm_index, vector_index, t) \
	    setString(val, name, getIndirect()[parm_index], vector_index, t);

class OBJ_API OBJ_Node : public OP_Network
{
public:
    virtual const char *getChildType() const override;
    virtual const char *getOpType() const override;

    virtual OP_OpTypeId	 getChildTypeID() const override;
    virtual OP_OpTypeId	 getOpTypeID() const override;
    static const char	*theChildTableName;

    // We must never use the result of this method to do a c-style cast.
    // If you need to cast, then use the casting methods provided.
    virtual OBJ_OBJECT_TYPE	 getObjectType() const = 0;

    virtual bool evalVariableValue(fpreal &val, 
                                   int var_id, 
                                   int thread) override;

    virtual bool evalVariableValue(UT_String &val, 
                                   int index, 
                                   int thread) override
				 {
				     return OP_Network::evalVariableValue(
							val, index, thread);
				 }

    // Global list of all known obsolete lists...
    static PRM_Template		*getObsolete();
    static PRM_Template		*mergeObsoleteLists(PRM_Template *t1,
						    PRM_Template *t2);
    static CH_LocalVariable	 ourLocalVariables[];
    
    // This finds all the known types and resolves them.  It doesn't mind
    // if they don't exist.
    virtual void resolveObsoleteParms(
	    PRM_ParmList *obsolete_parms) override;

    // Resolves a specific parm...
    void			 applyBackwardsPathCompatibility(
					PRM_ParmList *obsolete_parms,
					const char *oldname, 
					const char *newname, 
					const char *root = "/obj/", 
					const char *tail = "");
    void			 applyBackwardsNetNodeCompatibility(
					PRM_ParmList *obsolete_parms,
					const char *oldnetname,
					const char *oldnodename,
					const char *newname,
					const char *rootname,
					const char *property_parm=0);
    // This is a SHOP specific style one.  As SHOP_PATHs allowed any arbitrary
    // path internally, merely requiring they are relative to /shop, we
    // cannot just blindly prefix /shop/.  Specifically, if the old path
    // was /shop/bar, we don't want /shop//shop/bar.
    void			 applyBackwardsShopCompatibility(
					PRM_ParmList *obsolete_parms,
					const char *oldname,
					const char *newname);

    // These are convenience functions. Do NOT override in subclasses! 
    // Override the appropriate apply.*Transform() function instead!
    // NOTE: there are exceptions to the comment above. Notably, the
    //       OBJ_Bone class has no notion of scaling along one axis only.
    //       It also treats rotations specially, and therefore it overrides
    //       the parameter transformation methods below.

    /// Get the transform built from the trs parms of this object
    /// @param	context	The cook context, which contains the time.
    /// @param	xform	The output parameter, that contains the matrix described
    ///			by the transformation parameters (rotation, tranlation,
    ///			and scale).
    virtual bool getParmTransform(OP_Context &context, UT_DMatrix4 &xform );
    virtual bool getParmTransformRotates(OP_Context &context,
					 UT_DMatrix4 &xform );
    virtual bool getParmTransformTranslates(OP_Context &context,
					 UT_DMatrix4 &xform );
    virtual bool getParmTransformScales(OP_Context &context,
					 UT_DMatrix4 &xform );

    bool	 isUseFullTransformModel() const
		    { return myUseFullTransformModel; }

    // Delete all channels on the transform parms.
    void    destroyTransformParmChannels();

		// set the trs parms of the object to match the given world 
		// transform. rigid parm transform omits scales.  Returns
		// false if fail_if_any_locked_parms is true and any of the
		// parms are locked. Whether or not we change locked
		// parms, when not automatically failing, is specified by
		// skip_locked_parms.
    bool    setParmTransformFromWorld(
			OP_Context &context, const UT_DMatrix4 &world_xform,
			bool create_keys = false,
			bool fail_if_any_locked_parms = false,
			bool change_locked_parms = false);
    void    setRigidParmTransformFromWorld(
			OP_Context &context, const UT_DMatrix4 &world_xform,
			bool create_keys = false );
		// set the trs parms of the object to match the given parm 
		// transform. rigid parm transform omits scales.  Returns
		// false if fail_if_any_locked_parms is true and any of the
		// parms are locked. Whether or not we change locked
		// parms, when not automatically failing, is specified by
		// skip_locked_parms.
    bool    setParmTransform( OP_Context &context, const UT_DMatrix4 &xform,
			      bool create_keys = false,
			      bool fail_if_any_locked_parms = false,
			      bool change_locked_parms = false,
			      const UT_Vector3R *rotate_vals = NULL
                              );

    void    setRigidParmTransform( OP_Context &context,
				   const UT_DMatrix4 &xform,
				   bool create_keys = false );

    /// Get the pivot transform built from the pivot parms of this object
    /// @param	context	The cook context, which contains the time.
    /// @param	xform	The output parameter, that contains the matrix described
    ///			by the pivot transformation parameters (rotation and
    ///			and translation).
    bool    getParmPivotTransform(OP_Context &context, UT_DMatrix4 &xform);

    /// Set the pivot transform parms from the rotates and translates in the
    /// given xform matrix.
    bool    setParmPivotTransform(OP_Context &context, const UT_DMatrix4 &xform,
				  bool create_keys = false,
				  bool fail_if_any_locked_parms = false,
				  bool change_locked_parms = false,
				  const UT_Vector3R *rotate_vals = NULL);

    /// Obtains an accumulation of input independedn and pre-transform matrices.
    /// This is equivalent to:
    ///	    applyPreTransform(applyInputIndependent(identity))
    /// Which is is the portion of the local transform that does not
    /// depend on the parameters (transform or look-at). It is not widely used
    /// and only relevant for the handles to orient themselves in parent space.
    ///
    /// @param	context	The cook context, which contains the time.
    /// @param	mat	The output parameter that will contain the calculated
    ///			matrix.
    /// @return		0 if mat is the identity, non-zero otherwise.
    int	    getPreLocalTransform(OP_Context &context, UT_DMatrix4 &mat);

    /// Obtains the effective local transform which does not depend on input
    /// nodes. This transform includes input independent matrix, pre-transform
    /// matrix, parameter transform  matrix, and the look-at matrix. This method
    /// first cooks the object (if necessary) and then returns the member
    /// variable myXform.
    ///
    /// @param	context	The cook context, which contains the time.
    /// @param	mat	The output parameter that will contain the calculated
    ///			matrix.
    bool    getLocalTransform(OP_Context &context, UT_DMatrix4 &mat);

    /// Calculates the inverse of the getLocalToWorldTransform()
    bool    getInverseLocalToWorldTransform(
					OP_Context &context, UT_DMatrix4 &mat);

    /// Returns the effective world transform of the parent. 
    /// The input depenent transform ensures that any externally dependent
    /// transformation have been taken into account (eg, the length of the
    /// parent bone for OBJ_Bone). This transform is the base to which all the
    /// local transforms are applied (pre-transform, parameter, etc).
    ///
    /// In terms of methods this is calculated by
    /// applyInputDependentTransform( 
    ///	    getParentObject()->getChildToWorldTransform())
    /// If @c getParentObject() is @c NULL, then it simply returns @c
    /// applyInputDependentTransform(identity_matrix). 
    /// 
    /// Use this function to convert
    /// coordinates that have @b not been transformed by the follow path or
    /// pre-transform. Typically, this function is only used internally for
    /// manipulating the pre-transform. If the object is in a subnet, 
    /// then this will include the subnet's world transform.
    ///
    /// @param	context	The cook context, which contains the time.
    /// @param	mat	The output parameter that will contain the calculated
    ///			matrix.
    bool    getParentToWorldTransform(OP_Context &context, UT_DMatrix4 &mat);

    /// Returns a transform matrix that is only missing the
    /// local and look-at components.  
    /// In terms of methods this is calculated by
    ///	    applyPreTransform( applyInputIndependentTransform( 
    ///				    getParentToWorldTransform()))
    /// Use this function to convert co-ordinates from object space to world
    /// space. Object space includes everything @b except the object's local
    /// transform parameters. Typically, this function is used by handles to
    /// place themselves relative to the origin of the object.
    ///
    /// @param	context	The cook context, which contains the time.
    /// @param	mat	The output parameter that will contain the calculated
    ///			matrix.
    bool    getObjectToWorldTransform(OP_Context &context, UT_DMatrix4 &mat);

    /// Calculates the inverse of the getObjectToWorldTransform().
    bool    getInverseObjectToWorldTransform(
					OP_Context &context, UT_DMatrix4 &mat);

    /// Returns the full world transform of the object including
    /// all the component sub-matrices (parent and local).
    /// This is equivalent to
    ///		getLocalTransform() * getParentToWorldTransform()
    /// Use this function to convert co-ordinates from the local (geometry)
    /// space to world space.  It cooks the object first and returns the member
    /// variable @c myWorldTransform. Typically, this function is used to place
    /// child SOPs of the object in world space.
    ///
    /// @param	context	The cook context, which contains the time.
    /// @param	mat	The output parameter that will contain the calculated
    ///			matrix.
    bool    getLocalToWorldTransform(OP_Context &context, UT_DMatrix4 &mat);

    /// Returns the transforms provided by the object's parent.
    /// This is equivalent to applyOutputTransform( getLocalToWorldTransform())
    /// Typically, this function is only used internally for the implementation
    /// of @c getParentToWorldTransform(). 
    ///
    /// Note that in general, the following inequality holds, unless @c
    /// applyInputDependentTransform() has no effect:
    /// @code
    /// getParentToWorldTransform() != 
    ///     getParentObject()->getChildToWorldTransform()
    /// @endcode
    bool    getChildToWorldTransform(OP_Context &context, UT_DMatrix4 &mat);


    /// Obtains the world space matrix before the constraints were applied.
    ///
    /// @param	context	The cook context, which contains the time.
    /// @param	mat	The output parameter that will contain the calculated
    ///			matrix.
    SYS_DEPRECATED(16.5) // No longer needed or functional.
    bool    getPreConstraintsTransform(OP_Context &context, UT_DMatrix4 &mat);

    // single precision versions of the above functions 
    // (slightly less efficient since it has to do it in double precision and
    // then assign it to single precision)
    bool    getParmTransform(OP_Context &context, UT_Matrix4 &xform );
    bool    getParmPivotTransform(OP_Context &context, UT_Matrix4 &xform );
    int	    getPreLocalTransform(OP_Context &context, UT_Matrix4 &mat);
    bool    getLocalTransform(OP_Context &context, UT_Matrix4 &mat);
    bool    getParentToWorldTransform(OP_Context &context, UT_Matrix4 &mat);
    bool    getObjectToWorldTransform(OP_Context &context, UT_Matrix4 &mat);
    bool    getInverseObjectToWorldTransform(
					OP_Context &context, UT_Matrix4 &mat);
    bool    getLocalToWorldTransform(OP_Context &context, UT_Matrix4 &mat);
    bool    getInverseLocalToWorldTransform(
					OP_Context &context, UT_Matrix4 &mat);
    bool    getChildToWorldTransform(OP_Context &context, UT_Matrix4 &mat);
    SYS_DEPRECATED(16.5) // No longer needed or functional.
    bool    getPreConstraintsTransform(OP_Context &context, UT_Matrix4 &mat);

    // 2d methods.
    bool    getLocal2DToWorldTransform(OP_Context &context, UT_DMatrix4 &mat);
    virtual bool    get2DWorldTransform(UT_DMatrix4 &mat, OP_Context &context);

    // functions to override those in OP_Node.h
    virtual bool getWorldTransform(UT_Matrix4D &mat,
				   OP_Context &context) override;
    virtual bool getIWorldTransform(UT_Matrix4D &mat,
				    OP_Context &context) override;
    virtual bool getTransform( TransformMode mode, UT_Matrix4D &mat,
				 OP_Context &context) override;
    virtual int getRelativeTransform(OP_Node &to, 
                                     UT_Matrix4 &xform,
                                     OP_Context &context) override;
    virtual int getRelativeTransform(OP_Node &to, 
                                     UT_DMatrix4 &xform,
                                     OP_Context &context)  override;

    // This is used by specialized channel functions like qlinear()
    // to determine what the transform order should be:
    virtual void getXformOrder(UT_XformOrder &xord, 
                               const CH_Channel *chp) const override;

    // the following two methods are to be used along with
    // keepWorldTransformByOldInput()
    int			 keepPosWhenParenting();
    bool		 getPreTransformInput(OP_Context &context, 
					      UT_DMatrix4 &mat);
    int			 keepPosWhenMoving();

    /// This method considers the effect of the LOOKAT fields and produces
    /// a matrix that should be premultiplied to the orienation to get the
    /// lookat.  It takes the local->world transform (myWorldXform) as this
    /// is used to figure out where this is outside cookpaths. 
    /// If 'interest' is supplied, then interest will be onto this other node
    /// instead.
    /// NOTE: this function is virtual, because OBJ_Bone has a "capture"
    ///       mode in which the lookAt parameter (or input node's transform 
    ///       for that matter) is ignored. Bones
    ///	      override this method to provide the mode-dependent transorm.
    /// @param	context	The cook context, which contains the time.
    /// @param	world	The world matrix based on which the lookat matrix is 
    ///			calculated.
    ///	@param	lookat	The output parameter that will contain a calucated
    ///			look-at matrix. That look-at matrix will be
    ///			pre-multiplied with the local and the world transforms
    ///			to calculate their final form.
    ///	@param	interest    Usually, this node will register an interest in the
    ///			look-at object, so it gets dirtied when the look-at
    ///			object moves. The interest parameter specifies a node
    ///			that should be dirtied instead of the object on which
    ///			buildLookAt() got invoked.
    /// 
    /// @return This returns 0 if no lookat is needed, in which case the matrix
    ///		provided will not be altered!
    virtual int		 buildLookAt(OP_Context &context,
				     const UT_DMatrix4 &world, 
				     UT_DMatrix4 &lookat,
				     OP_Node *interest = 0);
    /// A stand alone version of buildLookAt that is not connected to the ui
    /// and always takes an upvector.
    int			 buildLookAt(OP_Context &context,
				     const UT_DMatrix4 &worldxform,
				     const UT_String &lookat,
				     UT_DMatrix4 &lookatxform,
				     UT_Vector3R &up);
    // Returns whether or not the lookat object is time dependent.
    bool	getLookatObjectIsTimeDependent(const UT_String &lookat,
					       fpreal t);
    // Returns the local to world transform of the lookat object.
    // Optionally adds an extra input. Returns true if a valid lookat
    // object was found. Sets xform to identity if no lookat or an
    // invalid lookat was found.
    bool	getLookatObjectTransform(const UT_String &lookat,
					 OP_Context &context,
					 OP_Node *addextrainputto,
					 UT_DMatrix4 &xform,
					 bool &timedep);
    // getPreLookatTransform gives us the pretransform matrix for an object
    // that can later be used to create a lookat matrix with buildinglookat.
    // getPreLooaktTransform can calculate the whole prelook at matrix,
    // which is the default setting or calculate the matrix without the
    // rotates included.
    int		getPreLookatTransform(OP_Context &context,
				      UT_DMatrix4 &wolrdXform,
				      UT_DMatrix4 &xform,
				      bool norotates = false);
    // maintain the original world position as if we were using the old
    // parent world xform by modifying the pretransform
    int		keepWorldTransformByOldInput(OP_Context &context, 
			    const UT_DMatrix4 &old_parent_world_xform);

    // maintain the last cooked world transform by modifying pretransform/parms
    void	keepLastWorldTransform(OP_Context &context);

    // Returns whether or not the lookat object is time dependent.
    bool	getPathObjectIsTimeDependent(const UT_String &path,
					     fpreal t);
    // Returns the local to world transform of the lookat object.
    // Optionally adds an extra input. Returns true if a valid lookat
    // object was found. Sets xform to identity if no lookat or an
    // invalid lookat was found.
    bool	getPathObjectDetailHandle(const UT_String &path,
					  OP_Context &context,
					  GU_ConstDetailHandle &gdh,
					  UT_DMatrix4 &xform);

    // Callback for building custom world transforms from objects.
    // The callback is responsible for setting the given matrix by
    // the parm transform of the object.
    // An example of using this would simply do
    //	    node->getParmTransform(context,mat)
    // Return 0 to stop processing, 1 to continue.
    typedef int (*ApplyParmTransformCallback)(
		    void *data, OBJ_Node *node, 
		    OP_Context &context, UT_DMatrix4 &mat);

    // Use these functions to build your own custom world transforms instead 
    // of traversing the object hieararchy yourself! If you do it yourself, 
    // then you will probably not take into account the output and 
    // pre-transforms.
    // DO NOT CALL THESE WITHIN AN OBJECT COOK PATH!!
    void	    buildCustomChildToWorldTransform(
			    OP_Context &context, UT_DMatrix4 &mat,
			    ApplyParmTransformCallback callback, void *data);
    void	    buildCustomObjectToWorldTransform(
			    OP_Context &context, UT_DMatrix4 &mat,
			    ApplyParmTransformCallback callback, void *data);

    /// Set the given world position by only modifying the pre-transform.
    /// This is less efficient than keepWorldTransformByOldInput() but more
    /// general. If your local t,r,s parms have not been modified, then use
    /// keepWorldTransformByOldInput instead.
    ///
    /// @param	context	    The cook context, which contains the time.
    /// @param	world_xform The world transform to set for this object.
    virtual int	    keepWorldTransform( 
				    OP_Context &context, 
				    const UT_DMatrix4 &world_xform );

    // this is overridden so that we can clear our own undo flags
    virtual void clearUndoFlags() override;

    /// Stores the current pre-transform for future undo.
    void		 savePreTransformForUndo();

    /// This sets the pre-transform to the identity matrix. This will modify 
    /// the object's world transform unless the pre-transform was already the 
    /// identity.
    void		 resetPreTransform();

    /// This transfers the object's transform parameters (@c L) to the
    /// pre-transform. This does not modify the effective world transform.
    virtual void	 transferLocalToPreTransform(fpreal gtime);

    /// This extracts the object's pre-transform into its transform parameters.
    /// If the extraction involved shears, then only the non-sheared portion
    /// is extracted. This does not modify the effective world transform.
    virtual void	 transferPreToLocalTransform(
			    fpreal gtime, bool follow_chan_refs = false);
    
    /// Transfers only the rotate portion of the matrix to pre-transform.
    virtual void	 transferLocalToPreTransformRotates(fpreal gtime);

    /// Transfers only the translates portion of the matrix to pre-transform.
    virtual void	 transferLocalToPreTransformTranslates(fpreal gtime);

    /// Transfers only the scales portion of the matrix to pre-transform.
    virtual void	 transferLocalToPreTransformScales(fpreal gtime);

    /// This sets the pretransform to the given matrix. This will modify the
    /// object's effective world transform unless the given pre-transform is
    /// identity.
    void		 setPreTransform(const UT_DMatrix4 &xform);

    // get the rotation euler angles that include the pre-transform
    void		 getAdjustedRotates(OP_Context &context, UT_Vector3R &r);

    // Takes the given rotation in degrees and adjusts it to include the 
    // pretransform
    void		 adjustRotatesToIncludePreTransform(
			    OP_Context &context, UT_Vector3R &r);

    // adjust the given rotations (in degrees) suitable for stuffing into a
    // rotation parm taking into account the pre-transform
    void		 adjustRotatesForPreTransform( 
			    OP_Context &context,
			    int return_as_radian, UT_Vector3R &rot);

    /// Returns current pretransform matrix.
    const UT_DMatrix4 &	 getPreTransform() const
			    { return myPreTransform; }
    UT_DMatrix4	&	 getPreTransform()  
			    { return myPreTransform; }
    int			 isPreTransformIdentity()
			    { return myPreTransformIsIdentity; }
    void		 setIsPreTransformIdentity(int yesno)
			    { myPreTransformIsIdentity = yesno; }

    virtual int	 	 setPickable(int onOff) override;
    virtual int		 getPickable() override;
    int			 setCachable(int onOff);
    int			 getCachable(int thread) const;
    virtual int		 setFlag(char flag, int onoff) override;

    virtual bool	 getBoundingBox(UT_BoundingBox &box, 
                	                OP_Context &ctx) override;

    SOP_Node		*getDisplaySopPtr()
			    { return CAST_SOPNODE(getDisplayNodePtr()); }
    SOP_Node		*getRenderSopPtr() 
			    { return CAST_SOPNODE(getRenderNodePtr()); }
    DOP_Node		*getDisplayDopPtr()
			    { return CAST_DOPNODE(getDisplayNodePtr()); }

    GU_DetailHandle	 getDisplayGeometryHandle(OP_Context &context,
						  int check_enable=1,
						  OP_Node **source_node=0);
    GU_DetailHandle	 getRenderGeometryHandle(OP_Context &context,
						 int check_enable=1);
    const GU_Detail	*getDisplayGeometry(OP_Context &context,
					    int check_enable=1,
					    OP_Node **source_node=0);
    const GU_Detail	*getRenderGeometry(OP_Context &context,
					   int check_enable=1);

    enum obj_OnionSkin
    {
	OFF,
	TRANSFORM,
	DEFORM
    };
    virtual obj_OnionSkin getOnionSkin() { return OFF; }
    static void		 buildOperatorTable(OP_OperatorTable &table);
    static void		 getManagementOperators(OP_OperatorList &op);
    static void		 installCommands(CMD_Manager *cman);
    static void		 initializeExpressions();
    static void		 buildMaterialOptMenu(void *obj,
					PRM_Name *names,
					int size,
					const PRM_SpareData *spare,
					const PRM_Parm *parm);
    static int		 handleMaterialOpt(void *obj,
					int index,
					fpreal now,
					const PRM_Template *tplate);
    bool		 processMaterialOpt(fpreal now, const char *operation,
					int argc, const char *argv[],
					UT_String &errors);


    // These convience functions allow you to deal with COP2 paths
    // and prepare them in a fashion the TIL resolver will liek.
    int			 getFullCOP2Path(const char *relpath, 
				UT_String &fullpath, int &flagdependent);
    void		 splitCOP2Path(const char *path,
				UT_String &net, UT_String &nodepath);


    // Menu callback functions available to general public

    static void		 buildObjectMenu(PRM_Name *, int max,
					 OBJ_OBJECT_TYPE mask);
    static void		 buildGeoObjMenu(
			    void *o, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildPathObjMenu(
			    void *o, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildKinChopMenu(
			    void *o, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildPopGeoMenu(
			    void *o, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildShadeObjMenu(
			    void *o, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildColorPlaneMenu(
			    void *o, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildAlphaPlaneMenu(
			    void *o, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildGeoChopMenu(
			    void *o, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildSoundChopMenu(
			    void *o, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildMicChopMenu(
			    void *o, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildChopNetMenu(
			    void *o, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildBoneCaptureMenu(
			    void *o, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildBoneChildMenu(
			    void *o, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildChildObjMenu(
			    void *o, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildPointGroupMenu(
			    void *o, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildPrimitiveGroupMenu(
			    void *o, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
	
    static int		 handlePreTransformMenu(void *o, int index, fpreal t,
						const PRM_Template *);

    virtual int		getDisplayOrigin() const override;
    virtual int		setDisplayOrigin(int on_off_unchanged) override;

    int			getParentingErrorFlag() const
			    { return myObjFlags.getParentingErrorFlag(); }
    void		setParentingErrorFlag(int o)
			    { myObjFlags.setParentingErrorFlag(o); }

    virtual int		isCookingRender() const override
			{ return myCookingRender ? 1:0; }
    
    virtual void	setCookingRender(int val) override
			{ myCookingRender = (val!=0); }

    OBJ_NodeFlags	&objflags()	{ return myObjFlags; }

    virtual OP_ERROR	 setInput(
	unsigned idx, OP_Node *op,
	unsigned outputIdx = 0) override;
    
    virtual OP_ERROR     setInputReference(
	unsigned idx, const char *label,
	int keeppos, 
	unsigned outputIdx = 0) override;
    
    virtual OP_ERROR	 setIndirectInput(
	unsigned idx, 
        OP_IndirectInput *in) override;

    virtual OP_ERROR	 setNamedInput(const OP_ConnectorId& input_name, 
                    	               OP_Node *op,
                    	               const OP_ConnectorId* output_name = NULL
                    	               ) override;
    
    virtual OP_ERROR	 setNamedInputReference(
	const OP_ConnectorId&  input_name, 
	const char *label, 
	int, 
	const OP_ConnectorId* output_name = NULL) override;
    
    virtual OP_ERROR	 setNamedIndirectInput(
	const OP_ConnectorId& input_name,
	OP_IndirectInput *input) override;

    /// Override this to specify the inputs which are needed for cooking this
    /// node which can be executed in parallel.
    virtual void getParallelInputs(
	OP_Context &context,
	OP_NodeList &nodes) const override;

    static void		 setGlobalKeepPosWhenParenting(OBJ_KeepPosType flag)
			 { ourKeepPosWhenParenting = flag; }
    static OBJ_KeepPosType getGlobalKeepPosWhenParenting()
			 { return ourKeepPosWhenParenting; }

    static void		 setGlobalKeepPosWhenMoving(OBJ_KeepPosType flag)
			 { ourKeepPosWhenMoving = flag; }
    static OBJ_KeepPosType getGlobalKeepPosWhenMoving()
			 { return ourKeepPosWhenMoving; }

    static void		 disallowGlobalKeepPos();
    static void		 allowGlobalKeepPos();

    static void		 disallowIgnorePretransform();
    static void		 allowIgnorePretransform();

    static void		 setIgnorePretransformValue(bool flag)
			 { ourIgnorePretransformValue = flag; }
    static bool		 getIgnorePretransform()
			 { return ourAllowIgnorePretransform
				  && ourIgnorePretransformValue; }
    static bool		 getIgnorePretransformValue()
			 { return ourIgnorePretransformValue; }

    virtual void	 opChanged(OP_EventType reason, 
                	           void *data=0) override;
    virtual void	 referencedParmChanged(int parm_index) override;

    // takes into account TRS order and pivot point
    UT_Vector3R		 getLocalPivot(OP_Context &context);
    UT_Vector3R		 getWorldPivot(OP_Context &context);

    void		 setParmFromHandle(
			    OP_Context &context,
			    PRM_Parm &parm,
			    int vec_idx,
			    fpreal val,
			    bool create_keys,
			    bool check_modified);
    void		 setParmFromHandle(
			    OP_Context &context,
			    PRM_Parm &parm,
			    const UT_Vector3R &val,
			    bool create_keys = false,
			    int key_idx = -1,
			    bool check_modified = true);

    virtual void	 setScaleFromHandle(fpreal t, const UT_Vector3R &s,
					    bool create_keys = false,
					    int key_idx = -1,
					    bool check_modified = true);
    virtual void	 setUniformScaleFromHandle(
					    fpreal t, fpreal uniform_scale,
					    bool create_keys = false,
					    bool check_modified = true);
    virtual void	 setRotateFromHandle(fpreal t, const UT_Vector3R &r,
					     bool create_keys = false,
					     int key_idx = -1,
					     bool check_modified = true);
    virtual void	 setTranslateFromHandle(fpreal t,
						const UT_Vector3R &trans,
						bool create_keys = false,
						int key_idx = -1,
						bool check_modified = true);
    virtual void	 setPivotFromHandle(fpreal t,
					    const UT_Vector3R &pivot,
					    bool create_keys = false,
					    int key_idx = -1,
					    bool check_modified = true);
    virtual void	 setPivotRotateFromHandle(fpreal t,
					    const UT_Vector3R &pivot_rotate,
					    bool create_keys = false,
					    int key_idx = -1,
					    bool check_modified = true);
    void		 setXformOrderFromHandle(fpreal t, int value,
						 bool create_keys = false,
						 bool check_modified = true);

    virtual const char * getSetScaleParmName() const	    { return "s"; }
    virtual const char * getSetRotateParmName() const	    { return "r"; }
    virtual const char * getSetTranslateParmName() const    { return "t"; }

    // Determines whether an object should be sent to renderers or not.
    virtual int		 isObjectRenderable(fpreal t) const;

    // Function returns whether the object is lit by a particular light
    virtual int		 isObjectLitBy(OBJ_Node *, fpreal) { return 0; }
    /// Function to resolve lighting based on light masks
    virtual bool	 isLightInLightMask(const OBJ_Node *light, fpreal now)
			    { return false; }

    // Function returns whether the object is lit by a particular subnet
    virtual int		 isObjectLitBySubnet(OBJ_Node *, fpreal) { return 0; }

    // Function to return the subset of the specified lights which actually
    // light this object.  The default implementation uses the virtual
    // isObjectLitBy method to determine this subset.
    virtual void	 getActingLightSubset(
			    const UT_ValArray<OBJ_Ambient *> &lights,
			    UT_ValArray<OBJ_Ambient *> &active, fpreal t);

    /// Return an interface to apply appearance operations.  The default
    /// behaviour is to return a NULL pointer.
    virtual SOP_ObjectAppearancePtr	getObjectAppearance();

    // Let each object type decide whether or not it should be drawn lit
    virtual int		 doDrawLit() const { return 0; }

    static const char *	displaySopToken;
    static const char *	renderSopToken;
    static const char *	boundingBoxToken;
    static const char *	boundingSphereToken;
    static const char *	input1ObjectToken;
    static const char *	input2ObjectToken;
    static const char *	input3ObjectToken;
    static const char *	input4ObjectToken;

    virtual void	 getNodeSpecificInfoText(OP_Context &context,
				OP_NodeInfoParms &iparms) override;
    virtual void 	 fillInfoTreeNodeSpecific(UT_InfoTree &tree, 
				const OP_NodeInfoTreeParms &parms) override;
    
    virtual void	 propagateEndBlockModify() override;

    //Absolute Width
    virtual fpreal       getW() const override;
    //Absolute Height
    virtual fpreal       getH() const override; 


    virtual OBJ_Ambient *castToOBJAmbient()     { return NULL; }
    virtual OBJ_Blend	*castToOBJBlend()	{ return NULL; }
    virtual OBJ_Bone	*castToOBJBone()	{ return NULL; }
    virtual OBJ_Camera	*castToOBJCamera()      { return NULL; }
    virtual OBJ_DopNet	*castToOBJDopNet()	{ return NULL; }
    virtual OBJ_Fetch	*castToOBJFetch()	{ return NULL; }
    virtual OBJ_Fog	*castToOBJFog()		{ return NULL; }
    virtual OBJ_Geometry*castToOBJGeometry()	{ return NULL; }
    virtual OBJ_Handle	*castToOBJHandle()	{ return NULL; }
    virtual OBJ_Light	*castToOBJLight()       { return NULL; }
    virtual OBJ_Null    *castToOBJNull()	{ return NULL; }
    virtual OBJ_Rivet	*castToOBJRivet()	{ return NULL; }
    virtual OBJ_Sticky	*castToOBJSticky()	{ return NULL; }
    virtual OBJ_SubNet	*castToOBJSubNet()      { return NULL; }
    virtual OBJ_StereoCamera* castToOBJStereoCamera()	{ return NULL; }

    // Build a shader string for the object.  If the shader string is built
    // successfully, the SHOP which generated the string will be returned.
    bool	 assignShader(int shop_type, const char *path,
			OBJ_SHADER_LOCATION where);
    OP_Node	*evalShaderString(UT_String &shader, int shop_type, fpreal now,
			const UT_Options *options, OP_Node *shader_node = 0);
    bool	 evalShaderHandle(UT_String &handle, int shop_type, fpreal now,
			const UT_Options *options, OP_Node *shader_node = 0);
    bool	 evalShaderLanguage(UT_String &lang, int shop_type, fpreal now,
			const UT_Options *options, OP_Node *shader_node = 0);
    OP_Node	*evalShaderData(SHOP_ReData &data, int shop_type, fpreal now,
			const UT_Options *options, OP_Node *shader_node = 0);
    OP_Node	*evalShaderBounds(UT_BoundingBox &box, int shop_type,fpreal now,
			const UT_Options *options, OP_Node *shader_node = 0);
    bool	 evalCoShaderStrings(UT_StringArray &strings,
			OP_NodeList &shops, int shop_type, fpreal now,
			const UT_Options *options, OP_Node *shader_node = 0);
    bool	 evalCoShaderHandles(UT_StringArray &handles,
			int shop_type, fpreal now,
			const UT_Options *options, OP_Node *shader_node = 0);
    bool	 evalCoShaderLanguages(UT_StringArray &languages,
			int shop_type, fpreal now,
			const UT_Options *options, OP_Node *shader_node = 0);
    void	 getShopTransform(UT_DMatrix4 &xform, int shop_type,
			fpreal now, const UT_Options *options);

    // Returns the token (name) of the material parameter.
    static const char	*getMaterialParmToken();

    // Get a node specified as a material 
    OP_Node		*getMaterialNode( fpreal now );

    // Get a particular SHOP.
    OP_Node		*getShaderNode(OP_Node *candidate, int shader_type, 
				    fpreal now, const UT_Options *options);
    OP_Node		*getShaderNode(int shader_type, fpreal now,
				     const UT_Options *options,
				     DEP_MicroNode *dep = NULL);

    // Load up the options with the local parameter defintions
    virtual const UT_Options	*createShaderParms(OP_Options &options,
						fpreal now,
						const UT_Options *src);
    
    // Combine the SOPs of other objects into this one. On success, returns
    // the display_merge SOP pointer, else NULL.
    //
    // objs		List of objects to combine. These will be deleted on
    //			return.
    // full_combine     If true, it performs a combine with the existing SOPs
    //			inside this object.
    // moved_map	Optional. On return, this filled with a map of the
    //			child node pointers from source node pointer to
    //			destination node pointer.
    OP_Node		*combine(OP_Context &context,
				 const UT_ValArray<OBJ_Node *> &objs,
				 bool full_combine = true,
				 UT_Map<OP_Node *, OP_Node *> *moved_map = NULL);

    virtual bool	 getHideDefaultParms() const
			    { return false; }

    virtual bool	 canCreateNewOpType() const override;

    virtual const OP_DataMicroNode &
			 parmListMicroNodeConst() const override
			    { return myXformMicroNode; }
    OP_VERSION		 parmListModVersion() const
			    { return parmListMicroNodeConst().modVersion(); }

    // myXformCookLevel is positive during the cook of myXform
    virtual const OP_DataMicroNode &
			 dataMicroNodeConst() const override
			 {
			     return (myXformCookLevel > 0)
					? myXformMicroNode
					: OP_Network::dataMicroNodeConst();
			 }

    // Set Parm Transform that can trigger notifications
    bool    setParmTransform( OP_Context &context, const UT_DMatrix4 &xform,
			      bool create_keys,
			      bool fail_if_any_locked_parms,
			      bool change_locked_parms,
			      const UT_Vector3R *rotate_vals,
                              bool check_modified
                              );
    // Set Parm Pivot Transform that can trigger notifications
    bool    setParmPivotTransform( OP_Context &context,
			      const UT_DMatrix4 &xform,
			      bool create_keys,
			      bool fail_if_any_locked_parms,
			      bool change_locked_parms,
			      const UT_Vector3R *rotate_vals,
                              bool check_modified
                              );

    bool hasConstraints() const { return myHasConstraints; }

protected:

	     OBJ_Node(OP_Network *parent, const char *name, OP_Operator *op);
    virtual ~OBJ_Node();

    bool		supportsDropShopType(int shop_type);
    virtual int		acceptDragDrop(DD_Source &src, 
               		               const char *label) override;
    
    virtual int		testDragDrop(DD_Source &src) override;
    virtual void	getDragDropChoice(DD_Source &src, 
                	                  DD_ChoiceList &c) override;


    // This method should add any extra interests that an object might
    //	have.  By default, geo instance, particle instance, shader spaces
    //	etc. are added.  This is called when objects cook.
    virtual void	addExtraDependencies(OP_Context &context);
    virtual int		getOpShadingInfo(UT_String &str, fpreal t);
    virtual int		getOpConstraintsInfo(UT_String &str, fpreal t);
    virtual bool	createSpareParametersFromChannels(
				UT_BitArray &selection,
				const CH_ChannelList &channels) override;
    virtual bool	createSpareParametersForLoad(UT_StringArray &names,
				PRM_ParmNameMap &nmap,
				UT_String &errors) override;
    virtual bool	getParameterOrProperty(
				const UT_StringRef &name, fpreal now,
				OP_Node *&op, PRM_Parm *&parm,
				bool create_missing_multiparms,
				PRM_ParmList *obsolete = 0) override;
    virtual bool	getParameterOrPropertyByChannel(
				const UT_StringRef &name,
				fpreal now,
				OP_Node *&op, PRM_Parm *&parm, int &vectoridx,
				PRM_ParmList *obsolete = 0) override;
    virtual int		findParametersOrProperties(fpreal now,
				OP_PropertyLookupList &list) override;

    /// Computes and pre-multiplies an input dependent matrix to the given 
    /// current matrix. This transform is
    /// defined as semantically dependent on the object's inputs. The OBJ_Blend
    /// object overrides this to do special processing. Also, the OBJ_Bone
    /// object overrides this to move its origin to the tip of the parent bone.
    /// By default, this method does not modify the given matrix, which is
    /// equivalent to identity matrix.
    /// @param	context	The cook context, which contains the time.
    /// @param	mat	Input and output parameter, that gets premuliplied by 
    //			the computed input dependent transform matrix, if any.
    ///
    /// @return		Returns 1 if the matrix was modified, 0 otherwise
    virtual int	    applyInputDependentTransform(
				OP_Context &context, UT_DMatrix4 &mat);

    /// Computes and pre-multiplies an input independent transform to 
    /// the given matrix. 
    /// This transform is defined as semantically independent of the 
    /// object's inputs. The default implementation applies a follow path 
    /// object transform, if available. 
    ///
    /// @param	context	The cook context, which contains the time.
    /// @param	mat	Input and output parameter, that gets premuliplied by 
    ///			the computed input independent transform matrix, if any.
    ///
    /// @return		Returns 1 if the matrix was modified, 0 otherwise
    virtual int	    applyInputIndependentTransform(
				OP_Context &context, UT_DMatrix4 &mat);

    ///	Computes and pre-multiplies an output transform to the given matrix.
    /// The output transform is a transform that a parent specifies for its
    /// children. This concept exists for completeness (since there is an input
    /// transform), but is not widely used.
    /// The default implementation does not apply any transform.
    ///
    /// @param	context	The cook context, which contains the time.
    /// @param	mat	Input and output parameters, that gets premultipled
    ///			by an appropriate output transform, if any.
    ///
    /// @return		Returns 1 if the matrix was modified, 0 otherwise
    virtual int	    applyOutputTransform(OP_Context &context, UT_DMatrix4 &mat);

    /// Premuliplies the pre-transform matrix, myPreTransform, to the given 
    /// matrix. Pre-matrix is a transformation layer between the input and the
    /// parameter transforms, and is used to define the origin point (and
    /// orientation) at which zero translation and zero rotation would place the
    /// object. This allows to have more meaningull parameter values during
    /// animation, where rotation of 0 degrees may need to correspond to a
    /// slightly bent limb (ie, rotated joint).
    ///
    /// @param	context	The cook context, which contains the time.
    /// @param	mat	The input and output parameter, that gets premultipled
    ///			by a pre-transform matrix.
    ///
    /// @return		Returns 1 if the matrix was modified, 0 otherwise
    virtual int	    applyPreTransform(OP_Context &context, UT_DMatrix4 &mat);

    bool	    getParentToWorldTransformForInputNoLock(
			OBJ_Node *input, OP_Context &context, UT_DMatrix4 &mat, bool applyInputDependentTransform=true);

    // Methods to obtatin and store transformations in a cache. Also
    // check if this object is or should be caching xform
    const UT_DMatrix4 &	 getCachedTransform( OP_Context &context,
					     OBJ_XformCache::OBJ_XformType type,
					     OBJ_XformCache::OBJ_LookupStatus &status ) const;
    void		 setCachedTransform( OP_Context &context,
					     OBJ_XformCache::OBJ_XformType type,
					     const UT_DMatrix4 &xform ) const;
    bool		 isCachingTransforms(int thread) const;

    virtual bool	 updateParmsFlags() override;
    
    /// Used to perform proper dependency tracking when cooking the local
    /// transform (myXform) from cookMyObj().
    class OBJ_API LocalCookScope
    {
    public:
	LocalCookScope(OP_Context &context, OBJ_Node& obj, bool constraint=false);
	~LocalCookScope();
    private:
	OP_Context&	myContext;
	OBJ_Node&	myObj;
	const bool	myWasTimeDep;
	const bool	myWasCooking;
	const bool	myConstraint;
    };
    friend class LocalCookScope;

    //
    //  The cookMe "caches" information - i.e. the world and local xform matrix
    virtual OP_ERROR		 cookMe(OP_Context &context) override;
    virtual OP_ERROR		 bypassMe(OP_Context &context,
					  int &copied_input) override;
    virtual OP_ERROR		 cookMyObj(OP_Context &context);

    virtual OP_DataType	 getCookedDataType() const override;
    virtual void	 deleteCookedData() override;
    virtual int		 saveCookedData(const char *, OP_Context &) override;
    virtual int		 saveCookedData(std::ostream &os, 
               		                OP_Context &, 
               		                int binary = 0) override;

    virtual const char	*getFileExtension(int binary) const override;
    virtual OP_ERROR	 saveIntrinsic(std::ostream &os, 
                    	               const OP_SaveFlags &flags) override;
    virtual void	 saveIntrinsicCommand(std::ostream &os, 
                	                      const char *name) override;

    virtual bool	 loadPacket(UT_IStream &is, short class_id, short sig, 
				    const char *path=0) override;
    virtual bool	 loadPacket(UT_IStream &is, const char *token, 
				    const char *path=0) override;

    virtual void	 saveFlagsForUndo() override;
    virtual void	 getSaveFlagsString(UT_String &cmd,
					    const char *flags,
					    bool save_to_hip) const override;
    virtual void	 getPreParmSaveFlagsString( UT_String &cmd ) const override;
    virtual void	 getPostParmSaveFlagsString( UT_String &cmd ) const override;

    // appends the description of the xform to the text. If the xform is not 
    // an identity the label is printed first and the rst values follow.
    // If the xform is identity the text_if_identity is appended instead
    // of the whole label and xform printout
    void		getXformDescription(const UT_DMatrix4 &xform,
				     const char* label_if_not_identity,
				     const char* text_if_identity,
				     const UT_Vector3R pivot,
				     const UT_Vector3R pivot_rotate,
				     const UT_XformOrder order,
				     UT_WorkBuffer &text);

    // types of transormations that we can clean (i.e.,
    // transfer from local to pre-transform)
    enum OBJ_TransformComponent
    {
	OBJ_TRANSFORM_WHOLE,
	OBJ_TRANSFORM_ROTATES,
	OBJ_TRANSFORM_TRANSLATES,
	OBJ_TRANSFORM_SCALES
    };

    
    /// Calculates a matrix that needs to be premultiplied with pretransform
    /// to transfer the transformation of type 'type' to the pretransform matrix
    void		getParmTransformTransferMatrix( 
				        OP_Context &context,
					OBJ_TransformComponent type,
					UT_DMatrix4 &xform );

    // method for propagating the change in capture transform by xform. 
    // This method is
    // overloaded for OBJs that support capture parameters (e.g., OBJ_Bone)
    // in order to adjust their global capture position. This is handy
    // when editing the caputre pose and when we need to propagate
    // a transofrm (e.g., translation) to all the descendents. 
    virtual void	applyToCaptTransformAndDescendents(
					    OP_Context &context,
					    const UT_DMatrix4 &xform );
    
    // method for building a menu containg point groups
    virtual void	 buildPointGroupMenu(
			    PRM_Name *menu, int size, 
			    const PRM_SpareData *spare, const PRM_Parm *parm);

    // method for building a menu containg primitive groups
    virtual void	 buildPrimitiveGroupMenu(
			    PRM_Name *menu, int size, 
			    const PRM_SpareData *spare, const PRM_Parm *parm);


//_________________________________________________________________________
//
//  Convenience Error methods...
//_________________________________________________________________________
//
    void                 addError(int code, const char *msg = 0)
                         { UTaddError("OBJ", code, msg);}
    void                 addMessage(OBJ_Error code, const char *msg = 0)
                         { UTaddMessage("OBJ", code, msg);}
    void                 addWarning(OBJ_Error code, const char *msg = 0)
                         { UTaddWarning("OBJ", code, msg);}
    void                 addFatal(OBJ_Error code, const char *msg = 0)
                         { UTaddFatal("OBJ", code, msg);}

public:

    static PRM_SpareData *getMatchSelectButton(int option, int autoscope);
//  TODO: We have to make sure that the parent is cooked here!
    virtual OBJ_Node	*getParentObject();
    OP_ERROR		 setParentObject(OBJ_Node *node) 
			    { return setInput(0, node); }

    OBJ_Node		*getInstanceObject(fpreal now) const;
    OBJ_Node		*getGeometryInstance(fpreal now) const;
    OBJ_Node		*getParticleObject(fpreal now) const;

    void		 setLookAtObject(OBJ_Node *node);
    void		 setLookUpObject(OBJ_Node *node);
    void		 setPathObject(OBJ_Node *node);

protected:
    friend class OBJ_AutoTransformDependencies;
    virtual int		 collectPropertiesFromChannels(
					PI_EditScriptedParms &editparms,
					UT_BitArray &selection,
					const CH_ChannelList &channels);
    virtual int		 collectPropertiesFromParms(
					PI_EditScriptedParms &editparms,
					UT_StringArray &names,
					PRM_ParmNameMap &nmap);
    int			 collectProperties(PI_EditScriptedParms &editparms,
					UT_BitArray &selection,
					const CH_ChannelList &channels,
					PI_PropertyMap *pmap);
    int			 collectProperties(PI_EditScriptedParms &editparms,
					UT_StringArray &names,
					PRM_ParmNameMap &nmap,
					PI_PropertyMap *pmap);
    void		 createPropertiesShop();

    // Method to find a SHOP node of a specific type.  If the type is
    // SHOP_INVALID, any SHOP will be returned.  Otherwise, the SHOP at the
    // given path must be of the correct type.  NULL will be returned if the
    // type doesn't match or the path isn't found.

    // Used to get pointer to indirection indices for each object type
    virtual int		*getIndirect() const = 0;

    int			 getCachedParmIndex(const UT_StringRef &name,
					    int name_idx) const
			 {
			    int &parm_idx = getIndirect()[name_idx];
			    if (parm_idx < 0)
				parm_idx = getParmList()->getParmIndex(name);
			    return parm_idx;
			 }
    int			 getCachedParmIndex(const PRM_Name &name,
					    int name_idx) const
			 {
			    int &parm_idx = getIndirect()[name_idx];
			    if (parm_idx < 0)
				parm_idx = getParmList()->getParmIndex(name);
			    return parm_idx;
			 }
    PRM_Parm		*getCachedParm(const UT_StringRef &name, int name_idx)
			 {
			    return getParmList()->getParmPtr(
					getCachedParmIndex(name, name_idx));
			 }
    const PRM_Parm	*getCachedParm(
				const UT_StringRef &name, int name_idx) const
			 {
			    return getParmList()->getParmPtr(
					getCachedParmIndex(name, name_idx));
			 }

    virtual bool	 getTimeOverride()  { return false; }

    // inverseDirty is no more needed because setWorldXform does it automatically.
    SYS_DEPRECATED_HDK_REPLACE(16.0, "setWorldXform")
    void		 inverseDirty()	{ myInverseDirty = true; }

    const UT_DMatrix4	 &getInverseXform()
			  {
			      if (myInverseDirty)
			      {
				  myInverseDirty = false;
				  myWorldXform.invert(myIWorldXform);
			      }
			      return myIWorldXform;
			  }

    // Compute the object contraints given a local and a parent transform.
    // The contraint network is evaluated and given the parent and local_xforms.
    // This updates myXform and myWorldXform.
    bool computeConstraints(OP_Context &context, const UT_DMatrix4 &parent_xfo);

    // Returns the currently cooked local matrix. Doesn't perform cooking.
    const UT_Matrix4D&   getLocalXform() const
			 { return myXform; }

    // Sets the cooked local matrix during cooking.
    void		 setLocalXform(const UT_Matrix4D &m)
			 { myXform = m; }

    // Returns the currently cooked world matrix. Doesn't perform cooking.
    const UT_Matrix4D&   getWorldXform() const
			 { return myWorldXform; }

    // Sets the cooked world matrix during cooking.
    // Also sets the myInverseDirty flag properly.
    void		 setWorldXform(const UT_Matrix4D &m)
			 { myWorldXform = m; myInverseDirty=true; }

public:
    void  setTX(fpreal t, fpreal v, PRM_AddKeyType add_key=PRM_AK_MARK_PENDING)
	{ SET_FLOAT("t", I_T, 0, t, v, add_key) }
    void  setTY(fpreal t, fpreal v, PRM_AddKeyType add_key=PRM_AK_MARK_PENDING)
	{ SET_FLOAT("t", I_T, 1, t, v, add_key) }
    void  setTZ(fpreal t, fpreal v, PRM_AddKeyType add_key=PRM_AK_MARK_PENDING)
	{ SET_FLOAT("t", I_T, 2, t, v, add_key) }
    void  setRX(fpreal t, fpreal v, PRM_AddKeyType add_key=PRM_AK_MARK_PENDING)
	{ SET_FLOAT("r", I_R, 0, t, v, add_key) }
    void  setRY(fpreal t, fpreal v, PRM_AddKeyType add_key=PRM_AK_MARK_PENDING)
	{ SET_FLOAT("r", I_R, 1, t, v, add_key) }
    void  setRZ(fpreal t, fpreal v, PRM_AddKeyType add_key=PRM_AK_MARK_PENDING)
	{ SET_FLOAT("r", I_R, 2, t, v, add_key) }
    void  setSX(fpreal t, fpreal v, PRM_AddKeyType add_key=PRM_AK_MARK_PENDING)
	{ SET_FLOAT("s", I_S, 0, t, v, add_key) }
    void  setSY(fpreal t, fpreal v, PRM_AddKeyType add_key=PRM_AK_MARK_PENDING)
	{ SET_FLOAT("s", I_S, 1, t, v, add_key) }
    void  setSZ(fpreal t, fpreal v, PRM_AddKeyType add_key=PRM_AK_MARK_PENDING)
	{ SET_FLOAT("s", I_S, 2, t, v, add_key) }
    void  setPX(fpreal t, fpreal v, PRM_AddKeyType add_key=PRM_AK_MARK_PENDING)
	{ SET_FLOAT("p", I_P, 0, t, v, add_key) }
    void  setPY(fpreal t, fpreal v, PRM_AddKeyType add_key=PRM_AK_MARK_PENDING)
	{ SET_FLOAT("p", I_P, 1, t, v, add_key) }
    void  setPZ(fpreal t, fpreal v, PRM_AddKeyType add_key=PRM_AK_MARK_PENDING)
	{ SET_FLOAT("p", I_P, 2, t, v, add_key) }
    void  setPIVOTRX(fpreal t, fpreal v,
		     PRM_AddKeyType add_key=PRM_AK_MARK_PENDING)
	{ SET_FLOAT("pr", I_PIVOTR, 0, t, v, add_key) }
    void  setPIVOTRY(fpreal t, fpreal v,
		     PRM_AddKeyType add_key=PRM_AK_MARK_PENDING)
	{ SET_FLOAT("pr", I_PIVOTR, 1, t, v, add_key) }
    void  setPIVOTRZ(fpreal t, fpreal v,
		     PRM_AddKeyType add_key=PRM_AK_MARK_PENDING)
	{ SET_FLOAT("pr", I_PIVOTR, 2, t, v, add_key) }
    void  setSCALE(fpreal t, fpreal v,
		   PRM_AddKeyType add_key=PRM_AK_MARK_PENDING)
	{ SET_FLOAT("scale", I_SCALE, 0, t, v, add_key) }

    void  setTRANSORDER(int v)  { SET_INT("xOrd", I_XORDER, 0, 0, v) }
    void  setROTORDER(int v)    { SET_INT("rOrd", I_RORDER, 0, 0, v) }
    void  setKEEPPOS(int v)     { SET_INT("keeppos", I_KEEPPOS, 0, 0, v) }
    void  setCHILDCOMP(int v)  	{ SET_INT("childcomp", I_CHILDCOMP, 0, 0, v) }

    int   KEEPPOS(fpreal t) const { INT_OBJ_PARM("keeppos", I_KEEPPOS, 0, t) }
    int	  TRS(fpreal t) const { INT_OBJ_PARM("xOrd", I_XORDER, 0, t); }
    int	  XYZ(fpreal t) const { INT_OBJ_PARM("rOrd", I_RORDER, 0, t); }
    int   CHILDCOMP(fpreal t) const { INT_OBJ_PARM("childcomp", I_CHILDCOMP, 0, t) } 

    void  T(fpreal *v, fpreal t)  { FLOAT_OBJ_PARMS("t",     I_T, v, t) }	
    void  R(fpreal *v, fpreal t)  { FLOAT_OBJ_PARMS("r",     I_R, v, t) }	
    void  S(fpreal *v, fpreal t)  { FLOAT_OBJ_PARMS("s",     I_S, v, t) }	
    void  P(fpreal *v, fpreal t)  { FLOAT_OBJ_PARMS("p",     I_P, v, t) }	
    void  PIVOTR(fpreal *v, fpreal t)
				  { FLOAT_OBJ_PARMS("pr", I_PIVOTR, v, t) }

    fpreal SCALE(fpreal t)	  { FLOAT_OBJ_PARM("scale",	I_SCALE, 0, t) }

    void  LOOKUP(UT_String &str, fpreal t)  
				  { STR_OBJ_PARM  ("lookup",  I_LOOKUP, 0, t) }
    // types of allowed path parameterization
    // NB: these correspond to the menu entries in thePathParmTypes[]
    enum obj_PathParmType
    {
	OBJ_PATHPARM_UNIFORM = 0,
	OBJ_PATHPARM_ARCLEN
    };

    fpreal PATHPOS(fpreal t)	  { FLOAT_OBJ_PARM("pos", I_PATHPOS, 0, t) }
    int    PATHPARMTYPE(fpreal t) { INT_OBJ_PARM("uparmtype", I_PATHUPARMTYPE,0,t) }
    int	   PATHORIENT(fpreal t)	  { INT_OBJ_PARM("pathorient", I_PATHORIENT, 0, t) }
    fpreal PATHROLL(fpreal t)	  { FLOAT_OBJ_PARM("roll", I_PATHROLL, 0, t) }
    void   PATHUP(fpreal *v, fpreal t)
				  { FLOAT_OBJ_PARMS("up", I_PATHUP, v, t) }
    fpreal PATHBANK(fpreal t)	  { FLOAT_OBJ_PARM("bank", I_PATHBANK, 0, t) }

    int   PICKING()	          {INT_OBJ_PARM("picking", I_PICKING, 0, 0) }
    void  PICKSCRIPT(UT_String &str, fpreal t)
		{STR_OBJ_PARM("pickscript", I_PICKSCRIPT, 0, t)}
    int	  CACHING()		  {INT_OBJ_PARM("caching", I_CACHING, 0, 0) }

    // This parameter is only important in specific classes, so is not
    // defined here at the base level.
    virtual int   VPORT_SHADEOPEN() { return 0; }
    virtual int	  VPORT_DISPLAYASSUBDIV() { return 0; }

    fpreal	SHADERATE(fpreal t, fpreal defval=1);
    int		PHANTOM(fpreal t, int default_value);
    int		MATTE(fpreal t, int default_value);
    int		RAYBOUNCE(fpreal t, int default_value);
    int		TRANSMITBOUNCE(fpreal t, int default_value);
    fpreal	RAYCLIP(fpreal t, fpreal def=0.001);
    fpreal	DISPBOUND(fpreal t, fpreal def=0);
    fpreal	SHADOWRATE(fpreal t, fpreal def=1);
    fpreal	REFLRATE(fpreal t, fpreal def=1);
    fpreal	LOD(fpreal t, fpreal def=1);
    void	PREINCLUDE(UT_String &str, fpreal t);
    void	POSTINCLUDE(UT_String &str, fpreal t);
    OBJ_MOTIONBLUR_TYPE		BLUR(fpreal t,
				    OBJ_MOTIONBLUR_TYPE def = OBJ_MBLUR_NONE);


    int	  USE_DCOLOR()
		{ return evalInt("use_dcolor", &getIndirect()[I_USE_DCOLOR],
				 0, 0); }
    fpreal DCOLORR(fpreal t)	{ FLOAT_OBJ_PARM("dcolor", I_DCOLOR, 0, t) }
    fpreal DCOLORG(fpreal t)	{ FLOAT_OBJ_PARM("dcolor", I_DCOLOR, 1, t) }
    fpreal DCOLORB(fpreal t)	{ FLOAT_OBJ_PARM("dcolor", I_DCOLOR, 2, t) }

    UT_Color	DCOLOR(fpreal t)
		{
		    UT_Color col(UT_RGB);
		    fpreal32 c[3];
		    evalFloats("dcolor", &getIndirect()[I_DCOLOR], c, t);
		    col.setRGB(c[0], c[1], c[2]);
		    return col;
		}

    void setDCOLORR(fpreal t, fpreal r)
		{ SET_FLOAT("dcolor", I_DCOLOR, 0, t, r, PRM_AK_MARK_PENDING) }
    void setDCOLORG(fpreal t, fpreal g)
		{ SET_FLOAT("dcolor", I_DCOLOR, 1, t, g, PRM_AK_MARK_PENDING) }
    void setDCOLORB(fpreal t, fpreal b)
		{ SET_FLOAT("dcolor", I_DCOLOR, 2, t, b, PRM_AK_MARK_PENDING) }

    int DISPLAY(fpreal t) const	{ INT_OBJ_PARM("display", I_DISPLAY, 0, t) }
    int parmIndexDISPLAY() const
	{ return getCachedParmIndex("display", I_DISPLAY); }

    // Determine whether the geometry is really displayed.  If the display
    // channel is turned off, then we use the display flag. Use this as opposed
    // to isDisplayDisabled() unless you do *not* want to check this object's
    // display flag.  A network can be specified to ignore the display state
    // of any parents starting with that network.
    bool		getObjectDisplay(fpreal t, const OP_Network *n=0) const
			{
			    return getDisplay() && !isDisplayDisabled(t, n);
			}
    GEO_ViewportLOD	getObjectDisplayStyle(fpreal t,
					      const OP_Network *net = NULL,
					      bool ignore_display=false) const;
    bool		isDisplayTimeDependent() const;

    // Determine whether the geometry is disabled for display.  If the display
    // channel is turned off, then we use the display flag.  A network can be
    // specified to ignore the display state of any parents starting with that
    // network.
    int			 isDisplayDisabled(fpreal t,
					   const OP_Network *net = 0) const;

    // setVisible() differs from setDisplay() in that it will try to set the
    // display flag on parent subnets and change visible object parameters as
    // necessary.
    virtual bool	 setVisible(int onoff) override;
    virtual bool	 getVisible() const override;

    virtual void	 evaluateBatchParms(PRM_BatchList &list, 
                	                    fpreal now) override;

    // These parameters are cached

    int		TDISPLAY() const
		{
		    return evalInt("tdisplay",
				   &getIndirect()[I_TDISPLAY], 0, 0);
		}
    int		parmIndexTDISPLAY() const
		{
		    return getCachedParmIndex("tdisplay", I_TDISPLAY);
		}

    int		parmIndexCONSTRAINTSON() const
		{
		    return getCachedParmIndex("constraints_on", I_CONSTRAINTS_ON);
		}

    int		parmIndexCONSTRAINTSPATH() const
		{
		    return getCachedParmIndex("constraints_path", I_CONSTRAINTS);
		}

    int		INSTANCE(UT_String &str, fpreal now) const;
    int		POINTINSTANCE(fpreal now) const;
    int		PARTICLE(UT_String &s, fpreal t) const;
    bool	PTMOTIONBLUR(UT_String &str, fpreal now) const;

    int		LOOKAT(UT_String &str, fpreal t) const
		{ 
		    STR_OBJ_PARM  ("lookatpath",  I_LOOKAT, 0, t) 
		    return str.isstring();
		}

    int		LOOKUPOBJ(UT_String &str, fpreal t) const
		{ 
		    STR_OBJ_PARM  ("lookupobjpath",  I_LOOKUPOBJ, 0, t) 
		    return str.isstring();
		}

    int		PATHSOP(UT_String &str, fpreal t) const
		{ 
		    STR_OBJ_PARM  ("pathobjpath", I_PATHOBJ, 0, t) 
		    return str.isstring();
		}

    // This is used by the viewer to determine whether or not to cook (and show)
    // particle guide geometry at the object level.
    int		PROMOTE_POPGUIDES(fpreal t) const
		{ return getDefaultedIProperty("vport_promote_popguides", t,1); }

    // This is used by DM_VPortAgent3D to determine what order to
    // draw objects in.  It itself will set it depending on user defined
    // order, so it really is a temporary one-per-object flag.
    int		getDisplayOrder() const
		{
		    return myDisplayOrder;
		}
    void	setDisplayOrder(int order)
		{
		    myDisplayOrder = order;
		}

    // This is used by OBJ_SubNet. This is called on all child objects
    // when the Visible Children parameter changes.
    void	setDisplayDisabledBySubnetNeedsUpdate()
		{ myDisplayDisabledBySubnetNeedsUpdate = 1; }

    void		 addTransformParmInterests( OP_Node *interested );

    int		getDefaultedIProperty(const char *name, fpreal now, int def) const
		{
		    int		val;
		    if (evalParameterOrProperty(name, 0, now, val))
			return val;
		    return def;
		}
    fpreal	getDefaultedFProperty(const char *name, fpreal now, fpreal def) const
		{
		    fpreal	val;
		    if (evalParameterOrProperty(name, 0, now, val))
			return val;
		    return def;
		}
    const char  *getDefaultedSProperty(const char *name, fpreal now,
				    UT_String &result, const char *def) const
		{
		    if (!evalParameterOrProperty(name, 0, now, result))
			result = def;
		    return result;
		}
    const char  *getDefaultedRawSProperty(const char *name, fpreal now,
				    UT_String &result, const char *def) const
		{
		    if (!evalParameterOrPropertyRaw(name, 0, now, result))
			result = def;
		    return result;
		}

    virtual int64	 getMemoryUsage(bool inclusive) const override;

    OP_Node	*getEvaluatedConstraints();

    // Evaluate the constraint path parameter and strip the optional channel name.
    OP_Node	*parseConstraintPath( UT_String &path, UT_String &channel, int add_dep );
    OP_Node	*parseConstraintPath( UT_String &path, UT_String &channel, int add_dep, bool &got_by_flag );

protected:
    virtual void	 checkTimeDependencies(int do_parms=1,
				int do_inputs=1,
				int do_extras=1) override;
private: // methods
    friend class OBJ_UndoPreTransform;

    void		 evalBLUR(UT_String &result, fpreal now);
    void		 checkKeepPosWhenParenting( OBJ_Node *parent_obj );

    const PRM_Parm *	 findTransformProperty(
				fpreal t, const UT_StringRef &name) const;
    PRM_Parm *		 findTransformProperty(
				fpreal t, const UT_StringRef &name);

    template <typename OBJ_NODE, typename VISITOR>
    static void		 visitTransformParms(
				OBJ_NODE *node,
				fpreal t,
				bool exclude_uniform_scale,
				VISITOR& visitor);

    bool		 hasTransformParmChannels(
					fpreal t,
					bool exclude_uniform_scale) const;

    int			 evalToggleInt(char *togglename, int toggleindirect,
					char *parm, int parmindirect,
					fpreal t, int def = 0);
    fpreal		 evalToggleFloat(char *togglename, int toggleindirect,
					char *parm, int parmindirect,
					fpreal t, fpreal defval = 1);
    int			 evalToggleParm(const char *togglename,
				       const char *token, fpreal now,
				       int value);
    fpreal		 evalToggleParm(const char *togglename,
				       const char *token, fpreal now,
				       fpreal value);

    void		 computeSubnetParentTransform(
			    OP_Context &context,
			    const UT_ValArray<OBJ_Node *> &parentstack,
			    UT_DMatrix4 &world_xform);

    void		 addTransformDependencies( OBJ_Node *subnet );

    void		 propagateKeepPosWhenMoving();

    void		 dirtyPreTransform();
    void		 updatePreTransformParms( bool from, bool undo=true);
    void		 updatePreTransformParms( bool from, bool undo, fpreal now, int thread);
    void		 getPreTransformParms( OP_Context &ctx, UT_Matrix4D &mat );

    void		 applyBackwardsRiTransmitCompatibility(
					PRM_ParmList *obsolete_parms);

    virtual const char	*inputLabel(unsigned) const override;

    void applyLimits( OP_Context &context, fpreal *t, fpreal *r, fpreal *s );
    template<int OFFSET>
    void applyLimit( OP_Context &context, fpreal *t );

    void		 onChangeSpareParms();
    void		 cookLocalXform(OP_Context &context, const UT_DMatrix4 &pmat);

    /// Declares that this node's data depends on the @c op for the given
    /// reason (OP_INTEREST_DATA, OP_INTEREST_FLAG).
    /// This OBJ override also supports OP_INTEREST_INPUT and redirects it to a call to addOpReference()
    /// @note There are no other acceptable values for type.
    void	 addExtraObjInput(OP_Node *op, OP_InterestType type);

    /// All OBJ nodes can be a selection owner.
    /// This replaces the previous inheritance from SOP_SelectionOwner.
    virtual bool isSelectionOwnerType() const final
    { return true; }

private: // data


    // Pre H16, myWorldXform and myXform used to be protected.
    // When cooking a derived OBJ_Node, use setWorldXform and setLocalXform instead.
    UT_DMatrix4		 myXform;		// Transform in parent's space
    UT_DMatrix4		 myWorldXform;		// Transform in world space
    UT_DMatrix4		 myIWorldXform;		// Inverse Xform in world space

    // myPreTransform is right multiplied into the actual parms version of
    // the local object transform when we cook.
    // ie. myXform includes myPreTransform after being cooked
    UT_DMatrix4		 myPreTransform;

    OP_Node		*myEvaluatedConstraints;

    // Micro-node for determining when myXform is dirty
    class obj_XformMicroNode : public OP_DataMicroNode
    {
    public:
	obj_XformMicroNode(OP_Node &node)
	    : OP_DataMicroNode(node)
	{
	}
	virtual const char *className() const
	{
	    return "obj_XformMicroNode";
	}
    };
    obj_XformMicroNode	 myXformMicroNode;
    int			 myXformCookLevel;

    DEP_MicroNode	 myModeMicroNode;

    int			 myShopMaterialIndex;		// Cache of parm index
    uint		 myShopMaterialIndexLastCheck;	// Add/remove count

    unsigned		 myPreTransformIsIdentity:1,
			 myTraversingForBlur:1,
			 mySubnetParentTimeDep:1,
			 myShowingPreTransform:1,
			 myUpdatingPreTransform:1,
			 myInverseDirty:1,
			 myUseFullTransformModel:1,
			 myCookingRender:1,
			 myLimitsFlags:18,
			 myHasLookAt:1,
			 myHasPath:1,
			 myHasConstraints:1;

    mutable unsigned	 myDisplayDisabledBySubnetNeedsUpdate:1,
			 myDisplayDisabledBySubnet:1,
			 myInstanceRecursion:1;

    OBJ_NodeFlags	 myObjFlags;

    // This gets this nodes precedence in display order.
    int			 myDisplayOrder;

    // These variables are for computing the parent xform
    UT_ValArray<OP_VERSION> mySubnetParentVersion;
    UT_DMatrix4		 mySubnetParentTransform;

    enum objLimits
    {
	MIN_TX = 1<<0,
	MIN_TY = 1<<1,
	MIN_TZ = 1<<2,
	MIN_RX = 1<<3,
	MIN_RY = 1<<4,
	MIN_RZ = 1<<5,
	MIN_SX = 1<<6,
	MIN_SY = 1<<7,
	MIN_SZ = 1<<8,

	MAX_TX = 1<<9,
	MAX_TY = 1<<10,
	MAX_TZ = 1<<11,
	MAX_RX = 1<<12,
	MAX_RY = 1<<13,
	MAX_RZ = 1<<14,
	MAX_SX = 1<<15,
	MAX_SY = 1<<16,
	MAX_SZ = 1<<17,
    };

    // Pair of Parm* and Long
    struct objSpareParam
    {
	objSpareParam();
	objSpareParam( OBJ_Node *obj, const char* name);

	void reset()
	{
	    parm = NULL;
	    id = -1;
	}

	PRM_Parm *parm;
	int id;
    };
    // Struct holding the object spare properties pointers
    // for faster access and to only add a pointer to OBJ_Node class.
    struct objSpareProperties
    {
	// returns true if all the Parm pointers are NULL.
	// this should be called before disposing of myProperties.
	// PLEASE UPDATE when adding new data members
	bool isEmpty() const;
	void reset();

	// Pre-Transform Category
	objSpareParam myPreXOrd;
	objSpareParam myPreROrd;
	objSpareParam myPreTranslate;
	objSpareParam myPreRotate;
	objSpareParam myPreScale;

	// Limits Category
	objSpareParam myLimitsOn[18];
	objSpareParam myLimits[18];

	objSpareParam myParentToBoneTip;
    };
    objSpareProperties	*myProperties;

private: // static data

    static OBJ_KeepPosType	ourKeepPosWhenParenting;
    static OBJ_KeepPosType    	ourKeepPosWhenMoving;
    static bool		    	ourAllowKeepPos;
    static bool			ourIgnorePretransformValue;
    static bool			ourAllowIgnorePretransform;
};

inline int
OBJ_Node::getCachable(int thread) const
{
    return evalIntT("caching", &getIndirect()[I_CACHING], 0, 0, thread);
}

inline bool
OBJ_Node::isCachingTransforms(int thread) const
{
    int mode = UT_Playback::getPlayback()->getInterceptMode();

    // the intercept mode is used by some CHOPs to map the current time
    // beyond the frame limits. In this case we cannot guarantee that
    // caching a matrix at frame, say, 2 and then after one play cycle 
    // again at frame 2 (which is sort of frame 302) is refering to the 
    // same xform (assuming 1..300 range). In these situatin we turn off 
    // caching.
    // NB: the intercept mode is entered via SCROLL LOCK key

    return (mode != 1) && getCachable(thread);
} 

#undef FLOAT_OBJ_PARM
#undef FLOAT_OBJ_PARMS
#undef INT_OBJ_PARM
#undef STR_OBJ_PARM

#undef SET_FLOAT
#undef SET_INT
#undef SET_STRING

extern "C" {
    SYS_VISIBILITY_EXPORT extern void newObjectOperator(OP_OperatorTable *table);
};

class OBJ_API OBJ_AutoTransformDependencies
{
public:
    OBJ_AutoTransformDependencies( OP_Context &ctx );
    ~OBJ_AutoTransformDependencies();

    void addExtraInput( OP_Node *src, OP_InterestType type=OP_INTEREST_DATA);
    void addExtraInput( OP_Node *src, int pi, int vi=0 );
    void addExtraInput( OP_Node *src, int pi, int vi_start, int vi_end );

    bool hasCaller() const  { return myCaller;}
private:
    OP_Context &myContext;
    OP_ContextData *myCallerUserData;
    OP_Node *myCaller;
    bool myAddTimeDep;
};
#endif
