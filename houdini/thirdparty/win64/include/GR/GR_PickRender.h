/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_PickRender ( GR Library, C++)
 *
 * COMMENTS:
 *	Pure virutal base class for component picking 
 */
#ifndef GR_PickRender_h
#define GR_PickRender_h

class RE_Render;
class RE_Geometry;
class GR_DisplayOption;
class GR_RenderInfo;
class GR_GeoRender;

#include "GR_API.h"
#include "GR_Defines.h"
#include "GR_PickRecord.h"
#include <UT/UT_VectorTypes.h>
#include <UT/UT_UniquePtr.h>
#include <RE/RE_Types.h>
#include <GU/GU_SelectType.h>

enum GR_PickCompMode
{
    GR_PICK_CONSTANT_ID = 0,
    GR_PICK_VARYING_ID  = 1,
    GR_PICK_INSTANCE_ID = 2
};

class GR_API GR_PickRender
{
public:
		    GR_PickRender(RE_Render *r,
				  const GR_DisplayOption *opts,
				  const GR_RenderInfo *info,
				  RE_Geometry *geo,
				  GR_GeoRender *gr_geo = NULL);
	           ~GR_PickRender();

    // Point picks
    int		renderPoints(int connect_start,
			     int connect_num,
			     RE_PrimType real_prim_type,
			     int real_connect_start,
			     int real_connect_num,
			     GR_PickCompMode id_mode,
			     GR_PickStyle style,
			     bool has_pick_map,
			     GR_SelectMode select_mode,
			     int select_inst,
			     RE_Geometry *pick_buffer);

    // primitive picks
    int		renderFacePrims(int connect_start,
				int connect_num,
				GR_PickCompMode id_mode,
				GR_PickStyle style,
				bool has_pick_map,
				bool wireframe,
				GR_SelectMode select_mode,
				int select_inst,
				RE_Geometry *pick_buffer);
    
    int		renderLinePrims(int connect_start,
				int connect_num,
				GR_PickCompMode id_mode,
				GR_PickStyle style,
				bool has_pick_map,
				bool wireframe,
				GR_SelectMode select_mode,
				int select_inst,
				RE_Geometry *pick_buffer);
    
    int		renderTetPrims(int connect_start,
			       int connect_num,
			       GR_PickCompMode id_mode,
			       GR_PickStyle style,
			       bool has_pick_map,
			       bool wireframe,
			       GR_SelectMode select_mode,
			       int select_inst,
			       RE_Geometry *pick_buffer);

    int		renderFaceDeformingPrims(int connect_start,
					 int connect_num,
					 GR_PickCompMode id_mode,
					 GR_PickStyle style,
					 bool has_pick_map,
					 bool wireframe,
					 GR_SelectMode select_mode,
					 int select_inst,
					 RE_Geometry *pick_buffer);

    int		renderSubDLinePrims(int connect_start,
				    int connect_num,
				    GR_PickCompMode id_mode,
				    GR_PickStyle style,
				    bool has_pick_map,
				    bool wireframe,
				    GR_SelectMode select_mode,
				    int select_inst,
				    RE_Geometry *pick_buffer,
				    UT_Vector2F lod_parms);
    
    // edge picks
    int		renderFaceEdges(int connect_start,
				int connect_num,
				GR_PickCompMode point_id_mode,
				bool midpoints,
				GR_PickStyle style,
				bool has_pick_map,
				GR_SelectMode select_mode,
				int select_inst,
				RE_Geometry *pick_buffer);
    int		renderMeshEdges(int connect_start,
				int connect_num,
				GR_PickCompMode point_id_mode,
				bool midpoints,
				GR_PickStyle style,
				bool has_pick_map,
				GR_SelectMode select_mode,
				int select_inst,
				RE_Geometry *pick_buffer);
    int		renderLineEdges(int connect_start,
				int connect_num,
				GR_PickCompMode point_id_mode,
				bool midpoints,
				GR_PickStyle style,
				bool has_pick_map,
				bool wireframe,
				GR_SelectMode select_mode,
				int select_inst,
				RE_Geometry *pick_buffer);

    // vertex picks
    int		renderFaceVertices(int connect_start,
				   int connect_num,
				   GR_PickCompMode id_mode,
				   GR_PickStyle style,
				   bool has_pick_map,
				   GR_SelectMode select_mode,
				   int select_inst,
				   RE_Geometry *pick_buffer);

    int		renderLineVertices(int connect_start,
				   int connect_num,
				   GR_PickCompMode id_mode,
				   GR_PickStyle style,
				   bool has_pick_map,
				   GR_SelectMode select_mode,
				   int select_inst,
				   RE_Geometry *pick_buffer);
    int		renderMeshVertices(int connect_start,
				   int connect_num,
				   GR_PickCompMode id_mode,
				   GR_PickStyle style,
				   bool has_pick_map,
				   GR_SelectMode select_mode,
				   int select_inst,
				   RE_Geometry *pick_buffer);
    // Breakpoints
    int		renderBreakpoints(int connect_start,
				  int connect_num,
				  GR_PickCompMode id_mode,
				  GR_PickStyle style,
				  bool has_pick_map,
				  bool wireframe,
				  GR_SelectMode select_mode,
				  int select_inst,
				  RE_Geometry *pick_buffer);

    // Guides
    int		renderGuidePoints(int connect_start,
				  int connect_num,
				  GR_PickCompMode prim_id_mode,
				  GR_PickStyle style,
				  GR_SelectMode select_mode,
				  int select_inst);
    int		renderGuideEdges(int connect_start,
				 int connect_num,
				 GR_PickCompMode prim_id_mode,
				 GR_PickStyle style,
				 GR_SelectMode select_mode,
				 int select_inst);

    // Normals
    int		renderPointNormals(int connect_start,
				   int connect_num,
				   GR_PickCompMode id_mode,
				   GR_PickStyle style,
				   bool has_pick_map,
				   bool normalize);
    int		renderFaceNormals(int connect_start,
				  int connect_num,
				  GR_PickStyle style,
				  bool has_pick_map);

    // converts raw ivec4 picks into GL-selection buffer picks, and returns
    // the number of picks.
    static int	filterPicks(UT_Array<GR_PickRecord> &pick_records,
			    const int32 *raw_picks,
			    int num_raw_picks,
			    int vector_size,
			    const UT_Vector3i &base_id);

protected:
    void	pickPointsSingle(int connect_start,
				 int connect_num,
				 RE_PrimType real_prim_type,
				 int real_connect_start,
				 int real_connect_num,
				 GR_PickCompMode id_mode,
				 GR_SelectMode select_mode,
				 int select_inst,
				 bool hide_selected);
    int		pickPointsMulti(int connect_start,
				int connect_num,
				GR_PickCompMode id_mode,
				GR_SelectMode select_mode,
				int select_inst,
				RE_Geometry *pick_buffer,
				bool visible_only);
    int		pickPointsUVVert(int connect_start,
				 int connect_num,
				 GR_PickStyle style,
				 GR_PickCompMode id_mode,
				 GR_SelectMode select_mode,
				 int select_inst,
				 RE_Geometry *pick_buffer);
   
    void	pickFacesSingle(int connect_start,
				int connect_num,
				GR_PickCompMode id_mode,
				GR_PickStyle style,
				bool wireframe,
				GR_SelectMode select_mode,
				int select_inst,
				bool hide_selected,
				bool deforming_faces);
    int		pickFacesMulti(int connect_start,
			       int connect_num,
			       GR_PickCompMode id_mode,
			       GR_SelectMode select_mode,
			       int select_inst,
			       RE_Geometry *pick_buffer,
			       bool visible_only,
			       bool has_pick_map,
			       bool wireframe,
			       bool deforming_faces,
			       bool pick_outside);
   
    int		pickFaceEdgesMulti(int connect_start,
				   int connect_num,
				   GR_PickCompMode point_id_mode,
				   GR_SelectMode select_mode,
				   int select_inst,
				   RE_Geometry *pick_buffer,
				   bool visible_only,
				   bool has_pick_map,
				   bool pick_outside);

    int		pickMeshEdgesMulti(int connect_start,
				   int connect_num,
				   GR_PickCompMode point_id_mode,
				   GR_SelectMode select_mode,
				   int select_inst,
				   RE_Geometry *pick_buffer,
				   bool visible_only,
				   bool contained,
				   bool has_pick_map);
    
    void	pickFaceVerticesSingle(int connect_start,
				       int connect_num,
				       GR_PickCompMode id_mode,
				       GR_SelectMode select_mode,
				       int select_inst,
				       bool hide_selected);
    int		pickFaceVerticesMulti(int connect_start,
				      int connect_num,
				      GR_PickCompMode id_mode,
				      GR_PickStyle style,
				      GR_SelectMode select_mode,
				      int select_inst,
				      RE_Geometry *pick_buffer);

    void	pickTetPrimSingle(int connect_start,
				  int connect_num,
				  GR_PickCompMode id_mode,
				  bool wireframe,
				  GR_SelectMode select_mode,
				  int select_inst,
				  bool hide_selected);
    int		pickTetPrimMulti(int connect_start,
				 int connect_num,
				 GR_PickCompMode id_mode,
				 GR_SelectMode select_mode,
				 int select_inst,
				 RE_Geometry *pick_buffer,
				 bool visible_only,
				 bool has_pick_map,
				 bool wireframe);

    bool	isInstanced(int &num_inst) const;
    void	drawGeo(int connect_start,
			int connect_end,
			GR_SelectMode select_mode,
			int select_inst);

    RE_Render				*r;
    const GR_DisplayOption		*opts;
    const GR_RenderInfo			*info;
    RE_Geometry				*geo;
    GR_GeoRender			*myGeoRender;
    UT_UniquePtr<RE_OverrideList>	 myOverrides;
};
#endif
