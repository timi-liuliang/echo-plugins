/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	dxf.h
 *
 * COMMENTS:    Private include file for dxf library
 *
 *
 */

#ifndef _PDMS_H_
#define _PDMS_H_

/**************************/
/* Coordinate Definitions */
/**************************/
typedef float COORD;

/*
* Single precision floating point coordinate triple:
*/
typedef struct {
	float x;
	float y;
	float z;
} F3D_COORD;

/*
* Transformation matrix
*/
typedef double TRANSFORM[4][4];

/*
* Vector
*/
typedef struct s_vector{
	float		i,j,k;
} DXFVECTOR;

/*
* Bound Vector
*/
typedef struct {
	F3D_COORD	origin;
	DXFVECTOR direction;
} BOUND_VECTOR;

/*
* RGB Color
*/
typedef struct {
	float	r;	/* red component */
	float	g;	/* green component */
	float	b;	/* blue component */
} COLOR_RGB;

/* MAXIMUMS */
#define MAX_CHARS 80
#define MAX_CHAR 80
#define	MAX_VRT 5000
#define MAX_EDGE 2500
#define	MAX_TVRT MAX_VRT
#define	MAX_ELEM 3000
#define	MAX_NORM MAX_ELEM
#define	MAX_MTL 25
#define	MAX_TEX 25
#define	MAX_GRP 100
#define MAX_VPE	512
#define MAX_CHILDREN 3
#define MAX_LEVELS_HIERARCHY 32
#define MAX_BLOCKS 10
#define MAX_LAYER_CHARS 80

/* POLYGON DEFAULTS */
#define POLY_2D 1
#define SPLINE_2D 2
#define POLY_3D 3
#define SPLINE_3D 4
#define DXF_MESH 5
#define DXF_POLY_MESH 6
#define	DXF_MAX_VRT 15000
#define	DXF_MAX_ELEM 10000

/* ATRIBUTE DEFAULTS */
#define DEF_MTL			-1

/* DEFAULT DIRECTIONS */
#define CW	1
#define CCW	2

typedef enum {
	 TRS, RST, STR, RTS, SRT, TSR } TRANS_PRIORITY;

/* AXIS COMBINATIONS */
typedef enum {
	 XYZ, YZX, ZXY, YXZ, ZYX, XZY } AXIS_COMBINATIONS;

typedef enum {
    DXF_LINE,
    DXF_POINT,
    DXF_POLY,
    DXF_CIRCLE,
    DXF_ARC,
    DXF_POLYARC
} ENT_TYPE;

typedef struct {
    /* STRINGS */
    char	*block;	/* block name 		* CODE 2 */
    char	*layer;	/* layer name 	 	* CODE 8 */
    /* COORDINATES */
    double	x0;	/* x0 coordinate 	* CODE 10 */
    double	x1;	/* x1 coordinate 	* CODE 11 */
    double	x2;	/* x2 coordinate 	* CODE 12 */
    double	x3;	/* x3 coordinate 	* CODE 13 */
    double	y0;	/* y0 coordinate 	* CODE 20 */
    double	y1;	/* y1 coordinate 	* CODE 21 */
    double	y2;	/* y2 coordinate 	* CODE 22 */
    double	y3;	/* y3 coordinate 	* CODE 23 */
    double	z0;	/* z0 coordinate 	* CODE 30 */
    double	z1;	/* z1 coordinate 	* CODE 31 */
    double	z2;	/* z2 coordinate 	* CODE 32 */
    double	z3;	/* z3 coordinate 	* CODE 33 */
    /* ELEVATION AND THICKNESS */
    double	elevation;/* z elevation 	* CODE 38 */
    double	extrusion;/* z extrusion 	* CODE 39 */
    /* FLOATING POINT VALUES */
    double	radius;	/* radius or st_width	* CODE 40 */
    double	end_width; /* ending width	* CODE 41 */
    double	bulge;  /* bulge factor 	* CODE 42 */
    double	z_scale; /* z-scale factor 	* CODE 43 */
    double	col_sep; /* column spacing  	* CODE 44 */
    double	row_sep; /* row spacing     	* CODE 45 */
    double	repeat; /* repeated value	* CODE 49 */
    double	ang0;	/* starting angle	* CODE 50 */
    double	ang1;	/* ending angle 	* CODE 51 */
    /* INTEGER VALUES */
    int	color;		/* color 	 	* CODE 62 */
    int	verts_follow; 	/*vertices follow	* CODE 66 */
    int	flags;		/* vertex or poly flag	* CODE 70 */
    int	M_polymesh; 	/* mesh M vert count	* CODE 71 */
    int	N_polymesh; 	/* mesh N vert count	* CODE 72 */
    int	M_density; 	/* smooth surface M	* CODE 73 */
		       	/* density */
    int	N_density; 	/* smooth surface N	* CODE 74 */
		       	/* density */
    int	smooth_type; 	/* smooth type 		* CODE 75 */

    /* ECS VECTOR (Extrusion Direction) */
    double	ECS_X;	/* X component 		* CODE 210 */
    double	ECS_Y;	/* Y component		* CODE 220 */
    double	ECS_Z;	/* Z component		* CODE 230 */
} ENT_VALS;

/* Enumerated Data Type for element type */
typedef enum {
    FO,			/* Face Outline */
    F,			/* Face */
    LINE,		/* Line */
    DXFPOINT,		/* Point */
    BSPLINE,		/* B-Spline */
    MESH,		/* Polygonal mesh */
    METHOD_DATA		/* Data to be used in a method */
} ETYPE;

/* Enumerated Data Type for method type */
typedef enum {
    MESH_METH,		/* Mesh */
    EXTRUSION,		/* Extrusion */
    REVOLUTION		/* Surface of Revolution */
} MTYPE;

/* Enumerated Data Type for texture map type */
typedef enum {
    AMBIENT_MAP,
    DIFFUSE_MAP,
    SPECULAR_MAP,
    TRANSPARENCY_MAP,
    SP_COEFFICIENT_MAP,
    BUMP_MAP
} TEX_TYPE;


		/***********************************/
		/********* GLOBAL POSITION  ********/
		/***********************************/
typedef struct s_global_position{
    TRANSFORM		transform;	/* transformation matrix */
    F3D_COORD		translate;	/* translation values */
    F3D_COORD		rotate;		/* rotational values */
    F3D_COORD		scale;		/* scale values */
    F3D_COORD		pivot;		/* pivot point (need not be the centroid)*/
    COORD		quat[4];	/* quaternion axes */
    TRANS_PRIORITY		tp;		/* transformation priority */
    AXIS_COMBINATIONS	rp;		/* rotational priority */
} GLOBAL_POSITION;

		/********************/
		/*** LINKED LISTS ***/
		/********************/
typedef struct s_linked_list *PTR_LL;

/* single linked list definition */
typedef struct s_linked_list {
    PTR_LL	next;	/* pointer to  the next "record" in a linked list */
    int		value;	/* value of record, usually an index into another
		         list or a type index if the ptr is used */
    unsigned long *data;	/* pointer to more data for this record */
} LINKED_LIST;


		/*******************/
		/*** TABLE SIZES ***/
		/*******************/
/* Sizes Data Structure */
typedef struct obj_sizes {
    /* current values */
    unsigned int	num_verts;	/* # of vertices in verts table */
    unsigned int	num_edges;	/* # of edges in edges table */
    unsigned int	num_tverts;	/* # of texture verts in tverts table*/
    unsigned int	num_norms;	/* # of normals in normals table */
    unsigned int	num_elems;	/* # of elements in elems table */
    unsigned int	num_methods;	/* # of methods in methods table */
    unsigned int	num_materials;	/* # materials in materials table */
    unsigned int	num_textures;	/* # textures in textures table */
    unsigned int 	num_udef;	/* # of user defined "values" */
    unsigned int	num_group_names; /* # of group names defined */

    /* maximum values */
    unsigned int	max_verts;	/* max # of vertices in verts table */
    unsigned int	max_edges;	/* max # of edges in edges table */
    unsigned int	max_norms;	/* max # of normals in normals table */
    unsigned int	max_tverts;	/* max # texture verts in tverts table*/
    unsigned int	max_elems;	/* max # of elements in elems table */
    unsigned int	max_methods;	/* max # of methods in methods table */
    unsigned int	max_materials;	/* max materials in materials table */
    unsigned int	max_textures;	/* max textures in textures table */
    unsigned int 	max_udef;	/* max # of user defined values */
    unsigned int	max_group_names; /* max # of possible group names */
} OBJ_SIZES;

		/**************/
		/* VERT_FLAGS */
		/**************/
/* Vertice Flags structure */
/* NOTE: 1 indicates TRUE , 0 indicates FALSE */
typedef union vert_flags {
    struct {
	unsigned  bit_1		: 1;	/* bit 1 - reserved */
	unsigned  bit_2		: 1;	/* bit 2 - reserved */
	unsigned  bit_3		: 1;	/* bit 3 - reserved */
	unsigned  bit_4		: 1;	/* bit 4 - reserved */
	unsigned  bit_5		: 1;	/* bit 5 - reserved */
	unsigned  bit_6		: 1;	/* bit 6 - reserved */
	unsigned  tagged	: 1;	/* marked for some operation */
	unsigned  bit_8		: 1;	/* bit 8 - user defined */
    } bit;

    unsigned  all;
} VERT_FLAGS;

		/***********/
		/* VERTICE */
		/***********/
/* Vertex Data Structure */
typedef struct s_vertice {
    float	x;		/* x coordinate */
    float	y;		/* y coordinate */
    float 	z;		/* z coordinate */
    int 	material;	/* materials index */
    int 	group;		/* group index */
    VERT_FLAGS	flags;		/* bit coded vertice flags */
} VERTICE;


		/**************/
		/* EDGE_FLAGS */
		/**************/
/* Edge Flags structure */
/* NOTE: 1 indicates TRUE , 0 indicates FALSE */
typedef union edge_flags {
    struct {
	unsigned  c_interp	: 1;	/* color interp */
	unsigned  d_interp	: 1;	/* dissolve interp */
	unsigned  b_interp	: 1;	/* bevel interp */
	unsigned  smooth	: 1;	/* edge should be "smoothed" */
	unsigned  bit_5		: 1;	/* bit 5 - reserved */
	unsigned  bit_6		: 1;	/* bit 6 - reserved */
	unsigned  tagged	: 1;	/* marked for some operation */
	unsigned  bit_8		: 1;	/* bit 8 - user defined */
    } bit;

    unsigned all;
} EDGE_FLAGS;

		/********/
		/* EDGE */
		/********/
typedef struct s_edge {
    int		v1;	/* index of vertice 1 of edge */
    int		v2;	/* index of vertice 2 of edge */
    int 	nccw1;	/* next counter-clockwise edge from vertice 1 */
    int 	ncw2;	/* next clockwise edge from vertice 2 */
    LINKED_LIST	*elems;	/* list of all elements sharing this edge */
    EDGE_FLAGS	flags;	/* bit coded edge flags */
} EDGE;

		/**********/
		/* NORMAL */
		/**********/
/* Normal Vector Data Structure */
typedef struct s_normal {
    float	i;		/* i coordinate */
    float	j;		/* j coordinate */
    float 	k;		/* k coordinate */
} NORMAL;

		/********************/
		/* TEXTURE VERTICES */
		/********************/
/* Texture Vertex Data Structure */
typedef struct s_texture_vertice {
    float	u;		/* u coordinate */
    float	v;		/* v coordinate */
    float 	w;		/* w coordinate */
} TEXTURE_VERTICE;

		/***************/
		/* ELEM_RFLAGS */
		/***************/
/* Element Render Flags structure */
/* NOTE: 1 indicates TRUE , 0 indicates FALSE */
typedef union u_elem_rflags {
    struct {
       unsigned  c_interp	: 1;	/* color interp */
       unsigned  d_interp	: 1;	/* dissolve interp */
       unsigned  b_interp	: 1;	/* bevel interp */
       unsigned  bit_4		: 1;	/* bit 4 - reserved */
       unsigned  bit_5		: 1;	/* bit 5 - reserved */
       unsigned  bit_6		: 1;	/* bit 6 - reserved */
       unsigned  tagged		: 1;	/* marked for some operation */
       unsigned  bit_8		: 1;	/* bit 8 - user defined */
    } bit;

    unsigned all;
} ELEM_RFLAGS;

		/**************/
		/* ELEM_FLAGS */
		/**************/
/* Element Flags structure */
/* NOTE: 1 indicates TRUE , 0 indicates FALSE */
typedef union elem_flags {
    struct {				/*  -- MEANING IF TRUE -- */
	unsigned  cw_visible	: 1;	/* clockwise side visible */
	unsigned  ccw_visible	: 1;	/* counter-clockwise side visible */
	unsigned  concave	: 1;	/* convex, false if concave */
	unsigned  hole		: 1;	/* element denotes a "hole" */
	unsigned  edges		: 1;	/* the vlist represents edges */
	unsigned  vert_normals	: 1;	/* normals defined for each vert */
	unsigned  tagged	: 1;	/* marked for some operation */
	unsigned  bit_8		: 1;	/* bit 8 - user defined */
    } bit;

    unsigned all;
} ELEM_FLAGS;

		/***************************************/
		/*************** ELEMENT ***************/
		/***************************************/
/* Element Data Structure */
typedef struct element {
    ETYPE 	type;		/* Element type */
    int 	num_verts;	/* number of vertices */
    int 	*vlist;		/* vertex list (optionally an edge list)*/
    int 	*tvlist;	/* texture vertice list */
    int 	*nlist;		/* normals list */
    LINKED_LIST	*link;		/* user defined linked list */
    int 	material;	/* materials index */
    int 	texture;	/* texture index */
    int 	smooth;		/* smooth group index */
    int 	group;		/* group */
    char 	*text;		/* Associated text */
    ELEM_FLAGS 	flags;		/* element flags */
    ELEM_RFLAGS	rflags;		/* element render flags */
} ELEMENT;


		/**************************************/
		/*************** METHOD ***************/
		/**************************************/
/* Element Data Structure */
typedef struct s_method {
    MTYPE 		type;		/* Element type */
    unsigned long	*data;		/* method data */
    LINKED_LIST 	*link;		/* user defined linked list */
    int			material;	/* materials index */
    int 		texture;	/* texture index */
    int 		smooth;		/* smooth group index */
    int 		group;		/* group */
    char 		*text;		/* Associated text */
} METHOD;

		/************************************/
		/*********** RENDER_FLAGS ***********/
		/************************************/
/* Render Flags structure */
/* NOTE: 1 indicates TRUE , 0 indicates FALSE */
typedef union render_flags {
    struct {
	unsigned  ambient	: 1;	/* use ambient component */
	unsigned  diffuse	: 1;	/* use diffuse component */
	unsigned  specular	: 1;	/* use specular component */
	unsigned  reflective	: 1;	/* capable of reflecting objects */
	unsigned  bit_4		: 1;	/* bit 4 - reserved */
	unsigned  bit_5		: 1;	/* bit 5 - reserved */
	unsigned  bit_6		: 1;	/* bit 6 - reserved */
	unsigned  bit_7		: 1;	/* bit 7 - user defined */
    } bit;

    unsigned all;
} RENDER_FLAGS;

		/**********************************/
		/************* MATERIAL ***********/
		/**********************************/
/* Material Data Structure */
/* note: all values are stored 0.0 to 1.0 (min to max) */
typedef struct s_material {
    char		*mtllib;		/* Material library */
    char 		*name;			/* Material Name */
    COLOR_RGB		ambient;		/* ambient color */
    COLOR_RGB		diffuse;		/* diffuse color */
    COLOR_RGB		specular;		/* specular color */
    COLOR_RGB		transmission;		/* transmission filter */
    float		spec_coefficient;	/* specular coefficient */
    float		transparency;		/* transparency */
    float		refraction;		/* index of refraction */
    LINKED_LIST		*link;			/* linked list for use with unknown mtl types */
    RENDER_FLAGS	flags;			/* rendering flags for material */
} S_MATERIAL;

		/***********************************/
		/************* TEX_MAP *************/
		/***********************************/
/* Texture Map Data structure */
typedef struct tex_map {
    TEX_TYPE		type;		/* type of texture map */
    char 		*file;		/* filename of texture map */
    float		min;		/* minimum value for texture map */
    float		max;		/* maximum value for texture map */
    F3D_COORD		origin;		/* origin of map */
    F3D_COORD		scale;		/* scale of map */
    F3D_COORD		*window;	/* window into map */
    unsigned char	flags;		/* texture map flags */
} TEX_MAP;


		/***********************************/
		/************* TEXTURE *************/
		/***********************************/
/* Texture Data Structure */
typedef struct texture {
    char 	*maplib;	/* texture map library name */
    char	*name;		/* name of texture */
    int		num_maps;	/* number of texture maps for this texture */
    TEX_MAP	*map_list;	/* array of texture map descriptions */
} DXFTEXTURE;

		/***********************************/
		/************* PHYSICS *************/
		/***********************************/
typedef struct t_physics {
    F3D_COORD	min;		/* minimum extents */
    F3D_COORD	max;		/* maximum extents */
    F3D_COORD	size;		/* size (max - min) */
    F3D_COORD	centroid;	/* centroid of object */
    float	surface_area;
    float	mass;
} PHYSICS;

		/********************************/
		/************ OBJECT ************/
		/********************************/
typedef struct object {

    /* tables */
    VERTICE 		*verts;		/* Pointer to vertices table */
    EDGE		*edges;		/* Pointer to edges table */
    NORMAL		*norms;		/* Pointer to normals table */
    TEXTURE_VERTICE	*tverts;	/* Pointer to texture vertices table*/
    ELEMENT		*elems;		/* Pointer to element table */
    METHOD		*methods;	/* Pointer to method table */
    S_MATERIAL		*materials;	/* Pointer to material table (0 - default)*/
    DXFTEXTURE		*textures;	/* Pointer to texture table (0 - default)*/
    char		**group_names;	/* Pointer to group name table */
    unsigned long	*udef;		/* User defined table */

    /* local object data */
    OBJ_SIZES		sizes;		/* current and maximum table sizes */
    GLOBAL_POSITION	global;		/* global transformation of object */
    PHYSICS		physics;		/* various physical data */
    char 		obj_name[MAX_CHARS];	/* Name of object */
    char		*text;			/* Associated text string */
} OBJECT;
/* OBJECT NOTE: the materials, textures, and group_names tables may be NULL
	even though valid indices are given in elements. This will most often be
	the case when no information such as an ascii group name or actual
	material values are available or when only an index is relevant.  Output
	routines should check for NULL tables and manufacture consistent default
	data when needed.
*/


		/***************************/
		/*********** BODY **********/
		/***************************/
typedef struct s_body{
    OBJECT 		*object;	/* Pointer to object */
    GLOBAL_POSITION	global;		/* global position data for this body */
    unsigned int	num_children;	/* Number of children for this body */
    unsigned int	max_children;	/* Current max size of child array */
    struct s_body	**child;	/* array of pointers to child bodies */
    struct s_body	*parent;	/* pointer to parent (NULL for root)*/
} BODY;


/* transformation stack */
typedef struct s_mstack{
    unsigned int	num_matrices;
    unsigned int	current;
    TRANSFORM		*stack;
} MSTACK;

typedef struct layer_ent{
    char		name[MAX_LAYER_CHARS];
    int			color;
} LAYER_TBL_ENTRY;


/********** Functions **********/
extern char **DxfLayers(BODY *body, int *num_layers);
extern void FreePdms(void);

#endif
