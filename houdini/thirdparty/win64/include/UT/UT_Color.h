#ifndef	_UT_Color_
#define	_UT_Color_

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include <iosfwd>

class UT_IStream;
class UT_JSONWriter;
class UT_JSONParser;
class UT_JSONValue;
class UT_Ramp;

//
// Each color space has its own range for each of the three parameters.
//
// RGB: 0..1, 0..1, 0..1
// HSL: 0..360, 0..1, 0..1
// HSV: 0..360, 0..1, 0..1
// XYZ: 0..1, 0..1, 0..1 work well 
// LAB: 0..100, a/b are unbounded but TIFF uses [-128, 127]
// TMI: -1..1, -1..1, -1..1

enum UT_ColorType {
	UT_HSL, UT_HSV, UT_RGB, UT_XYZ, UT_LAB, UT_TMI
};

// Conversion of UT_ColorType values to/from string for saving/loading.
UT_API extern const char *  UTnameFromColorType(UT_ColorType ctype);
UT_API extern UT_ColorType  UTcolorTypeFromName(const char *cname);

enum UT_ColorRamp {
    UT_COLORRAMP_FALSE,		// False colour
    UT_COLORRAMP_PINK,		// White-red
    UT_COLORRAMP_MONO,		// Black-white
    UT_COLORRAMP_BLACKBODY,	// Black-red-yellow-white
    UT_COLORRAMP_BIPARTITE,	// Cyan-blue then Red-yellow
    UT_COLORRAMP_NUMMODES	// Illegal value.
};

enum UT_MonoRamp {
    UT_MONORAMP_INCREASING,	// Slash
    UT_MONORAMP_DECREASING,	// Backslash
    UT_MONORAMP_HILL,		// slash backslash
    UT_MONORAMP_VALLEY,		// Backslash slash
    UT_MONORAMP_STEP,		// Underscore Overbar
    UT_MONORAMP_SQUARE,		// Pipe Overbar Pipe
    UT_MONORAMP_NUMMODES	// Illegal value.
};

class UT_Color;
#include "UT_VectorTypes.h"

//
//  Filter functions useable with UI_Vector class:
//
UT_API extern void		UT_RGBtoHSV(fpreal *values, int nvalues);
UT_API extern void		UT_HSVtoRGB(fpreal *values, int nvalues);

UT_API extern void		UT_RGBtoTMI(fpreal *values, int nvalues);
UT_API extern void		UT_TMItoRGB(fpreal *values, int nvalues);

#define UT_BLACK	(UT_Color::getConstantColor(0))
#define UT_BLUE		(UT_Color::getConstantColor(1))
#define UT_GREEN	(UT_Color::getConstantColor(2))
#define UT_RED		(UT_Color::getConstantColor(3))
#define UT_CYAN		(UT_Color::getConstantColor(4))
#define UT_MAGENTA	(UT_Color::getConstantColor(5))
#define UT_YELLOW	(UT_Color::getConstantColor(6))
#define UT_WHITE	(UT_Color::getConstantColor(7))
#define UT_GREY1	(UT_Color::getConstantColor(8))
#define UT_GREY2	(UT_Color::getConstantColor(9))
#define UT_GREY3	(UT_Color::getConstantColor(10))
#define UT_GREY4	(UT_Color::getConstantColor(11))
#define UT_GREY5	(UT_Color::getConstantColor(12))
#define UT_GREY6	(UT_Color::getConstantColor(13))
#define UT_GREY7	(UT_Color::getConstantColor(14))
#define UT_GREY8	(UT_Color::getConstantColor(15))
#define UT_GREY9	(UT_Color::getConstantColor(16))

/*
 *  UT_Color is the base class for colors in one of the above color spaces
 */

class UT_API UT_Color {
public:
    UT_Color();
    UT_Color(UT_ColorType t);
    UT_Color(UT_ColorType t, fpreal x, fpreal y, fpreal z);
    UT_Color(const UT_Color &c);
    explicit UT_Color(const char *colorname);

    ~UT_Color();

    UT_ColorType getType() const { return type; }
    
    void	 getValue(float &x, float &y, float &z) const
		    { x = u; y = v; z = w;}
    void	 getValue(fpreal64 &x, fpreal64 &y, fpreal64 &z) const
		    { x = fpreal64(u); y = fpreal64(v); z = fpreal64(w);}
    void	 getRGB(float *r, float *g, float *b) const
		 {
		     // Small optimization since we work in RGB most times.
		     if(type == UT_RGB) { *r = u; *g=v; *b = w; }
		     else		getRGBConvert(r,g,b);
		 }

    void	 getHSV(float *h, float *s, float *v) const;
    void	 getHSL(float *h, float *s, float *l) const;
    void	 getXYZ(float *x, float *y, float *z) const;
    void	 getLAB(float *l, float *a, float *b) const;
    void	 getTMI(float *t, float *m, float *i) const;
    void	 getColor(UT_Color &c) const { c = *this; }

    UT_Vector3F	 rgb() const;
    void	 setRGB(UT_Vector3F c);
    void	 setRGB(UT_Vector3D c);
    
    /// luminance() actually returns "luma", an approximation of luminance
    /// derived from the gamma corrected rgb values. So if you have linear
    /// rgb values and call this function, you will get the wrong result.
    fpreal	 luminance() const;

    /// relativeLuminance() returns relative luminance [0,1] assuming the color
    /// is in linear RGB.
    fpreal	 relativeLuminance() const;

    void	 setType(UT_ColorType t);
    void	 setValue(fpreal x, fpreal y, fpreal z) { u = x; v = y; w = z; }
    void	 setRGB(fpreal r, fpreal g, fpreal b);
    void	 setHSV(fpreal h, fpreal s, fpreal v);
    void	 setHSL(fpreal h, fpreal s, fpreal l);
    void	 setXYZ(fpreal x, fpreal y, fpreal z);
    void	 setLAB(fpreal l, fpreal a, fpreal b);
    void	 setLABluminance(fpreal l);
    void	 setTMI(fpreal t, fpreal m, fpreal i);
    void	 setUnique(int64 idx);
    
    void	 setColorByName(const char *colorname);
	
    void	clampToLegal();

    void	ghost(fpreal gfactor = 0.75f);
    UT_Color	copyGhosted( fpreal gfactor = 0.75f ) const;

    void	 operator=(const UT_Color &c);

    int		operator==(const UT_Color &color) const;
    int		operator!=(const UT_Color &c) const { return !(*this == c); }
    bool	operator<(const UT_Color &b) const;

    uint32	hash() const;

    void	brighten(fpreal amount);	// *this *= clr
    void	tintRGB(const UT_Color &clr);	// *this *= clr;
    void	addHSV(const UT_Color &clr);	// *this += clr
    void	addRGB(const UT_Color &clr);	// *this += clr
    void	subRGB(const UT_Color &clr);	// *this -= clr
    
    UT_Color	copyWithGamma( fpreal gamma ) const;
    void	gamma( fpreal gamma );

    // *this = lerp(*this, clr, bias) = (1-bias)*this + bias*clr
    void	blendRGB(const UT_Color &clr, fpreal bias=0.5);

    // Class I/O methods.  Return 0 if successful, -1 if not
    int		 save(std::ostream &os, int binary) const;
    void	 saveMinimalAsText(std::ostream &os) const;
    bool	 load(UT_IStream &is);

    /// @{
    /// Methods to serialize to a JSON stream.  The color is stored as a map:
    /// @code
    ///	    "type" : "color space",
    ///	    "data" : [ x, y, z, ... ]
    /// @endcode
    bool		save(UT_JSONWriter &w) const;
    bool		save(UT_JSONValue &v) const;
    bool		load(UT_JSONParser &p);
    /// @}

    const char	*className() const { return "UT_Color"; }

    /// Computes a ramp colour from the predefined ramp enums.
    static void getRampColor(UT_ColorRamp ramp, fpreal val,
			float *r, float *g, float *b);

    /// Return the name of the ramp colour.
    static const char *getRampColorName(UT_ColorRamp ramp);

    /// Builds a UT_Ramp to correspond to the given internal ramp.
    static UT_Ramp *buildRamp(UT_ColorRamp ramp);

    static UT_Ramp *buildMonoRamp(UT_MonoRamp ramp);

    /// A mapping from integers to colours that tries to ensure
    /// that the chosen colours are well separated and aesthetically
    /// pleasing.
    static void getUniqueColor(int64 idx,
			float *r, float *g, float *b);

    // Returns the standard color at index i.
    static const UT_Color &getConstantColor(int i);

    /// Obtain a random color that is contrasting to the specified other color.
    /// If null is passed for other color, a random color is returned.
    static void getRandomContrastingColor( const UT_Vector3 * contrast_to,
	unsigned color_index, UT_Vector3 &color );

protected:

    // modifies the color with a random luminance
    // (rand_num is a uniformly distributed random number between 0 and 1)
    // NB: - we can only handle colors with at least 1 zero component
    //     - black colors are un-modified
    static void assignRandomLuminance( UT_Vector3 &color );


protected:
    UT_ColorType	 type;
    float		 u;
    float		 v;
    float		 w;
    void		 convertToType(UT_ColorType type);
    void		 convertToHSL();
    void		 convertToHSV();
    void		 convertToRGB();
    void		 convertToXYZ();
    void		 convertToLAB();
    void		 convertToTMI();
    
    void		 getRGBConvert(float *r, float *g, float *b) const;

private:
    //To allow proper and safe saving for a UT_Color in UT_OptionFile, we 
    //override these operators. They simply call save/load respectively.
    friend UT_API std::ostream	&operator<<(std::ostream &os, const UT_Color &color);
};

// Overload for custom formatting of UT_Color with UTformat.
UT_API extern size_t format(char *buffer, size_t buffer_size, const UT_Color &v);


#endif
