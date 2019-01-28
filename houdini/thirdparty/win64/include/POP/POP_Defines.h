/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __POP_Defines_H__
#define __POP_Defines_H__

#define POP_TORQUECORRECTION		0.166666F

#define POP_ORIGIN_INDEX		0x0001
#define POP_ORIGIN_GEO			0x0002
#define POP_ORIGIN_PARENT		0x0004

#define POP_COLLIDE_DIE			0
#define POP_COLLIDE_BOUNCE		1
#define POP_COLLIDE_STOP		2
#define POP_COLLIDE_STICK		3
#define POP_COLLIDE_SLIDE		4
#define POP_COLLIDE_CONTINUE		5

#define POP_MINIMPULSE_BOUNCE		0
#define POP_MINIMPULSE_DIE		1
#define POP_MINIMPULSE_STOP		2
#define POP_MINIMPULSE_STICK		3
#define POP_MINIMPULSE_SLIDE		4

#define FORCE_FORCE			0
#define FORCE_FAN			1
#define FORCE_CONSTRAINT		2

#endif

