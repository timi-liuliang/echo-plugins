/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Conversion Utility Library (C++)
 *
 * COMMENTS:
 *	Save a gdp to RIB.  This is not able to load or convert any files.
 *
 */

#ifndef __GEO_IORib_h__
#define __GEO_IORib_h__

#include "GEO_API.h"
#include "GEO_IOTranslator.h"
#include "GEO_IORibExt.h"

#include <UT/UT_BitArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Array.h>
#include <GA/GA_AttributeRef.h>
#include <UT/UT_VectorTypes.h>
#include <TS/TS_Expression.h>
#include <iosfwd>

class GEO_Detail;
class GEO_Primitive;
class GEO_PrimPoly;
class GEO_Vertex;
class GEO_IORibXlate;
class GEO_IORibPrinter;
class GEO_PrimParticle;
class GA_PrimitiveGroup;

#define GEO_RIB_MAX_SEGMENTS	5

class GEO_API GEO_IORib : public GEO_IOTranslator
{
public:
	     GEO_IORib();
    virtual ~GEO_IORib();

    /// This method is used to create a copy of the sub-class
    virtual GEO_IOTranslator	*duplicate() const;

    virtual const char	*formatName() const;

    virtual int		 checkExtension(const char *name);
    virtual int		 checkMagicNumber(unsigned magic);

    virtual GA_Detail::IOStatus	 fileSave(const GEO_Detail *gdp, std::ostream &);
    virtual GA_Detail::IOStatus	 fileLoad(GEO_Detail *, UT_IStream &, bool);

    // This method does not duplicate the geometry, make sure it's stays
    //	around while fileSave is called!
    void		 setMBGeo(const GEO_Detail *gdp) { myGdps[1] = gdp; }
    // Add gdp's in sequence
    void		 addMBGeo(const GEO_Detail *gdp);
    void		 setMBParticle(int onoff) { myBlurParticle = onoff; }
    void		 setOutputPrimGroup( const GA_PrimitiveGroup *grp )
			 {
			     myOutPrimGroup = grp;
			 }

    //
    //	Setting the version of the renderer turns on extensions to the RIB
    //	stream.  These extensions are defined in GEO_IORibExt.h.  The different
    //	renderers we expect to see are:
    //		"rdc"	- RenderDotC		(Support Version 3.1)
    //		"bmrt"	- BMRT			(No extensions supported)
    //		"prman"	- Pixar's RenderMan	(Support Version 3.6 to 10.0)
    //
    void		 setVersion(const char *renderer,
				    int major=3, int minor=6)
			 {
			     myExtensions.setRenderer(renderer, major, minor);
			 }
    // GEO_IO is being used for IFDs.  If it's not flagged as being used for
    // IFD's, shading interpolation will be set on and the normal attribute
    // will be declared.
    void		 setForIFD(int onoff=1)	{ myIFDFlag = onoff; }

    void		 setCurveBasis(const char *basis);
    void		 setPolySubdivide(int on);
    void		 setPoints(int on, float size=1);

    // The shutter speed is used for particle blur.
    void		 setShutterSpeed(float shutter=1)
			 {
			     myShutterTime = shutter;
			 }

    // The following are public, but shouldn't really be called unless
    //	you really know what you're doing...
    static void		 outputQuadricAttributes(GEO_IORibPrinter &,
				const GEO_Primitive *, GA_Size vtx,
				int dup = 4, float pscale = 1);
    // This returns 1 if the "width" parameter was found, 0 if not.
    static int		 outputParticleAttributes(GEO_IORibPrinter &out,
					 const GEO_PrimParticle *part,
					 float pscale=1, int dup=1,
					 int allow_N=0);

private:
    int			 isBlurred() const	{ return myGdps[1] != 0; }
    int			 hasExtension(int ext)
			 {
			     return myExtensions.hasExtension(ext);
			 }

    const GEO_Detail	*myGdps[GEO_RIB_MAX_SEGMENTS];
    int			 mySegments;
    GA_ROHandleV3	 ptVel;		// Point velocity
    int			 myBlurParticle;	// Motion blur particles

    unsigned		 myGeoFlag;	// For geometry interpretation
    float		 myPointSize;	// Size for RiPoints()
    float		 myShutterTime;

    const GA_PrimitiveGroup	*myOutPrimGroup;	// Motion blur gdp

#if 0
    int			   myParticleIndex;
    UT_Array<GEO_Detail*> *myParticleDetails;
    UT_Array<GEO_Detail*> *myParticleMBDetails;
#endif

    void	outputPolysAttrib(GEO_IORibPrinter &, const GEO_Detail *,
			    const UT_BitArray &, const GEO_IORibXlate *,
			    const UT_IntArray *group);
    void	outputBlobbyAttrib(GEO_IORibPrinter &, const GEO_Detail *,
				    const GEO_IORibXlate *,
				    UT_Array<TS_MetaPrimitive *> &prims);
    void	outputCurvesAttrib(GEO_IORibPrinter &, const GEO_Detail *,
			    const GEO_IORibXlate *,
			    const char *curve_interp,
			    int curve_step);

    void	outputParticleLinePosition(GEO_IORibPrinter &out,
					   const GEO_PrimParticle *part,
					   const GA_ROHandleV3 &vel,
					   float vscale);
    void	outputMatrix (GEO_IORibPrinter &out, const UT_Matrix4 &xform);
    void	outputBlobbies(GEO_IORibPrinter &out, int nblobs,
				    const GEO_Detail *gdp);
    int		outputPolys(GEO_IORibPrinter &out, const GEO_Detail *gdp);
    void	outputSubDivs(GEO_IORibPrinter &out);
    void	outputSubDivGroup(GEO_IORibPrinter &out,
				const GEO_Detail *gdp,
				UT_IntArray &group, // may be modified !
				bool first_time);
    int		outputPolyCurves(GEO_IORibPrinter &out, const GEO_Detail *gdp,
				 const char *curve_interp, int curve_step);
    void	outputNURBSCurves(GEO_IORibPrinter &out, const GEO_Detail *gdp);
    void	outputMesh   (GEO_IORibPrinter &out, const GEO_Primitive *prim);
    void	outputNURBS  (GEO_IORibPrinter &out, const GEO_Primitive *prim);
    void	outputBezier (GEO_IORibPrinter &out, const GEO_Primitive *prim);
    void	outputSphere (GEO_IORibPrinter &out, const GEO_Primitive *p[]);
    void	outputTube   (GEO_IORibPrinter &out, const GEO_Primitive *p[]);
    void	outputCircle (GEO_IORibPrinter &out, const GEO_Primitive *p[]);
    void	outputPoints (GEO_IORibPrinter &out, const GEO_Detail *gdp);
    void	outputParticles(GEO_IORibPrinter &out,
				const GEO_Primitive *prim);
    void	outputParticlePoints(GEO_IORibPrinter &out,
					const GEO_PrimParticle *part,
					const GA_ROHandleV3 &vel,
					float vscale, float pscale);
    void	outputParticleLines(GEO_IORibPrinter &out,
					const GEO_PrimParticle *part,
					float pscale);

    int 	isOutputPrim( const GEO_Primitive *prim );


    static void	outputExpressionBlobbies(GEO_IORibPrinter &out, 
					 const TS_MetaExpressionPtr &expr, 
					 int &floatAddr, int &exprNum);

    GEO_IORibExt		 myExtensions;
    UT_Array<GEO_IORibXlate *>	 myTranslators;

    int				 myIFDFlag;
};

// A wrapper class that implements buffered C-style output.
class GEO_API GEO_IORibPrinter
{
public:
		 GEO_IORibPrinter(std::ostream &os);
		~GEO_IORibPrinter();

    void	 print(const char *fmt, ...);
    void	 print(const UT_Matrix4 &mat);

    std::ostream &getStream();
    void	 flush();
private:
    static const int	 theBufferSize;
    static const int	 theBlockSize;

    std::ostream &myStream;
    unsigned	 mySize;
    unsigned	 myLineLen;
    char	*myFreeStart;
    char	*myFileBuffer;
};


#endif
