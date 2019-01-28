/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_Fetch.h (Custom Library, C++)
 *
 * COMMENTS:    An object to fetch it's transform from another object.
 *
 */

#ifndef __OBJ_Fetch__
#define __OBJ_Fetch__

#include "OBJ_API.h"
#include "OBJ_Geometry.h"

enum OBJ_FetchIndex
{
    I_FETCHOBJPATH = I_N_GEO_INDICES,
    I_FETCH_USE_INPUT_OF_THIS,
    I_FETCH_USE_INPUT_OF_FETCHED,
    I_FETCHSUBNET,
    I_FETCHINVERT,		// invert fetched matrix

    I_N_FETCH_INDICES
};


class OBJ_API OBJ_Fetch : public OBJ_Geometry
{
public:
				 OBJ_Fetch(OP_Network *net,
					   const char *name,
					   OP_Operator *op);
    virtual			~OBJ_Fetch();

    virtual OBJ_OBJECT_TYPE	 getObjectType() const;

    void			 FETCHOBJPATH(UT_String &str)
				 {
				    evalString(str, "fetchobjpath",
						&getIndirect()[I_FETCHOBJPATH],
						0, 0.0f);
				 }
    void			 setFETCHOBJPATH(const UT_String &str,
						 CH_StringMeaning meaning)
				 {
				    setString(str, meaning, "fetchobjpath",
					      getIndirect()[I_FETCHOBJPATH],
					      0, 0.0f);
				 }
    int				 USEINPUTOFTHIS()
				 {
				     return evalInt("useinputofthis",
				    &getIndirect()[I_FETCH_USE_INPUT_OF_THIS],
						0, 0.0f);
				 }
    void			 setUSEINPUTOFTHIS( bool flag )
				 {
				     setInt("useinputofthis",
				       getIndirect()[I_FETCH_USE_INPUT_OF_THIS],
					    0, 0.0f, flag);
				 }
    int				 USEINPUTOFFETCHED()
				 {
				     return evalInt("useinputoffetched",
				   &getIndirect()[I_FETCH_USE_INPUT_OF_FETCHED],
						0, 0.0f);
				 }
    void			 setUSEINPUTOFFETCHED( bool flag )
				 {
				     setInt("useinputoffetched",
				    getIndirect()[I_FETCH_USE_INPUT_OF_FETCHED],
					    0, 0.0f, flag);
				 }
    int				 FETCHSUBNET()
				 {
				     return evalInt("fetchsubnet",
						&getIndirect()[I_FETCHSUBNET],
						0, 0.0f);
				 }
    void			 setFETCHSUBNET( int fetch_subnet )
				 {
				     setInt("fetchsubnet",
					    getIndirect()[I_FETCHSUBNET],
					    0, 0.0f, fetch_subnet );
				 }
    bool			 FETCHINVERT()
				 {
				     return (bool) evalInt("invertfetched",
						&getIndirect()[I_FETCHINVERT],
						0, 0.0f);
				 }

    virtual void		 resolveObsoleteParms(
					PRM_ParmList *obsolete_parms);

    static OP_Node              *myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template		*getTemplateList();
    static PRM_Template		*getObsolete();

    virtual OBJ_Fetch		*castToOBJFetch()	{ return this; }

    virtual void		 getNodeSpecificInfoText(OP_Context &context,
					OP_NodeInfoParms &iparms);
    virtual void		 fillInfoTreeNodeSpecific(UT_InfoTree &tree, 
					const OP_NodeInfoTreeParms &parms);

protected:
    virtual bool	 	 updateParmsFlags();
    
    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				 { return fetchIndirect; }

    virtual OP_ERROR		 cookMyObj(OP_Context &context);

private:
    static int			*fetchIndirect;
};

#endif

