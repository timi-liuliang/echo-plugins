/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __POP_AttribDefn__
#define __POP_AttribDefn__

// Float Attributes
#define ATT_ACCEL    	GA_ATTRIB_POINT, "accel", 3
#define ATT_ALPHA	GA_ATTRIB_POINT, "Alpha", 1
#define ATT_ANGVEL    	GA_ATTRIB_POINT, "w", 3
#define ATT_BACKTRACK	GA_ATTRIB_POINT, "backtrack", 4
#define ATT_BORN    	GA_ATTRIB_PRIMITIVE, "born", 1
#define ATT_BOUNCE	GA_ATTRIB_POINT, "bounce", 1
#define ATT_CD		GA_ATTRIB_POINT, "Cd", 3
#define ATT_CHARGE	GA_ATTRIB_POINT, "charge", 1
#define ATT_CLING	GA_ATTRIB_POINT, "cling", 1
#define ATT_COM	        GA_ATTRIB_POINT, "com", 3
#define ATT_DIST	GA_ATTRIB_POINT, "dist", 1
#define ATT_DRAG	GA_ATTRIB_POINT, "drag", 1
#define ATT_FDYNAMIC	GA_ATTRIB_POINT, "fdynamic", 1
#define ATT_FSTATIC	GA_ATTRIB_POINT, "fstatic", 1
#define ATT_HITCD	GA_ATTRIB_POINT, "hitCd", 3
#define ATT_HITFORCE	GA_ATTRIB_POINT, "hitforce", 1
#define ATT_HITN	GA_ATTRIB_POINT, "hitN", 3
#define ATT_HITPOS	GA_ATTRIB_POINT, "hitpos", 3
#define ATT_HITPOSUV	GA_ATTRIB_POINT, "hitposuv", 2
#define ATT_HITTIME	GA_ATTRIB_POINT, "hittime", 1
#define ATT_HITUV	GA_ATTRIB_POINT, "hituv", 3
#define ATT_LIFE    	GA_ATTRIB_POINT, "life", 2
#define ATT_MASS	GA_ATTRIB_POINT, "mass", 1
#define ATT_N		GA_ATTRIB_POINT, "N", 3
#define ATT_NEARESTDIST GA_ATTRIB_POINT, "nearestdist", 1
#define ATT_OAXIS	GA_ATTRIB_POINT, "oaxis", 3
#define ATT_OCENTER	GA_ATTRIB_POINT, "ocenter",3
#define ATT_ORADIUS	GA_ATTRIB_POINT, "oradius", 1
#define ATT_ORIENT	GA_ATTRIB_POINT, "orient", 4
#define ATT_OSPEED	GA_ATTRIB_POINT, "ospeed", 1
#define ATT_PIVOT	GA_ATTRIB_POINT, "pivot", 3
#define ATT_POSUV	GA_ATTRIB_POINT, "posuv", 2
#define ATT_PSCALE	GA_ATTRIB_POINT, "pscale", 1
#define ATT_PSCALE3	GA_ATTRIB_POINT, "scale", 3
#define ATT_PV		GA_ATTRIB_POINT, "pv", 3
#define ATT_REST	GA_ATTRIB_POINT, "rest", 3
#define ATT_ROT		GA_ATTRIB_POINT, "rot", 4
#define ATT_SPEED	GA_ATTRIB_POINT, "speed", 1
#define ATT_SPEEDLIMIT	GA_ATTRIB_POINT, "speedlimit", 2
#define ATT_SPRINGK	GA_ATTRIB_POINT, "springk", 1
#define ATT_SPRITEROT	GA_ATTRIB_POINT, "spriterot", 1
#define ATT_SPRITESCALE	GA_ATTRIB_POINT, "spritescale", 3
#define ATT_SPRITEUV	GA_ATTRIB_POINT, "spriteuv", 4
#define ATT_TENSION	GA_ATTRIB_POINT, "tension", 1
#define ATT_TORQUE	GA_ATTRIB_POINT, "torque", 3
#define ATT_TRANS	GA_ATTRIB_POINT, "trans", 3
#define ATT_UP    	GA_ATTRIB_POINT, "up", 3
#define ATT_UV		GA_ATTRIB_POINT, "uv", 3
#define ATT_V    	GA_ATTRIB_POINT, "v", 3


// Integer Attributes
#define ATT_ATTRACT	GA_ATTRIB_POINT, "attract", 1
#define ATT_FOLLOW	GA_ATTRIB_POINT, "follow", 1
#define ATT_GEN		GA_ATTRIB_POINT, "gen", 1
#define ATT_HITID	GA_ATTRIB_POINT, "hitid", 1
#define ATT_ID		GA_ATTRIB_POINT, "id", 1
#define ATT_NEAREST	GA_ATTRIB_POINT, "nearest", 1
#define ATT_NEXTID	GA_ATTRIB_DETAIL, "nextid", 1
#define ATT_NUMHIT	GA_ATTRIB_POINT, "numhit", 1
#define ATT_NUMPROXIMITY GA_ATTRIB_POINT, "numproximity", 1
#define ATT_ORBIT	GA_ATTRIB_POINT, "orbit", 1
#define ATT_ORIGIN	GA_ATTRIB_POINT, "origin", 1
#define ATT_PARENT	GA_ATTRIB_POINT, "parent", 1
#define ATT_POSPRIM	GA_ATTRIB_POINT, "posprim", 1
#define ATT_PSTATE    	GA_ATTRIB_POINT, "pstate", 1
#define ATT_SOURCE    	GA_ATTRIB_PRIMITIVE, "source", 1

// String Attributes
#define ATT_EVENT	GA_ATTRIB_DETAIL, "event", 0
#define ATT_GENERATOR   GA_ATTRIB_PRIMITIVE, "generator", 1
#define ATT_INSTANCE	GA_ATTRIB_POINT, "instance", 1
//#define ATT_POSOBJ	GA_ATTRIB_POINT, "posobj", 1
//#define ATT_POSSOP	GA_ATTRIB_POINT, "possop", 1
#define ATT_SPRITESHOP	GA_ATTRIB_POINT, "spriteshop", 1
#define ATT_POSPATH	GA_ATTRIB_POINT, "pospath", 1

#endif
