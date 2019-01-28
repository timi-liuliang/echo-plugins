/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_DrawParms.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Parameter class containing all parameters for GR_Primitive::draw()
 */
#ifndef GR_DrawParms_h
#define GR_DrawParms_h

class GR_DisplayOption;
class RE_MaterialAtlas;

class GR_DrawParms
{
public:
    const GR_DisplayOption *opts;
    const RE_MaterialAtlas *materials;
    int			    default_mat_id;
    bool		    draw_instanced;
    int			    instance_group;
    bool		    hide_obj_instances;
    bool		    allow_tinting;
    bool		    force_visible;

    GR_DrawParms(const GR_DisplayOption *o,
		 const RE_MaterialAtlas *m,
		 int def_mat_id,
		 bool inst,
		 int  inst_group,
		 bool hide_inst = false,
		 bool allow_tint = true,
		 bool force_vis = false)
	: draw_instanced(inst),
	  opts(o),
	  materials(m),
	  default_mat_id(def_mat_id),
	  instance_group(inst_group),
	  hide_obj_instances(hide_inst),
	  allow_tinting(true),
	  force_visible(force_vis)
	{}
};

#endif
