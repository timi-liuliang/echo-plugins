/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	AU_Constants.h ( Audio Library, C++)
 *
 * COMMENTS:
 *
 *	Commonly used definitions
 */


#ifndef __AU_Constants__
#define __AU_Constants__

#define	AU_MAX_CHANNELS	4
#if defined(LINUX) || defined(WIN32)
static const int AU_MAX_RATE = 44100;
#else
static const int AU_MAX_RATE = 48000;
#endif
static const int AU_MIN_RATE = 4000;

#define AU_MIN_SCRUB_FREQUENCY	10
#define AU_MAX_SCRUB_FREQUENCY	100

#endif

