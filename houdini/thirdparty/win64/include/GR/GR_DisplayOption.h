/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_DisplayOption.h
 *
 * COMMENTS:	Drawing options for the render library.  Authored by Mark.
 *
 */


#ifndef __GR_DisplayOption_h__
#define __GR_DisplayOption_h__

#include "GR_API.h"
#include <string.h>
#include <UT/UT_Defines.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_Color.h>
#include <UT/UT_Array.h>
#include "GR_Defines.h"
#include "GR_CommonDispOption.h"
#include "GR_OptionTable.h"
#include "GR_UserOption.h"

class RE_Render;
class RE_LightList;

#define ASSIGN_BUMP(flag) if(myFlags.flag != onoff) mySerialNum++; \
			  myFlags.flag=onoff

class GR_UserOption;

class GR_API GR_DisplayOption
{
public:
    GR_DisplayOption(GR_CommonDispOption *common_options = 0, int t = 0);
    ~GR_DisplayOption();
    GR_DisplayOption(const GR_DisplayOption &s);

    // Return the display set type this option set represents
    int			 getDisplaySet() const { return myDisplaySet; }

    GR_DisplayOption	&operator=(const GR_DisplayOption &opt);

    // Copy all but the common display options from the given GR_DisplayOption
    // object.
    void		 copyNonCommon(const GR_DisplayOption &opt);

    // Copy the user options (not done by copyNonCommon).
    void		 copyUserOptions(const GR_DisplayOption &opt);

    // Refresh the version key for all of the enabled user options from the
    // global option table.
    void		 refreshUserOptionVersions();

    // Return whether or not the specified object has the same set of enabled
    // user options.  Version keys are ignored.
    bool		 userOptionStatesEqual(
				    const GR_DisplayOption &opt) const;

    // Set the enabled user options to match the specified object.  Note that
    // version keys are also copied.  However, this is irrelevant as version
    // keys only matter while drawing and will have been refreshed in that
    // case anyway.
    void		 matchUserOptionStates(const GR_DisplayOption &opt);

    // Class I/O methods.  Return true if successful, false if not.
    bool	 save(std::ostream &, int indent=0) const;
    bool	 load(UT_IStream &);

    const char	*className() const;

    // Set the shading mode.  The draw mode is either wireframe or shaded,
    // but the shading mode can be hiddenwire, flag shaded, wire over gouraud,
    // etc.  Changing the shading mode will change the drawing mode as well as
    // other flags in this class.
    void	 shadingMode(GR_ShadingMode shading_mode);

    // Find which shading mode we're currently using.
    GR_ShadingMode shadingMode() const;

    // Utility methods to convert a GR_BoundaryMode to/from a token.  The parse
    // method returns GR_BOUNDARY_NONE on a failure.
    static void	saveBoundaryMode(std::ostream &os, GR_BoundaryMode mode);
    static GR_BoundaryMode parseBoundaryMode(const char *token,
					     bool for_uv_view = false);

    // Utility methods to convert a GR_ShadingMode to/from a token.  The parse
    // method returns -1 on a failure.
    static void	saveShadingMode(std::ostream &os, GR_ShadingMode mode);
    static int	parseShadingMode(const char *token);

    // "Get" methods.
    bool 	drawPointMarkers() const { return myFlags.drawPointMarkers;    }
    bool 	drawPointNums	() const { return myFlags.drawPointNums;       }
    bool 	drawPointNmls   () const { return myFlags.drawPointNmls;       }
    bool 	drawPointTxtNums() const { return myFlags.drawPointTxtNums;    }
    bool 	drawPointPos	() const { return myFlags.drawPointPos;        }
    bool 	drawPointVelocity() const { return myFlags.drawPointVelocity;  }
    bool 	drawPointCoincident() const
					 { return myFlags.drawPointCoincident; }
    bool 	drawVertexMarkers() const{ return myFlags.drawVertexMarkers;   }
    bool 	drawVertexNums	() const { return myFlags.drawVertexNums;      }
    bool  	drawVertexTxtNums() const { return myFlags.drawVertexTxtNums;  }
    bool	drawVertexNmls  () const { return myFlags.drawVertexNmls;      }
    bool 	drawPrimNums	() const { return myFlags.drawPrimNums;        }
    bool 	drawPrimNmls	() const { return myFlags.drawPrimNmls;        }
    bool 	drawPrimHulls	() const { return myFlags.drawPrimHulls;       }
    bool	drawPrimBreakpoints() const{return myFlags.drawPrimBreakpoints;}
    bool 	drawProfiles	() const { return myFlags.drawProfiles;        }
    bool 	drawProfNums	() const { return myFlags.drawProfNums;	       }
    bool	drawLit		() const { return myFlags.drawLit;	       }
    bool	drawFaded	() const { return myFlags.drawFaded;	       }
    bool	drawGhosted	() const { return myFlags.drawGhosted;	       }
    bool	drawXRay	() const { return myFlags.drawXRay;	       }
    bool	appliesToSelection() const { return myFlags.appliesToSelection;}
    bool	toggleAffectsSet() const { return myFlags.toggleAffectsSet;    }
    bool	boundBox	() const;
    bool	hlGhostFill	() const;
    bool	hlXray		() const;
    int 	drawMode	() const;
    bool	wireHiddenLine	() const;
    bool	flatShaded	() const;
    bool	wireOverShaded	() const;
    
    bool	lockShadingMode () const { return myFlags.lockShadingMode;     }
    bool	uvFillBackFaces	() const { return myFlags.uvFillBackFaces;     }
    bool	uvFillOverlap	() const { return myFlags.uvFillOverlap;       }
    bool	tintBackfaces   () const { return myFlags.tintBackfaces; }

    // "Set" methods.
    void	drawPointMarkers(bool onoff){ASSIGN_BUMP(drawPointMarkers);}
    void	drawPointNums(bool onoff)    { ASSIGN_BUMP(drawPointNums);}
    void	drawPointNmls(bool onoff)    { ASSIGN_BUMP(drawPointNmls);}
    void	drawPointTxtNums(bool onoff) { ASSIGN_BUMP(drawPointTxtNums);}
    void	drawPointPos(bool onoff)     { ASSIGN_BUMP(drawPointPos);}
    void	drawPointVelocity(bool onoff){ ASSIGN_BUMP(drawPointVelocity);}
    void	drawPointCoincident(bool onoff)
				{ ASSIGN_BUMP(drawPointCoincident);}
    void	drawVertexMarkers(bool onoff)
				{ ASSIGN_BUMP(drawVertexMarkers);}
    void	drawVertexNums(bool onoff)   { ASSIGN_BUMP(drawVertexNums);}
    void	drawVertexNmls(bool onoff)   { ASSIGN_BUMP(drawVertexNmls);}
    void	drawVertexTxtNums(bool onoff){ ASSIGN_BUMP(drawVertexTxtNums);}
    void	drawPrimNums	(bool onoff) { ASSIGN_BUMP(drawPrimNums);}
    void	drawPrimNmls	(bool onoff) { ASSIGN_BUMP(drawPrimNmls);}
    void	drawPrimHulls	(bool onoff) { ASSIGN_BUMP(drawPrimHulls);}
    void	drawPrimBreakpoints (bool onoff)
			        { ASSIGN_BUMP(drawPrimBreakpoints);}

    void	drawProfiles	(bool onoff) { myFlags.drawProfiles	=onoff;}
    void	drawProfNums	(bool onoff) { myFlags.drawProfNums	=onoff;}
    void	drawLit		(bool onoff) { myFlags.drawLit		=onoff;}
    void	drawFaded	(bool onoff) { myFlags.drawFaded	=onoff;}
    void	drawGhosted	(bool onoff) { myFlags.drawGhosted	=onoff;}
    void	drawXRay	(bool onoff) { myFlags.drawXRay		=onoff;}
    void	appliesToSelection(bool onoff)
				{ myFlags.appliesToSelection = onoff; }
    void	toggleAffectsSet(bool onoff) { myFlags.toggleAffectsSet	=onoff;}
    void	lockShadingMode (bool onoff) { myFlags.lockShadingMode  =onoff;}
    void	uvFillBackFaces	(bool onoff) { myFlags.uvFillBackFaces	=onoff;}
    void	uvFillOverlap	(bool onoff) { myFlags.uvFillOverlap	=onoff;}
    void	tintBackfaces   (bool onoff) { myFlags.tintBackfaces    =onoff;}

    // Decorations with enhanced visibility options.
    void	pointMarkersVisible(GR_DecorVisibility m);
    void	pointNumsVisible(GR_DecorVisibility m);
    void	pointNmlsVisible(GR_DecorVisibility m);
    void	pointTxtNumsVisible(GR_DecorVisibility m);
    void	pointPosVisible(GR_DecorVisibility m);
    void	pointVelocityVisible(GR_DecorVisibility m);
    void	vertexMarkersVisible(GR_DecorVisibility m);
    void	vertexNumsVisible(GR_DecorVisibility m);
    void	vertexNmlsVisible(GR_DecorVisibility m);
    void	vertexTxtNumsVisible(GR_DecorVisibility m);
    void	primNumsVisible(GR_DecorVisibility m);
    void	primNmlsVisible(GR_DecorVisibility m);
    void	primBreakpointsVisible(GR_DecorVisibility m);
    
    GR_DecorVisibility 	pointMarkersVisible() const;
    GR_DecorVisibility 	pointNumsVisible() const;
    GR_DecorVisibility 	pointNmlsVisible   () const;
    GR_DecorVisibility 	pointTxtNumsVisible() const;
    GR_DecorVisibility 	pointPosVisible	() const;
    GR_DecorVisibility 	pointVelocityVisible	() const;
    GR_DecorVisibility 	vertexMarkersVisible() const;
    GR_DecorVisibility 	vertexNumsVisible() const;
    GR_DecorVisibility 	vertexNmlsVisible() const;
    GR_DecorVisibility 	vertexTxtNumsVisible() const;
    GR_DecorVisibility 	primNumsVisible() const;
    GR_DecorVisibility 	primNmlsVisible() const;
    GR_DecorVisibility 	primBreakpointsVisible() const;

    GR_DecorVisibility	getVisibilityFor(GR_Decoration d) const;

    GR_BoundaryMode	boundaryMode() const { return myBoundaryMode; }
    void		boundaryMode(GR_BoundaryMode mode)
			{
			    if (mode != myBoundaryMode)
			    {
				myBoundaryMode = mode;
				mySerialNum++;
			    }
			}
    GR_BoundaryMode	uvBoundaryMode() const { return myBoundaryUVMode; }
    void		uvBoundaryMode(GR_BoundaryMode mode)
			{
			    if (mode != myBoundaryUVMode)
			    {
				myBoundaryUVMode = mode;
				mySerialNum++;
			    }
			}

    GR_CommonDispOption&common() { return *myCommonOptions; }
    const GR_CommonDispOption&common() const { return *myCommonOptions; }

    void	 enableUserOption(const char *name, bool onoff);
    bool	 getUserOptionState(const char *name) const;
    void	 disableAllUserOptions();

    bool	 hasEnabledUserOptions() const
				{ return myUserOptions.entries() > 0; }

    // This method is called when user options are being destroyed.  We need
    // to take the option_id since there is no way at that time for us to map
    // an option name to the id.
    void	 removeUserOption(int option_id);

    // Methods to query the enabled user options.
    int		 getNumUserOptions() const { return myUserOptions.entries(); }
    const GR_UserOption	*getUserOption(int i) const;
    const GR_UserOption	*getOption(const char *name) const;
    int		 getOptionId(int i) const;

    // Display list support
    void	clearNonCachedDrawFlag() const { myNonCachedDrawFlag = false; }
    void	setNonCachedDrawFlag() const   { myNonCachedDrawFlag = true; }
    bool	hasNonCachedDraw() const { return myNonCachedDrawFlag ; }

    void	setNonCachedPass(bool nc_pass) { myNonCachedPass = nc_pass; }
    bool	isNonCachedPass() const	       { return myNonCachedPass; }

    void		setLightList(const RE_LightList *list) {myLights=list;}
    const RE_LightList *getLightList() const { return myLights; }

    int		getSerialNum() const { return (common().getSerialNum() +
					       mySerialNum + myRefineSerial); }
    int		getRefineSerial() const { return myRefineSerial; }
    void	bumpRefineSerial() { myRefineSerial++; }
    int64	getUniqueID() const { return int64(mySerialNum)<<int64(32) |
					     int64(common().getUniqueID()); }

    void	print(std::ostream &os) const;

    // return true if 'opt' uses the same shading mode and would request the
    // same attributes as this display option set.
    bool	compatibleWith(const GR_DisplayOption &opt) const;

    int		getVisContext() const { return common().getVisContext(); }

private:
    GR_CommonDispOption *myCommonOptions;
    int			 myDisplaySet;
    bool		 myOwnCommonOptionsFlag;

    //
    // A simple class to store the unique id and version of a GR_UserOption.
    // This needs to be here or else the UT_Array template won't be able
    // to compile on Windows.
    //
    class gr_UserOptionRef
    {
    public:
	gr_UserOptionRef(): myId(-1), myVersion(-1) {}
	gr_UserOptionRef( int id, int version )
	    : myId(id), myVersion(version) {}

	int		 operator==(const gr_UserOptionRef &opt)
		     { return myId == opt.myId && myVersion == opt.myVersion; }
	static int	 compareID(const gr_UserOptionRef *r1,
			       const gr_UserOptionRef *r2)
		     {
			 return r1->myId - r2->myId;
		     }

	int		 myId;
	int		 myVersion;
    };

    // This array consists only of references to the enabled user options, and
    // is stored sorted by option id for performance reasons.
    UT_Array<gr_UserOptionRef>	myUserOptions;

    struct grDrawFlags
    {
    unsigned	 drawPointMarkers :1,	// draw points 
		 drawPointNums	  :1,	// draw point numbers
		 drawPointNmls	  :1,	// draw point normals
		 drawPointTxtNums :1,	// draw point texture numbers
		 drawPointPos     :1,	// draw point positions 
		 drawPointVelocity:1,	// draw point velocity (v)
		 drawPointCoincident:1, // draw # of point coincidences 

		 drawVertexMarkers:1,	// draw vertex markers
		 drawVertexNums	  :1,	// draw vertex numbers
		 drawVertexNmls   :1,	// draw vertex normals
		 drawVertexTxtNums:1,	// draw vertex texture numbers

		 drawPrimNums	  :1,	// draw primitive numbers
		 drawPrimNmls	  :1,	// draw primitive normals
		 drawPrimHulls	  :1,	// draw Spline prim hulls
		 drawPrimBreakpoints:1,	// draw primitive break points

		 drawProfiles	  :1,	// draw profiles
		 drawProfNums	  :1,	// draw profile numbers
		 drawFaded	  :1,	// draw without point colors or textures
		 drawGhosted	  :1,	// draw blended with BG color
	
		 appliesToSelection:1,	// use for selected or non-selected?
		 toggleAffectsSet:1,    // RHS toolbar toggles affect this set

		 drawLit	  :1,
		 drawXRay	  :1,
		 lockShadingMode  :1,
		 uvFillBackFaces  :1,
		 uvFillOverlap	  :1,
		 tintBackfaces    :1;
    };
    grDrawFlags		myFlags;
    GR_BoundaryMode	myBoundaryMode;
    GR_BoundaryMode	myBoundaryUVMode;
    GR_ShadingMode	myShadingMode;
    mutable bool	myNonCachedDrawFlag;
    bool		myNonCachedPass;
    const RE_LightList *myLights;
    int			mySerialNum;
    int			myUniqueID;
    int			myRefineSerial;

    GR_DecorVisibility myVisibility[GR_MAX_DECORATIONS];
};

inline void
GR_DisplayOption::pointMarkersVisible(GR_DecorVisibility m)
{ myVisibility[GR_POINT_MARKER] = m;}

inline void
GR_DisplayOption::pointNumsVisible(GR_DecorVisibility m)
{ myVisibility[GR_POINT_NUMBER]	  = m; }

inline void
GR_DisplayOption::pointNmlsVisible(GR_DecorVisibility m)
{ myVisibility[GR_POINT_NORMAL]	  = m; }

inline void
GR_DisplayOption::pointTxtNumsVisible(GR_DecorVisibility m)
{ myVisibility[GR_POINT_UV] = m; }

inline void
GR_DisplayOption::pointPosVisible(GR_DecorVisibility m)
{ myVisibility[GR_POINT_POSITION]  = m; }

inline void
GR_DisplayOption::pointVelocityVisible(GR_DecorVisibility m)
{ myVisibility[GR_POINT_VELOCITY]  = m; }

inline void
GR_DisplayOption::vertexMarkersVisible(GR_DecorVisibility m)
{ myVisibility[GR_VERTEX_MARKER]  = m; }

inline void
GR_DisplayOption::vertexNumsVisible(GR_DecorVisibility m)
{ myVisibility[GR_VERTEX_NUMBER]  = m; }

inline void
GR_DisplayOption::vertexNmlsVisible(GR_DecorVisibility m)
{ myVisibility[GR_VERTEX_NORMAL]  = m; }

inline void
GR_DisplayOption::vertexTxtNumsVisible(GR_DecorVisibility m)
{ myVisibility[GR_VERTEX_UV] = m; }

inline void
GR_DisplayOption::primNumsVisible(GR_DecorVisibility m)
{ myVisibility[GR_PRIM_NUMBER]	  = m; }

inline void
GR_DisplayOption::primNmlsVisible(GR_DecorVisibility m)
{ myVisibility[GR_PRIM_NORMAL]	  = m; }

inline void
GR_DisplayOption::primBreakpointsVisible(GR_DecorVisibility m)
{ myVisibility[GR_PRIM_BREAKPOINT]	  = m; }

inline GR_DecorVisibility
GR_DisplayOption::pointMarkersVisible() const
{ return myVisibility[GR_POINT_MARKER];   }

inline GR_DecorVisibility
GR_DisplayOption::pointNumsVisible() const
{ return myVisibility[GR_POINT_NUMBER];      }

inline GR_DecorVisibility
GR_DisplayOption::pointNmlsVisible() const
{ return myVisibility[GR_POINT_NORMAL];      }

inline GR_DecorVisibility
GR_DisplayOption::pointTxtNumsVisible() const
{ return myVisibility[GR_POINT_UV];   }

inline GR_DecorVisibility
GR_DisplayOption::pointPosVisible() const
{ return myVisibility[GR_POINT_POSITION];      }

inline GR_DecorVisibility
GR_DisplayOption::pointVelocityVisible() const
{ return myVisibility[GR_POINT_VELOCITY];      }

inline GR_DecorVisibility
GR_DisplayOption::vertexMarkersVisible() const
{ return myVisibility[GR_VERTEX_MARKER];     }

inline GR_DecorVisibility
GR_DisplayOption::vertexNumsVisible() const
{ return myVisibility[GR_VERTEX_NUMBER];     }

inline GR_DecorVisibility
GR_DisplayOption::vertexNmlsVisible() const
{ return myVisibility[GR_VERTEX_NORMAL];     }

inline GR_DecorVisibility
GR_DisplayOption::vertexTxtNumsVisible() const
{ return myVisibility[GR_VERTEX_UV]; }

inline GR_DecorVisibility
GR_DisplayOption::primNumsVisible() const
{ return myVisibility[GR_PRIM_NUMBER];       }

inline GR_DecorVisibility
GR_DisplayOption::primNmlsVisible() const
{ return myVisibility[GR_PRIM_NORMAL];       }

inline GR_DecorVisibility
GR_DisplayOption::primBreakpointsVisible() const
{ return myVisibility[GR_PRIM_BREAKPOINT];       }

inline GR_DecorVisibility
GR_DisplayOption::getVisibilityFor(GR_Decoration d) const
{
    if(d < GR_USER_DECORATION)
	return myVisibility[d];
    
    return GRgetOptionTable()->getOption(d-GR_USER_DECORATION)->getVisibility();
}

inline bool
GR_DisplayOption::boundBox() const
{
    return (myShadingMode == GR_BOUNDING_BOX_WIRE ||
	    myShadingMode == GR_BOUNDING_BOX_SHADED);
}

inline bool
GR_DisplayOption::hlGhostFill() const
{
    return myShadingMode == GR_HIDDEN_LINE_GHOST;
}

inline bool
GR_DisplayOption::hlXray() const
{
    return myShadingMode == GR_WIRE_GHOST;
}

inline int
GR_DisplayOption::drawMode() const
{
    if(myShadingMode >= GR_WIRE && myShadingMode <= GR_HIDDEN_LINE_GHOST)
	return GR_DRAW_WIREFRAME;
    else if(myShadingMode >= GR_FLAT_SHADED)
	return GR_DRAW_SHADED;

    return 0; // no enum for bbox
}

inline bool
GR_DisplayOption::wireHiddenLine() const
{
    return (myShadingMode == GR_WIRE_GHOST ||
	    myShadingMode == GR_HIDDEN_LINE_INVISIBLE ||
	    myShadingMode == GR_HIDDEN_LINE_GHOST);
}

inline bool
GR_DisplayOption::flatShaded() const
{
    return (myShadingMode == GR_FLAT_SHADED ||
	    myShadingMode == GR_FLAT_WIRE_SHADED);
}

inline bool
GR_DisplayOption::wireOverShaded() const
{
    return (myShadingMode == GR_SMOOTH_WIRE_SHADED ||
	    myShadingMode == GR_FLAT_WIRE_SHADED);
}


#endif
